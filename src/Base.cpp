#include "Base.h"

void Base::addMatch(int quality) {
    reads++;
    matches++;
    this->quality += quality;
}

int Base::getMatches(void) {
    return matches;
}

void Base::addMismatch(int quality) {
    reads++;
    mismatches++;
    this->quality += quality;
}

int Base::getMismatches(void) {
    return mismatches;
}

void Base::addInsertion(void) {
    insertions++;
}

int Base::getInsertions(void) {
    return insertions;
}

void Base::addDeletion(void) {
    reads++;
    deletions++;
}

int Base::getDeletions(void) {
    return deletions;
}

int Base::getReads(void) {
    return reads;
}

int Base::getQuality(void) {
    return quality;
}

