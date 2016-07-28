#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "controlBlock.h"
#define SENDER false
#define PORT 30000
#define IP "127.0.0.1"

int main() {
	if (SENDER) {
		controlBlock cBlock;
		std::string ip = IP;
		auto outS = cBlock.createOutputSocket(ip, PORT);
		if (!cBlock.createRtpVal(outS)) {
			std::cout << "ERROR: -1" << std::endl;
			return -1;
		}
		(*cBlock.socketRtpMap[outS]).setSSRC(1001);
		std::string text;

		std::ifstream myfile("from.txt");
		int i = 0;
		if (myfile.is_open())
		{
			while (std::getline(myfile, text))
			{
				//std::cout << text << std::endl;
				std::vector<uint8_t> data(text.begin(), text.begin()+text.size());
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
		auto inS = cBlock.createInputSocket(PORT);
		std::vector<uint8_t> data;
		std::ofstream myfile("to.txt");
		if (myfile.is_open()) 
		{
			for (;;) {
				size_t len = cBlock.receiveRtpData(inS);
			
				std::vector<uint8_t> data = *(*cBlock.socketRtpMap[inS]).getPayload();
				//size_t len = cBlock.receiveRawData(&data, inS);	
				myfile << "r: ";
				std::ostream_iterator<uint8_t> output_iterator(myfile);
				std::copy(data.begin(), data.end(), output_iterator);
				myfile << std::endl;
			}
			myfile.close();
		}
		return 1;
	}
}
