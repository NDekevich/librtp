#include "controlBlock.h"
#include "boost\asio.hpp"

//TODO!!!!!!!!
controlBlock::controlBlock()
{
	generateSSRC();
	initializeOut();
	generateSeqNum();
	generateRtpTimestamp();


}


controlBlock::~controlBlock()
{
}
//TODO!!!!!!!!
void controlBlock::setSdesItems(rtcp::Rtcp::rtpSdesTypes type, std::string value)
{

	if (((int)type > 0)&&((int)type<15)) {
	outInfo.sdesInfo[(int)type - 1] = value;
	}

}
//TODO!!!!!!!!
void controlBlock::initializeOut() {

	outInfo.timeLastRtcp = 0;
	outInfo.currentTime = 0;
	outInfo.senders = 0;
	outInfo.pmembers = 0;
	outInfo.members = 0;
	outInfo.we_sent = false;
	//outInfo.rtcp_bw = 123123123;
	outInfo.initial = true;
	calculateRtcpInterval();

}
//TODO!!!!!!!!
uint8_t* controlBlock::decodeRtpPacket(uint8_t * packet)
{
	rtpPacketer.setRtpPacket(packet);
	if (rtpPacketer.getVersion() == 2) {
		if (rtpPacketer.getPayloadType() < 128) {
			if (conversationMembers[rtpPacketer.getSSRC()].highestSeqNum < rtpPacketer.getSeqNum()) 
			{
				conversationMembers[rtcpPacketer.getsHeaderSSRC()].packetsReceived++;
				conversationMembers[rtcpPacketer.getsHeaderSSRC()].inputPacketLost += rtpPacketer.getSeqNum() - conversationMembers[rtcpPacketer.getsHeaderSSRC()].highestSeqNum - 1;
				// add conversationMembers[rtcpPacketer.getsHeaderSSRC()].octetsReceived+= rtpPacketer
			} else {
				return nullptr;
			}
		}
	}
	return nullptr;
}
//todo!!!!!!!!
void controlBlock::decodeRtcpPacket(uint8_t* packet)  //not scalable
{
	rtcpPacketer.setRtcpPacket(packet);
	if (rtcpPacketer.getVersion() == 2) {
		if (rtcpPacketer.getPayload() == 203)
		{
			conversationMembers[rtcpPacketer.getsHeaderSSRC()].leftConversation = true;
		}
		
		if (rtcpPacketer.getPayload() > 190) 
		{

			conversationMembers[rtcpPacketer.getsHeaderSSRC()].packetsReceived++;
			conversationMembers[rtcpPacketer.getsHeaderSSRC()].octetsReceived += rtcpPacketer.getHeaderLength();
			conversationMembers[rtcpPacketer.getsHeaderSSRC()].jitter = countJitter();
			
		}
	}


}

std::shared_ptr<boost::asio::ip::udp::socket> controlBlock::createOutputSocket(std::string ip, short port)
{

	boost::asio::ip::udp::endpoint ep(boost::asio::ip::address::from_string(ip), port);
	std::shared_ptr<boost::asio::ip::udp::socket> socket(new boost::asio::ip::udp::socket(io_service, ep));

	return socket;
}

std::shared_ptr<boost::asio::ip::udp::socket> controlBlock::createInputSocket(short port)
{
	boost::asio::ip::udp::endpoint ep(boost::asio::ip::udp::v4(), port);
	std::shared_ptr<boost::asio::ip::udp::socket> socket(new boost::asio::ip::udp::socket(io_service,ep));

	return socket;
}
