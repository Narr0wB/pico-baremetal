
#ifndef USB_TEST_H
#define USB_TEST_H

#include "usb_common.h"

typedef void (*usb_ep_handler)(uint8_t *buf, uint16_t len);

struct usb_endpoint_configuration {
    const struct usb_endpoint_descriptor *descriptor;
    usb_ep_handler handler;

    // Pointers to endpoint + buffer control registers
    // in the USB controller DPSRAM
    volatile uint32_t *endpoint_control;
    volatile uint32_t *buffer_control;
    volatile uint8_t *data_buffer;

    // Toggle after each packet (unless replying to a SETUP)
    uint8_t next_pid;
};

static struct usb_endpoint_descriptor ep0_in_desc = {
  .bLength = sizeof(struct usb_endpoint_descriptor),
  .bDescriptorType = USB_DT_ENDPOINT,
  .bEndpointAddress = USB_DIR_IN | 0,
  .bmAttributes = USB_TRANSFER_TYPE_CONTROL,
  .wMaxPacketSize = 64,
  .bInterval = 0
};

static struct usb_endpoint_descriptor ep0_out_desc = {
  .bLength = sizeof(struct usb_endpoint_descriptor),
  .bDescriptorType = USB_DT_ENDPOINT,
  .bEndpointAddress = USB_DIR_OUT | 0,
  .bmAttributes = USB_TRANSFER_TYPE_CONTROL,
  .wMaxPacketSize = 64,
  .bInterval = 0
};




#endif // USB_TEST_H
