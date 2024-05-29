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

    if (fseek(file, startoffset, SEEK_SET) != 0) {
        throw std::runtime_error("failed to seek from the starting offset");
    }

    size_t bytesRead = fread(buffer.data(), 1, buffer.size(), file);
    if (bytesRead < buffer.size()) {
        throw std::runtime_error("failed to read. bytes read exceeds the buffer size");
    }

    constexpr int src_offsets[] = { 0, 7, 14, 21, 25, 32, 39, 46 };
    constexpr int dest_offsets[] = { 0, 4, 8, 12, 16, 20, 24, 28 };

    for (int i = 0; i < 8; ++i) {
        std::memcpy(aesbuffer.data() + dest_offsets[i], buffer.data() + src_offsets[i], 4);
    }

    return VectorToHexString(aesbuffer);
}