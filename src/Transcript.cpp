#include "Transcript.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

Transcript::Transcript(const std::string &sequenceName, StrandPolarity polarity,
                       int startPosition)
    : sequenceName(sequenceName), polarity(polarity),
      startPosition(startPosition), length(1),
      currentIndex(startPosition), reads(1, 0), matches(1, 0),
      mismatches(1, 0), insertions(1, 0),
      deletions(1, 0), quality(1, 0)
{
    std::cout << "create transcript: " << startPosition << std::endl;
    currentPositionReads = reads.begin();
    currentPositionMatches = matches.begin();
    currentPositionMismatches = mismatches.begin();
    currentPositionInsertions = insertions.begin();
    currentPositionDeletions = deletions.begin();
    currentPositionQuality = quality.begin();
}

bool Transcript::isReadInside(size_t position, StrandPolarity polarity) {
    return ((polarity == this->polarity) && (position >= startPosition)
            && (position < startPosition + length));
}

void Transcript::addRead(size_t position, size_t length) {
    size_t end1 = startPosition + this->length;
    size_t end2 = position + length;
    if (end2 < end1)
        return;

    size_t back = end2 - end1;
    // ssize_t back = (position + length) - (startPosition + this->length);

    reads.insert(reads.end(), back, 0);
    matches.insert(matches.end(), back, 0);
    mismatches.insert(mismatches.end(), back, 0);
    insertions.insert(insertions.end(), back, 0);
    deletions.insert(deletions.end(), back, 0);
    quality.insert(quality.end(), back, 0);
    this->length += back;
}

void Transcript::advance(size_t index) {
    if (index < startPosition)
        abort();
    if ((index - startPosition) >= reads.size()) {
        size_t toBeAdded = index - startPosition - reads.size() + 1;
        reads.insert(reads.end(), toBeAdded, 0);
        matches.insert(matches.end(), toBeAdded, 0);
        mismatches.insert(mismatches.end(), toBeAdded, 0);
        deletions.insert(deletions.end(), toBeAdded, 0);
        insertions.insert(insertions.end(), toBeAdded, 0);
        quality.insert(quality.end(), toBeAdded, 0);
        length = reads.size();
    }
    ssize_t mov = (ssize_t) index - (ssize_t) currentIndex;
    std::advance(currentPositionReads, mov);
    std::advance(currentPositionMatches, mov);
    std::advance(currentPositionMismatches, mov);
    std::advance(currentPositionInsertions, mov);
    std::advance(currentPositionDeletions, mov);
    std::advance(currentPositionQuality, mov);
    currentIndex = index;
}

void Transcript::addMatch(size_t index, uint16_t quality) {
    advance(index);
    (*currentPositionReads)++;
    (*currentPositionMatches)++;
    (*currentPositionQuality) += quality;
}

void Transcript::addMismatch(size_t index, uint16_t quality) {
    advance(index);
    (*currentPositionReads)++;
    (*currentPositionMismatches)++;
    (*currentPositionQuality) += quality;
}

void Transcript::addInsertion(size_t index, uint16_t quality) {
    advance(index);
    (*currentPositionInsertions)++;
    // (*currentPositionQuality) += quality;
}

void Transcript::addDeletion(size_t index) {
    advance(index);
    (*currentPositionReads)++;
    (*currentPositionDeletions)++;
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
    auto itReads = reads.begin();
    auto itMatches = matches.begin();
    auto itMismatches = mismatches.begin();
    auto itInsertions = insertions.begin();
    auto itDeletions = deletions.begin();
    auto itQuality = quality.begin();
    while ((itReads != reads.end()) && (itMatches != matches.end())
            && (itMismatches != mismatches.end()) && (itInsertions != insertions.end())
            && (itDeletions != deletions.end())) {
        if (*itReads) {
            file << sequenceName << " "
                 << i+1
                 << std::setw(4)  << *itReads
                 << std::setw(10) << (double) *itQuality / *itReads
                 << std::setw(10) << (double) *itMismatches / *itReads
                 << std::setw(10) << (double) *itInsertions / *itReads
                 << std::setw(10) << (double) *itDeletions / *itReads << std::endl;
        }
        itReads++;
        itMatches++;
        itMismatches++;
        itInsertions++;
        itDeletions++;
        itQuality++;
        i++;
    }

    return 0;
}

