//#pragma pack(1)
#include <stdint.h>
#include <memory>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/endian/conversion.hpp>

#define IS_BIG_ENDIAN 0



namespace rtcp
{


	class Rtcp
	{

	public:
		Rtcp();
		//template<typename rtcpPacket>
		//rtcp(rtcpPacket packet);
		~Rtcp();
		enum rtcpPayloadTypes {

			SenderReport = 200,
			ReceiverReport = 201,
			SourceDescription = 202,
			Goodbye = 203,
			AppDef = 204
		};

		enum rtpSdesTypes {
			END = 0,	// end of SDES list
			CNAME = 1,	// canonical name
			NAME = 2,	// user name
			EMAIL = 3,	// user's electronic mail address
			PHONE = 4,	// user's phone adress
			LOC = 5,	// user's geographic location
			TOOL = 6,	// name of application/tool
			NOTE = 7,	// notice about source
			PRIV = 8,	// private extension
			H323_CADDR = 9,	// H.323 callable address
			APSI = 10,	// Application Specific Identifier
			RGRP = 11	// Reporting Group Identifier
		};

		struct sdesItem {
			uint8_t rtpSdesType;
			uint8_t itemLength;
			std::vector<char> item;
			sdesItem(int sdesType, std::string data);
			sdesItem(int sdesType, std::vector<char> data);
		};

		struct senderInfo {
			uint32_t ntpSecondsTimestamp;
			uint32_t ntpFractionTimestamp;
			uint32_t rtpTimestamp;
			uint32_t packetCount;
			uint32_t octetCount;
			senderInfo();
		};

		struct reportBlock {
			uint32_t ssrc;
			uint8_t fractionLost;
			unsigned int packetsLost : 24;
			uint32_t highestSeqNum;
			uint32_t interarrivalJitter;
			uint32_t lastSR;
			uint32_t delaySinceLSR;
			reportBlock();
		};



		struct rtcpHeader {
			uint8_t firstOctet; // 0-1 version, 2 padding, 3-7 RECEPTION Report count
			uint8_t payloadType;
			uint16_t length;
		};


		void setVersion(int version = 2);
		int getVersion() const;

		void setPadding(bool padding = false);
		bool getPadding() const;

		void setReportCount(int rc);
		int getReportCount();

		void setPayload(rtcpPayloadTypes pc);
		rtcpPayloadTypes getPayload() const { return (rtcpPayloadTypes)header.payloadType; }

		void setHeaderLength(uint16_t length);
		uint16_t getHeaderLength() const { return header.length; }

		void setHeaderSSRC(uint32_t ssrc);
		uint32_t getsHeaderSSRC() const { return SSRC; }

		bool addReportBlock(reportBlock block);
		std::vector<reportBlock> getReportBlocks() { return reports; }
		void resetReportBlock();

		bool addSdesItem(sdesItem item);
		std::vector<sdesItem> getSdesItems() { return items; }
		void resetSdesItems();

		bool addLeaver(uint32_t ssrc);
		std::vector<uint32_t> getLeavers() { return otherLeavers; }
		void resetLeavers();
		void addGoodbyeText(std::string);



#if IS_BIG_ENDIAN
		std::shared_ptr<std::vector<uint8_t>> createRtcpPacket() {
			try {
				std::shared_ptr<std::vector<uint8_t>> outPacket(new std::vector <uint8_t>);
				uint8_t* ptr;
				ptr = (uint8_t*)(&header);

				(*outPacket).insert((*outPacket).begin(), ptr, ptr + sizeof(header));
				ptr = (uint8_t*)(&SSRC);
				(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(SSRC));

				switch (getPayload())
				{
				case SenderReport:
					ptr = (uint8_t*)(&senderReport);
					(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(senderReport));
				case ReceiverReport:
					setReportCount(reportCount);
					(*outPacket)[0] = header.firstOctet;

					//for each (reportBlock rp in reports)
					BOOST_FOREACH(reportBlock rp, reports)
					{
						ptr = (uint8_t*)(&rp);
						(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(rp));
					}
					break;

				case SourceDescription:          
					setReportCount(sdesCount);
					(*outPacket)[0] = header.firstOctet;
					BOOST_FOREACH(sdesItem si , items) 
					{
						(*outPacket).push_back(si.rtpSdesType);
						(*outPacket).push_back(si.itemLength);
						(*outPacket).insert((*outPacket).end(), si.item.begin(), si.item.end());	
					}
					break;

				case Goodbye:
					setReportCount(leaverCount);
					(*outPacket)[0] = header.firstOctet;
					BOOST_FOREACH(uint32_t l , otherLeavers) {
						ptr = (uint8_t*)(&l);
						(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(l));
					}
					if (optGoodbyeText) {
						ptr = &goodbyeTextLength;
						(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(goodbyeTextLength));
						(*outPacket).insert((*outPacket).end(), goodbyeText.begin(), goodbyeText.end());
					}
					break;
				case AppDef:
					break;
				default:
					break;

				}
				return outPacket;
			}
			catch (std::exception& e) {
				std::cout << "ERROR: creating RTCP" << std::endl;
				std::cerr <<"Error rtcp.h 1 : " <<e.what() << std::endl;
				
			}
			return nullptr;
		}


