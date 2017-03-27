# Photon-Blynk-Ubidots-

A copy of my Blynk project can be found here: https://www.dropbox.com/s/25wxv7nrfb861zj/Image-1.jpg?dl=0

This project uses Blynk and 2 Particle Photon's to control a thing. In my case, to control a central heating system. The standard "on/off" times are embedded in code. 

There are 2 other methods for requested an "on" condition. 1. Constant heat. 2. Timed heat.

Constant heat turns sets the "on" condition to true.

Time heat sets the "on" condition to true for a period of between 1 min and 60 mins via the Blynk interface.

There are 2 Photons in my system. The main control system, mine is called Bert, uses a relay shield. Buy one: http://bit.ly/2n4NFKl.

The second Photon, mine is called Ernie, is located in the Entrance Hall and uses an MPL3115A2 to determine ambient temperature. It sends "pump profile" data to Bert via the Particle eco-system.

Happy building!
