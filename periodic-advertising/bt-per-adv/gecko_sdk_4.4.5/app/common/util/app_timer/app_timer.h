/***************************************************************************//**
 * @file
 * @brief Timer service header
 *
 * Timer service for applications with less strict timing requirements.
 * This module is based on the sleeptimer, but runs the timer callbacks in
 * non-interrupt context. This behavior gives more flexibility for the callback
 * implementation but causes a less precise timing.
 *
 * @note If your application requires precise timing, please use the sleeptimer
 *       directly.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#ifndef APP_TIMER_H
#define APP_TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_sleeptimer.h"
#include "sl_power_manager.h"

// Forward declaration
typedef struct app_timer app_timer_t;

/***************************************************************************//**
 * Expected prototype of the user's callback function which is called when a
 * timer expires.
 *
 * @param timer Pointer to the timer handle.
 * @param data An extra parameter for the user application.
 ******************************************************************************/
typedef void (*app_timer_callback_t)(app_timer_t *timer, void *data);

/// Timer structure
struct app_timer {
  sl_sleeptimer_timer_handle_t sleeptimer_handle;
  app_timer_callback_t callback;
  void *callback_data;
  app_timer_t *next;
  bool triggered;
  bool periodic;
  uint32_t timeout_ms;
  uint16_t overflow_counter;
  uint16_t overflow_max;
};

/***************************************************************************//**
 * Start timer or restart if it is running already.
 *
 * @param[in] timer Pointer to the timer.
 * @param[in] timeout_ms Timer timeout, in milliseconds.
 * @param[in] callback Callback function that is called when timeout expires.
 * @param[in] callback_data Pointer to user data that will be passed to callback.
 * @param[in] is_periodic Reload timer when it expires if true.
 *
 * @return Status of the operation.
 ******************************************************************************/
sl_status_t app_timer_start(app_timer_t *timer,
                            uint32_t timeout_ms,
                            app_timer_callback_t callback,
                            void *callback_data,
                            bool is_periodic);

/***************************************************************************//**
 * Stop running timer.
 *
 * @param[in] timer Pointer to the timer.
 *
 * @return Status of the operation.
 ******************************************************************************/
sl_status_t app_timer_stop(app_timer_t *timer);

/***************************************************************************//**
 * Execute timer callback functions.
 *
 ******************************************************************************/
void sli_app_timer_step(void);

/***************************************************************************//**
 * Routine for power manager handler
 *
 * @return SL_POWER_MANAGER_WAKEUP if there is an unhandled timer event
 ******************************************************************************/
sl_power_manager_on_isr_exit_t sli_app_timer_sleep_on_isr_exit(void);

/***************************************************************************//**
 * Checks if it is ok to sleep now
 *
 * @return false if there is an unhandled timer event
 ******************************************************************************/
bool sli_app_timer_is_ok_to_sleep(void);

#endif // APP_TIMER_H
