// Support setting gpio pins at mcu start
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_INITIAL_PINS
#include "board/gpio.h" // gpio_out_setup
#include "board/pgm.h" // READP
#include "ctr.h" // DECL_CTR
#include "initial_pins.h" // initial_pins
#include "sched.h" // DECL_INIT

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
DECL_CTR("DECL_INITIAL_PINS " __stringify(CONFIG_INITIAL_PINS));

/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
void
initial_pins_setup(void)
{
    if (sizeof(CONFIG_INITIAL_PINS) <= 1)
        return;
    int i;
    for (i=0; i<initial_pins_size; i++) {
        const struct initial_pin_s *ip = &initial_pins[i];
        gpio_out_setup(READP(ip->pin), READP(ip->flags) & IP_OUT_HIGH);
    }
}
/**
 * Transforms the sign-up request data to match the backend's expected format.
 *
 * @param {SignUpRequest} signUpData - The original sign-up request data.
 *
 * @returns {Object} The transformed sign-up request data with the following changes:
 * - `firstName` is mapped to `first_name`
 * - `lastName` is mapped to `last_name`
 * - `email` is mapped to `username`
 * - All other properties remain unchanged.
 */
DECL_INIT(initial_pins_setup);
