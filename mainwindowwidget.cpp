#include "mainwindowwidget.h"
#include "ui_mainwindowwidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include "initializer.h"
#include "databasesettings.h"
#include "projectwidget.h"
#include "editorwidget.h"


QMainWindowWidget::QMainWindowWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QMainWindowWidget)
{
    ui->setupUi(this);

    //! Connects QMenu Actions to MainWindow Slots.
    connect(ui->actionNew, SIGNAL(triggered(bool)) , this, SLOT(addEndeavor()));
    connect(ui->actionBackup_Database, SIGNAL(triggered(bool)), this, SLOT(backupDatabase()));
    connect(ui->actionMove_Database, SIGNAL(triggered(bool)), this, SLOT(moveDatabase()));

/* Connection Archived. Refer to UcDbSettings header for information. */
/* connect(ui->actionRestore_Default_Settings, SIGNAL(triggered(bool)), this, SLOT(restoreSettings())); */

    QInitializer::setLists();
    loadGeneralViews();
}

QMainWindowWidget::~QMainWindowWidget()
{
    delete ui;
}

/*! \fn void MwDashboard::loadGeneralViews()
 * Retrieves all Endeavors and passes them to be added as tabs.
 * \sa void MwDashboard::addEndeavorTab(QProject =  *end), QList<int> UcInitializer::getEndeavors()
 */
void QMainWindowWidget::loadGeneralViews(){
    foreach(int i, QInitializer::getEndeavors()){
        addEndeavorTab(new QProject(this, i));
    }
}


/*! \fn void MwDashboard::addEndeavorTab(QProject =  *end)
 * Adds tabs per each WiEndavour object created.
 * \param end Endeavor to initialize WiEndavour with
 */
void QMainWindowWidget::addEndeavorTab(QProject *end){
    QProjectWidget *wid = new QProjectWidget(this, end);
    QIcon ico;
    //! Default "General" tab to have a different icon
    if(end->enId == 1)
        ico.addFile(":/images/taball.png");
    else
        ico.addFile(":/images/tabgeneral.png");
    ui->tabMain->addTab(wid, ico, end->enName);

    connect(wid, SIGNAL(forceRefresh(QString)), this, SLOT(catchRefresh(QString)));
    connect(wid, SIGNAL(removeTab()), this, SLOT(removeCurrentTab()));
}

/*! \fn void MwDashboard::removeCurrentTab()
 * Removes tab that is active. Used upon deleting an endeavor and recieving removeTab signal from WiEndavour widget
 */
void QMainWindowWidget::removeCurrentTab(){
    ui->tabMain->removeTab(ui->tabMain->currentIndex());
}

/*! \fn void MwDashboard::addEndeavor()
 * Adds a new WiEndeavor tab upon adding a new Endeavor.
 */
void QMainWindowWidget::addEndeavor(){
    QProject *newEnd = new QProject(this, 0);
    QEditorWidget di(this, newEnd);
    int res = di.exec();
    if(res == QDialog::Accepted){
        newEnd->initialize();
        addEndeavorTab(newEnd);
    }
}

/*! \fn void MwDashboard::catchRefresh(QString str)
 * Edit the name of the active tab if it was changed upon editing Endeavor. Used upon recieving forceRefresh signal from WiEndavour widget
 */
void QMainWindowWidget::catchRefresh(QString str){
    ui->tabMain->setTabText(ui->tabMain->currentIndex(), str);
}

//Settings Section

/*! \fn void MwDashboard::backupDatabase()
 * Opens a file window for user to select where to back up the database, processes action and alerts if successful or not.
 * \sa bool UcDbSettings::backupWorkingDatabase(QString dir)
 */
void QMainWindowWidget::backupDatabase(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select a Directory to Back up database file"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()){
        if(QDatabaseSettings::backupWorkingDatabase(dir)){
            QMessageBox::information(this, "Success", "Operation Completed");
        }else{
            QMessageBox::information(this, "Failed", "Operation Failed. Please try again");
        }
    }
}

/*! \fn void MwDashboard::moveDatabase()
 * Opens a file window for user to select where to move the database, processes action and alerts if successful or not.
 * \sa bool UcDbSettings::moveDatabase(QString dir)
 */
void QMainWindowWidget::moveDatabase(){
    int result = QMessageBox::warning(this, tr("Moving Database"), tr("This option is enabled to be utilized with an external syncronization service. A conflict would cause the application to stop.\nClick Yes to proceed if you understand the implications. Click No to return"), QMessageBox::Yes | QMessageBox::No);
    if(result == QMessageBox::Yes){
        QString dir = QFileDialog::getExistingDirectory(0, tr("Select a Directory to move database file to"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
        if(!dir.isEmpty()){
            if(QDatabaseSettings::moveDatabase(dir)){
                QMessageBox::information(this, "Success", "Operation Completed. Please restart for changes to take effect.");
            }else{
                QMessageBox::critical(this, "Failed", "Operation Failed. Please try again");
            }
        }
    }
}


/* Function Archived. Refer to UcDbSettings header for information. *//*
void MwDashboard::restoreSettings(){
    int result = QMessageBox::question(this, tr("Restore Original Settings"), tr("It is recommended that you back up your database before proceeding. Do you want to continue?"), QMessageBox::Yes | QMessageBox::No);
    if(result == QMessageBox::Yes){
        if(UcDbSettings::restoreDefaultDatabase()){
            QMessageBox::information(this, "Success", "Operation Completed. Please restart for changes to take effect.");
        }else{
            QMessageBox::critical(this, "Failed", "Operation Failed. Please try again");
        }
    }
}
*/

void QMainWindowWidget::on_actionTest_triggered()
{
    QApplication::alert(this, 1000);
}
