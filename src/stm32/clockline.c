// Code to enable clock lines on stm32
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "internal.h" // struct cline

// Enable a peripheral clock
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
enable_pclock(uint32_t periph_base)
{
    struct cline cl = lookup_clock_line(periph_base);
    irqstatus_t flag = irq_save();
    *cl.en |= cl.bit;
    *cl.en; // Pause 2 cycles to ensure peripheral is enabled
    if (cl.rst) {
        // Reset peripheral
        *cl.rst = cl.bit;
        *cl.rst = 0;
    }
    irq_restore(flag);
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
int
is_enabled_pclock(uint32_t periph_base)
{
    struct cline cl = lookup_clock_line(periph_base);
    return *cl.en & cl.bit;
}
