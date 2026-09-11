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

All 7 wires run **directly from the right-half nice!nano to the trackpad** — no Aurora Corne PCB pads are involved. The trackpad end connects to the labeled test pads on the back of the Cirque trackpad PCB (see the [Cirque Pinnacle pinout reference](https://cirquepinnacle.readthedocs.io/en/latest/#pinout) for a photo).

| nice!nano pin | nRF52840 GPIO | SPI Signal | Trackpad pad |
| :--- | :--- | :--- | :--- |
| D3 | P0.20 | SCK | SCK (FFC pin 1) |
| D2 | P0.17 | MOSI | SI (FFC pin 5) |
| D20 | P0.29 | MISO | SO (FFC pin 2) |
| D19 | P0.02 | CS | SS (FFC pin 3) |
| D0 | P0.08 | DR | DR (FFC pin 4) |
| VCC | — | 3.3V Power | VDD (FFC pin 12) |
| GND | — | Ground | GND (FFC pin 11) |

The pins are split across both sides of the nice!nano:

* **Left header:** D0, D2, D3
* **Right header:** D19, D20
* **Right header, top:** VCC, GND

Because the wires land on the nice!nano itself, solder them before seating the controller in its sockets (or lift it out first) so you have access to both pin headers.

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
