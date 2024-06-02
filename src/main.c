
#include "resets.h"
#include "usb.h"
#include "usb_common.h"
#include "usb_struct.h"
#include "interrupts.h"
#include "usb_test.h"
#include "rom_table.h"

static uint8_t dev_addr = 0;
static uint8_t configured = 0;
static uint8_t ep0_buf[64];

void ep0_in_handler(uint8_t *buf, uint16_t len);
void ep0_out_handler(uint8_t *buf, uint16_t len);

static struct usb_endpoint_configuration ep0_in_config = {
  .descriptor = &ep0_in_desc,
  .handler = &ep0_in_handler,
  .endpoint_control = NULL,
  .buffer_control = &usb_dpram->ep_buf_ctrl[0].in,
  .data_buffer = &usb_dpram->ep0_buf_a[0]
};

static struct usb_endpoint_configuration ep0_out_config = {
  .descriptor = &ep0_out_desc,
  .handler = &ep0_out_handler,
  .endpoint_control = NULL,
  .buffer_control = &usb_dpram->ep_buf_ctrl[0].out,
  .data_buffer = &usb_dpram->ep0_buf_a[0]
};

void usb_start_transfer(struct usb_endpoint_configuration *ep, uint8_t *buf, uint16_t len) {
  if (len > 64) {
    return;
  }

  // Prepare the value to put in the buffer control register
  uint32_t val = len | USB_BUF_CTRL_AVAIL;
  
  // If the given endpoint is a transmit endpoint then copy data into buffer and get ready to send it
  if (ep->descriptor->bEndpointAddress & USB_DIR_IN && buf != NULL) {
    // Copy data into endpoint buffer
    memcpy((void*) ep->data_buffer, (void*)buf, len);
    
    // Mark as full
    val |= USB_BUF_CTRL_FULL;
  }

  val |= ep->next_pid ? USB_BUF_CTRL_DATA1_PID : USB_BUF_CTRL_DATA0_PID;
  ep->next_pid ^= 1u;

  write32(ep->buffer_control, val);
}

void usb_acknowledge_out_request() {
  usb_start_transfer(&ep0_in_config, NULL, 0);
}

void usb_set_device_address(volatile struct usb_setup_packet *pkt) {
  dev_addr = (pkt->wValue & 0xFF);
  usb_hw->dev_addr_ctrl = dev_addr;
  usb_acknowledge_out_request();
}

void usb_set_device_configuration(volatile struct usb_setup_packet *pkt) {
  usb_acknowledge_out_request();
  configured = 1;
}

void usb_handle_setup_packet(void) {
  volatile struct usb_setup_packet *pkt = (volatile struct usb_setup_packet*) &usb_dpram->setup_packet;
  uint8_t req_direction = pkt->bmRequestType;
  uint8_t req = pkt->bRequest;
  
  ep0_in_config.next_pid = 1u;

  if (req_direction == USB_DIR_OUT) {
    if (req == USB_REQUEST_SET_ADDRESS) {
      usb_set_device_address(pkt);
    }
    else if (req == USB_REQUEST_SET_CONFIGURATION) {
      usb_set_device_configuration(pkt);
    }
    else {
      // If the request is something other than setting the device address or setting the device configuration then just acknowlege the request and do nothing
      usb_acknowledge_out_request();
    } 
  }
  else if (req_direction == USB_DIR_IN) {
    if (req == USB_REQUEST_GET_DESCRIPTOR) {
      uint16_t descriptor_type = pkt->wValue >> 8;
      
      switch (descriptor_type) {
        case USB_DT_DEVICE: {
          // usb_handle_device_descriptor(pkt);
          break;
        }
        case USB_DT_CONFIG: {
          // usb_handle_config_descriptor(pkt);
          break;
        }
        case USB_DT_STRING: {
          // usb_handle_string_descriptor(pkt); 
          break;
        }

        default:
          // Do nothing if the request is not supported 
          return;
      }
    }
  }
  
}

void usb_bus_reset(void) {
  dev_addr = 0;
  usb_hw->dev_addr_ctrl = 0;
  configured = 0;
}

void usb_handle_buff_status(void) {
  uint32_t buffers = usb_hw->buf_status;
  
  uint ep0_in_buf  = 1u << 0;
  uint ep0_out_buf = 1u << 1;

  if (buffers & ep0_in_buf) {
    hw_clear_bits(&usb_hw->buf_status, ep0_in_buf);
    buffers &= ~ep0_in_buf;

    uint32_t buffer_control = *ep0_in_config.buffer_control;
    uint16_t len = buffer_control & USB_BUF_CTRL_LEN_MASK;

    ep0_in_config.handler((uint8_t*) ep0_in_config.data_buffer, len);
  }

  if (buffers & ep0_out_buf) {
    hw_clear_bits(&usb_hw->buf_status, ep0_out_buf);
    buffers &= ~ep0_out_buf;

    uint32_t buffer_control = *ep0_out_config.buffer_control;
    uint16_t len = buffer_control & USB_BUF_CTRL_LEN_MASK;

    ep0_out_config.handler((uint8_t*) ep0_out_config.data_buffer, len);
  }
}

// Interrupt service routine for usb events and interrupts
void usb_isr(void) {
  uint32_t status = usb_hw->ints;
  uint32_t handled = 0;

  // If setup packet is received
  if (status & USB_INTS_SETUP_REQ_BITS) {
    handled |= USB_INTS_SETUP_REQ_BITS;
     
    // Clear the interrupt bits
    hw_set_bits(&usb_hw->sie_status, USB_SIE_STATUS_SETUP_REC_BITS);

    usb_handle_setup_packet();
  }
  
  // If any of the buffers associated with the endpoints have completed transfer
  if (status & USB_INTS_BUFF_STATUS_BITS) {
    handled |= USB_INTS_BUFF_STATUS_BITS;
    usb_handle_buff_status();
  }

  if (status & USB_INTS_BUS_RESET_BITS) {
    handled |= USB_INTS_BUS_RESET_BITS;

    // Clear the interrupt bits
    hw_set_bits(&usb_hw->sie_status, USB_SIE_STATUS_BUS_RESET_BITS);
    
    usb_bus_reset();
  }
}

void usb_init() {
    // Reset usb controller
    reset_block(RESETS_RESET_USBCTRL_BITS);

    // Take controller out of reset (Wait until it gets out of reset)
    unreset_block_wait(RESETS_RESET_USBCTRL_BITS);

    // Clear usb dpram
    memset((uint8_t*) usb_dpram, 0, sizeof(*usb_dpram));

    // Enable usb interrupt
    enable_interrupt(USBCTRL_IRQ);

    // Setup interrupt function
    // set_isr(USBCTRL_IRQ, usb_isr);

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
    
    // Signal to the host that this is a high speed device
    usb_hw->sie_ctrl = USB_SIE_CTRL_PULLUP_EN_BITS;
    return;
}

void usb_ep_available(struct usb_endpoint_configuration *ep) {
  hw_set_bits(ep->buffer_control, USB_BUF_CTRL_AVAIL);
  hw_clear_bits(ep->buffer_control, USB_BUF_CTRL_FULL);
}

void ep0_in_handler(uint8_t* buf, uint16_t len) {
  // After sending data to the host, we have to tell the host that we are ready to receive data again
  // After each successful transaction make ep0 out ready for a new OUT token
  usb_ep_available(&ep0_out_config);
}

void ep0_out_handler(uint8_t* buf, uint16_t len) {
  usb_ep_available(&ep0_out_config);
}


int main(void) {
    usb_init();

    for (;;) {};

    return 0;
}
