#include <ESP8266WiFi.h>
#include <ArduinoJson.h>      // https://arduinojson.org/
#include <WebSocketsClient.h> // download and install from https://github.com/Links2004/arduinoWebSockets
#include <SocketIOclient.h>
#include <Servo.h>

#define SSID "DR"
#define PASSWORD "@A12345678#"
#define SERVER "192.168.100.4" // Server URL (without https://www)

Servo servo;
SocketIOclient socketIO;

void messageHandler(uint8_t *payload)
{
  StaticJsonDocument<64> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error)
  {
    Serial.println(error.f_str());
    return;
  }

  String messageKey = doc[0];
  bool value = doc[1];

  if (messageKey == "buttonState")
  {
    digitalWrite(LED_BUILTIN, value);

    servo.write(70);
    delay(1000);
    servo.write(170);
    // socketIO.sendEVENT("logging:1");
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();

    // add evnet name
    // Hint: socket.on('event_name', ....
    uint64_t now = millis();
    array.add("logging");

    // add payload (parameters) for the event
    JsonObject param1 = array.createNestedObject();
    param1["now"] = (uint32_t)now;

    Serial.println(array);
    // JSON to String (serializion)
    String output;
    serializeJson(doc, output);

    // Send event
    socketIO.sendEVENT(output);

    // Print JSON for debugging
    Serial.println(output);
  }
}

void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case sIOtype_DISCONNECT:
    Serial.println("Disconnected!");
    break;

  case sIOtype_CONNECT:
    Serial.printf("Connected to url: %s%s\n", SERVER, payload);

    // join default namespace (no auto join in Socket.IO V3)
    socketIO.send(sIOtype_CONNECT, "/");
    break;

  case sIOtype_EVENT:
    messageHandler(payload);
    break;
  }
}

void setupWiFi()
{
  Serial.println("\nConnecting...");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected : ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  servo.attach(D8);
  servo.write(170);

  Serial.begin(9600);

  setupWiFi();

  // server address, port and URL
  socketIO.begin(SERVER, 4001, "/socket.io/?EIO=4");

  socketIO.onEvent(socketIOEvent);
}

void loop()
{
  socketIO.loop();
}