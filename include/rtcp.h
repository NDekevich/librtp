

#include <stdint.h>
#include <memory>
#include <vector>





class rtcp
{
public:
	rtcp();
	template<typename rtcpPacket>
	rtcp(rtcpPacket packet);
	~rtcp();
	class Rtcp
	{

	public:
		enum rtcpPayloadTypes {

			senderReport = 200,
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
			rtpSdesTypes rtpSdesType : 8;
			uint8_t itemLength;
			char* item;
			sdesItem(rtpSdesTypes type, uint8_t length, char* item);
		};

		struct senderInfo {
			uint32_t ntpSecondsTimestamp;
			uint32_t ntpFractionTimestamp;
			uint32_t rtpTimestamp;
			uint32_t packetCount;
			uint32_t octetCount;
			senderInfo(uint32_t ntpSecondsTimestamp, uint32_t ntpFractionTimestamp, uint32_t rtpTimestamp, uint32_t packetCount, uint32_t octetCount);
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
			rtcpPayloadTypes payloadType : 8;
			uint16_t length;
		};

		void setVersion(int version = 2);
		int getVersion() const;

		void setPadding(bool padding = false);
		bool getPadding() const;

		void setReportCount(int rc);
		int getReportCount();

		void setPayload(rtcpPayloadTypes pc);
		rtcpPayloadTypes getPayload() const { return header.payloadType; }

		void setHeaderLength(uint16_t length);
		uint16_t getHeaderLength() const { return header.length; }

		void setHeaderSSRC(uint32_t ssrc);
		uint32_t getsHeaderSSRC() const { return SSRC; }

		bool addReportBlock(reportBlock block);
		std::vector<reportBlock> getReportBlocks() { return reports;}

		bool addSdesItem(sdesItem item);
		std::vector<sdesItem> getSdesItems() { return items; }

		template <typename rtcpPacket>
		rtcpPacket createRtcpPacket(rtcpPayloadTypes type)
		{

			rtcpPacket packet;
			int packLength = header.length + 1;
			uint8_t* packet_pointer;

			switch type:
			case senderReport:
				
				break;
			case ReceiverReport:
				
				break;
			case SourceDescription:
				
				break;
			case Goodbye:
				
				break;
			case AppDef:
				
				break;
			default:
				packet = nullptr;
				
				break;
				packet = packet_pointer;
			return packet;

		}

		template<typename rtcpPacket>
		void setRtcpPacket(rtcpPacket inpacket);

	private:
		rtcpHeader header;
		std::vector<reportBlock> reports;
		std::vector<sdesItem> items;
		uint32_t SSRC;


	};
};

