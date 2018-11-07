#!/usr/bin/env node

const i2c = require('i2c-bus');


// constants and configuration
const I2C_ADDRESS = 4;
const COMMANDS = {
    NO_OP: {
        opcode: 0
    },
    LIGHT_ON: {
        opcode: 1
    },
    LIGHT_OFF: {
        opcode: 2
    },
    READ_LIGHT: {
        opcode: 3,
        result: {
            value: "byte"
        }
    },
    READ_PH: {
        opcode: 4,
        result: {
            value: "word"
        }
    },
    READ_TEMPERATURE: {
        opcode: 5,
        result: {
            value: "word"
        }
    },
    READ_EC: {
        opcode: 6,
        result: {
            value: "word"
        }
    },
    READ_ALL: {
        opcode: 7,
        result: {
            light_value: "byte",
            ph_value: "word",
            temperature_value: "word",
            ec_value: "word",
        }
    }
};


function get_return_buffer(return_info) {
    if (return_info === undefined || return_info === null) {
        return null;
    }
    else {
        let size = 0;

        for (var p in return_info) {
            const type = return_info[p];

            switch (type) {
                case "byte":
                    size += 1;
                    break;

                case "word":
                    size += 2;
                    break;

                default:
                    console.error(`unrecognized data type '${type}'`);
            }
        }

        return Buffer.alloc(size);
    }
}

function parse_result(return_info, buffer) {
    if (return_info === undefined || return_info === null) {
        return null;
    }
    else {
        let offset = 0;
        let result = {};

        for (var p in return_info) {
            const type = return_info[p];

            switch (type) {
                case "byte":
                    result[p] = buffer[offset];
                    offset += 1;
                    break;

                case "word":
                    result[p] = buffer[offset] + (buffer[offset + 1] << 8);
                    offset += 2;
                    break;

                default:
                    console.error(`unrecognized data type '${type}'`);
            }
        }

        return result;
    }
}

function send_command(command_name) {
    if (command_name in COMMANDS) {
        let command = COMMANDS[command_name];
        let output_buffer = new Buffer.from([command.opcode]);
        let result_buffer = get_return_buffer(command.result);

        bus.i2cWriteSync(I2C_ADDRESS, output_buffer.length, output_buffer);

        if (result_buffer !== null && result_buffer !== undefined) {
            bus.i2cReadSync(I2C_ADDRESS, result_buffer.length, result_buffer);

            let parsed_result = parse_result(command.result, result_buffer);

            console.log(command_name);
            console.log(JSON.stringify(parsed_result, null, 2));
            console.log();
        }
        else {
            console.log("no return value");
        }
    }
    else {
        console.error(`unrecognized command '${command}'`);
    }
}

function sleep(millis) {
    return new Promise(resolve => setTimeout(resolve, millis));
}

// main

async function main() {
    send_command("READ_LIGHT");
    await sleep(1000);
    send_command("READ_PH");
    await sleep(1000);
    send_command("READ_TEMPERATURE");
    await sleep(1000);
    send_command("READ_EC");
    await sleep(1000);
    send_command("READ_ALL");
}

const bus = i2c.openSync(1);
main();

