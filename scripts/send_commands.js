#!/usr/bin/env node

const i2c = require('i2c-bus');
const { COMMANDS, send_command } = require('../scripts/lib/i2c_commands');


function sleep(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
}


// main

const bus = i2c.openSync(1);

async function main() {
    for (let i = 2; i < process.argv.length; i++) {
        let command = process.argv[i].toUpperCase();

        console.log(`sending '${command}'`);
        let result = send_command(bus, command);

        // show result
        if (result !== null && result !== undefined) {
            console.log(JSON.stringify(result, null, 2));
        }

        // give i2c some time before sending another command
        await sleep(500);
    }
}

if (process.argv.length <= 2) {
    let commands = COMMANDS.map(command => command.toLowerCase())

    console.log("usage: send_commands.js <command>+");
    console.log("Existing commands:");
    console.log("  " + commands.join("\n  "));

    process.exit(0);
}

main();
