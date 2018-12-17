const fs = require('fs');
const path = require('path');
const async = require('async');
const request = require('request');
const last_sample_file = path.join(__dirname, "..", "db", "last_sample.json");


function round(number, decimal_digits) {
    if (number === undefined) {
        return undefined;
    }

    const factor = Math.pow(10, decimal_digits);

    return Math.round(number * factor) / factor;
}

function read_last_sample() {
    let result = null;

    if (fs.existsSync(last_sample_file)) {
        const text = fs.readFileSync(last_sample_file, { encoding: 'utf8' });

        try {
            result = JSON.parse(text);
        }
        catch(e) {
            console.error("Faild to parse last sample");
            console.error(text);
        }
    }

    return result;
}

function send_command(command, callback) {
    request.get(
        {
            url: `http://localhost:8081/${command.toLowerCase()}`
        },
        (err, response, body) => {
            if (err) {
                callback(err);
                return;
            }
            else {
                callback(null, JSON.parse(body));
            }
        }
    )
}

function get_sensor_data(req, res) {
    const precision = 2;

    async.series(
        {
            light: next => send_command('read_light', next),
            motor: next => send_command('read_motor', next)
        },
        (err, results) => {
            if (err) {
                console.error('An error occurred getting sensor data: ' + err);
                res.render("sensors", {});
            }
            else {
                const data = read_last_sample() || {};

                res.render("sensors", {
                    temperature: round(data.temperature, precision),
                    ec: round(data.ec, precision),
                    ph: round(data.pH, precision),
                    light: results.light.value ? "true" : "false",
                    motor: results.motor.value ? "true" : "false"
                });
            }
        }
    );
}

function get_schedule(req, res) {
    res.render("schedule", {});
}

function save_schedule(req, res) {
    console.log(`body = ${JSON.stringify(req.body)}`);

    res.json({ message: "OK" });
}

module.exports = function(app) {
    return {
        get_sensor_data: get_sensor_data,
        get_schedule: get_schedule,
        save_schedule: save_schedule
    }
};
