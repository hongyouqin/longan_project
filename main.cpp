#include "mainwindow.h"
#include <QApplication>
#include "configs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.Initialize();

    auto system = Configs::GetSystemConfig();
    if (system->show_camera == 1) {
        w.show();
    } else {
        w.hide();
    }

    return a.exec();
}
