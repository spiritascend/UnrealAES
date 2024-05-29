#pragma once


#include <vector>
#include <sstream>
#include <thread>
#include <future>

class Disk
{

public:
    static std::uintptr_t FindPattern(FILE* file, const char* signature);

private:
    static std::vector<int> PatternToByteVector(const char* pattern);
    static std::uintptr_t FindPatternInChunk(FILE* file, const std::vector<int>& patternBytes, std::uintptr_t chunkOffset, size_t chunkSize);
};

