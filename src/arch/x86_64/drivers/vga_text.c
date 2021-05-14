#include "vga_text.h"
#include <core/port.h>
#include <memset.h>

#define FB_COMMAND_PORT      0x3D4
#define FB_DATA_PORT         0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

uint16_t* framebuffer = (uint16_t*)VIDEO_ADDRESS;

struct vga_char {
    char ch: 8;
    char fc: 4; // foreground color
    char bc: 4; // background color
};

uint64_t x, y;

void vga_text_init()
{
    memset(framebuffer, 0, VGA_TEXT_HEIGHT * VGA_TEXT_WIDTH * sizeof(uint16_t));
    port_byte_out(0x3D4, 0x0A);
	port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xC0) | 0);
 
	port_byte_out(0x3D4, 0x0B);
	port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xE0) | 0);
}

void vga_text_write_at(char c, uint8_t scheme, uint8_t x, uint8_t y)
{
  if (x < VGA_TEXT_WIDTH) {
    if (y < VGA_TEXT_WIDTH) {
      framebuffer[y * VGA_TEXT_WIDTH + x] = scheme << 8 | (c & 0xFF);
    }
  }
}

void vga_text_move_cursor(uint8_t x, uint8_t y)
{
  uint16_t pos = y * VGA_TEXT_WIDTH + x;

  port_byte_out(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  port_byte_out(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
  port_byte_out(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  port_byte_out(FB_DATA_PORT, pos & 0x00FF);
}

void vga_text_disable_cursor()
{
  port_byte_out(FB_COMMAND_PORT, 0x0A);
  port_byte_out(FB_DATA_PORT, 0x20);
}

void _putchar(char character)
{
    unsigned short *loc;
    if (character == 0x08 && x)
    {
        x--;
        _putchar(' ');
        x--;
    }
    else if (character == 0x09)
    {
        x = (x + 8) & ~(7);
    }
    else if (character == '\r')
    {
        x = 0;
    }
    else if (character == '\n')
    {
        y++;
        x = 0;
    } else if (character == '\f')
    {
        ;
    }
    else if (character >= ' ')
    {
        framebuffer[x + y * 80] = character | COLOR_WHITE << 8;
        x++;
    }
    if (x >= 80)
    {
        _putchar('\n');
    }
    scroll();
    vga_text_move_cursor(x, y);
}

uint8_t get_text_attribute(uint8_t foreground, uint8_t background){
	return (background << 4) | (foreground & 0x0F);
}


/*
 * Calculates the 16-bit value of a character, foreground and background.
 */
uint16_t get_text_value(uint8_t character, uint8_t foreground, uint8_t background){
	return character | (get_text_attribute(foreground, background) << 8);
}

char vga_getchar(size_t x, size_t y)
{
    size_t offset = y * VGA_TEXT_HEIGHT + x;
    return framebuffer[offset];
}

void scroll()
{
    unsigned int blank = ' ' | COLOR_WHITE << 8;
    if(y >= 25) { 
        int i;
        for(i=0; i<24*80; i++) {
            framebuffer[i] = framebuffer[i+80];
        }
        for(i=24*80; i<25*80; i++) {
            framebuffer[i] = blank;
        }
        y = 24;
    }
}