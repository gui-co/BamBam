#pragma once

#include "Base.h"

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
    bool isReadInside(size_t position, StrandPolarity polarity);
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
    std::list<Base> data;
    size_t currentIndex;
    std::list<Base>::iterator currentPosition;
    void advance(size_t index);
};

