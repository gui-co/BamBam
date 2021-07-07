#pragma once

#include "FastaReader.h"
#include "BamReader.h"
#include "Transcript.h"

#include <map>
#include <string>
#include <list>

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
    std::map<std::string, std::list<Transcript*> > results;
    Transcript *findTranscript(std::string sequenceName,
                               Transcript::StrandPolarity polarity, size_t pos);

};

