#pragma once

enum PinStatus {
  UN_INITIALIZED,
  UNARMED,
  ARMED,
  TRIGGERED
};

class LaunchPin {
  public:
    LaunchPin(int pin);
    void init();
    char getStatus();

  private:
    int pin;
    int status;
    int cache[10] = { 0 };
    int writer = 0;

};