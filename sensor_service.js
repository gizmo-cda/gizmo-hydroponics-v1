const path       = require('path');
const express    = require('express');
const app        = express();
const morgan     = require('morgan');
const bodyParser = require('body-parser');

// general configuration
const HTTP_PORT = 8081;
const NODE_ENV  = process.env.NODE_ENV || 'development';
const LOGGING   = NODE_ENV === 'production' ? 'combined' : 'dev';

// useful paths
const app_directory = path.join(__dirname, "sensor_service");

// setup logging
app.use(morgan(LOGGING));

app.use((req, res, next) => {
    res.setHeader('Access-Control-Allow-Origin', '*')
    next();
});

// setup body parsing for forms and such
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// setup app routes
require('./sensor_service/routes/sensor_routes.js')(app);

// setup HTTP
app.listen(HTTP_PORT);
console.log(`Sensor service started on port ${HTTP_PORT}`);
