/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "1234";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
	server.send(200, "text/html", "<h1>You are connected</h1><a href=\"/home\">Go Home</a>");
}

void handleHome(){
  int a = analogRead(A0);
  server.sendHeader("Refresh","5",false);
  server.send(200, "text/html", "<h1>You are home</h1><h2>a="+String(a)+"</a>");
}

void setup() {
	delay(1000);
	Serial.begin(115200);
	Serial.print("Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
  server.on("/home", handleHome);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}
