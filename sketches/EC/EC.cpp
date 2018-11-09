#include "EC.h"


EC::EC(DallasTemperature *sensors)
{
  // adding digital pin resistance, see README for an explanation
  _r1 += _ra;

  // set ground pin as output for temperature probe
  // and set to ground so it can sink current
  pinMode(_temp_probe_negative, OUTPUT);
  digitalWrite(_temp_probe_negative , LOW);

  // same but for positive
  pinMode(_temp_probe_positive , OUTPUT);
  digitalWrite(_temp_probe_positive , HIGH);

// set source and sinking pins and leave ground connected permanently
  pinMode(_ec_pin, INPUT);
  pinMode(_ec_power, OUTPUT);
  pinMode(_ec_ground, OUTPUT);
  digitalWrite(_ec_ground, LOW);
}

void EC::begin()
{
  // gives sensor time to settle
  delay(100);
  _sensors->begin();
  delay(100);
}

void EC::update_readings()
{
  float raw;
  float v_in = 5;
  float v_drop = 0;
  float rc = 0;
  float buffer = 0;

  // read temperature of solution
  _sensors->requestTemperatures();
  _temperature = _sensors->getTempCByIndex(0);

  // estimate resistance of liquid
  // NOTE: we need to read the pin twice because first reading will be low
  // due to residual charge
  digitalWrite(_ec_power, HIGH);
  raw = analogRead(_ec_pin);
  raw = analogRead(_ec_pin);
  digitalWrite(_ec_power, LOW);

  // calculate E 
  v_drop = (v_in * raw) / 1024.0;
  rc = (v_drop * _r1) / (v_in - v_drop);

  // account for digital pin resitance
  rc -= _ra;
  
  _ec = 1000.0 / (rc * _k);

  // compensate for temperaure
  _ec25  =  _ec / (1.0 + _temp_coefficient * (_temperature - 25.0));
  _ppm = _ec25 * (_ppm_conversion * 1000);
}

float EC::get_ec()
{
  return _ec;
}

float EC::get_ec25()
{
  return _ec25;
}

float EC::get_ppm()
{
  return _ppm;
}

float EC::get_temperature()
{
  return _temperature;
}
