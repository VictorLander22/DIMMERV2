/*
*****************************************************************************
* KPWireClass
* Keepin - Leonardo Aquino
* Controle de escrita e leitura pelo TwoWire
* v1.0 - Implementação
*****************************************************************************
*/
#include "KPPCF8583Class.h"
#include "Arduino.h"

KPPCF8583Class::KPPCF8583Class(int device_address)
{
  _address = device_address >> 1;
}

bool KPPCF8583Class::getRtcBatStatus()
{

  Serial.printf("Status do I2C: %d", Wire.status());
  Serial.println();

  Wire.beginTransmission(_address);
  Wire.write(0x40);
  Wire.endTransmission();
  Wire.requestFrom(_address, 4);

  uint32_t bateryValue = Wire.read() | Wire.read() << 8 | Wire.read() << 16 | Wire.read() << 24;

  Serial.printf("Checking RTC Battery (%d = 67305985?)\n", bateryValue); //_bateryCheck[0], _bateryCheck[1], _bateryCheck[2], _bateryCheck[3]);

  //if ((_bateryCheck[0]==1) && (_bateryCheck[1]==2) && (_bateryCheck[2]==3) && (_bateryCheck[3]==4))
  if (bateryValue == 67305985)
  {

    Serial.println("Battery is OK");
    return true;
  }
  else
  {

    Serial.println("Battery is low: Setting RTC to default");
    return false;
  }
}

void KPPCF8583Class::setBateryMemStatus()
{
  Wire.beginTransmission(_address);
  Wire.write(0x40);
  Wire.write(1);
  Wire.write(2);
  Wire.write(3);
  Wire.write(4);
  Wire.endTransmission();
}

void KPPCF8583Class::setOutputs()
{
  Wire.beginTransmission(_address);
  Wire.write(0x20);
  Wire.write(outValues & 0xFF);
  Wire.write((outValues >> 8) & 0xFF);
  Wire.endTransmission();
}

uint16_t KPPCF8583Class::getOutputs()
{
  Wire.beginTransmission(_address);
  Wire.write(0x20);
  Wire.endTransmission();
  Wire.requestFrom(_address, 2);
  outValues = Wire.read() | Wire.read() << 8;
  return outValues;
}