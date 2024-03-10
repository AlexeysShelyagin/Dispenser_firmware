#include "updater.h"

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

void Client_updater::start_upload(){
    display_info("preparing...");

    Serial.println("Starting webserver...");
    WiFi.softAP(HOST_SSID, HOST_PASSWORD);
    page_ip = WiFi.softAPIP().toString();
    Serial.println("-----------------------\nNetwork config:");
    Serial.println("    MAC:" + String(WiFi.macAddress()));
    Serial.println("     IP:" + page_ip);
    Serial.println("-----------------------");

    if (!MDNS.begin(MDNS_NAME)){
        Serial.println("ERROR: setting up MDNS responder");
        display_info("Failed create server");

        delay(1000);
        return;
    }

    WebServer server(WEB_SERVER_PORT);
    
    Serial.println("Server created");
    display_info(page_ip + "\nWaiting a file");


    server.on("/", HTTP_GET, [&server]() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", loginIndex);
    });

    server.on("/serverIndex", HTTP_GET, [&server]() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", serverIndex); 
    });

    server.on("/update", HTTP_POST, [&server]() {
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    }, [&server, this](){
        HTTPUpload& upload = server.upload();

        if(upload.status == UPLOAD_FILE_START){
            Serial.println("Recieving file: " +  String(upload.filename));
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)){
                Serial.println("ERROR: update begin failed    CODE: " + String(Update.getError()));
                display_info("Upload failed");

                delay(1000);
                exit = true;
                return;
            }
            Serial.println(">>>" + human_readable_size(upload.totalSize));
        } 
        if(upload.status == UPLOAD_FILE_WRITE){
            Update.write(upload.buf, upload.currentSize);

            display_info("Uploading:\n" + human_readable_size(upload.currentSize));
        } 
        if(upload.status == UPLOAD_FILE_END){
            if (!Update.end(true)) {
                Serial.println("ERROR: updload ended incorrectly    CODE: " + String(Update.getError()));
                display_info("Upload failed");

                delay(1000);
                exit = true;
                return;
            } 

            Serial.println("Updated successfully! (" + human_readable_size(upload.totalSize) + ")");
            display_info("Completed!\nRestarting...");
            delay(1000);

            exit = true;
            success = true;
        }
    });

    server.begin();

    exit = false;
    success = false;
    while(!exit){
        server.handleClient();
        delay(1);
    }

    if(success)
        ESP.restart();
}