/***************************************************************************//**
 * @brief Initialization APIs for Bluetooth stack
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

// Include the catalog first to make sure everything below can see it
#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#else // SL_COMPONENT_CATALOG_PRESENT
#error The SL component catalog required by Bluetooth stack is not present in the build
#endif // SL_COMPONENT_CATALOG_PRESENT

// Define the initilization mode for the configuration definition headers. The
// initialization mode for the Bluetooth stack depends on the on-demand start
// component. When on-demand start is used, features support de-initialization.
// When on-demand start is not used, the de-init functions are omitted to
// eliminate code that's not needed in the build.
#if defined(SL_CATALOG_BLUETOOTH_ON_DEMAND_START_PRESENT)
#define SLI_BT_INIT_MODE on_demand
#else
#define SLI_BT_INIT_MODE init_always
#endif

#include "sl_status.h"
#include "sl_bt_api.h"
#include "sl_bt_stack_config.h"
#include "sl_bt_stack_init.h"
#include "sl_bluetooth.h"
#include "sl_bluetooth_config.h"
#include "sl_btctrl_linklayer.h"
#include "sli_bt_gattdb_def.h"
#include "sli_bt_config_defs.h"

#ifdef SL_CATALOG_GATT_CONFIGURATION_PRESENT
extern const sli_bt_gattdb_t gattdb;
#else
const sli_bt_gattdb_t gattdb = { 0 };
#endif // SL_CATALOG_GATT_CONFIGURATION_PRESENT

// Forward declaration of BGAPI classes
SLI_BT_DECLARE_BGAPI_CLASS(bt, system);
SLI_BT_DECLARE_BGAPI_CLASS(bt, nvm);
SLI_BT_DECLARE_BGAPI_CLASS(bt, ota);
SLI_BT_DECLARE_BGAPI_CLASS(bt, gap);
SLI_BT_DECLARE_BGAPI_CLASS(bt, sm);
SLI_BT_DECLARE_BGAPI_CLASS(bt, external_bondingdb);
SLI_BT_DECLARE_BGAPI_CLASS(bt, accept_list);
SLI_BT_DECLARE_BGAPI_CLASS(bt, resolving_list);
SLI_BT_DECLARE_BGAPI_CLASS(bt, advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, legacy_advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, extended_advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, periodic_advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, scanner);
SLI_BT_DECLARE_BGAPI_CLASS(bt, sync);
SLI_BT_DECLARE_BGAPI_CLASS(bt, pawr_advertiser);
SLI_BT_DECLARE_BGAPI_CLASS(bt, sync_scanner);
SLI_BT_DECLARE_BGAPI_CLASS(bt, periodic_sync);
SLI_BT_DECLARE_BGAPI_CLASS(bt, pawr_sync);
SLI_BT_DECLARE_BGAPI_CLASS(bt, past_receiver);
SLI_BT_DECLARE_BGAPI_CLASS(bt, advertiser_past);
SLI_BT_DECLARE_BGAPI_CLASS(bt, sync_past);
SLI_BT_DECLARE_BGAPI_CLASS(bt, cs);
SLI_BT_DECLARE_BGAPI_CLASS(bt, cs_test);
SLI_BT_DECLARE_BGAPI_CLASS(bt, l2cap);
SLI_BT_DECLARE_BGAPI_CLASS(bt, connection);
SLI_BT_DECLARE_BGAPI_CLASS(bt, gatt);
SLI_BT_DECLARE_BGAPI_CLASS(bt, gattdb);
SLI_BT_DECLARE_BGAPI_CLASS(bt, gatt_server);
SLI_BT_DECLARE_BGAPI_CLASS(bt, cte_receiver);
SLI_BT_DECLARE_BGAPI_CLASS(bt, cte_transmitter);
SLI_BT_DECLARE_BGAPI_CLASS(bt, test);
SLI_BT_DECLARE_BGAPI_CLASS(bt, coex);
SLI_BT_DECLARE_BGAPI_CLASS(bt, resource);
SLI_BT_DECLARE_BGAPI_CLASS(bt, connection_analyzer);

// Forward declaration of the internal Bluetooth stack init function
sl_status_t sli_bt_init_stack(const sl_bt_configuration_t *config,
                              const struct sli_bt_feature_use *features,
                              const struct sli_bgapi_class * const *bgapi_classes);

// Some features do not correspond directly to a particular component but are
// needed depending on a specific combination of components. Decide the derived
// feature selections here to simplify the feature inclusion rules below.

// Advertiser backwards compatibility is needed if the application uses the
// "advertiser" component but not any of the components that supersede its
// functionality.
#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PRESENT)            \
  && !defined(SL_CATALOG_BLUETOOTH_FEATURE_LEGACY_ADVERTISER_PRESENT)   \
  && !defined(SL_CATALOG_BLUETOOTH_FEATURE_EXTENDED_ADVERTISER_PRESENT) \
  && !defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_ADVERTISER_PRESENT)
#define SLI_BT_ENABLE_ADVERTISER_BACKWARDS_COMPATIBILITY
#endif

// Extended advertising feature is included if it's explicitly used or needed
// for backwards compatibility, but only when there's no device incompatibility.
#if (defined(SL_CATALOG_BLUETOOTH_FEATURE_EXTENDED_ADVERTISER_PRESENT) \
  || defined(SLI_BT_ENABLE_ADVERTISER_BACKWARDS_COMPATIBILITY))        \
  && !defined(SL_CATALOG_BLUETOOTH_EXTENDED_ADVERTISING_INCOMPATIBLE_PRESENT)
#define SLI_BT_ENABLE_EXTENDED_ADVERTISER_FEATURE
#endif

// Deprecated "periodic_adv" feature is only relevant for applications that need
// advertiser backwards compatibility, as new applications would use
// "periodic_advertiser". Include "periodic_adv" if it's used together with
// advertiser backwards compatibility, but only when there's no device incompatibility.
#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_ADV_PRESENT) \
  && defined(SLI_BT_ENABLE_ADVERTISER_BACKWARDS_COMPATIBILITY) \
  && !defined(SL_CATALOG_BLUETOOTH_EXTENDED_ADVERTISING_INCOMPATIBLE_PRESENT)
#define SLI_BT_ENABLE_PERIODIC_ADV_FEATURE
#endif

// Scanner backwards compatibility is needed if the application uses the
// "scanner" component but not any of the components that supersede its
// functionality.
#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SCANNER_PRESENT)          \
  && !defined(SL_CATALOG_BLUETOOTH_FEATURE_LEGACY_SCANNER_PRESENT) \
  && !defined(SL_CATALOG_BLUETOOTH_FEATURE_EXTENDED_SCANNER_PRESENT)
#define SLI_BT_ENABLE_SCANNER_BACKWARDS_COMPATIBILITY
#endif

// The scanner event handler is included if the legacy or extended scanner
// component is used.
#if defined(SL_CATALOG_BLUETOOTH_FEATURE_LEGACY_SCANNER_PRESENT) \
  || defined(SL_CATALOG_BLUETOOTH_FEATURE_EXTENDED_SCANNER_PRESENT)
#define SLI_BT_ENABLE_SCANNER_BASE
#endif

// Extended scanner feature is included if it's explicitly used or needed
// for backwards compatibility, but only when there's no device incompatibility.
#if (defined(SL_CATALOG_BLUETOOTH_FEATURE_EXTENDED_SCANNER_PRESENT) \
  || defined(SLI_BT_ENABLE_SCANNER_BACKWARDS_COMPATIBILITY))        \
  && !defined(SL_CATALOG_BLUETOOTH_EXTENDED_SCANNING_INCOMPATIBLE_PRESENT)
#define SLI_BT_ENABLE_EXTENDED_SCANNER_FEATURE
#endif

// Periodic synchronization backwards compatibility is needed if the application
// uses the "sync" component but not any of the components that supersede its
// functionality.
#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PRESENT)            \
  && !defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_SCANNER_PRESENT)  \
  && !defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_SYNC_PRESENT) \
  && !defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_SYNC_PRESENT)
#define SLI_BT_ENABLE_SYNC_BACKWARDS_COMPATIBILITY
#endif

// If the build configuration needs a specific feature, we pick it for inclusion
// in the feature and BGAPI lists, as applicable.

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYSTEM_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, system);
#define SLI_BT_FEATURE_SYSTEM SLI_BT_USE_FEATURE(bt, system),
#define SLI_BT_BGAPI_SYSTEM SLI_BT_USE_BGAPI_CLASS(bt, system),
#else
#define SLI_BT_FEATURE_SYSTEM
#define SLI_BT_BGAPI_SYSTEM
#endif

#if defined(SL_CATALOG_BLUETOOTH_ON_DEMAND_START_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, on_demand_start);
#define SLI_BT_FEATURE_ON_DEMAND_START SLI_BT_USE_FEATURE(bt, on_demand_start),
#else
#define SLI_BT_FEATURE_ON_DEMAND_START
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_NVM_PRESENT)
#define SLI_BT_BGAPI_NVM SLI_BT_USE_BGAPI_CLASS(bt, nvm),
#else
#define SLI_BT_BGAPI_NVM
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_OTA_CONFIG_PRESENT)
#define SLI_BT_BGAPI_OTA_CONFIG SLI_BT_USE_BGAPI_CLASS(bt, ota),
#else
#define SLI_BT_BGAPI_OTA_CONFIG
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_GAP_PRESENT)
#define SLI_BT_BGAPI_GAP SLI_BT_USE_BGAPI_CLASS(bt, gap),
#else
#define SLI_BT_BGAPI_GAP
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SM_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, sm);
#define SLI_BT_FEATURE_SM SLI_BT_USE_FEATURE(bt, sm),
#define SLI_BT_BGAPI_SM SLI_BT_USE_BGAPI_CLASS(bt, sm),
#else
#define SLI_BT_FEATURE_SM
#define SLI_BT_BGAPI_SM
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_BUILTIN_BONDING_DATABASE_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, builtin_bonding_database);
#define SLI_BT_FEATURE_BUILTIN_BONDING_DATABASE SLI_BT_USE_FEATURE(bt, builtin_bonding_database),
#else
#define SLI_BT_FEATURE_BUILTIN_BONDING_DATABASE
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_EXTERNAL_BONDING_DATABASE_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, external_bonding_database);
#define SLI_BT_FEATURE_EXTERNAL_BONDING_DATABASE SLI_BT_USE_FEATURE(bt, external_bonding_database),
#define SLI_BT_BGAPI_EXTERNAL_BONDINGDB   SLI_BT_USE_BGAPI_CLASS(bt, external_bondingdb),
#else
#define SLI_BT_FEATURE_EXTERNAL_BONDING_DATABASE
#define SLI_BT_BGAPI_EXTERNAL_BONDINGDB
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ACCEPT_LIST_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, accept_list);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, accept_list);
#define SLI_BT_FEATURE_ACCEPT_LIST SLI_BT_USE_FEATURE_WITH_CONFIG(bt, accept_list, SLI_BT_FEATURE_CONFIG_NAME(bt, accept_list)),
#define SLI_BT_BGAPI_ACCEPT_LIST SLI_BT_USE_BGAPI_CLASS(bt, accept_list),
#else
#define SLI_BT_FEATURE_ACCEPT_LIST
#define SLI_BT_BGAPI_ACCEPT_LIST
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_RESOLVING_LIST_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, resolving_list);
#define SLI_BT_FEATURE_RESOLVING_LIST SLI_BT_USE_FEATURE(bt, resolving_list),
#define SLI_BT_BGAPI_RESOLVING_LIST SLI_BT_USE_BGAPI_CLASS(bt, resolving_list),
#else
#define SLI_BT_FEATURE_RESOLVING_LIST
#define SLI_BT_BGAPI_RESOLVING_LIST
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, advertiser);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, advertiser);
#define SLI_BT_FEATURE_ADVERTISER SLI_BT_USE_FEATURE_WITH_CONFIG(bt, advertiser, SLI_BT_FEATURE_CONFIG_NAME(bt, advertiser)),
#define SLI_BT_BGAPI_ADVERTISER   SLI_BT_USE_BGAPI_CLASS(bt, advertiser),
#else
#define SLI_BT_FEATURE_ADVERTISER
#define SLI_BT_BGAPI_ADVERTISER
#endif

#if defined(SLI_BT_ENABLE_ADVERTISER_BACKWARDS_COMPATIBILITY)
SLI_BT_DECLARE_FEATURE(bt, advertiser_compatibility);
#define SLI_BT_FEATURE_ADVERTISER_COMPATIBILITY SLI_BT_USE_FEATURE(bt, advertiser_compatibility),
#else
#define SLI_BT_FEATURE_ADVERTISER_COMPATIBILITY
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_LEGACY_ADVERTISER_PRESENT)
#define SLI_BT_BGAPI_LEGACY_ADVERTISER SLI_BT_USE_BGAPI_CLASS(bt, legacy_advertiser),
#else
#define SLI_BT_BGAPI_LEGACY_ADVERTISER
#endif

#if defined(SLI_BT_ENABLE_EXTENDED_ADVERTISER_FEATURE)
SLI_BT_DECLARE_FEATURE(bt, extended_advertiser);
#define SLI_BT_FEATURE_EXTENDED_ADVERTISER SLI_BT_USE_FEATURE(bt, extended_advertiser),
#else
#define SLI_BT_FEATURE_EXTENDED_ADVERTISER
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_EXTENDED_ADVERTISER_PRESENT)
#define SLI_BT_BGAPI_EXTENDED_ADVERTISER SLI_BT_USE_BGAPI_CLASS(bt, extended_advertiser),
#else
#define SLI_BT_BGAPI_EXTENDED_ADVERTISER
#endif

#if defined(SLI_BT_ENABLE_PERIODIC_ADV_FEATURE)
SLI_BT_DECLARE_FEATURE(bt, periodic_adv);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, periodic_adv);
#define SLI_BT_FEATURE_PERIODIC_ADV SLI_BT_USE_FEATURE_WITH_CONFIG(bt, periodic_adv, SLI_BT_FEATURE_CONFIG_NAME(bt, periodic_adv)),
#else
#define SLI_BT_FEATURE_PERIODIC_ADV
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_ADVERTISER_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, periodic_advertiser);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, periodic_advertiser);
#define SLI_BT_FEATURE_PERIODIC_ADVERTISER SLI_BT_USE_FEATURE_WITH_CONFIG(bt, periodic_advertiser, SLI_BT_FEATURE_CONFIG_NAME(bt, periodic_advertiser)),
#define SLI_BT_BGAPI_PERIODIC_ADVERTISER SLI_BT_USE_BGAPI_CLASS(bt, periodic_advertiser),
#else
#define SLI_BT_FEATURE_PERIODIC_ADVERTISER
#define SLI_BT_BGAPI_PERIODIC_ADVERTISER
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_ADVERTISER_PRESENT)
#define SLI_BT_BGAPI_PAWR_ADVERTISER SLI_BT_USE_BGAPI_CLASS(bt, pawr_advertiser),
#else
#define SLI_BT_BGAPI_PAWR_ADVERTISER
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SCANNER_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, scanner);
#define SLI_BT_FEATURE_SCANNER SLI_BT_USE_FEATURE(bt, scanner),
#define SLI_BT_BGAPI_SCANNER SLI_BT_USE_BGAPI_CLASS(bt, scanner),
#else
#define SLI_BT_FEATURE_SCANNER
#define SLI_BT_BGAPI_SCANNER
#endif

#if defined(SLI_BT_ENABLE_SCANNER_BACKWARDS_COMPATIBILITY)
SLI_BT_DECLARE_FEATURE(bt, scanner_compatibility);
#define SLI_BT_FEATURE_SCANNER_COMPATIBILITY SLI_BT_USE_FEATURE(bt, scanner_compatibility),
#else
#define SLI_BT_FEATURE_SCANNER_COMPATIBILITY
#endif

#if defined(SLI_BT_ENABLE_SCANNER_BASE)
SLI_BT_DECLARE_FEATURE(bt, scanner_base);
#define SLI_BT_FEATURE_SCANNER_BASE SLI_BT_USE_FEATURE(bt, scanner_base),
#else
#define SLI_BT_FEATURE_SCANNER_BASE
#endif

#if defined(SLI_BT_ENABLE_EXTENDED_SCANNER_FEATURE)
SLI_BT_DECLARE_FEATURE(bt, extended_scanner);
#define SLI_BT_FEATURE_EXTENDED_SCANNER SLI_BT_USE_FEATURE(bt, extended_scanner),
#else
#define SLI_BT_FEATURE_EXTENDED_SCANNER
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, sync);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, sync);
#define SLI_BT_FEATURE_SYNC SLI_BT_USE_FEATURE_WITH_CONFIG(bt, sync, SLI_BT_FEATURE_CONFIG_NAME(bt, sync)),
#define SLI_BT_BGAPI_SYNC SLI_BT_USE_BGAPI_CLASS(bt, sync),
#else
#define SLI_BT_FEATURE_SYNC
#define SLI_BT_BGAPI_SYNC
#endif

// Sync scanner feature is needed when specifically included or when sync
// backwards compatibility needs to be enabled
#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_SCANNER_PRESENT) \
  || defined(SLI_BT_ENABLE_SYNC_BACKWARDS_COMPATIBILITY)
SLI_BT_DECLARE_FEATURE(bt, sync_scanner);
#define SLI_BT_FEATURE_SYNC_SCANNER SLI_BT_USE_FEATURE(bt, sync_scanner),
#else
#define SLI_BT_FEATURE_SYNC_SCANNER
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_SCANNER_PRESENT)
#define SLI_BT_BGAPI_SYNC_SCANNER SLI_BT_USE_BGAPI_CLASS(bt, sync_scanner),
#else
#define SLI_BT_BGAPI_SYNC_SCANNER
#endif

#if defined(SLI_BT_ENABLE_SYNC_BACKWARDS_COMPATIBILITY)
SLI_BT_DECLARE_FEATURE(bt, sync_compatibility);
#define SLI_BT_FEATURE_SYNC_COMPATIBILITY SLI_BT_USE_FEATURE(bt, sync_compatibility),
#else
#define SLI_BT_FEATURE_SYNC_COMPATIBILITY
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_SYNC_PRESENT)
#define SLI_BT_BGAPI_PERIODIC_SYNC SLI_BT_USE_BGAPI_CLASS(bt, periodic_sync),
#else
#define SLI_BT_BGAPI_PERIODIC_SYNC
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_SYNC_PRESENT)
#define SLI_BT_BGAPI_PAWR_SYNC SLI_BT_USE_BGAPI_CLASS(bt, pawr_sync),
#else
#define SLI_BT_BGAPI_PAWR_SYNC
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAST_RECEIVER_PRESENT)
#define SLI_BT_BGAPI_PAST_RECEIVER SLI_BT_USE_BGAPI_CLASS(bt, past_receiver),
#else
#define SLI_BT_BGAPI_PAST_RECEIVER
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PAST_PRESENT)
#define SLI_BT_BGAPI_ADVERTISER_PAST SLI_BT_USE_BGAPI_CLASS(bt, advertiser_past),
#else
#define SLI_BT_BGAPI_ADVERTISER_PAST
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PAST_PRESENT)
#define SLI_BT_BGAPI_SYNC_PAST SLI_BT_USE_BGAPI_CLASS(bt, sync_past),
#else
#define SLI_BT_BGAPI_SYNC_PAST
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CS_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, cs);
#define SLI_BT_FEATURE_CS SLI_BT_USE_FEATURE(bt, cs),
#define SLI_BT_BGAPI_CS SLI_BT_USE_BGAPI_CLASS(bt, cs),
#else
#define SLI_BT_FEATURE_CS
#define SLI_BT_BGAPI_CS
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CS_TEST_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, cs_test);
#define SLI_BT_FEATURE_CS_TEST SLI_BT_USE_FEATURE(bt, cs_test),
#define SLI_BT_BGAPI_CS_TEST SLI_BT_USE_BGAPI_CLASS(bt, cs_test),
#else
#define SLI_BT_FEATURE_CS_TEST
#define SLI_BT_BGAPI_CS_TEST
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_L2CAP_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, l2cap);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, l2cap);
#define SLI_BT_FEATURE_L2CAP SLI_BT_USE_FEATURE_WITH_CONFIG(bt, l2cap, SLI_BT_FEATURE_CONFIG_NAME(bt, l2cap)),
#define SLI_BT_BGAPI_L2CAP SLI_BT_USE_BGAPI_CLASS(bt, l2cap),
#else
#define SLI_BT_FEATURE_L2CAP
#define SLI_BT_BGAPI_L2CAP
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, connection);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, connection);
#define SLI_BT_FEATURE_CONNECTION SLI_BT_USE_FEATURE_WITH_CONFIG(bt, connection, SLI_BT_FEATURE_CONFIG_NAME(bt, connection)),
#define SLI_BT_BGAPI_CONNECTION SLI_BT_USE_BGAPI_CLASS(bt, connection),
#else
#define SLI_BT_FEATURE_CONNECTION
#define SLI_BT_BGAPI_CONNECTION
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_STATISTICS_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, connection_statistics);
#define SLI_BT_FEATURE_CONNECTION_STATISTICS SLI_BT_USE_FEATURE(bt, connection_statistics),
#else
#define SLI_BT_FEATURE_CONNECTION_STATISTICS
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_USER_POWER_CONTROL_PRESENT) \
  && defined(SL_CATALOG_BLUETOOTH_FEATURE_POWER_CONTROL_PRESENT)
#error bluetooth_feature_power_control and bluetooth_feature_user_power_control cannot coexist.
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_POWER_CONTROL_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, power_control);
#define SLI_BT_FEATURE_POWER_CONTROL SLI_BT_USE_FEATURE(bt, power_control),
#else
#define SLI_BT_FEATURE_POWER_CONTROL
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_USER_POWER_CONTROL_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, user_power_control);
#define SLI_BT_FEATURE_USER_POWER_CONTROL SLI_BT_USE_FEATURE(bt, user_power_control),
#else
#define SLI_BT_FEATURE_USER_POWER_CONTROL
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_GATT_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, gatt);
#define SLI_BT_FEATURE_GATT SLI_BT_USE_FEATURE(bt, gatt),
#define SLI_BT_BGAPI_GATT SLI_BT_USE_BGAPI_CLASS(bt, gatt),
#else
#define SLI_BT_FEATURE_GATT
#define SLI_BT_BGAPI_GATT
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_DYNAMIC_GATTDB_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, dynamic_gattdb);
SLI_BT_DECLARE_FEATURE_CONFIG(bt, dynamic_gattdb);
#define SLI_BT_FEATURE_DYNAMIC_GATTDB SLI_BT_USE_FEATURE_WITH_CONFIG(bt, dynamic_gattdb, SLI_BT_FEATURE_CONFIG_NAME(bt, dynamic_gattdb)),
#define SLI_BT_BGAPI_DYNAMIC_GATTDB SLI_BT_USE_BGAPI_CLASS(bt, gattdb),
#else
#define SLI_BT_FEATURE_DYNAMIC_GATTDB
#define SLI_BT_BGAPI_DYNAMIC_GATTDB
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_GATT_SERVER_PRESENT)
#define SLI_BT_BGAPI_GATT_SERVER SLI_BT_USE_BGAPI_CLASS(bt, gatt_server),
#else
#define SLI_BT_BGAPI_GATT_SERVER
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_AOA_RECEIVER_PRESENT) \
  || defined(SL_CATALOG_BLUETOOTH_FEATURE_AOD_RECEIVER_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, cte_receiver);
#define SLI_BT_FEATURE_CTE_RECEIVER SLI_BT_USE_FEATURE(bt, cte_receiver),
#define SLI_BT_BGAPI_CTE_RECEIVER SLI_BT_USE_BGAPI_CLASS(bt, cte_receiver),
#else
#define SLI_BT_FEATURE_CTE_RECEIVER
#define SLI_BT_BGAPI_CTE_RECEIVER
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_AOA_TRANSMITTER_PRESENT) \
  || defined(SL_CATALOG_BLUETOOTH_FEATURE_AOD_TRANSMITTER_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, cte_transmitter);
#define SLI_BT_FEATURE_CTE_TRANSMITTER SLI_BT_USE_FEATURE(bt, cte_transmitter),
#define SLI_BT_BGAPI_CTE_TRANSMITTER SLI_BT_USE_BGAPI_CLASS(bt, cte_transmitter),
#else
#define SLI_BT_FEATURE_CTE_TRANSMITTER
#define SLI_BT_BGAPI_CTE_TRANSMITTER
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_TEST_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, test);
#define SLI_BT_FEATURE_TEST SLI_BT_USE_FEATURE(bt, test),
#define SLI_BT_BGAPI_TEST SLI_BT_USE_BGAPI_CLASS(bt, test),
#else
#define SLI_BT_FEATURE_TEST
#define SLI_BT_BGAPI_TEST
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_WHITELISTING_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, whitelisting);
#define SLI_BT_FEATURE_WHITELISTING SLI_BT_USE_FEATURE(bt, whitelisting),
#else
#define SLI_BT_FEATURE_WHITELISTING
#endif

#if defined(SL_CATALOG_RAIL_UTIL_COEX_PRESENT)
#define SLI_BT_BGAPI_COEX SLI_BT_USE_BGAPI_CLASS(bt, coex),
#else
#define SLI_BT_BGAPI_COEX
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_RESOURCE_REPORT_PRESENT)
#define SLI_BT_BGAPI_RESOURCE SLI_BT_USE_BGAPI_CLASS(bt, resource),
#else
#define SLI_BT_BGAPI_RESOURCE
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_USE_ACCURATE_API_ADDRESS_TYPES_PRESENT)
SLI_BT_DECLARE_FEATURE(bt, accurate_api_address_types);
#define SLI_BT_FEATURE_ACCURATE_API_ADDRESS_TYPES SLI_BT_USE_FEATURE(bt, accurate_api_address_types),
#else
#define SLI_BT_FEATURE_ACCURATE_API_ADDRESS_TYPES
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_ANALYZER_PRESENT)
#define SLI_BT_BGAPI_CONNECTION_ANALYZER SLI_BT_USE_BGAPI_CLASS(bt, connection_analyzer),
#else
#define SLI_BT_BGAPI_CONNECTION_ANALYZER
#endif

/** @brief Structure that specifies the Bluetooth configuration */
static const sl_bt_configuration_t bt_config = SL_BT_CONFIG_DEFAULT;

