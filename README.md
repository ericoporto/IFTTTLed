# IFTTTLed
A LED for the Internet! Code and HowTo to build a LED to be
 triggered with [IFTTT/Maker channel](https://ifttt.com/maker), using Arduino.

# Code
The code you will need is here and is `iftttled.ino` and `webtypes.h`.

# Configuration
1. Go to https://www.whatismyip.com/ and notedown your IP address. This is your Internet modem IP, visible to the external world.
2. You need now to open you internet modem/router webpage -
 192.168.0.1 for most modems. Look for the words **Port
  Forwarding**. There should be a page containing a table with the
   External Port and the respective Internal IP and Port.
 * If you can't find this, you can look up on information about
 your modem/router on the Internet. Any change will require knowing
  your modem/router password.
3. Now we will redirect the external port 13041 (a random port) to
 your Arduino IP and Port - we will use in this example IP
  192.168.0.100 and port 80 (the default for web server).
4. To test, open the terminal (in Linux) and type
 (change `555.555.555.555` to the IP from 1st step):
 * `curl --data "off" http://555.555.555.555:13041` *for turning the LED off*
 * `curl --data "on" http://555.555.555.555:13041` *for turning the LED on*
 * both commands should return `<HTML><BODY>TEST OK!</BODY></HTML>`.
5. If your led turns on and off accordingly, it's working.
Now we need to connect this to [IFTTT](https://ifttt.com/)!

# IFTTT

TBD

Test with
    curl -X POST https://maker.ifttt.com/trigger/led_on/with/key/YOURKEYHERE

and

    curl -X POST https://maker.ifttt.com/trigger/led_off/with/key/YOURKEYHERE
