const fs = require('fs');
const path = require('path');
const last_sample_file = path.join(__dirname, "..", "db", "last_sample.json");


function round(number, decimal_digits) {
    const factor = Math.pow(10, decimal_digits);

    return Math.round(number * factor) / factor;
}

function read_last_sample() {
    let result = null;

    if (fs.existsSync(last_sample_file)) {
        const text = fs.readFileSync(last_sample_file);
        console.log(`text = ${text}`);

        try {
            result = JSON.parse(text);
        }
        catch(e) {
        }
    }

    return result;
}

function get_sensor_data(req, res) {
    const precision = 2;
    const data = read_last_sample();

    if (data !== null) {
        res.render("sensors", {
            temperature: round(data.temperature, precision),
            ec: round(data.ec, precision),
            ph: round(data.pH, precision)
        });
    }
    else {
        res.render("sensors", {});
    }
}


module.exports = function(app) {
    return {
        get_sensor_data: get_sensor_data
    }
};
