#include "codecInterface.h"
#include "unordered_map"
#include <boost/any.hpp>
#include "opusInterface.h"



opusInterface::opusInterface(std::unordered_map<std::string, int> sets) {
		encoder = opus_encoder_create(sets["fs"], sets["channels"], sets["application"], error_encoder);
		decoder = opus_decoder_create(sets["fs"], sets["channels"], error_decoder);
	}
opusInterface::	~opusInterface() {
		opus_encoder_destroy(encoder);
		opus_decoder_destroy(decoder);
	}

	int opusInterface::createEncoder(std::unordered_map<std::string, int> sets) {
		encoder = opus_encoder_create(sets["fs"], sets["channels"], sets["application"], error_encoder);
		return *error_encoder;
	}

	int opusInterface::createDecoder(std::unordered_map<std::string, int> sets) {
		opus_decoder_destroy(decoder);
		decoder = opus_decoder_create(sets["fs"], sets["channels"], error_decoder);
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



	std::vector<uint8_t> opusInterface::code(std::vector<uint8_t>) {
		std::vector<uint8_t> vector;
		return vector;
	}

	std::vector<uint8_t> opusInterface::decode(std::vector<uint8_t>) {
		std::vector<uint8_t> vector;
		return vector;
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
