/*
BMP 180

#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");
	while (1) {}
  }
}
  
void loop() {
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
    
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
    
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");

  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  Serial.print("Real altitude = ");
  Serial.print(bmp.readAltitude(102000));
  Serial.println(" meters");
    
  Serial.println();
  delay(5000);
}

*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*

dht without mail


#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 33     // Digital pin connected to the DHT sensor

#define DHTTYPE    DHT11     // DHT 11


DHT dht(DHTPIN, DHTTYPE);



String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}


String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.begin();
  
}
 
void loop(){

  Serial.print("DHT Temp : " );
  Serial.print(readDHTTemperature());
  Serial.print("DHT hum : " );
  Serial.print(readDHTHumidity());

  delay(1000);

}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*


// gmail password : rhwmmpmzmcevncsm

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "ESP32_MailClient.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 33     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "eslam";
const char* password = "987654321";

// To send Emails using Gmail on port 465 (SSL)
#define emailSenderAccount    "iotcs52@gmail.com"
#define emailSenderPassword   "rhwmmpmzmcevncsm"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "[ALERT] ESP32 Temperature"

// Default Recipient Email Address
String inputMessage = "iotcs52@gmail.com";
String enableEmailChecked = "checked";
String inputMessage2 = "true";
// Default Threshold Temperature Value
String inputMessage3 = "22.0";
String lastTemperature;

// HTML web page to handle 3 input fields (email_input, enable_email_input, threshold_input)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Email Notification with Temperature</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h2>ESP32 Temperature</h2> 
  <h3>%TEMPERATURE% &deg;C</h3>
  <h2>ESP Email Notification</h2>
  <form action="/get">
    Email Address <input type="email" name="email_input" value="%EMAIL_INPUT%" required><br>
    Enable Email Notification <input type="checkbox" name="enable_email_input" value="true" %ENABLE_EMAIL%><br>
    Temperature Threshold <input type="number" step="0.1" name="threshold_input" value="%THRESHOLD%" required><br>
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}
// Replaces placeholder with DS18B20 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return lastTemperature;
  }
  else if(var == "EMAIL_INPUT"){
    return inputMessage;
  }
  else if(var == "ENABLE_EMAIL"){
    return enableEmailChecked;
  }
  else if(var == "THRESHOLD"){
    return inputMessage3;
  }
  return String();
}

// Flag variable to keep track if email notification was sent or not
bool emailSent = false;

const char* PARAM_INPUT_1 = "email_input";
const char* PARAM_INPUT_2 = "enable_email_input";
const char* PARAM_INPUT_3 = "threshold_input";

// Interval between sensor readings. Learn more about timers: https://RandomNerdTutorials.com/esp32-pir-motion-sensor-interrupts-timers/
unsigned long previousMillis = 0;     
const long interval = 5000;    

// The Email Sending data object contains config and data to send
SMTPData smtpData;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  
  // Start the DHT11 sensor
  dht.begin();

  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Receive an HTTP GET request at <ESP_IP>/get?email_input=<inputMessage>&enable_email_input=<inputMessage2>&threshold_input=<inputMessage3>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET email_input value on <ESP_IP>/get?email_input=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      // GET enable_email_input value on <ESP_IP>/get?enable_email_input=<inputMessage2>
      if (request->hasParam(PARAM_INPUT_2)) {
        inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
        enableEmailChecked = "checked";
      }
      else {
        inputMessage2 = "false";
        enableEmailChecked = "";
      }
      // GET threshold_input value on <ESP_IP>/get?threshold_input=<inputMessage3>
      if (request->hasParam(PARAM_INPUT_3)) {
        inputMessage3 = request->getParam(PARAM_INPUT_3)->value();
      }
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    Serial.println(inputMessage2);
    Serial.println(inputMessage3);
    request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   
    lastTemperature = readDHTTemperature();
    float temperature = lastTemperature.toFloat();  
    // Check if temperature is above threshold and if it needs to send the Email alert
    if(temperature > inputMessage3.toFloat() && inputMessage2 == "true" && !emailSent){
      String emailMessage = String("Temperature above threshold. Current temperature: ") + 
                            String(temperature) + String("C");
      if(sendEmailNotification(emailMessage)) {
        Serial.println(emailMessage);
        emailSent = true;
      }
      else {
        Serial.println("Email failed to send");
      }    
    }
    // Check if temperature is below threshold and if it needs to send the Email alert
    else if((temperature < inputMessage3.toFloat()) && inputMessage2 == "true" && emailSent) {
      String emailMessage = String("Temperature below threshold. Current temperature: ") + 
                            String(temperature) + String(" C");
      if(sendEmailNotification(emailMessage)) {
        Serial.println(emailMessage);
        emailSent = false;
      }
      else {
        Serial.println("Email failed to send");
      }
    }
  }
}

bool sendEmailNotification(String emailMessage){
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
  smtpData.addRecipient(inputMessage);

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

*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <FirebaseESP32.h>

const int bltin_led=26, ldr=35;

#define FIREBASE_HOST "iot-qamd-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "0BMSRFJYCFMk8t1j1TNzVGAM8SVCU8KDhVe3DQ1b"
#define WIFI_SSID "eslam"
#define WIFI_PASSWORD "987654321"

// Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

void setup() {
  Serial.begin(115200);
  pinMode(ldr,INPUT);
  pinMode(bltin_led,OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WIFI");

  while (WiFi.status() != 3){
    Serial.print(".");

    delay(300);
  }
  Serial.println(" ");
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.setReadTimeout(firebaseData, 1000 * 60);

  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  String path = "/data"; 

  Serial.println("----------------------------------");
  Serial.println("Connected..."); 
}


void loop() {
  int ldr_state = digitalRead(ldr);
  if(ldr_state==1)
  {
    digitalWrite(bltin_led,HIGH);
  }
  else
  {
    digitalWrite(bltin_led,LOW);
  }
  
  Serial.println(ldr_state);
  json.set("/data", ldr_state);
  Firebase.updateNode(firebaseData, "/LDR", json);
  delay(100);
}

