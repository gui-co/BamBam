#pragma once

#include "BamRead.h"

#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

class BamReader {

public:
    int setBamFile(const std::string &filename);
    BamRead takeNextRead(void);
    BamRead getNextRead(void);

private:
    std::ifstream file;
    std::vector<uint8_t> currentBlock;
    std::vector<std::string> sequences;
    size_t bamPosition;
    int inflateNextBlock(void);
    size_t readBam(char *buffer, size_t count);
    void read(void);
    std::queue<BamRead> readsReady;
    std::mutex queueMutex;
    std::condition_variable queueNotEmpty;

};

