/*
*****************************************************************************
* SettingCcontrol
* Keepin - Leonardo Aquino
* Controle das configurações da placa
* v1.0 - Implementação
*****************************************************************************
*/

#ifndef KPIRCTRL_H
#define KPIRCTRL_H

// Bibliotecas

struct KPIRClass
{
  string codigo;
  void sendIRCMD(String Codigo, String Codigo2, int QtdeBit, int PortaIRS, int vModelo, int q)
};

#endif
