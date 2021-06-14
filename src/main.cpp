#include "FastaReader.h"
#include "BamReader.h"

#include <unistd.h>

#include <iostream>

void usage(const char *prog) {
    std::cout << "Usage: " << prog << " -f <fasta_file> -b <bam_file>"
              << std::endl;
}


int main(int argc,  char** argv) {
    int opt;
    std::string fastaFile;
    std::string bamFile;
    while ((opt = getopt(argc, argv, "f:b:")) != -1) {
        switch (opt) {
            case 'f':
                fastaFile = std::string(optarg);
                break;
            case 'b':
                bamFile = std::string(optarg);
                break;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    if (bamFile.empty()) {
        std::cout << "[Error]: please provide a bam file" << std::endl;
        usage(argv[0]);
        return -1;
    }
    BamReader bamReader;
    if (bamReader.setBamFile(bamFile) != 0) {
        std::cout << "[Error]: bam file is not valid" << std::endl;
        usage(argv[0]);
        return -1;
    }

    if (fastaFile.empty()) {
        std::cout << "[Error]: please provide a fasta file" << std::endl;
        usage(argv[0]);
        return -1;
    }
    FastaReader fastaReader;
    if (fastaReader.readFastaFile(fastaFile) != 0) {
        std::cout << "[Error]: fasta file is not valid" << std::endl;
        usage(argv[0]);
        return -1;
    }

    return 0;
}

