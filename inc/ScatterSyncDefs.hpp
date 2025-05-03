#pragma once

using u_char  = unsigned char;
using u_long  = unsigned long;
using u_llong = unsigned long long;

// Contains specific definitions for Scatter Sync.
namespace ssdef {
    constexpr int WSX { 800 }; // Window size in the X axis, or width.
    constexpr int WSY { 600 }; // Window size in the Y axis, or height.
}
