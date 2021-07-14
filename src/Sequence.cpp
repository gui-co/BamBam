#include "Sequence.h"

#include <iostream>
#include <fstream>

Sequence::Sequence(const std::string &sequenceName, const std::string &sequence)
    : name(sequenceName), sequence(sequence)
{
    /* Nothing to do */
}

Sequence::~Sequence(void) {
    for (auto it = begin(); it != end(); it++) {
        delete *it;
        *it = nullptr;
    }
}

std::string Sequence::getName(void) {
    return name;
}

Transcript *Sequence::getTranscript(Transcript::StrandPolarity polarity,
                                   size_t startPosition) {
    Transcript *transcript = nullptr;
    auto transcriptIt = begin();
    while ((!transcript) && (transcriptIt != end())) {
        if ((*transcriptIt)->isReadInside(startPosition, polarity))
            transcript = (*transcriptIt);
        transcriptIt++;
    }
    if (!transcript) {
        std::cout << "Creation of a new transcript starting at "
                  << startPosition << std::endl;
        transcript = new Transcript(name, polarity, startPosition);
        push_back(transcript);
    }
    return transcript;
}

