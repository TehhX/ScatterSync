#pragma once

#include <wx/msgdlg.h>

// Displays a simple popup window with MSSG as the message and no further user input.
#define POPUP(MSSG) \
    { auto mssgDialog { new wxMessageDialog { nullptr, MSSG, wxMessageBoxCaptionStr, wxSTAY_ON_TOP | wxICON_WARNING | wxOK } }; \
    mssgDialog->ShowModal(); }

// Displays a yes/no popup window with MSSG as the message, executes EXEC if yes, nothing if no.
#define YN_POP(MSSG, EXEC) \
    { auto mssgDialog { new wxMessageDialog { nullptr, MSSG, wxMessageBoxCaptionStr, wxSTAY_ON_TOP | wxICON_WARNING | wxYES | wxNO } }; \
    if (mssgDialog->ShowModal() == wxID_YES) { \
        EXEC \
    } }
