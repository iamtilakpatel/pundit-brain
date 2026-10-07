# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright (C) 2026 iamankushpandit <https://github.com/iamankushpandit>
# SPDX-FileCopyrightText: Copyright (C) 2026 Tilak Patel <https://github.com/iamtilakpatel>
#
# Originally part of Ivy AI -- https://github.com/iamankushpandit/esp_ai
# Modified for Pundit Brain -- https://github.com/iamtilakpatel/pundit-brain
#
# Free software under GPL-3.0-or-later, with the Espressif SDK linking
# exception in LICENSE.exception.
# See LICENSE, LICENSE.exception, NOTICE.md and THIRD_PARTY.md.

if(CONFIG_BOARD_WAVESHARE_AMOLED_2_16)
    # No peripheral implementation is available for this board yet.
    idf_component_register(INCLUDE_DIRS "include")
    return()
endif()

idf_component_register(
    SRCS "board_i2c.c" "board_lcd.c" "board_touch.c" "board_audio.c" "board_sd.c" "board_battery.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_driver_i2c esp_driver_spi esp_driver_gpio esp_driver_ledc esp_driver_i2s
             esp_driver_sdmmc esp_lcd fatfs sdmmc esp_adc log)