/** @brief Table of used Bluetooth features */
static const struct sli_bt_feature_use bt_used_features[] =
{
  // Invoke the feature inclusion macro for each feature. Depending on the build
  // configuration, the feature inclusion rules above have defined the macro to
  // either empty or the relevant feature use declaration.
  SLI_BT_FEATURE_ON_DEMAND_START
  SLI_BT_FEATURE_SYSTEM
  SLI_BT_FEATURE_SM
  SLI_BT_FEATURE_BUILTIN_BONDING_DATABASE
  SLI_BT_FEATURE_EXTERNAL_BONDING_DATABASE
  SLI_BT_FEATURE_ACCEPT_LIST
  SLI_BT_FEATURE_RESOLVING_LIST
  SLI_BT_FEATURE_SCANNER
  SLI_BT_FEATURE_SCANNER_COMPATIBILITY
  SLI_BT_FEATURE_SCANNER_BASE
  SLI_BT_FEATURE_EXTENDED_SCANNER
  SLI_BT_FEATURE_SYNC
  SLI_BT_FEATURE_SYNC_SCANNER
  SLI_BT_FEATURE_SYNC_COMPATIBILITY
  SLI_BT_FEATURE_ADVERTISER
  SLI_BT_FEATURE_ADVERTISER_COMPATIBILITY
  SLI_BT_FEATURE_EXTENDED_ADVERTISER
  SLI_BT_FEATURE_PERIODIC_ADV
  SLI_BT_FEATURE_PERIODIC_ADVERTISER
  SLI_BT_FEATURE_CS
  SLI_BT_FEATURE_CS_TEST
  SLI_BT_FEATURE_L2CAP
  SLI_BT_FEATURE_CONNECTION
  SLI_BT_FEATURE_CONNECTION_STATISTICS
  SLI_BT_FEATURE_DYNAMIC_GATTDB
  SLI_BT_FEATURE_CTE_RECEIVER
  SLI_BT_FEATURE_CTE_TRANSMITTER
  SLI_BT_FEATURE_TEST
  SLI_BT_FEATURE_POWER_CONTROL
  SLI_BT_FEATURE_USER_POWER_CONTROL
  SLI_BT_FEATURE_GATT
  SLI_BT_FEATURE_WHITELISTING
  SLI_BT_FEATURE_ACCURATE_API_ADDRESS_TYPES
  { NULL, NULL }
};

