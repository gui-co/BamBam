#pragma once

#include "FastaReader.h"
#include "BamReader.h"

class Analyzer {

public:
    void setFastaReader(FastaReader *reader);
    void setBamReader(BamReader *reader);
    void analyze(void);

private:
    FastaReader *fastaReader;
    BamReader *bamReader;
};

