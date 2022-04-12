void LoopReconexao()
{
  // reconexao
  if ((!vConfigWIFI && ((WiFi.getMode() != WIFI_STA) || (WiFi.status() != WL_CONNECTED))) || (WiFi.getMode() == WIFI_OFF))
  {
    conectar();
  }
}

void conectar()
{
  // WiFi.disconnect();
  // WiFi.softAPdisconnect();
  DevSet.getDeviceSettings();

  vConfigWIFI = bitRead(DevSet.mode, 2);

  // String retorno = wifiPadrao();
  // vConfigWIFI = retorno;
  if (scanningWifi >= 0)
  {
    if (!vConfigWIFI && (vListaWifi.lastIndexOf(DevSet.wifiSSID) >= 0)) // Wifi STA
    {

      wifiConectSTA();

      int waitingWifi = 0;

      while (WiFi.status() != WL_CONNECTED)
      {
        if (waitingWifi > 20 && WiFi.getMode() < 2)
        {
          wifiConectAP();
          break;
        }

        if (digitalRead(buttonState) == HIGH)
        {
          slog("resetando");
          // WiFi.disconnect();
          // DevSet.factoryReset();
          // ESP.restart();
        }

        delay(400);
        // chip3.write(LedWifiConnected, !chip3.read(LedWifiConnected));
        slog(".");
        waitingWifi++;
      }
      slogln("");
      // SetupUDP();
    }
    else if (WiFi.getMode() < 2)
    {
      wifiConectAP();
      // SetupUDP();
    }
  }
  if (WiFi.getMode() != 1)
    getAvalibleNetwork();
}

// void listawifi2()
// {
//   gRequest->send(200, sdefTextHtml, vListaWifi);
// }

void WifiNetworkScan()
{
  millisNetworkScan = millisAtual;
  getAvalibleNetwork();
  gRequest->send(200, sdefTextHtml, vListaWifi);
}

void getAvalibleNetwork()
{
  int n = WiFi.scanComplete();
  scanningWifi = n;
  if (n >= 0)
  {
    // scanningWifi = n;
    slogln((String)n + " network(s) found");
    vListaWifi = "";
    for (int i = 0; i < n; i++)
    {
      Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
      vListaWifi += WiFi.SSID(i);
      vListaWifi += "|";
    }
    vListaWifi += "*";
    WiFi.scanDelete();
  }
  else if ((millisAtual > millisNetworkScan) && n != -1)
  {
    millisNetworkScan = millisAtual + 30000;
    // scanningWifi = n;
    slogln("\nNetwork scan started");
    WiFi.scanNetworks(true);
  }
}

void gravasenhawifi()
{
  if (gRequest->arg("s") == Senha)
  {
    if (vSenhaAP == gRequest->arg("a"))
    {

      vSenhaAP = gRequest->arg("v");

      if (vSenhaAP.length() >= 8)
      {
        gRequest->send(200, sdefTextHtml, sdefOK);
        DevSet.apWifiPwd = vSenhaAP;
        DevSet.setApWifiPwd();
        slogln("Alterado: " + vSenhaAP);
      }
      else
      {
        gRequest->send(200, sdefTextHtml, "-1");
      }
    }
    else
    {
      gRequest->send(200, sdefTextHtml, "-1");
    }
  }
  else
  {
    gRequest->send(200, sdefTextHtml, "-1");
  }
}

void gravasenhahttp()
{
  if (gRequest->arg("s") == Senha)
  {
    String newHttpUser = gRequest->arg("u");
    String newHttpPwd = gRequest->arg("v");

    if ((newHttpUser.length() >= 4 && newHttpPwd.length() >= 4) && (gRequest->arg("ua") == DevSet.httpUser && gRequest->arg("a") == DevSet.httpPwd))
    {
      String ip = WiFi.localIP().toString();
      if (ip == "(IP unset)")
        ip = DevSet.numberToIpString(DevSet.apWifiIP);
      String restartPage(FPSTR(webRestart));
      restartPage.replace("#oldip#", ip);
      restartPage.replace("#newip#", DevSet.numberToIpString(DevSet.wifiIP));
      gRequest->send_P(200, sdefTextHtml, restartPage.c_str());

      DevSet.httpUser = newHttpUser;
      DevSet.httpPwd = newHttpPwd;
      DevSet.setHttpSeg();

      slogln("Reiniciando sistema depois de alterar http senha");
      delay(300);
      ESP.restart();
    }
    else
    {
      gRequest->send(200, sdefTextHtml, "-1");
    }
  }
  else
  {
    gRequest->send(200, sdefTextHtml, "-1");
  }
}

