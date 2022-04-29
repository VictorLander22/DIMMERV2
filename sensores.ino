void trataDevice(String (&Devices)[20])
{
  String valorDevice = "";
  LittleFS.begin();
  File f = LittleFS.open("/device.txt", "r");
  if (f)
    String valorDevice = f.readStringUntil('*');
  //(!DEBUG_ON) ?:   Serial.println(valorDevice);
  f.close();
  LittleFS.end();

  int posicaoDevice = 0;

  for (uint8_t i = 0; i < 20; i++)
  {
    Devices[i] = "";
  }

  for (int i = 0; i <= valorDevice.length(); i++)
  {
    if (valorDevice[i] != ';')
    {
      if (valorDevice[i] != '*' && int(valorDevice[i]) != 13 && int(valorDevice[i]) != 0)
      {
        Devices[posicaoDevice] += valorDevice[i];
      }
    }
    else
    {
      if (posicaoDevice >= 20)
      {
        posicaoDevice = 20;
      }
      else
      {
        posicaoDevice += 1;
      }
    }
  }
  slogln(valorDevice);
  DeviceAlterado = false;
}

void trataSensores()
{
  uint16_t inValues = sensor2.read8() << 8 | sensor1.read8();
  uint16_t outValues = chip2.read8() << 8 | chip1.read8();
  String sInValues, sOutValues;
  static unsigned long limparUltimoDisparo = 0;

  for (size_t i = 0; i <= 15; i++)
  {
    if (i < 8)
    {
      sInValues += (String)bitRead(inValues, 7 - i);
      sOutValues += (String)bitRead(outValues, 7 - i);
    }
    else
    {
      sInValues += (String)bitRead(inValues, 15 - (i - 8));
      sOutValues += (String)bitRead(outValues, 15 - (i - 8));
    }
  }

  // Envio de UDP em broadcast
  if (millisAtual >= millisSendUDP)
  {
    limparUltimoDisparo++;

    if (limparUltimoDisparo > 1200)
    {
      limparUltimoDisparo = 0;
      ultimoDisparo = "";
    }

    char replyPacekt[255] = "";
    String valInputs = "2934d03" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + sInValues + "|" + sOutValues + "|" + getRSSI() + "*";
    valInputs.toCharArray(replyPacekt, 255);

    SendUDP(broadcastIP, localUdpPort, String(replyPacekt));
    udp.broadcast(replyPacekt);

    millisSendUDP = millisAtual + interval;
  }

  for (int numSensor = 0; numSensor <= 15; numSensor++)
  {
    //  if (SensorAlterado && (sSensor1 != "11111111" || sSensor2 != "11111111"))
    // if (verificaSensores(numSensor, valorSensores[numSensor]))
    if (verificaSensores(numSensor, (String)bitRead(inValues, numSensor)))
    {
      if (msgDisparada[numSensor] == false)
      {
        if (notificar)
        {
          if (nomeSensores[numSensor + 1] != "")
          {
            // sendDataToFirebase("Sensor " + nomeSensores[numSensor + 1] + " disparado", numSensor, "1");
          }
          else
          {
            // sendDataToFirebase("Sensor " + String(numSensor + 1) + " disparado", numSensor, "1");
          }
        }
        else if (enviarsms)
        {
          // sms sendSMS(numSensor);
          slogln("Sensor disparado");
        }
        ultimoDisparo = sInValues; // sSensor1 + sSensor2;
        limparUltimoDisparo = 0;
        slogln("Sensor disparado");
      }
    }
  }
}

// void sendDataToFirebase(String MSG, int numSen, String vTag)
// {
//   WiFiClient cliente;
//   String Devices[20];
//   trataDevice(Devices);
//   String serve = "AAAAKmhc-Lk:APA91bFrNKIu9Ufqj3AmtXBPcd-87smD9kO4Z6CNdBOejY9nGFjvMCBCP2-7eTVqXlqmJpL2o4cTXD6eD4gbkvnpihPFxougju-536CCPZZn62DNTGhgtUFHoBPw0yyq4xmbjb7Bm9IF";

//   String reg = "";

