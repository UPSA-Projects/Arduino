#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

Servo myservo;  // create servo object to control a servo

void setup() {
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());


  // Definir pin como salida
  pinMode(6 , OUTPUT);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

}


void loop() {
  Serial.println("Comenanzo Prueba LED");
  digitalWrite(6 , HIGH);   // poner el Pin en HIGH
  delay(1000);                   // esperar un segundo
  digitalWrite(6 , LOW);    // poner el Pin en LOW
  delay(1000);                   // esperar un segundo

  //listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    String cadena = ""; // Declarar Cadena para Capturar URL

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);


        // Capture Data
        if (cadena.length()<50)
        {
          cadena.concat(c);

           // Buscar campo data
          int posicion = cadena.indexOf("data");
          String command = cadena.substring(posicion);

          if (command == "data=0")
          {
            // Cerrar
            myservo.write(1);
            delay(1000);
            Serial.println("Cerrar Puerta...");
          }
          if (command == "data=1")
          {
            myservo.write(150);
            delay(1000);
            Serial.println("Abriendo Puerta...");
          }
        }


        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 60");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<main id=\"main\" class=\"main-content\">"); //add some titles and contect classifications
          /*build the table****************************/
          client.println("<TABLE BORDER=\"5\" WIDTH=\"50%\" CELLPADDING=\"4\" CELLSPACING=\"3\">");
          client.println("<TR>");
          client.println("<TH COLSPAN=\"2\"><BR><H3>SENSOR DATA</H3>");
          client.println("</TH>");
          client.println("</TR>");
          client.println("<TR>");
          client.println("<TH>CHANNEL</TH>");
          client.println("<TH>VALUE</TH>");
          client.println("</TR>");
          /*******************************************/
          // output the value of each analog input pin to the table
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel); //read analogs
            /*Add the channel to the table with the class identifier*/
            client.println("<TR ALIGN=\"CENTER\">");
            client.print("<TD class=\"chan\">");
            client.print(analogChannel);
            client.println("</TD>");
            /*******************************************************/
            /*Add the coorosponding value to the table*************/
            client.print("<TD class=\"value\">");
            client.print(sensorReading);
            client.println("</TD>");
            client.println("</TR>");
            /******************************************************/
          }
          /*End the HTML****************/
          client.println("</TABLE>");
          client.println("</main>");
          client.println("</html>");
          /***************************/
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}