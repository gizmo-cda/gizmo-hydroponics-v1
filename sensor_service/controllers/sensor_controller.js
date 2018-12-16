const i2c = require('i2c-bus');
const { COMMANDS, send_command } = require('../../scripts/lib/i2c_commands');

let bus;


function sleep(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
}

function get_sensor_data(req, res) {
    let command = req.params.command.toUpperCase();
    let result = send_command(bus, command);

    if (result !== null && result !== undefined) {
        res.json(result);
    }
    else {
        res.json({ "message": "OK" });
    }
}


module.exports = function(app) {
    bus = i2c.openSync(1);

    return {
        get_sensor_data: get_sensor_data
    }
};
