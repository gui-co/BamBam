#pragma once

#include "Analyzer.h"

#include <thread>

class Exporter {

public:
    void setAnalyzer(Analyzer *analyzer);
    void run(void);
    void wait(void);

private:
    void exportTranscripts(void);
    Analyzer *bamAnalyzer;
    std::thread exporterThread;
};

