#include "Disk.h"

std::vector<int> Disk::PatternToByteVector(const char* pattern)
{
    auto bytes = std::vector<int>{};
    const auto start = const_cast<char*>(pattern);
    const auto end = const_cast<char*>(pattern) + strlen(pattern);

    for (auto current = start; current < end; ++current)
    {
        if (*current == '?')
        {
            ++current;
            if (*current == '?')
                ++current;
            bytes.push_back(-1);
        }
        else
        {
            bytes.push_back(strtoul(current, &current, 16));
        }
    }
    return bytes;
}

std::uintptr_t Disk::FindPatternInChunk(FILE* file, const std::vector<int>& patternBytes, std::uintptr_t chunkOffset, size_t chunkSize)
{
    std::vector<std::uint8_t> buffer(chunkSize);
    fseek(file, chunkOffset, SEEK_SET);
    size_t bytesRead = fread(buffer.data(), 1, chunkSize, file);

    const auto s = patternBytes.size();
    const auto d = patternBytes.data();

    for (size_t i = 0; i <= bytesRead - s; ++i)
    {
        bool found = true;
        for (size_t j = 0; j < s; ++j)
        {
            if (buffer[i + j] != d[j] && d[j] != -1)
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            return chunkOffset + i;
        }
    }

    return std::uintptr_t(-1);
}


std::uintptr_t Disk::FindPattern(FILE* file, const char* signature)
{
    if (!file)
        throw std::invalid_argument("Invalid file pointer");

    const auto patternBytes = PatternToByteVector(signature);
    fseek(file, 0, SEEK_END);
    std::uintptr_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const size_t numThreads = std::thread::hardware_concurrency();
    const size_t chunkSize = (fileSize + numThreads - 1) / numThreads;

    std::vector<std::future<std::uintptr_t>> futures;
    for (size_t i = 0; i < numThreads; ++i)
    {
        std::uintptr_t chunkOffset = i * chunkSize;
        futures.push_back(std::async(std::launch::async, [file, patternBytes, chunkOffset, chunkSize]() {
            return Disk::FindPatternInChunk(file, patternBytes, chunkOffset, chunkSize);
            }));
    }

    for (auto& future : futures)
    {
        std::uintptr_t result = future.get();
        if (result != std::uintptr_t(-1))
        {
            return result;
        }
    }

    throw std::runtime_error("Pattern not found in file");
}


