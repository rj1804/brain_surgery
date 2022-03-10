#include <string.h>
#include <stdlib.h>

#include "paint.h"
#include "oled.h"
#include "xprintf.h"

int invert = 0;
int color;

#define swap(a, b) ({int tmp = a; a = b; b = tmp;})
#define isgn(a) (a < 0 ? -1 : 1)

void paint_draw_hline(int x1, int x2, int y)
{
	if (x1 > x2)
		swap(x1, x2);
	int px;
	for ( px = x1; px <= x2; px++ )
		set_pixel( px, y );
}

void paint_draw_vline(int x, int y1, int y2)
{
	if (y1 > y2)
		swap(y1, y2);

	int py;
	for ( py = y1; py <= y2; py++ )
		set_pixel( x, py );
}

void paint_draw_line( int x1, int y1, int x2, int y2 )
{
	int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

	dx = x2 - x1;		/* the horizontal distance of the line */
	dy = y2 - y1;		/* the vertical distance of the line */
	dxabs = abs( dx );
	dyabs = abs( dy );
	sdx = isgn( dx );
	sdy = isgn( dy );
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;

	if ( dxabs >= dyabs ) {	/* the line is more horizontal than vertical */
		for ( i = 0; i < dxabs; i++ ) {
			y += dyabs;
			if ( y >= dxabs ) {
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			set_pixel( px, py );
		}
	} else {		/* the line is more vertical than horizontal */
		for ( i = 0; i < dyabs; i++ ) {
			x += dxabs;
			if ( x >= dyabs ) {
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			set_pixel( px, py );
		}
	}
}

void paint_draw_rect( int x, int y, int width, int height )
{
	paint_draw_hline( x, x + width - 1, y );
	paint_draw_hline( x, x + width - 1, y + height - 1 );
	paint_draw_vline( x, y, y + height - 1 );
	paint_draw_vline( x + width - 1, y, y + height - 1 );
}

void paint_draw_fillrect( int x, int y, int width, int height )
{
	int i, y2 = y + height - 1;
	for ( i = x; i < x + width; i++ )
		paint_draw_vline( i, y, y2 );
}

int paint_text_size(const char* fmt, ...)
{
	char str[64];

	va_list ap;
	va_start (ap, fmt);	
	xvsprintf(str, fmt, ap);
	va_end (ap);

	char *c = str;
	int size = 0;
	while(*c) {
		size += paint_char_width(*c);
		c++;
	}
	return size;
}

void paint_draw_char( int x, int y, int c, int size )
{
	int i, j, stride;
	const uint8_t *bitmap;

	if ( c < FONT_SMALL.FirstAsciiCode )
		c = FONT_SMALL.FirstAsciiCode;
	stride = FONT_SMALL.GlyphBytesWidth;
	bitmap = &FONT_SMALL.GlyphBitmaps[( c - FONT_SMALL.FirstAsciiCode ) * FONT_SMALL.GlyphHeight * FONT_SMALL.GlyphBytesWidth];

	for ( j = 0; j < paint_text_height(); j++ ) {
		for ( i = 0; i < paint_char_width( c ); i++ ) {
			int bi = i >> 3;
			if ( ( bitmap[bi] << ( i & 0x7 ) ) & 0x80 ) {
				set_pixel( size * i + x, size * j + y );
				if( size == 2 ) {
					set_pixel( size * i + x + 1, size * j + y );
					set_pixel( size * i + x + 1, size * j + y + 1 );
					set_pixel( size * i + x,     size * j + y + 1 );
				}
			}
		}
		bitmap += stride;
	}
}

void paint_draw_str_size(int x, int y, int size, char *str)
{
	char *c = str;
	while(*c) {
		paint_draw_char(x, y, *c, size);
		x += paint_char_width(*c) * size;
		c++;
	}
}

void paint_draw_text_size(int x, int y, int size, const char* fmt, ...)
{
	char str[64];
       
        va_list ap;
        va_start (ap, fmt);
        xvsprintf(str, fmt, ap);
        va_end (ap);
 	
	paint_draw_str_size( x, y, size, str );
}

