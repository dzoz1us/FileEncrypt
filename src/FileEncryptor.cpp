#include "FileEncryptor.h"
#include "CryptoEngine.h"
#include "FileHeader.h"
#include "FileHandler.h"
#include <iostream>
#include <cstdio>

bool FileEncryptor::encrypt(const std::string& src, const std::string& dst,
                             const std::string& pwd) {
    return processFile(src, dst, pwd, Direction::Encrypt);
}

bool FileEncryptor::decrypt(const std::string& src, const std::string& dst,
                             const std::string& pwd) {
    return processFile(src, dst, pwd, Direction::Decrypt);
}

bool FileEncryptor::processFile(const std::string& srcPath,
                                 const std::string& dstPath,
                                 const std::string& password,
                                 Direction direction) {
    try {
        CryptoEngine engine;
        FileHeader header;
        FileHandler srcFile, dstFile;

        if (!srcFile.openRead(srcPath)) {
            std::cerr << "Error: Cannot open source file: " << srcPath << std::endl;
            return false;
        }

        std::string tmpPath = dstPath + ".tmp";
        if (!dstFile.openWrite(tmpPath)) {
            std::cerr << "Error: Cannot create output file" << std::endl;
            return false;
        }

        std::vector<uint8_t> salt, iv;
        if (direction == Direction::Encrypt) {
            salt = engine.generateSalt();
            iv = engine.generateIV();
            dstFile.writeChunk(header.pack(salt, iv));
        } else {
            auto rawHeader = srcFile.readChunk(FileHeader::HEADER_SIZE);
            if (!header.unpack(rawHeader, salt, iv)) {
                std::cerr << "Error: Invalid file format." << std::endl;
                dstFile.close();
                std::remove(tmpPath.c_str());
                return false;
            }
        }

        auto key = engine.deriveKey(password, salt);

        if (direction == Direction::Encrypt)
            engine.initEncrypt(key, iv);
        else
            engine.initDecrypt(key, iv);

        uint64_t total = srcFile.fileSize();
        uint64_t done = 0;
        std::vector<uint8_t> chunk;

        while (!(chunk = srcFile.readChunk(CHUNK_SIZE)).empty()) {
            std::vector<uint8_t> processed;
            engine.update(chunk, processed);
            dstFile.writeChunk(processed);
            done += chunk.size();
            int pct = static_cast<int>(done * 100 / total);
            std::cout << "\rProcessing... " << pct << "%" << std::flush;
        }

        std::vector<uint8_t> finalBlock;
        if (!engine.finalize(finalBlock)) {
            std::cerr << "\nError: Wrong password or corrupted file." << std::endl;
            dstFile.close();
            std::remove(tmpPath.c_str());
            return false;
        }
        dstFile.writeChunk(finalBlock);
        std::cout << "\rProcessing... 100%" << std::endl;

        engine.cleanup();
        srcFile.close();
        dstFile.close();

        std::remove(dstPath.c_str());
        std::rename(tmpPath.c_str(), dstPath.c_str());
        std::cout << "Done! Saved as: " << dstPath << std::endl;
        return true;

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return false;
    }
}