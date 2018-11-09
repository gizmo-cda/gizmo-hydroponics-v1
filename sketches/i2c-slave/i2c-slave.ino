#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EC.h>

// i2c configuration
#define I2C_ADDRESS      4

// i2c commands
#define NO_OP            0
#define LIGHT_ON         1
#define LIGHT_OFF        2
#define MOTOR_ON         3
#define MOTOR_OFF        4
#define READ_LIGHT       5
#define READ_MOTOR       6
#define READ_PH          7
#define READ_TEMPERATURE 8
#define READ_EC          9
#define READ_ALL         10

// one wire configuration
#define ONE_WIRE_BUS 10

// useful constants
#define ON  1
#define OFF 0

// globals
byte light_state = OFF;
byte motor_state = OFF;

byte buf[14];
int buf_len = 0;

// setup oneWire instance to communicate with any OneWire devices
// and pass along oneWire reference to Dallas Temperature.
OneWire oneWire = OneWire(ONE_WIRE_BUS);
DallasTemperature sensors = DallasTemperature(&oneWire);
EC ec = EC(&sensors);

// custom data types
typedef union float_bytes {
  byte byte_values[4];
  float float_value;
} FLOAT_BYTES;

void setup() {
  // turn on LED
  pinMode(13, OUTPUT);

  // start serial for output
  Serial.begin(9600);
  
  // initialize i2c as slave and setup callbacks
  Serial.println("Initializing I2C...");
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  // initialize onewire
  Serial.println("Initializing OneWire...");
  ec.begin();

  // we're ready to go
  Serial.println("Ready!");
}

void loop() {
  delay(500);
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
        break;
      case LIGHT_OFF:
        Serial.println("light off");
        light_state = OFF;
        break;
      case MOTOR_ON:
        Serial.println("motor on");
        motor_state = ON;
        break;
      case MOTOR_OFF:
        Serial.println("motor off");
        motor_state = OFF;
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
        add_float(0.5);
        break;
      case READ_TEMPERATURE:
        Serial.println("read temperature");
//        ec.update_readings();
//        add_float(ec.get_temperature());
        add_float(0.6);
        break;
      case READ_EC:
        Serial.println("read ec");
//        ec.update_readings();
//        add_float(ec.get_ec());
        add_float(0.7);
        break;
      case READ_ALL:
        Serial.println("read all");
//        ec.update_readings();
        add_byte(light_state);
        add_byte(motor_state);
        add_float(0.5);
        add_float(0.6);
        add_float(0.7);
//        add_float(ec.get_temperature());
//        add_float(ec.get_ec());
        break;
      default:
        Serial.println("ignoring unrecognized command");
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

