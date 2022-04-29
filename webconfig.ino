void ConfigurarWebServer(void)
{
  // server.on("/", handleHtmlConfig);
  server.onNotFound(ExecuteFunction);
  // server.on(
  //     "/fupload", HTTP_POST, [](AsyncWebServerRequest *request)
  //     { request->send(200); },
  //     onUpload);
  server.begin();
  // slogln(F("HTTP server started"));
}

void ExecuteFunction(AsyncWebServerRequest *request)
{
  gRequest = request;
  asyncExecuteFunction = true;
  // request->send(200, sdefTextHtml, "OK");
}

void AsyncFunctions()
{
  if (asyncExecuteFunction)
  {
    asyncExecuteFunction = false;

    String functionName = gRequest->url();

    slogln(functionName);

    for (size_t i = 0; i < gRequest->args(); i++)
    {
      slogln("[" + (String)i + "] " + gRequest->getParam(i)->name() + " : " + gRequest->getParam(i)->value());
    }
    if (functionName == F("/"))
      handleHtmlConfig();
    else if (functionName == F("/espbackup"))
      AsyncBackupEsp(true);
    else if (functionName == F("/esprestore"))
      AsyncRestoreEsp(true);
    else if (functionName == F("/espformat"))
      AsyncFormatEsp(true);
    else if (functionName == F("/filedir"))
      FileDir();
    else if (functionName == F("/filedownload"))
      FileDownload();
    else if (functionName == F("/filedelete"))
      FileDelete();
    else if (functionName == F("/fileupload"))
      FileUpload();
    else if (functionName == F("/api"))
      api();
    else if (functionName == F("/chipmac"))
      GetChipMac();
    else if (functionName == F("/controle"))
      controle();
    else if (functionName == F("/gravarwifi"))
      gravawifi();
    else if (functionName == F("/gravasenhawifi"))
      gravasenhawifi();
    else if (functionName == F("/gravasenhahttp"))
      gravasenhahttp();
    else if (functionName == F("/situacao"))
      situacao();
    else if (functionName == F("/chipid"))
      retornachip();
    // else if (functionName == F("/chamaddns"))
    //   chamaddns();
    else if (functionName == F("/consultaagenda"))
      conagenda();
    else if (functionName == F("/gravaragenda"))
      gravaragenda();
    else if (functionName == F("/atualizahora"))
      atualizahora();
    else if (functionName == F("/lersensores"))
      lersensores();
    else if (functionName == F("/consultasensor"))
      consensor();
    else if (functionName == F("/gravadevice"))
      gravadevice();
    else if (functionName == F("/buscadevice"))
      buscadevice();
    else if (functionName == F("/executeupdate"))
    {
      AsyncBackupEsp(false);
      ExecuteUpdate(true, false);
    }
    // executeupdate();
    else if (functionName == F("/executeupdatebeta"))
    {
      AsyncBackupEsp(false);
      ExecuteUpdate(true, true);
    }
    // executeupdateBeta(true);
    else if (functionName == F("/versao"))
      versao();
    // else if (functionName == F("/link"))
    //   linkversao();
    // else if (functionName == F("/link"))
    //   linkversaoBeta();
    else if (functionName == F("/ultimodisparo"))
      ultimodisp();
    else if (functionName == F("/buscaNotificar"))
      buscaNotificar();
    else if (functionName == F("/gravanot"))
      gravanot();
    // sms else if (functionName == F("/gravasms"))
    // sms gravasms();
    // sms else if (functionName == F("/consultasms"))
    // sms consultasms();
    else if (functionName == F("/wifi"))
      valorwifi();
    else if (functionName == F("/listawifi"))
      WifiNetworkScan();
    // else if (functionName == F("/listawifi2"))
    //   listawifi2();

    else if (functionName == F("/modelo"))
      fmodelo();
    else if (functionName == F("/memoria"))
      fMemoria();
    else if (functionName == F("/apiativo"))
      apiativo();
    else if (functionName == F("/apiconfig"))
      apiconfig();
    else if (functionName == F("/alterasenhapi"))
      alterasenhapi();
    else if (functionName == F("/gravacena"))
      gravacena();
    // else if (functionName == F("/log"))
    //   readlog();
    else if (functionName == F("/gravacloud"))
      GravaCloud();
    else if (functionName == F("/valida"))
      valida();
    else if (functionName == F("/reset"))
      wifireset();
    // else if (functionName == F("/about"))
    //   about();
    else if (functionName == F("/gravasensor"))
      AsyncSaveInputConfig();
    else if (functionName == F("/reiniciar"))
      reiniciar();
    else if (functionName == F(""))
      Serial.println("Recebeu um valor vazio");

        else
      handleNotFound();
  }
}

