//#include "libavcodec/avcodec.h"
//#pragma pack(1)


// ADD RTCP sending when receiving/sending rtp packets

#pragma once

#include "rtp.h"
#include "rtcp.h"

#include "codecInterface.h"
#include "opusInterface.h"


#include <boost/asio.hpp>
#include <boost/array.hpp>

#include <unordered_map>
#include <queue>

#define BUFFER_SIZE 128000
#define PACKETS_TO_IGNORE 5

using socket_udp = boost::asio::ip::udp::socket;

class controlBlock
{
public:
	controlBlock();
	~controlBlock();

	//TEMPLATES ARE PLACEHOLDERS=d
	void initializeOut();

	//void setSdesItems(rtcp::Rtcp::rtpSdesTypes type, std::string value);
	void setRtpPrecodedFormat(int format) {
		outInfo.precodedFormat = format;
	}
	
	void setRtpCoding(bool codingOn) {
		outInfo.precoded = true;
	}

	void createConversationMembrer(uint32_t ssrc);

	uint32_t calculatePacketLoss();
	uint32_t calculateRtcpInterval();

	uint8_t* decodeRtpPacket(uint8_t* packet);
	void decodeRtcpPacket(uint8_t* packet);

	uint8_t* createRtpPacket(uint8_t* data);
	uint8_t* createRtcpPacket();


	int receiveRtpData(std::shared_ptr <boost::asio::ip::udp::socket> socket) {
		try {
			if (socketRtpMap[socket] == nullptr) {
				if (!(createRtpVal(socket))) {
					std::cerr << "Could not create rtp receiver\n\n";
					return 0;
				}
			}
			std::vector<uint8_t> v(BUFFER_SIZE);
			//v.reserve(BUFFER_SIZE);
			int len = (*socket).receive(boost::asio::buffer(v));
			std::vector<uint8_t> temp(v.begin(), v.begin() + len);
			rtp::Rtp* packet = socketRtpMap[socket].get();
			(*socketRtpMap[socket].get()).setRtpPacket(temp);
			convMember* convM = &(conversationMembers[((*socketRtpMap[socket].get()).getSSRC())]);
			if ((*convM).packetsReceived < PACKETS_TO_IGNORE) 
			{
				(*convM).packetsReceived++;
			}
			else
			{
				if ((*convM).highestSeqNum < (*packet).getSeqNum()) {
					(*convM).packetsReceived++;
					(*convM).octetsReceived += len;
					//		int D = ((*packet).getTimestamp() - (*convM).lastRtpTimestamp) - (currentRtpTime() - (*convM).lastRtpTimestampArrival);
						//	(*convM).jitter = (*convM).jitter + (std::abs(D) - (*convM).jitter) / 16;
					(*convM).lastRtpTimestamp = (*packet).getTimestamp();
					//	(*convM).lastRtpTimestampArrival = currentRtpTime();
					(*convM).packetLost += ((*packet).getSeqNum() - (*convM).highestSeqNum - 1);
				}
			}
			return len;
		}
		catch (std::exception& e)
		{
			std::cerr<<"Error Cblock 1: " << e.what() << std::endl;
			return 0;
		}
	}

