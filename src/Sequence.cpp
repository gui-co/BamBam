#include "Sequence.h"

#include <iostream>
#include <fstream>

Sequence::Sequence(const std::string &sequenceName, const std::string &sequence)
    : name(sequenceName), sequence(sequence)
{
    /* Nothing to do */
}

Sequence::~Sequence(void) {
    for (auto it = transcripts.begin(); it != transcripts.end(); it++) {
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
    auto transcriptIt = transcripts.begin();
    while ((!transcript) && (transcriptIt != transcripts.end())) {
        if ((*transcriptIt)->isReadInside(startPosition, polarity))
            transcript = (*transcriptIt);
        transcriptIt++;
    }
    if (!transcript) {
        std::cout << "Creation of a new transcript starting at "
                  << startPosition << std::endl;
        transcript = new Transcript(name, polarity, startPosition);
        transcripts.push_back(transcript);
    }
    return transcript;
}

std::list<Transcript*>::iterator Sequence::begin(void) {
    return transcripts.begin();
}

std::list<Transcript*>::iterator Sequence::end(void) {
    return transcripts.end();
}

void Sequence::exportToFile(const std::string &directory) {
    std::cout << "Exporting " << transcripts.size() << " transcripts from "
              << "sequence " << name << std::endl;
    std::ofstream filePlus(directory + name + "_plus.data");
    std::ofstream fileMinus(directory + name +"_minus.data");
    for (auto it = transcripts.begin(); it != transcripts.end(); it++) {
        if ((*it)->isPlus())
            (*it)->exportToFile(filePlus);
        else
            (*it)->exportToFile(fileMinus);
    }
}

