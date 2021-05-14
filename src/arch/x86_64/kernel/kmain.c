#include <core/multiboot.h>
#include <drivers/serial.h>
#include <drivers/vga_text.h>
#include <core/printf.h>
#include <core/tss.h>
#include <core/irq.h>
#include <drivers/pit.h>
#include <mmu/frame.h>

void print_ok()
{
  printf("  [");
  printf(" OK ");
  printf("]\n");
}


void kmain(uint64_t addr)
{
    multiboot_info_t* mbi = (multiboot_info_t*)addr;

    serial_init(SERIAL_COM1, SERIAL_SPEED_115200);
    serial_print(SERIAL_COM1, "INFO: Serial Inited\n");

    vga_text_init();

    printf("INFO: VGA Initied");
    serial_print(SERIAL_COM1, "INFO: VGA Initied\n");
    print_ok();

    extern gdt_table_t gdt64;

    tss_init();

    printf("INFO: TSS init");
    serial_print(SERIAL_COM1, "INFO: TSS init\n");
    print_ok();

    isr_init();

    printf("INFO: ISR init");
    print_ok();
    serial_print(SERIAL_COM1, "INFO: ISR init\n");

    timer_init();

    printf("INFO: PIT init");
    print_ok();
    serial_print(SERIAL_COM1, "INFO: PIT init\n");

    frame_init(mbi);

    printf("INFO: Frame init");
    print_ok();
    serial_print(SERIAL_COM1, "INFO: Frame init\n");

    for (;;);
}