void wifiConectSTA()
{
  slogln(F("\nWifi trying conection in: STA MODE"));

  WiFi.mode(WIFI_STA);
  tipoWifiAtual = 1;
  const char *ssid = DevSet.wifiSSID.c_str();

  const char *password = DevSet.wifiPwd.c_str();
  IPAddress ip(DevSet.wifiIP);
  IPAddress subnet(DevSet.wifiMSK);
  IPAddress gateway(DevSet.wifiGTW);
  IPAddress dns(8, 8, 8, 8);

  slogln(ssid);
  // (!DEBUG_ON) ?:   Serial.println(password);
  // (!DEBUG_ON) ?:   Serial.println(ip);
  // (!DEBUG_ON) ?:   Serial.println(subnet);
  // (!DEBUG_ON) ?:   Serial.println(gateway);

  IpDispositivo = ip;
  local_IP = ip;
  setBroadcastIP(DevSet.wifiMSK);

  WiFi.config(ip, gateway, subnet, DNS1, DNS2);
  WiFi.begin(ssid, password);

  // chip3.write(LedWifiConnected, LOW);
  // chip3.write(LedWifiHI, HIGH);
  // chip3.write(LedWifiLOW, HIGH);
}

void wifiConectAP()
{
  slog("Rede não localizada: ");
  slogln(DevSet.wifiSSID);
  slogln("Wifi trying conection in: AP MODE");

  WiFi.mode(WIFI_AP);
  tipoWifiAtual = 2;
  // listawifi();
  // getAvalibleNetwork();
  // //chip3.write(LedWiFI, HIGH);
  IPAddress local_IP(DevSet.apWifiIP);
  IPAddress gateway(DevSet.apWifiGTW);
  IPAddress subnet(DevSet.apWifiMSK);

  slog("Setting soft-AP configuration ... ");
  slogln(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  slog("Setting soft-AP ... ");
  slogln("idencitifcador");
  // int chipId = ESP.getChipId();
  String NomeRede = "KEEPIN_" + gchipId;
  slogln(NomeRede);
  const char *nRede = NomeRede.c_str();
  slogln(nRede);

  //(!DEBUG_ON) ?:   Serial.println(WiFi.softAP(nRede) ? "Ready" : "Failed!");

  if (!WiFi.softAP(nRede, vSenhaAP.c_str()))
  {
    // wifireset2();
    DevSet.factoryReset();
  }

  slog("Soft-AP IP address = ");
  slogln(WiFi.softAPIP());
  IpDispositivo = local_IP;
  setBroadcastIP(DevSet.apWifiMSK);

  // chip3.write(LedWifiConnected, HIGH);
  // chip3.write(LedWifiHI, HIGH);
  // chip3.write(LedWifiLOW, HIGH);
}

void SetupPing()
{ // execute ping routine
  for (int i = 0; i < numDNSquery; i++)
  {
    if (ips[i])
    {
      if (!WiFi.hostByName(ips[i], addrs[i]))
        addrs[i].fromString(ips[i]);
    }

    Pings[i].on(true, [](const AsyncPingResponse &response)
                {
                  IPAddress addr(response.addr);
                  if (!response.answer)
                    return false; });

    Pings[i].on(false, [](const AsyncPingResponse &response)
                {
                  IPAddress addr(response.addr);
                  if (response.total_recv > 1)
                  {
                    slogln(F("Internet connected"));
                    numberPingResponse++;
                  }
                  else
                    slogln(F("Internet disconnected"));

                  if (numberPingResponse > 0)
                  {
                    enableConnection = true;
                    hasInternet = true;
                  }
                  else
                  {
                    enableConnection = false;
                    hasInternet = false;
                  }
                  return true; });
  }

  LoopPing();
}

void LoopPing()
{
  if (!client.connected() && (tipoWifiAtual != 2))
  {
    numberPingResponse = 0;
    for (int i = 0; i < numDNSquery; i++)
    {
      if (addrs[i].toString().c_str() != "255.255.255.255")
      {
        Serial.printf("started ping to %s:\n", addrs[i].toString().c_str());
        Pings[i].begin(addrs[i]);
      }
    }
  }
}

void UpdatePing()
{
  slogln(F("Atualizando endereços do ping"));
  for (int i; i < numDNSquery; i++)
  {
    if (ips[i])
    {
      if (!WiFi.hostByName(ips[i], addrs[i]))
        addrs[i].fromString(ips[i]);
    }
  }
}

void setBroadcastIP(uint32_t _currentSubnet)
{
  IPAddress _broadcastIP(_currentSubnet);
  for (int i = 0; i <= 3; i++)
  {
    // slogln(_broadcastIP[i]);
    (_broadcastIP[i] == 0) ? _broadcastIP[i] = 255 : _broadcastIP[i] = IpDispositivo[i];
  }
  broadcastIP = _broadcastIP;
}