void handleHtmlConfig()
{
  if (!gRequest->authenticate(DevSet.httpUser.c_str(), DevSet.httpPwd.c_str()))
    return gRequest->requestAuthentication();

  String defaultPage(FPSTR(webDefaultPage));
  defaultPage.replace("#ipfixo#", "true");
  defaultPage.replace("#utc#", String(DevSet.utcConfig));
  defaultPage.replace("#ssid#", DevSet.wifiSSID);
  defaultPage.replace("#pwd#", DevSet.wifiPwd);
  defaultPage.replace("#ip#", DevSet.numberToIpString(DevSet.wifiIP));
  defaultPage.replace("#msk#", DevSet.numberToIpString(DevSet.wifiMSK));
  defaultPage.replace("#gtw#", DevSet.numberToIpString(DevSet.wifiGTW));
  gRequest->send(200, sdefTextHtml, defaultPage.c_str());
}

// void about()
// {
//   gRequest->send_P(200, sdefTextHtml, webAbout);
// }

void reiniciar()
{
  String restartPage(FPSTR(webRestart));
  restartPage.replace("#oldip#", CurrentIP());
  restartPage.replace("#newip#", DevSet.numberToIpString(DevSet.wifiIP));
  gRequest->send_P(200, sdefTextHtml, restartPage.c_str());

  delay(300);
  ESP.restart();
}

void gravawifi()
{
  gRequest->send(200, sdefTextHtml, "<html>ok<meta charset='UTF-8'><script>location.replace(\"http://" + CurrentIP() + "\")</script></html>");

  String wifiSSID = gRequest->arg("txtnomerede");
  String wifiPWD = gRequest->arg("txtsenha");
  const char *wifiIP = gRequest->arg("txtip").c_str();
  const char *wifiMSK = gRequest->arg("txtmascara").c_str();
  const char *wifiGTW = gRequest->arg("txtgateway").c_str();
  bitWrite(DevSet.mode, 2, 0); // wifiPadrao
  DevSet.wifiSSID = wifiSSID;
  DevSet.wifiPwd = wifiPWD;
  DevSet.wifiIP = DevSet.ipStringToNumber(wifiIP);
  DevSet.wifiMSK = DevSet.ipStringToNumber(wifiMSK);
  DevSet.wifiGTW = DevSet.ipStringToNumber(wifiGTW);
  DevSet.setWifi();
  slogln(F("New WIFI Settings"));
  DevSet.showVariables();
}

void FileDir()
{
  String arquivos = "";
  LittleFS.begin();
  slogln(F("Consultar sistema de arquivos"));
  Dir dir = LittleFS.openDir("/");
  while (dir.next())
  {
    arquivos += dir.fileName() + " ";
    if (dir.fileSize())
    {
      File f = dir.openFile("r");
      arquivos += f.size();
      f.close();
    }
    arquivos += "<BR>";
  }
  LittleFS.end();

  arquivos += "*";

  gRequest->send(200, sdefTextHtml, arquivos);
}

void FileDownload()
{
  String path = gRequest->arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  LittleFS.begin();
  bool fileExist = LittleFS.exists(path);

  if (fileExist)
  {
    slogln(F("Arquivo existe"));
    gRequest->send(LittleFS, path, String(), true);
  }
  else
  {
    slogln(F("Arquivo não existe"));
    gRequest->send(200, sdefTextHtml, F("File not found"));
  }

  LittleFS.end();
}

void FileDelete()
{
  String path = gRequest->arg("f");

  if (!path.startsWith("/"))
    path = "/" + path;

  LittleFS.begin();
  if (LittleFS.exists(path))
  {
    slogln(F("Arquivo existe"));
    if (LittleFS.remove(path))
    {
      slogln(F("Removido"));
      gRequest->send(200, sdefTextHtml, F("Removido"));
    }
  }
  else
  {
    slogln(F("Arquivo não existe"));
    gRequest->send(200, sdefTextHtml, F("File not found"));
  }
  LittleFS.end();
}

