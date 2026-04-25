#include "FileHeader.h"
#include <cstring>
#include <stdexcept>

std::vector<uint8_t> FileHeader::pack(const std::vector<uint8_t>& salt,
                                       const std::vector<uint8_t>& iv) {
    std::vector<uint8_t> header(HEADER_SIZE);
    size_t offset = 0;

    // Magic "FENC"
    uint32_t magic = MAGIC;
    std::memcpy(header.data() + offset, &magic, sizeof(magic));
    offset += sizeof(magic);

    // Version
    uint16_t version = VERSION;
    std::memcpy(header.data() + offset, &version, sizeof(version));
    offset += sizeof(version);

    // Salt
    std::memcpy(header.data() + offset, salt.data(), 16);
    offset += 16;

    // IV
    std::memcpy(header.data() + offset, iv.data(), 16);

    return header;
}

bool FileHeader::unpack(const std::vector<uint8_t>& rawHeader,
                         std::vector<uint8_t>& salt,
                         std::vector<uint8_t>& iv) {
    if (!isValid(rawHeader)) return false;

    salt.assign(rawHeader.begin() + 6, rawHeader.begin() + 22);
    iv.assign(rawHeader.begin() + 22, rawHeader.begin() + 38);
    return true;
}

bool FileHeader::isValid(const std::vector<uint8_t>& rawHeader) {
    if (rawHeader.size() < HEADER_SIZE) return false;
    uint32_t magic;
    std::memcpy(&magic, rawHeader.data(), sizeof(magic));
    return magic == MAGIC;
}