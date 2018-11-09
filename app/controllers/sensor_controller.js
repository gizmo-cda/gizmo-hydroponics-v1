function round(number, decimal_digits) {
    const factor = Math.pow(10, decimal_digits);

    return Math.round(number * factor) / factor;
}

function get_sensor_data(req, res) {
    const precision = 2;
    const temperature = 27 + Math.random();
    const ec = 200 + Math.random();
    const ph = 10 + Math.random();

    res.render("sensors", {
        temperature: round(temperature, precision),
        ec: round(ec, precision),
        ph: round(ph, precision)
    });
}


module.exports = function(app) {
    return {
        get_sensor_data: get_sensor_data
    }
};
