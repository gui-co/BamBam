#include "Transcript.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

Transcript::Transcript(const std::string &sequenceName, StrandPolarity polarity,
                       int startPosition)
    : sequenceName(sequenceName), polarity(polarity),
      startPosition(startPosition), length(1),
      currentIndex(startPosition), data(1)
{
    currentPosition = data.begin();
}

bool Transcript::isReadInside(size_t position, StrandPolarity polarity) {
    return ((polarity == this->polarity) && (position >= startPosition)
            && (position < startPosition + length));
}

void Transcript::advance(size_t index) {
    if (index < startPosition)
        abort();
    if ((index - startPosition) >= data.size()) {
        size_t toBeAdded = index - startPosition - data.size() + 1;
        data.insert(data.end(), toBeAdded, Base());
        length = data.size();
    }
    ssize_t mov = (ssize_t) index - (ssize_t) currentIndex;
    std::advance(currentPosition, mov);
    currentIndex = index;
}

void Transcript::addMatch(size_t index, uint16_t quality) {
    advance(index);
    (*currentPosition).addMatch(quality);
}

void Transcript::addMismatch(size_t index, uint16_t quality) {
    advance(index);
    (*currentPosition).addMismatch(quality);
}

void Transcript::addInsertion(size_t index) {
    advance(index);
    (*currentPosition).addInsertion();
}

void Transcript::addDeletion(size_t index) {
    advance(index);
    (*currentPosition).addDeletion();
}

bool Transcript::isPlus(void) {
    return polarity == PLUS_STRAND;
}

int Transcript::exportToFile(std::ofstream &file) {
    if (!file.is_open()) {
        std::cout << "[ERROR] unable to export analysis of sequence \"" 
                  << sequenceName << "\"" << std::endl;
        return -1;
    }

    file << std::fixed << std::setprecision(5);

    size_t i = startPosition;
    auto it = data.begin();
    while (it != data.end()) {
        if ((*it).getReads() != 0) {
            file << sequenceName << " "
                 << i+1
                 << std::setw(4)  << (*it).getReads()
                 << std::setw(10) << (double) (*it).getQuality()
                    / ((*it).getMatches() + (*it).getMismatches())
                 << std::setw(10) << (double) (*it).getMismatches()
                    / (*it).getReads()
                 << std::setw(10) << (double) (*it).getInsertions()
                    / (*it).getReads()
                 << std::setw(10) << (double) (*it).getDeletions()
                    / (*it).getReads()
                 << std::endl;
        }
        it++;
        i++;
    }

    return 0;
}