/** @brief Table of used BGAPI classes */
static const struct sli_bgapi_class * const bt_bgapi_classes[] =
{
  // Invoke the BGAPI class inclusion macro for each feature that provides a
  // BGAPI class. Depending on the build configuration, the feature inclusion
  // rules above have defined the macro to either empty or the relevant BGAPI
  // class declaration.
  SLI_BT_BGAPI_SYSTEM
  SLI_BT_BGAPI_NVM
  SLI_BT_BGAPI_OTA_CONFIG
  SLI_BT_BGAPI_GAP
  SLI_BT_BGAPI_SM
  SLI_BT_BGAPI_EXTERNAL_BONDINGDB
  SLI_BT_BGAPI_ACCEPT_LIST
  SLI_BT_BGAPI_RESOLVING_LIST
  SLI_BT_BGAPI_ADVERTISER
  SLI_BT_BGAPI_LEGACY_ADVERTISER
  SLI_BT_BGAPI_EXTENDED_ADVERTISER
  SLI_BT_BGAPI_PERIODIC_ADVERTISER
  SLI_BT_BGAPI_PAWR_ADVERTISER
  SLI_BT_BGAPI_SCANNER
  SLI_BT_BGAPI_SYNC
  SLI_BT_BGAPI_SYNC_SCANNER
  SLI_BT_BGAPI_PERIODIC_SYNC
  SLI_BT_BGAPI_PAWR_SYNC
  SLI_BT_BGAPI_PAST_RECEIVER
  SLI_BT_BGAPI_ADVERTISER_PAST
  SLI_BT_BGAPI_SYNC_PAST
  SLI_BT_BGAPI_CS
  SLI_BT_BGAPI_CS_TEST
  SLI_BT_BGAPI_L2CAP
  SLI_BT_BGAPI_CONNECTION
  SLI_BT_BGAPI_GATT
  SLI_BT_BGAPI_DYNAMIC_GATTDB
  SLI_BT_BGAPI_GATT_SERVER
  SLI_BT_BGAPI_CTE_RECEIVER
  SLI_BT_BGAPI_CTE_TRANSMITTER
  SLI_BT_BGAPI_TEST
  SLI_BT_BGAPI_COEX
  SLI_BT_BGAPI_RESOURCE
  SLI_BT_BGAPI_CONNECTION_ANALYZER
  NULL
};

