
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "intctrl.h"
#include "m0plus.h"
#include "addressmap.h"
#include "int.h"

void enable_interrupt(uint8_t int_code) {
    if (int_code > 24) return;

    uint32_t mask = 1u << int_code;

    // Clear any pending interrupts
    write32(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET, mask);

    // Enable interrupt in the ISER register
    write32(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET, mask);
}

void disable_interrupt(uint8_t int_code) {
    if (int_code > 24) return;

    uint32_t mask = 1u << int_code;

    // Disable interrupt in the ICER register
    write32(PPB_BASE + M0PLUS_NVIC_ICER_OFFSET, mask);
}

#endif // INTERRUPTS_H