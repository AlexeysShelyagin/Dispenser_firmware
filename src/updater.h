#ifndef UPDATER_H
#define UPDATER_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Update.h"

#include "display.h"
#include "config.h"

static String PROGMEM style = R"rawliteral(
<style>
    #file-input, input{width: 100%; height: 44px; border-radius: 4px; margin: 10px auto; font-size: 15px}
    input{background: #f1f1f1; border: 0; padding: 0 15px}
    body{background: #3498db; font-family: sans-serif; font-size: 14px; color: #777}
    #file-input{padding: 0; border: 1px solid #ddd; line-height: 44px; text-align: left; display: block; cursor:pointer}
    form{background: #fff; max-width: 258px; margin: 75px auto; padding: 30px; border-radius: 5px; text-align: center}
    .btn{background: #3498db; color: #fff; cursor: pointer;}
    .btn:hover{background: #2978ad;}
    .btn:disabled{background: #8f8f8f; cursor: auto;}
</style>
)rawliteral";

static String PROGMEM loginIndex = R"rawliteral(
<form name=loginForm>
<h1>ESP32 Login</h1>
<input name=userid placeholder='User ID'>
<input name=pwd placeholder=Password type=Password>
<input type=submit onclick=check(this.form) class=btn value=Login></form>
<script>
    function check(form) {
        if(form.userid.value==')rawliteral" + String(UPDATE_USERNAME) + R"rawliteral(' && 
        form.pwd.value==')rawliteral" + String(UPDATE_PASSWORD) + R"rawliteral(')
            window.open('/serverIndex')
        else
            alert('Wrong Password or Username')
    }
</script>
)rawliteral" + style;

static String PROGMEM serverIndex = R"rawliteral(
<form method='POST' action='upload' enctype='multipart/form-data' id='upload_form' onsubmit='uploaded()'>
    <label for='file' style="font-size: 150%;">Select firmware file:</label>
    <input type='file' name='data' id='file' style='text-wrap: wrap;' onchange='file_selected()'>
    <input type='submit' name='upload' id='btn' class='btn' value='Upload' disabled>
    <script>
        function file_selected(){
            document.getElementById('btn').disabled = false;
        }
        function uploaded(){
            document.getElementById('btn').disabled = true;
            document.getElementById('file').hidden = true;
        }
    </script>
</form>
)rawliteral" + style;

class Client_updater{
    String page_ip;
    bool exit = false, success = false;

    Display_SH1106 *display = nullptr;

    void display_info(String text);
public:
    Client_updater() = default;

    void init_display(Display_SH1106 *display_);

    void start_upload();
};

#endif