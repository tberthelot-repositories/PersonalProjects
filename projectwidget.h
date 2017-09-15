#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include "layoutmanager.h"
#include "project.h"
#include "sprint.h"
#include "listwidget.h"

namespace Ui {
class QProjectWidget;
}

/*! \class QProjectWidget
 * \brief This Widget is used to display an QProject =  object and its children, The cellular widget of  MwDashboard
 *
 * This widget displays all important information about QProject = , the current or selected sprint and their respective lists.
 */
class QProjectWidget : public QWidget, public QLayoutManager
{
    Q_OBJECT

public:
    /*! Constructor taking parent and QProject =  arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which contains the widget.
     * \param end Endeavor pointer held by the widget
     * \sa void QProjectWidget::setEnDetails(), void QProjectWidget::setUp()
     */
    explicit QProjectWidget(QWidget *parent = 0, QProject *end =0);
    //! Destructor
    ~QProjectWidget();

signals:
    /*! \fn void QProjectWidget::forceRefresh(QString val)
     * Emitted once Endeavor details are changed and has to change the MwWindow's tab name.
     * \param val Updated Endeavor name
     * \sa void MwDashboard::catchRefresh(QString str)
*/
    void forceRefresh(QString val);
    /*! \fn void QProjectWidget::removeTab()
     * Emitted after the endeavor has been deleted, signalling the parent list widget to take necessary action
     * \sa void MwDashboard::addEndeavorTab(QProject =  *end)
*/
    void removeTab();

private slots:
    void sprintComboInitialize(int val);
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnRefresh_clicked();
    void on_cmbSprints_currentIndexChanged(int index);

protected slots:
    void reInitialize();

protected:
    void setUp();
    void removeItemsFromLayout();

private:
    Ui::QProjectWidget *ui;
    QProject *endeavor; //!< Containing Endeavor Pointer
    void setEnDetails();
    void addList(QListWidget *list);
    void setSprintDetails(QSprint *spr);
};

#endif // PROJECTWIDGET_H

