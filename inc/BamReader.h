#pragma once

#include <string>
#include <fstream>
#include <vector>

class BamReader {

public:
    int setBamFile(const std::string &filename);

private:
    std::ifstream file;
    std::vector<uint8_t> currentBlock;
    std::vector<std::string> sequences;
    std::streamsize bamPosition;
    int inflateNextBlock(void);
    void readBam(char *buffer, std::streamsize count);

};

