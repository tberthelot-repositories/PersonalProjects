#ifndef ACTIVITYWIDGET_H
#define ACTIVITYWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include "activity.h"

namespace Ui {
class QActivityWidget;
}

/*! \class QActivityWidget
 * \brief This Widget is used to display an OcActivity object, The cellular widget of QListWidget and QActivityChildrenWidget
 *
 * This widget displays all important information about OcActivity object in a user friendly and compact manner. It can be dragged and dropped depending on the list it belongs to. It can be edited. And it has options that allow users to add notes and child activities to the containing OcActivity object.
 */
class QActivityWidget : public QWidget
{
    Q_OBJECT

public:
    /*! Constructor taking parent and OcActivity arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which executes or shows the widget
     * \param activ Activity pointer held by the widget
     */
    explicit QActivityWidget(QWidget *parent = 0, QActivity *activ =0);
    //! Destructor
    ~QActivityWidget();
    void setOrders(int);

signals:
    /*! \fn void QActivityWidget::moveUp(int val)
     * Emitted after QActivityWidget::on_btnUp_clicked() is called.
     * \param val The current order/index of the activity
*/
    void moveUp(int val);
    /*! \fn void QActivityWidget::moveDown(int val)
     * Emitted after QActivityWidget::on_btnDown_clicked() is called.
     * \param val The current order/index of the activity
*/
    void moveDown(int val);
    /*! \fn void QActivityWidget::forceRefresh()
     * Emitted after the activity has been deleted, signalling the parent list widget to redraw itself
*/
    void forceRefresh();

private slots:
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_btnUp_clicked();
    void on_btnDown_clicked();
    void on_btnViewChildren_clicked();
    void on_btnShowNotes_clicked();

protected:
    void mousePressEvent(QMouseEvent *);

private:
    Ui::QActivityWidget *ui;
    QActivity *activity; //!< Containing Activity Pointer
    void setValues();
    void setFrameOpacity(qreal opacity);

};

#endif // ACTIVITYWIDGET_H
