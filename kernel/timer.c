#include <mikoOS/kernel.h>
#include <mikoOS/timer.h>
#include "interrupt.h"

// counter
typedef u_int64_t timer_t;
timer_t counter = 0;

/////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////
static void timer_handler(struct registers regs);

/**
 * Timer handler test function currently.
 */
static void timer_handler(struct registers regs)
{
	if (counter != 0xffffffff) 
		counter++;
	else
		counter = 0;

}

/////////////////////////////////////////////////
// public functions
/////////////////////////////////////////////////

/**
 * Initialize system timer.
 */
void timer_init(void)
{
	set_handler_func(0x20, &timer_handler);
}

/**
 * Wait seconds.
 * @param how many seconds do you wait?
 */
inline void wait_loop_sec(u_int32_t sec)
{
	timer_t old = counter;
	timer_t end = old + (sec * 100);

	while (counter < end)
		; // nothing.
}


/**
 * Wait usec.
 * @param How many seconds do you wait?
 */
inline void wait_loop_usec(u_int32_t usec)
{
	timer_t old = counter;
	timer_t end = old + (usec / 1000);

	while (counter < end)
		; // nothing.
}

