//#pragma once
//#pragma pack(1)


#include <bitset>
#include <stdint.h>
#include <memory>
#include <vector>


#define MIN_HEADER_LENGTH 12
#define EXTENTION_HEADER_PROFILE_LENGTH 4

namespace rtp
{

	class Rtp
	{
	public: 
		Rtp();
		//template<typename rtpIO>
		Rtp(std::vector<uint8_t> inpacket);

	

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

			OPUS = 75,

			MaxRtpPayload = 127
		};

		

		struct rtpPacket {
			uint8_t firstOctet;
			uint8_t secondOctet;
			uint16_t seqNum;
			uint32_t timeStamp;
			uint32_t SSRC;
			rtpPacket();
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
		uint32_t getSSRC() const { return packet.SSRC; }


		void setExtensionLength(uint16_t length);
		uint16_t getExtensionLength() const { return extensionLength; }

		void setExtensionNum(uint16_t num);
		uint16_t getExtensionNum() const { return extensionNum; }

		void setHeaderExtension(std::vector<uint8_t> ext);


		bool addCSRC(uint32_t);
		std::vector<uint32_t> getCSRC() const { return CSRC; }

		void setSizeofPayload(uint32_t size);
		uint32_t getSizeofPayload() { return sizeofPayload; }


		void setPayload(std::vector<uint8_t > data);
		std::vector<uint8_t>* getPayload() { return &payload; }

		std::vector<uint8_t>* getHeaderExtension() { return &headerExtension; }
		//void setHeaderExtension(std::vector<uint8_t> ext);
		

		std::shared_ptr<std::vector<uint8_t>> createRtpPacket() const;
	
		void setRtpPacket(std::vector<uint8_t>);
			

	
		
		// bool validateHeader(rtpPacket inpacket) const;


	private:
		rtpPacket packet;
		std::vector<uint32_t> CSRC;
		uint16_t extensionNum;
		uint16_t extensionLength;
		std::vector<uint8_t> headerExtension;
		uint32_t sizeofPayload = 1;
		
		std::vector<uint8_t> payload;
	};






}
