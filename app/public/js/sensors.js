let temperature, ec, ph;

function go() {
    const toggles = document.querySelectorAll('[aria-pressed]');

    toggles.forEach(toggle => {
        toggle.addEventListener('click', e => {  
            let pressed = e.target.getAttribute('aria-pressed') === 'true';
            let new_state = !pressed;
            let new_value = String(new_state);
            let id = e.target.id;

            e.target.setAttribute('aria-pressed', new_value);

            switch (id) {
                case "light":
                    send_command(new_state ? "light_on" : "light_off");
                    break;

                case "pump":
                    send_command(new_state ? "motor_on" : "motor_off");
                    break;

                default:
                    console.error(`unknown button: ${id}`);
            }
        });
    });

    [temperature, ec, ph] = ["temperature", "ec", "ph"].map(id => document.getElementById(id))
}

function send_command(command) {
    fetch(`http://hydro.local:8081/${command}`, { method: "get" })
        .catch(err => console.error("error sending command: " + err) );
}

function read_samples() {
    fetch('http://hydro.local:8081/read_all', { method: "get" })
        .then(response => response.json())
        .then(update_samples)
        .catch(err => console.error("error sending command: " + err) );
}

function update_samples(data) {
    const precision = 2;

    temperature.innerText = data.temperature.toPrecision(precision);
    ec.innerText = data.ec.toPrecision(precision);
    ph.innerText = data.pH.toPrecision(precision);
}
