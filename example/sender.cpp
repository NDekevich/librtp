#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "controlBlock.h"


int main(int ac, char *av[]) {
	try 
	{
		bool sender;
		bool rs = false;
		std::string ip = "127.0.0.1";
		std::string file = "file.txt";
		short port = 10001;
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("s", "send packets to specified adress")
			("r", "receive from specified port")
			("ip", po::value<std::string>(&ip), "ip address")
			("port", po::value<short>(&port),"port number")
			("f", po::value<std::string>(&file), "file")
			("rs", "Print receive statistics")
			;

		po::positional_options_description p;
		p.add("f", -1);

		po::variables_map vm;
		po::store(po::command_line_parser(ac, av).
			options(desc).positional(p).run(), vm);
		po::notify(vm);


		if (vm.count("help")) {
			std::cout << "Usage: options_description [options]\n";
			std::cout << desc;
			return 0;
		}

		if (vm.count("s"))
		{
			sender = true;
			std::cout << "Sending to :" << std::endl;
		}

		if (vm.count("r"))
		{
			sender = false;
			std::cout << "Receiving from :  " << std::endl;
		}

		if (vm.count("ip"))
		{
			std::cout << "ip : "
				<< vm["ip"].as<std::string>() << "\n";

		}

		if (vm.count("port"))
		{
			std::cout << "port : "
				<< vm["port"].as<short>() << "\n";

		}

		if (vm.count("f")) {
			std:: cout << "from file " << vm["f"].as<std::string>()
				<< "\n";
		}
		
		if (vm.count("rs")) {
			std::cout << "Recieve stats" << std::endl;
			rs = true;
		}



		if (sender) {
			controlBlock cBlock;
			auto outS = cBlock.createOutputSocket(ip, port);
			if (!cBlock.createRtpVal(outS)) {
				std::cout << "ERROR: -1" << std::endl;
				return -1;
			}
			(*cBlock.socketRtpMap[outS]).setSSRC(1001);
			std::string text;

			std::ifstream myfile(file);
			int i = 0;
			if (myfile.is_open())
			{
				while (std::getline(myfile, text))
				{
					//std::cout << text << std::endl;
					std::vector<uint8_t> data(text.begin(), text.begin() + text.size());
					cBlock.sendRtpData(data, outS);
					data.clear();
					i++;
				}
				myfile.close();
			}
			return 1;
		}
		else
		{
			controlBlock cBlock;
			uint32_t ssrc = 0;
			rtp::Rtp::rtpPayloadTypes pt = rtp::Rtp::rtpPayloadTypes::PCMU;
			uint32_t packetsReceived = 0;
			uint32_t seqNum = 0;
			uint32_t timeStamp = 0;
			uint32_t timeStampStep = 0;
			bool timestampInit = true;
			auto inS = cBlock.createInputSocket(port);
			std::vector<uint8_t> data;
			std::ofstream myfile(file);
			if (myfile.is_open())
			{
				for (;;) {
					size_t len = cBlock.receiveRtpData(inS);	
					std::vector<uint8_t> data = *(*cBlock.socketRtpMap[inS]).getPayload();
					if (rs) {
					
						if (ssrc != (*cBlock.socketRtpMap[inS]).getSSRC()) 
						{
							ssrc = (*cBlock.socketRtpMap[inS]).getSSRC();
							std::cout << "new SSRC : " << ssrc << std::endl;
							std::cout << "timestamp reseted " << ssrc << std::endl;
							std::cout << "packets received reseted " << ssrc << std::endl;

							timestampInit = true;
							packetsReceived = 0;
						}
						if (pt != (*cBlock.socketRtpMap[inS]).getPayloadType()) 
						{
							pt = (*cBlock.socketRtpMap[inS]).getPayloadType();
							std::cout << "new payload type :  " << pt << std::endl;
							timestampInit = true;
						}
						packetsReceived++;
						if (packetsReceived % 100 == 0) 
						{
							std::cout << "received " << packetsReceived << " packets" << std::endl;
						}
						if (seqNum != (*cBlock.socketRtpMap[inS]).getSeqNum()) {
							seqNum = (*cBlock.socketRtpMap[inS]).getSeqNum();
							std::cout << "lost Packet" << std::endl;
							std::cout << "new SeqNum : " << seqNum <<std::endl;
							
						}
						seqNum++;
						if (timestampInit) {
							if (timeStamp == 0) {
								timeStamp = (*cBlock.socketRtpMap[inS]).getTimestamp();
							}
							else
							{
								timeStampStep = (*cBlock.socketRtpMap[inS]).getTimestamp() - timeStamp;
								timestampInit = false;
							}
						}
						else
						{
							if (timeStamp + timestampInit != (*cBlock.socketRtpMap[inS]).getTimestamp())
							{
								timeStamp = (*cBlock.socketRtpMap[inS]).getTimestamp();
								std::cout << "timestamp changed !" << std::endl;
							}
							else
							{
								timeStamp = (*cBlock.socketRtpMap[inS]).getTimestamp();
							}
						}

					}
					//size_t len = cBlock.receiveRawData(&data, inS);	

					std::ostream_iterator<uint8_t> output_iterator(myfile);
					std::copy(data.begin(), data.end(), output_iterator);
				}
				myfile.close();
			}
			return 1;
		}
	}
	catch (std::exception& e) {
		std::cerr <<"Error Sender:" << e.what() << std::endl;
		return 0;
	}
}
