#ifndef MAINWINDOWWIDGET_H
#define MAINWINDOWWIDGET_H

#include <QMainWindow>
#include "project.h"

namespace Ui {
class QMainWindowWidget;
}

/*! \class QMainWindowWidget
 * \brief This Main Window is used to display all QProject =  objects and their activities (UI MainWindow)
 */
class QMainWindowWidget : public QMainWindow
{
    Q_OBJECT

public:
    /*! Constructor
     * Sets up the initial view of the application.
     * \param parent
     * \sa void UcInitializer::setLists(), void QMainWindowWidget::loadGeneralViews()
     */
    explicit QMainWindowWidget(QWidget *parent = 0);
     //! Destructor
    ~QMainWindowWidget();

private slots:
    void addEndeavor();
    void catchRefresh(QString);
    void removeCurrentTab();
    //Settings Slots
    void backupDatabase();
    void moveDatabase();
    /* void restoreSettings(); */

    void on_actionTest_triggered();

private:
    Ui::QMainWindowWidget *ui;
    void loadGeneralViews();
    void addEndeavorTab(QProject *);

};

#endif // MAINWINDOWWIDGET_H
