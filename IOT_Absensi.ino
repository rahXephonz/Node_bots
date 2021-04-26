#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <CTBot.h>

//Network SSID saya
const char* ssid = "Goodtime";
const char* pass = "oldbutgold1@";

//pengenal host (server) = Ip Adress komputer kita
const char* host = "192.168.1.5";

//string token dan id
String token = "1702916513:AAGshbnfriwYv9qMDf9Me8qr8aneRV2Znhw";
const int rizky = 1251046723;
const int ihwal = 1275369227;
const int ruben = 1346399558;

//Mendeklarasikan Pin buzzer
const unsigned char Active_buzzer = 4; //Pin D2

//sediakan variabel untuk RFID
#define SDA_PIN 2 // Pin D4
#define RST_PIN 0 // Pin D3

MFRC522 mfrc522(SDA_PIN, RST_PIN);
HTTPClient http;
CTBot myBot;

void setup () {
  Serial.begin(9600);
  
  //Setting Koneksi wifi
  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid, pass);

  //konfigurasi
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  Serial.println("loading");
  //cek koneksi wifi
  while(WiFi.status() != WL_CONNECTED){
   //progress mengkoneksikan wifi
   delay(1000);
   Serial.print(".");
  }

  if(myBot.testConnection()){
    //koneksi terhubung
    Serial.println("Bot Terhubung");
  }

  else{
    Serial.println("Bot Gagal Terhubung");
  }
  
  Serial.println("Wifi Tersambung");
  Serial.println("IP Adress : ");
  Serial.println(WiFi.localIP());
  
  //Setting koneksi RFID
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.print("Dekatkan Tag RFID anda.");
  Serial.println();

  //Deklarasikan buzzer dan lampu sebagai OUTPUT 
  pinMode(Active_buzzer,OUTPUT); 
}

void loop () {
  if(! mfrc522.PICC_IsNewCardPresent())
  {
    return; //Mengulang proses
  }
  if(! mfrc522.PICC_ReadCardSerial()){
    return;
  }
  
  String IDTAG = "";
  for(byte i=0; i<mfrc522.uid.size; i++){
    IDTAG += mfrc522.uid.uidByte[i];
  }

  Serial.println(IDTAG);
  
  //Jika kartu terdeteksi buzzer menyala
  digitalWrite(Active_buzzer,HIGH); //menyalakan buzzer
  delay(500);
  noTone(Active_buzzer);

  if(IDTAG == "8320217024"){
   myBot.sendMessage(rizky, "Kepada Yth, Hari ini Rizky Hadir dan masuk Sekolah");
  }
  
  else if(IDTAG == "2081022050"){
   myBot.sendMessage(ihwal, "Kepada Yth, Hari ini Ihwal Sidiq Hadir dan masuk Sekolah");
  }
  
  else if(IDTAG == "1881483173"){
   myBot.sendMessage(ruben, "Kepada Yth, Hari ini Ruben Siman Hadir dan masuk Sekolah");
  }

  //kirim No RFID untuk disimpan ketabel tmprfid
  WiFiClient client;
  const int httpPort = 80;
  if(!client.connect(host, httpPort)){
    Serial.println("Gagal Terhubung");
    return;
  }

  String Link;
  Link = "http://192.168.1.5/absensi/kirimkartu.php?nokartu=" + IDTAG;
  http.begin(Link);

  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println(payload);
  http.end();

  delay(2000);
}