//   for (int i = 0; i <= 9; i++)
//   {
//     //    reg = "APA91bH1RwO17t1VXFfQ4HkQTsvIncLp-SjhGH3WTDtO3_fTDJiOdk43wjNR2-IaOPcEOJvfm-Gp5iRMdk8c-Sy-GNyspsUxK4JS8ZuPo6Nhe9tR9smyrxvSRWBU216mVgTN6UjzdjEj";
//     //    reg = "fshYIY1u_GY:APA91bFXybZ0qcmldA8uIqltpGz8nHImMYV6B2I_PqsCmQiE6WKQFSXJhq0zo3vlIi-h-JZLt5i6HyZbJE9qFGJvA_Qg2ioDdQqfNnIY8Emun_LzEUCO48xXTKs9GcyU9UuaaASkEDOX";
//     reg = String(Devices[i]);
//     //reg.trim();

//     if (reg.length() > 10)
//     {
//       //(!DEBUG_ON) ?:   Serial.println(reg.substring(0,1));
//       //(!DEBUG_ON) ?:   Serial.println(reg.substring(1));
//       String data = "";
//       if (reg.substring(0, 1) == "2")
//       {
//         slogln("Iphone");
//         data = "{";
//         data = data + "\"to\": \"" + reg.substring(1) + "\",";
//         data = data + "\"notification\": {";
//         //data = data + "\"custom_notification\": {";
//         data = data + "\"body\": \"" + MSG + "\",";
//         data = data + "\"title\" : \"" + MSG + "\",";
//         data = data + "\"tag\":\"" + vTag + "\",";
//         data = data + "\"badge\":\"0\",";
//         data = data + "\"content-available\": \"1\",";
//         data = data + "\"sound\":\"default\"";
//         data = data + "},";
//         data = data + "\"data\": {";
//         data = data + "\"app\": \"keepin\",";
//         data = data + "\"msg\": \"" + MSG + "\"";
//         data = data + "}";
//         data = data + " }";
//       }
//       else
//       {
//         data = "{";
//         data = data + "\"to\": \"" + reg.substring(1) + "\",";
//         data = data + "\"notification\": {";
//         //data = data + "\"custom_notification\": {";
//         data = data + "\"body\": \"" + MSG + "\",";
//         data = data + "\"title\" : \"" + MSG + "\",";
//         data = data + "\"tag\":\"" + vTag + "\",";
//         data = data + "\"badge\":\"0\",";
//         data = data + "\"content-available\": \"1\",";
//         data = data + "\"sound\":\"default\"";
//         data = data + "},";
//         data = data + "\"data\": {";
//         data = data + "\"app\": \"keepin\",";
//         data = data + "\"msg\": \"" + MSG + "\"";
//         data = data + "}";
//         data = data + " }";
//       }
//       slogln("Send data...");
//       //      if (cliente.connect("fcm.googleapis.com", 80)) {
//       if (cliente.connect("cloud.fcleal.com.br", 80))
//       {
//         slogln("Connected to the server..");
//         //        cliente.println("POST /fcm/send HTTP/1.1");
//         cliente.println("POST /api/keepin/fcm HTTP/1.1");
//         cliente.println("Authorization: key=" + serve + "");
//         cliente.println("Content-Type: application/json");
//         //        cliente.println("Host: fcm.googleapis.com");
//         cliente.println("Host: cloud.fcleal.com.br");
//         cliente.print("Content-Length: ");
//         cliente.println(data.length());
//         cliente.print("\n");
//         cliente.print(data);
//       }
//       slogln("Data sent...Reading response..");
//       while (cliente.available())
//       {
//         char c = cliente.read();
//         slog(c);
//       }
//       slogln("Finished!");
//       cliente.flush();
//       slogln(data);
//       msgDisparada[numSen] = true;
//       msgDisparadaRF[numSen] = true;
//     }
//   }
//   cliente.stop();
// }

void lersensores()
{
  String sSensor1 = String(sensor1.read8(), BIN);
  String sSensor2 = String(sensor2.read8(), BIN);

  while (sSensor1.length() < 8)
  {
    sSensor1 = '0' + sSensor1;
  }

  while (sSensor2.length() < 8)
  {
    sSensor2 = '0' + sSensor2;
  }

  gRequest->send(200, sdefTextHtml, sSensor1);
}

