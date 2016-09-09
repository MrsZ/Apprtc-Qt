#include "mainwindow.h"
#include <QApplication>

#include "webrtc/base/thread.h"
#include "webrtc/base/win32socketinit.h"
#include "webrtc/base/win32socketserver.h"


int main(int argc, char *argv[])
{
    rtc::EnsureWinsockInit();
    rtc::Win32Thread w32_thread;
    rtc::ThreadManager::Instance()->SetCurrentThread(&w32_thread);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
