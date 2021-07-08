#pragma once

#include "FastaReader.h"
#include "BamReader.h"
#include "Sequence.h"

#include <map>
#include <string>
#include <list>

class Analyzer {

public:
    ~Analyzer(void);
    void setFastaReader(FastaReader *reader);
    void setBamReader(BamReader *reader);
    void analyze(void);
    Sequence *getSequence(const std::string &sequenceName);
    void deleteSequence(const std::string &sequenceName);

private:
    FastaReader *fastaReader;
    BamReader *bamReader;
    std::map<std::string, Sequence*> results;
    std::string lastSequence;

};

