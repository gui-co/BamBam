#include "FastaReader.h"
#include "BamReader.h"
#include "Analyzer.h"
#include "Exporter.h"

#include <unistd.h>

#include <iostream>

void usage(const char *prog) {
    std::cout << std::endl
              << "Usage: " << prog << " -f <fasta_file> -b <bam_file>"
              << std::endl;
}


int main(int argc,  char** argv) {
    int opt;
    std::string fastaFile;
    std::string bamFile;
    std::string outputDirectory;
    while ((opt = getopt(argc, argv, "f:b:o:")) != -1) {
        switch (opt) {
            case 'f':
                fastaFile = std::string(optarg);
                break;
            case 'b':
                bamFile = std::string(optarg);
                break;
            case 'o':
                outputDirectory = std::string(optarg);
                break;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    if (bamFile.empty()) {
        std::cout << std::endl
                  << "Please provide a valid bam file. Aborting..."
                  << std::endl;
        usage(argv[0]);
        return -1;
    }
    BamReader bamReader;
    if (bamReader.setBamFile(bamFile) != 0) {
        std::cout << std::endl
                  << "BAM file is not valid. Aborting..."
                  << std::endl;
        usage(argv[0]);
        return -1;
    }

    if (fastaFile.empty()) {
        std::cout << std::endl
                  << "Please provide a valid fasta file. Aborting..."
                  << std::endl;
        usage(argv[0]);
        return -1;
    }
    FastaReader fastaReader;
    if (fastaReader.readFastaFile(fastaFile) != 0) {
        std::cout << "Fasta file is not valid. Aborting..." << std::endl;
        usage(argv[0]);
        return -1;
    }

    Analyzer analyzer;
    analyzer.setFastaReader(&fastaReader);
    analyzer.setBamReader(&bamReader);

    Exporter exporter;
    exporter.setExportDirectory(outputDirectory);
    exporter.setAnalyzer(&analyzer);
    exporter.run();

    analyzer.analyze();

    exporter.wait();

    return 0;
}

