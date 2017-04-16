/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://cmmakerclub.com            */

#include <ESP8266WiFi.h>
#include <MicroGear.h>

const char* ssid     = "NAzT";
const char* password = "espertap";

#define APPID   "CNX"
#define KEY     "tr72j3f0O5H3Tle"
#define SECRET  "fo0u4urmNAEEp9iDKUUV2xTAz"
#define DEVICE_NAME   "nat"

WiFiClient client;

int timer = 0;
MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
  String incommingMsg = String((char*)msg);
  if (incommingMsg == "ON") {
    digitalWrite(2, HIGH);
  }
  else if (incommingMsg == "OFF") {
    digitalWrite(2, LOW);
  }
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  /* Set the alias of this microgear ALIAS */
  microgear.setAlias(DEVICE_NAME);
}


void setup() {
  pinMode(2, OUTPUT);
  /* Add Event listeners */

  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE, onMsghandler);

  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED, onConnected);

  Serial.begin(115200);
  Serial.println("Starting...");

  /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
  /* You may want to use other method that is more complicated, but provide better user experience */
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, DEVICE_NAME);

  /* connect to NETPIE to a specific APPID */
  microgear.connect(APPID);
}

void loop() {
  /* To check if the microgear is still connected */
  if (microgear.connected()) {
    /* Call this method regularly otherwise the connection may be lost */
    microgear.loop();
  }
  else {
    Serial.println("connection lost, reconnect...");
    if (timer >= 5000) {
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 100;
  }

}
