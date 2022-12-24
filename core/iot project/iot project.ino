#include "WiFi.h"
#include <FirebaseESP32.h>
#include "ArduinoJson.h"
#include <HTTPClient.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "ESP32_MailClient.h"

const int bltin_led=2, ldr=35;


#define FIREBASE_HOST "iot-qamd-default-rtdb.firebaseio.com/config"
#define FIREBASE_AUTH "0BMSRFJYCFMk8t1j1TNzVGAM8SVCU8KDhVe3DQ1b"
#define WIFI_SSID "alaa"
#define WIFI_PASSWORD "mywn9050"
#define DHTPIN 27     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11

// To send Emails using Gmail on port 465 (SSL)
#define emailSenderAccount    "iotcs52@gmail.com"
#define emailSenderPassword   "rhwmmpmzmcevncsm"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "[ALERT] ESP32"


FirebaseData fbdo;
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
SMTPData smtpData;


String email ;
bool tempemailSent = false;
bool humemailSent = false;
bool altemailSent = false;
bool presemailSent = false;



float readDHTTemperature() {

  float t = dht.readTemperature();

  if (isnan(t)) {    
    return -1;
  }
  else {
    return t;
  }
}

float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    return -1;
  }
  else {
    return h;
  }
}

bool sendEmailNotification(String emailMessage,String receiver){
  // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("ESP32", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  smtpData.setMessage(emailMessage, true);

  // Add recipients
  smtpData.addRecipient(receiver);

  smtpData.setSendCallback(sendCallback);

  // Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData)) {
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    return false;
  }
  // Clear all data from Email object to free memory
  smtpData.empty();
  return true;
}

// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}



void setup(){
   Serial.begin(115200);
   pinMode(ldr,INPUT);
  pinMode(bltin_led,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");}

}

void loop() {
  float tempThresh ;
  float presThresh ;
  float humThresh ;
  float altThresh ;
  if(WiFi.status() == WL_CONNECTED){
  HTTPClient http;
  http.begin("https://iot-qamd-default-rtdb.firebaseio.com/config.json");

  
   if(HTTP_CODE_OK == http.GET()) {

    String str = http.getString();
    DynamicJsonDocument DJD(1024);
    deserializeJson(DJD,str);
    email = DJD["email"].as<String>();
    tempThresh = DJD["tempThresh"];
    presThresh = DJD["presThresh"];
    humThresh = DJD["humThresh"];
    altThresh = DJD["altThresh"];

   }
  http.end();
  }

  Serial.print("Email after change : ");
  Serial.println(String(email));
  Serial.print("tempThresh after change : ");
  Serial.println(String(tempThresh));
  Serial.print("presThresh after change : ");
  Serial.println(String(presThresh));
  Serial.print("humThresh after change : ");
  Serial.println(String(humThresh));
  Serial.print("altThresh after change : ");
  Serial.println(String(altThresh));

  HTTPClient httpGet;
  httpGet.begin("https://iot-qamd-default-rtdb.firebaseio.com/reads.json");
  DynamicJsonDocument DJD(1024);
  if(HTTP_CODE_OK == httpGet.GET()) {

    String str = httpGet.getString();


  deserializeJson(DJD,str);}
  httpGet.end();

  
  //Post
  float temp = DJD["temp"];
  float hum = DJD["hum"];
  if (readDHTTemperature() > -1){

  temp = readDHTTemperature();
  hum = readDHTHumidity();
  }
  float pres = bmp.readPressure();
  float alt = bmp.readAltitude();
  HTTPClient httpPut;
  httpPut.begin("https://iot-qamd-default-rtdb.firebaseio.com/reads.json");
  httpPut.addHeader("Content-Type", "application/json");
  httpPut.addHeader("x-http-method-override", "PUT");
  String httpPost =   "{\"temp\":"; 
  httpPost += temp;
  httpPost +=   ",\"hum\":"; 
  httpPost += hum;
  httpPost +=   ",\"pres\":"; 
  httpPost += pres;
  httpPost +=   ",\"alt\":"; 
  httpPost += alt;
  httpPost += "}";
  int httpResponseCode = httpPut.POST(httpPost);   
  if(httpResponseCode>0){
 
    String response = httpPut.getString();   
 
    Serial.println(httpResponseCode);
    Serial.println(response);          
 
   }else{
 
    Serial.print("Error on sending PUT Request: ");
    Serial.println(httpResponseCode);
 
   }
 
   httpPut.end();

  //  Check if temperature is above threshold and if it needs to send the Email alert
  if(temp > tempThresh && !tempemailSent){
    String emailMessage = String("Temperature above threshold. Current temperature: ") + 
                          String(temp) + String("C");
    if(sendEmailNotification(emailMessage,email)) {
      tempemailSent = true;
    }
    else {
      Serial.println("Email failed to send");
    }
  }
  // Check if temperature is below threshold and if it needs to send the Email alert
  else if(temp < tempThresh && tempemailSent) {
    String emailMessage = String("Temperature below threshold. Current temperature: ") + 
                          String(temp) + String(" C");
    if(sendEmailNotification(emailMessage,email)) {
      tempemailSent = false;
    }
    else {
      Serial.println("Email failed to send");
    }
  }

  
  if(hum > humThresh && !humemailSent){
    String emailMessage = String("Humidity above threshold. Current Humidity: ") + 
                          String(hum) + String("%");
    if(sendEmailNotification(emailMessage,email)) {
      humemailSent = true;
    }
    else {
      Serial.println("Email failed to send");
    }
  }
  else if(hum < humThresh && humemailSent) {
    String emailMessage = String("Humidity below threshold. Current Humidity: ") + 
                          String(hum) + String(" %");
    if(sendEmailNotification(emailMessage,email)) {
      humemailSent = false;
    }
    else {
      Serial.println("Email failed to send");
    }
  }

  if(alt > altThresh && !altemailSent){
    String emailMessage = String("Altitude above threshold. Current Altitude: ") + 
                          String(alt) + String("m");
    if(sendEmailNotification(emailMessage,email)) {
      altemailSent = true;
    }
    else {
      Serial.println("Email failed to send");
    }
  }
  else if(alt < altThresh && altemailSent) {
    String emailMessage = String("Altitude below threshold. Current Altitude: ") + 
                          String(alt) + String(" m");
    if(sendEmailNotification(emailMessage,email)) {
      altemailSent = false;
    }
    else {
      Serial.println("Email failed to send");
    }
  }

  if(pres > presThresh && !presemailSent){
    String emailMessage = String("Pressure  above threshold. Current Pressure : ") + 
                          String(pres) + String("pa");
    if(sendEmailNotification(emailMessage,email)) {
      presemailSent = true;
    }
    else {
      Serial.println("Email failed to send");
    }
  }
  else if(pres < presThresh && presemailSent) {
    String emailMessage = String("Pressure  below threshold. Current Pressure : ") + 
                          String(pres) + String(" pa");
    if(sendEmailNotification(emailMessage,email)) {
      presemailSent = false;
    }
    else {
      Serial.println("Email failed to send");
    }
  }

  int ldr_state = digitalRead(ldr);
  if(ldr_state==1)
    digitalWrite(bltin_led,HIGH);
  else
    digitalWrite(bltin_led,LOW);

  Serial.print("ldr state : ");
  Serial.println(ldr_state);



  Serial.println("---------------------------------------");


  delay(5000);
}
