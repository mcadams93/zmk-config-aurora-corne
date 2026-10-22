# Aurora Corne ZMK Config

ZMK firmware configuration for the [splitkb Aurora Corne](https://splitkb.com/products/aurora-corne) with a **Cirque GlidePoint 40mm trackpad** on the right half.

**Board:** nice!nano v2 (nRF52840)
**Shield:** splitkb_aurora_corne (split keyboard, left + right)
**ZMK:** pinned to v0.3 (avoids Zephyr 4.1 Cirque driver conflict with halfdane's module)
**Layout:** Custom (Colemak-DHm inspired) with homerow mods

## Features

### Keymap
* 4 layers: Default, Symbol, Cursor, Lower, plus mouse keys on the thumb keys
* Balanced homerow mods (250ms tapping term, 150ms prior idle)
* Mouse click thumb keys via a `mouse_ht` hold-tap behavior (200ms tapping term) — see [Mouse Click Thumb Keys](#mouse-click-thumb-keys) below

### Trackpad (Right Half)
The right half has a 40mm Cirque GlidePoint trackpad connected via **SPI**, wired directly from the trackpad's test pads to the nice!nano's GPIO pins. No adapter or pull-up resistors needed.

The trackpad ships in SPI mode by default — no resistor changes needed on the trackpad PCB.

Gestures (via [halfdane's zmk-input-gestures](https://github.com/halfdane/zmk-input-gestures)):
* **Pointer Movement:** Inner 88% of the trackpad surface
* **Tap-to-Click:** Tap anywhere on the inner surface
* **Double-Tap:** Double-tap anywhere on the inner surface
* **Circular Rim Scrolling:** Trace the outermost 12% rim (clockwise = scroll down)
* **Inertial Cursor:** Flick and lift to glide (96% decay)

#### Input Scaling

Raw trackpad input is scaled down before reaching the host:
* **Mouse movement:** scaled to 1/2 speed via `&zip_xy_scaler 1 2`
* **Scroll:** scaled to 1/3 speed via `&zip_scroll_scaler 1 3`

#### Mouse Click Thumb Keys

Left-click, right-click, and click-and-drag are handled by the two outer thumb keys using a `mouse_ht` hold-tap behavior (200ms tapping term):
* **Left outer thumb key:** hold for left-click, tap for Escape
* **Right outer thumb key:** hold for right-click, tap for Enter

To select text: hold the left outer thumb key (left-click) and slide a finger on the trackpad to drag-select.

---

## Parts List

| Component | Purpose |
| :--- | :--- |
| **splitkb Aurora Corne kit** | Split keyboard with nice!nano v2 controllers |
| **40mm Cirque GlidePoint Trackpad** | Trackpad only, no adapter needed ([Keycapsss](https://keycapsss.com)) |
| **30 AWG silicone wire** | 7 wires for SPI + power + DR |

---

## Trackpad Wiring Guide

The trackpad (Cirque TM040040, 40mm) connects to the right half over **SPI1**. Three wires go to the nice!nano pins, and four wires go to the Aurora Corne **OLED header** (the OLED is not used on the right half). The trackpad end connects to the labeled test pads on the back of the Cirque trackpad PCB (see the [Cirque Pinnacle pinout reference](https://cirquepinnacle.readthedocs.io/en/latest/#pinout) and the [Aurora Corne schematic](https://docs.splitkb.com/doc/aurora_corne_rev1.pdf)).

| Trackpad pad | Solder point | nRF52840 GPIO | SPI Signal |
| :--- | :--- | :--- | :--- |
| DR | nice!nano pin 029 (D20) | P0.29 | DR |
| SS | nice!nano pin 002 (D19) | P0.02 | CS |
| SO | nice!nano pin 115 (D18) | P1.15 | MISO |
| SI | OLED header SDA | P0.17 (D2) | MOSI |
| SCK | OLED header SCL | P0.20 (D3) | SCK |
| VDD | OLED header VCC | — | 3.3V Power |
| GND | OLED header GND | — | Ground |

The wires form two groups:

* **nice!nano pins:** 029, 002, 115 (DR, SS, SO)
* **OLED header:** SDA, SCL, VCC, GND (SI, SCK, VDD, GND)

**Mounting orientation:** install the trackpad face up in its holder with the FFC connector at the 6 o'clock position (toward the thumb keys and the power switch, opposite the nice!nano USB connector). The test pads face down toward the nice!nano. Seen from above, DR, SS and SO sit at the right side of the pad group, top to bottom, directly above nice!nano pins 029, 002 and 115. The X/Y direction for this mounting has not been tested yet; adjust `invert-x` / `invert-y` in the right overlay if needed.

> **Caution:** do **not** install R1 and R2 (the 4.7kΩ SDA/SCL pull-up resistors) on the right half, and do not install the OLED display, encoder or RGB LEDs there — they share pins with the trackpad. The firmware disables the I2C bus (`&pro_micro_i2c`) on the right half.

> **Note:** the schematic shows the OLED header order as SDA, SCL, VCC, GND. Check the order on the PCB silkscreen before soldering, and do a continuity test on each pad first.

Alternatively, SCK and SI can go to nice!nano pins D3 (P0.20) and D2 (P0.17) instead of the OLED header; the firmware is the same.

---

## Firmware Files

| File | Purpose |
| :--- | :--- |
| `config/west.yml` | West manifest with ZMK v0.3 + halfdane modules (gestures, input processors, cirque driver) |
| `config/splitkb_aurora_corne.conf` | Shared Kconfig: enables SPI, pointing, Cirque driver |
| `config/splitkb_aurora_corne.keymap` | Shared keymap (4 layers, homerow mods) |
| `config/splitkb_aurora_corne_right.overlay` | Right-half overlay: SPI1 mapped to the nice!nano trackpad pins, gesture config |
| `config/mouse.dtsi` | Legacy mouse key emulation (currently unused) |

---

## Flashing

1. Push to GitHub — the Actions workflow builds both halves automatically.
2. Download `firmware.zip` from the Actions artifacts.
3. Extract to find two UF2 files (left and right).
4. For each nice!nano: connect via USB-C, double-tap the reset button, drag the matching UF2 onto the mounted drive.
5. Pair via Bluetooth.
