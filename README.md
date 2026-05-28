# Embedded Systems Course and Labs for students from Automation and Applied Informatics from Faculty of Automation, Computers and Electronics, University of Craiova

[![Run Tests](https://github.com/mamuleanu/embedded-systems-course-atmega328p/actions/workflows/tests.yml/badge.svg)](https://github.com/mamuleanu/embedded-systems-course-atmega328p/actions/workflows/tests.yml)


## Features

- **No Arduino Libraries**: Direct register manipulation for maximum control and efficiency.
- **Drivers:**: Modular, documented, and reusable.
    - **GPIO**: Initialization, Write, Read, Toggle.
    - **Interrupts**: External Interrupts (INT0, INT1) with callback support.
    - **Timer**: 1ms System Tick (`Millis()`) using Timer0 CTC mode.
    - **EEPROM**: Read, Write, Update (lifespan-aware).
    - **ADC**: Blocking 10-bit Analog-to-Digital conversion.
    - **PWM**: High-level wrapper for Timer1 (16-bit) and Timer2 (8-bit) PWM generation.
    - **I2C**: Hardware TWI driver at 400kHz for communication with I2C peripherals.
    - **Display**: SSD1306 OLED driver with frame buffer, 5x7 font, and text rendering.
- **Board Support Package (BSP)**: Pin mappings for **Arduino Nano** and **Uno**.
- **Robust Build System**: `Makefile` for compilation, flashing, and testing.
- **Host-Based Unit Testing**: Run unit tests on your computer without hardware using register mocking.
- **Code Coverage**: Generate HTML reports (`lcov`) to verify test coverage.

## Roadmap

- [x] GPIO driver
- [x] ADC driver
- [x] EEPROM driver
- [x] Interrupt driver
- [x] Timer driver
- [x] PWM driver
- [x] I2C driver
- [x] SSD1306 OLED display driver
- [ ] SPI driver
- [x] UART driver
- [ ] Unit tests

## Project Structure

```
├── bsp/            # Board definitions (uno.h, nano.h)
├── drivers/        # Hardware Abstraction Layer
│   ├── adc/
│   ├── eeprom/
│   ├── display/    # SSD1306 OLED driver
│   ├── gpio/
│   ├── i2c/        # Hardware TWI/I2C driver
│   ├── interrupt/
│   └── timer/
├── src/            # Application source code (main.c)
├── test/           # Unit tests & Mocks
│   ├── mocks/      # Mock AVR registers for host testing
│   ├── framework/  # Minimal test runner
│   └── test_*.c    # Unit test files
├── utils/          # Helper macros (BIT manipulations)
└── Makefile        # Build configuration
```

## Build & Flash

### Prerequisites
- `avr-gcc` toolchain
- `avrdude`
- `make`

### Commands
| Command | Description |
|---------|-------------|
| `make all BOARD=nano` | Compile the project for Arduino Nano. |
| `make flash` | Flash the firmware to the connected board. |
| `make clean` | Remove build artifacts. |

## Testing & Coverage

This project supports running unit tests on your host machine (Mac/Linux) by mocking the AVR hardware registers.

### Prerequisites (for coverage)
- `gcc`
- `lcov` (`brew install lcov`)

### Commands
| Command | Description |
|---------|-------------|
| `make test` | Compile and run all unit tests (GPIO, PWM) on the host. |
| `make coverage` | Run tests and generate usage metrics. |
| `make coverage-html` | Generate a visual HTML report of code coverage. |

![Code Coverage Example](/img/code_coverage_example.png)

## Safe Box Project

This repository includes a complete **AVR-based Safe Box** project built using the drivers above, without any Arduino libraries.

### Overview
A fully functional electronic safe box controlled by a 4x4 matrix keypad, with visual and audio feedback, an OLED display, and a servo motor as the locking mechanism. The servo motor controls a physical latch that locks and unlocks the box door. The entire circuit is mounted inside a box, making it as close as possible to a real safe box — the keypad and display are visible on the outside, while all the electronics are hidden inside.

### Features
- **Password protection** — 4-digit PIN set by the user on first boot
- **PIN change** — while the safe is open, press `*` to set a new PIN at any time
- **3 attempt limit** — after 3 wrong attempts, a 10-second lockout is triggered
- **10-second lockout** — RGB LED cycles through colors and a countdown is shown on the OLED display
- **OLED display** — shows current state, PIN entry progress, remaining attempts and countdown
- **Buzzer feedback** — short beep on keypress, success melody on correct PIN, alarm pattern on wrong PIN
- **Green LED** — lights up when the safe is unlocked
- **Red LED** — lights up on wrong password attempt
- **RGB LED** — color flow effect during lockout countdown

### Hardware Components
| Component | Pin |
|---|---|
| 4x4 Keypad (rows) | D2, D3, D4, D5 |
| 4x4 Keypad (cols) | D6, D7, D8, D10 |
| Servo motor | D9 |
| RGB Red | D11 |
| Red LED | D12 |
| Buzzer | A0 |
| RGB Blue | A1 |
| RGB Green | A2 |
| Green LED | A3 |
| OLED SDA | A4 |
| OLED SCL | A5 |

### How it works
1. On power-on the safe starts **unlocked** — press `*` to set your 4-digit PIN
2. Enter 4 digits then press `*` to confirm — the safe locks automatically
3. Enter the correct PIN to unlock
4. While unlocked, press `*` again to set a new PIN
5. After 3 wrong attempts the keypad locks for 10 seconds

### Drivers used
- `drivers/gpio/` — all digital I/O
- `drivers/pwm/` — servo motor control
- `drivers/i2c/` — OLED communication
- `drivers/display/` — SSD1306 text rendering
- `utils/delay.h` — timing


## Usage Example

```c
#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"

int main(void) {
    
    Timer0_Init();
    GPIO_Init(LED_BUILTIN, GPIO_OUTPUT);

    uint32_t last_time = 0;

    while (1) {
            
        if (Millis() - last_time >= 1000) {
            last_time = Millis();
            GPIO_Toggle(LED_BUILTIN);
        }
    }
}

// PWM Usage Example
#include "drivers/pwm/pwm.h"
#include "bsp/uno.h"

int pwm_example(void) {
    // 50Hz for Servo on D9
    PWM_Init(UNO_D9, 50);
    // 1.5ms pulse (approx neutral)
    // Duty cycle calculation: (1.5ms / 20ms) * ICR1_TOP
    // Wrapper takes 0-255: (1.5/20)*255 = ~19
    PWM_SetDutyCycle(UNO_D9, 19);

    // 1kHz LED Dimming on D11
    PWM_Init(UNO_D11, 1000);
    PWM_SetDutyCycle(UNO_D11, 128); // 50%
    
    return 0;
}
```

