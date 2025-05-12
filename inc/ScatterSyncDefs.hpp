#pragma once

#include <wx/wx.h>

// Displays a simple popup window with MSSG as the message and no further user input.
#define POPUP(MSSG) \
    auto mssgDialog { new wxMessageDialog { nullptr, MSSG, wxMessageBoxCaptionStr, wxSTAY_ON_TOP | wxICON_WARNING | wxOK } }; \
    mssgDialog->ShowModal();

// Displays a yes/no popup window with MSSG as the message, executes EXEC if yes, nothing if no.
#define YN_POP(MSSG, EXEC) \
    { auto mssgDialog { new wxMessageDialog { nullptr, MSSG, wxMessageBoxCaptionStr, wxSTAY_ON_TOP | wxICON_WARNING | wxYES | wxNO } }; \
    if (mssgDialog->ShowModal() == wxID_YES) { \
        EXEC \
    } }

#define IGN_ERRS(EXEC) try { EXEC } catch(...) {}
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
