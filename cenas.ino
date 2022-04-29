void gravacena()
{
  String Senha = gRequest->arg("k");

  if (Senha == "kdi9e")
  {
    String ia = gRequest->arg("ia");
    String vTipo = gRequest->arg("t");
    String Valor = gRequest->arg("v");
    String IdCena = gRequest->arg("id");
    // slogln(Valor);
    LittleFS.begin();

    if (ia == "1") // inicio de arquivo
    {
      File rFile = LittleFS.open("/ce_" + IdCena + ".cfg", "w");
      rFile.println(Valor);
      rFile.close();
    }
    else
    {
      File rFile = LittleFS.open("/ce_" + IdCena + ".cfg", "a");
      rFile.print(Valor); // se colocar println esta dando erro ao acrescentar o caractere final
      rFile.print('\n');
      rFile.close();
    }

    LittleFS.end();
    // lerArquivo(IdCena);
    // delay(50);
    gRequest->send(200, sdefTextHtml, sdefOK);
  }
  else
  {
    gRequest->send(200, sdefTextHtml, "-1");
  }
}

void lerArquivo(String id)
{
  LittleFS.begin();

  File rFile = LittleFS.open("/ce_" + id + ".cfg", "r");
  String linhas;
  // linhas = rFile.readString();

  //(!DEBUG_ON) ?:   Serial.println("linhas das cenas");
  int qtde = 0;
  while (rFile.available())
  {
    String linhas = rFile.readStringUntil('\n');
    slog(linhas);
    qtde++;
  }

  rFile.close();
  LittleFS.end();
  //(!DEBUG_ON) ?:   Serial.print("qtde de linhas: ");
  slogln(qtde);
}

void triggerCena(String arq)
{
  cenaExecucao = true;
  cenaPAtual = 0;
  cenaPTotal = 0;
  ArqCena = arq;
}

void checkCena()
{
  if (cenaExecucao == true)
  {
    String Comando;
    static File rFile;
    LittleFS.begin();
    if (cenaPAtual == 0) // abre spiff e mantem aberto
    {
      rFile = LittleFS.open("/ce_" + ArqCena + ".cfg", "r");

      while (rFile.available())
      {

        String linhas = rFile.readStringUntil('\n');
        if (cenaPTotal == 0)
        {
          Comando = linhas;
        }
        cenaPTotal++;
      }
      slogln("Total cena: " + String(cenaPTotal));
      cenaPAtual++;
      slogln("Cena atual: " + String(cenaPAtual));

      rFile.close();
      LittleFS.end();
    }
    if (cenaPAtual >= 1)
    {
      //(!DEBUG_ON) ?:   Serial.print("processando cena: ");
      //(!DEBUG_ON) ?:   Serial.println(cenaPAtual);

      if (cenaPAtual == 1)
      {
        //(!DEBUG_ON) ?:   Serial.print(Comando);
        //(!DEBUG_ON) ?:   Serial.println("primeiro");
        //(!DEBUG_ON) ?:   Serial.println("Cena atual: " + String(cenaPAtual));
        slogln("Executar cena: " + Comando);
        executaCena(Comando);
        //(!DEBUG_ON) ?:   Serial.println("Exceutou cena primeiro");
      }
      else
      {
        slogln("Cena atual: " + String(cenaPAtual));
        // rFile.seek(0, SeekSet);

        LittleFS.begin();
        rFile = LittleFS.open("/ce_" + ArqCena + ".cfg", "r");
        int conCena = 1;

        while (rFile.available())
        {
          //(!DEBUG_ON) ?:   Serial.println("\nInicio While");
          //(!DEBUG_ON) ?:   Serial.println("conCena while: " + String(conCena));
          //(!DEBUG_ON) ?:   Serial.println("Cena atual while: " + String(cenaPAtual));

          String linhas = rFile.readStringUntil('\n');

          //(!DEBUG_ON) ?:   Serial.println("Codigo lido arquivo: " + linhas + "Posição no arquivo: " + rFile.position());

          if (conCena == cenaPAtual)
          {

            Comando = linhas;
            //(!DEBUG_ON) ?:   Serial.println("Executar cena while: " + Comando);
            //(!DEBUG_ON) ?:   Serial.println("Cena atual antes execução: " + String(cenaPAtual));
            slogln("Executar cena: " + Comando);
            executaCena(Comando);
            //(!DEBUG_ON) ?:   Serial.println("Cena atual apos execução" + String(cenaPAtual));
            rFile.close();
            LittleFS.end();
            // conCena = 0;
          }
          conCena++;

          //(!DEBUG_ON) ?:   Serial.println("ConCena fim while: " + String(conCena));
        }
        //(!DEBUG_ON) ?:   Serial.println("Sai do while");
      }
    }

    if (cenaPAtual > cenaPTotal)
    {
      rFile.close();
      LittleFS.end();
      slogln("\nfim da cena\n");
      cenaExecucao = false;
      cenaPAtual = 0;
      cenaPTotal = 0;
    }
  }
}

