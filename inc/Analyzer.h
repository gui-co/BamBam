#pragma once

#include "FastaReader.h"
#include "BamReader.h"
#include "Analysis.h"

#include <map>

class Analyzer {

public:
    void setFastaReader(FastaReader *reader);
    void setBamReader(BamReader *reader);
    void analyze(void);

private:
    FastaReader *fastaReader;
    BamReader *bamReader;
    std::map<std::string, Analysis*> results;
};

