//#include "libavcodec/avcodec.h"
#include "boost\asio.hpp"
#include "boost\array.hpp"
#include "RTP.h"
#include "rtcp.h"
#include <unordered_map>


#define BUFFER_SIZE 1024


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

	
	/*bool sendRtpData(std::vector<uint8_t> data, std::shared_ptr<boost::asio::ip::udp::socket> socket) {
		try {
			if (socketRtpMap[socket] == nullptr) {
				if (!(createRtpVal(socket))) {
					return false;
				}
			}
			(*socketRtpMap[socket].get()).setPayload(data);
			std::vector<uint8_t> prestart = *(*socketRtpMap[socket].get()).createRtpPacket();
			//uint8_t* packet = (*socketRtpMap[socket]).createRtpPacket();
			std::vector<uint8_t> buff;
			std::copy(packet, packet + 34, buff.begin());
			(*socket).send(boost::asio::buffer(buff));
			//rtpPacketSentStats(*packet);
			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
	}
	*/

	bool receiveRtpData(std::shared_ptr <boost::asio::ip::udp::socket> socket) {
		try {
			if (socketRtpMap[socket] == nullptr) {
				if (!(createRtpVal(socket))) {
					std::cerr << "Could not create rtp receiver\n\n";
					return false;
				}
			}
			std::vector<uint8_t> v(BUFFER_SIZE);
			int len = (*socket).receive(boost::asio::buffer(v));
			std::vector<uint8_t> temp(v.begin(), v.begin() + len);
			(*socketRtpMap[socket].get()).setRtpPacket(temp);
			return true;

		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
	}
	
	
	
	template<typename D>
	bool sendRtpData(D data, std::shared_ptr<boost::asio::ip::udp::socket> socket) {
		try {
			if (socketRtpMap[socket] == nullptr) {
				if (!(createRtpVal(socket))) {
					std::cerr << "Could not create rtp receiver\n\n";
					return false;
				}
			}
			(*socketRtpMap[socket].get()).setPayload(data);
			std::vector<uint8_t> v = *(*socketRtpMap[socket].get()).createRtpPacket();
			(*socket).send(boost::asio::buffer(v));
			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
	}
	
	
	
	
	template <typename R> 
	bool sendRtcpPacket(std::shared_ptr<boost::asio::ip::udp::socket> socket) {
		try {
			if (socketRtcpMap[socket] == nullptr) {
				if (!(createRtcpVal(socket))) {
					std::cerr << "Could not create rtp receiver\n\n";
					return false;
				}
			}
			std::vector<uint8_t> v = *(*socketRtpMap[socket].get()).createRtpPacket();
			(*socket).send(boost::asio::buffer(v));
			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
	}


	template <typename D>
	bool sendRawData(D data, std::shared_ptr<boost::asio::ip::udp::socket> socket) {
		try {
			(*socket).send(boost::asio::buffer(*data));
			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
	}

	template <typename D>
	int receiveRawData(D data, std::shared_ptr<boost::asio::ip::udp::socket> socket) {
		try {
			return (*socket).receive(boost::asio::buffer(*data));
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return -1;
		}
	}
	
	std::shared_ptr<boost::asio::ip::udp::socket> createOutputSocket(std::string ip, short port);
	std::shared_ptr<boost::asio::ip::udp::socket> createInputSocket(short port);

	bool createRtpVal(std::shared_ptr<boost::asio::ip::udp::socket> socket);
	bool deleteRtpVal(std::shared_ptr<boost::asio::ip::udp::socket> socket);

	bool createRtcpVal(std::shared_ptr<boost::asio::ip::udp::socket> socket);
	bool deleteRtcpVal(std::shared_ptr<boost::asio::ip::udp::socket> socket);


	std::shared_ptr<boost::asio::io_service> getService(){return io_service;}

	std::unordered_map<std::shared_ptr<boost::asio::ip::udp::socket>, std::shared_ptr<rtp::Rtp>> socketRtpMap;
	std::unordered_map<std::shared_ptr<boost::asio::ip::udp::socket>, std::shared_ptr<rtcp::Rtcp>> socketRtcpMap;

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
		
		int precodedFormat = 0;

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
	uint32_t getNtpTimestampS();
	uint32_t getNtpTimestampF();
	uint32_t generateSeqNum();
	uint32_t countJitter();

};