#pragma once

#include <string>
#include <vector>
#include <fstream>

class Analysis {

public:
    enum StrandPolarity {
        PLUS_STRAND,
        MINUS_STRAND
    };
    Analysis(const std::string &sequenceName, const std::string &sequence);
    void addMatch(size_t position, StrandPolarity pol);
    void addMismatch(size_t position, StrandPolarity pol);
    void addInsertion(size_t position, StrandPolarity pol);
    void addDeletion(size_t position, StrandPolarity pol);
    int exportToFile(std::ofstream &filePlus, std::ofstream &fileMinus);

private:
    std::string name;
    std::string sequence;
    std::vector<int> readsPlus;
    std::vector<int> readsMinus;
    std::vector<int> matchesPlus;
    std::vector<int> matchesMinus;
    std::vector<int> mismatchesPlus;
    std::vector<int> mismatchesMinus;
    std::vector<int> insertionsPlus;
    std::vector<int> insertionsMinus;
    std::vector<int> deletionsPlus;
    std::vector<int> deletionsMinus;

};

