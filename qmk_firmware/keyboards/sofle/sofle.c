// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include "quantum.h"

#ifdef SWAP_HANDS_ENABLE

__attribute__ ((weak))
const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] =
    // The LAYOUT macro could work for this, but it was harder to figure out the
    // identity using it.

    // This is the identity layout.
/*
{
    { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0} },
    { {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1} },
    { {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2} },
    { {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3} },
    { {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4} },

    { {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5} },
    { {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6} },
    { {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7} },
    { {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8} },
    { {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9} },
};
*/

    // This is the mirror, q <-> p, w <-> o, etc...
{
 { {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5} },
 { {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6} },
 { {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7} },
 { {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8} },
 { {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9} },

 { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0} },
 { {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1} },
 { {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2} },
 { {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3} },
 { {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4} },
};

#    ifdef ENCODER_MAP_ENABLE
const uint8_t PROGMEM encoder_hand_swap_config[NUM_ENCODERS] = {1, 0};
#    endif

#endif

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

static const char PROGMEM spin[][2] = {
    {0x95, 0},
    {0x96, 0},
    {0x97, 0},
    {0x98, 0}
};

static uint8_t current_frame = 0;
static uint32_t last_frame_time = 0;

static void render_name(void) {
    static const char PROGMEM s[] = {
        0x1E, 0x1F, 0
    };
    static const char PROGMEM a[] = {
        0x3E, 0x3F, 0
    };
    static const char PROGMEM n[] = {
        0x5E, 0x5F, 0
    };
    static const char PROGMEM t[] = {
        0x7E, 0x7F, 0
    };
    static const char PROGMEM o[] = {
        0xDE, 0xDF, 0
    };
    static const char PROGMEM l[] = {
        0xDC, 0xDD, 0
    };
    static const char PROGMEM u[] = {
        0xDA, 0xDB, 0
    };
    static const char PROGMEM c[] = {
        0xD8, 0xD9, 0
    };
    static const char PROGMEM smile[] = {
        0xC5, 0xC6, 0
    };

    oled_write_P(s, false);
    if (timer_elapsed(last_frame_time) > 100) {
        current_frame = (current_frame + 1) % 4;
        last_frame_time = timer_read();
    }
    oled_write_P(spin[current_frame], false);
    oled_write_ln_P(spin[current_frame], false);

    oled_write_P(a, false); oled_write_ln_P(l, false);
    oled_write_P(n, false); oled_write_ln_P(u, false);
    oled_write_P(t, false); oled_write_ln_P(c, false);
    oled_write_P(a, false); oled_write_ln_P(a, false);
    oled_write_P(t, false); oled_write_ln_P(s, false);
    oled_write_P(o, false); oled_write_ln_P(smile, false);
}

void print_status_narrow(void) {
    oled_write_P(PSTR("\n"), false);
    oled_write_P(PSTR("-----"), false);
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("0 TYP"), false);
            break;
        case 1:
            oled_write_P(PSTR("1 ALT"), false);
            break;
        case 2:
            oled_write_P(PSTR("2 MOV"), false);
            break;
        case 3:
            oled_write_P(PSTR("3 NUM"), false);
            break;
        case 4:
            oled_write_P(PSTR("Adjust"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("-----"), false);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_P(PSTR("CPSLK"), led_usb_state.caps_lock);
    oled_write_P(PSTR("-----"), false);

    oled_write_P(PSTR("\n"), false);
    render_name();
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (is_keyboard_master()) {
        print_status_narrow();
    }
    return true;
}

#endif

#ifdef ENCODER_ENABLE
bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return true;
}
#endif
