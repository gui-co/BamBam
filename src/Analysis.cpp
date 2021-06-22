#include "Analysis.h"

#include <iostream>

Analysis::Analysis(const std::string &sequenceName, const std::string &sequence)
    : name(sequenceName), sequence(sequence),
      readsPlus(sequence.size()), readsMinus(sequence.size()),
      matchesPlus(sequence.size()), matchesMinus(sequence.size()),
      mismatchesPlus(sequence.size()), mismatchesMinus(sequence.size()),
      insertionsPlus(sequence.size()), insertionsMinus(sequence.size()),
      deletionsPlus(sequence.size()), deletionsMinus(sequence.size())
{
    /* Nothing to do */
}

void Analysis::addMatch(size_t position, StrandPolarity pol) {
    if (position < sequence.size()) {
        if (pol == PLUS_STRAND) {
            readsPlus[position]++;
            matchesPlus[position]++;
        } else {
            readsMinus[position]++;
            matchesMinus[position]++;
        }
    }
}

void Analysis::addMismatch(size_t position, StrandPolarity pol) {
    if (position < sequence.size()) {
        if (pol == PLUS_STRAND) {
            readsPlus[position]++;
            mismatchesPlus[position]++;
        } else {
            readsMinus[position]++;
            mismatchesMinus[position]++;
        }
    }
}

void Analysis::addInsertion(size_t position, StrandPolarity pol) {
    if (position < sequence.size()) {
        if (pol == PLUS_STRAND) {
            readsPlus[position]++;
            insertionsMinus[position]++;
        } else {
            readsPlus[position]++;
            insertionsMinus[position]++;
        }
    }
}

void Analysis::addDeletion(size_t position, StrandPolarity pol) {
    if (position < sequence.size()) {
        if (pol == PLUS_STRAND) {
            readsPlus[position]++;
            deletionsPlus[position]++;
        } else {
            readsMinus[position]++;
            deletionsMinus[position]++;
        }
    }
}

int Analysis::exportToFile(std::ofstream &filePlus, std::ofstream &fileMinus) {
    if (!filePlus.is_open() || !fileMinus.is_open()) {
        std::cout << "[ERROR] unable to export analysis of sequence \"" 
                  << name << "\"" << std::endl;
        return -1;
    }

    for (size_t i = 0; i < sequence.size(); i++) {
        filePlus << name              << " "
                 << i+1               << " "
                 << "+"               << " "
                 << sequence[i]       << " "
                 << readsPlus[i]      << " "
                 << matchesPlus[i]    << " "
                 << mismatchesPlus[i] << " "
                 << insertionsPlus[i] << " "
                 << deletionsPlus[i]  << " " << std::endl;

        fileMinus << name               << " "
                  << i+1                << " "
                  << "-"                << " "
                  << sequence[i]        << " "
                  << readsMinus[i]      << " "
                  << matchesMinus[i]    << " "
                  << mismatchesMinus[i] << " "
                  << insertionsMinus[i] << " "
                  << deletionsMinus[i]   << " " << std::endl;
    }

    return 0;
}

