#ifndef __MIKOOS_ABORT_H
#define __MIKOOS_ABORT_H 1

#define KERN_ABORT(msg) kern_abort(__FILE__, __LINE__, __FUNCTION__, msg)

void kern_abort(const char *file, int line, const char *func, char *msg);

#endif // __MIKOOS_ABORT_H
