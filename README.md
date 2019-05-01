# Auto Drone

## What is an Auto Drone?

Auto Drone is, as the name suggests, an autonomous drone. Using various sensors,
the drone is able to calculate height off the ground, angle of rotation, and acceleration. Using data from these sensors, an on-board computer is able to
increase or decrease propellor speed in order to keep the drone stable.

## Purpose

Auto Drone was created to be a tag-along to an autonomous farming robot. Auto
Drone's purpose will be to collect data about farming conditions and types of
weeds in an orchard. Data collected from Auto Drone will be fed into
a cloud-based machine learning algorithm to identify the types of weeds specific
to each orchard. That data can help a pest control advisor to prescribe the
proper pesticides, and amount of pesticide needed to control weed growth.

## Accessibility and Technology

Auto Drone uses completely open-source technology. The main board used for this
project is an Arduino Uno. The sensors used are the HC-SR04 to determine height
from the ground, as well as an MPU6050 which determines angle of rotation,
acceleration, and temperature.
