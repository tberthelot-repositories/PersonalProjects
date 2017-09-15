#include "activitynoteswidget.h"
#include "ui_activitynoteswidget.h"
#include "editorwidget.h"

QActivityNotesWidget::QActivityNotesWidget(QWidget *parent, QActivity *vAct) : QDialog(parent), activity(vAct),
    ui(new Ui::QActivityNotesWidget)
{
    ui->setupUi(this);
    ui->vLayoutNotes->setAlignment(Qt::AlignTop);

    QString title = activity->text.left(30).append("...");
    this->setWindowTitle(title);
    ui->lblListName->setText(title);

    setUp();
}

QActivityNotesWidget::~QActivityNotesWidget()
{
    delete ui;
}

/*! \fn void QActivityNotesWidget::setUp()
 * Creates note objects that belong to activity, creates QNoteWidget note display widgets and adds them to layout.
 */
void QActivityNotesWidget::setUp(){
    foreach(int x, activity->getNotes()){
        addNote(new QNoteWidget(this, new QNote(this, x), true));
    }
}

/*! \fn void QActivityNotesWidget::reInitialize()
 * Removes all note items from the vertical layouts and sets them up again by retrieving notes from database
 */
void QActivityNotesWidget::reInitialize(){
    removeItemsFromLayout();
    setUp();
}

/*! \fn void QActivityNotesWidget::removeItemsFromLayout()
 * Removes all note items from the vertical layout.
 * \sa void UcLayoutMgr::removeItemsFromLayout(QLayout *layout)
 */
void QActivityNotesWidget::removeItemsFromLayout(){
    removeLayoutItemsFromLayout(ui->vLayoutNotes);
}

/*! \fn void QActivityNotesWidget::addNote(QNoteWidget *note)
 * Adds the QNoteWidget widgets passed to the Vertical Layout of the current widget.
 * \param note QNoteWidget Widget initialized previously
 */
void QActivityNotesWidget::addNote(QNoteWidget *note){
    note->setViewVisibility();
    //! Adds each widget to top of the layout, ordering notes from latest to oldest.
    ui->vLayoutNotes->insertWidget(0,note);
    //! Connect to reinitialize upon catching forceRefresh from an added QNoteWidget
    connect(note, SIGNAL(forceRefresh()), this, SLOT(reInitialize()));
}

/*! \fn void QActivityNotesWidget::on_btnAddChild_clicked()
 * Displays the Dialog to add a new activity, if returned as accepted adds a QNoteWidget widget to the top of vertical layout.
 */
void QActivityNotesWidget::on_btnAddChild_clicked()
{
    QNote *newNote = new QNote(this, 0);
    newNote->parentActivityId = activity->id;
    QEditorWidget di(this, newNote);
    int res = di.exec();
    if(res == QDialog::Accepted){
        addNote(new QNoteWidget(this, newNote, true));
    }
}
