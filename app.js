const path    = require('path');
const express = require('express');
const app     = express();
const morgan  = require('morgan');
const mustache   = require('mustache-express');

// general configuration
const HTTP_PORT = 8080;
const NODE_ENV  = process.env.NODE_ENV || 'development';
const LOGGING   = NODE_ENV === 'production' ? 'combined' : 'dev';

// useful paths
const app_directory    = path.join(__dirname, "app");
const public_directory = path.join(app_directory, "public");
const views_directory  = path.join(app_directory, "views");

// setup logging
app.use(morgan(LOGGING));

// setup templating
app.engine('mustache', mustache());
app.set('view engine', 'mustache');
app.set('views', views_directory);

// setup static content
app.use(express.static(public_directory));

// setup app routes
require('./app/routes/sensor_routes.js')(app);

// setup HTTP
app.listen(HTTP_PORT);
console.log(`Hydroponics server started on port ${HTTP_PORT}`);