	int receiveRtp_from(std::shared_ptr<boost::asio::ip::udp::socket> socket, short port) {
		try
		{
			(*socket).close();
			(*socket).bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));
			return (receiveRtpData(socket));
		}
		catch (std::exception& e)
		{
			std::cerr << "Error Cblock 2: " << e.what() << std::endl;
			return 0;
		}
	}


	template<typename D>
	int sendRtpData(D data, std::shared_ptr<boost::asio::ip::udp::socket> socket) {
		try {
			int size;
			if (socketRtpMap[socket] == nullptr) {
				if (!(createRtpVal(socket))) {
					std::cerr << "Could not create rtp receiver\n\n";
					return -1;
				}
			}
			if (outInfo.precoded)
			{
				(*socketRtpMap[socket].get()).setPayloadType((rtp::Rtp::rtpPayloadTypes)outInfo.precodedFormat);
				(*socketRtpMap[socket].get()).setPayload(data);
				size =  data.size();
			}
			else
			{
				std::unordered_map<std::string, int> tempMap;
				tempMap["fs"] = 48000;
				tempMap["channels"] = 1;
				tempMap["applications"] = OPUS_APPLICATION_VOIP;
				opusInterface opusC(tempMap);
				opus_encoder_ctl(opusC.encoder, OPUS_SET_COMPLEXITY(5), OPUS_SET_BITRATE(OPUS_AUTO), OPUS_SET_MAX_BANDWIDTH(OPUS_BANDWIDTH_FULLBAND));
				std::vector<uint16_t> data2;
				for (int i = 0; i++; i < data.size()) {
					data2[i / 2] = (data2[i / 2] << 8) + data[i];
				}

				int len = opus_encode(opusC.encoder, (opus_int16*)data2.data(), 20, data.data(), 480*20);
				std::vector<uint8_t>data3(data.begin(), data.begin() + len);
				//opus_int16*: Input signal (interleaved if 2 channels). length is frame_size*channels*sizeof(opus_int16)  = 20*1*2
				/*
				int encoder_error = 0;
				(*socketRtpMap[socket].get()).setPayloadType(75);
				auto encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_AUDIO, &encoder_error);
				opus_encoder_ctl(&encoder, OPUS_SET_COMPLEXITY(5), OPUS_SET_BITRATE(OPUS_AUTO), OPUS_SET_MAX_BANDWIDTH(OPUS_BANDWIDTH_FULLBAND));

				std::vector<uint8_t> data2();
				uint16_t temp = 0;
				int place = 0;

				for (int it1 = 0; it1 < data.size()/2; it1++) {
					temp = data[place] << 8;
					place++;
					temp = data[place] << 8;
					place++;
					data2.pushback(temp);
					temp = 0;
				}
				
				size = opus_encode(&encoder, data2.data(), 480, data.data(); 480*20*1);
				*/
				(*socketRtpMap[socket].get()).setPayload(data3);
				// ADD CODE FOR CODING DATA;
				//
			}
			//(*socketRtpMap[socket].get()).setTimestamp(currentRtpTime());
			std::vector<uint8_t> v = *(*socketRtpMap[socket].get()).createRtpPacket();
			(*socket).send(boost::asio::buffer(v));

			/*convMember* convM = &(conversationMembers[((*socketRtpMap[socket].get()).getSSRC())]);
			(*convM).packetsSent++;
			(*convM).octetsSent += v.size();
			*/return size;
		}
		catch (std::exception& e)
		{
			std::cerr<< "Error Cblock 3: " << e.what() << std::endl;
			return -1;
		}
	}

	template<typename D>
	bool sendRtp_to(D data, std::shared_ptr<boost::asio::ip::udp::socket> socket, std::string ip, short port)
	{
		try
		{
			(*socket).close();
			(*socket).bind(boost::asio::ip::udp::endpoint(ip, port));
			return sendRtpData(data, socket);
		}
		catch (std::exception& e)
		{
			std::cerr << "Error Cblock 4: " << e.what() << std::endl;
			return false;
		}
	}


	bool sendRtcpPacket(std::shared_ptr<boost::asio::ip::udp::socket> socket) {
		try {
			if (socketRtcpMap[socket] == nullptr) {
				if (!(createRtcpVal(socket))) {
					std::cerr << "Could not create rtp receiver\n\n";
					return false;
				}
			}



			std::vector<uint8_t> v = *(*socketRtcpMap[socket].get()).createRtcpPacket();
			(*socket).send(boost::asio::buffer(v));
			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << "Error Cblock 5: " << e.what() << std::endl;
			return false;
		}
	}

	bool receiveRtcpData(std::shared_ptr<boost::asio::ip::udp::socket> socket) {
		try
		{
			if (socketRtcpMap[socket] == nullptr) {
				if (!(createRtcpVal(socket))) {
					std::cerr << "Could not create rtcp receiver\n\n";
					return false;
				}
			}
			std::vector<uint8_t> v(BUFFER_SIZE);
			//v.reserve(BUFFER_SIZE);
			int len = (*socket).receive(boost::asio::buffer(v));
			std::vector<uint8_t> temp(v.begin(), v.begin() + len);
			(*socketRtcpMap[socket].get()).setRtcpPacket(temp);
			rtcp::Rtcp* packet = (socketRtcpMap[socket].get());
			convMember* convM = &(conversationMembers[((*socketRtcpMap[socket].get()).SSRC)]);
			if ((*convM).packetsReceived < PACKETS_TO_IGNORE) {
				(*convM).packetsReceived++;
			}
			else
			{
				if ((*convM).packetsReceived == PACKETS_TO_IGNORE){
					outInfo.members++;
				}
				(*convM).packetsReceived++;
				(*convM).octetsReceived += len;
				int type;
				switch ((*packet).getPayload())
				{
				case (200)://SenderReport
					(*convM).lastRtpTimestamp = (*packet).senderReport.rtpTimestamp;
					(*convM).packetsSentBySource += (*packet).senderReport.packetCount;
					(*convM).octetsSentBySource += (*packet).senderReport.octetCount;
					//add ntpCLOCK!!!

				case (201)://ReceiverReport
					BOOST_FOREACH(rtcp::Rtcp::reportBlock rb , (*packet).getReportBlocks()) {
						conversationMembers[rb.ssrc].fractionLostBySource = rb.fractionLost;
						conversationMembers[rb.ssrc].packetLostBySource = rb.packetsLost;
						conversationMembers[rb.ssrc].highestSeqNumBySource = rb.highestSeqNum;
						conversationMembers[rb.ssrc].jitterBySource = rb.interarrivalJitter;
						conversationMembers[rb.ssrc].lastSRBySource = rb.lastSR;
						conversationMembers[rb.ssrc].delaySlsrBySource = rb.delaySinceLSR;
					}
					break;
				case (202)://SourceDescriptionReport
					for (int i = 0; i < (*packet).getReportCount(); i++)
					{
						type = (*packet).getSdesItems()[i].rtpSdesType;
						(*convM).sdesItems[type].insert((*convM).sdesItems[type].begin(), (*packet).getSdesItems()[i].item.begin(), (*packet).getSdesItems()[i].item.end());
					}
					break;
				case (203)://GoodbyeReport
					(*convM).leftConversation = true;
					BOOST_FOREACH(uint32_t l , (*packet).otherLeavers)
					{
						(conversationMembers[l]).leftConversation = true;
					}
					if ((*packet).optGoodbyeText)
					{
						(*convM).leaveMsg.insert((*convM).leaveMsg.begin(), (*packet).goodbyeText.begin(), (*packet).goodbyeText.end());
					}
					break;
				case (204)://App specific report


					break;
				default:
					return false;
				}
			}
			//////////////////////////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << "Error Cblock 6: " << e.what() << std::endl;
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
			std::cerr << "Error Cblock 7: " << e.what() << std::endl;
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
			std::cerr << "Error Cblock 8: " << e.what() << std::endl;
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
	int rtpOffset;

	struct convMember {
		//uint32_t ssrc;
		uint32_t packetsSent;
		uint32_t octetsSent;
		uint32_t packetsReceived = 0;
		uint32_t octetsReceived;
		uint32_t packetLost;
		uint32_t highestSeqNum;
		uint32_t lastRtpTimestamp;
		uint32_t lastRtpTimestampArrival;
		uint32_t lastSR;
		uint32_t jitter;
		uint32_t delaySLSR;
		uint32_t fractionLost;

		uint32_t packetsSentBySource;
		uint32_t octetsSentBySource;
		uint32_t packetsReceivedBySource;
		uint32_t octetsReceivedBySource;
		uint32_t packetLostBySource;
		uint32_t highestSeqNumBySource;
		uint32_t lastRtpTimestampBySource;
		uint32_t lastRtpTimestampArrivalBySource;
		uint32_t jitterBySource;
		uint32_t lastSRBySource;
		uint32_t fractionLostBySource;
		uint32_t delaySlsrBySource;

		std::queue<int> itemsToSend;

		boost::asio::ip::udp::endpoint rtpPort;
		boost::asio::ip::udp::endpoint rtcpPort;

		std::vector<std::string> sdesItems;
		std::string leaveMsg;

		bool leftConversation = false;
	};



	struct outputInfo {
		uint32_t ssrc;

		uint32_t packetsSent;
		uint32_t octetsSent;
		uint32_t timeLastRtcp;
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

		std::vector<std::string> sdesItems;

	} outInfo;

	rtp::Rtp rtpPacketer;
	rtcp::Rtcp rtcpPacketer;

	std::shared_ptr<boost::asio::io_service> io_service;



	std::unordered_map<uint32_t, convMember> conversationMembers;
	int outputIdCount;
	int inputIdCount;

	uint32_t generateSSRC();
	void generateRtpTimestampOffset();
	uint32_t getNtpTimestampS();
	uint32_t getNtpTimestampF();
	uint32_t generateSeqNum();
	uint32_t currentRtpTime();
};
