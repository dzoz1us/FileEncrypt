#pragma once
#include <cstdint>
#include <vector>

class FileHeader {
public:
    static constexpr uint32_t MAGIC = 0x434E4546;
    static constexpr uint16_t VERSION = 0x0001;
    static constexpr size_t HEADER_SIZE = 38;

    std::vector<uint8_t> pack(const std::vector<uint8_t>& salt,
                              const std::vector<uint8_t>& iv);
    bool unpack(const std::vector<uint8_t>& rawHeader,
                std::vector<uint8_t>& salt,
                std::vector<uint8_t>& iv);
    static bool isValid(const std::vector<uint8_t>& rawHeader);
};