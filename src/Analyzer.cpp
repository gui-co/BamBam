#include "Analyzer.h"

#include "BamRead.h"
#include "Transcript.h"

#include <iostream>

Analyzer::~Analyzer(void) {
    for (auto i = results.begin(); i != results.end(); i++) {
        for (auto j = i->second.begin(); j != i->second.end(); j++) {
            delete *j;
            *j = nullptr;
        }
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

        uint8_t flag = read.getFlag();
        // ignore reads with these flag values
        if (flag & 0x0004 || flag & 0x0200 || flag & 0x0400 || flag & 0x0800) {
            read = bamReader->getNextRead();
            continue;
        }
        // Get strand polarity
        Transcript::StrandPolarity polarity;
        if (flag & 0x0010) {
            polarity = Transcript::MINUS_STRAND;
        } else {
            polarity = Transcript::PLUS_STRAND;
        }
        size_t readStartPosition = read.getStartPosition();

        Transcript *transcript = findTranscript(sequenceName, polarity,
                                                readStartPosition);

        size_t refPos = readStartPosition;
        size_t alnPos = 0;
        std::string alnSeq = read.getAlignedSequence();
        std::string cigar = read.getCigar();
        std::vector<int> qual = read.getQuality();
        for (size_t i = 0; i < cigar.size(); i++) {
            switch (cigar[i]) {
                case 'M':
                    if (alnSeq[alnPos] == refSeq[refPos]) {
                        transcript->addMatch(refPos);
                    } else {
                        transcript->addMismatch(refPos);
                    }
                    refPos++;
                    alnPos++;
                    break;
                case 'I':
                    if (refPos > 0) {
                        transcript->addInsertion(refPos - 1);
                    }
                    alnPos++;
                    break;
                case 'D':
                    transcript->addDeletion(refPos);
                    refPos++;
                    break;
                case 'N':
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
                    transcript->addMatch(refPos);
                    refPos++;
                    alnPos++;
                    break;
                case 'X':
                    transcript->addMismatch(refPos);
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
        for (auto jt = it->second.begin(); jt != it->second.end(); jt++) {
            if ((*jt)->isPlus())
                (*jt)->exportToFile(filePlus);
            else
                (*jt)->exportToFile(fileMinus);
        }
    }
}

Transcript *Analyzer::findTranscript(std::string sequenceName,
                                     Transcript::StrandPolarity polarity,
                                     size_t pos) {
    Transcript *transcript = nullptr;

    // Try to find the corresponding transcript
    auto sequenceIt = results.find(sequenceName);
    if (sequenceIt != results.end()) {
        auto transcriptIt = sequenceIt->second.begin();
        while ((!transcript) && (transcriptIt != sequenceIt->second.end())) {
            if ((*transcriptIt)->isReadInside(pos, polarity))
                transcript = *transcriptIt;
            transcriptIt++;
        }
    }
    // Create a new one if not found
    if (!transcript) {
        transcript = new Transcript(sequenceName, polarity, pos);
        if (sequenceIt == results.end()) {
            results[sequenceName] = std::list<Transcript*>();
        }
        sequenceIt = results.find(sequenceName);
        sequenceIt->second.push_back(transcript);
    }
    return transcript;
}

