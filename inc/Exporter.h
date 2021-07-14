#pragma once

#include "Analyzer.h"

#include <thread>
#include <filesystem>
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
    std::filesystem::path saveDirectory;
};

