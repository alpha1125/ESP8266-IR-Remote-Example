#include "Arduino.h"
#include "../lib/IRremoteESP8266/src/IRremoteESP8266.h"
#include "../lib/IRremoteESP8266/src/IRsend.h"
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include <vector>

#include "configuration.h"

#define DELAY_BETWEEN_COMMANDS 1000

//#define IR_SEND_PIN D2
//#define WEB_SERVER_PORT 80
//
//#define MDNS_NAME "sharpremote2"
//#define SSID "LeungIoT"
//#define SSID_PASSWORD "4uD*qtNWgy"

const char* mDNSName = MDNS_NAME;
const char* ssid = SSID;
const char* password = SSID_PASSWORD;

ESP8266WebServer server(WEB_SERVER_PORT);



IRsend irsend(IR_SEND_PIN);

const int led = BUILTIN_LED;

String rowDiv = "    <div class=\"row\">\n";
String endDiv = "    </div>\n";
char buttonTemplate[] = R"=====(
    <div class="%s" style="text-align: center">
      <button id="%s" type="button" class="%s" onclick='makeAjaxCall("%s")'>%s</button>
    </div>
)=====";





// Buttons are using the bootstrap grid for sizing - http://getbootstrap.com/css/#grid
String generateButton(const char *divClass, const char *label, const char *url, const char *btnClass="btn btn-default btn-block") {
    char s[1000];
    sprintf(
            s,
            buttonTemplate,

            divClass, // divClass
            url,      // btnID
            btnClass, // btnClass
            url,      // ajaxURL
            label     // buttn label.
    );
    return s;
}



// Buttons are using the bootstrap grid for sizing - http://getbootstrap.com/css/#grid
String generateSpacer(String classDefinition) {
    return  "<div class=\"" + classDefinition + "\"></div>\n";
}


void handleRoot() {
    digitalWrite(led, 0);
    String website = R"=====(<!DOCTYPE html>
<html>
  <head>
    <title>Sharp TV Remote</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <meta name="HandheldFriendly" content="true">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css">
<style>
html, body {
//    touch-action:none;
    -webkit-text-size-adjust: none;
}
</style>
  </head>
  <body>
    <div class="container-fluid">
)=====";

    // ------------------------- Power Controls --------------------------
    website += rowDiv;

    website += generateButton("col-12", "Power", "Power", "btn btn-danger  btn-block");
    website += generateButton("col-4", "Display", "Display");
    website += generateButton("col-4", "Sleep", "Sleep");
    website += generateButton("col-4", "AV mode", "AVmode");
    website += generateButton("col-4", "CC", "CC");
    website += generateButton("col-4", "Audio", "Audio");
    website += generateButton("col-4", "Mute", "Mute");
    website += endDiv;



    // ------------------------- Channel Controls --------------------------
    website += rowDiv;

    website += generateButton("col-3", "1", "1");
    website += generateButton("col-3", "2", "2");
    website += generateButton("col-3", "3", "3");
    website += generateButton("col-3", "CH +", "ChUp", "btn btn-primary  btn-block");
    website += endDiv;
    website += rowDiv;
    website += generateButton("col-3", "4", "4");
    website += generateButton("col-3", "5", "5");
    website += generateButton("col-3", "6", "6");
    website += generateButton("col-3", "CH -", "ChDn", "btn btn-primary  btn-block");
    website += endDiv;

    website += rowDiv;
    website += generateButton("col-3", "7", "7");
    website += generateButton("col-3", "8", "8");
    website += generateButton("col-3", "9", "9");
    website += generateButton("col-3", "Vol +", "VolUp", "btn btn-success  btn-block");
    website += endDiv;

    website += rowDiv;

    website += generateButton("col-3", "*", "*");
    website += generateButton("col-3", "0", "0");
    website += generateButton("col-3", "ENT", "ENT");
    website += generateButton("col-3", "Vol -", "VolDn", "btn btn-success  btn-block");
    website += endDiv;

    website += rowDiv;
    website += generateButton("col-3", "Flashback", "Flashback");
    website += generateButton("col-3", "View mode", "ViewMode");
    website += generateButton("col-3", "PC", "Pc");
    website += generateButton("col-3", "Input", "Input");
    website += endDiv;

    website += rowDiv;
    website += generateButton("col-6", "Freeze", "Freeze");
    website += generateButton("col-6", "Menu", "Menu");
    website += endDiv;

    website += rowDiv;


    // TODO: Insert a spacer here
    website += generateSpacer("col-4");
    website += generateButton("col-4", "Up", "Up");
    website += endDiv;

    website += rowDiv;

    website += generateButton("col-4", "Left", "Left");
    website += generateButton("col-4", "Enter", "Enter");
    website += generateButton("col-4", "Right", "Right");
    website += endDiv;

    website += rowDiv;
    // TODO: Insert a spacer here
    website += generateSpacer("col-4");
    website += generateButton("col-4", "Down", "Down");
    website += endDiv;

    website += rowDiv;
    website += generateButton("col-6", "Exit", "Exit");
    website += generateButton("col-6", "Return", "Return");


    website += endDiv;

    website += rowDiv;

    website += generateButton("col-3", "A", "A", "btn btn-danger btn-block");
    website += generateButton("col-3", "B", "B", "btn btn-success btn-block");
    website += generateButton("col-3", "C", "C", "btn btn-primary btn-block");
    website += generateButton("col-3", "D", "D", "btn btn-warning btn-block");
    website += endDiv;



    website += "    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js\"></script>\n";
    website += "    <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>\n";
    website += "    <script> function makeAjaxCall(url){$.ajax({\"url\": url})}</script>\n";
    website += "  </body>\n";
    website += "</html>\n";

    server.send(200, "text/html", website);
    digitalWrite(led, 1);
}

