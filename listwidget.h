#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "layoutmanager.h"
#include "listbase.h"
#include "activitywidget.h"

namespace Ui {
class QListWidget;
}

/*! \class QListWidget
 * \brief This Widget is used to display a number of QActivityWidget objects in a given order, The cellular widget of  WiEndavour
 *
 * QActivityWidget widgets in QListWidgets can be dragged and dropped depending on the characteristics of the list it belongs to.
 */
class QListWidget : public QWidget, public QLayoutManager
{
    Q_OBJECT

public:
    /*! Constructor taking parent, QListBase arguments, default or specified intege arguments for parent Id and Sprint Id and initializing object
     * \param parent Parent QWidget - Widget/Window which contains the widget.
     * \param list List pointer held by the widget
     * \param endavourId ID of the endeavour the list belongs to
     * \param paId Parent Id of the list activities if there is any, used in viewing Activity Children.
     * \param sprintId Sprint Id to be filtered activities with
     * \sa void void QListWidget::setUp()
     */
    explicit QListWidget(QWidget *parent = 0, QListBase *list = 0, int endavourId = 0, int paId = 0, int sprintId = 0);
    //! Destructor
    ~QListWidget();

signals:
    /*! \fn void QListWidget::forceRefresh()
     * Emitted after a new activity has been added to the list irrespective of its sprint (Due to it being a child activity) signalling the container to reinitialize itself.
     * \sa void QActivityChildrenWidget::addList(QListWidget *list)
*/
    void forceRefresh();
    /*! \fn void QListWidget::forceSprintRefresh(int val)
     * Emitted after a new activity has been added and has to be displayed in main widgets, signalling the parent endeavor widget to take necessary action
     * \param val Sprint Id
     * \sa void QListWidget::on_btnAddChild_clicked()
*/
    void forceSprintRefresh(int);

private slots:    
    void moveItemUp(int);
    void moveItemDown(int);
    void on_btnAddChild_clicked();

protected slots:
    void reInitialize();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *);
    void setUp();
    void removeItemsFromLayout();

private:
    QListBase *list; //!< Containing List Object pointer
    int endId; //!< ID of endeavour in WiEndavour which contains the list
    int parentActivityId; //!< Id of parent activity of which the activities are displayed. 0 by default for top most activities.
    int sprint; //!< Sprint ID of the endveaour to filter activities
    Ui::QListWidget *ui;
    int noOfItems; //!< Number of items currently in the layout.
    void addItemToLayout(int, QActivityWidget*);
    void connectToLayout(QActivityWidget*);

};

#endif // LISTWIDGET_H
