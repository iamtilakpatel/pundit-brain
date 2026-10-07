# AGENTS.md

## Mission

Port the existing Ivy AI ESP-IDF firmware to **Pundit Brain** on the
**Waveshare ESP32-S3-Touch-AMOLED-2.16** while preserving the working
offline AI architecture.

Primary target:

- ESP32-S3
- 16 MB flash
- 8 MB PSRAM
- ESP-IDF
- fully local inference

Upstream:

https://github.com/iamankushpandit/esp_ai

Do not redesign working AI components merely to make the port cleaner.

---

## Current priority

Bring up the Waveshare hardware in small verified stages:

1. build + boot
2. PMIC
3. display
4. touch
5. microphone capture
6. 16 kHz mono PCM
7. microSD
8. WakeNet
9. STT
10. local summary/LLM
11. UI integration

Work on the earliest incomplete stage unless explicitly instructed otherwise.

---

## Architecture rule

Separate hardware adaptation from AI logic.

Preferred flow:

    Waveshare hardware/BSP
            ↓
    board abstraction
            ↓
    existing audio / storage interfaces
            ↓
    WakeNet → STT → summary/LLM

Preserve hardware-independent components whenever practical, especially:

- story_core
- story_stt
- story_llm
- story_tts
- model/inference code

Modify these only when required by a demonstrated compatibility issue.

---

## Target hardware

Replace Freenove-specific board support with Waveshare equivalents for:

- ES7210 microphone input
- AMOLED display
- touch controller
- microSD
- AXP2101 PMIC
- QMI8658 IMU when needed
- PCF85063 RTC when needed

Use existing Waveshare BSP/example code when available.

Do not invent pin assignments, register values, device addresses, APIs,
component versions, or board behavior.

Derive hardware facts from repository code, vendored dependencies, official
BSP/examples, or explicit user-provided documentation.

---

## Audio contract

The downstream speech pipeline expects:

- 16 kHz
- mono
- PCM

Adapt ES7210 capture to that contract at the board/audio boundary.

Do not change the STT neural-network implementation just because the physical
microphone hardware changed.

For stereo or dual-mic input, convert to the required downstream format before
passing audio to STT.

---

## Memory contract

Assume only **8 MB PSRAM**.

Large AI workloads must not be assumed to coexist.

Preserve sequential memory reuse:

    capture
    → STT
    → release/reuse large STT buffers
    → summary/LLM
    → release/reuse

Avoid:

- unnecessary framebuffers
- duplicate audio buffers
- large permanent allocations
- copying tensors when ownership can be transferred or reused

Prefer explicit ownership and lifetime for large buffers.

---

## UI constraint

UI must not compromise AI memory availability.

Prefer:

- procedural animation
- partial display buffers
- small assets
- low-cost state animations

Avoid:

- video/GIF playback
- unnecessary full-screen double buffering
- large decoded image assets in PSRAM

UI states should be event-driven, for example:

- IDLE
- WAKE
- LISTENING
- TRANSCRIBING
- SUMMARIZING
- SAVED
- ERROR

Do not build advanced UI before the AI/audio pipeline works.

---

## Wake word

Target wake phrase:

**Hey Pundit**

Until the custom model is available, preserve a compatible existing WakeNet
model for integration testing.

Do not fabricate or train a substitute wake-word model unless explicitly asked.

---

## Scope discipline

Make the **smallest correct change** that advances the current milestone.

Do not:

- rewrite unrelated code
- rename working APIs without need
- reformat entire files
- upgrade dependencies without a demonstrated requirement
- replace upstream architecture with a new framework
- add speculative abstractions
- implement future roadmap features early

Prefer adapting interfaces over rewriting consumers.

---

## Investigation discipline

Before editing unfamiliar code:

1. locate the relevant call path
2. identify hardware-specific boundaries
3. inspect only necessary files
4. state the intended minimal change
5. edit
6. build/test
7. inspect the diff

Use search before broad file reading.

Avoid spending context on:

- build output directories
- generated files
- binaries
- model weights
- vendored libraries unrelated to the task

unless required for the current issue.

---

## Validation

Never report hardware functionality as working unless it was actually verified
on hardware.

Distinguish clearly between:

- compiled
- booted
- observed on serial
- verified on hardware
- inferred from code
- not yet tested

A successful compile is not proof that peripheral hardware works.

When hardware is unavailable, stop at the strongest verifiable result.

---

## Build failures

When a build fails:

1. identify the first meaningful error
2. determine root cause
3. fix that cause
4. rebuild

Do not perform broad speculative edits in response to cascaded compiler errors.

---

## Git discipline

Work on feature branches, not `main`.

Keep commits small and single-purpose.

Before finishing a task, inspect:

```bash
git status
git diff
```

Do not commit:

- build artifacts
- secrets
- credentials
- generated binaries unless intentionally tracked
- unrelated changes

Do not push, merge, delete branches, rewrite history, or modify remotes unless
explicitly requested.

---

## Licensing

For inherited Ivy AI files:

- preserve all existing upstream copyright and license notices
- never replace upstream authorship with Pundit Brain authorship
- when materially modifying a file, retain upstream attribution and add
  Pundit Brain modification attribution where appropriate

For files created entirely for Pundit Brain:

- use the Pundit Brain copyright/license header
- do not add Ivy AI attribution unless the file contains copied or adapted
  upstream code

For third-party files:

- preserve their original notices and licensing

Do not modify without explicit instruction:

- `LICENSE`
- `LICENSE.exception`

---

## Documentation

Document facts that are implemented or measured.

Do not present planned behavior as completed behavior.

For experimental results, record:

- hardware
- configuration
- test method
- measured result

Keep README changes concise and evidence-based.

---

## Response style

Be concise.

For normal implementation work, report only:

1. what changed
2. why
3. validation performed
4. remaining blocker or next step

Do not repeat repository background unless it affects the decision.

For substantial architectural changes, explain the proposed change before
implementing it.

---

## Stop and ask

Stop before proceeding when:

- hardware documentation conflicts
- required pin/API information cannot be verified
- a change would alter the AI architecture substantially
- licensing implications are unclear
- dependency upgrades would cascade through the project
- destructive Git operations are required
- two plausible approaches have materially different tradeoffs

Otherwise, proceed with the smallest safe implementation.
