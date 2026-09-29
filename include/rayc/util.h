#pragma once

#define __USED __attribute__((used))
#define __PACKED __attribute__((packed))
#define __CONSTRUCTOR(__prio) __attribute__((constructor(__prio)))

#define sizeof_field(__type, __member) sizeof(((__type *)0)->__member)

#define MAX(__a, __b) ((__a) > (__b) ? (__a) : (__b))
#define MIN(__a, __b) ((__a) < (__b) ? (__a) : (__b))
#define SWAP(__a, __b) ({ __typeof__(__a) tmp = __a; __a = __b; __b = tmp; })
#define CAP_MIN(__val, __min) ((__val) < (__min) ? (__min) : (__val))
#define CAP_MAX(__val, __max) ((__val) > (__max) ? (__max) : (__val))
#define CLAMP(__val, __min, __max) ((__val) < (__min) ? (__min) : ((__val) > (__max) ? (__max) : (__val)))
#define ARR_COUNT(__arr) (sizeof(__arr) / sizeof(__arr[0]))

#define ASSERT_RET(__condition, ...) ({ if (!(__condition)) return __VA_ARGS__; })
#define ASSERT(__condition, __fmt, ...) ({ if (!(__condition)) die("[%s:%d %s] " __fmt, __FILE__, __LINE__, __FUNCTION__, ## __VA_ARGS__); })

[[noreturn]] void die(const char * fmt, ...);
