#include <SoftwareSerial.h>

// serial config
#define     RX    10
#define     TX    11
SoftwareSerial AT(RX,TX); 

// TODO: change user config
String ssid     = "Vini"; //Wifi SSID
String password = "TESTE"; //Wifi Password
String apiKeyIn = "v3kGXejHcPu5uYL6F22DubNzQYoLbzkY"; // API Key
const unsigned int writeInterval = 25000; // write interval (in ms)

// ASKSENSORS API host config
String host = "api.asksensors.com";  // API host name
String port = "80";      // port

int AT_cmd_time; 
boolean AT_cmd_result = false; 

void setup() {
  Serial.begin(9600);
  // open serial 
  Serial.println("*****************************************************");
  Serial.println("********** Programa iniciado : Conectando ao Wifi");
  AT.begin(115200);
  Serial.println("Iniciando comandos AT com o ESP8266 ");
  sendATcmd("AT",5,"OK");
  sendATcmd("AT+CWMODE=1",5,"OK");
  Serial.print("Connecting to WiFi:");
  Serial.println(ssid);
  sendATcmd("AT+CWJAP=\""+ ssid +"\",\""+ password +"\"",20,"OK");
}

void loop() {

 // Create the URL for the request
  String url = "GET /write/";
  url += apiKeyIn;
  url += "?module1=";
  url += random(10, 100);
  Serial.println("*****************************************************");
  Serial.println("********** Abrindo conexão TCP ");
  sendATcmd("AT+CIPMUX=1", 10, "OK");
  sendATcmd("AT+CIPSTART=0, \"TCP\",\"" + host +"\"," + port, 20, "OK");
  sendATcmd("AT+CIPSEND=0," + String(url.length() + 4), 10, ">");
  
  Serial.print("********** Requisitando  URL: ");
  Serial.println(url);
  AT.println(url);
  delay(2000);
  sendATcmd("AT+CIPCLOSE=0", 10, "OK");
  
  Serial.println("********** Fechando conexão TCP ");
  Serial.println("*****************************************************");
  
  delay(writeInterval);   // delay
  }



// sendATcmd
void sendATcmd(String AT_cmd, int AT_cmd_maxTime, char readReplay[]) {
  Serial.print("AT command:");
  Serial.println(AT_cmd);

  while(AT_cmd_time < (AT_cmd_maxTime)) {
    AT.println(AT_cmd);
    if(AT.find(readReplay)) {
      AT_cmd_result = true;
      break;
    }
  
    AT_cmd_time++;
  }
  Serial.print("...Result:");
  if(AT_cmd_result == true) {
    Serial.println("DONE");
    AT_cmd_time = 0;
  }
  
  if(AT_cmd_result == false) {
    Serial.println("FAILED");
    AT_cmd_time = 0;
  }
  
  AT_cmd_result = false;
 }
