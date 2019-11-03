
#include <ESP8266WiFi.h>
#include<WebSocketsServer.h>

// Replace with your network credentials
const char* ssid     = "AndroidAP69C0";
const char* password = "hygb0690";

// Set web server port number to 80
WiFiServer server(80);
WebSocketsServer webSocket = WebSocketsServer(80);
// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
int led1State = 0;
int led2State = 0;
int motorState = 0;
int ledauto=0;
int motorpowerState = 0;

int lockState = 0;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
int number=1;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  // Print local IP address and start web server

//  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
 static char input[6];
 static uint8_t i;
int state = motorpowerState + 100*ledauto + 10*motorState + 1000*led2State + 10000*led1State ;
 Serial.print(state + "\r");
 
  if(Serial.available()){
    
     char c = Serial.read(); 
         
     if ( c != '\r' && i < 1 ) // "Carriage Return" is chosen  as the line ending character
     input[i++] = c;
     
     else
     {
     input[i] = '\0';
     i = 0;
       
      number = atoi( input );
     
     }  
    
  }
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
       // Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /led1/on") >= 0) {
              
              led1State = 1;
             // digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /led1/off") >= 0) {
             
              led1State = 0;
              //digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /led2/on") >= 0) {
              //Serial.println("3");
              led2State = 1;
              //digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /led2/off") >= 0) {
             // Serial.println("4");
              led2State = 0;
             // digitalWrite(output4, LOW);
            }
            else if (header.indexOf("GET /led/auto") >= 0) {
              //Serial.println("3");
              ledauto = 1;
              //digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /led/manual") >= 0) {
             // Serial.println("4");
              ledauto = 0;
             // digitalWrite(output4, LOW);
            }
            else if (header.indexOf("GET /motor/off") >= 0) {
             // Serial.println("4");
              motorpowerState = 0;
             // digitalWrite(output4, LOW);
            }
            else if (header.indexOf("GET /motor/on") >= 0) {
             // Serial.println("4");
              motorpowerState = 1;
             // digitalWrite(output4, LOW);
            }else if (header.indexOf("GET /motor/1") >= 0) {
             // Serial.println("4");
              motorState = 1;
             // digitalWrite(output4, LOW);
            }else if (header.indexOf("GET /motor/2") >= 0) {
             // Serial.println("4");
              motorState = 2;
             // digitalWrite(output4, LOW);
            }else if (header.indexOf("GET /motor/3") >= 0) {
             // Serial.println("4");
              motorState = 3;
             // digitalWrite(output4, LOW);
            }else if (header.indexOf("GET /motor/4") >= 0) {
             // Serial.println("4");
              motorState = 4;
             // digitalWrite(output4, LOW);
            }else if (header.indexOf("GET /motor/5") >= 0) {
             // Serial.println("4");
              motorState = 5;
             // digitalWrite(output4, LOW);
            }
             else if (header.indexOf("GET /intruder/off") >= 0) {
             // Serial.println("4");
              
              lockState=0;
             // digitalWrite(output4, LOW);
            }
            else if (header.indexOf("GET /lock/off") >= 0) {
             // Serial.println("4");
              
              lockState=0;
             // digitalWrite(output4, LOW);
            }
            else if (header.indexOf("GET /lock/on") >= 0) {
             // Serial.println("4");
              lockState = 1;
             // digitalWrite(output4, LOW);
            }

            //Alert page
            if(number ==1 && lockState==1   ){
              client.println("<!DOCTYPE html><html>");
              client.println("<head><style>h1 {color:red;}");
              client.println(" div {background-color: lightgrey;width: 500px;length:600px;border: 70px solid red;padding: 50px 50px;margin: 28px;}");
              client.println(" p {color:red;padding:150px 15px;text-align:center;font-size:40px;}");
              client.println(".button {background-color: #4CAF50;border: none;color: white;padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}</style></head>");    
              client.println("<body><h1></h1><div><p>Intruder alert<a href=\"/intruder/off\"><button class=\"button\">Off</button></a></p></div></body></html>");


              
              }

             // normal page
             
             else{
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            
            client.println(".button { display: inline-block; padding: 15px 25px;font-size: 24px; cursor: pointer;text-align: center;text-decoration: none;outline: none;color: #fff;background-color: #4CAF50;border: none;border-radius: 15px;box-shadow: 0 9px #999;}");
            
            
            client.println(".button:hover {background-color: #3e8e41}");
            client.println(".button:active { background-color: #3e8e41;box-shadow: 0 5px #666;transform: translateY(4px);}");

            client.println(".button2 { display: inline-block; padding: 15px 25px;font-size: 24px; cursor: pointer;text-align: center;text-decoration: none;outline: none;color: #fff;background-color: #4CAF50;border: none;border-radius: 15px;box-shadow: 0 9px #999;}");
            
            client.println(".button2:hover {background-color: #3e8e41}");
            client.println(".button2:active { background-color: #3e8e41;box-shadow: 0 5px #666;transform: translateY(4px);}");
            
            client.println(".button3 { display: inline-block; padding: 15px 25px;font-size: 24px; cursor: pointer;text-align: center;text-decoration: none;outline: none;color: #fff;background-color: #4CAF50;border: none;border-radius: 15px;box-shadow: 0 9px #999;}");
            
            client.println(".button3:hover {background-color: #3e8e41}");
            client.println(".button3:active { background-color: #3e8e41;box-shadow: 0 5px #666;transform: translateY(4px);}");

                 
            client.println(".button4 { display: inline-block; padding: 5px 15px;font-size: 12px; cursor: pointer;text-align: center;text-decoration: none;outline: none;color: #fff;background-color: #4CAF50;border: none;border-radius: 15px;box-shadow: 0 3px #999;}");           
            client.println(".button4:hover {background-color: #3e8e41}");
            client.println(".button4:active { background-color: #3e8e41;box-shadow: 0 5px #666;transform: translateY(4px);}");
            
            client.println(".button5 { display: inline-block; padding: 5px 15px;font-size: 12px; cursor: pointer;text-align: center;text-decoration: none;outline: none;color: #fff;background-color: #4CAF50;border: none;border-radius: 15px;box-shadow: 0 3px #999;}");           
            client.println(".button5:hover {background-color: #3e8e41}");
            client.println(".button5:active { background-color: #3e8e41;box-shadow: 0 5px #666;transform: translateY(4px);}");
            client.println("div{background-color: lightgrey;width: 250px;length:60px;border: 7px solid red;padding: 50px 50px;margin: 28px;}p {color:red;padding:150px 15px;text-align:center;font-size:40px;} </style></head>");
            
            
         
            
            // Web Page Heading
            client.println("<body><h1>Smart Lab</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            //client.println("<p>Led 1 - State " + output5State + "</p>");
           
            // If the output5State is off, it displays the ON button 


            if(ledauto == 0){
              client.println("<p><a href=\"/led/auto\"><button class=\"button button3 \">Led auto</button></a>");

                if(led1State == 0){
                  client.print("<pre><p><a href=\"/led1/on\"><button class=\"button\">Led 1-ON</button></a>");
                } else {
                  client.print("<pre><p><a href=\"/led1/off\"><button class=\"button \">Led 1-OFF</button></a>");
                } 
                
                // Display current state, and ON/OFF buttons for GPIO 4  
               // client.println("<p>Led2 - State " + output4State + "</p>");
              
                // If the output4State is off, it displays the ON button 
                      
                if (led2State==0) {
                  client.println("                  <a href=\"/led2/on\"><button class=\"button button2\">Led 2-ON</button></a></p></pre>");
                } else {
                  client.println("                  <a href=\"/led2/off\"><button class=\"button button2\">Led 2-OFF</button></a></p></pre>");
                }

   
           } else {
              client.println("<p><a href=\"/led/manual\"><button class=\"button button3\">Led manual</button></a>");
//              client.println("");
//                 client.println("</pre>");
            } 
       

                if(motorpowerState == 0){

                  client.println("<a href=\"/motor/on\"><button class=\"button button4\">motor-ON</button></a>");
                }

                else{
                  if(motorState == 3)  
                    client.println("<pre><div>        <input type=\"radio\" onclick=\"window.location='/motor/3';\" name=\"motor\" value=\"3\" checked> 3 ");
                  else
                    client.println("<pre><div>        <input type=\"radio\" onclick=\"window.location='/motor/3';\" name=\"motor\" value=\"3\" checked> 3 ");
                  
                    client.println("");
                    
                  if(motorState == 2)
                    client.println("     <input type=\"radio\" onclick=\"window.location='/motor/2';\" name=\"motor\" value=\"2\" checked> 2            <input type=\"radio\" onclick=\"window.location='/motor/4';\" name=\"motor\" value=\"4\"> 4"); 
                  else if (motorState == 4)
                   client.println("     <input type=\"radio\" onclick=\"window.location='/motor/2';\" name=\"motor\" value=\"2\" > 2            <input type=\"radio\" onclick=\"window.location='/motor/4';\" name=\"motor\" value=\"4\" checked> 4"); 
                  else if (motorState != 2 || motorState != 4)
                   client.println("     <input type=\"radio\" onclick=\"window.location='/motor/2';\" name=\"motor\" value=\"2\" > 2            <input type=\"radio\" onclick=\"window.location='/motor/4';\" name=\"motor\" value=\"4\" > 4"); 
                   
                   client.println("");
                  if(motorState == 1)
                    client.println(" <input type=\"radio\" onclick=\"window.location='/motor/1';\" name=\"motor\" value=\"1\" checked> 1     <a href=\"/motor/off\"><button class=\"button button4\">motor-OFF</button></a>    <input type=\"radio\" onclick=\"window.location='/motor/5';\" name=\"motor\" value=\"5\"> 5 </pre></div>");
                  else if(motorState == 5)
                    client.println(" <input type=\"radio\" onclick=\"window.location='/motor/1';\" name=\"motor\" value=\"1\" > 1     <a href=\"/motor/off\"><button class=\"button button4\">motor-OFF</button></a>    <input type=\"radio\" onclick=\"window.location='/motor/5';\" name=\"motor\" value=\"5\" checked> 5 </pre></div>");
                  else if (motorState != 5 || motorState != 1)
                    client.println(" <input type=\"radio\" onclick=\"window.location='/motor/1';\" name=\"motor\" value=\"1\" > 1     <a href=\"/motor/off\"><button class=\"button button4\">motor-OFF</button></a>    <input type=\"radio\" onclick=\"window.location='/motor/5';\" name=\"motor\" value=\"5\" > 5 </pre></div>");
                  
                 } 


//
//                client.println("<pre>");
//                client.println("");
//                client.println("</pre>");


                if(number == 1 && lockState ==0)
                client.println("<pre><p>Person in              <a href=\"/lock/on\"><button class=\"button button5\">Door lock</button></a>");
                else if(number%10 == 0 && lockState ==0)
                client.println("<pre><p>Person out              <a href=\"/lock/on\"><button class=\"button button5\">Door lock</button></a>");
                else if(number%10 == 0 && lockState ==1)
                client.println("<pre><p>Person out              <a href=\"/lock/off\"><button class=\"button button5\">Door open</button></a>");














            
            client.println("</pre></body></html>");
             }
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
   // Serial.println("Client disconnected.");
    //Serial.println("");
  }
}
