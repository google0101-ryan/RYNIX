#ifndef CORE_TSS_H
#define CORE_TSS_H

#include <stdint.h>

// This is purely for reference
#define KERNEL_BASE_SELECTOR 0x08
#define USER_BASE_SELECTOR   0x18
#define TSS_SEGMENT          0x30

typedef struct gdt_descriptor
{
  uint16_t limit15_0;
  uint16_t base15_0;
  uint8_t base23_16;
  uint8_t type;
  uint8_t limit19_16_and_flags;
  uint8_t base31_24;
} gdt_descriptor_t;

typedef struct gdt_table
{
  gdt_descriptor_t null_1;
  gdt_descriptor_t kernel_code;
  gdt_descriptor_t kernel_data;
  gdt_descriptor_t null_2;
  gdt_descriptor_t user_data;
  gdt_descriptor_t user_code;
  gdt_descriptor_t tss_low;
  gdt_descriptor_t tss_high;
} gdt_table_t;

typedef struct tss
{
  uint32_t reserved0;
  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;
  uint64_t reserved1;
  uint64_t ist1;
  uint64_t ist2;
  uint64_t ist3;
  uint64_t ist4;
  uint64_t ist5;
  uint64_t ist6;
  uint64_t ist7;
  uint64_t reserved2;
  uint16_t reserved3;
  uint16_t iopb_offset;
} tss_t;

void tss_init();

#endif