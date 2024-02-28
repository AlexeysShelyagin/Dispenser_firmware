#ifndef UPDATER_H
#define UPDATER_H

#include <SPIFFS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "display.h"
#include "config.h"
#include "values.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
    <meta name="Firmware update" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
</head>
<body>
    %UPLOAD_PAGE%
</body>
</html>
)rawliteral";

const char upload_page[] PROGMEM = R"rawliteral(
    <p><h1>Firmware Upload</h1></p>
    <p>Free Storage: %FREESPIFFS% | Used Storage: %USEDSPIFFS% | Total Storage: %TOTALSPIFFS%</p>
    <form method="POST" action="/upload" enctype="multipart/form-data"><input type="file" name="data"/><input type="submit" name="upload" value="Upload" title="Upload File"></form>
)rawliteral";

const char complete_page[] PROGMEM = R"rawliteral(
    <p><h1>Firmware Uploaded</h1></p>
    <p>close the page</p>
)rawliteral";

class Client_updater{
    AsyncWebServer *server;
    String page_ip;

    Display_SH1106 *display = nullptr;
public:
    Client_updater() = default;

    void init_display(Display_SH1106 *display_);

    void start_upload();
    void check_updates();
};

#endif