void executaCena(String comandoCena)
{
  //(!DEBUG_ON) ?:   Serial.print(comandoCena);
  String cmdChipID;
  String cmdIP;
  String cmdTipo;
  String cmdPorta;
  String cmdAcao;
  String cmdAcao2;
  String cmdQtde;
  String cmdValorMin;
  String cmdModelo;
  int i;
  int posicaoi = 1;
  int posicaof = 4;

  for (i = 0; i < comandoCena.length(); i++)
  {
    //(!DEBUG_ON) ?:   Serial.println("Posicao " + String(i));
    if (comandoCena[i] != '|' && posicaoi <= posicaof)
    {
      if (posicaoi == 1) // chip id
      {
        cmdChipID += comandoCena[i];
      }
      if (posicaoi == 2) // IP
      {
        cmdIP += comandoCena[i];
      }
      if (posicaoi == 3) // Tipo do comando (1 - Rele; 2 - IR; 3 - Intervalo; 4 - Dimmer)
      {
        cmdTipo += comandoCena[i];
      }
      if (posicaoi == 4)
      {
        if (cmdTipo == "1")
        {
          posicaof = 5;
          cmdPorta += comandoCena[i];
        }
        else if (cmdTipo == "2")
        {
          posicaof = 8;
          cmdQtde += comandoCena[i];
        }
        else if (cmdTipo == "3")
        {
          posicaof = 4;
          cmdAcao += comandoCena[i];
        }
        else if (cmdTipo == "4")
        {
          posicaof = 6;
          cmdPorta += comandoCena[i];
        }
      }
      else if (posicaoi == 5)
      {
        if (cmdTipo == "1")
        {
          cmdAcao += comandoCena[i];
        }
        else if (cmdTipo == "2")
        {
          cmdAcao += comandoCena[i];
        }
        else if (cmdTipo == "4")
        {
          cmdValorMin += comandoCena[i];
        }
      }
      else if (posicaoi == 6)
      {
        if (cmdTipo == "2")
        {
          cmdAcao2 += comandoCena[i];
          //          cmdPorta += comandoCena[i];
        }
        else if (cmdTipo == "4")
        {
          cmdAcao += comandoCena[i];
        }
      }
      else if (posicaoi == 7)
      {
        if (cmdTipo == "2")
        {
          cmdPorta += comandoCena[i];
          // cmdModelo += comandoCena[i];
        }
      }
      else if (posicaoi == 8)
      {
        if (cmdTipo == "2")
        {
          cmdModelo += comandoCena[i];
        }
      }
    }
    else // encontrou o caracter '|'
    {
      posicaoi++;
    }
  }

  IPAddress Destino;
  Destino.fromString(cmdIP);
  String Texto;
  // comandos
  //(!DEBUG_ON) ?:   Serial.println("Tipo " + cmdTipo);
  if (cmdTipo == "1") // Rele
  {
    if (Destino == IpDispositivo)
    {
      if (cmdAcao == "0") // desliga
      {
        LigaDesliga(cmdPorta.toInt() - 1, LOW, "", 0);
      }
      else if (cmdAcao == "1") // liga
      {
        LigaDesliga(cmdPorta.toInt() - 1, HIGH, "", 0);
      }
      else if (cmdAcao == "2") // pulso
      {
        LigaDesliga(cmdPorta.toInt() - 1, LOW, "", 1);
      }
      cenaPAtual++;
    }
    else // upd
    {
      char replyPacekt[255] = "";
      if (cmdAcao == "0") // desliga
      {
        Texto = cmdIP + "|" + cmdPorta + "|false|" + String(cmdChipID) + "|E|0|";
      }
      else if (cmdAcao == "1") // liga
      {
        Texto = cmdIP + "|" + cmdPorta + "|true|" + String(cmdChipID) + "|E|0|";
      }
      else if (cmdAcao == "2") // pulso
      {
        Texto = cmdIP + "|" + cmdPorta + "|false|" + String(cmdChipID) + "|E|1|";
      }
      slogln(Texto);
      Texto.toCharArray(replyPacekt, 255);
      slogln("enviado comando UDP");
      SendUDP(Destino, localUdpPort, String(replyPacekt));
      // Udp.beginPacket(Destino, localUdpPort);
      // Udp.write(replyPacekt);
      // Udp.endPacket();
      delay(200);
      cenaPAtual++;
    }
  }
  else if (cmdTipo == "2") // IR
  {
    if (Destino == IpDispositivo)
    {
      if (lastCnTime == 0 || (millisAtual - lastCnTime >= 300))
      {
        //(!DEBUG_ON) ?:   Serial.println("CenaAntes " + String(cenaPAtual));

               lastCnTime = millisAtual;
        // lastCnTime = millis();
        //(!DEBUG_ON) ?:   Serial.println("terminou IR");
        //  (!DEBUG_ON) ?:   Serial.println("Cena apos ir " + String(cenaPAtual));
        cenaPAtual++;
        // (!DEBUG_ON) ?:   Serial.println("Cena apos ir + 1 " + String(cenaPAtual));
        // delay(300);
      }
      else if (millisAtual - lastCnTime < 0)
      {
        lastCnTime = millisAtual;
        // lastCnTime = millis();
      }
    }
    else // upd - não implentado UPD para IR
    {
    }
  }
  else if (cmdTipo == "3") // timer
  {
    //(!DEBUG_ON) ?:   Serial.println("Entrou no timer " + String(lastCnTime));
    if (lastCnTime <= 0)
    {
      lastCnTime = millisAtual;
    }

    if (lastCnTime == 0 || (millisAtual - lastCnTime >= cmdAcao.toInt()))
    {
      cenaPAtual++;
      lastCnTime = -1;
    }
    else if (millisAtual - lastCnTime < 0)
    {
      lastCnTime = millisAtual;
    }
  }
  else if (cmdTipo == "4") // Dimmer
  {
    char replyPacekt[255] = "";
    String Texto = cmdIP + "|" + cmdPorta + "|" + cmdAcao + "|" + cmdChipID + "|X|";

    Texto += cmdValorMin + "|n|";
    Texto.toCharArray(replyPacekt, 255);

    slogln("enviado comando UDP");
    SendUDP(Destino, localUdpPort, String(replyPacekt));
    // Udp.beginPacket(Destino, localUdpPort);
    // Udp.write(replyPacekt);
    // Udp.endPacket();

    cenaPAtual++;
  }
  else
  {
    cenaPAtual++;
  }
}