#include "BamRead.h"

BamRead::BamRead(void)
{
    /* Nothing to do */
}

void BamRead::setName(char *array, size_t length) {
    name = std::string(array, length);
}

std::string BamRead::getName(void) const{
    return name;
}

void BamRead::setReadSequence(uint8_t *array, size_t length) {
    read.assign(array, array + length);
}

void BamRead::setCigar(uint32_t *array, size_t length) {
    cigar.assign(array, array + length);
}

std::vector<uint32_t> BamRead::getCigar(void) const {
    return cigar;
}

