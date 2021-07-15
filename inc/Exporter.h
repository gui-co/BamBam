#pragma once

#include "Analyzer.h"
#include "filesystem.h"

#include <thread>
#include <string>

class Exporter {

public:
    int setExportDirectory(const std::string &directory);
    void setAnalyzer(Analyzer *analyzer);
    void run(void);
    void wait(void);

private:
    void exportTranscripts(void);
    Analyzer *bamAnalyzer;
    std::thread exporterThread;
    fs::path saveDirectory;
};

