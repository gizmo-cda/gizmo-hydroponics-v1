#ifndef PH_h
#define PH_h

#include "Arduino.h"

#define LENGTH 40

class PH
{
  public:
    PH();

    // pins
    int sensor_pin = A7;
    int sampling_interval = 20;
    float voltage_4 = 0.914;
    float voltage_7 = 1.974;
//    float voltage_10 = 3.075;

    void update_readings();
    float get_voltage();
    float get_pH();

  private:
    unsigned long _last_sample_time = 0;
    int _readings[LENGTH];
    int _reading_index = 0;
    float _voltage = 0.0;
    float _pH = 0.0;

    float readings_average();
};

#endif
