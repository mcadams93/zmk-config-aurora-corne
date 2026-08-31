# Aurora Corne ZMK Config

ZMK firmware configuration for the [splitkb Aurora Corne](https://splitkb.com/products/aurora-corne) with a **Cirque GlidePoint 40mm trackpad** on the right half.

**Board:** nice!nano v2 (nRF52840)
**Shield:** splitkb_aurora_corne (split keyboard, left + right)
**ZMK:** pinned to v0.3 (avoids Zephyr 4.1 Cirque driver conflict with halfdane's module)
**Layout:** Custom (Colemak-DHm inspired) with homerow mods

## Features

### Keymap
* 4 layers: Default, Symbol, Cursor, Lower
* Balanced homerow mods (280ms tapping term, 150ms prior idle)

### Trackpad (Right Half)
The right half has a 40mm Cirque GlidePoint trackpad connected via I2C, sharing the bus with the OLED display (different I2C addresses: trackpad at `0x2A`, OLED at `0x3C`).

Gestures (via [halfdane's zmk-input-gestures](https://github.com/halfdane/zmk-input-gestures)):
* **Pointer Movement:** Inner 88% of the trackpad surface
* **Tap-to-Click:** Tap anywhere on the inner surface
* **Double-Tap:** Double-tap anywhere on the inner surface
* **Right-Click:** Tap in the bottom-right quadrant
* **Circular Rim Scrolling:** Trace the outermost 12% rim (clockwise = scroll down)
* **Inertial Cursor:** Flick and lift to glide (96% decay)

---

## Parts List

| Component | Purpose |
| :--- | :--- |
| **splitkb Aurora Corne kit** | Split keyboard with nice!nano v2 controllers |
| **40mm Cirque GlidePoint Kit** | Trackpad + I2C FFC adapter + cable ([Keycapsss](https://keycapsss.com)) |
| **30 AWG silicone wire** | One wire for the DR interrupt line |

No additional pull-up resistors, batteries, or controllers are needed — the Aurora Corne and the Keycapsss adapter provide everything.

---

## Trackpad Wiring Guide

The trackpad connects to the **right half** nice!nano v2 only.

### Step 1: Set I2C Mode on the Trackpad
The Cirque trackpad ships in SPI mode. Change it to I2C:
* Remove the resistor at **R1** (disables SPI)
* Bridge/close **R2** (enables I2C)

### Step 2: Connect the FFC Adapter
Connect the trackpad to the Keycapsss I2C FFC adapter using the included ribbon cable. Lift the black latch on both connectors, insert the cable (blue side facing up on the adapter, blue side facing the black dot on the trackpad), and press the latches closed.

The adapter includes built-in 4.7K ohm I2C pull-up resistors.

### Step 3: Wire the Adapter to the Aurora Corne
The adapter's 4-pin header has the same pin order as the Aurora Corne's OLED socket (verified from the [Aurora Corne schematic](https://docs.splitkb.com/doc/aurora_corne_rev1.pdf) and the [adapter schematic](https://github.com/keyboard-magpie/minimal-fpc-i2c-pcb)):

| Pin | Aurora Corne OLED header | Keycapsss adapter |
| :--- | :--- | :--- |
| 1 | GND | GND |
| 2 | VCC | VCC |
| 3 | SCL | SCL |
| 4 | SDA | SDA |

If you have OLED sockets installed, plug the adapter header directly into the right half's OLED socket — no soldering or extra wiring needed for I2C.

If you're also using an OLED display on the right half, you can't use both in the same socket. In that case, solder wires from the adapter header to the nice!nano v2 pins directly: **Pin 1** (square pad) to **GND**, **Pin 2** to **VCC**, **Pin 3** to **P0.20** (board label 20), **Pin 4** to **P0.17** (board label 17).

### Step 4: Wire the DR Interrupt Line
The FFC adapter does not carry the DR (Data Ready) signal. Solder one 30 AWG wire directly from the **DR** test pad on the back of the Cirque trackpad to **P0.31** (board label **31**) on the nice!nano v2.

The DR test pad is one of the larger copper circular pads on the back of the trackpad PCB, labeled "DR" (FFC pin 4). See the [Cirque Pinnacle pinout reference](https://cirquepinnacle.readthedocs.io/en/latest/#pinout) for a photo.

| Source | Signal | nice!nano v2 GPIO | Board label |
| :--- | :--- | :--- | :--- |
| Trackpad **DR** pad | Data Ready | **P0.31** | 31 |

### Pin Usage Summary (Right Half)

| nRF52840 GPIO | Board label | Used By |
| :--- | :--- | :--- |
| P0.08 | 008 | Free |
| P0.06 | 006 | RGB LED (WS2812) |
| P0.17 | 17 | **I2C SDA** (shared: OLED + trackpad) |
| P0.20 | 20 | **I2C SCL** (shared: OLED + trackpad) |
| P0.22 | 22 | Key matrix column |
| P0.24 | 24 | Key matrix column |
| P1.00 | 100 | Key matrix column |
| P0.11 | 011 | Key matrix column |
| P1.04 | 104 | Key matrix column |
| P1.06 | 106 | Key matrix column |
| P0.09 | 009 | Key matrix row |
| P1.11 | 111 | Key matrix row |
| P1.13 | 113 | Key matrix row |
| P0.10 | 010 | Key matrix row |
| P1.15 | 115 | Encoder B (free if no encoder) |
| P0.02 | 002 | Encoder A (free if no encoder) |
| P0.29 | 29 | Free |
| P0.31 | 31 | **Trackpad DR** (interrupt) |

---

## Firmware Files

| File | Purpose |
| :--- | :--- |
| `config/west.yml` | West manifest with ZMK v0.3 + halfdane modules (gestures, input processors, cirque driver) |
| `config/splitkb_aurora_corne.conf` | Shared Kconfig: enables I2C, pointing, Cirque driver |
| `config/splitkb_aurora_corne.keymap` | Shared keymap (4 layers, homerow mods) |
| `config/splitkb_aurora_corne_right.overlay` | Right-half devicetree overlay: Cirque trackpad on I2C0, DR on P0.31, gesture config |
| `config/mouse.dtsi` | Legacy mouse key emulation (currently unused) |

---

## Flashing

1. Push to GitHub — the Actions workflow builds both halves automatically.
2. Download `firmware.zip` from the Actions artifacts.
3. Extract to find two UF2 files (left and right).
4. For each nice!nano: connect via USB-C, double-tap the reset button, drag the matching UF2 onto the mounted drive.
5. Pair via Bluetooth.
