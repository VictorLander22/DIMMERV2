#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "KPDeviceSetting.h"

KPDeviceSettingClass::KPDeviceSettingClass()
{
  KPDeviceSettingClass::_factoryDefault();
}

void KPDeviceSettingClass::_factoryDefault()
{
  String chipMac = WiFi.macAddress();
  chipMac.replace(":", "");

  mode = 15; //b0:AllowApi=1, b1:UsaCloud=1, b2:wifiPadrao=1, b3:TipoMemoria=1

  apiPwd = "25d55ad283aa400af464c76d713c07ad"; ///Limit 35 bytes;

  apWifiSSID = "KEEPIN_" + chipMac; //Limit 35 bytes;
  apWifiPwd = "12345678";           //Limit 35 bytes;
  apWifiIP = 17082560;              //192.168.4.1
  apWifiMSK = 16777215;             //255.255.255.0
  apWifiGTW = 17082560;             //192.168.4.1

  utcConfig = -3;

  httpUser = "keepin"; //Limit 35 bytes;
  httpPwd = "keepin";  //Limit 35 bytes;
}

void KPDeviceSettingClass::setMode()
{
  KPEEPROMClass::begin(CFG_TOTAL);
  KPEEPROMClass::setEEPROMInt8(CFG_MODE, mode);
  KPEEPROMClass::end();
}

void KPDeviceSettingClass::setWifi()
{
  KPEEPROMClass::begin(CFG_TOTAL);
  KPEEPROMClass::setEEPROMInt8(CFG_MODE, mode);
  KPEEPROMClass::setEEPROMString(CFG_WIFISSID, wifiSSID);
  KPEEPROMClass::setEEPROMString(CFG_WIFIPWD, wifiPwd);
  KPEEPROMClass::setEEPROMInt32(CFG_WIFIIP, wifiIP);
  KPEEPROMClass::setEEPROMInt32(CFG_WIFIMSK, wifiMSK);
  KPEEPROMClass::setEEPROMInt32(CFG_WIFIGTW, wifiGTW);
  KPEEPROMClass::setEEPROMInt8(CFG_UTC, utcConfig);
  KPEEPROMClass::end();
}

void KPDeviceSettingClass::setApWifiPwd()
{
  KPEEPROMClass::begin(CFG_TOTAL);
  KPEEPROMClass::setEEPROMString(CFG_APWIFIPWD, apWifiPwd);
  KPEEPROMClass::end();
}

void KPDeviceSettingClass::setApiPwd()
{
  KPEEPROMClass::begin(CFG_TOTAL);
  KPEEPROMClass::setEEPROMString(CFG_APIPWD, apiPwd);
  KPEEPROMClass::end();
}

void KPDeviceSettingClass::setHttpSeg()
{
  KPEEPROMClass::begin(CFG_TOTAL);
  KPEEPROMClass::setEEPROMString(CFG_HTTPUSER, httpUser);
  KPEEPROMClass::setEEPROMString(CFG_HTTPPWD, httpPwd);
  KPEEPROMClass::end();
}

void KPDeviceSettingClass::getDeviceSettings()
{
  KPEEPROMClass::begin(CFG_TOTAL);
  mode = KPEEPROMClass::getEEPROMUInt8(CFG_MODE);
  wifiSSID = KPEEPROMClass::getEEPROMString(CFG_WIFISSID, CFG_TOTAL);
  wifiPwd = KPEEPROMClass::getEEPROMString(CFG_WIFIPWD, CFG_TOTAL);
  wifiIP = KPEEPROMClass::getEEPROMUInt32(CFG_WIFIIP);
  wifiMSK = KPEEPROMClass::getEEPROMUInt32(CFG_WIFIMSK);
  wifiGTW = KPEEPROMClass::getEEPROMUInt32(CFG_WIFIGTW);
  apiPwd = KPEEPROMClass::getEEPROMString(CFG_APIPWD, CFG_TOTAL);
  apWifiPwd = KPEEPROMClass::getEEPROMString(CFG_APWIFIPWD, CFG_TOTAL);
  utcConfig = KPEEPROMClass::getEEPROMUInt8(CFG_UTC);
  httpUser = KPEEPROMClass::getEEPROMString(CFG_HTTPUSER, CFG_TOTAL);
  httpPwd = KPEEPROMClass::getEEPROMString(CFG_HTTPPWD, CFG_TOTAL);
  KPEEPROMClass::end();
}

void KPDeviceSettingClass::factoryReset()
{
  KPDeviceSettingClass::_factoryDefault();
  //Armazena variaveis na EEPROM
  KPDeviceSettingClass::setMode();
  KPDeviceSettingClass::setWifi();
  KPDeviceSettingClass::setApWifiPwd();
  KPDeviceSettingClass::setApiPwd();
  KPDeviceSettingClass::setHttpSeg();
}

