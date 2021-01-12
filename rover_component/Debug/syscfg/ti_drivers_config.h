/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  The macros defines herein are intended for use by applications which
 *  directly include this header. These macros should NOT be hard coded or
 *  copied into library source code.
 *
 *  Symbols declared as const are intended for use with libraries.
 *  Library source code must extern the correct symbol--which is resolved
 *  when the application is linked.
 *
 *  DO NOT EDIT - This file is generated for the CC3220SF_LAUNCHXL
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_CC3220SF_LAUNCHXL
#ifndef DeviceFamily_CC3220
#define DeviceFamily_CC3220
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== ADC ========
 */

/* P58 */
extern const uint_least8_t          CONFIG_ADC_0_CONST;
#define CONFIG_ADC_0                0


/*
 *  ======== Crypto ========
 */

extern const uint_least8_t          CONFIG_Crypto_0_CONST;
#define CONFIG_Crypto_0             0

/*
 *  ======== GPIO ========
 */

/* P21 */
extern const uint_least8_t          slave_select_1_CONST;
#define slave_select_1              0
/* P61 */
extern const uint_least8_t          slave_select_2_CONST;
#define slave_select_2              1
/* P08 */
extern const uint_least8_t          slave_select_3_CONST;
#define slave_select_3              2
/* P18 */
extern const uint_least8_t          DEBUG_0_CONST;
#define DEBUG_0                     3
/* P53 */
extern const uint_least8_t          DEBUG_1_CONST;
#define DEBUG_1                     4
/* P59 */
extern const uint_least8_t          DEBUG_2_CONST;
#define DEBUG_2                     5
/* P60 */
extern const uint_least8_t          DEBUG_3_CONST;
#define DEBUG_3                     6
/* P62 */
extern const uint_least8_t          DEBUG_4_CONST;
#define DEBUG_4                     7
/* P63 */
extern const uint_least8_t          DEBUG_5_CONST;
#define DEBUG_5                     8
/* P64, LaunchPad LED D10 (Red) */
extern const uint_least8_t          CONFIG_LED_0_GPIO_CONST;
#define CONFIG_LED_0_GPIO           9

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== SPI ========
 */

extern const uint_least8_t          CONFIG_NWP_SPI_CONST;
#define CONFIG_NWP_SPI              0
/*
 *  MOSI: P07
 *  MISO: P06
 *  SCLK: P05
 *  SS: P50
 */
extern const uint_least8_t          CONFIG_SPI_0_CONST;
#define CONFIG_SPI_0                1


/*
 *  ======== Timer ========
 */

extern const uint_least8_t          CONFIG_TIMER_0_CONST;
#define CONFIG_TIMER_0              0
extern const uint_least8_t          CONFIG_TIMER_1_CONST;
#define CONFIG_TIMER_1              1
extern const uint_least8_t          CONFIG_TIMER_2_CONST;
#define CONFIG_TIMER_2              2

/*
 *  ======== UART ========
 */

/*
 *  TX: P03
 *  RX: P45
 */
extern const uint_least8_t          motor_driver_uart_CONST;
#define motor_driver_uart           0
/*
 *  TX: P55
 *  RX: P57
 *  XDS110 UART
 */
extern const uint_least8_t          CONFIG_UART_0_CONST;
#define CONFIG_UART_0               1


/*
 *  ======== Watchdog ========
 */

extern const uint_least8_t          CONFIG_WATCHDOG_0_CONST;
#define CONFIG_WATCHDOG_0           0


/*
 *  ======== LED ========
 */

/* P64, LaunchPad LED D10 (Red) */
extern const uint_least8_t          CONFIG_LED_0_CONST;
#define CONFIG_LED_0                0


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
