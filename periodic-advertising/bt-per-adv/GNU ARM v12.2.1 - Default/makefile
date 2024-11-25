################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include gecko_sdk_4.4.5/util/third_party/mbedtls/library/subdir.mk
-include gecko_sdk_4.4.5/util/silicon_labs/silabs_core/memory_manager/subdir.mk
-include gecko_sdk_4.4.5/protocol/bluetooth/src/subdir.mk
-include gecko_sdk_4.4.5/platform/service/system/src/subdir.mk
-include gecko_sdk_4.4.5/platform/service/sleeptimer/src/subdir.mk
-include gecko_sdk_4.4.5/platform/service/power_manager/src/subdir.mk
-include gecko_sdk_4.4.5/platform/service/mpu/src/subdir.mk
-include gecko_sdk_4.4.5/platform/service/iostream/src/subdir.mk
-include gecko_sdk_4.4.5/platform/service/device_init/src/subdir.mk
-include gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/subdir.mk
-include gecko_sdk_4.4.5/platform/security/sl_component/sl_protocol_crypto/src/subdir.mk
-include gecko_sdk_4.4.5/platform/security/sl_component/sl_mbedtls_support/src/subdir.mk
-include gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/subdir.mk
-include gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/subdir.mk
-include gecko_sdk_4.4.5/platform/radio/rail_lib/plugin/rail_util_pti/subdir.mk
-include gecko_sdk_4.4.5/platform/radio/rail_lib/plugin/rail_util_power_manager_init/subdir.mk
-include gecko_sdk_4.4.5/platform/radio/rail_lib/plugin/pa-conversions/subdir.mk
-include gecko_sdk_4.4.5/platform/radio/rail_lib/plugin/fem_util/subdir.mk
-include gecko_sdk_4.4.5/platform/emlib/src/subdir.mk
-include gecko_sdk_4.4.5/platform/emdrv/nvm3/src/subdir.mk
-include gecko_sdk_4.4.5/platform/emdrv/dmadrv/src/subdir.mk
-include gecko_sdk_4.4.5/platform/driver/debug/src/subdir.mk
-include gecko_sdk_4.4.5/platform/common/toolchain/src/subdir.mk
-include gecko_sdk_4.4.5/platform/common/src/subdir.mk
-include gecko_sdk_4.4.5/platform/bootloader/app_properties/subdir.mk
-include gecko_sdk_4.4.5/platform/bootloader/api/subdir.mk
-include gecko_sdk_4.4.5/platform/Device/SiliconLabs/BGM22/Source/subdir.mk
-include gecko_sdk_4.4.5/hardware/driver/configuration_over_swo/src/subdir.mk
-include gecko_sdk_4.4.5/hardware/board/src/subdir.mk
-include gecko_sdk_4.4.5/app/common/util/app_timer/subdir.mk
-include gecko_sdk_4.4.5/app/common/util/app_log/subdir.mk
-include gecko_sdk_4.4.5/app/bluetooth/common/in_place_ota_dfu/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := bt-per-adv
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: bt-per-adv.axf