void handleNotFound() {
    digitalWrite(led, 1);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    digitalWrite(led, 1);
}

void sendIrCommand(const char *ResponseMsg , int command) {
    Serial.println(ResponseMsg);
    irsend.sendSharpRaw(command, kSharpBits);
    server.send(200, "text/plain", ResponseMsg);


}

void setupRoutes(){
    server.on("/", handleRoot);

    server.on("/Power", []() {    sendIrCommand("Power", 0x41A2);  });
    server.on("/Display", []() {    sendIrCommand("Display", 0x4362);  });
    server.on("/Sleep", []() {    sendIrCommand("Sleep", 0x4162);  });
    server.on("/AVmode", []() {    sendIrCommand("AV mode", 0x407E);  });
    server.on("/CC", []() {    sendIrCommand("CC", 0x414A);  });
    server.on("/Audio", []() {    sendIrCommand("Audio", 0x4062);  });
    server.on("/Mute", []() {    sendIrCommand("Mute", 0x43A2);  });
    server.on("/1", []() {    sendIrCommand("1", 0x4202);  });
    server.on("/2", []() {    sendIrCommand("2", 0x4102);  });
    server.on("/3", []() {    sendIrCommand("3", 0x4302);  });
    server.on("/4", []() {    sendIrCommand("4", 0x4082);  });
    server.on("/5", []() {    sendIrCommand("5", 0x4282);  });
    server.on("/6", []() {    sendIrCommand("6", 0x4182);  });
    server.on("/7", []() {    sendIrCommand("7", 0x4382);  });
    server.on("/8", []() {    sendIrCommand("8", 0x4042);  });
    server.on("/9", []() {    sendIrCommand("9", 0x4242);  });
    server.on("/0", []() {    sendIrCommand("0", 0x4142);  });
    server.on("/*", []() {    sendIrCommand("*", 0x4572);  });
    server.on("/ENT", []() {    sendIrCommand("ENT", 0x4342);  });

    server.on("/ChUp", []() {    sendIrCommand("CH +", 0x4222);  });
    server.on("/ChDn", []() {    sendIrCommand("CH -", 0x4122);  });
    server.on("/VolUp", []() {    sendIrCommand("Vol +", 0x40A2);  });
    server.on("/VolDn", []() {    sendIrCommand("Vol -", 0x42A2);  });



    server.on("/Flashback", []() {    sendIrCommand("Flashback", 0x43d2);  });
    server.on("/ViewMode", []() {    sendIrCommand("View mode", 0x4016);  });
    server.on("/Pc", []() {    sendIrCommand("Pc", 0x3e06);  });
    server.on("/Input", []() {    sendIrCommand("Input", 0x4322);  });

    server.on("/Freeze", []() {    sendIrCommand("Freeze", 0x432a);  });
    server.on("/Up", []() {    sendIrCommand("Up", 0x43aa);  });
    server.on("/Down", []() {    sendIrCommand("Down", 0x406a);  });
    server.on("/Left", []() {    sendIrCommand("Left", 0x42be);  });
    server.on("/Right", []() {    sendIrCommand("Right", 0x41be);  });
    server.on("/Enter", []() {    sendIrCommand("Enter", 0x43be);  });
    server.on("/Menu", []() {    sendIrCommand("Menu", 0x4012);  });
    server.on("/Exit", []() {    sendIrCommand("Exit", 0x433e);  });
    server.on("/Return", []() {    sendIrCommand("Return", 0x40be);  });

    server.on("/A", []() {    sendIrCommand("A", 0x4236);  });
    server.on("/B", []() {    sendIrCommand("B", 0x4136);  });
    server.on("/C", []() {    sendIrCommand("C", 0x4336);  });
    server.on("/D", []() {    sendIrCommand("D", 0x40b6);  });

    server.onNotFound(handleNotFound);

    server.begin();
}

void setup(void) {
    irsend.begin();
    pinMode(led, OUTPUT);
    digitalWrite(led, 1);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("Attempting to connect to WiFi");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Mac: ");
    Serial.println(WiFi.macAddress());


    if (MDNS.begin(mDNSName)) {
        Serial.println("MDNS Responder Started");
    }

    MDNS.addService("http", "tcp", 80);

    setupRoutes();
    Serial.println("HTTP Server Started");
}

void loop(void) {
    server.handleClient();
}