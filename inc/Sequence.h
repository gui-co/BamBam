#pragma once

#include "Transcript.h"

#include <string>
#include <list>

class Sequence : public std::list<Transcript> {

public:
    Sequence(void) = default;
    Sequence(const std::string &sequenceName, const std::string &sequence);
    Sequence(Sequence &&sequence) = default;
    Sequence &operator=(Sequence &&rhs) = default;
    Transcript &getTranscript(Transcript::StrandPolarity polarity,
                              size_t position);
    std::string getName(void);

private:
    std::string name;
    std::string sequence;
};

