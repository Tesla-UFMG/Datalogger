/**
 * @file application.c
 * @author Felipe Telles (felipe.melo.telles@gmail.com)
 * @brief
 * @version 0.1
 * @date 04-08-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "application.h"

#include "SD.h"
#include "general_can.h"
#include "timer_handler.h"

static void application_state_machine(void);

typedef enum { SD_SAVE, CAN_POLL } state_e;

static state_e application_state  = CAN_POLL;
static uint32_t save_timer        = 0;

void application_run() {
    application_state_machine();
}

static void application_state_machine(void) {
    switch (application_state) {
        case CAN_POLL: {
            const HAL_StatusTypeDef status = CAN_poll();
            if (timer_wait_ms(save_timer, 10) && status == HAL_OK) {
                application_state = SD_SAVE;
            }

            break;
        }
        case SD_SAVE: {
            timer_restart(&save_timer);
            writeSD();
            application_state = SD_SAVE;

            break;
        }
    }
}
