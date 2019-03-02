# [MoveBoard App](https://devpost.com/software/change-name-later/)

## Inspiration
We found that a lack of honesty is a prevalent problem in society that can create tension between friends. For example, when people are talking about what kinds of tricks they've done on their skateboard, they're often prone to exaggeration. This makes it really hard to discern the truth from the exaggerated truth. We wanted to create an easy way for skateboarders and snowboarders to track their trick metrics and share them with their friends in an accountable, open way!

## What it does
It measures your board's movement and how quickly it's going. It measures that through the sensors built into the Arduino 101. It converts the absolute position every second, as given by the sensor, into "tricks" by measuring the change in position in all 3 axes and converting that to a list of skateboard tricks that we've compiled.

## How we built it
MoveBoard uses an Arduino 101 and its built-in gyroscopic and accelerometer sensors.

## Challenges we ran into
Due to limited hardware availability, we didn't have access to a Bluetooth dongle that goes with the Arduino 101. This means that we couldn't create as close of a prototype of our product as we wanted to. Instead of sending information to the web server via Bluetooth, we sent it through the USB cable, meaning a physical connection between the tracker and the viewing device had to be made.

## Accomplishments that we're proud of
We're proud of how quickly we picked up a new stack and adapted. Furthermore, we experimented with different ways of implementing the backend and storing information in a database. This allowed us to see the benefits and drawbacks of each and make an educated decision, not only for this hackathon, but later projects as well!

## What we learned
We learned how to use Arduino 101 and the accompanying IDE. None of us had worked with Arduino before so it was definitely a learning opportunity for everyone. Overall, it was a really rewarding experience!

## What's next for MoveBoard
In the future, we want to implement more features like speed tracking
