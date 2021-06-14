#include "Analysis.h"

Analysis::Analysis(const std::string &sequenceName, const std::string &sequence)
    : name(sequenceName), sequence(sequence), reads(sequence.size()),
      matches(sequence.size()), mismatches(sequence.size()),
      insertions(sequence.size()), deletions(sequence.size())
{
    /* Nothing to do */
}

void Analysis::addMatch(size_t position) {
    if (position < matches.size()) {
        reads[position]++;
        matches[position]++;
    }
}

void Analysis::addMismatch(size_t position) {
    if (position < mismatches.size()) {
        reads[position]++;
        mismatches[position]++;
    }
}

void Analysis::addInsertion(size_t position) {
    if (position < insertions.size()) {
        reads[position]++;
        insertions[position]++;
    }
}

void Analysis::addDeletion(size_t position) {
    if (position < deletions.size()) {
        reads[position]++;
        deletions[position]++;
    }
}

