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





	/*
	int* rawToCodec(int codecType,int* data);

	int* codecToRaw(int codecType, int* data);
	*/
	
	int createOutput(std::string ipAdress, int port);

	int createInput(int port);

	bool sendData(int connectionID, uint8_t data);
	
	uint8_t receiveData(int connectionID);



	




private:

	struct outputInfo {
		
		boost::asio::ip::udp::endpoint* endpoint;
		boost::asio::ip::udp::socket* socket;


		uint32_t packetsSent;
		uint32_t packetsReceived;
		outputInfo();
	};

	struct inputInfo {
		
		boost::asio::ip::udp::socket socket;
		boost::asio::ip::udp::endpoint endpoint;
		uint32_t packetsRecieved;
		uint32_t octetsRecieved;
		int32_t packetsLost;
		int32_t octetsLost;
		uint32_t highestSeqNum;
		uint32_t lastRtpTimestamp;
		uint32_t lastSR;
		uint32_t jitter;
		inputInfo();
	};



	boost::asio::io_service service;
	std::unordered_map<int, outputInfo> outputPort;	
	std::unordered_map<int, inputInfo> inputPort;
	int outputIdCount;
	int inputIdCount;


};

