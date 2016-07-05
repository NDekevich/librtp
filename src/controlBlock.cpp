#include "controlBlock.h"
#include "boost\asio.hpp"


controlBlock::controlBlock()
{
}


controlBlock::~controlBlock()
{
}

/*
int controlBlock::createOutput(std::string ipAdress, int port)
{
	//add block
	int connectionID = outputIdCount++;
	//block
	boost::asio::ip::udp::endpoint ep(boost::asio::ip::address::from_string(ipAdress), port);
	boost::asio::ip::udp::socket socket(service);
	socket.connect(ep);
	
	outputPort[connectionID] = outputInfo();
	outputPort[connectionID].endpoint = &ep;
	outputPort[connectionID].socket = &socket;


	return connectionID;
}
	*/