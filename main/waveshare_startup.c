// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright (C) 2026 Tilak Patel <https://github.com/iamtilakpatel>
//
// Part of Pundit Brain -- https://github.com/iamtilakpatel/pundit-brain
// See LICENSE, LICENSE.exception, NOTICE.md and THIRD_PARTY.md.

// Minimal Waveshare entry point; no board GPIOs or peripheral drivers used.
#include "esp_log.h"
#include "story_mem.h"

void app_main(void)
{
    ESP_LOGI("main", "Board: Waveshare ESP32-S3-Touch-AMOLED-2.16");
    story_mem_log("boot");
    ESP_LOGI("main", "PMIC, display, touch, audio, SD, IMU and RTC: uninitialized");
    ESP_LOGI("main", "UI, radio and AI pipeline: not started");
    ESP_LOGI("main", "Minimal startup complete; awaiting hardware bring-up");
}
