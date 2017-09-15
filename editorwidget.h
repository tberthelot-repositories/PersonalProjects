#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QDialog>
#include <QPushButton>
#include <QTextDocument>
#include "editor.h"
#include "project.h"
#include "activity.h"
#include "note.h"

namespace Ui {
class QEditorWidget;
}

/*! \class QEditorWidget
 * \brief This form is used to display OcActivity, QProject =  and OcNote objects on occasions of adding and updating them.(UI DialogBox Widget)
 */
class QEditorWidget : public QDialog
{
    Q_OBJECT

public:
    /*! Constructor taking parent and QProject =  arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which executes or shows the dialog box.
     * \param endv Endeavor object to be edited or added
     */
    explicit QEditorWidget(QWidget *parent = 0, QProject *endv = 0);

    /*! Constructor taking parent and OcActivity arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which executes or shows the dialog box.
     * \param act Activity object to be edited or added
     */
    explicit QEditorWidget(QWidget *parent = 0, QActivity *act = 0);

    /*! Constructor taking parent and OcNote arguments and initializing object
     * \param parent Parent QWidget - Widget/Window which executes or shows the dialog box.
     * \param note Note object to be edited or added
     */
    explicit QEditorWidget(QWidget *parent = 0, QNote *note = 0);

 //! Destructor
    ~QEditorWidget();

private:
    Ui::QEditorWidget *ui;
    QEditor *widget; //!< The widget that could be added to the dialog box at runtime. Could be either QEditActivityWidget, QEditProjectWidget or QNoteWidget (Inherited Items)
    void connectToDialog(int, QString, QString);

};

#endif // EDITORWIDGET_H
