
#define Placa_Version "2.44"
#define espMemory 52696
#define ntpServer "pool.ntp.org"
#define numDNSquery 5
#define pulseTime 600
//#define model8
#define DEBUG_ON

const String sdefOK = "ok";
const String sdefTextHtml = "text/html";

#ifdef DEBUG_ON
#define slog(x) Serial.print(x)
#define slogln(x) Serial.println(x)
#else
#define slog(x)
#define slogln(x)
#endif

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncWebServer.h>
//#include <FS.h>
#include <LittleFS.h>
#include <ESPAsyncUDP.h>
#include <Wire.h>
#include <RtcDateTime.h>
#include <PCF8583.h>
#include <PCF8574.h>
#include <ESP8266httpUpdate.h>
//#include <IRremoteESP8266.h>
//#include <IRrecv.h>
//#include <IRsend.h>
//#include <IRutils.h>
#include <RCSwitch.h>
// FIREBASE
//#include <FirebaseArduino.h>
//#include <FirebaseCloudMessaging.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include "AsyncPing.h"
#include "Ticker.h"
#include <SSD1306Wire.h>

#include <PubSubClient.h>

#include "src\KPDeviceSetting.h"
#include "src\KPPCF8583Class.h"
#include "src\webpage.h"
//#include "src\KPSomfy.h"// deletei a lib da pasta pra fazer teste

const String cloudServer = "http://cloud.keepin.com.br/api/";
const char *mqtt_server = "cloudmqtt.keepin.com.br";
const char *mqtt_server_user = "keepinadm";
const char *mqtt_server_userpw = "Keepin@2020@Cloud";

String vchipId;
String gchipId;
bool chipAtivo = true;
int nCiclos = 0;

bool g_pulsoHabilita[16];
unsigned long g_tempoInicioPulso[16];
unsigned long millisAtual;
unsigned long millisDebug;
unsigned long millisMqttReconnect;
unsigned long millisNetworkScan;
//unsigned long lastDisplay;
//unsigned long millisIREnabled;
unsigned long millisSendUDP;
unsigned long millisFreeMemory;

// File UploadFile;

bool TipoMemoria = true;
String vListaWifi = "";

String vssid = "";
String vpass = "";
String vip = "";
String vmask = "";
String vgateway = "";

bool vConfigWIFI = false;
unsigned long millisWifiLed;

int tipoWifiAtual;
int scanningWifi;

IPAddress DNS1(8, 8, 8, 8);
IPAddress DNS2(4, 4, 4, 4);
String vSenhaAP = "12345678";

// Congiguração chips I2C
PCF8574 chip1(0x21, &Wire);
PCF8574 chip3(0x25, &Wire);
PCF8574 sensor1(0x23, &Wire);
#ifdef model8
#include "src\KPPCFClass.h"
KPPCF chip2;
KPPCF sensor2;
#else
PCF8574 chip2(0x22, &Wire);
PCF8574 sensor2(0x24, &Wire);
#endif

PCF8583 Rtc(0xA0);
KPPCF8583Class memRtc(0xA0);
SSD1306Wire display(0x3c, -1, -1, GEOMETRY_128_64, I2C_ONE, 100000); //, D1, D2);

String s2Sensor1 = "";
String s2Sensor2 = "";

int buttonState = 13;
int counter = 0;
int counterRTC = 0;
boolean AgendaAlterada = true;
String Agendas[6] = {"", "", "", "", "", ""};

