#!/usr/bin/env node

const request = require('request');
const async = require('async');
const { COMMANDS } = require('../scripts/lib/i2c_commands');


// main

if (process.argv.length <= 2) {
    let commands = COMMANDS.map(command => command.toLowerCase())

    console.log("usage: send_commands.js <command>+");
    console.log("Existing commands:");
    console.log("  " + commands.join("\n  "));

    process.exit(0);
}
else {
    const commands = process.argv.slice(2);

    async.mapSeries(
        commands,
        (command, next) => {
            request.get(
                {
                    url: `http://localhost:8081/${command.toLowerCase()}`
                },
                next
            )
        },
        (err, results) => {
            if (err) {
                console.error("An error occurred: " + err);
                process.exit(1);
            }

            const replies = results.forEach(result => {
                const data = JSON.parse(result.body);

                console.log(JSON.stringify(data, null, 2));
            });
        }
    );
}
