#include "Transcript.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

Transcript::Transcript(const std::string &sequenceName, StrandPolarity polarity,
                       int startPosition)
    : sequenceName(sequenceName), polarity(polarity),
      startPosition(startPosition), currentIndex(startPosition)
{
    resize(1);
    currentPosition = begin();
}

bool Transcript::isReadInside(size_t position, StrandPolarity polarity) {
    return ((polarity == this->polarity) && (position >= startPosition)
            && (position < startPosition + size()));
}

void Transcript::advance(size_t index) {
    if (index < startPosition)
        abort();
    if ((index - startPosition) >= size()) {
        size_t toBeAdded = index - startPosition - size() + 1;
        insert(end(), toBeAdded, Base());
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