// Forward declaration of Bluetooth controller init functions
extern void ll_addrEnable();
extern sl_status_t sl_bt_ll_deinit();
#include "sl_bt_ll_config.h"
extern sl_status_t ll_connPowerControlEnable(const sl_bt_ll_power_control_config_t *);
extern void sl_bt_init_app_controlled_tx_power();
extern sl_status_t sl_btctrl_init_sniff(uint8_t);
extern void sl_btctrl_deinit_sniff(void);
#if defined(SL_CATALOG_RAIL_UTIL_COEX_PRESENT)
#include "coexistence-ble.h"
#endif

/**
 * @brief Initialize controller features according to the feature selection.
 *
 * This function is called by the Bluetooth host stack when Bluetooth is started.
 */
sl_status_t sli_bt_init_controller_features()
{
  sl_status_t status = SL_STATUS_OK;

#if defined(SL_CATALOG_RAIL_UTIL_COEX_PRESENT)
  sl_bt_init_coex_hal();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_MULTIPROTOCOL_PRESENT)
  sl_btctrl_init_multiprotocol();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_RADIO_WATCHDOG_PRESENT)
  sl_btctrl_enable_radio_watchdog();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PRESENT)
  sl_btctrl_init_adv();
#endif

#if defined(SLI_BT_ENABLE_EXTENDED_ADVERTISER_FEATURE)
  sl_btctrl_init_adv_ext();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SCANNER_PRESENT)
  sl_btctrl_init_scan();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_EVEN_SCHEDULING_PRESENT)
  sl_btctrl_enable_even_connsch();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PAWR_SCHEDULING_PRESENT)
  sl_btctrl_enable_pawr_connsch();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_PRESENT)
  sl_btctrl_init_conn();
