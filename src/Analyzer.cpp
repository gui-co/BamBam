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
    while (!read.getName().empty()) {
        std::string sequenceName = read.getSequenceName();
        std::string refSeq = fastaReader->getSequence(sequenceName);
        if (refSeq == "") {
            std::cout << "[ERROR] sequence \"" << sequenceName << "\" not "
                         "found in fasta file" << std::endl;
            continue;
        }

        Analysis *analysis;
        if (results.find(sequenceName) == results.end()) {
            results[sequenceName] = new Analysis(sequenceName, refSeq);
        }
        analysis = results.find(sequenceName)->second;

        uint8_t flag = read.getFlag();
        Analysis::StrandPolarity polarity;
        if (flag & 0x10) {
            polarity = Analysis::PLUS_STRAND;
        } else {
            polarity = Analysis::MINUS_STRAND;
        }
        size_t refPos = read.getStartPosition();
        size_t alnPos = 0;
        std::string alnSeq = read.getAlignedSequence();
        std::string cigar = read.getCigar();
        for (size_t i = 0; i < cigar.size(); i++) {
            switch (cigar[i]) {
                case 'M':
                    if (alnSeq[alnPos] == refSeq[refPos]) {
                        analysis->addMatch(refPos, polarity);
                    } else {
                        analysis->addMismatch(refPos, polarity);
                    }
                    refPos++;
                    alnPos++;
                    break;
                case 'I':
                    if (refPos > 0) {
                        analysis->addInsertion(refPos - 1, polarity);
                    }
                    alnPos++;
                    break;
                case 'D':
                    analysis->addDeletion(refPos, polarity);
                    refPos++;
                    break;
                case 'N':
                    analysis->addDeletion(refPos, polarity);
                    refPos++;
                    break;
                case 'S':
                    alnPos++;
                    break;
                case 'H':
                    break;
                case 'P':
                    break;
                case '=':
                    analysis->addMatch(refPos, polarity);
                    refPos++;
                    alnPos++;
                    break;
                case 'X':
                    analysis->addMismatch(refPos, polarity);
                    refPos++;
                    alnPos++;
                    break;
            }
        }

        read = bamReader->getNextRead();
    }
}

void Analyzer::exportData(const std::string &directory) {
    for (auto it = results.begin(); it != results.end(); it++) {
        std::ofstream filePlus(directory + it->first + "_plus.data");
        std::ofstream fileMinus(directory + it->first +"_minus.data");
        it->second->exportToFile(filePlus, fileMinus);
    }
}

