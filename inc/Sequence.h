#pragma once

#include "Transcript.h"

#include <string>
#include <list>

class Sequence {

public:
    Sequence(const std::string &sequenceName, const std::string &sequence);
    ~Sequence(void);
    Transcript *getTranscript(Transcript::StrandPolarity polarity,
                              size_t position);
    void exportToFile(const std::string &directory);

private:
    std::string name;
    std::string sequence;
    std::list<Transcript*> transcripts;
};

