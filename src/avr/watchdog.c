// Initialization of AVR watchdog timer.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/interrupt.h> // WDT_vect
#include <avr/wdt.h> // wdt_enable
#include "command.h" // shutdown
#include "irq.h" // irq_disable
#include "sched.h" // DECL_TASK

static uint8_t watchdog_shutdown;

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
ISR(WDT_vect)
{
    watchdog_shutdown = 1;
    shutdown("Watchdog timer!");
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
void
watchdog_reset(void)
{
    wdt_reset();
    if (watchdog_shutdown) {
        WDTCSR = 1<<WDIE;
        watchdog_shutdown = 0;
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
    // 0.5s timeout, interrupt and system reset
    wdt_enable(WDTO_500MS);
    WDTCSR = 1<<WDIE;
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

// Very early reset of the watchdog
void __attribute__((naked)) __visible __section(".init3")
watchdog_early_init(void)
{
    MCUSR = 0;
    wdt_disable();
}

// Support reset on AVR via the watchdog timer
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
command_reset(uint32_t *args)
{
    irq_disable();
    wdt_enable(WDTO_15MS);
    for (;;)
        ;
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
DECL_COMMAND_FLAGS(command_reset, HF_IN_SHUTDOWN, "reset");
