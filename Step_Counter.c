/******************************************************************************
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "gpio.h"
#include "icc.h"
#include "mxc_device.h"
#include "mxc_delay.h"
#include "led.h"
#include "pb.h"
#include "tft_st7735.h"

#define BEEP_FREQUENCY_HZ 1000
#define BEEP_DURATION_MS 60
#define BEEP_HALF_PERIOD_US (1000000 / (BEEP_FREQUENCY_HZ * 2))

static const mxc_gpio_cfg_t beep_pin = {
    MXC_GPIO2,
    MXC_GPIO_PIN_26,
    MXC_GPIO_FUNC_OUT,
    MXC_GPIO_PAD_NONE,
    MXC_GPIO_VSSEL_VDDIOH,
    MXC_GPIO_DRVSTR_0
};

static void beep(void)
{
    const int half_cycles = BEEP_DURATION_MS * 1000 / (BEEP_HALF_PERIOD_US * 2);

    for (int cycle = 0; cycle < half_cycles; cycle++) {
        MXC_GPIO_OutSet(beep_pin.port, beep_pin.mask);
        MXC_Delay(MXC_DELAY_USEC(BEEP_HALF_PERIOD_US));
        MXC_GPIO_OutClr(beep_pin.port, beep_pin.mask);
        MXC_Delay(MXC_DELAY_USEC(BEEP_HALF_PERIOD_US));
    }
}

void TFT_Print(char *str, int len, int x, int y, int font)
{
    text_t text;
    text.data = str;
    text.len = len;
    MXC_TFT_PrintFont(x, y, font, &text, NULL);
}

void TFT_test(void)
{
    char count_text[12];
    int count = 0;

    MXC_TFT_SetBackGroundColor(ROYAL_BLUE);
    MXC_TFT_SetForeGroundColor(WHITE);
    MXC_TFT_ClearScreen();

    TFT_Print("SW2 Count", sizeof("SW2 Count") - 1, 3, 30, (int)&Liberation_Sans16x16[0]);
    snprintf(count_text, sizeof(count_text), "%d", count);
    TFT_Print(count_text, strlen(count_text), 3, 55, (int)&Liberation_Sans28x28[0]);

    while (1) {
        if (PB_Get(0)) {
            count++;

            snprintf(count_text, sizeof(count_text), "%d", count);
            MXC_TFT_SetBackGroundColor(ROYAL_BLUE);
            MXC_TFT_ClearScreen();
            TFT_Print("SW2 Count", sizeof("SW2 Count") - 1, 3, 30, (int)&Liberation_Sans16x16[0]);
            TFT_Print(count_text, strlen(count_text), 3, 55, (int)&Liberation_Sans28x28[0]);

            beep();

            while (PB_Get(0)) {
                MXC_Delay(MXC_DELAY_MSEC(10));
            }
        }
    }
}

int main(void)
{
    MXC_Delay(MXC_DELAY_SEC(2));

    /* Enable cache */
    Board_Init();
    MXC_ICC_Enable(MXC_ICC0);

    /* Set system clock to 100 MHz */
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

    /* Initialize TFT display */
    MXC_TFT_Init();
    MXC_GPIO_Config(&beep_pin);
    MXC_GPIO_OutClr(beep_pin.port, beep_pin.mask);

    TFT_test();

    return 0;
}
