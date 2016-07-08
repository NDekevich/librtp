//#include "libavcodec/avcodec.h"
#include "boost\asio.hpp"
#include "RTP.h"
#include "rtcp.h"
#include <unordered_map>

class controlBlock
{
public:
	controlBlock();
	~controlBlock();

	template <typename rtpPacket>	
	rtpPacket packRtp(uint8_t* data);
	template <typename rtpPacket>
	uint8_t* unpackRtp(rtpPacket packet);

	template <typename rtcpPacket>
	rtcpPacket packRtcp(uint8_t* data);
	template <typename rtcpPacket>
	void unpackRtcp(rtcpPacket packet);

	/*
	void setRtpPadding(bool p);
	void setRtpExtension(bool x);
	void setRtpPayload(char* payload, int payloadLength);
	void setRtpMarker(bool m);
	*/

	void setSdesItems(rtcp::Rtcp::rtpSdesTypes type, std::string value);

	rtp::Rtp rtpPacker;
	
	rtcp::Rtcp rtcpPacker;
	
	void setRtpPrecodedFormat(int format);
	
	void setRtpCoding(bool codingOn);
	

private:
	bool rtpCoding = true;
	
	struct sdesItems {
		std::string cname = "";
		std::string name = "";
		std::string email = "";
		std::string phone = "";
		std::string loc = "";
		std::string tool = "";
		std::string note = "";
		std::string priv = "";
		std::string h323_caddr = "";
		std::string apsi = "";
		std::string rgrp = "";
	};



	struct outputInfo {
		uint32_t ssrc;
		uint32_t packetsSent;
		uint32_t octetsSent;
		uint32_t timeLastRtcp;
		uint32_t currentTime;
		uint32_t nextRtcp;
		uint32_t pmembers;
		uint32_t members;
		uint32_t senders;
		bool padding;
		bool headerExt;
		std::string sdesInfo[14];

		outputInfo();
	} outInfo;

	struct inputInfo {
		
		uint32_t ssrc;
		uint32_t packetsRecieved;
		uint32_t octetsRecieved;
		int32_t packetsLost;
		int32_t octetsLost;
		uint32_t highestSeqNum;
		uint32_t lastRtpTimestamp;
		uint32_t lastSR;
		uint32_t jitter;
		sdesItems sdesInfo;
		inputInfo();
	} inInfo;

	boost::asio::io_service service;
	std::unordered_map<int, outputInfo> outputPort;	
	std::unordered_map<int, inputInfo> inputPort;
	int outputIdCount;
	int inputIdCount;
	
	
	
	uint32_t getRtpTimestamp();
	uint32_t getNtpTimestamp();
	uint32_t getSeqNum();
	uint32_t countJitter();
	
};

