#include "updater.h"

bool uploading = false, complete = false;
uint32_t size;
String name;

String humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

void listFiles(){
    Serial.println("----------------\nFiles:");
    String res = "";
    File root = SPIFFS.open("/");
    File foundfile = root.openNextFile();
    while(foundfile){
        res += String(foundfile.name()) + "\n";
        foundfile = root.openNextFile();
    }
    root.close();
    foundfile.close();

    Serial.println(res);
    Serial.println("----------------");
}

String processor_callback(const String& var) {
    if(var == "UPLOAD_PAGE"){
        if(!complete)
            return String(upload_page);
        else
            return String(complete_page);
    }

    if (var == "FREESPIFFS")
        return humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes()));

    if (var == "USEDSPIFFS")
        return humanReadableSize(SPIFFS.usedBytes());

    if (var == "TOTALSPIFFS")
        return humanReadableSize(SPIFFS.totalBytes());

    return String();
}

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    if(!index){
        request -> _tempFile = SPIFFS.open("/" + filename, "w");
        uploading  = true;
        Serial.println("---------------------------");
        Serial.println("Upload started: " + filename);
    }
    if(len){
        request -> _tempFile.write(data, len);
        size = index + len;
        Serial.println("Progress:" + String(index) + "   " + String(len));
    }
    if(final){
        request -> _tempFile.close();
        request -> redirect("/");
        complete = true;
        size = index + len;
        name = filename;
        Serial.println("Upload complete: " + String(size));
        Serial.println("---------------------------");

        listFiles();
    }
}

void Client_updater::init_display(Display_SH1106 *display_){
    display = display_;
}

void Client_updater::start_upload(){
    display -> clear();
    display -> print("preparing...\nIt may take\na while");
    display -> show();

    SPIFFS.begin(true);

    Serial.print("formating SPIFFS... ");
    SPIFFS.format();
    Serial.println("done");

    Serial.println("Starting webserver...");
    WiFi.softAP(HOST_SSID, HOST_PASSWORD);
    page_ip = WiFi.softAPIP().toString();
    Serial.println("-----------------------\nNetwork config:");
    Serial.println("    MAC:" + String(WiFi.macAddress()));
    Serial.println("     IP:" + page_ip);
    Serial.println("-----------------------");

    server = new AsyncWebServer(WEB_SERVER_PORT);

    server -> on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        request -> send_P(200, "text/html", index_html, processor_callback);
    });
    server -> on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request -> send(200);
    }, handleUpload);

    server -> begin();
    Serial.println("Started");

    display -> clear();
    display -> print(page_ip);
    display -> show();
    
    while(!complete){
        display -> clear();
        if(uploading){
            display -> print("Uploading:\n" + humanReadableSize(size));
        }
        else{
            display -> print(page_ip + "\nwaiting file\nClick to abort");
        }
        display -> show();
    }

    display -> clear();
    display -> print("Completed\n" + name + "\nRestarting...");
    display -> show();

    values -> firmware_update = true;
    values -> save();

    delay(1000);

    ESP.restart();
}

void Client_updater::check_updates(){
    Serial.println(values -> firmware_update);
    if(!values -> firmware_update)
        return;
    
    display -> clear();
    display -> print("Updating...");
    display -> show();

    Serial.println("Firmware updating started");

    values -> firmware_update = false;
    values -> save();
    
    while (true){
        delay(100);
    }
}