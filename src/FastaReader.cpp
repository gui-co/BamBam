#include "FastaReader.h"

#include <fstream>
#include <iostream>

int FastaReader::readFastaFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "[Error]: Unable to open " << filename << std::endl;
        return -1;
    }

    char c;
    file.read(&c, sizeof(c));
    while (!file.eof() && !file.bad()) {
        if (c == '>') {
            // new sequence
            std::string name;
            file.read(&c, sizeof(c));
            while (c != ' ' && c != '|') {
                name.push_back(c);
                file.read(&c, sizeof(c));
            }
            do {
                file.read(&c, sizeof(c));
            } while (c != '\n' && !file.eof());
            std::string sequence;
            file.read(&c, sizeof(c));
            while (c != '>' && !file.eof()) {
                switch (c) {
                    case '(':
                        char c1, c2;
                        file.read(&c1, sizeof(c1));
                        file.read(&c2, sizeof(c2));
                        if (c1 == 'i' && c2 == ')') {
                            sequence.push_back(c1);
                            break;
                        } else {
                            return -1;
                        }
                    case 'A':
                    case 'C':
                    case 'G':
                    case 'T':
                    case 'U':
                    case 'R':
                    case 'Y':
                    case 'K':
                    case 'M':
                    case 'S':
                    case 'W':
                    case 'B':
                    case 'D':
                    case 'H':
                    case 'V':
                    case 'N':
                    case '-':
                        sequence.push_back(c);
                        break;
                    case '\n':
                        break;
                    default:
                        std::cout << "[Error]: sequence " << name << " is "
                                     "corrupted" << std::endl;
                        return -1;
                }
                file.read(&c, sizeof(c));
            }
            sequences[name] = sequence;
        } else {
            file.read(&c, sizeof(c));
        }
    }

    return 1;
}

std::string FastaReader::getSequence(const std::string &name) {
    auto it = sequences.find(name);
    if (it == sequences.end()) {
        return std::string();
    }
    return it->second;
}

