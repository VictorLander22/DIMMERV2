#ifndef KPPCF_H
#define KPPCF_H

// Bibliotecas
#include "Arduino.h"

class KPPCF
{
public:
    void begin();
    uint8_t read8();
    uint8_t read(const uint8_t pin);
    void write8(const uint8_t value);
    void write(const uint8_t pin, const uint8_t value);
};

#endif