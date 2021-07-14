#pragma once

#include "FastaReader.h"
#include "BamReader.h"
#include "Sequence.h"

#include <map>
#include <string>
#include <list>
#include <queue>
#include <mutex>
#include <condition_variable>

class Analyzer {

public:
    void setFastaReader(FastaReader *reader);
    void setBamReader(BamReader *reader);
    void analyze(void);
    Sequence takeLastSequence(void);

private:
    Sequence currentSequence;
    FastaReader *fastaReader;
    BamReader *bamReader;
    std::queue<Sequence> sequencesReady;
    std::mutex queueMutex;
    std::condition_variable queueNotEmpty;

};

