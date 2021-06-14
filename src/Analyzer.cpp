#include "Analyzer.h"

#include "BamRead.h"

#include <iostream>

Analyzer::~Analyzer(void) {
    for (auto i = results.begin(); i != results.end(); i++) {
        delete i->second;
        i->second = nullptr;
    }
}

void Analyzer::setFastaReader(FastaReader *reader) {
    fastaReader = reader;
}

void Analyzer::setBamReader(BamReader *reader) {
    bamReader = reader;
}

void Analyzer::analyze(void) {
    if (fastaReader == nullptr) {
        std::cout << "[Error]: please provide a valid fasta reader"
                  << std::endl;
        return;
    }
    if (bamReader == nullptr) {
        std::cout << "[Error]: please provide a valid bam reader" << std::endl;
        return;
    }

    BamRead read = bamReader->getNextRead();
    while (read.getName() != "") {
        sequence = fastaReader.getSequence(read.getSequenceName());
        if (sequence == "") {
            std::cout << "[Error]: sequence not found in fasta file"
                      << std::endl;
            continue;
        }
        read = bamReader->getNextRead();
    }
}

