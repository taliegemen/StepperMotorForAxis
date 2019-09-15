# StepperMotorForAxis
This is an Arduino Project that i made to control a stepper motor to position an axis of a device.
First of all, the motor tries to go to origin level if the sensor is High it means it is at origin.
After that it starts to move the other way to leave the sensor at Low.
With this operation completed you can serially send the arduino to go to a position.
In our system the motor turns 1 turn with repeating our lookout table 512 times and that moves the motor for 50mm.
So if you send the motor <A0050> it will make a turn. You can send the signal with times of 25.
If you send <HXXXX> motor will do the first operation again. If you send <WXXXX> motor send value of where it is.
