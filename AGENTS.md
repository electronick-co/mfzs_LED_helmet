# Repository Guidelines

## Project Structure & Module Organization
- `Helmet_eye/Helmet_eye.ino` contains the primary Arduino sketch for the round TFT display animation.
- `Helmet_eye/animation_frames.h` holds the generated PNG byte arrays that drive the default animation loop.
- `Helmet_eye/background*.h` offer static fallback assets; keep them in sync with the display resolution (240×240).
- Root-level JPEGs (`devileye.jpg`, `l_eye.jpg`) are source art; use them when creating new frame sets.

## Build, Test, and Development Commands
- Use the Arduino IDE or `arduino-cli compile --fqbn <board_fqbn> Helmet_eye` to build the sketch. Update `<board_fqbn>` for the LVGL Xiao Round Screen (e.g., `Seeeduino:samd:seeed_xiao`).
- Upload via `arduino-cli upload --fqbn <board_fqbn> -p <port> Helmet_eye` once the board is connected.
- Monitor runtime logs with `arduino-cli monitor -p <port> -c baudrate=115200` to confirm frame progression and PNG decoding status codes.

## Coding Style & Naming Conventions
- Follow the existing two-space indentation and brace style used in `Helmet_eye.ino`.
- Prefer descriptive camelCase for functions (`pngDrawToSprite`) and snake_case for globals sourced from libraries (e.g., `animation_frame_count`).
- Keep asset arrays declared as `const uint8_t ... PROGMEM` to ensure they reside in flash.

## Testing Guidelines
- There are no automated tests; validate changes by flashing to hardware and verifying frame rate, color correctness, and sprite refresh stability.
- Use the serial monitor to check for `PNG_SUCCESS` and investigate decoding failures before finalizing changes.
- When adding new assets, confirm the sketch still fits within flash limits by reviewing the compile-size summary.

## Commit & Pull Request Guidelines
- Write concise, imperative commit messages (e.g., `Add animation frame timing controls`).
- For pull requests, include a summary of visual changes, commands used for verification, and any remaining risks (frame memory usage, flicker, etc.).
- Attach before/after photos or short clips of the display when altering animations to help reviewers validate the behavior without hardware access.

## Asset Preparation Workflow
- Convert source art to PNG frames with ImageMagick: `magick input.gif frame_%02d.png` or adjust brightness variations via `magick base.png -modulate <value>,100,100 frame.png`.
- Generate header files from frames using the existing Python approach (`python make_frames.py`) so arrays remain byte-aligned and PROGMEM-compatible.
- Keep animations to ≤10 frames or ~650 KB total to avoid exhausting flash on the target board.
