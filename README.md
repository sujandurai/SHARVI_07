# SHARVI O7: Smart Companion Educational Rover

## Abstract

This paper presents **SHARVI O7**, a smart companion educational rover designed to integrate interactive learning with robotics. The system combines motion, visual display, audio feedback, and user interaction to create an engaging learning environment for children. Built using the Vega Aries V2.0 platform, the rover supports multiple modes including alphabet learning, number learning, shape recognition, maze solving, and quiz interaction. The proposed system aims to bridge the gap between traditional learning and interactive smart education by providing a low-cost, scalable, and modular solution.

## Keywords

Educational Robot, Embedded Systems, Smart Learning, LED Matrix, DFPlayer, Interactive Learning, IoT Rover

---

## I. Introduction

Traditional learning methods often lack engagement and interactivity, especially for early-stage learners. With advancements in embedded systems and robotics, there is a growing opportunity to create systems that combine education and entertainment. SHARVI O7 is designed as a **multi-functional educational rover** that enhances learning through visual, auditory, and physical interaction.

The system integrates sensors, display modules, audio systems, and control logic to deliver an interactive experience. It operates in two primary modes: **Fun Mode** and **Educational Mode**, making it suitable for both engagement and structured learning.

---

## II. Problem Statement

Existing educational tools either focus only on entertainment or purely on academic learning. Very few systems combine **robotic interaction, visual display, audio feedback, and gamified learning** in a single platform.

Children learn better through:

* Visual representation
* Audio reinforcement
* Physical interaction

SHARVI O7 addresses this by integrating all these elements into one system.

---

## III. System Architecture

The system consists of the following major components:

### A. Hardware Components

* Vega Aries V2.0 Controller
* LED Matrix Display (MAX7219)
* DFPlayer Mini Audio Module
* Speaker
* Touch Sensor
* Ultrasonic Sensor
* Motor Driver and DC Motors
* Bluetooth Module
* Power Supply

### B. Software Modules

* Mode Control Logic
* Display Control (LED Matrix)
* Audio Playback Control
* Sensor Processing (Obstacle Detection)
* Bluetooth Communication
* Quiz Evaluation Logic

---

## IV. Working Methodology

The system starts in idle mode and switches between modes using touch input.

### A. Educational Mode

1. Alphabet Mode – Displays letters and plays corresponding audio
2. Number Mode – Displays numbers and plays sound
3. Shape Mode – Displays shapes and teaches names
4. Quiz Mode – Interactive question-answer system
5. Maze Mode – Controlled navigation using Bluetooth

### B. Fun Mode

1. Obstacle Avoidance
2. Uncaught Bot Behavior
3. Emotional Interaction using display and sound

---

## V. Display and Audio Integration

The LED matrix provides visual output while the DFPlayer module delivers synchronized audio feedback. This dual interaction enhances learning efficiency.

Example:

* Display “A” → Play corresponding audio
* Display shape → Speak shape name

---

## VI. Results and Performance

The system successfully:

* Displays alphabets, numbers, and shapes
* Provides synchronized audio output
* Performs obstacle avoidance
* Executes quiz interaction with correct/wrong feedback
* Enables Bluetooth-based maze navigation

The integration of multiple modules results in smooth and interactive performance.

---

## VII. Advantages

* Combines fun and education
* Multi-sensory learning (visual + audio)
* Interactive and engaging
* Low-cost implementation
* Modular and expandable design

---

## VIII. Applications

* Primary education systems
* Smart toys
* Robotics demonstrations
* STEM learning kits
* Educational exhibitions

---

## IX. Future Scope

Future improvements include:

* AI-based interaction
* Voice recognition
* Mobile app integration
* Multi-language support
* Camera-based object recognition

---

## X. Conclusion

SHARVI O7 demonstrates how embedded systems and robotics can be used to enhance educational experiences. By integrating movement, display, sound, and interaction, the system provides a comprehensive learning platform that is both effective and engaging.

---

## References

[1] Microchip Technology, "PIC32CM Series Datasheet"
[2] MAX7219 LED Driver Datasheet
[3] DFPlayer Mini Module Documentation
[4] Embedded Systems Design Concepts, IEEE Resources
