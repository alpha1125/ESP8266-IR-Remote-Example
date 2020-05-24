#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#include "credentials.h"
#include "configuration.h"
#include "html.h" // HTML portion.

#include "Button.h" // IR Codes portion.

std::vector<Button> vectorOfButtons;

// Uncomment to override the credentials.h file.
// #define mySSID "wifi_ssid"
// #define myPASSWORD "wifi_password"
//#define MDNS_NAME "mdnsHostName"  // =>  e.g: mdnsHostName.local

ESP8266WebServer server(WEB_SERVER_PORT);
IRsend           irsend(IR_SEND_PIN);

const int led = LED_BUILTIN;

void handleHtmlGZ(const char *encodingString, const char *gzString, const int size) {
    digitalWrite(led, 0);

    server.sendHeader("content-encoding", "gzip");
    server.sendHeader("Expires", "Mon, 1 Jan 2222 10:10:10 GMT");
    server.send_P(200, encodingString, gzString, size);

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

void sendIrCommand(const String ResponseMsg, const int command) {
    Serial.println(ResponseMsg);
    irsend.sendSharpRaw(command, kSharpBits);
    server.send(200, "text/plain", ResponseMsg);
}

// convert a char* to hex values.
int x2i(char *s)
{
    int x = 0;
    for(;;) {
        char c = *s;
        if (c >= '0' && c <= '9') {
            x *= 16;
            x += c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            x *= 16;
            x += (c - 'A') + 10;
        }
        else break;
        s++;
    }
    return x;
}

void buttonPress(){
    String message = "";
    String label = "";
    String hexcode = "";
    bool error = false;

    if (server.arg("label")== ""){     //Parameter not found
        message += "label argument not found\n";
        error = true;
    }
    if (server.arg("hexcode")== ""){     //Parameter not found
        message += "hexcode argument not found\n";
        error = true;
    }

    if(error){
        server.send(400, "text/plain", message);
        Serial.println(message);
    }

    label = server.arg("label");
    hexcode = server.arg("hexcode");

    const int code = x2i(const_cast <char*>(hexcode.c_str()));

    sendIrCommand(
             "Received req:" + hexcode + "(" + label +")",
             code
      );

    server.send(200, "text/plain", message);
    Serial.println(message);

}

void setup(void) {
    irsend.begin();
    pinMode(led, OUTPUT);
    digitalWrite(led, 1);
    Serial.begin(115200);
    WiFi.begin(mySSID, myPASSWORD);
    Serial.println("Attempting to connect to WiFi");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(mySSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Mac: ");
    Serial.println(WiFi.macAddress());

    if (MDNS.begin(MDNS_NAME)) {
        Serial.println("MDNS Responder Started");
    }

    MDNS.addService("http", "tcp", 80);

    server.on("/", []{
        handleHtmlGZ("text/html", data_index_html_gz, data_index_html_gz_len);
    });
    server.on("/css/bootstrap.min.css", []{
        handleHtmlGZ("text/css", data_css_bootstrap_min_css_gz, data_css_bootstrap_min_css_gz_len);
    });

    server.on("/favicon.ico", []{
        handleHtmlGZ("image/x-icon", data_favicon_ico_gz, data_favicon_ico_gz_len);
    });

    server.on("/button", HTTP_POST, buttonPress);
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.println("HTTP Server Started");
}

void loop(void) {
    MDNS.update();  // needs to be in the loop to rebroadcast from time to time.
    server.handleClient();
}