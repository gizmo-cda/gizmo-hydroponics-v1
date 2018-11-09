#!/usr/bin/env node

const i2c = require('i2c-bus');
const { send_command } = require('../scripts/lib/i2c_commands');


function sleep(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
}


// main

const bus = i2c.openSync(1);
const state = process.argv[2];

async function main() {
    console.log(`state = '${state}'`);

    let commands =
        state === "1"
            ? [ "LIGHT_ON", "MOTOR_ON", "READ_LIGHT", "READ_MOTOR" ]
            : [ "LIGHT_OFF", "MOTOR_OFF", "READ_LIGHT", "READ_MOTOR" ];

    for (var cmd of commands) {
        console.log(`sending '${cmd}'`);
        send_command(bus, cmd);
        await sleep(1000);
    }

    console.log("done");
}

main();
