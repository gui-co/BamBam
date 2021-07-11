#pragma once

class Base {

public:
    void addMatch(int quality);
    int getMatches(void);
    void addMismatch(int quality);
    int getMismatches(void);
    void addInsertion(void);
    int getInsertions(void);
    void addDeletion(void);
    int getDeletions(void);
    int getReads(void);
    int getQuality(void);

private:
    int reads;
    int matches;
    int mismatches;
    int insertions;
    int deletions;
    int quality;
};

