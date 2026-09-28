MAX32690EVKIT SW2 Counter with TFT and Buzzer
This project demonstrates a simple user-interface application for the Analog Devices MAX32690EVKIT. It uses the onboard TFT display to show how many times the SW2 push button has been pressed and provides audible feedback by generating a short tone on GPIO P2.26.

Each valid press of SW2 increments the displayed count, redraws the value on the onboard 128 × 128 TFT display, and produces an approximately 1 kHz beep lasting about 60 ms.

Features
Counts presses of the onboard SW2 push button.

Displays the live count on the onboard TFT.

Shows the title SW2 Count using a 16 × 16 font.

Displays the numeric count using a larger 28 × 28 font.

Uses a royal-blue background with white text.

Generates a 1 kHz audible feedback tone for approximately 60 ms.

Uses GPIO P2.26 as the buzzer output.

Prevents repeated counting while the button remains pressed.

Uses the MAX32690 instruction cache and internal precision oscillator clock source.

Hardware Required
Analog Devices MAX32690EVKIT.

MAX32625PICO debugger/programmer connected through the SWD header.

USB cable for the MAX32625PICO debugger.

USB cable or suitable power source for the MAX32690EVKIT.

Piezo buzzer connected to GPIO P2.26, if the buzzer is not already connected through your EV-kit setup.

Jumper wires, if using an external buzzer.

Hardware Connections
The application uses the following resources:

Resource	Connection / function
SW2	Onboard push button, read through PB_Get(0)
TFT display	Onboard ST7735-based color TFT
Buzzer	GPIO P2.26
Debug/programming	MAX32625PICO via SWD
Board support package	EvKit_V1
Buzzer connection
The code generates the buzzer waveform on:

text
MAX32690 GPIO: P2.26
For an external piezo buzzer:

text
P2.26  → Piezo buzzer positive terminal
GND    → Piezo buzzer negative terminal
Use a piezo buzzer suitable for GPIO drive. Do not connect a high-current magnetic buzzer or speaker directly to the GPIO pin. Use a transistor or MOSFET driver if the buzzer requires more current than the GPIO can safely supply.

How It Works
After startup, the firmware:

Waits two seconds for the board and peripherals to stabilize.

Initializes the MAX32690EVKIT board support package.

Enables the instruction cache.

Selects the internal precision oscillator as the system clock source.

Initializes the onboard TFT display.

Configures P2.26 as a digital output for the buzzer.

Displays SW2 Count and the initial count value of 0.

Continuously checks the state of SW2.

Increments the count whenever SW2 is pressed.

Updates the count on the TFT display.

Generates a short beep.

Waits for the button to be released before accepting the next press.

Buzzer Operation
The beep() function manually toggles GPIO P2.26 to create a square wave.

c
#define BEEP_FREQUENCY_HZ 1000
#define BEEP_DURATION_MS  60
The pin remains high for 500 µs and low for 500 µs:

text
HIGH for 500 µs
LOW  for 500 µs
This produces one complete waveform every 1 ms:

𝑓
=
1
1
 ms
=
1000
 Hz
f= 
1 ms
1
​
 =1000 Hz
The code repeats the high-low cycle 60 times, producing a 1 kHz tone for approximately 60 ms.

Display Output
At startup, the TFT shows:

text
SW2 Count
0
After pressing SW2 three times:

text
SW2 Count
3
The title uses the Liberation_Sans16x16 font, while the counter value uses the larger Liberation_Sans28x28 font.

Software Requirements
Analog Devices CodeFusion Studio with MSDK support, or the older Maxim/ADI MSDK Eclipse environment.

MAX32690 device support package.

MAX32690EVKIT board support package: EvKit_V1.

MAX32625PICO CMSIS-DAP debugger.

OpenOCD, included with CodeFusion Studio/MSDK.

The project uses these MSDK headers:

c
#include "board.h"
#include "gpio.h"
#include "icc.h"
#include "mxc_device.h"
#include "mxc_delay.h"
#include "pb.h"
#include "tft_st7735.h"
Building with CodeFusion Studio
Install Visual Studio Code.

Install the Analog Devices CodeFusion Studio extension.

Install the CodeFusion Studio SDK and configure its path in VS Code.

Open this project folder in VS Code.

Configure the workspace as a CodeFusion Studio project.

Select:

text
Target: MAX32690
Board: EvKit_V1
Platform: MSDK
Core: Arm Cortex-M4
Debugger: CMSIS-DAP
Debug interface: SWD
Build the project:

text
Ctrl + Shift + B
→ CFS: build
Connect the MAX32625PICO to the MAX32690EVKIT SWD header.

Power the MAX32690EVKIT.

Flash and start the program:

text
CodeFusion Studio
→ CFS: flash and run
Building with MSDK
If using the command-line MSDK workflow, open the MSDK terminal in the project directory and run:

bash
make TARGET=MAX32690 BOARD=EvKit_V1
Flash the built application with:

bash
make flash.openocd
Depending on the SDK version, the precise build target or board name may vary. Check the project Makefile and MSDK documentation if the command is rejected.

Code Structure
Function	Purpose
main()	Performs startup initialization, configures the TFT and buzzer GPIO, then starts the application
TFT_test()	Runs the button counter, updates the display, and triggers the beep
TFT_Print()	Convenience wrapper that prepares a text_t structure and prints text to the TFT
beep()	Generates the 1 kHz, 60 ms buzzer waveform by toggling GPIO P2.26
PB_Get(0)	Reads the onboard SW2 push button
Project Behavior
text
Power on / Reset
        ↓
Wait 2 seconds
        ↓
Initialize board, cache, clock, TFT, and buzzer GPIO
        ↓
Display “SW2 Count” and 0
        ↓
Wait for SW2 press
        ↓
Increment count
        ↓
Update TFT display
        ↓
Generate short buzzer tone
        ↓
Wait for SW2 release
        ↓
Repeat forever
Notes and Limitations
The buzzer is generated using software delays and GPIO toggling. During the 60 ms beep, the CPU is occupied and does not run other foreground tasks.

This approach is suitable for a simple user-interface demo. A production design should use a timer/PWM peripheral to generate the tone without blocking the CPU.

The application polls the push button rather than using a GPIO interrupt.

Waiting for button release prevents repeated counting during a long press, but it is not a full software debounce implementation.

The display is cleared and redrawn after every press. This is simple and reliable, but updating only the changed count region would be more efficient.

The comment saying “Set system clock to 100 MHz” should be verified against the active clock-tree configuration. MXC_SYS_CLOCK_IPO selects the internal precision oscillator, but the final core frequency depends on configured clock dividers and SDK settings.

License
This project is based on Analog Devices/Maxim Integrated MSDK components. Review the copyright and license terms included in the source files and the installed MSDK before redistributing modified versions.
