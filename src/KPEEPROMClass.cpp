/*
*****************************************************************************
* KPEEPROMClassr
* Keepin - Leonardo Aquino
* Controle de escrita e leitura da EEPROM
* v1.0 - Implementação
*****************************************************************************
*/

#include "Arduino.h"
#include "KPEEPROMClass.h"

KPEEPROMClass::KPEEPROMClass()
{
}

int8_t KPEEPROMClass::getEEPROMInt8(int offset)
{
  // Le dado Int8 da KPEEPROMClass::EEPROMClass::(–128 a 127)
  return int8(KPEEPROMClass::EEPROMClass::read(offset));
}

uint8_t KPEEPROMClass::getEEPROMUInt8(int offset)
{
  // Le dado UInt8 da KPEEPROMClass::EEPROMClass::(0 a 255)
  return KPEEPROMClass::EEPROMClass::read(offset);
}

int16_t KPEEPROMClass::getEEPROMInt16(int offset)
{
  // Le dado Int16 da KPEEPROMClass::EEPROMClass::(–32768 a 32767)
  return short(word(KPEEPROMClass::EEPROMClass::read(offset), KPEEPROMClass::EEPROMClass::read(offset + 1)));
}

uint16_t KPEEPROMClass::getEEPROMUInt16(int offset)
{
  // Le dado UInt16 da KPEEPROMClass::EEPROMClass::(0 a 65535)
  return word(KPEEPROMClass::EEPROMClass::read(offset), KPEEPROMClass::EEPROMClass::read(offset + 1));
}

int32_t KPEEPROMClass::getEEPROMInt32(int offset)
{
  // Le dado Int32 da KPEEPROMClass::EEPROMClass::(–2.147.483.648 a 2.147.483.647)
  int32 l;
  l = KPEEPROMClass::EEPROMClass::read(offset);
  l = l << 8;
  l |= KPEEPROMClass::EEPROMClass::read(offset + 1);
  l = l << 8;
  l |= KPEEPROMClass::EEPROMClass::read(offset + 2);
  l = l << 8;
  l |= KPEEPROMClass::EEPROMClass::read(offset + 3);
  return l;
}

uint32_t KPEEPROMClass::getEEPROMUInt32(int offset)
{
  // Le dado UInt32 da KPEEPROMClass::EEPROMClass::(0 a 4.294.967.295)
  uint32 l;
  l = KPEEPROMClass::EEPROMClass::read(offset);
  l = l << 8;
  l |= KPEEPROMClass::EEPROMClass::read(offset + 1);
  l = l << 8;
  l |= KPEEPROMClass::EEPROMClass::read(offset + 2);
  l = l << 8;
  l |= KPEEPROMClass::EEPROMClass::read(offset + 3);
  return l;
}

String KPEEPROMClass::getEEPROMString(int offset, word memSize)
{
  // Le String da EEPROM
  String s = "";
  while (offset < memSize)
  {
    if (KPEEPROMClass::EEPROMClass::read(offset) == 0)
    {
      break;
    }
    s += char(KPEEPROMClass::EEPROMClass::read(offset));
    offset++;
  }
  return s;
}

void KPEEPROMClass::setEEPROMInt8(int offset, int8_t v)
{
  // Grava dado Int8 na KPEEPROMClass::EEPROMClass::(–128 a 127)
  KPEEPROMClass::EEPROMClass::write(offset, v);
}

void KPEEPROMClass::setEEPROMInt8(int offset, uint8_t v)
{
  // Grava dado UInt8 na KPEEPROMClass::EEPROMClass::(0 a 255)
  KPEEPROMClass::EEPROMClass::write(offset, v);
}

void KPEEPROMClass::setEEPROMInt16(int offset, int16_t v)
{
  // Grava dado Int16 na KPEEPROMClass::EEPROMClass::(–32768 a 32767)
  KPEEPROMClass::EEPROMClass::write(offset, highByte(v));
  KPEEPROMClass::EEPROMClass::write(offset + 1, lowByte(v));
}

void KPEEPROMClass::setEEPROMInt16(int offset, uint16_t v)
{
  // Grava dado UInt16 na KPEEPROMClass::EEPROMClass::(0 a 65535)
  KPEEPROMClass::EEPROMClass::write(offset, highByte(v));
  KPEEPROMClass::EEPROMClass::write(offset + 1, lowByte(v));
}

void KPEEPROMClass::setEEPROMInt32(int offset, int32_t v)
{
  // Grava dado Int32 na KPEEPROMClass::EEPROMClass::(–2147483648 a 2147483647)
  KPEEPROMClass::EEPROMClass::write(offset + 3, byte(v));
  v = v >> 8;
  KPEEPROMClass::EEPROMClass::write(offset + 2, byte(v));
  v = v >> 8;
  KPEEPROMClass::EEPROMClass::write(offset + 1, byte(v));
  v = v >> 8;
  KPEEPROMClass::EEPROMClass::write(offset, byte(v));
}

void KPEEPROMClass::setEEPROMInt32(int offset, uint32_t v)
{
  // Grava dado UInt32 na KPEEPROMClass::EEPROMClass::(0 a 4294967295)
  KPEEPROMClass::EEPROMClass::write(offset + 3, byte(v));
  v = v >> 8;
  KPEEPROMClass::EEPROMClass::write(offset + 2, byte(v));
  v = v >> 8;
  KPEEPROMClass::EEPROMClass::write(offset + 1, byte(v));
  v = v >> 8;
  KPEEPROMClass::EEPROMClass::write(offset, byte(v));
}

void KPEEPROMClass::setEEPROMString(int offset, String s)
{
  // Grava String na EEPROM
  for (byte b = 0; b <= s.length(); b++)
  {
    KPEEPROMClass::EEPROMClass::write(offset + b, s[b]);
  }
}