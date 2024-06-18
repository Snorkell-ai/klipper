// Watchdog code on rp2040
//
// Copyright (C) 2021-2022  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include "hardware/structs/psm.h" // psm_hw
#include "hardware/structs/watchdog.h" // watchdog_hw
#include "sched.h" // DECL_TASK

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
watchdog_reset(void)
{
    watchdog_hw->load = 0x800000; // ~350ms
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
DECL_TASK(watchdog_reset);

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
watchdog_init(void)
{
    psm_hw->wdsel = PSM_WDSEL_BITS & ~(PSM_WDSEL_ROSC_BITS|PSM_WDSEL_XOSC_BITS);
    watchdog_reset();
    watchdog_hw->ctrl = (WATCHDOG_CTRL_PAUSE_DBG0_BITS
                         | WATCHDOG_CTRL_PAUSE_DBG1_BITS
                         | WATCHDOG_CTRL_PAUSE_JTAG_BITS
                         | WATCHDOG_CTRL_ENABLE_BITS);
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
DECL_INIT(watchdog_init);
