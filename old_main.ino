// PINS
#define MODEM_RST            5
#define MODEM_PWRKEY         4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define MODEM_BAUD           115200

HardwareSerial modem(1);  // UART1

void setup() {
  Serial.begin(115200);
  delay(3000);

  // Power on the modem
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_POWER_ON, HIGH);
  
  pinMode(MODEM_RST, OUTPUT);
  digitalWrite(MODEM_RST, HIGH);
  
  pinMode(MODEM_PWRKEY, OUTPUT);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(100);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(3000);  // wait for module to boot

  modem.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  modem.println("AT");
  waitForResponse();

  delay(15000);

  modem.println("AT+CGATT=1");
  waitForResponse();
  modem.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  waitForResponse();
  modem.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  waitForResponse();
  modem.println("AT+SAPBR=1,1");
  waitForResponse();

  delay(3000);
  
  for(int i = 0; i < 10; i++){
    modem.println("AT+HTTPINIT");
    waitForResponse();
    modem.println("AT+HTTPPARA=\"CID\",1");
    waitForResponse();
    modem.println("AT+HTTPPARA=\"URL\",\"http://d1625vqz1wg000001r2ggoubn6yyyyyyb.oast.pro\"");
    waitForResponse();
    modem.println("AT+HTTPACTION=0");
    waitForResponse();  
    modem.println("AT+HTTPTERM");
    waitForResponse();
  }
  modem.println("AT+SAPBR=0,1");
  waitForResponse();
}


void loop() {
}

void waitForResponse() {
  long timeout = millis() + 5000;
  while (millis() < timeout) {
    while (modem.available()) {
      String line = modem.readStringUntil('\n');
      line.trim();
      if (line.length() > 0) {
        Serial.println(">> " + line);
      }
    }
  }
}


void setup() {
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);

  for(int i = 0; i < 10; i++){
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(1000);
  }

  for(int i = 0; i < 10; i++){
    digitalWrite(2, HIGH);
    delay(300);
    digitalWrite(2, LOW);
    delay(300);
  }
  
  digitalWrite(4, HIGH);
  
}

void loop() {

  /*digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  delay(3000);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  delay(8000);
*/
}
