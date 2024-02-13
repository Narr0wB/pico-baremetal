
#include "resets.h"
#include "memory.h"
#include "usb_struct.h"
#include "interrupts.h"

void usb_init() {
    // Reset usb controller
    reset_block(RESETS_RESET_USBCTRL_BITS);

    // Take controller out of reset (Wait until it gets out of reset)
    unreset_block_wait(RESETS_RESET_USBCTRL_BITS);

    // Clear usb dpram
    memset(usb_dpram, 0, sizeof(*usb_dpram));

    // Enable usb interrupt
    enable_interrupt(USBCTRL_IRQ);

    // Mux the controller to the PHY (physical) interface
    usb_hw->muxing = USB_USB_MUXING_TO_PHY_BITS | USB_USB_MUXING_SOFTCON_BITS;

    // Force VBUS detect so the device thinks it is plugged into a host
    usb_hw->pwr = USB_USB_PWR_VBUS_DETECT_BITS | USB_USB_PWR_VBUS_DETECT_OVERRIDE_EN_BITS;

    // Enable the USB controller in device mode.
    usb_hw->main_ctrl = USB_MAIN_CTRL_CONTROLLER_EN_BITS;

    // Enable an interrupt per EP0 transaction
    usb_hw->sie_ctrl = USB_SIE_CTRL_EP0_INT_1BUF_BITS;

    // Enable interrupts for when a buffer is done, when the bus is reset,
    // and when a setup packet is received
    usb_hw->inte = USB_INTS_BUFF_STATUS_BITS |
                   USB_INTS_BUS_RESET_BITS |
                   USB_INTS_SETUP_REQ_BITS;

    return;
}

int main(void) {
    usb_init();

    for (;;) {};

    return 0;
}