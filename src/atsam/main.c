// Main starting point for SAM3/SAM4 boards
//
// Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h" // armcm_main
#include "board/irq.h" // irq_disable
#include "board/misc.h" // bootloader_request
#include "command.h" // DECL_COMMAND_FLAGS
#include "internal.h" // WDT
#include "sched.h" // sched_main

#define FREQ_PERIPH_DIV (CONFIG_MACH_SAME70 ? 2 : 1)
#define FREQ_PERIPH (CONFIG_CLOCK_FREQ / FREQ_PERIPH_DIV)

#define FREQ_SAME70_CAN 80000000

/****************************************************************
 * watchdog handler
 ****************************************************************/

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
    WDT->WDT_CR = 0xA5000001;
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
    uint32_t timeout = 500 * 32768 / 128 / 1000;  // 500ms timeout
    WDT->WDT_MR = WDT_MR_WDRSTEN | WDT_MR_WDV(timeout) | WDT_MR_WDD(timeout);
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
is_enabled_pclock(uint32_t id)
{
    if (id < 32)
        return !!(PMC->PMC_PCSR0 & (1 << id));
    else
        return !!(PMC->PMC_PCSR1 & (1 << (id - 32)));
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
enable_pclock(uint32_t id)
{
    if (id < 32)
        PMC->PMC_PCER0 = 1 << id;
    else
        PMC->PMC_PCER1 = 1 << (id - 32);
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
uint32_t
get_pclock_frequency(uint32_t id)
{
#if CONFIG_MACH_SAME70
    if (id == MCAN0_CLOCK_ID || id == MCAN1_CLOCK_ID)
        return FREQ_SAME70_CAN;
#endif
    return FREQ_PERIPH;
}


/****************************************************************
 * Resets
 ****************************************************************/

#if CONFIG_MACH_SAME70
#define RST_PARAMS ((0xA5 << RSTC_CR_KEY_Pos) | RSTC_CR_PROCRST)
#else
#define RST_PARAMS ((0xA5 << RSTC_CR_KEY_Pos) | RSTC_CR_PROCRST \
                    | RSTC_CR_PERRST)
#endif

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
    RSTC->RSTC_CR = RST_PARAMS;
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

#if CONFIG_MACH_SAM3X || CONFIG_MACH_SAM4S
#define EFC_HW EFC0
#elif CONFIG_MACH_SAM4E || CONFIG_MACH_SAME70
#define EFC_HW EFC
#endif

void noinline __aligned(16) // align for predictable flash code access
bootloader_request(void)
{
    irq_disable();
    // Request boot from ROM (instead of boot from flash)
    while ((EFC_HW->EEFC_FSR & EEFC_FSR_FRDY) == 0)
        ;
    EFC_HW->EEFC_FCR = (EEFC_FCR_FCMD_CGPB | EEFC_FCR_FARG(1)
                        | EEFC_FCR_FKEY_PASSWD);
    while ((EFC_HW->EEFC_FSR & EEFC_FSR_FRDY) == 0)
        ;
    // Reboot
    RSTC->RSTC_CR = RST_PARAMS;
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
static void
matrix_init(void)
{
    // The ATSAM sram is in a "no default master" state at reset
    // (despite the specs).  That typically adds 1 wait cycle to every
    // memory access.  Set it to "last access master" to avoid that.
    MATRIX->MATRIX_SCFG[0] = (MATRIX_SCFG_SLOT_CYCLE(64)
                              | MATRIX_SCFG_DEFMSTR_TYPE(1));
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
armcm_main(void)
{
    SystemInit();
    matrix_init();
    sched_main();
}
