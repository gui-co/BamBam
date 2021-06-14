#pragma once

#include <string>
#include <vector>

class Analysis {

public:
    Analysis(const std::string &sequenceName, const std::string &sequence);
    void addMatch(size_t position);
    void addMismatch(size_t position);
    void addInsertion(size_t position);
    void addDeletion(size_t position);

private:
    std::string name;
    std::string sequence;
    std::vector<int> reads;
    std::vector<int> matches;
    std::vector<int> mismatches;
    std::vector<int> insertions;
    std::vector<int> deletions;

};

