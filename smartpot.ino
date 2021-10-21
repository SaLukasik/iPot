/*iPot kod
  autor: Sara Łukasik
  data: 04.07.2021

  Zostawiam połowicznie zaimplemetowaną funkcję przesyłania danych na stronę www. U mnie niestety nie wypaliła,
  bo powodowała zawieszenie działania przycisków i nawet po usunięciu delay blokowanie programu,
  ale myślę, że będzie ok dla projektu czysto nastawionego na przesyłanie danych, bo przy testowaniu działało spoko. 
  Może kommuś się przyda i oszczędzi godzin googlania jak używać WiFiNINA.
*/

#include <SPI.h>
#include <DHT.h>
/*#include <WiFiNINA.h>*/
#include <Servo.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

/*mierzne wartości const*/
float TEMPERATURE = 0;
float HUMIDITY_AIR = 0;
float HUMIDITY_EARTH = 0;

/*dioddy i switch const*/
const int BLUE_LED_A0 = A0;
const int SWITCH_A6 = A6;
const int SWITCH_A7 = A7;
bool A6STATUS = true;
bool A7STATUS = true;
bool pointer = true;
bool manual = true;
bool blinks = false;
bool blinkon = false;

/*display const*/
const int OLED_ADDRESS = 0x3C;
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 gDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/*DHT const*/
#define DHTPIN A1
#define DHTTYPE DHT11
DHT dht = DHT(DHTPIN, DHTTYPE);

/*earth humidiity const*/
#define EHPIN A3
#define EHLED A2
/*podlewanie const*/
int hours_period = 0;
int hours_to_water = 0;
int minutes_to_water = 0;

/*prymitywny timer*/
unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long time3 = 0;
unsigned long timeservo = 0;
unsigned long timeset = 0;
signed long clocker = 101; //gwarantuje brak błędów w pierwszym obrocie pętli

/*Silnik const*/
Servo myservo;
bool servogo = false;
bool servogoback = false;


/*wifi const*/
/*bool wifion = false;
  char WIFI_SSID[] = "JP2";
  char WIFI_PASSWORD[] = "2137";
  int WifiStatus = WL_IDLE_STATUS;

  IPAddress ServerIp(192, 168, 43, 420);
  const int SERVER_PORT = 69;

  WiFiServer server(SERVER_PORT);
  WiFiClient serverClient;

*/

void setup() {
  Serial.begin(9600);
  pinMode(BLUE_LED_A0, OUTPUT);
  digitalWrite(BLUE_LED_A0, HIGH);

  gDisplay.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  gDisplay.clearDisplay();
  gDisplay.setTextSize(1);
  gDisplay.setTextColor(WHITE, BLACK);
  gDisplay.clearDisplay();
  gDisplay.setCursor(1, 30);
  gDisplay.println("Air temp:");
  gDisplay.setCursor(1, 40);
  gDisplay.println("Air hum:");
  gDisplay.setCursor(1, 50);
  gDisplay.println("Earth hum:");
  gDisplay.setCursor(1, 0);
  gDisplay.println("Water period");
  gDisplay.setCursor(85, 0);
  gDisplay.println(hours_period);
  gDisplay.setCursor(120, 0);
  gDisplay.println("h" );


  gDisplay.setCursor(1, 9);
  gDisplay.println("Next water" );
  gDisplay.setCursor(85, 9);
  gDisplay.println(hours_to_water);
  gDisplay.setCursor(107, 9);
  gDisplay.println(":" );
  gDisplay.setCursor(115, 9);
  gDisplay.println(minutes_to_water);
  gDisplay.setCursor(31, 16);
  gDisplay.println("MODE:");
  gDisplay.setCursor(65, 16);
  gDisplay.println("MANUAL");
  gDisplay.display();

  /*temp sens*/
  dht.begin();

  /*earth humidiity setupt*/
  pinMode(EHPIN, INPUT);
  pinMode(EHLED, OUTPUT);
  pinMode(SWITCH_A6, INPUT);
  pinMode(SWITCH_A7, INPUT);

  /*silnik setup*/
  myservo.attach(9);

  /*wifi setup*/
  /*


    WiFi.config(ServerIp);
    WifiStatus = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // wait 10 seconds for connection:
    delay(10000);


    if (WifiStatus == WL_CONNECTED){
    wifion = true;
    digitalWrite(BLUE_LED_A0, HIGH);

    }
    else{
    wifion = false;
    digitalWrite(BLUE_LED_A0, LOW);
    }

    if (wifion == true){
     IPAddress adresip = WiFi.localIP();
    server.begin();
    }


  */
}

