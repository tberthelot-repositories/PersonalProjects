#ifndef ACTIVITYCHILDRENWIDGET_H
#define ACTIVITYCHILDRENWIDGET_H

#include <QDialog>
#include "layoutmanager.h"
#include "activity.h"
#include "listwidget.h"

namespace Ui {
class QActivityChildrenWidget;
}

/*! \class QActivityChildrenWidget
 * \brief This form is used to display lists of child OcActivity objects that belong to a particular parent activity (UI DialogBox Widget)
 */
class QActivityChildrenWidget : public QDialog, public QLayoutManager
{
    Q_OBJECT

public:

    /*! Constructor taking parent and OcActivity arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which executes or shows the dialog box.
     * \param act Parent Activity object of which the children are displayed in the form
     */
    explicit QActivityChildrenWidget(QWidget *parent = 0, QActivity *act = 0);
    //! Destructor.
    ~QActivityChildrenWidget();

protected slots:
    void reInitialize();

protected:
    void setUp();
    void removeItemsFromLayout();

private:
    QActivity *activity; //!< Parent Activity Object of which the children are displayed in the form.
    Ui::QActivityChildrenWidget *ui;
    void addList(QListWidget *list);

};

#endif // ACTIVITYCHILDRENWIDGET_H
