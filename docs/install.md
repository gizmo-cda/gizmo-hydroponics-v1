- Follow prompts to configure:
	- password
	- locale
	- keyboard
	- timezone
- Set background image, if desired
- Open Preferences->Raspberry Pi Configuration
	- Set hostname
	- Disable overscan
	- Turn on SSH
	- Turn on VNC
- Install utilities and node
```bash
	sudo apt-get update
	sudo apt-get upgrade -y
	sudo apt-get autoremove -y
	sudo apt-get install -y vim
	sudo apt-get install -y unclutter
	sudo apt-get install -y build-essential
	curl -L https://git.io/n-install | bash
	cd
	source .bashrc
	```
- Set some aliases
	```
	vim /home/pi/.bash_aliases
	alias ll='ls -alF'
	alias ..='cd ..'
	```
- Follow setup.md
- Update npm install
	```bash
	cd ~/app
	npm install
	```
- Setup school ID
	```bash
	mkdir -p ~/app/app/db
	cat '"<school-id>"' > id.json
```
