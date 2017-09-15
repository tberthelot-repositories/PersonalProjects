#ifndef ACTIVITYNOTESWIDGET_H
#define ACTIVITYNOTESWIDGET_H

#include <QDialog>
#include "layoutmanager.h"
#include "activity.h"
#include "notewidget.h"

namespace Ui {
class QActivityNotesWidget;
}

/*! \class QActivityNotesWidget
 * \brief This form is used to display OcNote objects that belong to a particular parent activity (UI DialogBox Widget)
 */
class QActivityNotesWidget : public QDialog, public QLayoutManager
{
    Q_OBJECT

public:
    /*! Constructor taking parent and OcActivity arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which executes or shows the dialog box.
     * \param act Parent Activity object of which the children are displayed in the form
     */
    explicit QActivityNotesWidget(QWidget *parent = 0, QActivity *act = 0);
     //! Destructor
    ~QActivityNotesWidget();

private slots:
    void on_btnAddChild_clicked();

protected slots:
    void reInitialize();

protected:
    void setUp();
    void removeItemsFromLayout();

private:
    QActivity *activity; //!< Parent Activity Object of which the notes are displayed in the form.
    Ui::QActivityNotesWidget *ui;
    void addNote(QNoteWidget *note);

};

#endif // ACTIVITYNOTESWIDGET_H
