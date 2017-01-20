#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

WiFiClient client;

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  
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
  if (numRuns <= maxRuns) {
    Serial.println("Running GetTemperature - Run #" + String(numRuns++));

    TembooChoreo GetTemperatureChoreo(client);

    // Invoke the Temboo client
    GetTemperatureChoreo.begin();

    // Set Temboo account credentials
    GetTemperatureChoreo.setAccountName(TEMBOO_ACCOUNT);
    GetTemperatureChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    GetTemperatureChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String AddressValue = "Charlotte,NC";
    GetTemperatureChoreo.addInput("Address", AddressValue);

    // Identify the Choreo to run
    GetTemperatureChoreo.setChoreo("/Library/Yahoo/Weather/GetTemperature");

    // Run the Choreo; when results are available, print them to serial
    GetTemperatureChoreo.run();
    
    while(GetTemperatureChoreo.available()) {
      char c = GetTemperatureChoreo.read();
      Serial.print(c);
    }
    GetTemperatureChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between GetTemperature calls
}
