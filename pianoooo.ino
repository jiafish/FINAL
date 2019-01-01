
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "pitches.h"

/*#include <dht.h>
dht DHT;*/
#define CONTROL_1 23
#define CONTROL_2 22
#define CONTROL_3 1
#define CONTROL_4 3
#define CONTROL_5 21
#define CONTROL_6 18
#define CONTROL_7 19

const int led_1= 36;
const int led_2= 39;
const int led_3= 34;
const int led_4= 35;
const int led_5= 32;
const int led_6= 33;
const int led_7= 25;
const int buzzer = 8;
int i=0;
int state=0;

int star_melody[]{
   NOTE_C3,NOTE_C3,NOTE_G3,NOTE_G3,NOTE_A3,NOTE_A3,NOTE_G3,
   NOTE_F3,NOTE_F3,NOTE_E3,NOTE_E3,NOTE_D3,NOTE_D3,NOTE_C3,
   NOTE_G3,NOTE_G3,NOTE_F3,NOTE_F3,NOTE_E3,NOTE_E3,NOTE_D3,
   NOTE_G3,NOTE_G3,NOTE_F3,NOTE_F3,NOTE_E3,NOTE_E3,NOTE_D3,
   NOTE_C3,NOTE_C3,NOTE_G3,NOTE_G3,NOTE_A3,NOTE_A3,NOTE_G3,
   NOTE_F3,NOTE_F3,NOTE_E3,NOTE_E3,NOTE_D3,NOTE_D3,NOTE_C3,
};
int bee_melody[]{
   NOTE_G3,NOTE_E3,NOTE_E3,NOTE_F3,NOTE_D3,NOTE_D3,
   NOTE_C3,NOTE_D3,NOTE_E3,NOTE_F3,NOTE_G3,NOTE_G3,NOTE_G3,
   NOTE_G3,NOTE_E3,NOTE_E3,NOTE_F3,NOTE_D3,NOTE_D3,
   NOTE_C3, NOTE_E3, NOTE_G3, NOTE_G3, NOTE_E3,
   NOTE_D3,NOTE_D3,NOTE_D3,NOTE_D3,NOTE_D3,NOTE_E3,NOTE_F3,
   NOTE_E3,NOTE_E3,NOTE_E3,NOTE_E3,NOTE_E3,NOTE_F3,NOTE_G3,
   NOTE_G3,NOTE_E3,NOTE_E3,NOTE_F3,NOTE_D3,NOTE_D3,
   NOTE_C3,NOTE_E3,NOTE_G3,NOTE_G3,NOTE_C3
};

int starBeat[]={
    1,1,1,1,1,1,2, 1,1,1,1,1,1,2,

    1,1,1,1,1,1,2, 1,1,1,1,1,1,2,

    1,1,1,1,1,1,2, 1,1,1,1,1,1,2

};
int beeBeat[]={
    1,1,2, 1,1,2, 1,1,1,1,1,1,2,

    1,1,2, 1,1,2, 1,1,1,1,4,

    1,1,1,1,1,1,2, 1,1,1,1,1,1,2,

    1,1,2, 1,1,2, 1,1,1,1,4

};

int led_all[]{led_1,led_2,led_3,led_4,led_5,led_6,led_7};

int led_star[]{
    led_1,led_1,led_5,led_5,led_6,led_6,led_5,
    led_4,led_4,led_3,led_3,led_2,led_2,led_1,
    led_5,led_5,led_4,led_4,led_3,led_3,led_2,
    led_5,led_5,led_4,led_4,led_3,led_3,led_2,
    led_1,led_1,led_5,led_5,led_6,led_6,led_5,
    led_4,led_4,led_3,led_3,led_2,led_2,led_1
};
int led_bee[]{
    led_5,led_3,led_3,led_4,led_2,led_2,
    led_1,led_2,led_3,led_4,led_5,led_5,led_5,
    led_5,led_3,led_3,led_4,led_2,led_2,
    led_1,led_3,led_5,led_5,led_3,
    led_2,led_2,led_2,led_2,led_2,led_3,led_4,
    led_3,led_3,led_3,led_3,led_3,led_4,led_5,
    led_5,led_3,led_3,led_4,led_2,led_2,
    led_1,led_3,led_5,led_5,led_1
};
int CONTROL_star[]{
    CONTROL_1,CONTROL_1,CONTROL_5,CONTROL_5,CONTROL_6,CONTROL_6,CONTROL_5,
    CONTROL_4,CONTROL_4,CONTROL_3,CONTROL_3,CONTROL_2,CONTROL_2,CONTROL_1,
    CONTROL_5,CONTROL_5,CONTROL_4,CONTROL_4,CONTROL_3,CONTROL_3,CONTROL_2,
    CONTROL_5,CONTROL_5,CONTROL_4,CONTROL_4,CONTROL_3,CONTROL_3,CONTROL_2,
    CONTROL_1,CONTROL_1,CONTROL_5,CONTROL_5,CONTROL_6,CONTROL_6,CONTROL_5,
    CONTROL_4,CONTROL_4,CONTROL_3,CONTROL_3,CONTROL_2,CONTROL_2,CONTROL_1
};
int CONTROL_bee[]{
    CONTROL_5,CONTROL_3,CONTROL_3,CONTROL_4,CONTROL_2,CONTROL_2, 
    CONTROL_1,CONTROL_2,CONTROL_3,CONTROL_4,CONTROL_5,CONTROL_5,CONTROL_5,
    CONTROL_5,CONTROL_3,CONTROL_3,CONTROL_4,CONTROL_2,CONTROL_2,
    CONTROL_1,CONTROL_3,CONTROL_5,CONTROL_5,CONTROL_3,
    CONTROL_2,CONTROL_2,CONTROL_2,CONTROL_2,CONTROL_2,CONTROL_3,CONTROL_4,
    CONTROL_3,CONTROL_3,CONTROL_3,CONTROL_3,CONTROL_3,CONTROL_4,CONTROL_5,
    CONTROL_5,CONTROL_3,CONTROL_3,CONTROL_4,CONTROL_2,CONTROL_2,
    CONTROL_1,CONTROL_3,CONTROL_5,CONTROL_5,CONTROL_1
};

