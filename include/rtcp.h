

#include <stdint.h>
#include <memory>
#include <vector>
#include <iostream>





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

		uint16_t calculateHeaderLength();


		//template <typename rtcpPacket>
		//rtcpPacket createRtcpPacket()
		std::shared_ptr<uint8_t> createRtcpPacket()
		{
			uint8_t* packet;
			header.length = calculateHeaderLength();
			int packLength = 4 * (header.length + 1);
			std::shared_ptr<uint8_t> packet_pointer(new uint8_t[packLength]);
			packet = (uint8_t*)packet_pointer.get();
			rtcpPayloadTypes type = getPayload();	
			switch (getPayload())
			{
			case SenderReport:
				memcpy(packet, &header, 4);
				packet += 4;
				memcpy(packet, &SSRC, 4);
				packet += 4;
				memcpy(packet, &senderReport, 20);
				packet += 20;
				break;
			case ReceiverReport:
				setReportCount(reportCount);
				memcpy(packet, &header, 4);
				packet += 4;
				memcpy(packet, &SSRC, 4);
				packet += 4;
				for (int i = 0; i < getReportCount(); i++)
				{
					memcpy(packet, &getReportBlocks()[i], 24);
					packet += 24;
				}
				break;
			case SourceDescription:
				setReportCount(sdesCount);
				memcpy(packet, &header, 4);
				packet += 4;
				memcpy(packet, &SSRC, 4);
				packet += 4;
				for(sdesItem i:getSdesItems()){
					memcpy(packet, &i.rtpSdesType, 1);
					packet++;
					memcpy(packet, &i.itemLength, 1);
					packet++;
					memcpy(packet, i.item.data(), sizeof(i.item)*i.item.size());
					packet += i.item.size();
				}
				break;
			case Goodbye:
				setReportCount(leaverCount);
				memcpy(packet, &header, 4);
				packet+= 4;
				memcpy(packet, &SSRC, 4);
				packet += 4;
				memcpy(packet, otherLeavers.data(), otherLeavers.size() * 4);
				packet += otherLeavers.size() * 4;
				if (optGoodbyeText) {
					memcpy(packet, &goodbyeTextLength, sizeof(goodbyeTextLength));
					packet += sizeof(goodbyeTextLength);
					memcpy(packet, goodbyeText.data(), goodbyeText.size());
					packet += goodbyeText.size();
				}
				break;
			case AppDef:
				packet = nullptr;
				break;
			default:
				packet = nullptr;
				break;
			}
				return packet_pointer;
		}

		//template<typename rtcpPacket>
		//bool setRtcpPacket(rtcpPacket inpacket) {
		bool setRtcpPacket(uint8_t* inpacket) {
			header = *(rtcpHeader*)inpacket;
			inpacket += 4;
			if (!(validateHeader())) {
				return false;
			}
			SSRC = *(uint32_t*)inpacket;
			inpacket += 4;
			switch (getPayload())
			{
			case SenderReport:
				senderReport = *(senderInfo*)inpacket;
				inpacket += 20;
				break;
			case ReceiverReport:
				resetReportBlock();
				for (int i = 0; i < getReportCount(); i++)
				{
					reportBlock report = *(reportBlock*)inpacket;
					reports.push_back(report);
					inpacket += 24;
				}
				break;
			case SourceDescription:
				resetSdesItems();
				for (int i = 0; i < getReportCount(); i++)
				{
					int sdesType = *(uint8_t*)inpacket;
					inpacket++;
					int sdesLength = *(uint8_t*)inpacket;
					inpacket++;
					std::vector<char> sdesData(inpacket, inpacket + sdesLength);
					inpacket += sdesLength;
					sdesItem item(sdesType, sdesData);
					items.push_back(item);
				}
				break;
			case Goodbye:
				resetLeavers();
				for (int i = 0; i < getReportCount(); i++)
				{
					uint32_t leaver = *(uint32_t*)inpacket;
					addLeaver(leaver);
					inpacket += 4;
				}
				if ((getReportCount() + 1) < getHeaderLength())
				{
					optGoodbyeText = true;
					goodbyeTextLength = *(uint8_t*)inpacket;
					inpacket++;
					goodbyeText.clear();
					for (int i = 0; i < getHeaderLength() - getReportCount() + 1; i++)
					{
						goodbyeText.push_back(*(char*)(inpacket));
						inpacket++;
					}
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
		bool validateHeader();


	
		rtcpHeader header;
		std::vector<reportBlock> reports;
		std::vector<sdesItem> items;
		uint32_t SSRC;
		senderInfo senderReport;
		int sdesCount;
		int reportCount;
		int leaverCount;


		bool optGoodbyeText;
		std::vector<uint32_t> otherLeavers;
		uint8_t goodbyeTextLength;
		std::string goodbyeText;

	};



}