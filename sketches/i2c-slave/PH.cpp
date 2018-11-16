#include "PH.h"

float map_range(float value, float in_min, float in_max, float out_min, float out_max) {
  float out_delta = out_max - out_min;
  float in_delta = in_max - in_min;

  return (value - in_min) * out_delta / in_delta + out_min;
}


PH::PH() {
}

void PH::update_readings() {
  unsigned long elapsed_time = millis() - _last_sample_time;

  if (elapsed_time > sampling_interval) {
    _readings[_reading_index++] = analogRead(sensor_pin);

    if (_reading_index >= LENGTH) {
      _reading_index = 0;
    }

    _voltage = readings_average() * 5.0 / 1024.0;
    _pH = map_range(_voltage, voltage_4, voltage_7, 4.0, 7.0);
//    _pH = map_range(_voltage, voltage_4, voltage_10, 4.0, 10.0);

    _last_sample_time = millis();
  }
}

float PH::readings_average() {
  int i;
  int max_value, min_value;
  long sum = 0;

  if (_readings[0] < _readings[1]) {
    min_value = _readings[0];
    max_value = _readings[1];
  }
  else {
    min_value = _readings[1];
    max_value = _readings[0];
  }

  for (i = 2; i < LENGTH; i++) {
    if (_readings[i] < min_value) {
      sum += min_value;
      min_value = _readings[i];
    }
    else if (_readings[i] > max_value) {
      sum += max_value;
      max_value = _readings[i];
    }
    else {
      sum += _readings[i];
    }
  }

  return (float) sum / (LENGTH - 2.0);
}

float PH::get_voltage() {
  return _voltage;
}

float PH::get_pH() {
  return _pH;
}
