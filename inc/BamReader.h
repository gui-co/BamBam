#pragma once

#include "BamRead.h"

#include <string>
#include <fstream>
#include <vector>

class BamReader {

public:
    int setBamFile(const std::string &filename);
    BamRead getNextRead(void);

private:
    std::ifstream file;
    std::vector<uint8_t> currentBlock;
    std::vector<std::string> sequences;
    size_t bamPosition;
    int inflateNextBlock(void);
    size_t readBam(char *buffer, size_t count);

};

