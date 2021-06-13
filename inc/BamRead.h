#pragma once

#include <string>
#include <vector>
#include <utility>

class BamRead {

public:
    int initFromBamBlock(const char *block, size_t length);
    void setSequenceName(const std::vector<std::string> &sequences);
    std::string getSequenceName(void);
    std::string getName(void) const;
    std::string getCigar(void) const;

private:
    int32_t refId;
    std::string sequenceName;
    std::string name;
    std::string cigar;
    std::string sequence;
    std::vector<int> quality;

};

