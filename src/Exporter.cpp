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
    while (true) {
        std::string sequenceName = bamAnalyzer->getLastSequenceName();
        if (sequenceName.empty())
            break;
        Sequence *sequence = bamAnalyzer->getSequence(sequenceName);
        sequence->exportToFile("./");
    }
}

