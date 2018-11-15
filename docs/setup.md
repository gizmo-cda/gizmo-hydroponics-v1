- [Setup Static IP Fallback](#setup-static-ip-fallback)
- [Setup Git Remote](#setup-git-remote)
- [Setup Node](#setup-node)
- [Setup Pimoroni](#setup-pimoroni)
- [Setup Service](#setup-service)
- [Setup Kiosk Mode](#setup-kiosk-mode)
- [Setup crontab](#setup-crontab)

---

# Setup Static IP Fallback

Edit `/etc/dhcpcd.conf` and add the following text to the end of the file:

```
interface eth0
fallback static_eth0

profile static_eth0
static ip_address=192.168.0.1/24
```

Note that recent version of Raspbian have lines similar to this that have been commented out. Feel free to uncomment
those lines, but be sure the ip_address matches what is listed above.

# Setup Git Remote

Run the following commands on the Pi to create the git remote directory:

```sh
cd
mkdir gizmo-hydroponics-v1.git
cd gizmo-hydroponics-v1.git
git init --bare
```

You will need to check-out a local copy of this repository. This will be the active application directory.

```sh
cd
git clone gizmo-hydroponics-v1.git app
```

In order to update the application directory on the Pi each time a commit to master is pushed to it, create the following script at `/home/pi/gizmo-hydroponics-v1.git/hooks/post-update:

```sh
#!/usr/bin/env bash

unset GIT_DIR

APP_DIR="/home/pi/app"
BRANCH="master"

if [ "$1" == "refs/heads/${BRANCH}" ]; then
    echo "Detected commit to ${BRANCH}"
    echo "Updating ${APP_DIR} directory."
    git -C "${APP_DIR}" pull
fi
```

Some things to note:

- This file needs to be executable
- This assumes you want to update `/home/pi/app`. Alter the `APP_DIR` variable in the script if you wish to use another directory.

In order to push to this remote repository, you'll need to add the remote to your development machine:

```sh
git remote add pi pi@hydroponics.local:gizmo-hydroponics-v1.git
```

You'll now be able to push to that repository:

```sh
git push pi master
```

# Setup Node

# Install Node

```sh
sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install -y build-essential
curl -L https://git.io/n-install | bash
n lts
n prune
```

You can check node and npm versions via:

```sh
node --version
npm --version
```

Note that you may need to logout and back in before `n` will be in your path.

# Setup Pimoroni

- [Enviro Phat Getting Started](https://learn.pimoroni.com/tutorial/sandyj/getting-started-with-enviro-phat)

```bash
curl https://get.pimoroni.com/envirophat | bash
```

Simply follow the prompts.

# Setup Service

Run the following to initialize the service and make it start automatically when the system boots:

```bash
sudo cp hydro-web.service /etc/systemd/system
sudo systemctl enable hydro-web
```

You can start/stop/restart the service using the following commands:

```bash
sudo systemctl start hydro-web
sudo systemctl stop hydro-web
sudo systemctl restart hydro-web
```

If you make changes to the service file, you will need to reload the service daemon and restart the service

```bash
sudo systemctl daemon-reload
sudo systemctl restart hydro-web
```

You can check the status of your service:

```bash
systemctl status hydro-web
```

# Setup Kiosk Mode

- Install unclutter. This utility hides the mouse.

```
sudo apt-get install unclutter
```

- Create `/home/pi/.config/lxsession/LXDE-pi/autostart` and paste in the following content:

```
@lxpanel --profile LXDE-pi
@pcmanfm --desktop --profile LXDE-pi
@xscreensaver -no-splash
@xset s off
@xset -dpms
@xset s noblank
@unclutter -idle 3
@chromium-browser --incognito --kiosk http://127.0.0.1:8080/
```

This will:

- prevent the screen saver from starting
- prevent the screen from going black
- make the cursor disappear after 3 seconds of inactivity
- will open Chromium in kiosk mode

# Setup crontab

NOTE: This section has not been finalized. The crontab entry below is being used for testing only.

Edit the pi user's crontab:

```bash
crontab -e
```

If you are asked to choose an editor, use `vim.basic`. Add the following line to the bottom of the crontab and save your file:

```
*/1 * * * * /home/pi/n/bin/node /home/pi/hydroponics_app/scripts/send_commands.js light_on read_light 2>&1 | /usr/bin/logger -t hydro_i2c
```
