#include "BamRead.h"

#include <iostream>

int BamRead::initFromBamBlock(const char *block, size_t length)
{
    startPosition = *(int32_t*)(block + 4);
    refId = *(int32_t*)(block + 0);
    uint8_t lReadName = *(uint8_t*)(block + 8);
    uint16_t nCigarOp = *(uint16_t*)(block + 12);
    uint32_t lSeq = *(uint16_t*)(block + 16);
    size_t namePos = 32;
    size_t cigarPos = namePos + lReadName * sizeof(uint8_t);
    size_t seqPos = cigarPos + nCigarOp * sizeof(uint32_t);
    size_t qualPos = seqPos + ((lSeq + 1) / 2) * sizeof(uint8_t);

    // name (name is null terminated
    if (block[32 + lReadName - 1] != '\0') {
        std::cout << "[Error]: alignment block is corrupted" << std::endl;
        return -1;
    }
    name = std::string(&(block[32]), lReadName - 1);

    // cigar
    for (size_t i = 0; i < nCigarOp; i++) {
        uint32_t value = *(uint32_t*)(block + cigarPos + i * sizeof(uint32_t));
        uint32_t opLen = value >> 4;
        char op;
        switch (value & 0x0000000F) {
            case 0:
                op = 'M';
                break;
            case 1:
                op = 'I';
                break;
            case 2:
                op = 'D';
                break;
            case 3:
                op = 'N';
                break;
            case 4:
                op = 'S';
                break;
            case 5:
                op = 'H';
                break;
            case 6:
                op = 'P';
                break;
            case 7:
                op = '=';
                break;
            case 8:
                op = 'X';
                break;
            default:
                std::cout << "[Error]: alignment block is corrupted"
                          << std::endl;
                return -1;
        }
        for (size_t j = 0; j < opLen; j++) {
            cigar.push_back(op);
        }
    }

    // seq
    for (size_t i = 0; i < (lSeq + 1) / 2; i++) {
        uint8_t value = *(uint8_t*)(block + seqPos + i * sizeof(uint8_t));
        uint8_t values[2];
        values[0] = value >> 4;
        values[1] = value & 0x0F;
        char bases[2];
        for (size_t i = 0; i < 2; i++) {
            switch (values[i]) {
                case 0:
                    bases[i] = '=';
                    break;
                case 1:
                    bases[i] = 'A';
                    break;
                case 2:
                    bases[i] = 'C';
                    break;
                case 3:
                    bases[i] = 'M';
                    break;
                case 4:
                    bases[i] = 'G';
                    break;
                case 5:
                    bases[i] = 'R';
                    break;
                case 6:
                    bases[i] = 'S';
                    break;
                case 7:
                    bases[i] = 'V';
                    break;
                case 8:
                    bases[i] = 'T';
                    break;
                case 9:
                    bases[i] = 'W';
                    break;
                case 10:
                    bases[i] = 'Y';
                    break;
                case 11:
                    bases[i] = 'H';
                    break;
                case 12:
                    bases[i] = 'K';
                    break;
                case 13:
                    bases[i] = 'D';
                    break;
                case 14:
                    bases[i] = 'B';
                    break;
                case 15:
                    bases[i] = 'N';
                    break;
            default:
                std::cout << "[Error]: alignment block is corrupted"
                          << std::endl;
                return -1;
            }
        alignedSequence.push_back(bases[i]);
        }
    }

    for (size_t i = 0; i < lSeq; i++) {
        uint8_t qual = *(uint8_t*)(block + qualPos + i * sizeof(uint8_t));
        quality.push_back(qual);
    }

    return 0;
}

void BamRead::setSequenceName(const std::vector<std::string> &sequences) {
    if ((refId != -1) && (refId < sequences.size())) {
        sequenceName = sequences[refId];
    }
}

std::string BamRead::getSequenceName(void) {
    return sequenceName;
}

std::string BamRead::getAlignedSequence(void) {
    return alignedSequence;
}

size_t BamRead::getStartPosition(void) {
    return startPosition;
}

std::string BamRead::getName(void) const {
    return name;
}

std::string BamRead::getCigar(void) const {
    return cigar;
}

