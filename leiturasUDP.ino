void SetupUDP()
{
  if (udp.listen(localUdpPort))
  {
    udp.onPacket([](AsyncUDPPacket packet)
                 { leituraUDP(packet); });
  }
}

void leituraUDP(AsyncUDPPacket packet)
{
  String cabecalho = String((char *)packet.data());
  //slogln(cabecalho);
  if (packet.length() > 16)
  {

    if (cabecalho.substring(0, 7) != "2934d03")
    {
      if (cabecalho.substring(0, 3) == "pw=")
      {

        SendUDP(packet.remoteIP(), 35701, getDevStatus());
      }
      else
      {

        String sIP = "";
        String Porta = "";
        int i2 = 0;
        String funcao = "";
        String ChipId = "";
        String Tipo = "";
        String Texto = "";
        String Telefone = "";

        for (int i = 0; i < packet.length(); i++)
        {
          if (cabecalho[i] != '|' && i2 <= 6)
          {
            if (i2 == 0)
            {
              sIP += cabecalho[i];
            }

            if (i2 == 1)
            {
              Porta += cabecalho[i];
            }
            if (i2 == 2)
            {
              funcao += cabecalho[i];
            }
            if (i2 == 3)
            {
              ChipId += cabecalho[i];
            }
            if (i2 == 4)
            {
              Tipo += cabecalho[i];
            }
            if (i2 == 5)
            {
              Texto += cabecalho[i];
            }
            if (i2 == 6)
            {
              Telefone += cabecalho[i];
            }
          }
          else
          {
            i2++;
          }
        }

        IPAddress Destino;
        Destino.fromString(sIP);

        if (Destino == IpDispositivo)
        {
          Porta = String(retornaPorta(Porta.toInt()));
          if (Tipo == "E")
          {
            if (funcao == "true")
            {
              //digitalWrite(Porta.toInt(), 1);
              LigaDesliga(Porta.toInt(), HIGH, "", Texto.toInt());
              slogln("led ligado UDP");
            }
            else
            {
              //digitalWrite(Porta.toInt(), 0);
              LigaDesliga(Porta.toInt(), LOW, "", Texto.toInt());
              slogln("led 1 desligado UDP");
            }
          }
          else if (Tipo == "C")
          {
            if (LePorta(Porta.toInt()) == HIGH)
            {
              //gRequest->send(200, sdefTextHtml, "true");

              Tipo = "R";
              funcao = "true";
              char replyPacekt[255] = "";
              IPAddress ipRetorno = packet.remoteIP();
              String Texto = String(ipRetorno[0]) + "." + String(ipRetorno[1]) + "." + String(ipRetorno[2]) + "." + String(ipRetorno[3]) + "|" + Porta + "|" + funcao + "|" + ChipId + "|" + Tipo + "|";
              Texto.toCharArray(replyPacekt, 255);

              SendUDP(packet.remoteIP(), packet.remotePort(), String(replyPacekt));
            }
            else
            {
              //gRequest->send(200, sdefTextHtml, "false");

              Tipo = "R";
              funcao = "false";
              char replyPacekt[255] = "";
              IPAddress ipRetorno = packet.remoteIP();
              String Texto = String(ipRetorno[0]) + "." + String(ipRetorno[1]) + "." + String(ipRetorno[2]) + "." + String(ipRetorno[3]) + "|" + Porta + "|" + funcao + "|" + ChipId + "|" + Tipo + "|";
              Texto.toCharArray(replyPacekt, 255);

              //(!DEBUG_ON) ?:   Serial.println("enviado comando UDP");
              SendUDP(packet.remoteIP(), packet.remotePort(), String(replyPacekt));
            }
          }
          else if (Tipo == "R")
          {
            if (funcao == "true")
            {
              //gRequest->send(200, sdefTextHtml, "true");
            }
            else
            {
              //gRequest->send(200, sdefTextHtml, "false");
            }
          }
          else if (Tipo == "S")
          {
            if (ultimoDisparo == "")
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

              //gRequest->send(200, sdefTextHtml, sSensor1 + sSensor2);
            }
            else
            {
              //gRequest->send(200, sdefTextHtml, ultimoDisparo);
            }
          }
          else if (Tipo == "N")
          {
            //gRequest->send(200, sdefTextHtml, String(notificar));
          }
          else if (Tipo == "A")
          {
            //gRequest->send(200, sdefTextHtml, sdefOK);
            gravasensor2(Texto);
          }
          else if (Tipo == "B")
          {
            //gRequest->send(200, sdefTextHtml, sdefOK);
            String Texto = ""; //gRequest->arg("j");
            String Telef = ""; //gRequest->arg("b");
            //sms gravasms2(Texto, Telef);
          }
          else if (Tipo == "D")
          {
            //gRequest->send(200, sdefTextHtml, consultasms2());
          }
          else if (Tipo == "F")
          {
            //gRequest->send(200, sdefTextHtml, sdefOK);
            String Valor = ""; //gRequest->arg("j");
            gravanot2(Valor);
          }
          else if (Tipo == "X") // retorno udp valor do dimmer
          {
            //gRequest->send(200, sdefTextHtml, funcao);
          }
          else if (Tipo == "I")
          {
            Inverte(Porta.toInt());
          }
          else if (Tipo == "H")
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

            if (DeviceAlterado)
            {
              //trataDevice();
            }

            String sChip1 = String(chip1.read8(), BIN);
            String sChip2 = String(chip2.read8(), BIN);

            while (sChip1.length() < 8)
            {
              sChip1 = '0' + sChip1;
            }

            while (sChip2.length() < 8)
            {
              sChip2 = '0' + sChip2;
            }

            // separa os valores ligos no array
            String valorSensores[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
            int posicaoSensor;

            for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
            {
              valorSensores[posicaoSensor] = sSensor1.substring(7 - posicaoSensor, 8 - posicaoSensor);
            }

            for (posicaoSensor = 0; posicaoSensor <= 7; posicaoSensor++)
            {
              valorSensores[posicaoSensor + 8] = sSensor2.substring(7 - posicaoSensor, 8 - posicaoSensor);
            }

            // int32_t rssi;
            // rssi = WiFi.RSSI();
            char replyPacekt[255] = "";
            String valSensores = sSensor1 + sSensor2;
            valSensores = "2934d03" + String(IpDispositivo[0]) + "." + String(IpDispositivo[1]) + "." + String(IpDispositivo[2]) + "." + String(IpDispositivo[3]) + "|" + valSensores + "|" + sChip1 + sChip2 + "|" + getRSSI() + "*";
            valSensores.toCharArray(replyPacekt, 255);
            //retorna udp

            //(!DEBUG_ON) ?:   Serial.println(Contador);
            //    Contador = 0;
            SendUDP(packet.remoteIP(), packet.remotePort(), String(replyPacekt));
          }

          //retorna udp
          char replyPacekt[255] = "";
          String Texto = sdefOK;
          Texto.toCharArray(replyPacekt, 255);

          //(!DEBUG_ON) ?:   Serial.println("enviado comando UDP");
          SendUDP(packet.remoteIP(), packet.remotePort(), String(replyPacekt));
        }
      }
    }
  }
  else if (packet.length() == 2)
  {
    //gRequest->send(200, sdefTextHtml, sdefOK);
  }
}

void SendUDP(IPAddress IP, uint16_t port, String msg)
{
  AsyncUDP udpSend;
  //Serial.println(udpSend.connected());
  if (udpSend.connect(IP, port))
  {
    //Serial.println(udpSend.connected());
    udpSend.print(msg);
    udpSend.close();
    // Serial.println(udpSend.connected());
  }
  //Serial.println(udpSend.connected());
}