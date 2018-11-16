#include <Wire.h>
#include "EC.h"
#include "PH.h"

// custom data types
typedef union float_bytes {
  byte byte_values[4];
  float float_value;
} FLOAT_BYTES;

// i2c configuration
#define I2C_ADDRESS      4

// i2c commands
#define NO_OP             0
#define LIGHT_ON          1
#define LIGHT_OFF         2
#define MOTOR_ON          3
#define MOTOR_OFF         4
#define READ_LIGHT        5
#define READ_MOTOR        6
#define READ_PH           7
#define READ_TEMPERATURE  8
#define READ_EC           9
#define READ_ALL          10
#define READ_ALL_EXTENDED 11

// useful constants
#define ON  1
#define OFF 0
#define PIN_OFF HIGH
#define PIN_ON LOW

// globals
byte light_state = OFF;
byte motor_state = OFF;
byte buf[26];
int buf_len = 0;

#define LIGHT_PIN 4
#define MOTOR_PIN 5

// EC/OneWire configuration
#define ONE_WIRE_BUS 10

EC ec = EC(ONE_WIRE_BUS);
PH ph = PH();

void setup() {
  // start serial for output
  Serial.begin(9600);
  
  // turn on LED
  pinMode(13, OUTPUT);

  // initialize light and motor control pins
  Serial.println("Initializing light and motor...");
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, PIN_OFF);
  digitalWrite(MOTOR_PIN, PIN_OFF);

  // initialize i2c as slave and setup callbacks
  Serial.println("Initializing I2C...");
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  // initialize ec
  Serial.println("Initializing EC (OneWire/DallasTemperature)...");
  ec.begin();

  // we're ready to go
  Serial.println("Ready!");
}

void loop() {
  ec.update_readings();
  ph.update_readings();
}

void add_byte(byte value) {
  if (buf_len + 1 <= sizeof(buf)) {
    buf[buf_len++] = value & 0xFF;
  }
  else {
    Serial.println("byte value not added as it will cause a buffer overflow");
  }
}

void add_int(int value) {
  if (buf_len + 2 <= sizeof(buf)) {
    buf[buf_len++] = value & 0xFF;
    buf[buf_len++] = (value >> 8) & 0xFF;
  }
  else {
    Serial.println("integer value not added as it will cause a buffer overflow");
  }
}

void add_float(float value) {
  FLOAT_BYTES float_buffer;

  if (buf_len + 4 <= sizeof(buf)) {
    float_buffer.float_value = value;

    for (int i = 0; i < 4; i++) {
      buf[buf_len++] = float_buffer.byte_values[i];
    }
  }
  else {
    Serial.println("float value not added as it will cause a buffer overflow");
  }
}

void receiveData(int byteCount) {
  int command;

  while (Wire.available()) {
    command = Wire.read();

    // reset buffer length
    buf_len = 0;

    switch (command) {
      case NO_OP:
        Serial.println("no-op");
        break;
      case LIGHT_ON:
        Serial.println("light on");
        light_state = ON;
        digitalWrite(LIGHT_PIN, PIN_ON);
        break;
      case LIGHT_OFF:
        Serial.println("light off");
        light_state = OFF;
        digitalWrite(LIGHT_PIN, PIN_OFF);
        break;
      case MOTOR_ON:
        Serial.println("motor on");
        motor_state = ON;
        digitalWrite(MOTOR_PIN, PIN_ON);
        break;
      case MOTOR_OFF:
        Serial.println("motor off");
        motor_state = OFF;
        digitalWrite(MOTOR_PIN, PIN_OFF);
        break;
      case READ_LIGHT:
        Serial.println("read light");
        add_byte(light_state);
        break;
      case READ_MOTOR:
        Serial.println("read motor");
        add_byte(motor_state);
        break;
      case READ_PH:
        Serial.println("read pH");
        add_float(ph.get_pH());
        break;
      case READ_TEMPERATURE:
        Serial.println("read temperature");
        add_float(ec.get_temperature());
        break;
      case READ_EC:
        Serial.println("read ec");
        add_float(ec.get_ec25());
        break;
      case READ_ALL:
        Serial.println("read all");
        // relay info
        add_byte(light_state);
        add_byte(motor_state);
        // pH info
        add_float(ph.get_pH());
        // ec info
        add_float(ec.get_temperature());
        add_float(ec.get_ec25());
        break;
      case READ_ALL_EXTENDED:
        Serial.println("read all, extended");
        // relay info
        add_byte(light_state);
        add_byte(motor_state);
        // pH info
        add_float(ph.get_voltage());
        add_float(ph.get_pH());
        // ec info
        add_float(ec.get_temperature());
        add_float(ec.get_ec());
        add_float(ec.get_ec25());
        add_float(ec.get_ppm());
        break;
      default:
        Serial.print("ignoring unrecognized command: ");
        Serial.println(command);
    }
  }
}

void sendData() {
  if (buf_len > 0) {
    Serial.print("  sending ");
    Serial.print(buf_len);
    Serial.println(" bytes");

    // send data
    Wire.write(buf, buf_len);

    // reset buffer length so we send this data only once
    buf_len = 0;
  }
  else {
    Serial.println("  no data to send");
  }
}

