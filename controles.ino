void executaPulso(int porta)
{
  if (porta < 8)
  {
    chip1.write(porta, LOW);
  }
  else
  {
    chip2.write(porta - 8, LOW);
  }
  g_tempoInicioPulso[porta] = millisAtual;
  g_pulsoHabilita[porta] = true;
}

void parseBytes(const char *str, char sep, byte *bytes, int maxBytes, int base)
{
  for (int i = 0; i < maxBytes; i++)
  {
    bytes[i] = strtoul(str, NULL, base);
    str = strchr(str, sep);
    if (str == NULL || *str == '\0')
    {
      break;
    }
    str++;
  }
}

void retornachip()
{
  gRequest->send(200, sdefTextHtml, vchipId);
}

void GetChipMac()
{
  gRequest->send(200, sdefTextHtml, gchipId);
}

void controle()
{
  gRequest->send(200, sdefTextHtml, sdefOK);

  String p = gRequest->arg("p");
  String k = gRequest->arg("k");
  String f = gRequest->arg("f");
  String Nome = gRequest->arg("n");
  int Tipoa = gRequest->arg("pu").toInt();
  if (p == "a")
  {
    chip1.write8(f.toInt() & 0xff);
    chip2.write8((f.toInt() >> 8) & 0xff);
    SaveOutputs();
  }
  else
  {
    int porta = p.toInt();
    porta = retornaPorta(porta);
    if (porta >= 0)

      if (k == "kdi9e")
      {

        if (f == "true")
        {
          LigaDesliga(porta, HIGH, Nome, Tipoa);
        }
        else
        {
          LigaDesliga(porta, LOW, Nome, Tipoa);
        }
      }
  }
}

void situacao()
{
  String p = gRequest->arg("p");
  String k = gRequest->arg("k");
  int porta = p.toInt();

  porta = retornaPorta(porta);
  if (porta >= 0)

    if (k == "kdi9e")
    {
      if (LePorta(porta) == HIGH)
      {
        gRequest->send(200, sdefTextHtml, "true");
      }
      else
      {
        gRequest->send(200, sdefTextHtml, "false");
      }
    }
}

void valida()
{
  gRequest->send(200, sdefTextHtml, "16|2|16|" + vchipId + "|");
}

void handleNotFound()
{
  // String message = "File Not Found\n\n";
  // message += "URI: ";
  // message += gRequest->url();
  // message += "\nMethod: ";
  // message += (gRequest->method() == HTTP_GET) ? "GET" : "POST";
  // message += "\nArguments: ";
  // message += gRequest->args();
  // message += "\n";
  // for (uint8_t i = 0; i < gRequest->args(); i++)
  // {
  //   message += " " + gRequest->argName(i) + ": " + gRequest->arg(i) + "\n";
  // }
  gRequest->send(404, "text/plain", F("Function Not Found"));
}

void LigaDesliga(int vPorta, int vFuncao, String Nome, int Tipo)
{
  if (Tipo != 1) //normal
  {
    if (vPorta < 8)
    {
      chip1.write(vPorta, !vFuncao);
    }
    else
    {
      chip2.write(vPorta - 8, !vFuncao);
    }
    SaveOutputs();
  }
  else //pulsado
  {
    executaPulso(vPorta);
  }

  //sendCloud(true);
}

int LePorta(int vPorta)
{
  if (vPorta < 8)
  {
    if (chip1.read(vPorta) == HIGH)
    {
      return LOW;
    }
    else
    {
      return HIGH;
    }
  }
  else
  {
    if (chip2.read(vPorta - 8) == HIGH)
    {
      return LOW;
    }
    else
    {
      return HIGH;
    }
  }
}

int LeSensor(int vPorta)
{
  if (vPorta < 8)
  {
    if (sensor1.read(vPorta) == HIGH)
    {
      return LOW;
    }
    else
    {
      return HIGH;
    }
  }
  else
  {
    if (sensor2.read(vPorta - 8) == HIGH)
    {
      return LOW;
    }
    else
    {
      return HIGH;
    }
  }
}

void ApagaPortas()
{
  chip1.write8(255);
  chip2.write8(255);
  chip3.write8(240);
}

void Inverte(int vPorta)
{
  if (vPorta < 8)
  {
    chip1.write(vPorta, !chip1.read(vPorta));
  }
  else
  {
    chip2.write(vPorta - 8, !chip2.read(vPorta - 8));
  }
  SaveOutputs();
}