void star(){
  state=1;
  }
void bee(){
  state=2;
  }
  
const char* ssid = "SUGAR C11";
const char* password = "xxxxxxxx";
String website; 
WebServer server(80);



void buildWeb(){
  website +="<html>";
  website +="<head>\n";
  website +="<meta charset='UTF-8'/>\n>";
  website +="<title>Piano</title>\n";
  website +="<style>";
  website += "html{background:#FFC8B4;}";
  website += ".title{text-align:center;color:#000000; }";
  website += "</style>";
  website += "</head>\n";
  website += "<body>";
  website += "<h1 class=\"title\">Piano Class</h1>";
 // website += "<input type=\"botton\" value=\"Little Star\" name=\"bnt1\" href=\"S\" style=\"width:100px;height:30px;\" onclick=\"location.href='http://blog.ilc.edu.tw/blog/gallery/1279/1279-3541615.png'\">;
 // website += "<input type=\"botton\" value=\"Little Bee\" name=\"bnt2\" href=\"B\" style=\"width:100px;height:30px;\" onclick=\"location.href='http://i.epochtimes.com/assets/uploads/2011/11/901310431571974.jpg'\">;
  website += "</body></html>";
}
void handleRoot() {
  server.send(200, "text/html",website);

}

void handleNotFound() {
 
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
 
}


void setup(void) {
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/S",star);
  server.on("/B",bee);
  server.on("/inline",[](){
    server.send(200,"text/plain","this works as well");
    });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  i=0;state=0;
  pinMode(led_1,OUTPUT);
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
  pinMode(led_4,OUTPUT);
  pinMode(led_5,OUTPUT);
  pinMode(led_6,OUTPUT);
  pinMode(led_7,OUTPUT);
  digitalWrite(led_1, 0);
  digitalWrite(led_1, 1);
  digitalWrite(led_1, 0);
  Serial.begin(115200);
  ledcSetup(0, 2000, 8);
  ledcAttachPin(8, 0);
  pinMode(CONTROL_1, INPUT); 
  pinMode(CONTROL_2, INPUT);
  pinMode(CONTROL_3, INPUT);
  pinMode(CONTROL_4, INPUT); 
  pinMode(CONTROL_5, INPUT);
  pinMode(CONTROL_6, INPUT);
  pinMode(CONTROL_7, INPUT);
}

void loop(void) {
   bool CONTROL_state=0;
   int flag=1;
   int noteDuration;
   
   if(state==1){
      digitalWrite(led_star[i],HIGH);
      
      if(flag==1){
          CONTROL_state=digitalRead(CONTROL_star[i]);
          if(i==42)flag=0;
          if(CONTROL_state==1) {
            noteDuration=800/starBeat[i];
            ledcWriteTone(0, star_melody[i]);
            delay(noteDuration*1.30);
            flag=1;i++;
            digitalWrite(led_star[i],LOW);
            }
       }
       if(flag==0){
          for(int j=0;j<42;j++){
           noteDuration=800/starBeat[i];
            ledcWriteTone(0, star_melody[i]);
            delay(noteDuration*1.30);
            }  state=0;
        }
   } //star教學
       
   if(state==2){
       digitalWrite(led_bee[i],HIGH);
       
      if(flag==1){
          CONTROL_state=digitalRead(CONTROL_bee[i]);
          if(i==47)flag=0;
          if(CONTROL_state==1 && i<47) {
            noteDuration=800/beeBeat[i];
            ledcWriteTone(0, bee_melody[i]);
            delay(noteDuration*1.30);
            flag=1;i++;
            digitalWrite(led_bee[i],LOW);
            }   
       }
      if(flag==0){
          for(int j=0;j<42;j++){
             noteDuration=800/beeBeat[i];
            ledcWriteTone(0, bee_melody[i]);
            delay(noteDuration*1.30);
            }  state=0;
        }
  }//bee教學
  if(state==0){
    i=0;
    server.handleClient();
    }
}  
