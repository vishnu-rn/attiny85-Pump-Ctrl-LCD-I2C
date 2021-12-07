# attiny85-Pump-Ctrl-LCD-I2C

Pump control using 2 level switches and and LCD display with I2C (16x2) for feedback.

The project was made possible with the help from https://www.instructables.com/id/Using-an-I2C-LCD-on-Attiny85/ by diy-bloke.
though when I tried to make it work, the LCD just threw out garbage, this was the case when I tried uploading the code through 
Arduino IDE as well. After some experimenting, I found out that setting clock internal 1 MHz and adding a delay before each command
makes the LCD work like it should.