# Tool invocations
bt-per-adv.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "/Users/mafaneh/Projects/ble-advertising-masterclass/periodic-advertising/bt-per-adv/autogen/linkerfile.ld" -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="bt-per-adv.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o bt-per-adv.axf -Wl,--start-group "./app.o" "./main.o" "./autogen/gatt_db.o" "./autogen/sl_bluetooth.o" "./autogen/sl_board_default_init.o" "./autogen/sl_device_init_clocks.o" "./autogen/sl_event_handler.o" "./autogen/sl_iostream_handles.o" "./autogen/sl_iostream_init_usart_instances.o" "./autogen/sl_power_manager_handler.o" "./gecko_sdk_4.4.5/app/bluetooth/common/in_place_ota_dfu/sl_bt_in_place_ota_dfu.o" "./gecko_sdk_4.4.5/app/common/util/app_log/app_log.o" "./gecko_sdk_4.4.5/app/common/util/app_timer/app_timer.o" "./gecko_sdk_4.4.5/hardware/board/src/sl_board_control_gpio.o" "./gecko_sdk_4.4.5/hardware/board/src/sl_board_init.o" "./gecko_sdk_4.4.5/hardware/driver/configuration_over_swo/src/sl_cos.o" "./gecko_sdk_4.4.5/platform/Device/SiliconLabs/BGM22/Source/startup_bgm22.o" "./gecko_sdk_4.4.5/platform/Device/SiliconLabs/BGM22/Source/system_bgm22.o" "./gecko_sdk_4.4.5/platform/bootloader/api/btl_interface.o" "./gecko_sdk_4.4.5/platform/bootloader/api/btl_interface_storage.o" "./gecko_sdk_4.4.5/platform/bootloader/app_properties/app_properties.o" "./gecko_sdk_4.4.5/platform/common/src/sl_assert.o" "./gecko_sdk_4.4.5/platform/common/src/sl_slist.o" "./gecko_sdk_4.4.5/platform/common/src/sl_string.o" "./gecko_sdk_4.4.5/platform/common/src/sl_syscalls.o" "./gecko_sdk_4.4.5/platform/common/toolchain/src/sl_memory.o" "./gecko_sdk_4.4.5/platform/driver/debug/src/sl_debug_swo.o" "./gecko_sdk_4.4.5/platform/emdrv/dmadrv/src/dmadrv.o" "./gecko_sdk_4.4.5/platform/emdrv/nvm3/src/nvm3_default_common_linker.o" "./gecko_sdk_4.4.5/platform/emdrv/nvm3/src/nvm3_hal_flash.o" "./gecko_sdk_4.4.5/platform/emdrv/nvm3/src/nvm3_lock.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_burtc.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_cmu.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_core.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_dbg.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_emu.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_eusart.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_gpcrc.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_gpio.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_i2c.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_iadc.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_ldma.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_letimer.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_msc.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_prs.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_rmu.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_rtcc.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_se.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_system.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_timer.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_usart.o" "./gecko_sdk_4.4.5/platform/emlib/src/em_wdog.o" "./gecko_sdk_4.4.5/platform/radio/rail_lib/plugin/fem_util/sl_fem_util.o" "./gecko_sdk_4.4.5/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.o" "./gecko_sdk_4.4.5/platform/radio/rail_lib/plugin/rail_util_power_manager_init/sl_rail_util_power_manager_init.o" "./gecko_sdk_4.4.5/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.o" "./gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/sl_se_manager.o" "./gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.o" "./gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.o" "./gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.o" "./gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/sl_se_manager_hash.o" "./gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.o" "./gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.o" "./gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/sl_se_manager_signature.o" "./gecko_sdk_4.4.5/platform/security/sl_component/se_manager/src/sl_se_manager_util.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/ba414ep_config.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/ba431_config.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/cryptodma_internal.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/cryptolib_types.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_aes.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_blk_cipher.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_dh_alg.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_ecc_curves.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_ecc_keygen_alg.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_ecdsa_alg.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_hash.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_math.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_memcmp.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_memcpy.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_primitives.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_rng.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_cryptoacc_library/src/sx_trng.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_mbedtls_support/src/cryptoacc_aes.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_mbedtls_support/src/cryptoacc_gcm.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ccm.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_cmac.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_mbedtls_support/src/mbedtls_ecdsa_ecdh.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_mbedtls_support/src/sl_psa_crypto.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_mbedtls_support/src/sli_psa_crypto.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_protocol_crypto/src/sli_protocol_crypto_radioaes.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_protocol_crypto/src/sli_radioaes_management.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/cryptoacc_management.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sl_psa_its_nvm3.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_cryptoacc_driver_trng.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_cryptoacc_transparent_driver_aead.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_cryptoacc_transparent_driver_cipher.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_cryptoacc_transparent_driver_hash.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_cryptoacc_transparent_driver_key_derivation.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_cryptoacc_transparent_driver_key_management.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_cryptoacc_transparent_driver_mac.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_cryptoacc_transparent_driver_signature.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_psa_trng.o" "./gecko_sdk_4.4.5/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.o" "./gecko_sdk_4.4.5/platform/service/device_init/src/sl_device_init_dcdc_s2.o" "./gecko_sdk_4.4.5/platform/service/device_init/src/sl_device_init_emu_s2.o" "./gecko_sdk_4.4.5/platform/service/device_init/src/sl_device_init_hfxo_s2.o" "./gecko_sdk_4.4.5/platform/service/device_init/src/sl_device_init_lfrco.o" "./gecko_sdk_4.4.5/platform/service/device_init/src/sl_device_init_lfxo_s2.o" "./gecko_sdk_4.4.5/platform/service/device_init/src/sl_device_init_nvic.o" "./gecko_sdk_4.4.5/platform/service/iostream/src/sl_iostream.o" "./gecko_sdk_4.4.5/platform/service/iostream/src/sl_iostream_retarget_stdio.o" "./gecko_sdk_4.4.5/platform/service/iostream/src/sl_iostream_stdlib_config.o" "./gecko_sdk_4.4.5/platform/service/iostream/src/sl_iostream_uart.o" "./gecko_sdk_4.4.5/platform/service/iostream/src/sl_iostream_usart.o" "./gecko_sdk_4.4.5/platform/service/mpu/src/sl_mpu.o" "./gecko_sdk_4.4.5/platform/service/power_manager/src/sl_power_manager.o" "./gecko_sdk_4.4.5/platform/service/power_manager/src/sl_power_manager_debug.o" "./gecko_sdk_4.4.5/platform/service/power_manager/src/sl_power_manager_hal_s2.o" "./gecko_sdk_4.4.5/platform/service/sleeptimer/src/sl_sleeptimer.o" "./gecko_sdk_4.4.5/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o" "./gecko_sdk_4.4.5/platform/service/sleeptimer/src/sl_sleeptimer_hal_prortc.o" "./gecko_sdk_4.4.5/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtcc.o" "./gecko_sdk_4.4.5/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o" "./gecko_sdk_4.4.5/platform/service/system/src/sl_system_init.o" "./gecko_sdk_4.4.5/platform/service/system/src/sl_system_process_action.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sl_apploader_util_s2.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sl_bt_stack_init.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sli_bt_accept_list_config.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sli_bt_advertiser_config.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sli_bt_connection_config.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sli_bt_dynamic_gattdb_config.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sli_bt_l2cap_config.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sli_bt_pawr_advertiser_config.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sli_bt_periodic_adv_config.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sli_bt_periodic_advertiser_config.o" "./gecko_sdk_4.4.5/protocol/bluetooth/src/sli_bt_sync_config.o" "./gecko_sdk_4.4.5/util/silicon_labs/silabs_core/memory_manager/sl_malloc.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/cipher.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/cipher_wrap.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/constant_time.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/platform.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/platform_util.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_aead.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_cipher.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_client.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_driver_wrappers_no_static.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_ecp.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_ffdh.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_hash.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_mac.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_pake.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_rsa.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_se.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_slot_management.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_crypto_storage.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/psa_util.o" "./gecko_sdk_4.4.5/util/third_party/mbedtls/library/threading.o" "/Users/mafaneh/Projects/ble-advertising-masterclass/periodic-advertising/bt-per-adv/gecko_sdk_4.4.5/protocol/bluetooth/bgcommon/lib/libbgcommon_efr32xg22_gcc_release.a" "/Users/mafaneh/Projects/ble-advertising-masterclass/periodic-advertising/bt-per-adv/gecko_sdk_4.4.5/protocol/bluetooth/bgstack/ll/lib/libbluetooth_controller_efr32xg22_gcc_release.a" "/Users/mafaneh/Projects/ble-advertising-masterclass/periodic-advertising/bt-per-adv/gecko_sdk_4.4.5/protocol/bluetooth/lib/libbluetooth_host_efr32xg22_gcc_release.a" "/Users/mafaneh/Projects/ble-advertising-masterclass/periodic-advertising/bt-per-adv/gecko_sdk_4.4.5/platform/emdrv/nvm3/lib/libnvm3_CM33_gcc.a" "/Users/mafaneh/Projects/ble-advertising-masterclass/periodic-advertising/bt-per-adv/gecko_sdk_4.4.5/platform/radio/rail_lib/autogen/librail_release/librail_config_bgm220pc22hna_gcc.a" "/Users/mafaneh/Projects/ble-advertising-masterclass/periodic-advertising/bt-per-adv/gecko_sdk_4.4.5/platform/radio/rail_lib/autogen/librail_release/librail_module_efr32xg22_gcc_release.a" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: bt-per-adv.out'
	arm-none-eabi-objcopy "bt-per-adv.axf" "bt-per-adv.out"
	@echo ' '

	@echo 'Building hex file: bt-per-adv.hex'
	arm-none-eabi-objcopy -O ihex "bt-per-adv.axf" "bt-per-adv.hex"
	@echo ' '

	@echo 'Building bin file: bt-per-adv.bin'
	arm-none-eabi-objcopy -O binary "bt-per-adv.axf" "bt-per-adv.bin"
	@echo ' '

	@echo 'Building s37 file: bt-per-adv.s37'
	arm-none-eabi-objcopy -O srec "bt-per-adv.axf" "bt-per-adv.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "bt-per-adv.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) bt-per-adv.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
