const i2c = require('i2c-bus');
const { COMMANDS, send_command } = require('../scripts/lib/i2c_commands');


function sleep(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
}

function get_sensor_data(req, res) {
    let command = req.body.command.toUpperCase();

    console.log(`sending '${command}'`);
    let result = send_command(bus, command);

    res.json(result);
}


module.exports = function(app) {
    return {
        get_sensor_data: get_sensor_data
    }
};
