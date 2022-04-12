/*
*****************************************************************************
* SettingCcontrol
* Keepin - Leonardo Aquino
* Controle das configurações da placa
* v1.0 - Implementação
*****************************************************************************
*/

#ifndef KPSETCTRL_H
#define KPSETCTRL_H

// Bibliotecas

#include "KPEEPROMClass.h"
#include "TimeLib.h"

class KPDeviceSettingClass : private KPEEPROMClass
{
public:
  // Variaveis de configuração da memoria - quantidade de bytes disponiveis para alocação

  //Variaveis de configuração - inicialização e padrão de fábrica

  uint8_t mode;    //b0:AllowApi, b1:UsaCloud, b2:wifiPadrao, b3:TipoMemoria ..
  String wifiSSID; //Limit 35 bytes;
  String wifiPwd;  //Limit 35 bytes;
  uint32_t wifiIP;
  uint32_t wifiMSK;
  uint32_t wifiGTW;

  String apiPwd; ///Limit 35 bytes;

  String apWifiSSID; //Limit 35 bytes;
  String apWifiPwd;  //Limit 35 bytes;
  uint32_t apWifiIP;
  uint32_t apWifiMSK;
  uint32_t apWifiGTW;

  int8_t utcConfig;

  String httpUser; //Limit 35 bytes;
  String httpPwd;  //Limit 35 bytes;

  KPDeviceSettingClass();

  // void begin();
  void verifyEEPROM();
  void setMode();
  void setWifi();
  void setApWifiPwd();
  void setApiPwd();
  void setHttpSeg();
  void setAPWifiSSID(String s);
  void getDeviceSettings();
  String getAPWifiSSID();
  uint32_t getMemSize();
  void factoryReset();

  uint32_t ipStringToNumber(const char *sIp);
  String numberToIpString(uint32_t numIp);

  void showVariables();

  String dateTimeStr(const time_t &t);

  uint32_t convUint32(byte *d);

private:
  const byte CFG_MODE = 0;
  const byte CFG_WIFISSID = CFG_MODE + 1;
  const byte CFG_WIFIPWD = CFG_WIFISSID + 35;
  const byte CFG_WIFIIP = CFG_WIFIPWD + 35;
  const byte CFG_WIFIMSK = CFG_WIFIIP + 4;
  const byte CFG_WIFIGTW = CFG_WIFIMSK + 4;
  const byte CFG_APIPWD = CFG_WIFIGTW + 4;
  const byte CFG_APWIFIPWD = CFG_APIPWD + 35;
  const byte CFG_UTC = CFG_APWIFIPWD + 35;
  const byte CFG_HTTPUSER = CFG_UTC + 1;
  const byte CFG_HTTPPWD = CFG_HTTPUSER + 35;
  const word CFG_TOTAL = CFG_HTTPPWD + 35;

  void _factoryDefault();
};

#endif
