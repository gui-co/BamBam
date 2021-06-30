#pragma once

#include <string>
#include <vector>
#include <utility>

class BamRead {

public:
    int initFromBamBlock(const char *block, size_t length);
    void setSequenceName(const std::vector<std::string> &sequences);
    std::string getSequenceName(void);
    std::string getAlignedSequence(void);
    size_t getStartPosition(void);
    size_t getLength(void);
    uint8_t getFlag(void);
    std::string getName(void) const;
    std::string getCigar(void) const;

private:
    int32_t refId;
    size_t startPosition;
    std::string sequenceName;
    std::string name;
    uint16_t flag;
    std::string cigar;
    std::string alignedSequence;
    std::vector<int> quality;
    int alignmentLength;

};

