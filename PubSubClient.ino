#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//-------- Define Pin ----------------------------
// #define RELAY_PIN 0  // Pin GPIO untuk relay
const int relayPin[] = {2, 4, 5, 12, 13, 14, 15, 16};


//------debug--------
#define DEBUG
#define DEBUGPESAN

// ----------- Variabel Callback Server ------------
char jsonMessage[] = "";
StaticJsonDocument<200> doc;

// ------------------- wifi config -----------------

const char* ssid = "RAHMAD";                  // Nama Wi-Fi
const char* password = "77777777";            // Password Wi-Fi
const char* mqtt_server = "thingsboard.cloud";      // Alamat ThingsBoard server
const char* access_token = "a35Ir6UtnmvW3ImNC22B";  // Access Token dari ThingsBoard



WiFiClient espClient;
PubSubClient client(espClient);

// setup Wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Mencoba Terhubung... ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Terhubung");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//setRelay
void relaySet(String kondisi, int noRelay)
{
  if(kondisi == "on" || kondisi== "ON")
  {
    if(noRelay==0)
    {
      digitalWrite(relayPin[0],LOW);
    }
    else if(noRelay==1)
    {
      digitalWrite(relayPin[1],LOW);
    }
    else if(noRelay==2)
    {
      digitalWrite(relayPin[2],LOW);
    }
    else if(noRelay==3)
    {
      digitalWrite(relayPin[3],LOW);
    }
    else if(noRelay==4)
    {
      digitalWrite(relayPin[4],LOW);
    }
    else if(noRelay==5)
    {
      digitalWrite(relayPin[5],LOW);
    }
    else if(noRelay==6)
    {
      digitalWrite(relayPin[6],LOW);
    }
    else if(noRelay==7)
    {
      digitalWrite(relayPin[7],LOW);
    }
  }
  else if(kondisi == "off" || kondisi== "OFF")
  {
    if(noRelay==0)
    {
      digitalWrite(relayPin[0],HIGH);
    }
    else if(noRelay==1)
    {
      digitalWrite(relayPin[1],HIGH);
    }
    else if(noRelay==2)
    {
      digitalWrite(relayPin[2],HIGH);
    }
    else if(noRelay==3)
    {
      digitalWrite(relayPin[3],HIGH);
    }
    else if(noRelay==4)
    {
      digitalWrite(relayPin[4],HIGH);
    }
    else if(noRelay==5)
    {
      digitalWrite(relayPin[5],HIGH);
    }
    else if(noRelay==6)
    {
      digitalWrite(relayPin[6],HIGH);
    }
    else if(noRelay==7)
    {
      digitalWrite(relayPin[7],HIGH);
    }
  }
}

// logic Relay dari Callback
void logic(String method, bool params) {
  // Serial.println(method);
  // String valueMethod= method.toString();

  if (params == 0) {
    if (method == "setValueRelay") {
      Serial.println("Relay0 TURN OFF");
      relaySet("off", 0);
    } else if (method == "setValueRelay1") {
      Serial.println("Relay1 TURN OFF");
      relaySet("off", 1);
    } else if (method == "setValueRelay2") {
      Serial.println("Relay2 TURN OFF");
      relaySet("off", 2);
    } else if (method == "setValueRelay3") {
      Serial.println("Relay3 TURN OFF");
      relaySet("off", 3);
    } else if (method == "setValueRelay4") {
      Serial.println("Relay4 TURN OFF");
      relaySet("off", 4);
    } else if (method == "setValueRelay5") {
      Serial.println("Relay5 TURN OFF");
      relaySet("off", 5);
    } else if (method == "setValueRelay6") {
      Serial.println("Relay6 TURN OFF");
      relaySet("off", 6);
    } else if (method == "setValueRelay7") {
      Serial.println("Relay7 TURN OFF");
      relaySet("off", 7);
    }
  } else if (params == 1) {
    if (method == "setValueRelay") {
      Serial.println("Relay0 TURN ON");
      relaySet("on", 0);
    } else if (method == "setValueRelay1") {
      Serial.println("Relay1 TURN ON");
      relaySet("on", 1);
    } else if (method == "setValueRelay2") {
      Serial.println("Relay2 TURN ON");
      relaySet("on", 2);
    } else if (method == "setValueRelay3") {
      Serial.println("Relay3 TURN ON");
      relaySet("on", 3);
    } else if (method == "setValueRelay4") {
      Serial.println("Relay4 TURN ON");
      relaySet("on", 4);
    } else if (method == "setValueRelay5") {
      Serial.println("Relay5 TURN ON");
      relaySet("on", 5);
    } else if (method == "setValueRelay6") {
      Serial.println("Relay6 TURN ON");
      relaySet("on", 6);
    } else if (method == "setValueRelay7") {
      Serial.println("Relay7 TURN ON");
      relaySet("on", 7);
    }
  }
}

// fungsi callback
void callback(char* topic, byte* payload, unsigned int length) {
#ifdef DEBUGPESAN
  Serial.println("Pesan [");
  Serial.print(topic);
  Serial.print("] ");
#endif

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  String jsonMessage = message;
  Serial.println(message);
  DeserializationError error = deserializeJson(doc, jsonMessage);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char* method = doc["method"];
  bool params = doc["params"];
#ifdef DEBUG
  Serial.println("Method : " + String(method));
  Serial.println("Params : ");
  Serial.print(params);
#endif
  logic(method, params);
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Menunggu Koneksi MQTT...");
    if (client.connect("ESP8266Client", access_token, NULL)) {
      Serial.println("Terhubung");
      client.subscribe("v1/devices/me/rpc/request/+");  // Subscribe ke RPC ThingsBoard
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println("Mencoba dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  // pinMode(RELAY_PIN, OUTPUT);
  // digitalWrite(RELAY_PIN, HIGH);  // Relay dalam kondisi mati
  for(int x =0 ;x<= sizeof(relayPin);x++)
  {
    pinMode(relayPin[x],OUTPUT);
    digitalWrite(relayPin[x],HIGH);
  }

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
