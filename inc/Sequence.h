#pragma once

#include "Transcript.h"

#include <string>
#include <list>

class Sequence : public std::list<Transcript*> {

public:
    Sequence(void) = default;
    Sequence(const std::string &sequenceName, const std::string &sequence);
    ~Sequence(void);
    Transcript *getTranscript(Transcript::StrandPolarity polarity,
                              size_t position);
    Sequence(Sequence &&sequence) = default;
    Sequence &operator=(Sequence &&rhs) = default;
    std::string getName(void);

private:
    std::string name;
    std::string sequence;
};

