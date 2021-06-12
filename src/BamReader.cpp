#include "BamReader.h"

#include <zlib.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

int BamReader::setBamFile(const std::string &filename) {
    file = std::ifstream(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "[Error]: Unable to open " << filename << std::endl;
        return -1;
    }
    char magic[4];
    readBam(reinterpret_cast<char*>(magic), 4);
    if (strncmp(magic, "BAM\1", 4) != 0) {
        std::cout << "[Error]: " << filename << " is not a BAM file"
                  << std::endl;
        return -1;
    }

    // header
    uint32_t lHeader;
    readBam(reinterpret_cast<char*>(&lHeader), sizeof(lHeader));
    char header[lHeader];
    readBam(reinterpret_cast<char*>(header), lHeader);

    // sequences
    uint32_t nSeq;
    readBam(reinterpret_cast<char*>(&nSeq), sizeof(nSeq));
    for (int i = 0; i < nSeq; i++) {
        uint32_t lName;
        readBam(reinterpret_cast<char*>(&lName), sizeof(lName));
        char name[lName];
        readBam(reinterpret_cast<char*>(name), lName);
        if (name[lName - 1] != '\0') {
            std::cout << "[Error]: header of " << filename << " is not "
                      << "correclty formatted" << std::endl;
            return -1;
        }
        sequences.push_back(std::string(name));
        uint32_t lRef;
        readBam(reinterpret_cast<char*>(&lRef), sizeof(lRef));
    }

    return 0;
}

int BamReader::inflateNextBlock(void) {
    // header
    uint8_t id1;
    file.read(reinterpret_cast<char*>(&id1), sizeof(id1));
    if (id1 != 31)
        return -1;
    uint8_t id2;
    file.read(reinterpret_cast<char*>(&id2), sizeof(id2));
    if (id2 != 139)
        return -1;
    uint8_t cm;
    file.read(reinterpret_cast<char*>(&cm), sizeof(cm));
    if (cm != 8)
        return -1;
    uint8_t flg;
    file.read(reinterpret_cast<char*>(&flg), sizeof(flg));
    if (flg != 4)
        return -1;
    file.seekg(6, std::ios_base::cur);
    uint16_t xlen;
    file.read(reinterpret_cast<char*>(&xlen), sizeof(xlen));
    uint8_t si1;
    file.read(reinterpret_cast<char*>(&si1), sizeof(si1));
    if (si1 != 66)
        return -1;
    uint8_t si2;
    file.read(reinterpret_cast<char*>(&si2), sizeof(si2));
    if (si2 != 67)
        return -1;
    uint16_t slen;
    file.read(reinterpret_cast<char*>(&slen), sizeof(slen));
    if (slen != 2)
        return -1;
    uint16_t bsize;
    file.read(reinterpret_cast<char*>(&bsize), sizeof(bsize));
    file.seekg(xlen - 6, std::ios_base::cur);

    // data
    uint32_t cDataLen = bsize - xlen - 19;
    uint8_t cData[cDataLen];
    file.read(reinterpret_cast<char*>(cData), cDataLen);
    uint32_t crc32;
    file.read(reinterpret_cast<char*>(&crc32), sizeof(crc32));
    uint32_t dataLen;
    file.read(reinterpret_cast<char*>(&dataLen), sizeof(dataLen));
    currentBlock.resize(dataLen);

    // inflate
    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.avail_in = 0;
    infstream.next_in = Z_NULL;
    int ret = inflateInit2(&infstream, -15);
    if (ret != Z_OK) {
        std::cout << "[Error]: unable to initialize zlib (return value: "
                  << ret << ", error message: " << infstream.msg << ")"
                  << std::endl;
        return ret;
    }
    infstream.avail_in = cDataLen;
    infstream.next_in = cData;
    infstream.avail_out = dataLen;
    infstream.next_out = currentBlock.data();
    ret = inflate(&infstream, Z_NO_FLUSH);
    if (ret != Z_STREAM_END  && ret != Z_OK) {
        std::cout << "[Error]: unable to inflate data (return value: " << ret
                  << ", error message: " << infstream.msg << ")" << std::endl;
        inflateEnd(&infstream);
        return -1;
    }
    inflateEnd(&infstream);
    bamPosition = 0;

    return 0;
}

size_t BamReader::readBam(char *buffer, size_t count) {
    size_t i = 0;
    while (i < count) {
        if (bamPosition >= currentBlock.size()) {
            int ret = inflateNextBlock();
            if (ret != 0) {
                return i;
            }
            if (currentBlock.size() == 0) {
                return i;
            }
        }
        buffer[i] = currentBlock[bamPosition];
        bamPosition++;
        i++;
    }
    return i;
}

