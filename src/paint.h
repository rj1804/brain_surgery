#ifndef _PAINT_H
#define _PAINT_H

#include <stdint.h>
#include <stdarg.h>
#include "font.h"

#define LCD_WIDTH  128
#define LCD_HEIGHT  64
#define FB_SIZE		( LCD_WIDTH * LCD_HEIGHT / 8)

extern uint8_t fb[FB_SIZE];
extern int invert;

enum color {
	COLOR_NO,
	COLOR_FULL,
};

extern int color;

#define FONT_SMALL	(FontStyle_font_8)

void paint_draw_hline(int x1, int x2, int y);
void paint_draw_vline(int x, int y1, int y2);
void paint_draw_line(int x1, int y1, int x2, int y2);
void paint_draw_rect(int x, int y, int w, int h);
void paint_draw_fillrect(int x, int y, int width, int height);
void paint_draw_rect_rounded(int x, int y, int width, int height);
void paint_draw_fillrect_rounded(int x, int y, int width, int height);
void paint_draw_char(int x, int y, int c, int size);
int paint_text_size(const char* fmt, ...);
void paint_draw_text_size(int x, int y, int size, const char* fmt, ...);
void paint_draw_str_size(int x, int y, int size, char *str);

#define paint_draw_text(x, y, fmt, ...) \
	paint_draw_text_size(x, y, 1, fmt, ##__VA_ARGS__)

#define paint_draw_str(x, y, str) \
	paint_draw_str_size(x, y, 1, str)

static inline void paint_set_color(enum color c)
{
	color = c;
}

void paint_init(void);

#define paint_width(void)       LCD_WIDTH
#define paint_height(void)      LCD_HEIGHT
#define paint_text_height(void) 8

static inline int paint_char_width(int c)
{
	if (c < FONT_SMALL.FirstAsciiCode)
		c = FONT_SMALL.FirstAsciiCode;
	return FONT_SMALL.GlyphWidth[c-FONT_SMALL.FirstAsciiCode];
}

static inline void set_pixel(int x, int y)
{
	if( x & 0xFF80 || y & 0xFFC0) {
		return;
	}

	int on;
	switch (color) {
		case COLOR_NO:
			on = 0;
			break;
		default:
		case COLOR_FULL:
			on = 1;
			break;
	}
	if( invert ) {
		on = 1 - on;
	}		
	uint8_t *buf = &fb[(y / 8) * 128 + x];
	if (on)
		*buf |=   1 << (y & 7);
	else
		*buf &= ~(1 << (y & 7));
}

#define paint_set_pixel set_pixel

#endif
