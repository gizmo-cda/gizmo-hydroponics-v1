#include "EC.h"


EC::EC(uint8_t one_wire_bus)
{
  // setup oneWire instance to communicate with any OneWire devices
  // and pass along oneWire reference to Dallas Temperature.
  _oneWire = new OneWire(one_wire_bus);
  _sensors = new DallasTemperature(_oneWire);
}

void EC::begin()
{
  // adding digital pin resistance, see README for an explanation
  r1 += ra;

  // set ground pin as output for temperature probe
  // and set to ground so it can sink current
  pinMode(temp_probe_negative, OUTPUT);
  digitalWrite(temp_probe_negative , LOW);

  // same but for positive
  pinMode(temp_probe_positive , OUTPUT);
  digitalWrite(temp_probe_positive , HIGH);

// set source and sinking pins and leave ground connected permanently
  pinMode(ec_pin, INPUT);
  pinMode(ec_power, OUTPUT);
  pinMode(ec_ground, OUTPUT);
  digitalWrite(ec_ground, LOW);

  // gives sensor time to settle
  delay(100);
  _sensors->begin();
  delay(100);
}

void EC::update_readings()
{
  float raw;
  float v_drop;
  float rc;

  // read temperature of solution
  _sensors->requestTemperatures();
  _temperature = _sensors->getTempCByIndex(0);

  // estimate resistance of liquid
  // NOTE: we need to read the pin twice because first reading will be low
  // due to residual charge
  digitalWrite(ec_power, HIGH);
  raw = analogRead(ec_pin);
  raw = analogRead(ec_pin);
  digitalWrite(ec_power, LOW);

  // calculate E 
  v_drop = (v_in * raw) / 1024.0;
  rc = (v_drop * r1) / (v_in - v_drop);
  // account for digital pin resitance
  rc -= ra;
  _ec = 1000.0 / (rc * k);

  // compensate for temperaure
  _ec25  =  _ec / (1.0 + temp_coefficient * (_temperature - 25.0));
  _ppm = _ec25 * (ppm_conversion * 1000);
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
