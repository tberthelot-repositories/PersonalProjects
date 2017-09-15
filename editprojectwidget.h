#ifndef EDITPROJECTWIDGET_H
#define EDITPROJECTWIDGET_H

#include <QWidget>
#include "editor.h"
#include "project.h"

namespace Ui {
class QEditProjectWidget;
}

/*! \class QEditProjectWidget
 * \brief This Widget is used to allow user to add or edit a QProject =  object, Inherits QEditor and therefore is used with DiEditor
 *
 * This widget displays inputs for all necessary and optional data required to create an QProject =  object and enables recording it in the database or doing modifications on existing values.
 */
class QEditProjectWidget : public QEditor
{
    Q_OBJECT

public:
    /*! Constructor taking parent and OcActivity arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which shows the widget
     * \param endv Endeavor pointer held by the widget
     * \sa void QEditProjectWidget::reset()
     */
    explicit QEditProjectWidget(QWidget *parent = 0, QProject *endv = 0);
    //! Destructor
    ~QEditProjectWidget();

public slots:
    void performAccepted();
    void performRejected();
    void reset();

private slots:
    void on_chkNotify_toggled(bool checked);
    void on_txtName_textChanged(const QString &arg1);

protected:
    void setValues();
    void getValues();

private:
    QProject *end; //!< Containing Endeavor Pointer
    bool edit; //!< If form was opened in an Edit Item context or Add Item context (Depending on Endeavor ID)
    Ui::QEditProjectWidget *ui;
};

#endif // EDITPROJECTWIDGET_H

