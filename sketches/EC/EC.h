#ifndef EC_h
#define EC_h

#include "Arduino.h"
#include <DallasTemperature.h>

class EC
{
  public:
    EC(DallasTemperature *sensor);

    void begin();
    void update_readings();
    float get_ec();
    float get_ec25();
    float get_ppm();
    float get_temperature();

  private:
    DallasTemperature *_sensors;

    float _temperature;
    float _ec;
    float _ec25;
    float _ppm;

    // pins
    uint8_t _one_wire_bus    = 10;
    int _temp_probe_positive = 8;
    int _temp_probe_negative = 9;
    int _ec_pin              = A0;
    int _ec_ground           = A1;
    int _ec_power            = A3;
    
    // coefficients and other constants
    float _k                 = 5.81;
    float _temp_coefficient  = 0.019;
    float _ppm_conversion    = 0.7;
    int _r1                  = 1000;
    int _ra                  = 25;    // resistance of powering pins
};

#endif
