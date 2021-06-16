#pragma once

#include "FastaReader.h"
#include "BamReader.h"
#include "Analysis.h"

#include <map>
#include <string>

class Analyzer {

public:
    ~Analyzer(void);
    void setFastaReader(FastaReader *reader);
    void setBamReader(BamReader *reader);
    void analyze(void);
    void exportData(const std::string &directory);

private:
    FastaReader *fastaReader;
    BamReader *bamReader;
    std::map<std::string, Analysis*> results;

};

