# Web Gamepad API Hardware Activator

As a measure against web browser fingerprinting, most web browsers seem to exclude gamepads from `navigator.getGamepads()` until one of their buttons (not axis) changes state. This can be undesirable in circumstances, where gamepad-based applications are exhibited in a public setting, i.e. where the gamepad (especially the gamepad/joystick axis) should be available immediately.

This sketch offers a hardware based solution to the problem by attaching an additional microcontroller to the controller board of the joystick in question: The microcontroller repeatedly simulates a button press by pulling the respective pin on the gamepad encoder board to either HIGH or LOW (depending on the configuration).

## Hardware

Currently, only the Arduino Nano hardware has been tested in combination with the EG Starts Zero Delay USB encoder. The Arduino Nano is powered via the 5V/GND pins of the encoder board to ensure that both boards operate at the same/similar electrical potentials (powering the Arduino through either its 5V or VIN pin should work reliably). The signal pin of the Arduino (pin 13 by default) is connected to one of the many button input pins of the encoder board (choose one that is not used in your application).

Exchanging the microcontroller for a different one may not work as easily since the sketch relies on the sleep capabilities of the ATmega328P micro controller.

When using a different gamepad encoder, it may be necessary to convert the logic level of the micro controller to that of the encoder, e.g. using a voltage divider.

## Build system

This project use the [PlatformIO](https://platformio.org/) tool to configure, build and flash the binaries for the microcontroller:

```shell
# Build the binary for the default environment
pio run

# Build and flash the binary to the micro controller for the default environment
pio run --target upload

# Build and flash the binary to the micro controller for the another environment
pio run --environment nanoatmega328 --target upload
```

## Configuration

The sketch exposes several configurable variables:

- `PIN_LED` (default: `LED_BUILTIN`, i.e. pin 13 on the Arduino Nano): The pin that the signalling LED is connected to.
- `PIN_BUTTON` (default: `PIN_LED`): The pin that the external gamepad controller board is connected to.
- `BUTTON_ACTIVE_DURATION` (default: 50): The duration of the signalling in milliseconds.
- `INVERT` (default: undefined): When defined, use LOW as the active state and HIGH otherwise.

These variables can be set during the build process via the PlatformIO command line, e.g.

```shell
PLATFORMIO_BUILD_FLAGS="-DINVERT -DBUTTON_ACTIVE_DURATION=100"  pio run --target upload
```

## Credits

Developed by Christian Stussak for IMAGINARY gGmbH.

## License

Copyright © 2023 IMAGINARY gGmbH

Licensed under the MIT license (see the [`LICENSE`](LICENSE) file).
