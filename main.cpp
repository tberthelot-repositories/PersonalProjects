#include "mainwindowwidget.h"
#include <QApplication>
#include <QMessageBox>
#include "databasesettings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(QDatabaseSettings::checkDbExists()){
        QMainWindowWidget w;
        w.showMaximized();
        return a.exec();
    }else{
        QMessageBox::information(0, "Database File Missing", "Please make sure database exists at " + QDatabaseSettings::getDatabaseLoc() + " and restart the application.");
        a.quit();
        return 0;
    }
}
