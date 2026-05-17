#include "FileHeader.h"
#include <cstring>

std::vector<uint8_t> FileHeader::pack(const std::vector<uint8_t>& salt,
                                       const std::vector<uint8_t>& iv) {
    std::vector<uint8_t> header(HEADER_SIZE);
    size_t off = 0;
    uint32_t magic = MAGIC;
    std::memcpy(header.data() + off, &magic, 4); off += 4;
    uint16_t ver = VERSION;
    std::memcpy(header.data() + off, &ver, 2); off += 2;
    std::memcpy(header.data() + off, salt.data(), 16); off += 16;
    std::memcpy(header.data() + off, iv.data(), 16);
    return header;
}

bool FileHeader::unpack(const std::vector<uint8_t>& raw,
                         std::vector<uint8_t>& salt,
                         std::vector<uint8_t>& iv) {
    if (!isValid(raw)) return false;
    salt.assign(raw.begin() + 6, raw.begin() + 22);
    iv.assign(raw.begin() + 22, raw.begin() + 38);
    return true;
}

bool FileHeader::isValid(const std::vector<uint8_t>& raw) {
    if (raw.size() < HEADER_SIZE) return false;
    uint32_t magic;
    std::memcpy(&magic, raw.data(), 4);
    return magic == MAGIC;
}