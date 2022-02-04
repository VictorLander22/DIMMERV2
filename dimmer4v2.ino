#include "globalvar.h"

//########################################################################################################################################################
void setup(void)
{
  // SetupDimmer(); // configurar os pinos de saida do dimmer para COM por  serial
  Serial.begin(115200);
  slogln();
  Wire.begin(4, 5); // Para o d1 mini pro os pinos SDA e SCL sao invertidos
  Wire.setClock(100000L);
  delay(300); // Wait for start I2C transmission
  slogln();

  scanI2c();
  //chip1.begin();
  //.begin();
  //chip3.begin();
  //sensor1.begin();
  //sensor2.begin();

  ApagaPortas();

  //IniciaRTC();

  //CheckLittleFS();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  String Razao = ESP.getResetReason();
  slog(F("Motivo Reset: "));
  slogln(Razao);

  delay(1000); // esperar para começar.. permite o monitoramento logo no inicio ao desligar a placa

  // Set chip id
  vchipId = (String)ESP.getChipId();
  gchipId = WiFi.macAddress();
  gchipId.replace(":", "");

  slogln();
  slogln("Keepin Firmware: " + String(Placa_Version));
  slogln("Keepin ID: " + vchipId);
  slogln("Keepin MAC: " + gchipId);

  // Reset mode

  if (digitalRead(buttonState))
  {
    slogln();
    slogln(F("Factory reset\n"));

    DevSet.factoryReset();
  }
  else
  {
    slogln();
    slogln(F("Simple restart\n"));
  }

  DevSet.verifyEEPROM();
  convertConfig();

  DevSet.getDeviceSettings();

  DevSet.showVariables();

  lerConfiguracao();

  millisAtual = millis();
  lastDebounceTime = millisAtual;
  starTime = millisAtual;
  // rfmilis = millisAtual;
  millisWifiLed = millisAtual;

  // verificar se ha necessidade de colocar um delay aqui para evitar dos relés abrirem e fecharem muito rapido

  Memoria();

  scanningWifi = WiFi.scanNetworks();

  // WiFi.scanNetworksAsync(prinScanResult);
  slogln("Available Wifi: " + (String)scanningWifi);

  conectar(); // consome 1K da ram 19000

  // Wait for connection
  slog(F("Connected... IP address: "));
  slogln(WiFi.localIP());

  ConfigurarWebServer();

  retornaNotificar();

  slogln("Notificar: " + String(notificar));

  LoadInputs();

  MqttSetup(); // consome 2k da ram 11400

  SetupUDP();

  SetupPing();

  LoopPing();

  timer.attach(30, LoopPing);

  // SetupSomfy();

  dispText[0] = "INITIALING KEEPIN " + (String)Placa_Version;
}

//########################################################################################################################################################
void loop(void)
{
  nCiclos++;

  millisAtual = millis();

  if (chipAtivo == true) // Verifica se o chip é reconhecido com um chip válido
  {
    LoopLedStatus();

    LoopReconexao();

    HorarioAtual = carregaHora();

    LoopCloud();

    LoopAgenda();

    LoopLedRunning();

    // LoopResetFabrica();

    // mesh_node.acceptRequest();
    // leituraUDP();

    trataSensores();

    //checkCena();

    MqttLoop();

    MillisResets();

    ResetSaidasPulsadas();

    AsyncFunctions();

    //ReadDimmerValue(); // Funcao pra receber valores do dimmer

    if ((millisAtual > millisFreeMemory))
    {
      FreeMemory(F("loop()"));
      millisFreeMemory = millisAtual + 3000;
    }
  }
}
