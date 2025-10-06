# Add inputs and outputs from these tool invocations to the build variables
OUT_DIR += \
/$(SRC_PATH) \
/$(SRC_PATH)/common \
/$(SRC_PATH)/zcl
 
OBJS += \
$(OUT_PATH)/$(SRC_PATH)/common/main.o \
$(OUT_PATH)/$(SRC_PATH)/zcl/zcl_reporting.o \
$(OUT_PATH)/$(SRC_PATH)/zcl/zcl_onoffSwitchCfg.o \
$(OUT_PATH)/$(SRC_PATH)/zb_appCb.o \
$(OUT_PATH)/$(SRC_PATH)/zcl_appCb.o \
$(OUT_PATH)/$(SRC_PATH)/app_bootloader.o \
$(OUT_PATH)/$(SRC_PATH)/app_endpoint_cfg.o \
$(OUT_PATH)/$(SRC_PATH)/app_reporting.o \
$(OUT_PATH)/$(SRC_PATH)/app_utility.o \
$(OUT_PATH)/$(SRC_PATH)/app_onoff.o \
$(OUT_PATH)/$(SRC_PATH)/app_led.o \
$(OUT_PATH)/$(SRC_PATH)/app_button.o \
$(OUT_PATH)/$(SRC_PATH)/app_switch.o \
$(OUT_PATH)/$(SRC_PATH)/app_arith64.o \
$(OUT_PATH)/$(SRC_PATH)/app_relay.o \
$(OUT_PATH)/$(SRC_PATH)/app_uart.o \
$(OUT_PATH)/$(SRC_PATH)/app_monitoring.o \
$(OUT_PATH)/$(SRC_PATH)/app_main.o



# Each subdirectory must supply rules for building sources it contributes
$(OUT_PATH)/$(SRC_PATH)/%.o: $(SRC_PATH)/%.c 
	@echo 'Building file: $<'
	@$(CC) $(GCC_FLAGS) $(INCLUDE_PATHS) -c -o"$@" "$<"


