# IFTTTLed
A LED for the Internet! Code and HowTo to build a LED to be
 triggered with [IFTTT/Maker channel](https://ifttt.com/maker), using Arduino.

# Code
The code you will need is here and is [iftttled.ino](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/iftttled.ino) and [webtypes.h](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/webtypes.h). Download both to a folder in your Arduino Sketch workspace. When iftttled.ino is open, certify that webtypes.h is also open in another tab of the same Arduino IDE instance - this should happen automatically.

# Configuration
1. Let's check if our circuit and code is working.
To test, open the terminal (in Linux) and type
* `curl --data "off" 192.168.0.100:13041` *for turning the LED off*
* `curl --data "on" 192.168.0.100:13041` *for turning the LED on*
* both commands should return `<HTML><BODY>TEST OK!</BODY></HTML>` if 192.168.0.100 is the correct Arduino IP.
2. Go to https://www.whatismyip.com/ and notedown your IP address.
This is your Internet modem IP, visible to the external world.
3. You need now to open you internet modem/router webpage -
 192.168.0.1 for most modems. Look for the words **Port
  Forwarding**. There should be a page containing a table with the
   External Port and the respective Internal IP and Port.
 * If you can't find this, you can look up on information about
 your modem/router on the Internet. Any change will require knowing
  your modem/router password.
4. Now we will redirect the external port 13041 (a random port) to
 your Arduino IP and Port - we will use in this example IP
  192.168.0.100 and port 80 (the default for web server).
5. To test, open the terminal (in Linux) and type
 (change `555.555.555.555` to the IP from 2nd step):
 * `curl --data "off" http://555.555.555.555:13041` *for turning the LED off*
 * `curl --data "on" http://555.555.555.555:13041` *for turning the LED on*
 * both commands should return `<HTML><BODY>TEST OK!</BODY></HTML>`.
6. If your led turns on and off accordingly, it's working.
Now we need to connect this to [IFTTT](https://ifttt.com/)!

# Build

* A LED
* A 1k Ohm Resistor
* Wire
* An Arduino UNO
* An Arduino Ethernet Shield

The led Cathode must be placed on pin 2 of the Arduino Ethernet Shield.

Connect the resistor on the led Anode, and the other pin from the resistor on GND.

Use Wires if necessary, for better grip and flexibility.

# IFTTT

So basically we are going to create a recipe that both the **event** and the
**action** uses the Maker Channel - because is the fastest way to test things are
working. And actually we will need TWO recipes: one for turning the LED ON and
the other for turning the LED OFF.

1. Go to http://ifttt.com/myrecipes/personal

2. Click on create a recipe

 ![step 0](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/img/step0.png)

3. Select the Maker Channel for the *this*

 ![step 1](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/img/step1.png)

4. Click on the **Receive a web request**

 ![step 2](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/img/step2.png)

5. Type the Event Name as `led_on`

 ![step 3](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/img/step3.png)

6. The *that* is the Maker Channel again

 ![step 4](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/img/step4.png)

7. Click on the **Make a web request**

 ![step 5](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/img/step5.png)

8. Fill the form below, the URL must contain your IP instead of those 555.555.555.555 (remember, you can get those [whatismyip](https://www.whatismyip.com/)). Don't forget to place the port that you are forwarding. The **Method** should be `POST`, **Content Type** should be `text/plain` and the **Body** should be `on`. Cick in Create Action!

 ![step 6](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/img/step6.png)

9. Now we can Create the Recipe!

 ![step 7](https://raw.githubusercontent.com/ericoporto/IFTTTLed/master/img/step7.png)

10. You will need to create now another recipe, but this time instead of the **event name** should be `led_off` and the **Body** of the action should contain `off`

11. time to test.

Test
----

* Test with:

        curl -X POST https://maker.ifttt.com/trigger/led_on/with/key/YOURKEYHERE

* and the answer should be:

        Congratulations! You've fired the led_on event

* the led should switch on!

* and now test with

        curl -X POST https://maker.ifttt.com/trigger/led_off/with/key/YOURKEYHERE

* and the answer should now be:

        Congratulations! You've fired the led_on event

* the led should switch off!
