//////////////////////////////////////////////////////
///       FreeSubtitleAssistant
///       version 0.1
///
///       2021-6-17
///       by Marv
//////////////////////////////////////////////////////


#include "mw.h"
#include <QApplication>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MW w;
    w.show();
    return a.exec();
}
