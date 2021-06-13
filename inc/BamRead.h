#pragma once

#include <string>
#include <vector>
#include <utility>

class BamRead {

public:
    int initFromBamBlock(const char *block, size_t length);
    std::string getName(void) const;

private:
    std::string name;
    std::vector<std::pair<uint32_t, char>> cigar;
    std::string sequence;

};