void KPDeviceSettingClass::showVariables()
{

  Serial.println("\nDevice Settings");
  Serial.printf("Mode      = AllowApi=%d UsaCloud=%d wifiPadrao=%d TipoMemoria=%d (%d)\n", (bitRead(mode, 0)), (bitRead(mode, 1)), (bitRead(mode, 2)), (bitRead(mode, 3)), mode);
  Serial.print("API PWD   = ");
  Serial.println(apiPwd);
  Serial.println("\nSTA MODE - Wifi Configuration");
  Serial.print("Wifi SSID = ");
  Serial.println(wifiSSID);
  Serial.print("Wifi PWD  = ");
  Serial.println(wifiPwd);
  Serial.print("Wifi IP   = ");
  Serial.print(KPDeviceSettingClass::numberToIpString(wifiIP));
  Serial.printf(" (%u)\n", wifiIP);
  Serial.print("Wifi MSK  = ");
  Serial.print(KPDeviceSettingClass::numberToIpString(wifiMSK));
  Serial.printf(" (%u)\n", wifiMSK);
  Serial.print("Wifi GTW  = ");
  Serial.print(KPDeviceSettingClass::numberToIpString(wifiGTW));
  Serial.printf(" (%u)\n", wifiGTW);
  Serial.println("\nAP MODE - Wifi Configuration");
  Serial.print("Wifi SSID = ");
  Serial.println(apWifiSSID);
  Serial.print("Wifi PWD  = ");
  Serial.println(KPDeviceSettingClass::apWifiPwd);
  Serial.print("Wifi IP   = ");
  Serial.print(KPDeviceSettingClass::numberToIpString(apWifiIP));
  Serial.printf(" (%u)\n", apWifiIP);
  Serial.print("Wifi MSK  = ");
  Serial.print(KPDeviceSettingClass::numberToIpString(apWifiMSK));
  Serial.printf(" (%u)\n", apWifiMSK);
  Serial.print("Wifi GTW  = ");
  Serial.print(KPDeviceSettingClass::numberToIpString(apWifiGTW));
  Serial.printf(" (%u)\n", apWifiGTW);
  Serial.printf("UTC  =  (%d)\n", utcConfig);

  Serial.print("Http User:  = ");
  Serial.println(httpUser);
  Serial.print("Http Pwd:   = ");
  Serial.println(httpPwd);
  Serial.println();
}

uint32_t KPDeviceSettingClass::getMemSize()
{

  Serial.println(CFG_TOTAL);
  return CFG_TOTAL;
}

void KPDeviceSettingClass::setAPWifiSSID(String s)
{
  apWifiSSID = "KEEPIN_" + s;
}

String KPDeviceSettingClass::getAPWifiSSID()
{
  return apWifiSSID;
}

uint32_t KPDeviceSettingClass::ipStringToNumber(const char *sIp)
{
  unsigned int ip[4];
  char dummyString[2];

  if (sscanf(sIp, "%u.%u.%u.%u%1s", &ip[3], &ip[2], &ip[1], &ip[0], dummyString) == 4)
  {
    if ((ip[0] < 256) && (ip[1] < 256) && (ip[2] < 256) && (ip[3] < 256))
    {
      return ((ip[0] << 24) + (ip[1] << 16) + (ip[2] << 8) + ip[3]);
    }
  }
  return 0;
}

String KPDeviceSettingClass::numberToIpString(uint32_t numIP)
{
  IPAddress sIp(numIP);
  return sIp.toString();
}

uint32_t KPDeviceSettingClass::convUint32(byte *d)
{
  uint32_t adc_value;
  adc_value = *((long *)d);
  return adc_value;
}

void KPDeviceSettingClass::verifyEEPROM()
{
  KPEEPROMClass::begin(CFG_TOTAL);
  uint16_t pwd = getEEPROMUInt16(CFG_APWIFIPWD);
  KPEEPROMClass::end();

  Serial.print("EEPROM Status: ");
  if (pwd == 65535) //if eeprom never used
  {

    Serial.println("Factory reset");
    wifiSSID = "";
    wifiPwd = "";
    wifiIP = 0;
    wifiMSK = 0;
    wifiGTW = 0;
    KPDeviceSettingClass::factoryReset();
  }
  else
  {
    Serial.println("OK");
  }
}

String KPDeviceSettingClass::dateTimeStr(const time_t &t)
{
  // Retorna data/hora no formato dd/mm/aaaa hh:mm:ss
  String s = "";
  if (day(t) < 10)
  {
    s += '0';
  }
  s += String(day(t)) + '/';
  if (month(t) < 10)
  {
    s += '0';
  }
  s += String(month(t)) + '/' + String(year(t)) + ' ';
  if (hour(t) < 10)
  {
    s += '0';
  }
  s += String(hour(t)) + ':';
  if (minute(t) < 10)
  {
    s += '0';
  }
  s += String(minute(t)) + ':';
  if (second(t) < 10)
  {
    s += '0';
  }
  s += String(second(t));
  return s;
}