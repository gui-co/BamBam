#pragma once

#include <string>
#include <list>
#include <fstream>

class Transcript {

public:
    enum StrandPolarity {
        PLUS_STRAND,
        MINUS_STRAND,
    };

    Transcript(const std::string &SequenceName, StrandPolarity polarity,
               int startPosition);
    bool isReadInside(size_t position, StrandPolarity polarit);
    void addRead(size_t position, size_t length);
    void addMatch(size_t position, uint16_t quality);
    void addMismatch(size_t position, uint16_t quality);
    void addInsertion(size_t position);
    void addDeletion(size_t position);
    bool isPlus(void);
    int exportToFile(std::ofstream &file);
    size_t startPosition;
    size_t length;

private:
    std::string sequenceName;
    StrandPolarity polarity;
    std::list<uint16_t> reads;
    std::list<uint16_t> matches;
    std::list<uint16_t> mismatches;
    std::list<uint16_t> insertions;
    std::list<uint16_t> deletions;
    std::list<uint16_t> quality;
    size_t currentIndex;
    std::list<uint16_t>::iterator currentPositionReads;
    std::list<uint16_t>::iterator currentPositionMatches;
    std::list<uint16_t>::iterator currentPositionMismatches;
    std::list<uint16_t>::iterator currentPositionInsertions;
    std::list<uint16_t>::iterator currentPositionDeletions;
    std::list<uint16_t>::iterator currentPositionQuality;
    void advance(size_t index);
};

