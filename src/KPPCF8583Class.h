/*
*****************************************************************************
* KPWIREClass
* Keepin - Leonardo Aquino
* Controle de escrita e leitura pelo TwoWire
* v1.0 - Implementação
*****************************************************************************
*/

#ifndef KPPCF8583Class_H
#define KPPCF8583Class_H

// Bibliotecas
#include "Arduino.h"
#include "PCF8583.h"

class KPPCF8583Class
{
public:
  KPPCF8583Class(int device_address);

  uint16_t outValues;

  bool getRtcBatStatus();
  void setBateryMemStatus();
  void setOutputs();
  uint16_t getOutputs();

private:
  int _address;
  uint8_t _bateryCheck[4];
};

#endif
