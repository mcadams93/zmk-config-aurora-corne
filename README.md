# Aurora Corne ZMK Config

ZMK firmware configuration for the [splitkb Aurora Corne](https://splitkb.com/products/aurora-corne) with a **Cirque GlidePoint 40 mm trackpad** on the right half and a **nice!view** display on the left half.

**Board:** nice!nano v2 (nRF52840), built as `nice_nano//zmk`
**Shield:** `splitkb_aurora_corne_left` (+ `nice_view_adapter nice_view`) / `splitkb_aurora_corne_right`
**ZMK:** pinned to commit [`cb615aee`](https://github.com/zmkfirmware/zmk/commit/cb615aee9a7c427e251f2ab62080c1a44a165da9) (`config/west.yml`)
**Layout:** Custom (Colemak-DHm inspired) with positional home-row mods; left half is the BLE central, right half is the peripheral

## Features

### Keymap

| # | Layer | How to reach it |
| :--- | :--- | :--- |
| 0 | Default | Base layer; `&to 0` from the inner thumb key on Symbol/Cursor |
| 1 | Symbol | Hold Space (right inner thumb); or tap the right thumb on Lower (`&to 1`) |
| 2 | Cursor | Hold Backspace (left middle thumb); or tap the left thumb on Lower (`&to 2`) |
| 3 | Lower (numbers) | Hold Delete or Tab (the two innermost thumbs) |

**Home-row mods** (C I E A / H T S N = Ctrl Alt Cmd Shift / Shift Cmd Alt Ctrl):
* Balanced hold-tap, 250 ms tapping term, 150 ms quick-tap, 150 ms `require-prior-idle-ms`.
* Positional: a mod only triggers when the next key is on the opposite hand or a thumb (`hold-trigger-key-positions`, `hold-trigger-on-release`).
* **S** (Alt) and **N** (Ctrl) on the right use `hmr_slow` with a 300 ms tapping term to avoid misfires on slow rolls.

**Mouse thumb keys** (`mouse_ht`, balanced, 200 ms):
* Left outer thumb: tap = Esc, hold = left click
* Right outer thumb: tap = Enter, hold = right click

**Media keys** on the Cursor layer, on the default-layer positions of L D W V:

| Key | Action |
| :--- | :--- |
| L | Mute |
| D | Play/Pause |
| W | Volume − |
| V | Tap = Next track, hold = Previous track |

The V hold sends a single *tap* of Previous (`prev_tap` macro), because holding Previous on macOS rewinds instead of skipping.

The Cursor layer also has arrows, Home/End/PgUp/PgDn, undo/redo/cut/copy/paste/find, Mission Control, and Bluetooth (`BT_CLR_ALL`, `BT_SEL 0`, `BT_SEL 1`).

### Trackpad (Right Half)

* Zephyr's built-in `cirque,pinnacle` driver over SPI, in **relative** mode (smooth over BLE).
* Sensitivity **2x**. 4x lost touches on battery (floating ground when not held / not on USB); 1x was erratic.
* Axes are swapped and both inverted on the right half (`zip_xy_transform`) before the data is relayed.
* `zmk,input-split` relays input to the listener on the left (central) half.
* **Tap → one short click** (20 ms) via this repo's custom `tap-click` input processor. It never holds a button, so taps can't cause accidental drag selections.
* **Drag / select:** hold the Esc thumb key (left click) and move a finger on the trackpad.
* **Cursor layer (hold Backspace):** finger movement scrolls (1/16 speed, vertical inverted); tap = right click.
* Cirque hardware sleep after ~5 s without touch (`sleep-mode-enable`); 3 idle packets after lift.

### Display (Left Half)

A nice!view (Sharp memory LCD) on the left half only, with a custom status screen adapted from ZMK's nice!view widgets:
* **Battery** for both halves, bar + percentage: **L** (with charging indicator) and **R** (shows `--` until the right half reports; full charge shows `100`)
* **Output** status icon (USB / BLE connected / BLE disconnected / unpaired)
* **Bluetooth profiles** 1 and 2 (`BT_SEL 0` / `BT_SEL 1`) as circles: solid ring = connected, dashed = paired, filled = active
* **Modifier row** `S C A G` (Shift, Ctrl, Alt, Cmd/GUI); a box is filled while that modifier (left or right) is held, including home-row mods
* **Active layer** name

---

## Parts List

| Component | Purpose |
| :--- | :--- |
| **splitkb Aurora Corne kit** | Split keyboard PCB and case |
| **2× nice!nano v2** | Controllers |
| **2× LiPo batteries** | 110 mAh used in this build |
| **Cirque TM040040 (40 mm GlidePoint)** | Trackpad, right half, no adapter needed ([Keycapsss](https://keycapsss.com)) |
| **nice!view** | Display, left half |
| **30 AWG silicone wire** | 7 wires for the trackpad, 1 bodge wire for nice!view CS |

No RGB LEDs are fitted (their pins are reused, see below).

---

## Trackpad Wiring Guide

The trackpad (Cirque TM040040, 40 mm) connects to the right half over **SPI1**. Three wires go to the nice!nano pins, and four wires go to the Aurora Corne **OLED header** (no OLED on the right half). The trackpad end connects to the labeled test pads on the back of the Cirque PCB (see the [Cirque Pinnacle pinout reference](https://cirquepinnacle.readthedocs.io/en/latest/#pinout) and the [Aurora Corne schematic](https://docs.splitkb.com/doc/aurora_corne_rev1.pdf)).

| Trackpad pad | Solder point | nRF52840 GPIO | SPI Signal |
| :--- | :--- | :--- | :--- |
| DR | nice!nano pin 029 (D20) | P0.29 | DR |
| SS | nice!nano pin 002 (D19) | P0.02 | CS |
| SO | nice!nano pin 115 (D18) | P1.15 | MISO |
| SI | OLED header SDA | P0.17 (D2) | MOSI |
| SCK | OLED header SCL | P0.20 (D3) | SCK |
| VDD | OLED header VCC | — | 3.3 V Power |
| GND | OLED header GND | — | Ground |

The wires form two groups:

* **nice!nano pins:** 029, 002, 115 (DR, SS, SO)
* **OLED header:** SDA, SCL, VCC, GND (SI, SCK, VDD, GND)

**Mounting orientation:** install the trackpad face up in its holder with the FFC connector at the 6 o'clock position (toward the thumb keys and the power switch, opposite the nice!nano USB connector). The test pads face down toward the nice!nano. Seen from above, DR, SS and SO sit at the right side of the pad group, top to bottom, directly above nice!nano pins 029, 002 and 115. The firmware's axis swap/invert matches this orientation.

> **Caution:** do **not** install R1 and R2 (the 4.7 kΩ SDA/SCL pull-ups) on the right half, and do not install an OLED, encoder or RGB LEDs there — they share pins with the trackpad. The right overlay disables `&pro_micro_i2c` (its SDA/SCL pins are the trackpad MOSI/SCK) and `&right_encoder` (the shield puts it on P0.02/P1.15, the trackpad CS/MISO).

> **Note:** the schematic shows the OLED header order as SDA, SCL, VCC, GND. Check the order on the PCB silkscreen before soldering, and do a continuity test on each pad first.

Alternatively, SCK and SI can go to nice!nano pins D3 (P0.20) and D2 (P0.17) instead of the OLED header; the firmware is the same.

---

## nice!view Wiring Guide (Left Half)

The nice!view uses SPI and has 5 pins; the Aurora OLED socket has 4. Four pins go in the OLED socket, CS needs a bodge wire.

| nice!view pin | Solder point on left half | Pro Micro pin | nRF52840 GPIO |
| :--- | :--- | :--- | :--- |
| GND | OLED header GND | — | — |
| VCC | OLED header VCC (3.3 V) | — | — |
| SCL/SCK | OLED header SCL | D3 | P0.20 |
| SDA/MOSI | OLED header SDA | D2 | P0.17 |
| CS | bodge wire to nice!nano pin D1 (labelled 006 / TX) | D1 | P0.06 |

Steps:
1. Mount the nice!view's four matching pins in the OLED socket (cut a 5-pin header/socket down to 4 if needed).
2. Run a short wire from the nice!view CS pin to nice!nano pin D1.

> **Caution:** check the pin order against the [nice!view silkscreen / pinout](https://nicekeyboards.com/docs/nice-view/pinout-schematic) **and** the Aurora OLED header silkscreen before soldering — the order was not verified for this build. Do a continuity test.

> **Caution:** D1 is the Aurora's RGB underglow data line. Do **not** fit underglow LEDs; the left overlay disables `&spi3` (underglow SPI). The nice!view is 3.3 V only.

Alternative: put CS on D0 (P0.08) instead and add to `config/splitkb_aurora_corne_left.overlay`:

```dts
&nice_view_spi { cs-gpios = <&pro_micro 0 GPIO_ACTIVE_HIGH>; };
```

See the [nice!view getting started guide](https://nicekeyboards.com/docs/nice-view/getting-started).

---

## Firmware Files

| File | Purpose |
| :--- | :--- |
| `build.yaml` | CI build matrix: left (+ `nice_view_adapter nice_view`), right, `settings_reset` |
| `.github/workflows/build.yml` | GitHub Actions workflow building the matrix |
| `config/west.yml` | West manifest pinning ZMK to `cb615aee` |
| `config/splitkb_aurora_corne.conf` | Shared Kconfig: pointing, SPI, Pinnacle driver, stack sizes, sleep, BLE tuning |
| `config/splitkb_aurora_corne_left.conf` | Left only: display, peripheral battery fetching, stock nice!view widget off |
| `config/splitkb_aurora_corne.dtsi` | Shared devicetree: `zmk,input-split` device, `tap_click` processor, trackpad listener + scroll layer |
| `config/splitkb_aurora_corne_left.overlay` | Enables the listener; disables `spi3` (underglow) for nice!view CS |
| `config/splitkb_aurora_corne_right.overlay` | SPI1 pins, Cirque Pinnacle node, XY transform, disables `pro_micro_i2c` and `right_encoder` |
| `config/splitkb_aurora_corne.keymap` | Keymap: 4 layers, home-row mods, mouse/media hold-taps |
| `Kconfig`, `CMakeLists.txt`, `zephyr/module.yml` | Makes the repo a Zephyr module; options `ZMK_INPUT_PROCESSOR_TAP_CLICK` and `ZMK_CORNE_STATUS_SCREEN` |
| `src/input_processor_tap_click.c` | `tap-click` input processor: touch → one short click |
| `dts/bindings/input_processors/zmk,input-processor-tap-click.yaml` | Binding for it (`click-ms`, default 20) |
| `src/display/custom_status_screen.c`, `src/display/widgets/*` | Custom nice!view status screen (adapted from ZMK) |
| `boards/shields/.gitkeep` | Placeholder for the module's board root |

---

## Flashing

1. Push to GitHub — Actions builds all targets.
2. Download `firmware.zip` from the run's artifacts. It contains:
   * `splitkb_aurora_corne_left nice_view_adapter nice_view` UF2 → left half
   * `splitkb_aurora_corne_right` UF2 → right half
   * `settings_reset` UF2
3. Connect a nice!nano via USB-C, double-tap reset, drag the UF2 onto the mounted drive.

Which half to flash:
* Keymap, display, trackpad listener/scroll/tap-click → **left**
* Trackpad driver settings or right overlay → **right**

Re-pairing: flash `settings_reset` to both halves, then the normal firmware, and "Forget" the keyboard in macOS Bluetooth settings before pairing again.

The right (peripheral) half only wakes from sleep on its own key presses.

---

## Troubleshooting

* **Trackpad only works when held or on USB:** grounding/sensitivity issue; keep sensitivity at 2x.
* **No trackpad at all:** check that the right overlay still disables `pro_micro_i2c` and `right_encoder`, then check the wiring.
* **Stuck selection:** shouldn't happen with tap-click; hold and release the Esc thumb key to release the left button.
* **Trackpad lags / stops on battery:** charge the battery; the power switch must be ON while charging.
