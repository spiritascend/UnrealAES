#pragma once


#include <vector>
#include <sstream>
#include <iomanip>

class AesProvider
{
public:
	static std::string GetAESKey(FILE* file, long long startoffset);
private:
	const static int32_t AES_BYTESIZE = 32;

	static std::string VectorToHexString(const std::vector<uint8_t>& buffer);
};

