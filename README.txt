3D printing "Stepper robot arm"　was uploaded by Bajdi August 17, 2015. (https://www.thingiverse.com/thing:970881)

This stepping motor can be made into a bipolar stepping motor by changing the internal circuit pattern. 
Bajdi changed it to bipolar and uses it, but I use it as unipolar in this repository.

The electrical circuit is shown in circuit1.jpg and circuit2.jpg. The schematic of the robot arm controller is shown in circuit3.jpg. 
The parts list is shown in Parts_list.pdf.


A total of 13 motor control pins and 3 limit switch pins were required, leaving only two A4 and A5 pins on the Arduino uno.
I had to control 3 stepper motors and 1 servo motor with these 2 pins.
I have achieved this with two tact switches and two potentiometers. Two potentiometers control two stepper motors respectively. 
When each tact switch is pressed separately, another stepping motor rotates left and right. 
When both tact switches are pressed simultaneously, a gripper attached to a servo motor opens and closes.