void AsyncSaveInputConfig()
{
  String Valor = gRequest->arg("s");
  int Indice = gRequest->arg("p").toInt();
  String Senha = gRequest->arg("k");
  String gv = gRequest->arg("gn");
  String nomeS = gRequest->arg("nome");
  // String idAgenda = gRequest->arg("ag");

  String nomesG = "";

  if (gv == "1")
  {
    nomeSensores[Indice - 1] = nomeS;
  }

  if (Senha == "kdi9e")
  {
    for (uint8_t id = 0; id < 16; id++)
    {
      nomesG += nomeSensores[id] + "|";
    }
    nomesG += "*";
    LittleFS.begin();

    // Save input configs
    File f = LittleFS.open("/sensores.txt", "w");
    if (f)
      f.println(Valor);
    f.close();
    // delay(300);

    // Save input names
    f = LittleFS.open("/nsensores.txt", "w");
    if (f)
      f.println(nomesG);
    f.close();

    LittleFS.end();
  }
  SensorAlterado = true;
  gRequest->send(200, sdefTextHtml, sdefOK);
}

void gravasensor2(String Valor)
{
  SensorAlterado = true;

  LittleFS.begin();
  File f = LittleFS.open("/sensores.txt", "w");
  f.println(Valor);
  f.close();
  LittleFS.end();
}

