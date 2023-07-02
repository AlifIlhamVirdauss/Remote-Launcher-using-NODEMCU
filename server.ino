#include <ESP8266WiFi.h>
#include <Servo.h>
#define LED 4
Servo myservo; //declare servo

const char* ssid = "Poco M3";                  // Your wifi Name       
const char* password = "12345678"; // Your wifi Password
const char* Commands_Reply;                 // The command variable that is sent to the client

const char * host = "192.168.167.12";          // IP Client

int flag = 0;

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  
  pinMode(LED, OUTPUT); // Declare the LED as an output
  myservo.attach(5);
  myservo.write(0);
  Serial.begin(115200);                     // initialize serial:
  delay(10);

Serial.println("");
  Serial.println("Server-------------------------------");
  Serial.print("Configuring access point");
  WiFi.begin(ssid, password);


  // Waiting to connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");

  // Start the server
    server.begin();
    Serial.println("Server started");

  // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    Serial.println("-------------------------------------");
    Serial.println("");
}

void loop() {

WiFiClient client = server.available();
  if (!client) {
     int led = 4;
     if (flag == 0){digitalWrite(LED, HIGH);}
    return;
  }
   // Wait until the client sends some data
  Serial.println("Server-------------------------------");
  Serial.println("New client");
  Serial.print("From client = ");
  while(!client.available()){
      delay(1);
  }
  
  // Read the first line of the request -------------------------------------
   String req = client.readStringUntil('\r');
   Serial.println(req);
   client.flush();

   if (req.indexOf("LED_On") != -1){
      Commands_Reply = "LED Status : On";
      Serial.print("Server send = ");
      Serial.println(Commands_Reply);
      client.print(String("GET ") + Commands_Reply + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      flag = 1;
      digitalWrite(LED, LOW);
      myservo.write(90);
     
    }
   else if (req.indexOf("LED_Off") != -1){
    Commands_Reply = "LED Status : Off";
    Serial.print("Server send = ");
      Serial.println(Commands_Reply);
      client.print(String("GET ") + Commands_Reply + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    digitalWrite(LED, LOW);
     myservo.write(0);
     
    }
   else {
     Serial.println("invalid request");
     client.stop();
     return;
    }

   client.flush();
   Serial.println("Client disonnected");
   Serial.println("-------------------------------------");
   Serial.println("");
}
