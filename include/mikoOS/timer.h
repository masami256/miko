#ifndef __MIKOOS_TIMER_H
#define __MIKOOS_TIMER_H 1

void timer_init(void);

inline void wait_loop_sec(u_int32_t sec);
inline void wait_loop_usec(u_int32_t usec);

#endif // __MIKOOS_TIMER_H