boolean verificaSensores(int nsensor, String vsAtual)
{
  if (SensorAlterado)
  {
    SensorAlterado = false;
    consultaSensor();
  }

  // String texto = consultaAgenda(i);
  String texto = Sensores[nsensor];
  texto.trim();

  String Porta = "";
  String Funcao = "";
  String PortaAparelho = "";
  String Msg = "";
  String vChip = "";
  String vIPDest = "";
  String PortaAparelho2 = "";
  String vChip2 = "";
  String vIPDest2 = "";
  String vTipo1 = "";
  String vTipo2 = "";
  int i3 = 1;
  boolean valorRetorno = false;
  enviarsms = false;
  static unsigned long inicioAcionado[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  static boolean vDisparadoSensor[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};

  // separa dados nas variaveis
  for (uint8_t i2 = 0; i2 <= 100; i2++)
  {
    if (texto[i2] != '|' && i3 <= 11 && texto != "")
    {
      if (i3 == 1)
      {
        Porta += texto[i2];
      }

      if (i3 == 2)
      {
        Funcao += texto[i2];
      }

      if (i3 == 3)
      {
        PortaAparelho += texto[i2];
      }

      if (i3 == 4)
      {
        Msg += texto[i2];
      }

      if (i3 == 5) // ip
      {
        vIPDest += texto[i2];
      }

      if (i3 == 6) // ChipID
      {
        vChip += texto[i2];
      }

      if (i3 == 7) // Porta Aparelho 2
      {
        PortaAparelho2 += texto[i2];
      }

      if (i3 == 8) // IP Destino 2
      {
        vIPDest2 += texto[i2];
      }

      if (i3 == 9) // ChipID 2
      {
        vChip2 += texto[i2];
      }

      if (i3 == 10) // vTipo1 - Se é retenção ou pulso
      {
        vTipo1 += texto[i2];
      }

      if (i3 == 11) // vTipo2 - Se é retenção ou pulso
      {
        vTipo2 += texto[i2];
      }
    }
    else
    {
      i3++;
    }

  } // fim separar dados

  // iniciar tratamento.
  String ValorAtuacao;
  if (Funcao == "0" || Funcao == "2" || Funcao == "3" || Funcao == "8")
  {
    ValorAtuacao = "0";
  }
  else if (Funcao == "1")
  {
    ValorAtuacao = "1";
  }
  else if (Funcao == "4")
  {
    enviarsms = true;
    ValorAtuacao = "0";
  }
  else if (Funcao == "5")
  {
    ValorAtuacao = "0";
  }
  else if (Funcao == "6" || Funcao == "9" || Funcao == "10")
  {
    ValorAtuacao = vsAtual;
  }
  else if (Funcao == "7")
  {
    ValorAtuacao = "0";
  }

  // varial com o status atual da prota

  if (vsAtual == "0")
  {
    estadoAtual[nsensor] = HIGH;
  }
  else
  {
    estadoAtual[nsensor] = LOW;
  }

  if (nsensor == 3)
  {
    //(!DEBUG_ON) ?:   Serial.println(Funcao);
    //(!DEBUG_ON) ?:   Serial.print("estadoAtual: ");
    //(!DEBUG_ON) ?:   Serial.println(estadoAtual[nsensor]);
    //       ultimoEstado[nsensor] = estadoAtual[nsensor];
    //(!DEBUG_ON) ?:   Serial.print("ultimoEstado: ");
    //      (!DEBUG_ON) ?:   Serial.println(ultimoEstado[nsensor]);
  }

  ////
  ///  Reseta os marcadores de acionamento
  if (estadoAtual[nsensor] == ultimoEstado[nsensor] && ValorAtuacao != vsAtual)
  {
    if (Funcao == "7")
    {
      ultimoEstado[nsensor] = estadoAtual[nsensor];
    }
  }

  if (Funcao == "8" && ValorAtuacao != vsAtual) // trata dupla funcao
  {
    if (inicioAcionado[nsensor] > 0)
    {
      if (millisAtual < inicioAcionado[nsensor])
      {
        inicioAcionado[nsensor] = (4294967295 - inicioAcionado[nsensor]) + millisAtual;
      }

      if ((millisAtual - inicioAcionado[nsensor] > 1300) && (vDisparadoSensor[nsensor] == false))
      {
        slogln("maior que 2 segundos"); /// acionamento 2
        inicioAcionado[nsensor] = 0;
        vDisparadoSensor[nsensor] = true;

        IPAddress Destino;
        Destino.fromString(vIPDest2);

        IPAddress Comparacao(192, 168, 4, 1);
        if (Destino == IpDispositivo || IpDispositivo == Comparacao || vTipo2 == "2")
        {
          lastDebounceTime = millisAtual;
          if (vTipo2 == "2") // cenas
          {
            triggerCena(PortaAparelho2);
          }
          else if (vTipo2 == "1")
          {
            LigaDesliga(PortaAparelho2.toInt() - 1, HIGH, "", 1);
          }
          else
          {
            Inverte(PortaAparelho2.toInt() - 1);
          }
          ultimoEstado[nsensor] = estadoAtual[nsensor];
          // LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
        }
        else
        {
          // udp
          char replyPacekt[255] = "";
          String Texto = "";
          if (vTipo2 == "1")
          {
            Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|E|1|";
          }
          else
          {
            Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|I|";
          }
          Texto.toCharArray(replyPacekt, 255);
          slogln("enviado comando UDP1");
          SendUDP(Destino, localUdpPort, String(replyPacekt));
          // Udp.beginPacket(Destino, localUdpPort);
          // Udp.write(replyPacekt);
          // Udp.endPacket();
          msgDisparada[nsensor] = true;

          slogln(replyPacekt);
        }
      }
      else if (vDisparadoSensor[nsensor] == false)
      {
        slogln("Menor que 2 segundos");
        inicioAcionado[nsensor] = 0;
        vDisparadoSensor[nsensor] = true;

        IPAddress Destino;
        Destino.fromString(vIPDest);
        IPAddress Comparacao(192, 168, 4, 1);

        if (Destino == IpDispositivo || IpDispositivo == Comparacao)
        {
          lastDebounceTime = millisAtual;
          if (vTipo1 == "1")
          {
            LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 1);
          }
          else
          {
            Inverte(PortaAparelho.toInt() - 1);
          }
          ultimoEstado[nsensor] = estadoAtual[nsensor];
          // LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
        }
        else
        {
          // udp
          char replyPacekt[255] = "";
          String Texto = "";
          if (vTipo1 == "1")
          {
            Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|1|";
          }
          else
          {
            Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
          }
          Texto.toCharArray(replyPacekt, 255);
          slog("enviar comando UDP2");
          SendUDP(Destino, localUdpPort, String(replyPacekt));
          // Udp.beginPacket(Destino, localUdpPort);
          // Udp.write(replyPacekt);
          // Udp.endPacket();
          msgDisparada[nsensor] = true;
          slogln(replyPacekt);
        }
      }
    }
  }
  else if (Funcao == "8" && ValorAtuacao == vsAtual) // caso tenha ficado mais de 2 segundos ligado já executa.
  {
    if (millisAtual < inicioAcionado[nsensor])
    {
      inicioAcionado[nsensor] = (4294967295 - inicioAcionado[nsensor]) + millisAtual;
    }

    if ((millisAtual - inicioAcionado[nsensor] > 1300) && (vDisparadoSensor[nsensor] == false))
    {
      slogln("maior que 2 segundos");
      inicioAcionado[nsensor] = 0;
      vDisparadoSensor[nsensor] = true;

      IPAddress Destino;
      Destino.fromString(vIPDest2);
      IPAddress Comparacao(192, 168, 4, 1);

      if (Destino == IpDispositivo || IpDispositivo == Comparacao || vTipo2 == "2")
      {
        lastDebounceTime = millisAtual;
        if (vTipo2 == "2") // cenas
        {
          triggerCena(PortaAparelho2);
        }
        else if (vTipo2 == "1")
        {
          LigaDesliga(PortaAparelho2.toInt() - 1, HIGH, "", 1);
        }
        else
        {
          Inverte(PortaAparelho2.toInt() - 1);
        }
        ultimoEstado[nsensor] = estadoAtual[nsensor];
        // LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
      }
      else
      {
        // udp
        char replyPacekt[255] = "";
        String Texto = "";
        if (vTipo2 == "1")
        {
          Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|E|1|";
        }
        else
        {
          Texto = vIPDest2 + "|" + PortaAparelho2 + "|false|" + String(vChip2) + "|I|";
        }
        Texto.toCharArray(replyPacekt, 255);
        slogln("enviado comando UDP3");
        SendUDP(Destino, localUdpPort, String(replyPacekt));
        // Udp.beginPacket(Destino, localUdpPort);
        // Udp.write(replyPacekt);
        // Udp.endPacket();
        msgDisparada[nsensor] = true;
        slogln(replyPacekt);
      }
    }
  }

  if (Msg == "1")
  {
    valorRetorno = true;
  }

  if (ValorAtuacao == vsAtual && estadoAtual[nsensor] != ultimoEstado[nsensor])
  {
    if (Funcao == "0" || Funcao == "1" || Funcao == "4")
    {
      valorRetorno = true;
    }
    else if (Funcao == "2") // liga porta
    {
      IPAddress Destino;
      Destino.fromString(vIPDest);
      IPAddress Comparacao(192, 168, 4, 1);

      if (Destino == IpDispositivo || IpDispositivo == Comparacao)
      {
        LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 0);
      }
      else // envia udp
      {
        char replyPacekt[255] = "";
        String Texto = vIPDest + "|" + PortaAparelho + "|true|" + String(vChip) + "|E|0|";
        slogln(Texto);
        Texto.toCharArray(replyPacekt, 255);
        slogln("enviado comando UDP4");
        SendUDP(Destino, localUdpPort, String(replyPacekt));
        // Udp.beginPacket(Destino, localUdpPort);
        // Udp.write(replyPacekt);
        // Udp.endPacket();
        msgDisparada[nsensor] = true;
        slogln(replyPacekt);
      }
      if (Msg == "1")
      {
        valorRetorno = true;
      }
      else
      {
        valorRetorno = false;
      }
    }
    else if (Funcao == "3") // desliga porta
    {
      IPAddress Destino;
      Destino.fromString(vIPDest);
      IPAddress Comparacao(192, 168, 4, 1);

      if (Destino == IpDispositivo || IpDispositivo == Comparacao)
      {
        LigaDesliga(PortaAparelho.toInt() - 1, LOW, "", 0);
      }
      else
      {
        char replyPacekt[255] = "";
        String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|0|";
        Texto.toCharArray(replyPacekt, 255);
        slogln("enviado comando UDP5");
        SendUDP(Destino, localUdpPort, String(replyPacekt));
        // Udp.beginPacket(Destino, localUdpPort);
        // Udp.write(replyPacekt);
        // Udp.endPacket();
        msgDisparada[nsensor] = true;
        slogln(replyPacekt);
      }
      if (Msg == "1")
      {
        valorRetorno = true;
      }
      else
      {
        valorRetorno = false;
      }
    }
    else if (Funcao == "5")
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          IPAddress Destino;
          Destino.fromString(vIPDest);
          IPAddress Comparacao(192, 168, 4, 1);

          if (Destino == IpDispositivo || IpDispositivo == Comparacao)
          {
            lastDebounceTime = millisAtual;
            Inverte(PortaAparelho.toInt() - 1);
            ultimoEstado[nsensor] = estadoAtual[nsensor];
            //  delay(300);
          }
          else
          {
            // udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
            Texto.toCharArray(replyPacekt, 255);
            slogln("enviado comando UDP6");
            SendUDP(Destino, localUdpPort, String(replyPacekt));
            // Udp.beginPacket(Destino, localUdpPort);
            // Udp.write(replyPacekt);
            // Udp.endPacket();
            msgDisparada[nsensor] = true;
            slogln(replyPacekt);
          }
        }
      }
      // valorRetorno = false;
    }
    else if (Funcao == "6") // interruptor normal
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          IPAddress Destino;
          Destino.fromString(vIPDest);
          IPAddress Comparacao(192, 168, 4, 1);

          if (Destino == IpDispositivo || Destino == Comparacao)
          {
            lastDebounceTime = millisAtual;
            Inverte(PortaAparelho.toInt() - 1);
            ultimoEstado[nsensor] = estadoAtual[nsensor];
            // LigaDesliga(PortaAparelho.toInt()-1, HIGH, "", 0);
          }
          else
          {
            // udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
            Texto.toCharArray(replyPacekt, 255);
            slog("enviado comando UDP7");
            slogln(Texto);
            SendUDP(Destino, localUdpPort, String(replyPacekt));
            // Udp.beginPacket(Destino, localUdpPort);
            // Udp.write(replyPacekt);
            // Udp.endPacket();
            msgDisparada[nsensor] = true;
            slogln(replyPacekt);
          }
        }
      }
      else if (estadoAtual[nsensor] == LOW && ultimoEstado[nsensor] == HIGH)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          lastDebounceTime = millisAtual;
          Inverte(PortaAparelho.toInt() - 1);
          // LigaDesliga(PortaAparelho.toInt()-1, LOW, "", 0);
          ultimoEstado[nsensor] = estadoAtual[nsensor];
        }
      }
      //   valorRetorno = false;
    }
    else if (Funcao == "7")
    {
      IPAddress Destino;
      Destino.fromString(vIPDest);
      IPAddress Comparacao(192, 168, 4, 1);

      if ((Destino == IpDispositivo && ultimoEstado[nsensor] != estadoAtual[nsensor]) || (ultimoEstado[nsensor] != estadoAtual[nsensor] && IpDispositivo == Comparacao))
      {
        LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 1);
        ultimoEstado[nsensor] = estadoAtual[nsensor];
      }
      //      else if (Destino == IpDispositivo && ultimoEstado[Porta.toInt()] == estadoAtual[nsensor])
      //      {
      //        if (millisAtual - lastPulso > 10000 || millisAtual - lastPulso < 0)
      //        {
      //          ultimoEstado[Porta.toInt()] = !estadoAtual[nsensor];
      //          lastPulso = millisAtual;
      //        }
      //      }
      else if (Destino != IpDispositivo && ultimoEstado[Porta.toInt()] != estadoAtual[nsensor] && IpDispositivo != Comparacao)
      {
        // udp
        char replyPacekt[255] = "";
        String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|E|1|";
        Texto.toCharArray(replyPacekt, 255);
        slogln("enviado comando UDP8");
        SendUDP(Destino, localUdpPort, String(replyPacekt));
        // Udp.beginPacket(Destino, localUdpPort);
        // Udp.write(replyPacekt);
        // Udp.endPacket();
        msgDisparada[nsensor] = true;
        ultimoEstado[nsensor] = estadoAtual[nsensor];
        slogln(replyPacekt);
      }
    }
    else if (Funcao == "8")
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {

          if (inicioAcionado[nsensor] == 0)
          {
            inicioAcionado[nsensor] = millisAtual;
            vDisparadoSensor[nsensor] = false;
          }
        }
      }

      //     valorRetorno = false;
    }
    else if (Funcao == "9") // interruptor invertido
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          IPAddress Destino;
          Destino.fromString(vIPDest);
          IPAddress Comparacao(192, 168, 4, 1);

          if (Destino == IpDispositivo || IpDispositivo == Comparacao)
          {
            lastDebounceTime = millisAtual;
            // Inverte(PortaAparelho.toInt()-1);
            ultimoEstado[nsensor] = estadoAtual[nsensor];
            LigaDesliga(PortaAparelho.toInt() - 1, LOW, "", 0);
          }
          else
          {
            // udp
            char replyPacekt[255] = "";
            String Texto = vIPDest + "|" + PortaAparelho + "|false|" + String(vChip) + "|I|";
            Texto.toCharArray(replyPacekt, 255);
            slogln("enviado comando UDP9");
            SendUDP(Destino, localUdpPort, String(replyPacekt));
            // Udp.beginPacket(Destino, localUdpPort);
            // Udp.write(replyPacekt);
            // Udp.endPacket();
            msgDisparada[nsensor] = true;
            slogln(replyPacekt);
          }
        }
      }
      else if (estadoAtual[nsensor] == LOW && ultimoEstado[nsensor] == HIGH)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          lastDebounceTime = millisAtual;
          // Inverte(PortaAparelho.toInt()-1);
          LigaDesliga(PortaAparelho.toInt() - 1, HIGH, "", 0);
          ultimoEstado[nsensor] = estadoAtual[nsensor];
        }
      }
      //   valorRetorno = false;
    }
    else if (Funcao == "10") // Interruptor Cena
    {
      if (estadoAtual[nsensor] == HIGH && ultimoEstado[nsensor] == LOW)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          // cena1
          triggerCena(PortaAparelho);
        }
      }
      else if (estadoAtual[nsensor] == LOW && ultimoEstado[nsensor] == HIGH)
      {
        if ((millisAtual - lastDebounceTime) > debounceDelay)
        {
          // cena2
          triggerCena(PortaAparelho2);
        }
      }
      valorRetorno = false;
    }
  }
  else
  {
    if (msgDisparada[nsensor] == true)
    {
      msgDisparada[nsensor] = false;
    }
    valorRetorno = false;
  }
  ultimoEstado[nsensor] = estadoAtual[nsensor];
  // slogln(ultimoEstado[nsensor]);
  return valorRetorno;
}

