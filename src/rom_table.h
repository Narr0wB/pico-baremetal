
#ifndef ROM_TABLE_H
#define ROM_TABLE_H

#include "address_mapped.h"

typedef void* (*rom_table_lookup_fn)(uint16_t *data_table, uint32_t code);
typedef uint8_t* (*memset_func)(uint8_t* dst, uint8_t val, uint32_t len);
typedef uint8_t* (*memcpy_func)(uint8_t* dst, uint8_t* src, uint32_t len);


#define hword_as_ptr(hword) (void*)(uintptr_t)(*(uint16_t*)(uintptr_t)(hword))

uint32_t rom_table_code(char c1, char c2) {
  return  (c2 << 8) | c1;
}

void *rom_func_lookup_inline(uint32_t code) {
  rom_table_lookup_fn rom_table_lookup = (rom_table_lookup_fn) hword_as_ptr(0x18);
  uint16_t* data_table = (uint16_t*) hword_as_ptr(0x18);
  return rom_table_lookup(data_table, code);
}

void *rom_func_lookup(uint32_t code) {
  return rom_func_lookup_inline(code);
}

#define memset(dst, val, len) ((memset_func)rom_func_lookup(rom_table_code('M', 'S')))(dst, val, len)
#define memcpy(dst, src, len) ((memcpy_func)rom_func_lookup(rom_table_code('M', 'C')))(dst, src, len)

#endif // !ROM_TABLE_H
