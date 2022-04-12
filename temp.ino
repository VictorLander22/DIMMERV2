// void teste()
// {
//   FreeMemory(F("ptest()"));
//   char *ptest = (char *)malloc(2048);
//   strcpy(ptest, "abcdef");
//   slogln(ptest);
//   FreeMemory(F("ptest()"));
//   free(ptest);
//   FreeMemory(F("ptest()"));
// }

// void teste1()
// {
//   uint8_t qtdDados = 5;
//   uint8_t numObj = 1;

//   LittleFS.begin();
//   File f = LittleFS.open("/ce_10.cfg", "r");
//   uint16_t numDados = f.readStringUntil('\n').toInt() + 1;
//   String str = f.readStringUntil('\0');
//   f.close();
//   LittleFS.end();

//   DynamicJsonDocument doc(numDados * JSON_ARRAY_SIZE(qtdDados) + JSON_ARRAY_SIZE(numDados) + JSON_OBJECT_SIZE(numObj));
//   deserializeJson(doc, str);

//   JsonArray array = doc.as<JsonArray>();
//   for (JsonVariant v : array)
//   {
//     Serial.println(v[0].as<String>());
//     // Serial.println(v[1].as<String>());
//     // Serial.println(v[2].as<int>());
//     // Serial.println(v[3].as<int>());
//     // Serial.println(v[4].as<int>());
//   }

//   gRequest->send(200, sdefTextHtml, sdefOK);
// }

// void ReceiveSceneJson()
// { // criar o arquivo json para cena

//   DynamicJsonDocument doc(gRequest->arg("json").toInt() * 16 * 5 + gRequest->arg("json").toInt() * 16 + 2 * 16);

//   // parse a JSON array
//   deserializeJson(doc, gRequest->arg("v"));

//   // extract the values
//   Serial.println((String)doc["q"]);

//   JsonArray array = doc["v"].as<JsonArray>();
//   for (JsonVariant v : array)
//   {
//     Serial.println(v[0].as<String>());
//     Serial.println(v[1].as<String>());
//     Serial.println(v[2].as<int>());
//     Serial.println(v[3].as<int>());
//     Serial.println(v[4].as<int>());
//   }
// }

// void showDateTime()
// {
//   Serial.printf("Data hora: %u/%u/%u %u:%u:%u\n", Rtc.day, Rtc.month, Rtc.year, Rtc.hour, Rtc.minute, Rtc.second);
//   delay(200);
// }

// void mostarEEProm()
// {
//   DevSet.getDeviceSettings();
//   DevSet.showVariables();
// }

/*void SetupSomfy()
{
    // somfy.ChangeRemoteNumber(0xFFFFFFFB);
    // somfy.ChangeRemoteNumber(ChipId);
    somfy.ChangeRemoteNumber(gchipId.toInt()); // endereço do MAC
    //somfy.ChangeRemoteNumber(vchipID.toInt());
    somfy.AddRemote("AAAA");
    somfy.AddRemote("BBBB");
    somfy.AddRemote("CCCC");
    somfy.AddRemote("DDDD");
    somfy.AddRemote("EEEE");
    somfy.Setup();
}
*/

// void SetupDimmer() Para com por serial
//{

// pinMode(1, FUNCTION_3);

//  pinMode(3, FUNCTION_3);
//}

// void SendValueDimmer(String port, String value)
//{
//  Comparaçao do valor com 170 em uma XOR,valores diff igual a 1.
//  uint8_t valueXor = value.toInt() ^ 170;
// Serial.print(String(value.toInt()));
// Serial.print(String(valueXor));
// char frame[3] = "";

// switch case pro char da port
// switch (port.toInt())
//{
// case 1:
//  frame[0] = 'm';
// break;
// case 2:
//  frame[0] = 'n';
// break;
// case 3:
//  frame[0] = 'o';
// break;
// case 4:
//  frame[0] = 'p';
// break;
// default:
//  break;
//}

// delay(200);
// Serial.print(frame[0]);
// Serial.print(char(atoi(value.c_str())));
// Serial.print(char(valueXor));
// delay(200);
// }

void ReadDimmerValue(int index)
{

    // lamp 1//
    Wire.requestFrom(0x38, 1, 1);
    vectorLeituraDimmer[0] = Wire.read();
    // lamp 2//
    Wire.requestFrom(0x38, 1, 1);
    vectorLeituraDimmer[1] = Wire.read();
    // lamp 3//
    Wire.requestFrom(0x38, 1, 1);
    vectorLeituraDimmer[2] = Wire.read();
    // lamp 4//
    Wire.requestFrom(0x38, 1, 1);
    vectorLeituraDimmer[3] = Wire.read();
    // modo energ//
    Wire.requestFrom(0x38, 1, 1);
    vectorLeituraDimmer[4] = Wire.read();

    for (byte i = 0; i < 5; i++)

    {

        Serial.println(vectorLeituraDimmer[i]);
    }
    gRequest->send(200, sdefTextHtml, String(vectorLeituraDimmer[index]));
}
void SendValueDimmerI2C(String port, String value, String modo_energ)
{

    vectorValueDimmer[0] = port.toInt();
    vectorValueDimmer[1] = value.toInt();
    vectorValueDimmer[2] = modo_energ.toInt();
    // Comparaçao dos valores em uma XOR.
    uint8_t valueXor = vectorValueDimmer[0] ^ vectorValueDimmer[1] ^ vectorValueDimmer[2];

    vectorValueDimmer[3] = valueXor;
    // vectorValueDimmer[3] = Xor.toInt();
    Wire.beginTransmission(0x38);      // caso o endereco n seja este verificar com um codigo de Scan de enderecos I2C
    for (byte i = 0; i < 4; i = i + 1) //  loop for para escrever parte a parte do vetor
    {
        Wire.write(vectorValueDimmer[i]);
        Serial.println(vectorValueDimmer[i]);
        // delay(500);
    }

    Wire.endTransmission();
    // prints
    //   Serial.println(vectorValueDimmer[0]);
    //  Serial.println(vectorValueDimmer[1]);
    //  Serial.println(vectorValueDimmer[2]);
    //  Serial.println(vectorValueDimmer[3]);
}

void offDimmer(String port)
{

    ReadDimmerValue(port.toInt() - 1);                                   // - 1 pq o index comeca de 0
    lstDimmer[port.toInt() - 1] = vectorLeituraDimmer[port.toInt() - 1]; // -1 Porque os vetores comecam de 0
    SendValueDimmerI2C(port, "0", "0");

    slogln(F("O valor do vetor lstdimmer e ................."));
    Serial.println(lstDimmer[0]);
    Serial.println(lstDimmer[1]);
    Serial.println(lstDimmer[2]);
    Serial.println(lstDimmer[3]);
}
void onDimmer(String port)
{
    SendValueDimmerI2C(port, String(lstDimmer[port.toInt() - 1]), "0");
    gRequest->send(200, sdefTextHtml, String(lstDimmer[port.toInt() - 1]));

    slogln(F("O valor enviado a funcao de sendo e ....."));
    Serial.println(lstDimmer[port.toInt() - 1]);
}