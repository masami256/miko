#ifndef __MIKOOS_TYPES_H
#define __MIKOOS_TYPES_H

typedef enum {
	false = 0,
	true,
} bool;

#define NULL (void *)0

typedef signed char           int8_t;
typedef unsigned char       u_int8_t;
typedef short                int16_t;
typedef unsigned short     u_int16_t;
typedef long                  int32_t;
typedef unsigned long       u_int32_t;
typedef long long            int64_t;
typedef unsigned long long u_int64_t;

typedef int                  ssize_t;
typedef u_int32_t             size_t;

#endif // __MIKOOS_TYPES_H
