# ESP8266-IR-Remote-Example
based on https://github.com/witnessmenow/ESP8266-IR-Remote-Example lots of changes, and learning.

1. Updated IRremoteESP8266 v2.7.7 (as of writing of this README.md)
1. Used platformio and clion, as the setup.  Not arduino IDE.
1. Included code for SHARP GA667WJSA -- LCD TV LC32SB24U LC60E79U LCC6077UN LC52E77U

The code serves some files
index.html
css/bootstrap.min.css
favicon.ico (and homescreen icons)

the text/binary files are gzipped first, and stored as part of the html.h file.  It's automatically done over the build process.

I write about the build process in detail here: https://lloydleung.com/2020/05/23/platformio-clion-automation-of-gzip-encoded-files-as-bin2hex-encode-header-files/


The process to customize::
1. use https://github.com/crankyoldgit/IRremoteESP8266/tree/master/examples/IRrecvDumpV2 to obtain your IR codes (you'll need to an IR sensor)
2. modify the json (index.html), for codes and html layout (using bootstrap 4.5)
3. modify main.cpp sendIrCommand() to send your appropriate ir method (NEC/SONY/etc...) that will stay the same.

Future (maybe, and in no particular order):
1. OTA updates
2. MQTT to send/recieve the status of the device is up/down, similar to tasmota devices.  However, there is no feedback... so unsure how that will function.

Tips on this, hae a second ESP8266, so one can record, and the other send, so you can debug this easier...