#if !defined(SL_CATALOG_BLUETOOTH_CONNECTION_PHY_UPDATE_INCOMPATIBLE_PRESENT)
  sl_btctrl_init_phy();
#endif
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_STATISTICS_PRESENT)
  sl_btctrl_init_conn_statistics();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_POWER_CONTROL_PRESENT)
#include "sl_bt_power_control_config.h"
  const sl_bt_ll_power_control_config_t power_control_config = {
    .activate_power_control = SL_BT_ACTIVATE_POWER_CONTROL,
    .golden_rssi_min_1m = SL_BT_GOLDEN_RSSI_MIN_1M,
    .golden_rssi_max_1m = SL_BT_GOLDEN_RSSI_MAX_1M,
    .golden_rssi_min_2m = SL_BT_GOLDEN_RSSI_MIN_2M,
    .golden_rssi_max_2m = SL_BT_GOLDEN_RSSI_MAX_2M,
    .golden_rssi_min_coded_s8 = SL_BT_GOLDEN_RSSI_MIN_CODED_S8,
    .golden_rssi_max_coded_s8 = SL_BT_GOLDEN_RSSI_MAX_CODED_S8,
    .golden_rssi_min_coded_s2 = SL_BT_GOLDEN_RSSI_MIN_CODED_S2,
    .golden_rssi_max_coded_s2 = SL_BT_GOLDEN_RSSI_MAX_CODED_S2
  };

  status = ll_connPowerControlEnable(&power_control_config);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_USER_POWER_CONTROL_PRESENT)
  sl_bt_init_app_controlled_tx_power();
