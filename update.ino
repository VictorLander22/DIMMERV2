void ExecuteUpdate(bool isPost, bool beta)
{
  WiFiClient client;

  String url = (beta) ? "http://keepin.com.br/firmware/16/beta/firmware16.ino.bin" : "http://keepin.com.br/firmware/16/firmware16.bin";
  // http://keepin.com.br/firmware/16/autoresidencial.ino.bin

  slog("Iniciando Update em: ");

  slogln(url);

  t_httpUpdate_return ret = ESPhttpUpdate.update(client, url);

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    if (isPost)
      gRequest->send(200, sdefTextHtml, F("HTTP_UPDATE_FAILD Error: ") + String(ESPhttpUpdate.getLastErrorString().c_str()));

    break;

  case HTTP_UPDATE_NO_UPDATES:
    slogln(F("HTTP_UPDATE_NO_UPDATES"));
    if (isPost)
      gRequest->send(200, sdefTextHtml, F("HTTP_UPDATE_NO_UPDATES"));

    break;

  case HTTP_UPDATE_OK:
    slogln(sdefOK);
    if (isPost)
      gRequest->send(200, sdefTextHtml, F("HTTP_UPDATE_OK"));

    break;

  default:
    if (isPost)
      gRequest->send(200, sdefTextHtml, sdefOK);
  }
  //}
  BeepBuzzer();
}

void versao()
{
  gRequest->send(200, sdefTextHtml, Placa_Version);
}

// void linkversao()
// {
//   gRequest->send(200, sdefTextHtml, "http://keepin.com.br/firmware/16/versao.txt");
// }

// void linkversaoBeta()
// {
//   gRequest->send(200, sdefTextHtml, "http://keepin.com.br/firmware/16/beta/versao.txt");
// }

// void logData(String dados)
// {
//   RtcDateTime HorarioAtual;
//   HorarioAtual = carregaHora();

//   char time[50];
//   sprintf(time, "%02d/%02d/%02d %02d:%02d:%02d", HorarioAtual.Day(), HorarioAtual.Month(), HorarioAtual.Year(), HorarioAtual.Hour(), HorarioAtual.Minute(), HorarioAtual.Second());

//   dados = "Data: " + String(time) + " - " + dados;

//   LittleFS.begin();
//   File f = LittleFS.open("/log.txt", "a");
//   f.println(dados);
//   f.close();
//   LittleFS.end();
// }

// void readlog()
// {
//   LittleFS.begin();
//   String comando = gRequest->arg("c");

//   if (comando == "delete")
//   {
//     if (LittleFS.remove("/log.txt"))
//     {
//       LittleFS.end();
//       gRequest->send(200, sdefTextHtml, "remove");
//     }
//     else
//     {
//       LittleFS.end();
//       gRequest->send(200, sdefTextHtml, "falha");
//     }
//   }
//   else if (comando == "read")
//   {
//     File f = LittleFS.open("/log.txt", "r");
//     String dados = "<html><body><p><h3><strong>Log</strong></p></h3><br><ul>";
//     while (f.available())
//     {
//       dados = dados + "<li>" + f.readStringUntil('\n') + "</li>";
//     }
//     dados = dados + "</ul></body></html>";
//     f.close();
//     LittleFS.end();
//     gRequest->send(200, sdefTextHtml, dados);
//   }
//   else
//   {
//     gRequest->send(200, sdefTextHtml, sdefOK);
//   }
// }