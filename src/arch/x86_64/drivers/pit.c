#include "pit.h"
#include <core/irq.h>
#include <core/port.h>
#include <drivers/vga_text.h>

uint64_t ticks = 0;
unsigned int tick;

static void timer_callback(isr_stack_t* stack)
{
    ticks++;

    if (tick == 0)
    {
        vga_text_write_at('|', COLOR_WHITE, 79, 0);
        tick++;
    } else if (tick == 1)
    {
        vga_text_write_at('/', COLOR_WHITE, 79, 0);
        tick++;
    } else if (tick == 2)
    {
        vga_text_write_at('-', COLOR_WHITE, 79, 0);
        tick++;
    } else if (tick == 3)
    {
        vga_text_write_at('\\', COLOR_WHITE, 79, 0);
        tick++;
    } else if (tick == 4)
    {
        vga_text_write_at('|', COLOR_WHITE, 79, 0);
        tick = 0;
    }
}

uint64_t timer_tick()
{
  return tick;
}

uint64_t timer_uptime()
{
  return timer_tick() / TIMER_HZ;
}

void timer_init()
{
  isr_register_handler(IRQ0, timer_callback);

  uint64_t divisor = 1193180 / TIMER_HZ;

  port_byte_out(0x43, 0x36);
  port_byte_out(0x40, divisor & 0xff);
  port_byte_out(0x40, divisor >> 8);
}