#endif

#if defined(SLI_BT_ENABLE_EXTENDED_SCANNER_FEATURE)
  sl_btctrl_init_scan_ext();
#endif

#if defined(SLI_BT_ENABLE_PERIODIC_ADV_FEATURE)
  // A build with the host stack doesn't have the dedicated config file
  // "sl_bluetooth_periodic_adv_config.h". We use the global advertiser count to
  // enable all advertisers as periodic advertisers.
#include "sl_bluetooth.h" // For SL_BT_COMPONENT_ADVERTISERS
#include "sl_bluetooth_config.h"
  sl_btctrl_init_periodic_adv();
  status = sl_btctrl_alloc_periodic_adv(SL_BT_CONFIG_MAX_ADVERTISERS);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_ADVERTISER_PRESENT)
#include "sl_bt_periodic_advertiser_config.h"
  sl_btctrl_init_periodic_adv();
  sl_btctrl_alloc_periodic_adv(SL_BT_CONFIG_MAX_PERIODIC_ADVERTISERS);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_ADVERTISER_PRESENT)
#include "sl_bt_pawr_advertiser_config.h"
  struct sl_btctrl_pawr_advertiser_config pawr_config = {
      .max_pawr_sets = SL_BT_CONFIG_MAX_PAWR_ADVERTISERS,
      .max_advertised_data_length_hint = SL_BT_CONFIG_MAX_PAWR_ADVERTISED_DATA_LENGTH_HINT,
      .subevent_data_request_count = SL_BT_CONFIG_PAWR_PACKET_REQUEST_COUNT,
      .subevent_data_request_advance = SL_BT_CONFIG_PAWR_PACKET_REQUEST_ADVANCE,
  };
  status = sl_btctrl_pawr_advertiser_configure(&pawr_config);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PRESENT)
