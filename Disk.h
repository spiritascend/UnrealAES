#pragma once

#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <sstream>

class Disk
{

public:
    static std::uintptr_t FindPattern(FILE* file, const char* signature);

private:
    const static int ReadInBufferSize = 4096;

    static std::vector<int> PatternToByteVector(const char* pattern);
};

