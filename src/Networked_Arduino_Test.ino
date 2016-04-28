#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
IPAddress ip(10, 0, 1, 7);
IPAddress myDns(10, 0, 1, 1);
IPAddress gateway(10, 0, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// use default telnet port 23
EthernetServer server(23);

boolean alreadyConnected = false;
int ledPin = 2;
String commandString;

void setup() {
  pinMode(ledPin, OUTPUT);
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  server.begin();
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect (needed for Leonardo only)
  }
  Serial.print("Chat server address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for new client
  EthernetClient client = server.available();

  // when client send the first byte, say hello
  if (client) {
    if (!alreadyConnected) {
      // clean out the input buffer
      client.flush();
      commandString = ""; // clear the command string variable
      server.println("--> Please type your command and press Return ...");
      alreadyConnected = true;
    }
    while (client.available()) {
      // read the bytes coming in from the client
      char newChar = client.read();
      if (newChar == 0x0D) {
        server.print("Received this command: \n");
        server.println(commandString);
        processCommand(commandString);
      } else {
        Serial.println(newChar);
        commandString += newChar;
      }
    }
  }
}

void processCommand(String command) {
  server.print("Processing command: ");
  server.println(command);
  if (command.indexOf("photo") > -1) {
    Serial.println("Photo command received");
    server.print("Reading from photoresistor: ");
    server.println(analogRead(A0));
    commandString = "";
    return;
  }
  if (command.indexOf("ledon") > -1) {
    server.println("LED On command received");
    digitalWrite(ledPin, HIGH);
    server.println("LED was turned on");
    commandString = "";
    return;
  }
  if (command.indexOf("ledoff") > -1) {
    server.println("LED Off command received");
    digitalWrite(ledPin, LOW);
    server.println("LED was turned off");
    commandString = "";
    return;
  }
  commandString = "";
  instructions();
}

void instructions() {
  server.println("I don't understand");
  server.println("Please use one of these commands:");
  server.println("* photo, to get a reading from the photoresistor");
  server.println("* ledon, to turn the LED on");
  server.println("* ledoff, to turn the LED off");
}
