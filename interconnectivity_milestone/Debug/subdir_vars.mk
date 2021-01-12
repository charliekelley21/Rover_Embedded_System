################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../CC3220SF_LAUNCHXL_FREERTOS.cmd 

SYSCFG_SRCS += \
../common.syscfg 

C_SRCS += \
../A2D.c \
./syscfg/ti_drivers_config.c \
./syscfg/ti_net_config.c \
./syscfg/ti_drivers_net_wifi_config.c \
../debug.c \
../event_group.c \
../json_parser.c \
../main_freertos.c \
../mqtt_callback_queue.c \
../mqtt_client_app.c \
../mqtt_if.c \
../network_if.c \
../task1.c \
../task2.c \
../task_queues.c \
../uart_term.c 

GEN_FILES += \
./syscfg/ti_drivers_config.c \
./syscfg/ti_net_config.c \
./syscfg/ti_drivers_net_wifi_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./A2D.d \
./syscfg/ti_drivers_config.d \
./syscfg/ti_net_config.d \
./syscfg/ti_drivers_net_wifi_config.d \
./debug.d \
./event_group.d \
./json_parser.d \
./main_freertos.d \
./mqtt_callback_queue.d \
./mqtt_client_app.d \
./mqtt_if.d \
./network_if.d \
./task1.d \
./task2.d \
./task_queues.d \
./uart_term.d 

OBJS += \
./A2D.obj \
./syscfg/ti_drivers_config.obj \
./syscfg/ti_net_config.obj \
./syscfg/ti_drivers_net_wifi_config.obj \
./debug.obj \
./event_group.obj \
./json_parser.obj \
./main_freertos.obj \
./mqtt_callback_queue.obj \
./mqtt_client_app.obj \
./mqtt_if.obj \
./network_if.obj \
./task1.obj \
./task2.obj \
./task_queues.obj \
./uart_term.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.exp \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"A2D.obj" \
"syscfg\ti_drivers_config.obj" \
"syscfg\ti_net_config.obj" \
"syscfg\ti_drivers_net_wifi_config.obj" \
"debug.obj" \
"event_group.obj" \
"json_parser.obj" \
"main_freertos.obj" \
"mqtt_callback_queue.obj" \
"mqtt_client_app.obj" \
"mqtt_if.obj" \
"network_if.obj" \
"task1.obj" \
"task2.obj" \
"task_queues.obj" \
"uart_term.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\ti_utils_build_linker.cmd.exp" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"A2D.d" \
"syscfg\ti_drivers_config.d" \
"syscfg\ti_net_config.d" \
"syscfg\ti_drivers_net_wifi_config.d" \
"debug.d" \
"event_group.d" \
"json_parser.d" \
"main_freertos.d" \
"mqtt_callback_queue.d" \
"mqtt_client_app.d" \
"mqtt_if.d" \
"network_if.d" \
"task1.d" \
"task2.d" \
"task_queues.d" \
"uart_term.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_drivers_config.c" \
"syscfg\ti_net_config.c" \
"syscfg\ti_drivers_net_wifi_config.c" 

C_SRCS__QUOTED += \
"../A2D.c" \
"./syscfg/ti_drivers_config.c" \
"./syscfg/ti_net_config.c" \
"./syscfg/ti_drivers_net_wifi_config.c" \
"../debug.c" \
"../event_group.c" \
"../json_parser.c" \
"../main_freertos.c" \
"../mqtt_callback_queue.c" \
"../mqtt_client_app.c" \
"../mqtt_if.c" \
"../network_if.c" \
"../task1.c" \
"../task2.c" \
"../task_queues.c" \
"../uart_term.c" 

SYSCFG_SRCS__QUOTED += \
"../common.syscfg" 


