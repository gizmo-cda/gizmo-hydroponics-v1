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
    float offset = 0.23;
    int sampling_interval = 20;

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
