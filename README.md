# Golf-Bot

🏌️ Golf-BOT (ENGR 3100 Final Project)

Golf-BOT is an Arduino-based autonomous robot that locates, tracks, approaches, and strikes a golf ball using Pixy2 computer vision (Signature 1) and servo motor control. A proportional controller keeps the ball centered while driving, and once close (pixel width threshold), the bot aligns, nudges forward, and performs a putting swing.

📸 Demo Video

Video #1: 
https://drive.google.com/file/d/1pFJkDqQQc59rjWfJwUSvCaVInL8ACEIz/view?usp=sharing

Video #2: 
https://drive.google.com/file/d/1mrCYS8novTV8ScCD3sQgaekj74rZ9VeW/view?usp=sharing

⚙️ Features

Pixy2 object detection using color signatures

Autonomous search behavior (spins when no ball is detected)

P-controller yaw correction to stay centered on the ball

Distance estimation using detected ball pixel width

Automated striking routine using a dedicated putter servo

🧰 Hardware Used

Arduino (UNO or compatible)

Pixy2 Camera

3x Vex Servo Motors

External Power Supply (6x AA Batteries)

Vex Robotics Kit

🧠 How It Works

Pixy2 scans for detected blocks and looks for Signature 1

If no ball is found → robot spins to search

If ball is found → robot calculates horizontal error from the center of view

A proportional controller generates a turn command to yaw toward the ball

Once the ball appears “close enough” (pixel width threshold) → robot stops, nudges forward, and strikes

📁 Code Overview

Main file: Golf-BOT.ino

Key Functions

yawTurn(u) – turns robot toward the ball based on P-controller output

Spin() – rotates robot to search when ball isn’t detected

golfShot() – alignment + nudge + strike sequence

strikeBall() – putter servo wind-up and swing motion

🔧 Tunable Parameters

You can adjust these values to match your robot setup:

kp – proportional gain (steering aggressiveness)

CLOSE_WIDTH – pixel width threshold for “ball is close”

Servo PWM values in yawTurn() for motor speed tuning

Delays inside nudge() and strikeBall() for timing adjustments

🚀 Setup Instructions

Install the Pixy2 Arduino library

Connect Pixy2 and servos to the Arduino

Train the golf ball as Signature 1 in PixyMon

Upload Golf-BOT.ino to the Arduino using the Arduino IDE

Power the servos with an external supply (recommended)

🧪 Notes

Servo PWM values depend heavily on your motor orientation and wiring

Continuous rotation servos may need calibration for true “stop” values

Performance improves with consistent lighting and a high-contrast ball color

👤 Author

Dylan Venditti
ENGR 3100 — Final Project (Fall 2025)
