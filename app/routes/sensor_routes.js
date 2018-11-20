module.exports = function(app) {
    const sensor = require('../controllers/sensor_controller')(app);

    app.route("/")
        .get(sensor.get_sensor_data);

    app.route("/schedule")
        .get(sensor.get_schedule)
        .post(sensor.save_schedule);
};
