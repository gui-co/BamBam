#include "Analyzer.h"

#include "BamRead.h"
#include "Transcript.h"

#include <iostream>

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
            read = bamReader->getNextRead();
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

        if (currentSequence.getName() == "") {
            currentSequence = Sequence(sequenceName, refSeq);
        } else if (currentSequence.getName() != sequenceName) {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                sequencesReady.push(Sequence(std::move(currentSequence)));
            }
            queueNotEmpty.notify_one();
            currentSequence = Sequence(sequenceName, refSeq);
        }

        Transcript &transcript = currentSequence.getTranscript(
                polarity, readStartPosition);

        size_t refPos = readStartPosition;
        size_t alnPos = 0;
        std::string alnSeq = read.getAlignedSequence();
        std::string cigar = read.getCigar();
        std::vector<int> qual = read.getQuality();
        for (size_t i = 0; i < cigar.size(); i++) {
            switch (cigar[i]) {
                case 'M':
                    if (alnSeq[alnPos] == refSeq[refPos]) {
                        transcript.addMatch(refPos, qual[alnPos]);
                    } else {
                        transcript.addMismatch(refPos, qual[alnPos]);
                    }
                    refPos++;
                    alnPos++;
                    break;
                case 'I':
                    transcript.addInsertion(refPos);
                    alnPos++;
                    break;
                case 'D':
                    transcript.addDeletion(refPos);
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
                    transcript.addMatch(refPos, qual[alnPos]);
                    refPos++;
                    alnPos++;
                    break;
                case 'X':
                    transcript.addMismatch(refPos, qual[alnPos]);
                    refPos++;
                    alnPos++;
                    break;
            }
        }

        read = bamReader->getNextRead();
    }

    // push empty sequence when done
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        sequencesReady.push(Sequence(std::move(currentSequence)));
    }
    queueNotEmpty.notify_one();
    currentSequence = Sequence();
}

Sequence Analyzer::takeLastSequence(void) {
    std::unique_lock<std::mutex> lock(queueMutex);
    while (sequencesReady.empty())
        queueNotEmpty.wait(lock);
    Sequence sequence = std::move(sequencesReady.front());
    sequencesReady.pop();
    return sequence;
}

