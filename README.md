# PokeBot — Bluetooth-Controlled Sumo Robot

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Control](https://img.shields.io/badge/Control-Bluetooth-blueviolet)
![CAD](https://img.shields.io/badge/CAD-Fusion%20360-orange)
![Material](https://img.shields.io/badge/Material-PLA-lightgrey)
![Status](https://img.shields.io/badge/Status-Completed-success)
![Competition](https://img.shields.io/badge/Battles%20Won-2%2F3-success)

> A fully 3D-printed, Bluetooth-controlled sumo robot integrating custom mechanical design, embedded control, power electronics, and PCB fabrication.

---

## Overview

**PokeBot** is a manually controlled sumo robot developed as an integrated electronics, PCB fabrication, embedded systems, and mechanical design project.

The robot was originally inspired by **Snom**, with the objective of translating its general geometry and appearance into a functional sumo robot. The final design uses a **two-wheel differential-drive system**, an **ESP32** for control and Bluetooth communication, and a custom-designed PCB for power conversion and electrical integration.

Every component of the chassis and external enclosure was designed from scratch in **Autodesk Fusion 360** and manufactured using **PLA FDM 3D printing**.

The completed robot participated in three sumo battles, winning **2 out of 3 matches**.

---

## Final Specifications

| Parameter | Final Implementation |
|---|---|
| **Dimensions** | 20 × 20 × 15 cm |
| **Mass** | 420 g |
| **Chassis Material** | PLA |
| **Manufacturing** | FDM 3D Printing |
| **Mechanical CAD** | Autodesk Fusion 360 |
| **Microcontroller** | ESP32 |
| **Drive System** | 2 × DC Gearmotors |
| **Steering** | Differential Drive |
| **Wireless Communication** | Bluetooth |
| **User Interface** | Smartphone |
| **Control Platform** | Dabble GamePad |
| **Battery System** | 2 × 3.7 V Li-ion |
| **Nominal Battery Voltage** | 7.4 V |
| **Charging Module** | TP4056 |
| **Custom PCB** | 7.4 V → 5 V DC Buck Converter |
| **Competition Record** | 2 Wins / 3 Battles |
| **Win Rate** | 66.7% |

---

## System Architecture

```text
                         ┌──────────────────┐
                         │    Smartphone    │
                         │  Dabble GamePad  │
                         └────────┬─────────┘
                                  │
                              Bluetooth
                                  │
                                  ▼
                         ┌──────────────────┐
                         │      ESP32       │
                         │ Control + Comms. │
                         └────────┬─────────┘
                                  │
                             GPIO / PWM
                                  │
                                  ▼
                         ┌──────────────────┐
                         │   Motor Driver   │
                         └────┬────────┬────┘
                              │        │
                              ▼        ▼
                          Motor A    Motor B
                              │        │
                              └───┬────┘
                                  │
                          Differential Drive


                   2 × 3.7 V Li-ion Batteries
                                  │
                                7.4 V
                                  │
                                  ▼
                         ┌──────────────────┐
                         │    Custom PCB    │
                         │  DC Buck Stage   │
                         │   7.4 V → 5 V    │
                         └────────┬─────────┘
                                  │
                                 5 V
                                  │
                                  ▼
                                ESP32
```

---

## Mechanical Design

The complete mechanical structure of PokeBot was designed in **Autodesk Fusion 360**.

This included the:

- Main chassis
- Upper enclosure
- Motor mounting geometry
- Internal component placement
- Battery placement
- Electronics mounting
- External Snom-inspired geometry

The complete structure was manufactured using **PLA** through FDM 3D printing.

### Design Constraints

The mechanical design had to balance several requirements:

- Remain within a **20 × 20 cm footprint**
- Maintain sufficient internal volume for electronics and batteries
- Securely mount both DC motors
- Minimize unnecessary mass
- Survive impacts during competition
- Maintain adequate wheel contact and traction
- Allow access to internal electronics
- Preserve the intended PokeBot appearance

The final assembled robot measured:

**20 cm × 20 cm × 15 cm**

with a total mass of:

**420 g**

---

## Drive System

PokeBot uses two independently controlled DC gearmotors in a **differential-drive configuration**.

No conventional steering mechanism is required. Direction is controlled by independently changing the direction of rotation of each motor.

| Command | Motor A | Motor B | Robot Motion |
|---|---|---|---|
| Forward | Forward | Forward | Forward |
| Reverse | Reverse | Reverse | Reverse |
| Left | Reverse | Forward | Rotate Left |
| Right | Forward | Reverse | Rotate Right |
| Stop | Stop | Stop | Stop |

Driving the motors in opposite directions allows PokeBot to perform approximately **zero-radius turns**, providing high maneuverability during sumo battles.

---

## Electronics

### ESP32 Control

The **ESP32** acts as the central processing and communication unit of the robot.

Its primary responsibilities are:

- Receiving Bluetooth commands
- Processing smartphone GamePad inputs
- Generating motor direction signals
- Generating PWM signals
- Controlling the motor driver
- Coordinating robot movement

Bluetooth communication is handled directly by the ESP32, eliminating the need for a separate wireless communication module.

---

## Bluetooth Control

PokeBot is manually controlled from a smartphone using the **Dabble** platform.

The ESP32 receives commands using the `DabbleESP32` library and maps the GamePad directional inputs directly to robot movements.

```text
UP      → Move Forward
DOWN    → Move Backward
LEFT    → Rotate Left
RIGHT   → Rotate Right
NONE    → Stop
```

This provides a simple wireless human-machine interface without requiring a dedicated physical remote controller.

---

## Motor Control

The ESP32 generates both direction and PWM signals for the two DC motors.

### GPIO Configuration

| Function | ESP32 GPIO |
|---|---:|
| Motor A PWM | GPIO 5 |
| Motor A IN1 | GPIO 18 |
| Motor A IN2 | GPIO 19 |
| Motor B PWM | GPIO 27 |
| Motor B IN1 | GPIO 26 |
| Motor B IN2 | GPIO 25 |
| Motor Driver Standby | GPIO 4 |

### PWM Configuration

```text
PWM Frequency:  1 kHz
Resolution:     8 bits
PWM Range:      0–255
Default Speed:  200
```

The firmware continuously processes Bluetooth input and updates the motor direction and speed accordingly.

The main control structure can be summarized as:

```cpp
Dabble.processInput();

if (GamePad.isUpPressed())
    moveForward(motorSpeed);
else if (GamePad.isDownPressed())
    moveBackward(motorSpeed);
else if (GamePad.isLeftPressed())
    turnLeft(motorSpeed);
else if (GamePad.isRightPressed())
    turnRight(motorSpeed);
else
    stopMotors();
```

> The complete ESP32 firmware is available in the source files of this repository.

---

## Power System

The final PokeBot uses:

**2 × 3.7 V lithium-ion batteries**

providing a nominal combined battery voltage of:

**7.4 V**

The battery system supplies the robot's motor and control electronics while remaining compact enough to fit inside the 3D-printed enclosure.

### Power Architecture

```text
              2 × 3.7 V Li-ion
                      │
                      ▼
                    7.4 V
                      │
             ┌────────┴────────┐
             │                 │
             ▼                 ▼
       Motor System       Custom PCB
                              │
                              ▼
                       Buck Converter
                         7.4 V → 5 V
                              │
                              ▼
                            ESP32
```

A **TP4056** module was incorporated into the battery and charging implementation.

---

## Custom PCB

One of the central electronics components developed for PokeBot was a **custom-designed and fabricated PCB**.

Rather than relying exclusively on external converter modules and loose wiring, the PCB was designed specifically around the electrical requirements of the robot.

### Main Functions

The final PCB performs two primary functions:

1. **DC voltage conversion**
2. **ESP32 electrical interconnection**

The board converts the nominal battery voltage from:

```text
7.4 V DC
   │
   ▼
DC Buck Converter
   │
   ▼
5 V DC
```

The regulated **5 V output** provides the required low-voltage supply for the ESP32-side electronics.

The PCB also provides a dedicated connection interface for the ESP32, improving the organization of the electrical architecture and reducing the amount of loose internal wiring.

---

## Original Electronic Architecture

During the schematic-design stage, PokeBot was divided into several functional subsystems:

```text
              ┌─────────────────┐
              │  Power Supply   │
              └────────┬────────┘
                       │
          ┌────────────┼────────────┐
          │            │            │
          ▼            ▼            ▼
  Microcontroller   Wireless     Motor Driver
       ESP32      Communication    System
          │                         │
          └─────────────────────────┘
                       │
                       ▼
                    Motors
```

The original electrical architecture separated the design into:

- Power supply
- Microcontroller processing
- Bluetooth communication
- Motor driving
- Mechanical actuation

This modular architecture provided the basis for the PCB and system integration stages.

The final physical implementation evolved from the original schematic as component selections and power requirements were refined during prototyping.

---

## Design Evolution

The project went through several iterations between the initial engineering proposal and the final competition robot.

### Initial Concept

The original design established the use of:

- ESP32-based control
- Bluetooth communication
- Two DC gearmotors
- Rechargeable lithium-ion power
- H-bridge motor control
- Custom PCB development
- Fully 3D-printed structure

### Final Implementation

Following fabrication, testing, and system integration, the final architecture consisted of:

- **2 DC motors**
- **2 × 3.7 V lithium-ion batteries**
- **ESP32**
- **Bluetooth smartphone control**
- **Dabble GamePad**
- **TP4056**
- **Custom 7.4 V → 5 V buck-converter PCB**
- **Fully PLA 3D-printed chassis**
- **Fusion 360 mechanical design**

This iterative development process allowed the original design to evolve into a practical competition-ready implementation.

---

## Competition Performance

PokeBot was tested in an actual sumo robot competition rather than exclusively under laboratory conditions.

### Results

```text
┌─────────────────────────────┐
│      COMPETITION RECORD     │
├─────────────────────────────┤
│ Battles              3      │
│ Wins                 2      │
│ Losses               1      │
│ Win Rate          66.7%     │
└─────────────────────────────┘
```

**Final result: 2 wins out of 3 battles.**

The competition provided a complete system-level test involving:

- Wireless communication
- Real-time user control
- Motor response
- Mechanical durability
- Battery performance
- Traction
- Turning capability
- PCB operation
- Electromechanical integration

The final result demonstrated that the complete mechanical, electrical, and embedded system could operate successfully under actual competition conditions.

---

## Repository Structure

A recommended organization for the complete project repository is:

```text
PokeBot/
│
├── README.md
│
├── firmware/
│   └── MotorDriver_Try1.ino
│
├── pcb/
│   ├── schematics/
│   ├── pcb-layout/
│   └── fabrication-files/
│
├── mechanical/
│   ├── fusion360/
│   └── stl/
│
├── documentation/
│   ├── project-proposal.pdf
│   └── schematics.pdf
│
└── media/
    ├── photos/
    └── renders/
```

---

## Technologies Used

### Hardware

- ESP32
- DC Gearmotors
- Motor Driver
- 3.7 V Lithium-ion Batteries
- TP4056
- Custom PCB
- FDM 3D-Printed Components

### Embedded Software

- Arduino IDE
- C/C++
- Dabble
- DabbleESP32
- ESP32 Bluetooth

### Electronic Design

- Schematic Capture
- PCB Design
- DC Power Conversion
- PWM Motor Control
- H-Bridge Motor Control
- PCB Fabrication

### Mechanical Design

- Autodesk Fusion 360
- FDM Additive Manufacturing
- PLA
- Mechanical Assembly

---

## Engineering Skills Demonstrated

PokeBot integrates multiple areas of electrical and mechatronics engineering into a single working system.

### Embedded Systems

- ESP32 programming
- Embedded C/C++
- Bluetooth communication
- Real-time command processing
- GPIO control
- PWM generation

### Electronics

- DC motor control
- H-bridge implementation
- DC power conversion
- Voltage regulation
- Battery integration
- Schematic design
- PCB design
- PCB fabrication
- Electronic assembly and soldering

### Mechanical Engineering

- Mechanical CAD
- Autodesk Fusion 360
- Component packaging
- Motor mounting
- Chassis design
- Design for additive manufacturing
- FDM 3D printing
- PLA prototyping

### Mechatronics

- Electromechanical system integration
- Hardware/software integration
- Differential-drive control
- Power and control integration
- Iterative prototyping
- Physical testing
- Competition-based validation

---

## Project Development Process

The complete project followed an engineering workflow from initial concept to physical validation:

```text
Concept
   │
   ▼
Requirements Definition
   │
   ▼
System Architecture
   │
   ├──────────────┐
   ▼              ▼
Electronic     Mechanical
Design           CAD
   │              │
   ▼              ▼
PCB Design     3D Printing
   │              │
   └──────┬───────┘
          ▼
   Firmware Development
          │
          ▼
   Hardware Assembly
          │
          ▼
   System Integration
          │
          ▼
        Testing
          │
          ▼
      Competition
```

This process required continuous iteration between the mechanical, electrical, and embedded subsystems.

---

## Future Improvements

Several improvements could be implemented in a future revision of PokeBot.

### Electronics

- Integrate the motor driver directly into a revised custom PCB
- Add battery-voltage monitoring
- Add motor-current sensing
- Improve power distribution
- Add low-voltage battery protection
- Reduce the number of external modules
- Improve connector placement and serviceability

### Control

- Add wheel encoders
- Implement closed-loop wheel-speed control
- Add configurable speed profiles
- Implement acceleration limiting
- Improve turning control
- Add autonomous movement modes

### Sensors

- Add infrared reflectance sensors for ring-edge detection
- Add opponent-detection sensors
- Add current sensing for motor-load estimation
- Add telemetry for battery and motor status

### Mechanical Design

- Improve wheel traction
- Optimize center of gravity
- Improve weight distribution
- Reduce chassis mass
- Increase structural rigidity in high-impact areas
- Improve access to batteries and electronics
- Optimize the front geometry for sumo competition

### Autonomous Operation

A future version could transition from manual Bluetooth control to autonomous sumo operation by combining:

```text
Ring Sensors
     │
     ▼
    ESP32 ◄──── Opponent Sensors
     │
     ▼
Decision / Control Algorithm
     │
     ▼
Motor Driver
     │
     ▼
Differential Drive
```

This would allow the robot to detect the arena boundary, locate an opponent, and determine its own movement strategy without direct smartphone input.

---

## Project Outcome

PokeBot successfully progressed through the complete engineering development cycle:

```text
Concept
   ↓
System Architecture
   ↓
Electronic Design
   ↓
PCB Design
   ↓
Mechanical CAD
   ↓
3D Printing
   ↓
Firmware Development
   ↓
Manufacturing
   ↓
System Integration
   ↓
Testing
   ↓
Competition
```

The resulting platform was a:

**420 g**

**20 × 20 × 15 cm**

fully functional sumo robot combining:

- Custom PLA mechanical construction
- Fusion 360 CAD design
- ESP32 embedded control
- Bluetooth communication
- Smartphone-based control
- PWM DC motor control
- Two-motor differential drive
- Lithium-ion battery power
- Custom PCB design and fabrication
- 7.4 V → 5 V DC power conversion

The completed robot successfully won **2 of its 3 competition battles**, providing physical validation of the integrated mechanical, electronic, and embedded design.

---

## Documentation

This repository is intended to preserve the complete development process of PokeBot, including:

- Initial project proposal
- Electronic schematics
- PCB design
- PCB fabrication files
- ESP32 firmware
- Fusion 360 mechanical designs
- STL files
- Final robot photographs
- Competition results

> **Note:** The original proposal and schematic documents represent earlier stages of development. Some component selections and aspects of the power architecture changed during prototyping and integration. The specifications presented in the **Final Specifications** section describe the completed competition version of PokeBot.

---

## Author

**Marcos Rodrigo López Agustín**

Electrical & Mechatronics Engineering  
Universidad del Valle de Guatemala

---

## Disclaimer

This repository documents an academic engineering project developed for educational purposes.

The electrical schematics and initial design documentation represent the project's development process and may contain components or subsystem architectures that differ from the final physical implementation.

When reproducing or modifying the power system, appropriate lithium-ion battery charging, protection, and electrical safety practices should be followed.

---

## Final Result

**PokeBot**

`420 g` • `20 × 20 × 15 cm` • `ESP32` • `Bluetooth` • `Custom PCB` • `Fusion 360` • `PLA` • `2/3 Battles Won`