#include "sl_bluetooth_periodic_sync_config.h"
  sl_btctrl_init_periodic_scan();
  status = sl_btctrl_alloc_periodic_scan(SL_BT_CONFIG_MAX_PERIODIC_ADVERTISING_SYNC);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_SYNC_PRESENT)
#include "sl_bt_pawr_sync_config.h"
  struct sl_btctrl_pawr_synchronizer_config pawr_sync_config = {
      .max_pawr_sets = SL_BT_CONFIG_MAX_PAWR_SYNCHRONIZERS,
  };
  status = sl_btctrl_pawr_synchronizer_configure(&pawr_sync_config);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_RESOLVING_LIST_PRESENT)
#include "sl_bt_resolving_list_config.h"
  sl_btctrl_init_privacy();
  status = sl_btctrl_allocate_resolving_list_memory(SL_BT_CONFIG_RESOLVING_LIST_SIZE);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_AFH_PRESENT)
  status = sl_btctrl_init_afh(1);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_HIGH_POWER_PRESENT)
  sl_btctrl_init_highpower();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PHY_SUPPORT_CONFIG_PRESENT)
#include "sl_btctrl_phy_support_config.h"
#if SL_BT_CONTROLLER_2M_PHY_SUPPORT == 0
  sl_btctrl_disable_2m_phy();
