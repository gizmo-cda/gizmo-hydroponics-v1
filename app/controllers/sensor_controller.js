function get_sensor_data(req, res) {
    res.render("sensors", { data: {
        ph: 0,
        temperature: 1,
        ec: 2
    }});
}


module.exports = function(app) {
    return {
        get_sensor_data: get_sensor_data
    }
};