void consultaSensor()
{

  String texto = "";
  String nomeS = "";
  LittleFS.begin();

  File f = LittleFS.open("/sensores.txt", "r");
  if (f)
    texto = f.readStringUntil('*');
  texto += '*';
  f.close();
  delay(300);
  f = LittleFS.open("/nsensores.txt", "r");
  if (f)
    nomeS = f.readStringUntil('*');
  nomeS += '*';
  f.close();

  LittleFS.end();

  int posicao = 0;
  int contador = 0;
  int i = 0;
  String textoSensor = "";
  if (texto.length() > 4)
  {
    Sensores[i] = "";
    while (texto[posicao] != '*')
    {
      if (texto[posicao] != '|')
      {
        textoSensor += texto[posicao];
      }
      else if (texto[posicao] == '|' && contador < 11)
      {
        contador++;
        textoSensor += texto[posicao];
      }

      if (contador == 11)
      {
        Sensores[i] = textoSensor;
        textoSensor = "";
        contador = 0;
        i++;
      }
      posicao++;
    }
  }
  // nomes dos sensores
  posicao = 0;
  i = 0;
  String textoNome = "";

  if (nomeS.length() > 4)
  {
    nomeSensores[i] = "";
    while (nomeS[posicao] != '*')
    {
      if (nomeS[posicao] != '|')
      {
        textoNome += nomeS[posicao];
      }
      else if (nomeS[posicao] == '|')
      {
        nomeSensores[i] = textoNome;
        textoNome = "";
        i++;
      }
      posicao++;
    }
  }

  for (uint8_t i = 0; i < 16; i++)
  {
    slogln("ConsultaSensor[" + (String)i + "] - " + Sensores[i] + " - " + nomeSensores[i]);
  }
}

