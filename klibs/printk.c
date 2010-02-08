#include <mikoOS/kernel.h>
#include <mikoOS/printk.h>
#include <mikoOS/stdlib.h>

#define PRINTK_BUF_SIZE (64 + 1)

#define COLUMNS (80)
#define LINES (24)
#define ATTRIBUTE (7)
#define VRAM 0xb8000

static int xpos;
static int ypos;

static volatile unsigned char *video;

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void kputc(int c);
static void kputs(char *s);

/**
 * It's same as putchar(3).
 * @param c is goint to be displayed.
 */
static void kputc(int c)
{
	if (c == '\n' || c == '\r') {
	newline:
		xpos = 0;
		ypos++;
		if (ypos >= LINES)
			ypos = 0;
		return;
	}
	
	*(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
	*(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;
	
	xpos++;
	if (xpos >= COLUMNS)
		goto newline;
}

/**
 * It's same as puts(3).
 * @param s is a string to display.
 */
static void kputs(char *s)
{
	while (*s)
		kputc(*s++);
}


/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////

/**
 * Clear screen
 */
void cls(void)
{
	int i;
	
	video = (unsigned char *) VRAM;
	
	for (i = 0; i < COLUMNS * LINES * 2; i++)
		*(video + i) = 0;
	
	xpos = 0;
	ypos = 0;
}

/**
 * It's a simple printf(3) for kernel.
 * @param fmt as format strings.
 * @param arguments for format strings.
 */
void printk(char *fmt, ...) 
{
        va_list ap;
        int ival;
        char *sval;

        va_start(ap, fmt);
        while (*fmt) {
                char buf[PRINTK_BUF_SIZE] = { 0 };
		if (*fmt == '%') {
                        switch (*++fmt) {
                        case 'd':
                        case 'c':
			case 'u':
                                ival = va_arg(ap, int);
                                itoa(ival, buf);
                                kputs(buf);
                                break;
                        case 'x':
                                ival = va_arg(ap, int);
                                itox(ival, buf);
                                kputs(buf);
                                break;
                        case 's':
                                sval = va_arg(ap, char *);
				kputs(sval);
                                break;
                        default:
				break;
			}
                        fmt++;
                } else
                        kputc(*fmt++);
        }

        va_end(ap);
}
