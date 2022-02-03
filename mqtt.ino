// recebido mqtt pela cloud
void callback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';

  String strRec = (String)(char *)payload;

  newMqttMsg = true;

  slogln("mqttcloud: " + String(topic) + "-" + (String)msgMqtt);

  String str = "Recibido MQTT: " + strRec;

  client.publish(mqttTopicoCloudRet, str.c_str());

  msgMqtt = (char *)strRec.c_str();

  api();

  // controle do dimmer

  // String port = MqttArg((char *)payload, "p"); // o argumento do p  e convertido em uma carga util

  // String value = MqttArg((char *)payload, "v"); // o argumento do v  e convertido em uma carga util

  // SendValueDimmer(port, value); // dimmer
  // if ((char)payload[0] == '1')
  // {
  //   digitalWrite(BUILTIN_LED, LOW); // Turn the LED on (Note that LOW is the voltage level
  // }
  // else
  // {
  //   digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH
  // }
}

void MqttCloudReconnect()
{

  slogln(F("Iniciando MQTT connection..."));

  if (!client.connected())
  {
    //(!DEBUG_ON) ?:
    slog(F("Attempting MQTT connection..."));
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_server_user, mqtt_server_userpw))
    {
      //(!DEBUG_ON) ?:
      slogln("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(mqttTopicoCloud);

      //(!DEBUG_ON) ?:
      slogln("WiFi connected");
      //(!DEBUG_ON) ?:
      slogln("IP address: " + WiFi.localIP().toString());

      NtpSetDateTimeNTP();
      UpdatePing();
      hasInternet = true;
      hasMQTT = true;
    }
    else
    {
      //(!DEBUG_ON) ?:
      hasMQTT = false;
      slog("failed, rc=");
      //(!DEBUG_ON) ?:
      Serial.print(client.state());
      //(!DEBUG_ON) ?:
      slogln(" try again in 30 seconds");
    }
    enableConnection = false;
  }
}

void MqttSetup()
{
  //(!DEBUG_ON) ?:
  slogln();

  str = "keepin/placas/" + gchipId + "/cloud";
  strRet = str + "/ret";
  mqttTopicoCloud = str.c_str();
  mqttTopicoCloudRet = strRet.c_str();

  //(!DEBUG_ON) ?:
  slogln(mqtt_server);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.setBufferSize(2048);
  //(!DEBUG_ON) ?:
  slogln("Id Chip: " + gchipId);

  //(!DEBUG_ON) ?:
  slogln("Subscribe cloud: " + (String)mqttTopicoCloud);
}

void MqttLoop()
{
  /*
 * Publish the counter value as String
 */

  if (!client.connected() && (tipoWifiAtual != 2) && ((millisAtual >= millisMqttReconnect)))
  {
    slogln("MQTT disconnected!!!");

    millisMqttReconnect = millisAtual + 60000;

    if (enableConnection)
    {
      MqttCloudReconnect();
    }
  }
  client.loop();
}

String MqttArg(char *msg, char *pkey)
{
  char *pch;
  char *lch;
  char key[] = "";
  int plen, llen;

  strcat(key, pkey);
  strcat(key, "=");
  pch = strstr(msg, key);

  if (pch != NULL)
  {
    pch = pch + strlen(pkey) + 1;
    plen = strlen(pch);
    lch = strchr(pch, '&');
    if (lch == NULL)
    {
      lch = strchr(pch, '\0');
      llen = 0;
    }
    llen = strlen(lch);

    String msgtrata = pch;
    msgtrata.remove(plen - llen);
    slog(key);
    slogln(msgtrata);
    return msgtrata;
  }
  else
  {
    slogln(key);
    return "";
  }
}
