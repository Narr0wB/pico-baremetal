
#ifndef INT_H
#define INT_H

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef short int16_t;
typedef char int8_t;

typedef unsigned long long size_t;
typedef unsigned int uintptr_t;

#define write32(addr, value) *((uint32_t*) (addr)) = value

#endif // INT_H