#pragma once

// Lazy static cast
#define SC(T, VALUE) static_cast<T>(VALUE)

// Returns one of the IF<x>'s depending on whether <VALUE> is positive, negative, or zero. Should be a number value.
#define SIGN(VALUE, IFPOS, IFNEG, IFZERO) VALUE > 0 ? IFPOS : VALUE < 0 ? IFNEG : IFZERO

// Typedefs
using u_char  = unsigned char;
using u_long  = unsigned long;
using u_llong = unsigned long long;
using u_short = unsigned short;

// Window size constants
#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600
#define WINDOW_SIZE { WINDOW_SIZE_X, WINDOW_SIZE_Y }

// wxColor constants
#define WXC_BLACK {   0,   0,   0 }
#define WXC_DGREY {  18,  18,  18 }
#define WXC_LGREY {  64,  64,  64 }
#define WXC_WHITE { 255, 255, 255 }
#define WXC_RED   { 255,   0,   0 }