String lerSensor()
{

  String texto;
  LittleFS.begin();
  File f = LittleFS.open("/sensores.txt", "r");
  if (f)
    texto = f.readStringUntil('*');
  f.close();
  LittleFS.end();

  return texto;
}

void consensor()
{

  String Senha = gRequest->arg("k");

  if (Senha == "kdi9e")
  {
    gRequest->send(200, sdefTextHtml, lerSensor());
  }
  else
  {
    gRequest->send(200, sdefTextHtml, "-1");
  }
}

void gravadevice()
{

  gRequest->send(200, sdefTextHtml, sdefOK);
  String Valor = gRequest->arg("d");
  // String Senha = ;

  if (gRequest->arg("k") == "kdi9e")
  {
    LittleFS.begin();
    File f = LittleFS.open("/device.txt", "w");

    // if (!f)
    // {
    //   LittleFS.format();
    //   File f = LittleFS.open("/device.txt", "w");
    // }

    f.println(Valor);

    f.close();
    LittleFS.end();
    DeviceAlterado = true;
    slogln("gravado device");
  }
}

void buscadevice()
{
  if (gRequest->arg("k") == "kdi9e")
  {
    LittleFS.begin();
    File f = LittleFS.open("/device.txt", "r");
    if (!f)
    {
      gRequest->send(200, sdefTextHtml, "");
    }
    else
    {
      String valorDevice = f.readStringUntil('*');
      gRequest->send(200, sdefTextHtml, valorDevice);
    }
    f.close();
    LittleFS.end();
  }
}

