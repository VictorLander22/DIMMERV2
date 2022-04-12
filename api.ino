void api()
{
  String vPassApi, action, apiPort, apiSource, valueApi, typeApi, formatApi, apiIndex, ret = "-1";
  bool isPost = false;
  if (!newMqttMsg)
  {
    isPost = true;
    vPassApi = gRequest->arg("pw");
    apiIndex = gRequest->arg("i");
    vPassApi.toLowerCase();
    action = gRequest->arg("a");
    apiPort = gRequest->arg("p");
    apiSource = gRequest->arg("s");
    valueApi = gRequest->arg("v");
    typeApi = gRequest->arg("t");
    formatApi = gRequest->arg("f"); // criado para a apliacao do dimmer(line 295)
    // if (action == "i")
    //{
    //   irNumBits = gRequest->arg("qt1").toInt();
    //  irModel = gRequest->arg("m1").toInt();
    //  irPort = gRequest->arg("p1").toInt();
    //   irData = gRequest->arg("c1") + gRequest->arg("c12");
    //  }
  }
  else
  {
    newMqttMsg = false;
    // isPost = false;
    vPassApi = MqttArg(msgMqtt, "pw");
    apiIndex = MqttArg(msgMqtt, "i");
    vPassApi.toLowerCase();
    action = MqttArg(msgMqtt, "a");
    apiPort = MqttArg(msgMqtt, "p");
    apiSource = MqttArg(msgMqtt, "s");
    valueApi = MqttArg(msgMqtt, "v");
    typeApi = MqttArg(msgMqtt, "t");
    formatApi = MqttArg(msgMqtt, "f"); // criado para a apliacao do dimmer(line 295)
    // if (action == "i")
    //  {
    //    irNumBits = MqttArg(msgMqtt, "qt1").toInt();
    //    irModel = MqttArg(msgMqtt, "m1").toInt();
    //   irPort = MqttArg(msgMqtt, "p1").toInt();
    //   irData = MqttArg(msgMqtt, "c1");
    //   }
  }
  if (AlowApi == true && vPassApi == ApiPass)
  {
    slogln(action);
    if (action == "q") // consulta
    {
      // todas portas
      if (apiPort == "a")
      {
        String sDados1 = ""; //(char *)malloc(64);
        String sDados2 = ""; //(char *)malloc(64);

        if (apiSource == "o") // saidas
        {
          // sDados1 = String(chip1.read8(), BIN);
          // sDados2 = String(chip2.read8(), BIN);
        }
        else if (apiSource == "i") // entradas
        {
          // sDados1 = String(sensor1.read8(), BIN);
          // sDados2 = String(sensor2.read8(), BIN);
        }
        else
        {
          // sDados1 = '00000000';
          sDados1 = "00000000";
          sDados2 = "00000000";
        }

        slogln("tamanho");
        slogln(sDados1.length());
        while (sDados1.length() < 8)
        {
          sDados1 = '0' + sDados1;
        }
        while (sDados2.length() < 8)
        {
          sDados2 = '0' + sDados2;
        }

        for (uint8_t i = 0; i < 8; i++)
        {
          if (sDados1[i] == '0')
          {
            sDados1[i] = '1';
          }
          else
          {
            sDados1[i] = '0';
          }
        }

        for (uint8_t i = 0; i < 8; i++)
        {
          if (sDados2[i] == '0')
          {
            sDados2[i] = '1';
          }
          else
          {
            sDados2[i] = '0';
          }
        }

        ret = (String)sDados2 + (String)sDados1;
      }
      else
      {
        if (apiSource == "o")
        {
          if (LePorta(apiPort.toInt() - 1) == HIGH)
          {
            ret = "1";
          }
          else
          {
            ret = "0";
          }
        }
        else
        {
          if (LeSensor(apiPort.toInt() - 1) == HIGH)
          {
            ret = "1";
          }
          else
          {
            ret = "0";
          }
        }
      }
    }
    else if (action == "a") // ação
    {
      if (apiPort == "a") // todos
      {
        if (apiSource == "o") // saidas
        {
          if (valueApi == "0")
          {
            if (typeApi == "n" || typeApi == "p")
            {
              //  // chip1.write8(255);
              // chip2.write8(255);
              SaveOutputs();
              ret = "1";
            }
          }
          else if (valueApi == "1")
          {
            if (typeApi == "n")
            {
              // chip1.write8(0);
              // chip2.write8(0);
              SaveOutputs();
              ret = "1";
            }
            else if (typeApi == "p")
            {
              // chip1.write8(0);
              // chip2.write8(0);
              for (uint8_t pulsoApiCount = 0; pulsoApiCount <= 15; pulsoApiCount++)
              {
                g_tempoInicioPulso[pulsoApiCount] = millisAtual;
                g_pulsoHabilita[pulsoApiCount] = true;
              }
              SaveOutputs();
              ret = "1";
            }
          }
        }
      }
      else // porta
      {
        if (apiSource == "o") // saidas
        {
          if (typeApi == "n") // normal
          {
            if (valueApi == "0") // desliga
            {
              LigaDesliga(apiPort.toInt() - 1, LOW, "", 0);
              ret = "1";
            }
            else if (valueApi == "1") // Liga
            {
              LigaDesliga(apiPort.toInt() - 1, HIGH, "", 0);
              ret = "1";
            }
          }
          else if (typeApi == "p") // pulsado
          {
            if (valueApi == "0") // desliga
            {
              LigaDesliga(apiPort.toInt() - 1, LOW, "", 1);
              ret = "1";
            }
            else if (valueApi == "1") // Liga
            {
              LigaDesliga(apiPort.toInt() - 1, HIGH, "", 1);
              ret = "1";
            }
          }
        }
      }
    }
    // else if (action == "i") // Infravermelho
    //{
    // irEnSend = true;
    // ret = "1";
    // }
    else if (action == "c") // Cenas
    {
      ret = "1";
      slogln("Numero da cena: " + valueApi);
      triggerCena(valueApi);
    }
    else if (action == "l") // linha de ação
    {
      for (int i = 1; i <= apiPort.length(); i++)
      {
        int posicaoPorta = i - 1;
        if (posicaoPorta <= 15)
        {
          if (apiPort[posicaoPorta] == '0')
          {
            LigaDesliga(posicaoPorta, LOW, "", 0);
          }
          else if (apiPort[posicaoPorta] == '1')
          {
            LigaDesliga(posicaoPorta, HIGH, "", 0);
          }
        }
      }
      ret = "1";
    }
    else if (action == "update") // executaupdate
    {

      ExecuteUpdate(false, true);
      ret = "1";
    }
    else if (action == "bkp") // executaupdate
    {

      if (typeApi == "b")
        AsyncBackupEsp(false);
      else if (typeApi == "r")
        AsyncRestoreEsp(false);
      else if (typeApi == "f")
        AsyncFormatEsp(false);
      ret = "1";
    }
    // teste dimmer
    else if (action == "dimmer")
    {
      if (typeApi == "w") // Para escrita
      {

        slogln(F("Entrei write dimmer API"));

        SendValueDimmerI2C(apiPort, valueApi, formatApi); // porta (1 a 4),valor(0 a 100),formato ( 0 ou 100)

        ret = "1";
      }
      if (typeApi == "r") // Para leitura
      {
        slogln(F("Entrei read  API"));
        ReadDimmerValue(apiIndex.toInt());
        // ret = "1";
      }
      if (typeApi == "off")
      {
        slogln(F("Entrei off dimmer  API"));
        offDimmer(apiPort);
        // ret = "1";
      }
      if (typeApi == "on")
      {
        slogln(F("Entrei on dimmer  API"));
        onDimmer(apiPort);
      }
    }
  }

  if (isPost)
    gRequest->send(200, sdefTextHtml, (String)ret);
}

void apiativo()
{
  if (AlowApi == true)
  {
    gRequest->send(200, sdefTextHtml, "1");
  }
  else
  {
    gRequest->send(200, sdefTextHtml, "0");
  }
}

void apiconfig()
{
  if (gRequest->arg("s") == Senha)
  {
    gRequest->send(200, sdefTextHtml, sdefOK);

    AlowApi = (gRequest->arg("v") == "1") ? true : false;
    bitWrite(DevSet.mode, 0, AlowApi);
    DevSet.setMode();
  }
  else
  {
    gRequest->send(200, sdefTextHtml, "-1");
  }
}

void alterasenhapi()
{

  if (gRequest->arg("s") == Senha)
  {

    MD5Builder md5;
    md5.begin();
    md5.add(gRequest->arg("a"));
    md5.calculate();

    if (ApiPass == md5.toString())
    {

      gRequest->send(200, sdefTextHtml, sdefOK);

      String req = gRequest->arg("v");
      if (req == "")
      {
        req = "12345678"; // se não houver registro, vai para o padrão
      }

      md5.begin();
      md5.add(req);
      md5.calculate();
      ApiPass = md5.toString();

      DevSet.apiPwd = ApiPass;
      DevSet.setApiPwd();
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