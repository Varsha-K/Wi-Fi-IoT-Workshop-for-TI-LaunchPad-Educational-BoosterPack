/* TembooTwitterDemo.ino
 *
 * Example of Temboo Twitter IoT and Educational BoosterPack MK II
 * Hardware Required:
 * - TI LaunchPad
 * - BOOSTXL-CC3100 (CC3100 Wi-Fi BoosterPack)
 * - BOOSTXL-EDUMKII (Educational BoosterPack MK II)
 * 
 */

#include <WiFi.h>
#include <WiFiClient.h>

#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

WiFiClient client;

String inputstring = "no input detected";
int action = 0;
void setup() {
  pinMode(PUSH1, INPUT_PULLUP); //LP pushbutton left
  pinMode(PUSH2, INPUT_PULLUP); //LP pushbutton right
  pinMode(32, INPUT_PULLUP); // BP pushbutton 2
  pinMode(33, INPUT_PULLUP); // BP pushbutton 1
  pinMode(5, INPUT_PULLUP); // BP pushbutton joystick

  //start UART communication
  Serial.begin(9600);
  Serial.println("Welcome to the Temboo IoT LaunchPad Demo");
  
  int wifiStatus = WL_IDLE_STATUS;

  // Determine if the WiFi Shield is present
  Serial.print("\n\nShield:");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("FAIL");

    // If there's no WiFi shield, stop here
    while(true);
  }

  Serial.println("OK");

  // Try to connect to the local WiFi network
  while(wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi:");
    wifiStatus = WiFi.begin(WIFI_SSID, WPA_PASSWORD);

    if (wifiStatus == WL_CONNECTED) {
      Serial.println("OK");
    } else {
      Serial.println("FAIL");
    }
    delay(5000);
  }

  Serial.println("Setup complete.\n");
}

void loop() {

  if(digitalRead(PUSH1)==0){
    action = 1;
    inputstring = "LP Button left";
  }
  if(digitalRead(PUSH2)==0){
    action = 1;
    inputstring = "LP Button right";
  }
  if(digitalRead(32) == 0){
    action = 1;
    inputstring = "BP Button 2";
  }
  if(digitalRead(33) == 0){
    action = 1;
    inputstring = "BP Button 1";
  }
  if(digitalRead(5) == 0){
    action = 1;
    inputstring = "BP Joystick Button";
  }
    if(action == 1){
      Serial.println("Running StatusesUpdate");

      TembooChoreo StatusesUpdateChoreo(client);
  
      // Invoke the Temboo client
      StatusesUpdateChoreo.begin();
  
      // Set Temboo account credentials
      StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
      StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
      StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);
      Serial.println("Setting App Key information");
  
      // Set Choreo inputs
      String StatusUpdateValue = "For account " + String(TEMBOO_ACCOUNT) + ", Current value = " + inputstring;
      StatusesUpdateChoreo.addInput("StatusUpdate", StatusUpdateValue);
      String AccessTokenValue = "3361531038-PfYFeP3r9ykZ9hZzWCcsaQp4AHSHAX0BcBbVQ2K";
      StatusesUpdateChoreo.addInput("AccessToken", AccessTokenValue);
      String ConsumerKeyValue = "oa1pgd6ibazJFOa8X9Gp5BrmF";
      StatusesUpdateChoreo.addInput("ConsumerKey", ConsumerKeyValue);
      String ConsumerSecretValue = "f1S7cqL2aJi3HVyFOnQ9wCihao0080OIQxm0rpp41AB1yc91xJ";
      StatusesUpdateChoreo.addInput("ConsumerSecret", ConsumerSecretValue);
      String AccessTokenSecretValue = "RzlVqDmPHKaD1L315bvJ2MqhKfJW7Zsr76YfkomuKrL2a";
      StatusesUpdateChoreo.addInput("AccessTokenSecret", AccessTokenSecretValue);
      Serial.println("Setting twitter information");

      // Identify the Choreo to run
      StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");
      Serial.println("Setting choreo");
      
      
      // Run the Choreo; when results are available, print them to serial
      StatusesUpdateChoreo.run();
      Serial.println("Running Choreo");
    
      while(StatusesUpdateChoreo.available()) {
        char c = StatusesUpdateChoreo.read();
        Serial.print(c);
      }
      StatusesUpdateChoreo.close();
      Serial.println("Completed StatusesUpdate");
    }
    action = 0;
    delay(2000); // this is the same as delay() but saves power
  }
