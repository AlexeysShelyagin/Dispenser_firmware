#include "updater.h"

bool uploading = false, completed = false, failed = false;
uint32_t uploaded_size = 0;

String human_readable_size(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

void Client_updater::init_display(Display_SH1106 *display_){
    display = display_;
}

void Client_updater::display_info(String text){
    display -> clear();
    display -> print(text);
    display -> show();
}

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    if(!index){
        Serial.println("Recieving file: " +  String(filename));
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)){
            Serial.println("ERROR: update begin failed    CODE: " + String(Update.getError()));
            failed = true;
            return;
        }
        uploading = true;
    }
    if(len){
        Update.write(data, len);

        uploaded_size = index + len;
    }
    if(final){
        if (!Update.end(true)) {
            Serial.println("ERROR: updload ended incorrectly    CODE: " + String(Update.getError()));
            failed = true;
            return;
        } 

        Serial.println("Updated successfully! (" + human_readable_size(index + len) + ")");
        completed = true;
    }
}

void Client_updater::start_upload(){
    display_info("preparing...");

    Serial.println("Starting webserver...");
    WiFi.softAP(HOST_SSID, HOST_PASSWORD);
    page_ip = WiFi.softAPIP().toString();
    Serial.println("-----------------------\nNetwork config:");
    Serial.println("    MAC:" + String(WiFi.macAddress()));
    Serial.println("     IP:" + page_ip);
    Serial.println("-----------------------");

    AsyncWebServer *server = new AsyncWebServer(WEB_SERVER_PORT);

    server -> on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request -> send(200, "text/html", loginIndex);
    });

    server -> on("/serverIndex", HTTP_GET, [](AsyncWebServerRequest *request) {
        request -> send(200, "text/html", serverIndex); 
    });

    server -> on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request -> send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    }, handleUpload);

    server -> begin();
    Serial.println("Server created");
    display_info(page_ip + "\nWaiting a file");

    while(!completed){
        if(uploading){
            display_info("Uploading:\n" + human_readable_size(uploaded_size));
        }
        if(failed){
            display_info("Upload failed");
            delay(4000);
            break;
        }
    }

    server -> end();
    WiFi.softAPdisconnect(true);

    if(completed){
        display_info("Completed!\nRestarting...");
        delay(1000);
        ESP.restart();
    }
}