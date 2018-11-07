#!/usr/bin/env node

const i2c = require('i2c-bus');

const bus = i2c.openSync(1);

const I2C_ADDRESS = 4;
const NO_OP = 0;
const LIGHT_ON = 1;
const LIGHT_OFF = 2;
const READ_LIGHT = 3;
const READ_PH = 4;
const READ_TEMPERATURE = 5;
const READ_EC = 6;
const READ_ALL = 7;

buf = new Buffer.from([READ_LIGHT]);
bus.i2cWriteSync(I2C_ADDRESS, buf.length, buf);
result = bus.i2cReadSync(I2C_ADDRESS, 1, buf);
console.log(`result = ${result}`);

