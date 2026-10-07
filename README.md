# Pundit Brain

An experiment in building a **fully offline wearable second brain** on the ESP32-S3.

Pundit Brain is an early-stage project to capture conversations, transcribe speech locally, summarize what matters, and later let me search or ask questions about those memories, all without an internet connection.

> **Status: early development.** Currently preparing the board-layer port from the Freenove FNK0104B to Waveshare ESP32-S3 hardware. Nothing here is ready to flash on the target board yet.

**Target board:** [Waveshare ESP32-S3-Touch-AMOLED-2.16](https://www.waveshare.com/esp32-s3-touch-amoled-2.16.htm)  
ESP32-S3R8, 2.16" 480×480 AMOLED touch display, dual microphones, Wi-Fi + BLE.

**Target wake phrase:** **"Hey Pundit."** The upstream project currently uses **"Hey Ivy,"** which will remain as a fallback until the custom wake-word model is working reliably.

## Why I'm building this

Most AI assistants depend on a phone, cloud API, or remote server. I wanted to explore a harder question:

> **How much of a useful personal memory system can run entirely on a small microcontroller with only 8 MB of PSRAM?**

I'm not trying to build a miniature ChatGPT. I want Pundit Brain to become good at a small set of useful tasks:

- remembering conversations
- converting speech to text locally
- summarizing what happened
- storing transcripts and important information
- eventually searching past memories
- eventually answering simple questions about what I previously recorded
- working when Wi-Fi and cellular service are unavailable

A later version may use Wi-Fi only for optional backup or export, such as to Google Drive. The AI processing is intended to stay local.

## Project origin

Pundit Brain began as a fork of [Ivy AI](https://github.com/iamankushpandit/esp_ai) by [iamankushpandit](https://github.com/iamankushpandit).

Ivy AI shows that open-vocabulary speech recognition and a small local language model can run completely on an ESP32-S3 with 8 MB of PSRAM.

I am using Ivy AI as the technical foundation while steering the project toward a different goal: a wearable offline memory system.

### Upstream Ivy AI provides

- Espressif WakeNet wake-word detection
- offline Conformer CTC speech-to-text
- local 4-bit language-model inference
- local text-to-speech
- microSD-backed model storage
- shared memory between STT and LLM phases
- ESP-IDF firmware

These are upstream capabilities unless this repository specifically states otherwise.

### What I'm changing

The upstream project already proves that offline STT and a small local language model can run on the ESP32-S3. My work will focus on:

- porting the firmware to wearable-oriented Waveshare ESP32-S3 hardware
- changing the wake phrase to **"Hey Pundit"**
- using the board's dual microphones
- recording conversations to microSD
- processing longer recordings locally
- generating a simple local summary, starting with lightweight extractive methods

Later, I want to explore structured memories, local search, and an **Ask My Memory** mode.

## V1 scope

To keep the first version realistic and finishable, V1 is deliberately small:

1. Wake phrase: **"Hey Pundit"**
2. Record audio to microSD
3. Transcribe speech locally
4. Generate a simple local summary
5. Save the transcript and summary to microSD

**About the summary:** I'll start with lightweight extractive methods such as keyword and sentence scoring, and only use a small language model if testing shows that it is reliable. For a memory device, preserving facts matters more than generating fluent text.

Everything beyond that is a stretch goal after the basic pipeline works.

### Planned V1 system

```text
Dual microphones
       │
       ▼
   Local VAD
       │
       ▼
  "Hey Pundit"
       │
       ▼
  Record audio
       │
       ▼
    microSD
       │
       ▼
   Local STT
       │
       ▼
   Transcript
       │
       ▼
 Local summary
   processing
       │
       ▼
Transcript + Summary
       │
       ▼
    microSD
```

No cloud AI service is required for the planned V1 system.

## Why the ESP32-S3?

The target hardware has:

- **8 MB PSRAM**
- **16 MB flash**
- two CPU cores running up to 240 MHz

That is tiny compared with the hardware normally used for language models, so the challenge is designing the whole system around those limits.

One technique from Ivy AI that I plan to keep is reusing memory between AI stages:

```text
STT phase:
  use PSRAM for speech recognition
  → write the transcript to storage
  → release memory

LLM / summary phase:
  reuse the same PSRAM
  → process the transcript
  → write the summary to storage
```

If V1 uses a language model for summarization, the speech model and language model will not need to remain in memory at the same time.

## Roadmap

### 1. Hardware port

- [ ] Build and flash the Waveshare ESP-IDF examples
- [ ] Verify display, touch, microSD, and power management
- [ ] Verify both microphones
- [ ] Record 16 kHz PCM audio to microSD

### 2. Offline speech

- [ ] Port Ivy's Conformer STT pipeline
- [ ] Feed microphone audio into the STT engine
- [ ] Transcribe speech locally
- [ ] Measure latency and accuracy

### 3. Pundit V1

- [ ] Start the custom **"Hey Pundit"** WakeNet model request early, in parallel with the hardware port
- [ ] Keep **"Hey Ivy"** working as a fallback in the meantime
- [ ] Integrate the custom wake word once available
- [ ] Record longer conversations
- [ ] Generate a simple local summary, extractive first and small LM only if reliable
- [ ] Save transcript + summary to microSD

### Later

- structured memory extraction
- decisions and action items
- local text search
- **Ask My Memory**
- touchscreen memory browser
- battery optimization
- wearable enclosure
- optional Wi-Fi backup/export

## Questions I want to investigate

1. How accurate can open-vocabulary STT be on an ESP32-S3?
2. Within 8 MB of PSRAM, which produces more reliable conversation summaries: lightweight extractive methods or a tiny language model?
3. How should longer conversations be processed without losing important information?
4. What is the tradeoff between AI processing speed, accuracy, and battery life?

I plan to document both what works and what doesn't.

## Privacy and consent

Pundit Brain records speech, so privacy matters.

- The system is being designed so normal AI processing and storage stay **on the device**.
- No cloud AI service is required for the planned V1.
- Any future Wi-Fi backup or export will be optional.
- Recording laws vary by location. A device like this should only be used with the knowledge and consent of the people being recorded.

## Build status

**Not ready for the target hardware yet.**

The current source comes from Ivy AI and targets the Freenove FNK0104B.

I will add verified build and flash instructions here once the target Waveshare board can:

1. boot,
2. initialize the display,
3. access microSD,
4. capture microphone audio, and
5. run the offline STT pipeline.

Until then, see the [upstream Ivy AI repository](https://github.com/iamankushpandit/esp_ai) for the original build instructions.

## Attribution and license

Pundit Brain is based on the open-source [Ivy AI](https://github.com/iamankushpandit/esp_ai) project by [iamankushpandit](https://github.com/iamankushpandit).

Ivy AI provides the original ESP32-S3 offline AI architecture and implementation that this project is being adapted from.

### Inherited components and licenses

| Component | License |
|---|---|
| Ivy AI project code | GPL-3.0-or-later |
| Conformer STT code | Apache-2.0 |
| Conformer model weights | CC-BY-4.0 |
| SVOX Pico | Apache-2.0 |
| cardputer-ai GPT-Neo engine | MIT |
| Espressif ESP-SR | Espressif license for ESP chips |

Individual model weights may have their own dataset or usage restrictions.

See [`LICENSE`](LICENSE), [`NOTICE.md`](NOTICE.md), and the [upstream repository](https://github.com/iamankushpandit/esp_ai) for details.

This repository remains under the licensing requirements inherited from Ivy AI and its dependencies. New files and modifications will retain all required upstream copyright, license, and attribution notices.

Original Ivy AI code and documentation: Copyright © 2026 iamankushpandit.