		bool setRtcpPacket(std::vector<uint8_t> inPacket) {
			try {
				int position = 0;
				senderInfo* ptr;
				reportBlock rb;
				std::vector<char> v;
				header = *(rtcpHeader*)inPacket.data();
				position += sizeof(header);
				if (!(validateHeader())) {

					return false;
				}
				SSRC = *(uint32_t*)(inPacket.data() + position);

				position += sizeof(SSRC);
				switch (getPayload()) {
				case SenderReport:
					ptr = (senderInfo*)(inPacket.data() + position);
					senderReport = *ptr;
					position += sizeof(senderReport);

				case ReceiverReport:
					resetReportBlock();
					for (int i = 0; i < getReportCount(); i++) {
						rb = *(reportBlock*)(inPacket.data() + position);
						position += sizeof(rb);
						addReportBlock(rb);
					}
					break;
				case SourceDescription:
					resetSdesItems();
					for (int i = 0; i < getReportCount(); i++) {
						v.clear();
						int t = inPacket[position];
						position++;
						int l = inPacket[position];
						position++;				
						v.insert(v.begin(), inPacket.begin() + position, inPacket.begin()+position+l);
						position += l;
						sdesItem si(t, v);
						addSdesItem(si);
					}
					break;
				case Goodbye:
					resetLeavers();
					for (int i = 0; i < getReportCount(); i++) {
						uint32_t* ptr = (uint32_t*)(inPacket.data() + position);
						otherLeavers.push_back(*ptr);
						position += 4;
					}
					break;
				case AppDef:
					break;
				default:
					return false;
					break;
				}
				return true;
			}
			catch (std::exception& e) {
				std::cout << "ERROR: setting RTCP" << std::endl;
				std::cerr << "Error rtcp.h 2: " << e.what() << std::endl;
				return false;
			}
		}
#else
std::shared_ptr<std::vector<uint8_t>> createRtcpPacket() {
	try {
		uint16_t temp16;
		uint32_t temp32;
		uint8_t* ptr;

		reportBlock tempRP;
		senderInfo tempSR;

		std::shared_ptr<std::vector<uint8_t>> outPacket(new std::vector <uint8_t>);
		(*outPacket).push_back(header.firstOctet);
		(*outPacket).push_back(header.payloadType);
		temp16 = boost::endian::endian_reverse(header.length);
		ptr = (uint8_t*)&temp16;
		(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(temp16));
		temp32 = boost::endian::endian_reverse(SSRC);
		ptr = (uint8_t*)(&temp32);
		(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(temp32));
		switch (getPayload())
		{
		case SenderReport:

			tempSR.ntpFractionTimestamp = boost::endian::endian_reverse(senderReport.ntpFractionTimestamp);
			tempSR.ntpSecondsTimestamp = boost::endian::endian_reverse(senderReport.ntpSecondsTimestamp);
			tempSR.octetCount = boost::endian::endian_reverse(senderReport.octetCount);
			tempSR.packetCount = boost::endian::endian_reverse(senderReport.packetCount);
			tempSR.rtpTimestamp = boost::endian::endian_reverse(senderReport.rtpTimestamp);
			ptr = (uint8_t*)(&tempSR);
			(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(senderReport));
		case ReceiverReport:
			setReportCount(reportCount);
			(*outPacket)[0] = header.firstOctet;
			
			//for each (reportBlock rp in reports)
			BOOST_FOREACH(reportBlock rp, reports)
			{
				tempRP.delaySinceLSR = boost::endian::endian_reverse(rp.delaySinceLSR);
				tempRP.fractionLost = rp.fractionLost;
				tempRP.highestSeqNum = boost::endian::endian_reverse(rp.highestSeqNum);
				tempRP.interarrivalJitter = boost::endian::endian_reverse(rp.interarrivalJitter);
				tempRP.lastSR = boost::endian::endian_reverse(rp.lastSR);
				tempRP.ssrc = boost::endian::endian_reverse(rp.ssrc);
				temp32 = boost::endian::endian_reverse(rp.packetsLost) << 8;
				tempRP.packetsLost = temp32;
				ptr = (uint8_t*)&tempRP;
				(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(tempRP));
			}
			break;
		case SourceDescription:
			setReportCount(sdesCount);
			(*outPacket)[0] = header.firstOctet;
			BOOST_FOREACH(sdesItem si, items)
			{
				(*outPacket).push_back(si.rtpSdesType);
				(*outPacket).push_back(si.itemLength);
				(*outPacket).insert((*outPacket).end(), si.item.begin(), si.item.end());
			}
			break;

		case Goodbye:
			setReportCount(leaverCount);
			(*outPacket)[0] = header.firstOctet;
			BOOST_FOREACH(uint32_t l, otherLeavers) {
				temp32 = boost::endian::endian_reverse(l);
				ptr = (uint8_t*)(&temp32);
				(*outPacket).insert((*outPacket).end(), ptr, ptr + sizeof(l));
			}
			if (optGoodbyeText) {
				(*outPacket).push_back(goodbyeTextLength);
				(*outPacket).insert((*outPacket).end(), goodbyeText.begin(), goodbyeText.end());
			}
			break;
		case AppDef:
			break;
		default:
			break;

		}
		return outPacket;
	}
	catch (std::exception& e) {
		std::cout << "ERROR: creating RTCP" << std::endl;
		std::cerr << "Error rtcp.h 1 : " << e.what() << std::endl;

	}
	return nullptr;
}


bool setRtcpPacket(std::vector<uint8_t> inPacket) {
	try {
		int position = 0;
		senderInfo* ptr;
		reportBlock rb;
		std::vector<char> v;
		uint16_t temp16;
		uint32_t temp32;

		header.firstOctet = *inPacket.data();
		position++;
		header.payloadType = *(inPacket.data() + position);
		position++;
		temp16 = *(uint16_t*)(inPacket.data() + position);
		header.length = boost::endian::endian_reverse(temp16);
		position += 2;
		
		if (!(validateHeader())) {
			return false;
		}
		SSRC = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
		position += sizeof(SSRC);

		switch (getPayload()) {
		case SenderReport:
			senderReport.ntpSecondsTimestamp = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
			position += sizeof(senderReport.ntpSecondsTimestamp);
			senderReport.ntpFractionTimestamp = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
			position += sizeof(senderReport.ntpFractionTimestamp);
			senderReport.rtpTimestamp = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
			position += sizeof(senderReport.rtpTimestamp);
			senderReport.packetCount = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
			position += sizeof(senderReport.packetCount);
			senderReport.octetCount = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
			position += sizeof(senderReport.octetCount);

		case ReceiverReport:
			resetReportBlock();
			for (int i = 0; i < getReportCount(); i++) {
				rb.ssrc = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
				position += sizeof(rb.ssrc);
				rb.fractionLost = *(inPacket.data() + position);
				position++;
				temp32 = boost::endian::endian_reverse((*(uint32_t*)(inPacket.data() + position)<<8));
				rb.packetsLost = temp32;
				position += 3;
				rb.highestSeqNum = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
				position += 4;
				rb.interarrivalJitter = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
				position += 4;
				rb.lastSR = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
				position += 4;
				rb.delaySinceLSR = boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position));
				position += 4;
				addReportBlock(rb);
			}
			break;
		case SourceDescription:
			resetSdesItems();
			for (int i = 0; i < getReportCount(); i++) {
				v.clear();
				int t = inPacket[position];
				position++;
				int l = inPacket[position];
				position++;
				v.insert(v.begin(), inPacket.begin() + position, inPacket.begin() + position + l);
				position += l;
				sdesItem si(t, v);
				addSdesItem(si);
			}
			break;
		case Goodbye:
			resetLeavers();
			for (int i = 0; i < getReportCount(); i++) {
				otherLeavers.push_back(boost::endian::endian_reverse(*(uint32_t*)(inPacket.data() + position)));
				position += 4;
			}
			if (inPacket.begin() + position != inPacket.end()) {
				goodbyeTextLength = (inPacket[position]);
				goodbyeText.clear();
				goodbyeText.insert(goodbyeText.begin(), inPacket.data() + position, inPacket.data() + position + goodbyeTextLength);
			}
			
			break;
		case AppDef:
			break;
		default:
			return false;
			break;
		}
		return true;
	}
	catch (std::exception& e) {
		std::cout << "ERROR: setting RTCP" << std::endl;
		std::cerr << "Error rtcp.h 2: " << e.what() << std::endl;
		return false;
	}
}




#endif


		bool validateHeader();


	
		rtcpHeader header;
		std::vector<reportBlock> reports;
		std::vector<sdesItem> items;
		uint32_t SSRC;
		senderInfo senderReport;
		int sdesCount = 0;
		int reportCount = 0;
		int leaverCount = 0;


		bool optGoodbyeText;
		std::vector<uint32_t> otherLeavers;
		uint8_t goodbyeTextLength;
		std::string goodbyeText;
		
	};



}