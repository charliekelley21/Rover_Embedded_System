################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Spencer/workspace_v10/interconnectivity_milestone" --include_path="C:/Users/Spencer/workspace_v10/interconnectivity_milestone/Debug" --include_path="C:/ti/simplelink_cc32xx_sdk_4_20_00_07/source" --include_path="C:/ti/simplelink_cc32xx_sdk_4_20_00_07/source/ti/posix/ccs" --include_path="C:/Users/Spencer/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/Users/Spencer/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/Users/Spencer/workspace_v10/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/include" -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/Spencer/workspace_v10/interconnectivity_milestone/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-609497457: ../common.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1010/ccs/utils/sysconfig_1.5.0/sysconfig_cli.bat" -s "C:/ti/simplelink_cc32xx_sdk_4_20_00_07/.metadata/product.json" -o "syscfg" --compiler ccs "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_drivers_config.c: build-609497457 ../common.syscfg
syscfg/ti_drivers_config.h: build-609497457
syscfg/ti_net_config.c: build-609497457
syscfg/ti_utils_build_linker.cmd.exp: build-609497457
syscfg/syscfg_c.rov.xs: build-609497457
syscfg/ti_drivers_net_wifi_config.c: build-609497457
syscfg/: build-609497457

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Spencer/workspace_v10/interconnectivity_milestone" --include_path="C:/Users/Spencer/workspace_v10/interconnectivity_milestone/Debug" --include_path="C:/ti/simplelink_cc32xx_sdk_4_20_00_07/source" --include_path="C:/ti/simplelink_cc32xx_sdk_4_20_00_07/source/ti/posix/ccs" --include_path="C:/Users/Spencer/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/Users/Spencer/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/Users/Spencer/workspace_v10/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/include" -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/Spencer/workspace_v10/interconnectivity_milestone/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


