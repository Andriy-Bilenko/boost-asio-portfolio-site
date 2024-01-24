# boost-asio-portfolio-site (https://bilenko.sytes.net)
## About the program
Of course there are a lot of already established server programs, but for the sake of learning Boost ASIO and trying out network programming, these small HTTP and HTTPS servers were created.
I was curious how do some people host websites accessible through tor network, so I hosted one (http://defaultuzrc5mqrl2r73673vwgvpzscoubjb7n23wezmyq4ffoyjeiyd.onion), and told how to do that below.
Also I was surprized when I noticed some IP's that are not mine or my friends' visiting my site, and there were a lot of them, so I composed a python IP lookup script that traverses the list of IPs using freely accessible webservice through "torred" firefox browser and outputs the geolocation and the organisation visiting.
**Note #1**: This project is not intended for accomplishing any malicious goals, so authors do not take the responsibility for code misuse or modifications.
Note #2: Website UI was not designed from ground zero, it is a modified template created by [Nisar](https://github.com/nisarhassan12/).
## Working program
### HTTPS server
<img src="src/https.gif" width=100% alt="gif">

### HTTP server
<img src="src/http.gif" width=30% alt="gif">

## Server setup
**Ubuntu:**
`sudo apt update`
`sudo apt install g++ make`
`sudo apt install libboost-all-dev`
`sudo apt-get install libssl-dev`
`dpkg -s libboost-all-dev | grep Version`
**Arch linux (might not be the full list):**
`sudo pacman -Syu`
`sudo pacman -S base-devel`
`sudo pacman -S boost boost-libs openssl boost-asio`
**to run the server:**
`cd http` or `cd https`
`sudo make run`

to test it out visit https://localhost or http://localhost respectively
## Onion website
#### Setup
On arch linux `sudo pacman -S tor`
On ubuntu `sudo apt install tor`
On both `sudo service tor start`

after that add lines
```
HiddenServiceDir /var/lib/tor/your_hidden_service_name/
HiddenServicePort 80 127.0.0.1:80
```
somewhere to `/etc/tor/torrc`
then restart tor service:
`sudo systemctl restart tor`

-  make sure your HTTP server is up and running
-  allow connections from outside on port 80 (and to be doubly sure on port 9050)
-  install [tor browser](https://www.torproject.org/download/)
-  and visit your website on link in the file `/var/lib/tor/your_hidden_service_name/hostname`.

Note #3: If changes are made to the torrc you'd normally need to refresh the tor service.

#### Extra: customizing v3 .onion link
you can use [eschalot tool](https://github.com/ReclaimYourPrivacy/eschalot) , `./eschalot -ct5 -p {name}` and get yourself the .onion hostname starting with {name}. Put files `hostname`, `hs_ed25519_public_key` and `hs_ed25519_secret_key` in place of the same files in `/var/lib/tor/your_hidden_service_name/` and restart tor service. Clear cache and restart tor browser if you want to see changes made.

## Lookup script
**On arch:**
`sudo pacman -S python`
`sudo pacman -S python-virtualenv`
**On ubuntu**
`sudo apt install python3`
`sudo apt install python3-venv`
**to run it:**
make sure to have sessions.log in the same directory as the script and tor service installed
`python3 -m venv venv`
`source venv/bin/activate`
`python ip_lookup.py`
(upon errors install needed python modules using pip)



If you ran into some issues by any chance or need to contact the developer, it would be great to recieve your valuable feedback on email: *bilenko.a.uni@gmail.com*.

<div align="right">
<table><td>
<a href="#start-of-content">↥ Scroll to top</a>
</td></table>
</div>
