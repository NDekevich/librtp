#include "controlBlock.h"

/*
todo note :







!!!CHANGE DATA TYPE TO VECTORS!!!!!!!!!!!!!!! 
fromRtcpleft only




create generating rtp/rtcp methods;
change way data is passed;


working on:
sending and receiving data from network(via rtp/rtcp);
(sendRtpData
receiveRtpData)




not working at all yet (blank functions implemented)
No jitter;
octet/packet count unrelaible;
generateSSRC not workign yet=)
timestamp not working yet(both)
*/


//TODO!!!!!!!!
controlBlock::controlBlock() :
	io_service(new boost::asio::io_service)
		

{

	//generateSSRC();
	initializeOut();
	//generateSeqNum();
	//generateRtpTimestamp();
	

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
//	calculateRtcpInterval();

}




std::shared_ptr<boost::asio::ip::udp::socket> controlBlock::createOutputSocket(std::string ip, short port)
{
	try {
		boost::asio::ip::udp::endpoint ep(boost::asio::ip::address::from_string(ip), port);
		std::shared_ptr<boost::asio::ip::udp::socket> socket(new boost::asio::ip::udp::socket(*io_service)); 
		(*socket).connect(ep);
		return socket;
	}
	catch (std::exception& e)
	{

		std::cerr << e.what() << std::endl;
		return nullptr;
	}
}

std::shared_ptr<boost::asio::ip::udp::socket> controlBlock::createInputSocket(short port)
{

	try {
		boost::asio::ip::udp::endpoint ep(boost::asio::ip::udp::v4(), port);
		std::shared_ptr<boost::asio::ip::udp::socket> socket(new boost::asio::ip::udp::socket(*io_service,ep));
		
		return socket;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return nullptr;
	}
}

bool controlBlock::createRtpVal(std::shared_ptr<boost::asio::ip::udp::socket> socket)
{
	try {
		std::shared_ptr<rtp::Rtp> rtp = std::shared_ptr<rtp::Rtp>(new rtp::Rtp);
		socketRtpMap[socket] = rtp;
		return true;
	} 
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
}

bool controlBlock::deleteRtpVal(std::shared_ptr<boost::asio::ip::udp::socket> socket)
{
	try {
		socketRtpMap.erase(socket);
		return true;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
}

bool controlBlock::createRtcpVal(std::shared_ptr<boost::asio::ip::udp::socket> socket)
{
	try {
		std::shared_ptr<rtcp::Rtcp> rtcp;
		socketRtcpMap[socket] = rtcp;
		return true;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
}

bool controlBlock::deleteRtcpVal(std::shared_ptr<boost::asio::ip::udp::socket> socket)
{
	try {
		socketRtcpMap.erase(socket);
		return true;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
}

uint32_t controlBlock::generateSSRC()
{
	uint32_t randomSSRC = 1;
	return 
}

uint32_t controlBlock::generateRtpTimestamp()
{
	uint32_t rtpTime = 100;
	return rtpTime;
}

uint32_t controlBlock::getNtpTimestampS()
{
	uint32_t ntpS = 100;
	return 100;
}

uint32_t controlBlock::getNtpTimestampF()
{
	uint32_t ntpF = 5;
	return ntpF;
}

uint32_t controlBlock::generateSeqNum()
{
	uint32_t seqNum = 0;
	return seqNum;
}