void loop() {
  /*
    if (WifiStatus == WL_CONNECTED){
      wifion = true;
      digitalWrite(BLUE_LED_A0, HIGH);

    }
    else{
      wifion = false;
      digitalWrite(BLUE_LED_A0, LOW);
    }

      if (wifion == true){
                serverClient = server.available();
                  if (serverClient) {

                    boolean puste = true;
                    while (serverClient.connected()) {
                      if (serverClient.available()) {
                        char c = serverClient.read();
                        if (c == '\n' && puste) {

                          serverClient.println("HTTP/1.1 200 OK");
                          serverClient.println("Content-Type: text/html");
                          serverClient.println("Connection: close");  // the connection will be closed after completion of the response
                          serverClient.println("Refresh: 5");  // refresh the page automatically every 5 sec
                          serverClient.println();
                          serverClient.println("<!DOCTYPE HTML>");
                          serverClient.println("<html>");
                          // output the value of each analog input pin
                            serverClient.print("analog input ");
                            serverClient.print(" is ");
                            serverClient.print(TEMPERATURE);
                            serverClient.println("");
                          serverClient.println("</html>");

                        }
                        if (c == '\n') {
                          // you're starting a new line
                          puste = true;
                        } else if (c != '\r') {
                          // you've gotten a character on the current line
                          puste = false;
                        }
                      }
                      break;

                    }
                    // give the web browser time to receive the data
                    delay(1);

                    // close the connection:
                    serverClient.stop();

                    Serial.println("client disonnected");
                  }
      }
  */


  if ((digitalRead(SWITCH_A6) == HIGH ) && (digitalRead(SWITCH_A7) == HIGH) && (A6STATUS == true)) {
    A6STATUS = false;
    A7STATUS = false;

    if (manual == true) {
      manual = false;
      gDisplay.setCursor(65, 16);
      gDisplay.println("AUTO    ");
    }
    else {
      manual = true;
      gDisplay.setCursor(65, 16);
      gDisplay.println("MANUAL");
    }
  }
  else if ((digitalRead(SWITCH_A6) == HIGH ) && (A6STATUS == true)) {
    A6STATUS = false;
    if (hours_period < 750) {
      hours_period += 1;
      blinkon = false;
      /* if (wifion){
         wifion = true;
         digitalWrite(BLUE_LED_A0, HIGH);

        }
        else{
         wifion = false;
         digitalWrite(BLUE_LED_A0, LOW);
        }
      */
    }
    gDisplay.setCursor(85, 0);
    gDisplay.println("    ");
    gDisplay.setCursor(85, 0);
    gDisplay.println(hours_period);
    digitalWrite(BLUE_LED_A0, HIGH);
    timeset = millis();
  }
  else if ((digitalRead(SWITCH_A7) == HIGH ) && (A7STATUS == true)) {
    A7STATUS = false;
    if (hours_period > 0) {
      hours_period -= 1;
      blinkon = false;
      /* if (wifion){
         wifion = true;
         digitalWrite(BLUE_LED_A0, HIGH);

        }
        else{
         wifion = false;
         digitalWrite(BLUE_LED_A0, LOW);
        }*/

    }
    timeset = millis();

    gDisplay.setCursor(85, 0);
    gDisplay.println("    ");
    gDisplay.setCursor(85, 0);
    gDisplay.println(hours_period);
    digitalWrite(BLUE_LED_A0, HIGH);

  }
  if ((digitalRead(SWITCH_A6) == LOW ) && (A6STATUS == false)) {
    A6STATUS = true;

  }
  if ((digitalRead(SWITCH_A7) == LOW ) && (A7STATUS == false)) {
    A7STATUS = true;

  }

  if ((blinkon == true) && (millis() - time2) > 500) {
    if (blinks == true) {
      blinks = false;
      digitalWrite(BLUE_LED_A0, LOW);

    }
    else {
      blinks = true;
      digitalWrite(BLUE_LED_A0, HIGH);
    }
    time2 = millis();
  }

  if ((millis() - time3) > 100) {
    if (hours_period != 0) {
      clocker = ( signed long)hours_period * 60 * 60 * 1000 - (( signed long)millis() - ( signed long)timeset);

      /*timeset -= 60 * 100; <- kracanie czasu do prezentacji*/ 
      if (clocker < 101) {

        if (manual == true) {
          hours_period = 0;
          gDisplay.setCursor(85, 0);
          gDisplay.println("    ");
          gDisplay.setCursor(85, 0);
          gDisplay.println(hours_period);
          blinkon = true;
        }
        else {
          timeset = millis();
          servogo = true;
          //miesjce na mechanizm podlewania
        }

      }


    }
    time3 = millis();

  }

  if (servogo == true) {
    servogo = false;
    digitalWrite(BLUE_LED_A0, LOW);
    myservo.write(0);

    timeservo = millis();
    servogoback = true;
  }

  if ((servogoback == true) && ((millis() - timeservo) > 2800)) {
    servogoback = false;
    digitalWrite(BLUE_LED_A0, HIGH);
    myservo.write(180);
  }



  if ((millis() - time1) > 3000) {

    if (hours_period != 0) {


      hours_to_water = (int)clocker / 60 / 60 / 1000;
      minutes_to_water = (int)clocker / 60 / 1000 % 60;
      gDisplay.setCursor(85, 9);
      gDisplay.println("            " );
      gDisplay.setCursor(85, 9);
      gDisplay.println(hours_to_water);
      gDisplay.setCursor(107, 9);
      gDisplay.println(":    " );
      gDisplay.setCursor(115, 9);
      gDisplay.println(minutes_to_water);

    }
    else {
      gDisplay.setCursor(85, 9);
      gDisplay.println(0);
      gDisplay.setCursor(107, 9);
      gDisplay.println(":    " );
      gDisplay.setCursor(115, 9);
      gDisplay.println("0  ");
    }


    TEMPERATURE = dht.readTemperature() - 5; //blad czujnika, pokauje o 5 C  za duzo
    gDisplay.setCursor(60, 30);
    gDisplay.println(TEMPERATURE);
    gDisplay.setCursor(100, 30);
    gDisplay.println("C");


    HUMIDITY_AIR = dht.readHumidity();
    gDisplay.setCursor(55, 40);
    gDisplay.println(HUMIDITY_AIR);
    gDisplay.setCursor(95, 40);
    gDisplay.println("%");
    gDisplay.display();

    HUMIDITY_EARTH = analogRead(EHPIN);
    gDisplay.setCursor(65, 50);
    gDisplay.println("        ");
    gDisplay.setCursor(65, 50);
    if (HUMIDITY_EARTH < 450) {
      gDisplay.println("HIGH!!!");
      digitalWrite(EHLED, HIGH);
    }
    else if ((HUMIDITY_EARTH >= 450) && (HUMIDITY_EARTH < 550) ) {
      gDisplay.println("HIGH");
      digitalWrite(EHLED, LOW);
    }
    else if ((HUMIDITY_EARTH >= 550) && (HUMIDITY_EARTH < 650) ) {
      gDisplay.println("MEDIUM L");
      digitalWrite(EHLED, LOW);
    }
    else if ((HUMIDITY_EARTH >= 650) && (HUMIDITY_EARTH < 750) ) {
      gDisplay.println("MEDIUM H");
      digitalWrite(EHLED, LOW);
    }
    else if ((HUMIDITY_EARTH >= 750) && (HUMIDITY_EARTH < 850) ) {
      gDisplay.println("LOW");
      digitalWrite(EHLED, LOW);
    }
    else if ((HUMIDITY_EARTH >= 850)) {
      gDisplay.println("LOW!!!");
      digitalWrite(EHLED, HIGH);

    }


    time1 = millis();
  }





}
