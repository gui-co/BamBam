#pragma once

#include <string>
#include <map>

class FastaReader {

public:
    int readFastaFile(const std::string &filename);
    std::string getSequence(const std::string &name);

private:
    std::map<std::string, std::string> sequences;

};

