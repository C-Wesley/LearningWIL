# Thruster Controls
We are using two Blue Robotics T200 thrusters and their Basic Electronic Speed Controllers (ESC).

<a href="https://bluerobotics.com/store/thrusters/t100-t200-thrusters/t200-thruster-r2-rp/" target="_blank">T200 Thruster</a>

<a href="https://bluerobotics.com/store/thrusters/speed-controllers/besc30-r3//" target="_blank">Basic ESC</a>

<a href="https://bluerobotics.com/store/comm-control-power/powersupplies-batteries/battery-lp-4s-10ah/" target="_blank"> Lithum Polymer Battery </a>

Objectives:
-Understand PWM Control of T200's
-Use the Serial Monitor to test different speeds

The T200 thrusters use a three phase brushless motor.
You can think of each phase as a separate coil that is wraped around a stator. 
These coils are energized following a pattern to make the rotor spin. See diagram below. 

![Three Phase Motor Diagram](https://github.com/C-Wesley/LearningWIL/blob/main/Thruster%20Controls/img/3phaseBrushlessMotor.gif)

Example Wiring Diagram: 
<a href="https://github.com/C-Wesley/LearningWIL/blob/main/Thruster%20Controls/img/ConnectionDiagram.png" target="_blank"> Lithum Polymer Battery </a>

We don't directly control the three phase connections. Instead we use an ESC. The ESC requires a PWM signal from the microcontroller and power from a battery. We have a wiring harness that allows us to power two ESC's in parallel. 

PWM stands for Pulse Width Modulation. Typically, we don't varry the period of this signal, but instead only vary the duty cycle. However the T200 thrusters required a very specific PWM signal. 

It's wants a signal with a 20ms period and a high puls of between 1100 -1900 microseconds, centered at 1500. That means when we set our PWM to have a high signal for 1500 microseconds, the motors will be turnd off. 

|High Pulse Time | Motor Direction | 
| 1475-1525      | OFF             | 
| 1100-1475      | REVERSE         |
| 1525-1900      | FORWARD         |


