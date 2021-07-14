#include "Exporter.h"

#include <string>
#include <iostream>
#include <iomanip>

int Exporter::setExportDirectory(const std::string &directory) {
    using namespace std::filesystem;
    if (directory.empty())
        saveDirectory = current_path();
    else
        saveDirectory = path(directory);

    if (exists(saveDirectory) && is_directory(saveDirectory))
        return 0;

    if (exists(saveDirectory)) {
        std::cout << "[ERROR] \"" << directory << "\" is not a directory"
                  << std::endl;
        return -1;
    }

    if (!create_directory(saveDirectory)) {
        std::cout << "[ERROR] \"" << directory << "\" does not exist and it is "
                  << "not possible to create it" << std::endl;
        return -1;
    } else {
        std::cout << "[INFO] Export directory \"" << directory << "\" was "
                  << "created" << std::endl;
        return 1;
    }
}

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
    using std::filesystem::path;
    while (true) {
        Sequence sequence = bamAnalyzer->takeLastSequence();
        std::string sequenceName = sequence.getName();
        if (sequenceName.empty())
            break;
        else
            std::cout << "Export of " << sequenceName << std::endl;

        std::ofstream fileLog(saveDirectory / path(sequenceName + ".data"));
        std::ofstream filePlus(saveDirectory
                               / path(sequenceName + "_plus.data"));
        std::ofstream fileMinus(saveDirectory
                                / path(sequenceName +"_minus.data"));
        filePlus << std::fixed << std::setprecision(5);
        fileMinus << std::fixed << std::setprecision(5);

        fileLog << "# " << sequenceName << " "
                << sequence.size() << " transcripts" << std::endl;
        auto transcriptIt = sequence.begin();
        while (transcriptIt != sequence.end()) {
            Transcript &transcript = (*transcriptIt);
            fileLog << transcript.startPosition + 1 << " "
                    << transcript.startPosition + 1 + transcript.size()
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
                if (base.getReads() == 0) {
                    baseIt++;
                    continue;
                }
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

