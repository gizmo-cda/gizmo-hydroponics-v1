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
    MOTOR_ON: {
        opcode: 3,
    },
    MOTOR_OFF: {
        opcode: 4,
    },
    READ_LIGHT: {
        opcode: 5,
        result: {
            value: "byte"
        }
    },
    READ_MOTOR: {
        opcode: 6,
        result: {
            value: "byte"
        }
    },
    READ_PH: {
        opcode: 7,
        result: {
            value: "float"
        }
    },
    READ_TEMPERATURE: {
        opcode: 8,
        result: {
            value: "float"
        }
    },
    READ_EC: {
        opcode: 9,
        result: {
            value: "float"
        }
    },
    READ_ALL: {
        opcode: 10,
        result: {
            light: "byte",
            motor: "byte",
            pH: "float",
            temperature: "float",
            ec: "float"
        }
    },
    READ_ALL_EXTENDED: {
        opcode: 11,
        result: {
            relays: {
                light: "byte",
                motor: "byte"
            },
            ph: {
                voltage: "float",
                pH: "float"
            },
            ec: {
                temperature: "float",
                ec: "float",
                ec25: "float",
                ppm: "float"
            }
        }
    }
};


function get_buffer_size(return_info) {
    if (return_info === undefined || return_info === null) {
        return null;
    }

    let size = 0;

    for (var p in return_info) {
        const type = return_info[p];

        if (typeof type === "object") {
            size += get_buffer_size(type);
        }
        else {
            switch (type) {
                case "byte":
                    size += 1;
                    break;

                case "word":
                    size += 2;
                    break;

                case "float":
                    size += 4;
                    break;

                default:
                    console.error(`unrecognized data type '${type}'`);
            }
        }
    }

    return size;
}

function get_return_buffer(return_info) {
    if (return_info === undefined || return_info === null) {
        return null;
    }

    let size = get_buffer_size(return_info);
    // console.log(size);

    return {
        buffer: Buffer.alloc(size),
        offset: 0,
        get_byte: function() { return this.buffer[this.offset++] },
        get_word: function() { return this.buffer[this.offset++] + (this.buffer[this.offset++] << 8) },
        get_float: function() {
            let byteArray = new Int8Array(this.buffer.slice(this.offset, this.offset + 4));
            let floatArray = new Float32Array(byteArray.buffer);
            
            this.offset += 4;
            
            return floatArray[0];
        }
    };
}

function parse_result(return_info, buffer) {
    if (return_info === undefined || return_info === null) {
        return null;
    }

    let result = {};

    for (var p in return_info) {
        const type = return_info[p];

        if (typeof type === "object") {
            result[p] = parse_result(type, buffer);
        }
        else {
            switch (type) {
                case "byte":
                    result[p] = buffer.get_byte();
                    break;

                case "word":
                    result[p] = buffer.get_word();
                    break;

                case "float":
                    result[p] = buffer.get_float();
                    break;

                default:
                    console.error(`unrecognized data type '${type}'`);
            }
        }
    }

    return result;
}

function send_command(bus, command_name) {
    var result;

    if (command_name in COMMANDS) {
        let command = COMMANDS[command_name];
        let output_buffer = new Buffer.from([command.opcode]);
        let result_buffer = get_return_buffer(command.result);

        bus.i2cWriteSync(I2C_ADDRESS, output_buffer.length, output_buffer);

        if (result_buffer !== null && result_buffer !== undefined) {
            bus.i2cReadSync(I2C_ADDRESS, result_buffer.buffer.length, result_buffer.buffer);

            result = parse_result(command.result, result_buffer);
        }
    }
    else {
        console.error(`unrecognized command '${command_name}'`);
    }

    return result;
}


module.exports = {
    I2C_ADDRESS: I2C_ADDRESS,
    COMMANDS: Object.keys(COMMANDS),
    send_command: send_command
};
