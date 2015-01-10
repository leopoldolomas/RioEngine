//--------------------------------------------------------------- @License begins
// RioEngine: The late night Coke -without whores- debugging sessions
// 2012-2015 Leopoldo Lomas Flores. Torreon, Coahuila. MEXICO
// leopoldolomas [at] gmail
// www.rioengine.com
// www.leopoldolomas.info
// "You have a problem, you face it like a man."
// 
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
// software, either in source code form or as a compiled binary, for any purpose,
// commercial or non-commercial, and by any means.
// 
// In jurisdictions that recognize copyright laws, the author or authors of this
// software dedicate any and all copyright interest in the software to the public
// domain. We make this dedication for the benefit of the public at large and to
// the detriment of our heirs and successors. We intend this dedication to be
// an overt act of relinquishment in perpetuity of all present and future
// rights to this software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//--------------------------------------------------------------- @License ends

#include <windows.h>
#include <tchar.h>
#include <QApplication>
#include <QtMessageHandler>
#include "macro.h"
#include "editor/editorwindow.h"
#include "editor/logwindow/logwindow.h"

#if VLD_ENABLED
#include <vld.h>
#endif

LogWindow*        logWindow = NULL;
QtMessageHandler  logWindowMessageHandler;
QtMessageHandler  mainMessageHandler;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    (mainMessageHandler)(type, context, msg); // redirect the message to the console

    if(logWindow) {
        logWindow->log(msg);
    }
}

int main(int argc, char *argv[])
{
#if VLD_ENABLED
    std::string str = "MemoryLeaksReport.txt";
    std::wstring widestr = std::wstring(str.begin(), str.end());
    VLDSetReportOptions(VLD_OPT_REPORT_TO_FILE, widestr.c_str());
#endif

    QApplication a(argc, argv);
    a.setStyle("Fusion");

    logWindow = new LogWindow();
    logWindow->show();

    logWindowMessageHandler = QtMessageHandler(myMessageOutput);
    mainMessageHandler = qInstallMessageHandler(logWindowMessageHandler);

    EditorWindow w = EditorWindow();
    w.show();

    int result = a.exec();

#if VLD_ENABLED
    VLDReportLeaks();
#endif

    SAFE_RELEASE(logWindow);

    return result;
}
