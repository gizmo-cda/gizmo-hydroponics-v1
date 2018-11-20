function send_schedule() {
    // TODO: build schedule object here
    const schedule = {
        "name": "test",
        "value": 10
    };

    try {
        fetch(
            '/schedule',
            {
                method: 'POST',
                credentials: 'include',
                body: JSON.stringify(schedule),
                headers: { 'Content-Type': 'application/json' }
            }
        )
        .then(res => res.json())
        .then(json => {
            console.log(json);
        });
    }
    catch(e) {
        console.error(e);
    }

    return false;
}
