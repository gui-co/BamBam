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

BamRead BamReader::getNextRead(void) {
    size_t r;
    size_t size;
    bool success = true;
    size_t progress = 0;

    // blocksize
    uint32_t blockSize;
    size = sizeof(blockSize);
    r = readBam(reinterpret_cast<char*>(&blockSize), size);
    if (r != size) {
        // end of file
        return BamRead();
    }
    // refID
    uint32_t refId;
    size = sizeof(refId);
    r = readBam(reinterpret_cast<char*>(&refId), size);
    success &= (r == size);
    progress += size;
    // pos
    uint32_t pos;
    size = sizeof(pos);
    r = readBam(reinterpret_cast<char*>(&pos), size);
    success &= (r == size);
    progress += size;
    // l_read_name
    uint8_t lReadName;
    size = sizeof(lReadName);
    r = readBam(reinterpret_cast<char*>(&lReadName), size);
    success &= (r == size);
    progress += size;
    // mapq
    uint8_t mapq;
    size = sizeof(mapq);
    r = readBam(reinterpret_cast<char*>(&mapq), size);
    success &= (r == size);
    progress += size;
    // bin
    uint16_t bin;
    size = sizeof(bin);
    r = readBam(reinterpret_cast<char*>(&bin), size);
    success &= (r == size);
    progress += size;
    // n_cigar_op
    uint16_t nCigarOp;
    size = sizeof(nCigarOp);
    r = readBam(reinterpret_cast<char*>(&nCigarOp), size);
    success &= (r == size);
    progress += size;
    // flag
    uint16_t flag;
    size = sizeof(flag);
    r = readBam(reinterpret_cast<char*>(&flag), size);
    success &= (r == size);
    progress += size;
    // l_seq
    uint32_t lSeq;
    size = sizeof(lSeq);
    r = readBam(reinterpret_cast<char*>(&lSeq), size);
    success &= (r == size);
    progress += size;
    // next_refID
    uint32_t nextRefId;
    size = sizeof(nextRefId);
    r = readBam(reinterpret_cast<char*>(&nextRefId), size);
    success &= (r == size);
    progress += size;
    // next_pos
    uint32_t nextPos;
    size = sizeof(nextPos);
    r = readBam(reinterpret_cast<char*>(&nextPos), size);
    success &= (r == size);
    progress += size;
    // tlen
    uint32_t tLen;
    size = sizeof(tLen);
    r = readBam(reinterpret_cast<char*>(&tLen), size);
    success &= (r == size);
    progress += size;
    // read_name
    char readName[lReadName];
    size = lReadName * sizeof(char);
    r = readBam(reinterpret_cast<char*>(readName), size);
    success &= (r == size);
    progress += size;
    // cigar
    uint32_t cigar[nCigarOp];
    size = nCigarOp * sizeof(uint32_t);
    r = readBam(reinterpret_cast<char*>(cigar), size);
    success &= (r == size);
    progress += size;
    // seq
    uint8_t seq[(lSeq + 1) / 2];
    size = ((lSeq + 1) / 2) * sizeof(uint8_t);
    r = readBam(reinterpret_cast<char*>(seq), size);
    success &= (r == size);
    progress += size;
    char qual[lSeq];
    size = lSeq * sizeof(char);
    r = readBam(reinterpret_cast<char*>(qual), size);
    success &= (r == size);
    progress += size;
    // move to the end of the read
    uint32_t endSize = blockSize - progress;
    uint8_t dummy[endSize];
    r = readBam(reinterpret_cast<char*>(dummy), endSize);
    success &= (r == endSize);

    if (!success) {
        std::cout << "[Error]: alignment block is corrupted" << std::endl;
        return BamRead();
    }

    BamRead read;
    read.setName(readName, lReadName);
    read.setReadSequence(seq, (lSeq + 1) / 2);
    read.setCigar(cigar, nCigarOp);
    return read;
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

