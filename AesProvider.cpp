#include "AesProvider.h"


std::string AesProvider::VectorToHexString(const std::vector<uint8_t>& vec) {
    std::ostringstream oss;
    oss << "0x";
    for (const auto& byte : vec) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

std::string AesProvider::GetAESKey(FILE* file, long long startoffset) {
    std::vector<std::uint8_t> buffer(58);
    std::vector<std::uint8_t> aesbuffer(AES_BYTESIZE);
    fseek(file, startoffset, SEEK_SET);
    size_t bytesRead = fread(buffer.data(), 1, buffer.size(), file);

    if (bytesRead < buffer.size()) {
        throw std::runtime_error("Failed to read enough data from the file.");
    }

    int numberofbytesinserted = 0;
    int offset = 0;

    while (numberofbytesinserted < AES_BYTESIZE) {
        if (numberofbytesinserted == 8) {
            offset += 4;
        }
        std::memcpy(aesbuffer.data() + numberofbytesinserted, buffer.data() + offset, 4);
        numberofbytesinserted += 4;
        offset += 7;
    }

    return VectorToHexString(aesbuffer);
}