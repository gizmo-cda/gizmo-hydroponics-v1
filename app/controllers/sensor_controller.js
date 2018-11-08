function get_sensor_data(req, res) {
    res.render("sensors", {
        ph: 10.01,
        temperature: 100.67,
        ec: 200.67
    });
}


module.exports = function(app) {
    return {
        get_sensor_data: get_sensor_data
    }
};
