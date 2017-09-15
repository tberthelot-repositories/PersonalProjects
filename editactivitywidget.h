#ifndef EDITACTIVITYWIDGET_H
#define EDITACTIVITYWIDGET_H

#include <QWidget>
#include <QDateTime>
#include "editor.h"
#include "activity.h"
#include "project.h"

namespace Ui {
class QEditActivityWidget;
}

/*! \class QEditActivityWidget
 * \brief This Widget is used to allow user to add or edit a OcActivity object, Inherits QEditor and therefore is used with DiEditor
 *
 * This widget displays inputs for all necessary and optional data required to create an OcActivity object and enables recording it in the database or doing modifications on existing values.
 */
class QEditActivityWidget : public QEditor
{
    Q_OBJECT

public:
    /*! Constructor taking parent and OcActivity arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which shows the widget
     * \param actv Activity pointer held by the widget
     * \sa void QEditActivityWidget::reset()
     */
    explicit QEditActivityWidget(QWidget *parent = 0, QActivity *actv = 0);
    //! Destructor
    ~QEditActivityWidget();

public slots:
    void performAccepted();
    void performRejected();
    void reset();

private slots:
    void on_chkNotify_toggled(bool checked);
    void on_chkPoints_toggled(bool checked);
    void on_chkCreatedDtBased_toggled(bool checked);
    void on_sbxSprint_valueChanged(int arg1);
    void on_dtNotify_dateTimeChanged(const QDateTime &dateTime);
    void on_txtEdName_textChanged(); 

protected:
    void setValues();
    void getValues();

private:
    QActivity *activity; //!< Containing Activity Pointer
    bool edit; //!< If form was opened in an Edit Item context or Add Item context (Depending on Activity ID)
    QProject *end; //!< Endeavor the activity belongs to
    Ui::QEditActivityWidget *ui;
    void setSprintText(int);


};

#endif // EDITACTIVITYWIDGET_H
