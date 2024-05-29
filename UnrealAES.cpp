#include "Disk.h"
#include "AesProvider.h"
#include <chrono>
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];

    auto start = std::chrono::high_resolution_clock::now();

    FILE* file = nullptr;
    errno_t err = fopen_s(&file, filename, "rb");
    if (err != 0) {
        perror("Error Opening File for Finding Pattern");
        return EXIT_FAILURE;
    }

    try {
        long long offset = Disk::FindPattern(file, "45 33 C9 48 8D 55 D0 48 3B C2 72 2A 48 8D 55 D0");
        offset -= 62;

        std::string aeskey = AesProvider::GetAESKey(file, offset);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "AES Key: " << aeskey << std::endl;
        std::cout << "Completed in " << duration << " ms" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    fclose(file);
    return EXIT_SUCCESS;
}
