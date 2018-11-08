#include <Wire.h>

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

// globals
byte buf[8];
int buf_len = 0;


void setup() {
  // turn on LED
  pinMode(13, OUTPUT);

  // start serial for output
  Serial.begin(9600);
  
  // initialize i2c as slave and setup callbacks
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

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

void receiveData(int byteCount) {
  int command;

  while (Wire.available()) {
    command = Wire.read();
    Serial.print("command received: ");
    Serial.println(command);

    // reset buffer length
    buf_len = 0;

    switch (command) {
      case NO_OP:
        Serial.println("no-op");
        break;
      case LIGHT_ON:
        Serial.println("light on");
        break;
      case LIGHT_OFF:
        Serial.println("light off");
        break;
      case MOTOR_ON:
        Serial.println("motor on");
        break;
      case MOTOR_OFF:
        Serial.println("motor off");
        break;
      case READ_LIGHT:
        Serial.println("read light");
        add_byte(1);
        break;
      case READ_MOTOR:
        Serial.println("read motor");
        add_byte(0);
        break;
      case READ_PH:
        Serial.println("read pH");
        add_int(256);
        break;
      case READ_TEMPERATURE:
        Serial.println("read temperature");
        add_int(257);
        break;
      case READ_EC:
        Serial.println("read ec");
        add_int(258);
        break;
      case READ_ALL:
        Serial.println("read all");
        add_byte(1);
        add_byte(0);
        add_int(256);
        add_int(257);
        add_int(258);
        break;
      default:
        Serial.println("ignoring unrecognized command");
    }
  }
}

void sendData() {
  if (buf_len > 0) {
    Serial.print("Sending ");
    Serial.print(buf_len);
    Serial.println(" bytes");

    // send data
    Wire.write(buf, buf_len);

    // reset buffer length so we send this data only once
    buf_len = 0;
  }
  else {
    Serial.println("no data to send");
  }
}

