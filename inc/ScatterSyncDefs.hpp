#pragma once

#include <wx/wx.h>

#define WH_EVER while(true)
#define SC(T, VALUE) static_cast<T>(VALUE)

using u_char  = unsigned char;
using u_long  = unsigned long;
using u_llong = unsigned long long;

// Contains specific definitions for Scatter Sync.
namespace SS_GLOBALDEFS {
    inline constexpr int WSX { 800 }; // Window size in the X axis, or width.
    inline constexpr int WSY { 600 }; // Window size in the Y axis, or height.

    inline static const wxColor BLACK { 0, 0, 0 };
    inline static const wxColor DARK_GREY { 18, 18, 18 };
    inline static const wxColor LIGHT_GREY { 64, 64, 64 };
    inline static const wxColor WHITE { 255, 255, 255 };
}
