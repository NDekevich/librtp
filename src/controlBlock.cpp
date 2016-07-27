#include "controlBlock.h"

/*
todo note :

finish parsing RTCP in/out messages.
Change sdesItems rtcp + senderReport (possibly);



not working at all yet (blank functions implemented)
octet/packet count unrelaible;
generateSSRC not workign yet
timestamp not working yet(both)
*/


//TODO!!!!!!!!
controlBlock::controlBlock() :
	io_service(new boost::asio::io_service)

{
	outInfo.ssrc = generateSSRC();
	initializeOut();
	generateSeqNum();
	generateRtpTimestampOffset();
}


controlBlock::~controlBlock()
{
	socketRtcpMap.clear();
	socketRtpMap.clear();
}

//TODO!!!!!!!!

/* void controlBlock::setSdesItems(rtcp::Rtcp::rtpSdesTypes type, std::string value)
{

	if (((int)type > 0)&&((int)type<15)) 
	{
	outInfo.sdesInfo[(int)type - 1] = value;
	}

}*/
//TODO!!!!!!!!
void controlBlock::initializeOut() {

	outInfo.timeLastRtcp = 0;
//	outInfo.currentTime = 0;
	outInfo.senders = 0;
	outInfo.pmembers = 0;
	outInfo.members = 0;
	outInfo.we_sent = false;
//	outInfo.rtcp_bw = 123123123;
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
	//	std::shared_ptr<rtp::Rtp> rtpP = std::shared_ptr<rtp::Rtp>(new rtp::Rtp);
		socketRtpMap[socket] = std::make_shared<rtp::Rtp>();
		return true;
	} 
	catch (std::exception& e)
	{
		std::cout << "could not create RTP" << std::endl;
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
		std::shared_ptr<rtcp::Rtcp> rtcp = std::shared_ptr<rtcp::Rtcp>(new rtcp::Rtcp);
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
	return randomSSRC;
}

void controlBlock::generateRtpTimestampOffset()
{
	rtpOffset = 100;
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
