#include "Exporter.h"

#include <string>

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
        std::string sequenceName = bamAnalyzer->getLastSequenceName();
        if (sequenceName.empty())
            break;
        Sequence *sequence = bamAnalyzer->getSequence(sequenceName);
        std::ofstream filePlus(directory + sequence->getName() + "_plus.data");
        std::ofstream fileMinus(directory + sequence->getName() +"_minus.data");
        for (auto it = sequence->begin(); it != sequence->end(); it++) {
            if ((*it)->isPlus())
                (*it)->exportToFile(filePlus);
            else
                (*it)->exportToFile(fileMinus);
        }
    }
}

