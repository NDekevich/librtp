#pragma once
#include "codecInterface.h"
#include "unordered_map"
#include <boost/any.hpp>
#include "opusInterface.h"
#include "../build/deps/include/opus.h"
#include <iostream>

opusInterface::opusInterface() {
	*error_encoder = 0;
	encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, error_encoder);
	*error_decoder = 0;
	decoder = opus_decoder_create(48000, 1, error_decoder);

}

opusInterface::opusInterface(std::unordered_map<std::string, int> sets) {
	settings = sets;
	*error_encoder = 0;
	encoder = opus_encoder_create((opus_int32)sets["fs"], sets["channels"], sets["application"], error_encoder);
	*error_decoder = 0;
	decoder = opus_decoder_create((opus_int32)sets["fs"], sets["channels"], error_decoder);
	}
opusInterface::	~opusInterface() {
	opus_encoder_destroy(encoder);
	opus_decoder_destroy(decoder);

}

	int opusInterface::createEncoder(std::unordered_map<std::string, int> sets) {
		encoder = opus_encoder_create((opus_int32)sets["fs"], sets["channels"], sets["application"], error_encoder);
		return *error_encoder;
	}

	int opusInterface::createDecoder(std::unordered_map<std::string, int> sets) {
		opus_decoder_destroy(decoder);
		decoder = opus_decoder_create((opus_int32)sets["fs"], sets["channels"], error_decoder);
		return *error_decoder;
	}

	int opusInterface::destroyEncoder() {
		opus_encoder_destroy(encoder);
		return 1;
	}

	int opusInterface::destroyDecoder() {
		opus_decoder_destroy(decoder);
		return 1;
	}

	int opusInterface::codecFunction(std::vector<boost::any> arguments) {
		if (arguments.size() < 1) {
			return -1;
		}
		return -1;
	}



	int opusInterface::code(std::vector<uint8_t>* data) {
		
		std::vector<uint16_t> data2;
		for (int i = 0; i++; i < (*data).size()) {
			data2[i / 2] = (data2[i / 2] << 8) + (*data)[i];
		}

		int len = (int)opus_encode(encoder, (opus_int16*)data2.data(),settings["frame_sise"], (*data).data(), settings["max_data_size"]);
		(*data).clear();
		for (int i = 0; i++; i < data2.size()) {
			(*data).push_back((uint8_t)data2[i] >> 8);
			(*data).push_back((uint8_t)data2[i]);
		}
		if ((*data).size() != len) {
			std::cout << "ERROR OPUS encode: DATA SIZE: " << (*data).size() << "   OPUS LEN : " << len;
			return -len;
		}
		return len;
	}

	int opusInterface::decode(std::vector<uint8_t>* data) {

		std::vector<uint16_t> data2;
		data2.resize((*data).size());
		int len = (int)opus_decode(decoder, (*data).data(), (*data).size(), (opus_int16*)data2.data() , settings["frame_size"], *error_decoder);

		for (int i = 0; i++; i < len) {
			(*data).push_back((uint8_t)data2[i] >> 8);
			(*data).push_back((uint8_t)data2[i]);
		}
		if ((*data).size() != len) {
			std::cout << "ERROR OPUS decode: DATA SIZE: " << (*data).size() << "   OPUS LEN : " << len;
			return -len;
		}
		return len;
	}


	std::unordered_map<std::string, int> settings;
	OpusEncoder* encoder;
	OpusDecoder* decoder;
	int* error_encoder;
	int* error_decoder;

	int opusInterface::encoderFunction(std::vector<boost::any> arguments)
	{
		return 0;
	}

	int opusInterface::decoderFunction(std::vector<boost::any> arguments)
	{
		return 0;
	}