void FileUpload()
{
  gRequest->send_P(200, sdefTextHtml, webUpload);
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  File UploadFile;
  if (!index)
  {
    slogln("UploadStart: " + filename);
    if (!filename.startsWith("/"))
      filename = "/" + filename;
    LittleFS.begin();
    LittleFS.remove(filename);                 // Remove a previous version, otherwise data is appended the file again
    UploadFile = LittleFS.open(filename, "a"); // Open the file for writing in LittleFS (create it, if doesn't exist)
  }

  // Serial.printf("%s", (const char *)data);
  UploadFile.write(data, len); // Write the received bytes to the file

  if (final)
  {
    slogln("UploadEnd: " + filename + "(" + index + len + ")");
    if (UploadFile) // If the file was successfully created
    {
      UploadFile.close(); // Close the file again
      request->send(200, sdefTextHtml, F("<h3>File was successfully uploaded</h3>"));
      //  LittleFS.end();
    }

    LittleFS.end();
  }
}

void AsyncBackupEsp(bool isPost)
{
  if (isPost)
    gRequest->send(200, sdefTextHtml, F("Backup!"));
  WiFiClient cliente;
  HTTPClient http;
  String uri = cloudServer + "postfile";
  String path = "";

  LittleFS.begin();
  Dir dir = LittleFS.openDir("/");
  while (dir.next())
  {
    path = dir.fileName();
    if (!path.startsWith("/"))
      path = "/" + path;
    bool fileExist = LittleFS.exists(path);
    if (fileExist)
    {
      slogln(path);
      http.setTimeout(2000);
      http.setReuse(true);

      File f = LittleFS.open(path, "r");
      http.begin(cliente, uri);
      http.addHeader("Content-Type", "application/octet-stream");
      http.addHeader("dirname", gchipId);
      http.addHeader("filename", path);

      int httpCode = http.sendRequest("POST", &f, f.size());

      slogln(uri);
      slogln(httpCode);

      if (httpCode >= 200 && httpCode < 300)
      {
        String payload = http.getString();
        slogln(payload);
      }

      http.end();
      f.close();
    }
  }
  LittleFS.end();
  BeepBuzzer();
}

void AsyncRestoreEsp(bool isPost)
{
  if (isPost)
    gRequest->send(200, sdefTextHtml, F("Restore!"));
  HTTPClient http;
  WiFiClient client;
  // String payload;

  http.setTimeout(2000);
  http.setReuse(true);
  http.begin(client, cloudServer + "dir");

  http.addHeader("dirname", gchipId);
  int httpCode = http.GET();
  String payload = http.getString();
  http.end();

  slogln(payload);
  slogln(payload.length());
  DynamicJsonDocument dirlist(payload.length() * 3);

  // DynamicJsonDocument dirlist(payload.length() * 16 * 5 + payload.length() * 16 + 2 * 16);

  auto error = deserializeJson(dirlist, payload);

  if (error)
  {
    slog(F("deserializeJson() failed with code "));
    slogln(error.c_str());
    return;
  }
  else
  {
    LittleFS.begin();
    for (int i = 0; i < dirlist["file"].size(); i++)
    {
      // Serial.println("AsyncRestoreEsp 1");
      const String file = dirlist["file"][i];
      // Serial.println("/" + dir);

      http.setTimeout(2000);
      http.setReuse(true);

      http.begin(client, cloudServer + "download");
      http.addHeader("dirname", gchipId);
      http.addHeader("filename", file);

      httpCode = http.GET();

      if (httpCode == 200)
      {
        slogln("/" + file);
        payload = http.getString(); // Get the request response payload
        slogln(payload);
        File f = LittleFS.open("/" + file, "w");
        if (f)
        {
          f.print(payload);
        }
        f.close();
      }
      http.end();
    }
    LittleFS.end();
    delay(50);
  }
  BeepBuzzer();
}

void AsyncFormatEsp(bool isPost)
{
  if (isPost)
    gRequest->send(200, sdefTextHtml, F("Formating!"));
  slogln(F("Formating"));
  // LittleFS.format();
  LittleFS.format();
  slogln(F("Format SUCCESS!"));
  BeepBuzzer();
}
/*void SetupSomfy()
{
  somfy.ChangeRemoteNumber(gchipId.toInt()); // endereço do MAC
  somfy.AddRemote("AAAA");
  somfy.AddRemote("BBBB");
  somfy.AddRemote("CCCC");
  somfy.AddRemote("DDDD");
  somfy.AddRemote("EEEE");
  somfy.Setup();
}*/