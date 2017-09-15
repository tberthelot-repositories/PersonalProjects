#include "editorwidget.h"
#include "ui_editorwidget.h"

#include "editprojectwidget.h"
#include "editactivitywidget.h"
#include "notewidget.h"

QEditorWidget::QEditorWidget(QWidget *parent, QProject *endv) :
    QDialog(parent),
    ui(new Ui::QEditorWidget)
{
    ui->setupUi(this);
    widget = new QEditProjectWidget(this, endv);
    ui->verticalLayout->insertWidget(0,widget);

    connectToDialog(endv->enId, endv->enName, "Endeavor");

    this->setMinimumSize(450, 180);

}

QEditorWidget::QEditorWidget(QWidget *parent, QActivity *act) :
    QDialog(parent),
    ui(new Ui::QEditorWidget)
{
    ui->setupUi(this);    
    widget = new QEditActivityWidget(this, act);
    ui->verticalLayout->insertWidget(0,widget);

    connectToDialog(act->id, act->text.left(30).append("..."), "Activity");

    this->setMinimumSize(400, 200);
}

QEditorWidget::QEditorWidget(QWidget *parent, QNote *note) :
     QDialog(parent),
     ui(new Ui::QEditorWidget)
{
    ui->setupUi(this);

    /*! Note Text is in HTML format and therefor it to be displayed as plain text some conversion is required <a href="http://stackoverflow.com/a/9546425/3132503">Source</a>
*/
    QTextDocument doc;
    doc.setHtml(note->noteText);
    widget = new QNoteWidget(this, note, false);
    ui->verticalLayout->insertWidget(0,widget);
    connectToDialog(note->noteId, doc.toPlainText().left(30).append("..."), "Note");
    this->setMinimumSize(250, 200);
}


QEditorWidget::~QEditorWidget()
{
    delete ui;
}


/*! \fn void DiEditor::connectToDialog(int id, QString str, QString type)
 * Sets the name of the Editor Dialog Box.
 * \param id of object
 * \param str String to be set as title
 * \param type Alternative text to be set as title if object is yet to be defined
 */
void QEditorWidget::connectToDialog(int id, QString str, QString type){
    QString addnew("Add New ");
    addnew.append(type);

    //! Determining if dialog box is opened for adding an object or editing an object
    if(id > 0)
        this->setWindowTitle(str);
    else
        this->setWindowTitle(addnew);

    QPushButton *saveBtn = ui->buttonBox->button(QDialogButtonBox::Save);
    QPushButton *discardBtn = ui->buttonBox->button(QDialogButtonBox::Discard);
    QPushButton *resetBtn = ui->buttonBox->button(QDialogButtonBox::Reset);
    if(id == 0)
        saveBtn->setEnabled(false);


    //! Connecting Buttonbox button signals to QEditor Widget's Slots.
    connect(saveBtn, SIGNAL(clicked()), widget, SLOT(performAccepted()));
    connect(discardBtn, SIGNAL(clicked()), widget, SLOT(performRejected()));
    connect(resetBtn, SIGNAL(clicked()), widget, SLOT(reset()));

     //! Connecting QEditor signals to Dialog's slots and also save button for validation purposes.
    connect(widget, SIGNAL(accepted()), this, SLOT(accept()));
    connect(widget, SIGNAL(rejected()), this, SLOT(close()));
    connect(widget, SIGNAL(formValidated(bool)), saveBtn, SLOT(setEnabled(bool)));

}