boolean SensorAlterado = true;
String Sensores[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
String nomeSensores[16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
boolean msgDisparada[16] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
String ultimoDisparo = "";
boolean notificar = false;
boolean enviarsms = false;
boolean estadoAtual[16] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
boolean ultimoEstado[16] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

// debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 150;
unsigned long rfDelay = 1000;

boolean DeviceAlterado = true;
// String Devices[20];

// RtcDateTime ultimaconsulta;
int Minuto = -1;
unsigned long millisLedRunning = 0;
bool ConsultouCloud = false;

RtcDateTime HorarioAtual;

int correct_address = 0;

unsigned int request_i = 0;
unsigned int response_i = 0;

const int LedWifiConnected = 4;
const int LedWifiHI = 5;
const int LedWifiLOW = 6; // 14;
const int LedRunning = 7;

// UDP
// WiFiUDP Udp;
AsyncUDP udp;
uint16_t localUdpPort = 4210;
// char incomingPacket[255];
// FIM UDP

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress broadcastIP;
IPAddress IpDispositivo;

AsyncWebServer server(80);
AsyncWebServerRequest *gRequest;
bool asyncExecuteFunction;

int contadorled = 0;

unsigned long starTime = 0;
unsigned long interval = 2000;

// Dispositivos
String Senha = "kdi9e";

//   IR   //
//uint16_t RECV_PIN = 14;
//uint16_t CAPTURE_BUFFER_SIZE = 1024;
//const uint16_t kMinUnknownSize = 12;
//#define TIMEOUT 15U
//#if DECODE_AC
//const uint8_t TIMEOUT = 50;
//#else
//const uint8_t TIMEOUT = 15;
//#endif
//#define TIMEOUT 15U
//IRrecv irrecv(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);
//decode_results results;
//irparams_t save;
//int tamanho = -1;
//String codigoIR = "-1";

//IRsend irsend(16);
//boolean enReadIR = false;
//int Modelo = 0;
//boolean irEnSend = false;
//uint16_t irNumBits;
//uint16_t irModel;
//uint8_t irPort;
//String irData = "";

//   IR   //

//   RF  //
//RCSwitch mySwitch = RCSwitch();
//RCSwitch sSendRF = RCSwitch();
//int rxRF = 12;
//int txRF = 15;
// boolean enReadRF = false;
//int tamanhoRF = -1;
//int gProtocoloRF = 1;
//String codigoRF = "-1";
//boolean SensorRFAlterado = true;
//String SensoresRF[30] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
//boolean sensorRFDisparado = false;
//int numSensorDisparado = -1;
//String ultimoDisparoRF = "";
//boolean estadoAtualRF[30] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
//boolean ultimoEstadoRF[30] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
//boolean msgDisparadaRF[30] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
//int numSensorMSG = 0;
//unsigned long rfmilis = 0;
//   RF  //
int Buzzer = 3;

// ----         Dados Firebase      ----- //
//#define FIREBASE_HOST "automacao-6cdcc.firebaseio.com"
//#define FIREBASE_AUTH "m7WSNbRjPJck9YhLE9AdaFKRtnG6U7ENDiUA3IUF"
// int vchipId = 0;
// ----         Fim Dados Firebase      ----- //

// --- API ///
bool AlowApi = false;
String ApiPass = "12345678";
int memorialivre = 0;

//   CENAS   //
bool cenaExecucao = false;
String ArqCena = "";
int cenaPAtual = 0;
int cenaPTotal = 0;
unsigned long lastCnTime = -1;

//   CLOUD ///
bool usaCloud = false;
char *msgMqtt;
bool newMqttMsg;
bool hasMQTT = false;
bool hasCloud = false;

// EEPROM//
KPDeviceSettingClass DevSet;
byte *ipTemp;
byte ipConfigTemp[4];

// RTC//
// PING//
bool hasInternet = false;
bool enableConnection = true;
uint8_t numberPingResponse;
Ticker timer;
AsyncPing Pings[numDNSquery];
IPAddress addrs[numDNSquery];
const char *ips[] = {"8.8.8.8", "8.8.4.4", "208.67.222.222", "google.com", "keepin.com.br"};

// uint8_t shouldUpdate;
bool executeCloud;
uint16_t lastOutputs;

const uint8_t dispY[5] = {1, 14, 30, 42, 54};
String dispText[5] = {"", "", "", "", ""};
uint8_t dispLine = 0;
//bool hasDisplay = false;
bool clock2s;

String idChip = "";
const char *mqttTopicoCloud;
const char *mqttTopicoCloudRet;
String str = "";
String strRet = "";
String *clientestr;

WiFiClient espClient;
PubSubClient client(espClient);

//Somfy somfy(txRF, 0x00000000); // initial remote id - change this one,  it will be incremented for remotes added afterwards

boolean enReadDimmer = false;
// DIMMER//
