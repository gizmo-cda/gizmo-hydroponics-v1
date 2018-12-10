#ifndef EC_h
#define EC_h

#include "Arduino.h"
#include <DallasTemperature.h>
#include <OneWire.h>


class EC
{
  public:
    EC(uint8_t one_wire_bus);

    // pins
    int temp_probe_positive = 8;
    int temp_probe_negative = 9;
    int ec_pin              = A0;
    int ec_ground           = A1;
    int ec_power            = A3;
    
    // coefficients and other constants
    float k                 = 9.76;
    float temp_coefficient  = 0.019;
    float ppm_conversion    = 0.7;
    int r1                  = 1000;
    int ra                  = 25;    // resistance of powering pins
    float v_in              = 5;
    int sampling_interval   = 5000;

    void begin();
    void update_readings();

    float get_ec();
    float get_ec25();
    float get_ppm();
    float get_temperature();

  private:
    OneWire *_oneWire;
    DallasTemperature *_sensors;

    unsigned long _last_sample_time = 0;
    float _temperature;
    float _ec;
    float _ec25;
    float _ppm;
};

#endif
