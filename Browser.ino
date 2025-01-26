#include <WiFi.h>
#include <HTTPClient.h>
// made by @wdxvvy | wdxvvy.com.tr
const char* ssid = "SSID"; // Wi-Fi SSID
const char* password = "Password"; // Wi-Fi Password

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  

  // Wait for Wi-Fi connection

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);

    Serial.println("Connecting...");

  }

  Serial.println("Connected!");

}


void loop() {

  String url = ""; // Initialize URL variable

  bool validUrl = false; // Flag to check if URL is valid


  while (!validUrl) {

    Serial.println("Please enter a URL:");

    while (Serial.available() == 0) {

      // Wait for user to enter a URL

    }

    

    url = Serial.readStringUntil('\n'); // Read the URL

    Serial.print("You entered URL: ");

    Serial.println(url);

    

    HTTPClient http;

    http.begin(url); // Connect to the URL

    int httpCode = http.GET(); // Send GET request


    if (httpCode > 0) { // If request is successful

      String payload = http.getString(); // Get the response

      Serial.println("Response:");

      

      // Extract content from <body>

      int startIndex = payload.indexOf("<body>") + 6; // Start after <body>

      int endIndex = payload.indexOf("</body>");

      

      if (startIndex >= 0 && endIndex >= 0) {

        String bodyContent = payload.substring(startIndex, endIndex);

        

        // Extract text from <p>, <b>, and <font> tags

        extractTextFromTags(bodyContent, "<p>");

        extractTextFromTags(bodyContent, "<b>");

        extractTextFromTags(bodyContent, "<font>");

        

      } else {

        Serial.println("Could not find body content."); // It searches body for text's and other things to show

      }

      

      validUrl = true; // Set flag to true to exit the loop

    } else {

      Serial.printf("HTTP error code: %d\n", httpCode);

      Serial.println("Please try again with a different URL.");

    }


    http.end(); // Close the connection

  }


  delay(10000); // Wait for 10 seconds before the next loop

}


void extractTextFromTags(String content, String tag) {

  int tagStart = 0;

  while ((tagStart = content.indexOf(tag, tagStart)) != -1) {

    int tagEnd = content.indexOf("</" + tag.substring(1), tagStart); // Find the closing tag

    if (tagEnd != -1) {

      String innerText = content.substring(tagStart + tag.length(), tagEnd);

      innerText.replace("&quot;", "\""); // Replace HTML entities

      innerText.replace("&nbsp;", " "); // Replace non-breaking spaces

      Serial.println(innerText); // Print the extracted text

      tagStart = tagEnd + 1; // Move past the closing tag

    } else {

      break; // No closing tag found

    }

  }

}
