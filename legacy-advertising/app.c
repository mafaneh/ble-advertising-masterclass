/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "app.h"

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;

#define DEVICE_NAME_VALUE "Novel Bits"
#define DEVICE_NAME_LENGTH (sizeof(DEVICE_NAME_VALUE)-1) // includes the Null terminator

typedef struct
{
  // Flags
  uint8_t flags_len;
  uint8_t flags_type;
  uint8_t flags_value;

  // Device Name (Complete Local Name)
  uint8_t device_name_len;
  uint8_t device_name_type;
  uint8_t device_name_value[DEVICE_NAME_LENGTH];

} adv_data_t;

static const adv_data_t adv_data =
{
    // Flags
    .flags_len = 0x02,
    .flags_type = 0x01,
    .flags_value = 0x06,

    // Device Name
    .device_name_len = DEVICE_NAME_LENGTH+1, // adding 1 to include the type field
    .device_name_type = 0x09,
    .device_name_value = DEVICE_NAME_VALUE

};

//typedef struct
//{
//    uint8_t manuf_spec_data_len;
//    uint8_t manuf_spec_data_type;
//    uint8_t manuf_spec_data_company_id[2];
//    uint8_t manuf_spec_data_value[27];
//
//} scan_response_data_t;
//
//static const scan_response_data_t scan_rsp_data =
//{
//    .manuf_spec_data_len = 30,
//    .manuf_spec_data_type = 0xFF,
//    .manuf_spec_data_company_id = { 0xD3, 0x08},
//    .manuf_spec_data_value = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
//                               0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B
//    }
//};

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application init code here!                         //
  // This is called once during start-up.                                    //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;
  bd_addr address;
  uint8_t address_type;

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:

      app_log("Starting Advertising Data Example\r\n");

      // Print the Bluetooth Address (note the reversed order of the bytes)
      sc = sl_bt_system_get_identity_address(&address, &address_type);
      app_assert_status(sc);

      app_log("Bluetooth Device Address: ");
      for (int i=0; i<5; i++)
        {
          app_log("%02X:", address.addr[5-i]);
        }
      app_log("%02X (%s)\r\n", address.addr[0], address_type == 0 ? "Public device address":"Static device address");

      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Standard Advertising Data
      app_log("Setting Standard Advertising Data\r\n");
      sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle,
                                                  sl_bt_advertiser_advertising_data_packet,
                                                  sizeof(adv_data),
                                                  (uint8_t *)&adv_data);
      app_assert_status(sc);


      // Scan Response Data
//      app_log("Setting Scan Response Data\r\n");
//      sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle,
//                                            sl_bt_advertiser_scan_response_packet,
//                                                  sizeof(scan_rsp_data),
//                                                  (uint8_t *)&scan_rsp_data);
//      app_assert_status(sc);

      // Set advertising interval to 100ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160, // min. adv. interval (milliseconds * 1.6)
        160, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
      app_assert_status(sc);

      // Start advertising
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_advertiser_non_connectable);
      app_assert_status(sc);

//      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
//                                         sl_bt_advertiser_scannable_non_connectable);
//      app_assert_status(sc);
      break;

    ///////////////////////////////////////////////////////////////////////////
    // Add additional event handlers here as your application requires!      //
    ///////////////////////////////////////////////////////////////////////////

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}
