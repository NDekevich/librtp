//#pragma once
#include <bitset>
#include <stdint.h>
#include <vector>

#define MIN_HEADER_LENGTH 12
#define EXTENTION_HEADER_PROFILE_LENGTH 4

namespace rtp
{

	class Rtp
	{
	public: 
		Rtp(bool padding = false, bool headerExt = false);
		template<typename rtpIO>
		Rtp(rtpIO input);

	

	//	Rtp(int version = 2, bool padding = false, bool headerExt = false, );

		enum rtpPayloadTypes {
			// Audio/Video Payload
			PCMU = 0,	//G711U
			GSM = 3,
			G723 = 4,
			DVI4_8k = 5,	//8k hz clock rate
			DVI4_16k = 6,	//16k hz clock rate
			LPC = 7,
			PCMA = 8,	//G711A
			G722 = 9,
			L16_2c = 10,	//Stereo
			L16_1c = 11,	//Mono
			QCELP = 12,
			CN = 13,
			MPA = 14,
			G728 = 15,
			DVI4_11k = 16,	//11025 hz clock rate
			DVI4_22k = 17,	//22050 hz clock rate
			G729 = 18,
			CELB = 25,
			JPEG = 26,
			nv = 28,
			H261 = 31,
			MPV = 32,
			MP2T = 33,
			H263 = 34,

			MaxRtpPayload = 127
		};

		

		struct rtpPacket {
			uint8_t firstOctet;
			uint8_t secondOctet;
			uint16_t seqNum;
			uint32_t timeStamp;
			uint32_t SSRC;
		};

		

		void setVersion(int version=2);
		int getVersion() const;

		void setPadding(bool padding=false); 
		bool getPadding() const;
		
		void setExtension(bool extension=false);
		bool getExtension() const;

		void setCSRCcount(int cc=0);
		int getCSRCcount() const;

		void setMarker(bool marker=false);
		bool getMarker() const;

		void setPayloadType(rtpPayloadTypes pt);
		rtpPayloadTypes getPayloadType() const;

		void setSeqNum(uint16_t seqNum);
		uint16_t getSeqNum() const { return packet.seqNum; }

		void setTimestamp(uint32_t time);
		uint32_t getTimestamp() const { return packet.timeStamp; }

		void setSSRC(uint32_t ssrc);
		uint32_t getSSRC() const { return packet.seqNum; }


		void setExtensionLength(uint16_t length);
		uint16_t getExtensionLength() const { return extensionLength; }

		void setExtensionNum(uint16_t num);
		uint16_t getExtensionNum() const { return extensionNum; }

		template <typename extensionType>
		void Rtp::setHeaderExtension(extensionType pointer);


		bool addCSRC(uint32_t);

/*
		void setPayload(int* payload);
		int* getPayload() const;
*/
		
		

		uint8_t* createRtpPacket() const;
		template <typename rtpIO>
		void setRtpPacket(rtpIO input);

	
		
		// bool validateHeader(rtpPacket inpacket) const;


	private:
		rtpPacket packet;
		std::vector<uint32_t> CSRC;
		uint16_t extensionNum;
		uint16_t extensionLength;
		void* headerExtension;
	};
/*
	class Rtcp
	{
	public :
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
			uint8_t rtpSdesType;
			uint32_t length;

		};


		struct senderInfo {
			uint64_t ntpTimestamp;
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
		};


		struct rtcpHeader {
			std::bitset<8> firstOctet; // 0-1 version, 2 padding, 3-7 RECEPTION Report count
			uint8_t payloadType;
			uint16_t length;
			uint32_t SSRC;
		};

		Rtcp();


		void setVersion(int version = 2);
		int getVersion() const;
		
		void setPadding(bool padding = false);
		bool getPadding() const;


		void setPayload(rtcpPayloadTypes pc);
		rtcpPayloadTypes getPayload() const;

		void setLength(uint16_t length);
		uint16_t getLength() const;

		void setSSRC(uint32_t ssrc);
		uint32_t getSSRC() const;



		



	};
	*/

}