void ultimodisp()
{
  if (gRequest->arg("k") == "kdi9e")
  {
    gRequest->send(200, sdefTextHtml, ultimoDisparo);
  }
}

void retornaNotificar()
{
  LittleFS.begin();
  File f = LittleFS.open("/notific.txt", "r");
  if (f)
  {
    String valor = f.readStringUntil('*');
    if (valor == "true")
      notificar = true;
    else
      notificar = false;
  }

  f.close();
  LittleFS.end();
}

void buscaNotificar()
{

  String Senha = gRequest->arg("k");

  if (Senha == "kdi9e")
  {
    gRequest->send(200, sdefTextHtml, String(notificar));
  }
  else
  {
    gRequest->send(200, sdefTextHtml, "0");
  }
}

void gravanot()
{
  gRequest->send(200, sdefTextHtml, sdefOK);
  String Valor = gRequest->arg("v");
  String Senha = gRequest->arg("k");

  if (Senha == "kdi9e")
  {
    LittleFS.begin();
    File f = LittleFS.open("/notific.txt", "w");

    f.println(Valor);

    f.close();
    LittleFS.end();
    slogln("gravado notificacao");

    if (Valor == "true")
    {
      notificar = true;
    }
    else
    {
      notificar = false;
    }
  }
}

void gravanot2(String Valor)
{

  LittleFS.begin();
  File f = LittleFS.open("/notific.txt", "w");

  // if (!f)
  // {
  //   LittleFS.format();
  //   File f = LittleFS.open("/notific.txt", "w");
  // }

  f.println(Valor);

  f.close();
  LittleFS.end();
  slogln("gravado notificacao");

  if (Valor == "true")
  {
    notificar = true;
  }
  else
  {
    notificar = false;
  }
}

void LoadInputs()
{
  uint16_t inputs = getInputs();
  for (int i = 0; i < 16; i++)
  {
    estadoAtual[i] = (bitRead(inputs, i));
    ultimoEstado[i] = estadoAtual[i];
  }
}