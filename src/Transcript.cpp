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

std::list<Base>::iterator Transcript::begin(void) {
    return data.begin();
}

std::list<Base>::iterator Transcript::end(void) {
    return data.end();
}

