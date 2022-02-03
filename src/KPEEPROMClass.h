/*
*****************************************************************************
* KPEEPROMClass
* Keepin - Leonardo Aquino
* Controle de escrita e leitura da EEPROM
* v1.0 - Implementação
*****************************************************************************
*/

#ifndef KPEEPROMClass_H
#define KPEEPROMClass_H

// Bibliotecas

#include "EEPROM.h"
//#include "TimeLib.h"

class KPEEPROMClass : public EEPROMClass
{
public:
  KPEEPROMClass();
  using EEPROMClass::begin;
  using EEPROMClass::commit;
  using EEPROMClass::end;

  //Funções para leitura da EEPROM
  int8_t getEEPROMInt8(int offset);
  uint8_t getEEPROMUInt8(int offset);
  int16_t getEEPROMInt16(int offset);
  uint16_t getEEPROMUInt16(int offset);
  int32_t getEEPROMInt32(int offset);
  uint32_t getEEPROMUInt32(int offset);
  String getEEPROMString(int offset, word memSize);
  //Funções para gravação na EEPROM
  void setEEPROMInt8(int offset, int8_t v);
  void setEEPROMInt8(int offset, uint8_t v);
  void setEEPROMInt16(int offset, int16_t v);
  void setEEPROMInt16(int offset, uint16_t v);
  void setEEPROMInt32(int offset, int32_t v);
  void setEEPROMInt32(int offset, uint32_t v);
  void setEEPROMString(int offset, String s);
};

#endif
