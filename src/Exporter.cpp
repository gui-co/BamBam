#include "Exporter.h"

#include <string>
#include <iomanip>

void Exporter::setAnalyzer(Analyzer *analyzer) {
    bamAnalyzer = analyzer;
}

void Exporter::run(void) {
    exporterThread = std::thread(&Exporter::exportTranscripts, this);
}

void Exporter::wait(void) {
    exporterThread.join();
}

void Exporter::exportTranscripts(void) {
    std::string directory = "./";
    while (true) {
        Sequence sequence = bamAnalyzer->takeLastSequence();
        std::string sequenceName = sequence.getName();
        std::ofstream fileLog(directory + sequenceName + ".data");
        std::ofstream filePlus(directory + sequenceName + "_plus.data");
        std::ofstream fileMinus(directory + sequenceName +"_minus.data");
        filePlus << std::fixed << std::setprecision(5);
        fileMinus << std::fixed << std::setprecision(5);

        fileLog << "# " << sequenceName << " "
                << sequence.size() << " transcripts" << std::endl;
        auto transcriptIt = sequence.begin();
        while (transcriptIt != sequence.end()) {
            Transcript &transcript = (*transcriptIt);
            fileLog << transcript.startPosition + 1 << " "
                    << transcript.startPosition + 1 + transcript.length
                    << std::endl;

            std::ofstream *file;
            if (transcript.isPlus())
                file = &filePlus;
            else
                file = &fileMinus;

            size_t position = transcript.startPosition + 1;
            auto baseIt = transcript.begin();
            while (baseIt != transcript.end()) {
                Base &base = *baseIt;
                *file << sequenceName << " "
                      << position
                      << std::setw(4) << base.getReads()
                      << std::setw(10) << (double) base.getQuality()
                         / (base.getMatches() + base.getMismatches())
                      << std::setw(10) << (double) base.getMismatches()
                         / base.getReads()
                      << std::setw(10) << (double) base.getInsertions()
                         / base.getReads()
                      << std::endl;
                baseIt++;
                position++;
            }
            transcriptIt++;
        }
    }
}

