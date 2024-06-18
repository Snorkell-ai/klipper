// Commands for counting edges on GPIO input pins
//
// Copyright (C) 2021  Adrian Keet <arkeet@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "basecmd.h" // oid_alloc
#include "board/gpio.h" // struct gpio_in
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "command.h" // DECL_COMMAND
#include "sched.h" // DECL_TASK

struct counter {
    struct timer timer;
    uint32_t poll_ticks;
    uint32_t sample_ticks, next_sample_time;
    uint32_t count, last_count_time;
    uint8_t flags;
    struct gpio_in pin;
};

enum {
    CF_PENDING = 1,
};

static struct task_wake counter_wake;

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
static uint_fast8_t
counter_event(struct timer *timer)
{
    struct counter *c = container_of(timer, struct counter, timer);

    uint32_t time = c->timer.waketime;
    uint8_t last_value = c->count & 1;
    uint8_t value = gpio_in_read(c->pin);
    if (last_value != value) {
        c->count++;
        c->last_count_time = time;
    }
    // useful invariant: c->count & 1 == value

    if (timer_is_before(c->next_sample_time, time)) {
        c->flags |= CF_PENDING;
        c->next_sample_time = time + c->sample_ticks;
        sched_wake_task(&counter_wake);
    }

    c->timer.waketime += c->poll_ticks;
    return SF_RESCHEDULE;
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
command_config_counter(uint32_t *args)
{
    struct counter *c = oid_alloc(
        args[0], command_config_counter, sizeof(*c));
    c->pin = gpio_in_setup(args[1], args[2]);
    c->timer.func = counter_event;
}
DECL_COMMAND(command_config_counter,
             "config_counter oid=%c pin=%u pull_up=%c");

void
command_query_counter(uint32_t *args)
{
    struct counter *c = oid_lookup(args[0], command_config_counter);
    sched_del_timer(&c->timer);
    c->timer.waketime = args[1];
    c->poll_ticks = args[2];
    c->sample_ticks = args[3];
    c->next_sample_time = c->timer.waketime; // sample immediately
    sched_add_timer(&c->timer);
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
DECL_COMMAND(command_query_counter,
             "query_counter oid=%c clock=%u poll_ticks=%u sample_ticks=%u");

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
counter_task(void)
{
    if (!sched_check_wake(&counter_wake))
        return;

    uint8_t oid;
    struct counter *c;
    foreach_oid(oid, c, command_config_counter) {
        if (!(c->flags & CF_PENDING))
            continue;
        irq_disable();
        uint32_t waketime = c->timer.waketime;
        uint32_t count = c->count;
        uint32_t count_time = c->last_count_time;
        c->flags &= ~CF_PENDING;
        irq_enable();
        sendf("counter_state oid=%c next_clock=%u count=%u count_clock=%u",
              oid, waketime, count, count_time);
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
DECL_TASK(counter_task);
