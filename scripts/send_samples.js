#!/usr/bin/env node

const fs = require('fs');
const path = require('path');
const request = require('request');
const i2c = require('i2c-bus');
const { COMMANDS, send_command } = require('../scripts/lib/i2c_commands');


const db_dir = path.join(__dirname, "..", "app", "db");
const samples_file = path.join(db_dir, "samples.json");
const last_sample_file = path.join(db_dir, "last_sample.json");
const id_file = path.join(db_dir, "id.json");
const id = require(id_file);


function sleep(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
}


// main

const bus = i2c.openSync(1);

// read sensors
const result = send_command(bus, "READ_ALL");

// transform data
const data = {
    datetime: Date.now(),
    temperature: result.temperature_value,
    ec: result.ec_value,
    pH: result.ph_value
};

// write to log file for backup
fs.appendFileSync(samples_file, JSON.stringify(data));
fs.writeFileSync(last_sample_file, JSON.stringify(data));

// send data to web server
const config = {
    url: `https://hydro.gizmo-cda.org/school/${id}/sample`,
    json: data
};

request.post(
    config,
    (err, response, body) => console.log(body)
);
