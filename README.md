# HP Server Fan Controller

I have a HP ProLiant MicroServer Gen 8 and in my opinion the fans were too noisy to be acceptable in the home. I replaced the 40mm power supply fan with a *Sunon HA40201V4-0000-999* and the rear 120mm extractor fan with an *Arctic BioniX F120*. The silentpcreview site had some useful notes on replacing the main fan, given that HP use a non standard connector.

After this, things were improved but I still wanted to run the 120mm main fan at a lower (quieter) speed. There is a PWM speed control wire that the server uses to set the speed according to how hot it is. There is also a rotation detection wire that the server uses to check whether the fan is actually spinning and hasn't failed. In my machine it seems like the rotation detection mechanism stopped working but I could get it to stop complaining and boot up if I just pulled the voltage to ground.

I made my own board to intercept the speed control and rotation detection wires. I prototyped the circuit and code with an Arduino Uno first and then converted to a bare Atmega 328P once I had it working. The code is quite short and has comments so take a look.

<a href="https://epixstudios.co.uk/filer/canonical/1532891767/5/"><img src="https://epixstudios.co.uk/filer/canonical/1532891767/5/" alt="Top side photo of board" width=33.3% /></a><a href="https://epixstudios.co.uk/filer/canonical/1532891767/6/"><img src="https://epixstudios.co.uk/filer/canonical/1532891767/6/" alt="Bottom side photo of board" width=33.3% /></a><a href="https://epixstudios.co.uk/filer/canonical/1532891767/7/"><img src="https://epixstudios.co.uk/filer/canonical/1532891767/7/" alt="Board wired up and mounted inside case" width=33.3% /></a>


## Speed control

I used the `pulseIn()` method to read the pulse width (in milliseconds) that the server was sending to set the fan speed. By printing this to the serial connection I was able to determine how long the signal was high during normal operation. Using this value, plus a bit of a buffer, I created a threshold. Below this I would be setting my own lower speed using `analogWrite()` and some trial and error. If the server instructs the fan to go faster with a pulse width higher than the threshold then I set the fan to a high value. This means that the fan will kick in when it has to.


## Rotation detection

I don't think this works correctly on my machine (or at least as I expected to). Anyway, I can still use it to signal to ther server that there is a fan failure so it can shut down. If I set it low (0V) then all is OK, and if I set it high (5V) then the server's light flashes red instead of being blue, it bleeps, shutsdown. This is done with `digitalWrite(alert_pin, LOW)`.

I read pulses from the fan for a fixed period of time using `millis()` and `digitalRead(fan_pin)` and expect to count the fan's signal as being high and low during the period. I confirmed this works by running the fan at a slow speed and sticking a piece of plastic into the rotors.
