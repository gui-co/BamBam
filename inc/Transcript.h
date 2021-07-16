#pragma once

#include "Base.h"

#include <string>
#include <list>
#include <fstream>

class Transcript : public std::list<Base> {

public:
    enum StrandPolarity {
        PLUS_STRAND,
        MINUS_STRAND,
    };

    Transcript(const std::string &SequenceName, StrandPolarity polarity,
               int startPosition);
    bool isReadInside(size_t position, StrandPolarity polarity);
    void addMatch(size_t position, uint16_t quality);
    void addMismatch(size_t position, uint16_t quality);
    void addInsertion(size_t position);
    void addDeletion(size_t position);
    bool isPlus(void);
    size_t startPosition;

private:
    std::string sequenceName;
    StrandPolarity polarity;
    size_t currentIndex;
    std::list<Base>::iterator currentPosition;
    void advance(size_t index);
};

