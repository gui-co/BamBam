#pragma once

#include <string>
#include <vector>

class BamRead {

public:
    BamRead(void);
    void setName(char *array, size_t length);
    std::string getName(void) const;
    void setReadSequence(uint8_t *array, size_t length);
    void setCigar(uint32_t *array, size_t length);
    std::vector<uint32_t> getCigar(void) const;

private:
    std::string name;
    std::vector<uint8_t> read;
    std::vector<uint32_t> cigar;

};

