#include "Sequence.h"

#include <iostream>
#include <fstream>

Sequence::Sequence(const std::string &sequenceName, const std::string &sequence)
    : name(sequenceName), sequence(sequence)
{
    /* Nothing to do */
}

std::string Sequence::getName(void) {
    return name;
}

Transcript &Sequence::getTranscript(Transcript::StrandPolarity polarity,
                                    size_t startPosition) {
    for (auto it = begin(); it != end(); it++) {
        if ((*it).isReadInside(startPosition, polarity))
            return (*it);
    }
    emplace_back(Transcript(name, polarity, startPosition));
    return back();
}

