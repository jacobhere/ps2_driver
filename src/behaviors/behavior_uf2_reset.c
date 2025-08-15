#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zmk/behavior.h>
#include <zmk/matrix.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>

#include "nrf_nvic.h"

#if DT_HAS_COMPAT_STATUS_OKAY(zmk_behavior_uf2_reset)

void reset_to_uf2(void) {
    NRF_POWER->GPREGRET = 0x57; // 0xA8 OTA, 0x4e Serial
    NVIC_SystemReset();         // or sd_nvic_SystemReset();
}

static int behavior_uf2_reset_init(const struct device *dev) { return 0; }

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    reset_to_uf2();
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct zmk_behavior_driver_api behavior_uf2_reset_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
};

#define UF2_RESET_INST(n)                                                                         \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_uf2_reset_init, NULL, NULL, NULL, APPLICATION,            \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_uf2_reset_driver_api);

DT_INST_FOREACH_STATUS_OKAY(UF2_RESET_INST)

#endif
