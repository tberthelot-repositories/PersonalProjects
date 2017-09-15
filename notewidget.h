#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include <QWidget>
#include "editor.h"
#include "note.h"

namespace Ui {
class QNoteWidget;
}

/*! \class QNoteWidget
 * \brief This Widget is used to allow user to add or edit and view a OcNote object, Inherits QEditor and therefore is used with DiEditor. The cellular widget of QActivityNotesWidget
 *
 * This widget displays inputs for all necessary and optional data required to create an OcNote object and enables recording it in the database or doing modifications on existing values.
 */
class QNoteWidget : public QEditor
{
    Q_OBJECT

public:
    /*! Constructor taking parent and OcActivity arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which shows the widget
     * \param vNote Note pointer held by the widget
     * \param vView If the widget is used in a view context or edit/add context.
     * \sa void QNoteWidget::reset()
     */
    explicit QNoteWidget(QWidget *parent = 0, QNote *vNote = 0, bool vView = false);
    //! Destructor
    ~QNoteWidget();
    void setViewVisibility();

private slots:
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_chkNum_toggled(bool checked);
    void on_txteNoteText_textChanged();
    void on_txtleTag_textChanged(const QString &arg1);

public slots:
    void performAccepted();
    void performRejected();
    void reset();

signals:
    /*! \fn void forceRefresh();
     * Emitted after deleting the containing OcNote object to inform the container to reset it's layout.
     * /sa void QActivityNotesWidget::reInitialize()
*/
    void forceRefresh();

protected:
    void setValues();
    void getValues();

private:
    QNote *note; //!< Containing Activity Pointer
    bool view; //!< If form was opened in an Edit/Add Item context or View context
    bool edit; //!< If form was opened in an Edit Item context or Add Item context (Depending on Activity ID)
    Ui::QNoteWidget *ui;
    void validateForm();
    void setNullNumericVisibility();
};

#endif // NOTEWIDGET_H

