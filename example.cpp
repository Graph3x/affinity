#include "lillygo_sim800l.h"
#include "BMP280.h"
#include "ADXL345.h"
#include "gps.h"
#include "millis_timer.h"
#include "mosfetPyro.h"
#include "arduino_logger.h"
#include "trajectory_controller.h"
#include "launch_pin.h"

#include <cstring>
#include <cstdint>

#define LAUNCH_PIN 35
#define BUZZER_PIN 15
#define AURP_SIZE 65
#define MULTI_SIZE 195

#define GPS_RX 13
#define GPS_TX 14

#define BACK_DEPLOY_TIME 9000
#define SERVER_IP "ip"

LillyGoSIM800L comms = LillyGoSIM800L();
ADXL345 accel = ADXL345();
BMP280 bmp = BMP280();
GPS gps = GPS();
LaunchPin launchPin = LaunchPin(LAUNCH_PIN);

MillisTimer timer = MillisTimer();
TrajectoryController trajectoryController = TrajectoryController(bmp);

ArduinoLogger logger = ArduinoLogger(115200);
SimpleMosfetChannel pyro = SimpleMosfetChannel(2);

void buzz()
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(2000);
  digitalWrite(BUZZER_PIN, LOW);
}

void setup()
{
  delay(8000);
  Serial.begin(115200);
  
  Serial.println();

  bmp.calibrate();
  gps.init(GPS_RX, GPS_TX);
  Serial.println("sensors up");

  comms.powerOn();
  comms.connect();
  comms.connectUDP(SERVER_IP, "14959");
  
  Serial.println("comms up");

  pinMode(BUZZER_PIN, OUTPUT);
  buzz();
}

long lastTick = 0;
long lastStatus = 0;
uint8_t mpacket[MULTI_SIZE] = {0};
int mwriter = 0;

int status = TrajectoryStatus::UNKNOWN;
void loop()
{
  // READ DATA
  float temp = bmp.readTemperature();
  float height = bmp.readHeight();

  struct AccelData accelData = accel.getAccel();
  double accelx = accelData.x;
  double accely = accelData.y;
  double accelz = accelData.z;

  gps.parseData();
  double gpsAlt = gps.getAltitude();
  double gpsLon = gps.getLon();
  double gpsLat = gps.getLat();

  char pinStatus = launchPin.getStatus();
  Serial.println(digitalRead(LAUNCH_PIN));

  // update status
  if(millis() - lastStatus > 100){
    status = trajectoryController.tick();
  }

  // add new packets
  if (millis() - lastTick > 300)
  {
    lastTick = millis();
    Serial.println((int)pinStatus);

    uint8_t packet[AURP_SIZE] = {0};
    unsigned long timestamp = millis();
    unsigned int start = 0;
    std::memcpy(&packet, &timestamp, sizeof(unsigned long));
    start += sizeof(unsigned long);

    std::memcpy(&packet[start], &temp, sizeof(float));
    start += sizeof(float);

    std::memcpy(&packet[start], &height, sizeof(float));
    start += sizeof(float);

    std::memcpy(&packet[start], &accelx, sizeof(double));
    start += sizeof(double);

    std::memcpy(&packet[start], &accely, sizeof(double));
    start += sizeof(double);

    std::memcpy(&packet[start], &accelz, sizeof(double));
    start += sizeof(double);

    std::memcpy(&packet[start], &gpsAlt, sizeof(double));
    start += sizeof(double);

    std::memcpy(&packet[start], &gpsLon, sizeof(double));
    start += sizeof(double);

    std::memcpy(&packet[start], &gpsLat, sizeof(double));
    start += sizeof(double);

    std::memcpy(&packet[start], &pinStatus, sizeof(char));
    start += sizeof(char);

    std::memcpy(&packet[start], &status, sizeof(int));
    start += sizeof(int);

    std::memcpy(&mpacket[AURP_SIZE * (mwriter % 3)], &packet, AURP_SIZE);
    mwriter++;
  }

  if (timer.timeSinceLaunch() == 0 && pinStatus == TRIGGERED)
  {
    timer.launch();
    pyro.unlock();
  }

  // primary pyro
  if (status == DESCENDING && pyro.getStatus() == READY)
  {
    //pyro.blow(); experimental feature disabled for the first flight
    //timer.blowPyro();
    digitalWrite(BUZZER_PIN, HIGH);
  }

  // backup pyro
  if (timer.timeSinceLaunch() > BACK_DEPLOY_TIME && pyro.getStatus() == READY)
  {
    pyro.blow();
    timer.blowPyro();
    digitalWrite(BUZZER_PIN, HIGH);
  }

  if (pyro.getStatus() == BLOWN && timer.timeSincePyro() > 5000)
  {
    pyro.off();
  }

  // little stupid but working async UDP
  if (comms.getStatus() == CONNECTED)
  {
    comms.prepUDP(MULTI_SIZE);
  }

  if (comms.getStatus() == UDP_READY)
  {
    comms.sendUDP(mpacket);
  }
}