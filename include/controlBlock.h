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

	//TEMPLATES ARE PLACEHOLDERS=d

	template <typename rtpPacket>	
	rtpPacket packRtp(uint8_t* data);
	template <typename rtpPacket>
	uint8_t* unpackRtp(rtpPacket packet);

	template <typename rtpPacket>
	rtpPacket packRtcp(uint8_t* data);
	template <typename rtpPacket>
	void unpackRtcp( rtpPacket packet);

	void initializeOut();
	
	void setRtpPadding(bool p);
	void setRtpExtension(bool x);
	void setRtpPayload(char* payload, int payloadLength);
	void setRtpMarker(bool m);
	
	void setRtpExtLength(uint16_t length);
	void setRtpExtProfile(uint16_t profile);
	void setRtpExtData(uint32_t* data);

	void setSdesItems(rtcp::Rtcp::rtpSdesTypes type, std::string value);	
	void setRtpPrecodedFormat(int format);
	void setRtpCoding(bool codingOn);
	
	void createConversationMembrer(uint32_t ssrc);
	
	uint32_t calculatePacketLoss();
	uint32_t calculateRtcpInterval();
	
	uint8_t* decodeRtpPacket(uint8_t* packet);
	void decodeRtcpPacket(uint8_t* packet);

	uint8_t* createRtpPacket(uint8_t* data);
	uint8_t* createRtcpPacket();

	bool sendRtpPacket(uint8_t* packet, std::shared_ptr<boost::asio::ip::udp::socket> socket);
	bool sendRtpPacket(uint8_t* packet, std::vector<std::shared_ptr<boost::asio::ip::udp::socket>> sockets);

	bool sendRtcpPacket(uint8_t* packet, std::shared_ptr<boost::asio::ip::udp::socket> socket);
	bool sendRtcpPacket(uint8_t* packet, std::vector<std::shared_ptr<boost::asio::ip::udp::socket>> sockets);
	
	std::shared_ptr<boost::asio::ip::udp::socket> createOutputSocket(std::string ip, short port);
	std::shared_ptr<boost::asio::ip::udp::socket> createInputSocket(short port);

	std::shared_ptr<boost::asio::io_service> getService(){return io_service;}


private:
	
	struct sdesItems {
		std::string cname ;
		std::string name ;
		std::string email ;
		std::string phone ;
		std::string loc ;
		std::string tool ;
		std::string note ;
		std::string priv ;
		std::string h323_caddr ;
		std::string apsi ;
		std::string rgrp ;
	};

	struct convMember {
		uint32_t ssrc;
		uint32_t packetsSent;
		uint32_t octetsSent;
		uint32_t packetsReceived;
		uint32_t octetsReceived;

		uint32_t inputPacketLost;
		uint32_t outputPacketLost;	
		uint32_t highestSeqNum;
		uint32_t lastRtpTimestamp;
		uint32_t lastSR;
		uint32_t jitter;
		boost::asio::ip::udp::endpoint rtpPort;
		boost::asio::ip::udp::endpoint rtcpPort;

		sdesItems items;

		bool leftConversation = false;

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
		uint32_t rtcp_bw;
		uint32_t avg_rtcp_size;

		bool we_sent;
		bool initial;
		bool precoded = true;
		
		uint32_t precodedFormat = 0;

		bool padding;
		bool headerExt;
		bool marker;		
		std::string sdesInfo[14];

		sdesItems items;

	} outInfo;

	rtp::Rtp rtpPacketer;
	rtcp::Rtcp rtcpPacketer;

	std::shared_ptr<boost::asio::io_service> io_service;
	std::unordered_map<int, convMember> conversationMembers;
	int outputIdCount;
	int inputIdCount;
	
	uint32_t generateSSRC();
	uint32_t generateRtpTimestamp();
	uint32_t getNtpTimestamp();
	uint32_t generateSeqNum();
	uint32_t countJitter();
	
};