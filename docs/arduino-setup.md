- [External Libraries](#external-libraries)
- [i2c](#i2c)

---

# External Libaries

In order to use the electrical conductivity Arduino sketches, two libraries are
needed:

- OneWire, v2.3.4
- DallasTemperature, v3.8.0

These can be installed using the Library Manager in the Arduino application.

# i2c

The `i2c-slave` sketch turns an Arduino into an i2c device. Each i2c device
requires a unique address. For this project, we are using:

- 0x04
