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


std::uintptr_t Disk::FindPattern(FILE* file, const char* signature)
{
    if (!file)
        throw std::invalid_argument("Invalid file pointer");

    const auto patternBytes = PatternToByteVector(signature);
    const auto s = patternBytes.size();
    const auto d = patternBytes.data();
    std::vector<std::uint8_t> buffer(ReadInBufferSize);


    std::uintptr_t fileOffset = 0;

    while (!feof(file))
    {
        size_t bytesRead = fread(buffer.data(), 1, buffer.size(), file);
        if (bytesRead == 0)
            break;

        for (size_t i = 0; i < bytesRead - s; ++i)
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
                return fileOffset + i;
            }
        }

        fileOffset += bytesRead;
        fseek(file, fileOffset, SEEK_SET);
    }

    throw std::exception("Pattern not found in file");
}


