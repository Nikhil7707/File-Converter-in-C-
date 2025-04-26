#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("File Converter");
    app.setOrganizationName("FileConverter");
    
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}