#endif
#if SL_BT_CONTROLLER_CODED_PHY_SUPPORT == 0
  sl_btctrl_disable_coded_phy();
#endif
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_WHITELISTING_PRESENT)
  ll_addrEnable();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_AOA_RECEIVER_PRESENT) \
  || defined(SL_CATALOG_BLUETOOTH_FEATURE_AOD_RECEIVER_PRESENT)
  status = sl_btctrl_init_cte_receiver();
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_AOA_TRANSMITTER_PRESENT) \
  || defined(SL_CATALOG_BLUETOOTH_FEATURE_AOD_TRANSMITTER_PRESENT)
  status = sl_btctrl_init_cte_transmitter();
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_ADVERTISER_PAST_PRESENT)
  sl_btctrl_init_past_local_sync_transfer();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PAST_PRESENT)
  sl_btctrl_init_past_remote_sync_transfer();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAST_RECEIVER_PRESENT)
  sl_btctrl_init_past_receiver();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CS_PRESENT)
#include "sl_bluetooth_cs_config.h"
  struct sl_btctrl_cs_config cs_config = { 0 };
  cs_config.configs_per_connection = SL_BT_CONFIG_MAX_CS_CONFIGS_PER_CONNECTION;
  cs_config.procedures = SL_BT_CONFIG_MAX_CS_PROCEDURES;
  sl_btctrl_init_cs(&cs_config);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_ANALYZER_PRESENT)
#include "sl_bluetooth_connection_analyzer_config.h"
  status = sl_btctrl_init_sniff(SL_BT_CONFIG_MAX_CONNECTION_ANALYZERS);
  if (status != SL_STATUS_OK) {
    return status;
  }
#endif

  return status;
}

/**
 * @brief De-initialize controller features according to the feature selection.
 *
 * This function is called by the Bluetooth host stack when Bluetooth is stopped.
 */
void sli_bt_deinit_controller_features()
{
#if defined(SL_CATALOG_BLUETOOTH_FEATURE_CONNECTION_ANALYZER_PRESENT)
  sl_btctrl_deinit_sniff();
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_RESOLVING_LIST_PRESENT)
  sl_btctrl_allocate_resolving_list_memory(0);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_SYNC_PRESENT)
  sl_btctrl_alloc_periodic_scan(0);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_ADVERTISER_PRESENT)
  struct sl_btctrl_pawr_advertiser_config pawr_config = {
      .max_pawr_sets = 0,
      .max_advertised_data_length_hint = 0,
      .subevent_data_request_count = 0,
      .subevent_data_request_advance = 0,
  };
  (void) sl_btctrl_pawr_advertiser_configure(&pawr_config);
#endif

#if defined(SL_CATALOG_BLUETOOTH_FEATURE_PAWR_SYNC_PRESENT)
  struct sl_btctrl_pawr_synchronizer_config pawr_sync_config = {
      .max_pawr_sets = 0,
  };
  (void) sl_btctrl_pawr_synchronizer_configure(&pawr_sync_config);
#endif

#if defined(SLI_BT_ENABLE_PERIODIC_ADV_FEATURE) \
  || defined(SL_CATALOG_BLUETOOTH_FEATURE_PERIODIC_ADVERTISER_PRESENT)
  (void) sl_btctrl_alloc_periodic_adv(0);
#endif

  (void) sl_bt_ll_deinit();
}

// Initialize the Bluetooth stack.
sl_status_t sl_bt_stack_init()
{
  // Initialize the Bluetooth stack with the given configuration, features, and BGAPI classes
  return sli_bt_init_stack(&bt_config, bt_used_features, bt_bgapi_classes);
}