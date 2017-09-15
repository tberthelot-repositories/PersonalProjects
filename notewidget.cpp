#include "notewidget.h"
#include "ui_notewidget.h"

#include <QMessageBox>
#include <QStringList>
#include <QCompleter>
#include "editorwidget.h"

QNoteWidget::QNoteWidget(QWidget *parent, QNote *vNote, bool vView) :
    QEditor(parent), note(vNote),view(vView), edit(vNote->noteId > 0),
    ui(new Ui::QNoteWidget)
{
    ui->setupUi(this);

    //! Add Possible Suggestions for Numeric Tags
    QStringList suggestions;
    suggestions << "Time Allocated " << "Time Spent" << "Budget" << "Cost" << "Priority";
    QCompleter *autoComplete= new QCompleter(suggestions, this);
    autoComplete->setCaseSensitivity(Qt::CaseInsensitive);
    ui->txtleTag->setCompleter(autoComplete);

    if(edit){
       setValues();
    }
    ui->lblAddedDate->setVisible(view);
    ui->btnEdit->setVisible(view);
    ui->btnDelete->setVisible(view);
    ui->lblicoAddedDate->setVisible(view);
}

QNoteWidget::~QNoteWidget()
{
    delete ui;
}

/*! \fn void QNoteWidget::setValues()
 * Sets all the initialized values of the note object to display controls assigned for them.
 */
void QNoteWidget::setValues(){
    ui->txteNoteText->setHtml(note->noteText);

    if(view)
        ui->lblAddedDate->setText(note->created.toString("dd MMMM yy"));

    ui->chkNum->setChecked(!note->numeric.isNull());
    if(!note->numeric.isNull()){
        ui->sbxDblNumeric->setValue(note->numeric.toDouble());
        ui->txtleTag->setText(note->numerictag);
    }
    setNullNumericVisibility();
}

/*! \fn void QNoteWidget::getValues()
 * Updates existing values of the Note object by assigning user input values from respective controls
 */
void QNoteWidget::getValues(){
    note->noteText = ui->txteNoteText->toHtml();
    if(ui->chkNum->isChecked()){
        note->numeric = ui->sbxDblNumeric->value();
        note->numerictag = ui->txtleTag->text();
    }else{
        note->numeric = QVariant();
        note->numerictag = "";
    }
}


/*! \fn void QNoteWidget::setViewVisibility()
 * Disables editable controls makes certain controls visible to enable a non interactive "view"
 * \sa  void QActivityNotesWidget::addNote(QNoteWidget *note)
 */
void QNoteWidget::setViewVisibility(){

    ui->chkNum->setChecked(!view);
    //Num Box and Text Tag disabled cuz of toggle
    ui->chkNum->setVisible(!view);

    ui->txteNoteText->setReadOnly(view);

    ui->btnDelete->setVisible(view);
    ui->btnEdit->setVisible(view);
    ui->lblAddedDate->setVisible(view);
    ui->lblicoAddedDate->setVisible(view);

}


/*! \fn void QNoteWidget::setNullNumericVisibility()
 * If there isn't a numeric values, disables  the controls that display them in "View" Mode.
 */
void QNoteWidget::setNullNumericVisibility(){
    bool isNull = note->numeric.isNull();
    if(view){
        ui->lblicoNumeric->setVisible(!isNull);
        ui->sbxDblNumeric->setVisible(!isNull);
        ui->txtleTag->setVisible(!isNull);
    }
}

/*! \fn void QNoteWidget::validateForm()
 * Checks user's ability to proceed with the action depending on values input on txteNoteText and txtleTag
 */
void QNoteWidget::validateForm(){
    bool valid = true;
    QTextDocument docu;
    docu.setHtml(ui->txteNoteText->toHtml());
    if(docu.toPlainText().size() <= 0){
        ui->lblValidation->setText("You must enter the Note Text to proceed");
        valid  = false;
    }
    else if(ui->chkNum->isChecked() && ui->txtleTag->text().size() <=0){
        ui->lblValidation->setText("You must enter the Numeric Value Tag to proceed");
        valid = false;
    }else{
        ui->lblValidation->setText("");
    }
   emit formValidated(valid);
}

/*! \fn void QNoteWidget::performAccepted()
 * Performs final actions on note dependin on it's context (To add new or edit existing). Emits accepted signal to mark the function as finalized and successful.
 * \sa  void QEditor::accepted();
 */
void QNoteWidget::performAccepted(){
    getValues();
    if(edit){
        if(note->editItem())
            QMessageBox::information(this, "Edited", "Operation Successful");
        else
            QMessageBox::information(this, "Edited", "Operation Failed. Please Try Again");
    }else
    {
        if(note->addItem())
            QMessageBox::information(this, "Added", "Operation Successful");
        else
            QMessageBox::information(this, "Added", "Operation Failed. Please Try Again");
    }
    emit accepted();
}

/*! \fn void QNoteWidget::performRejected()
 * Discards changes made in UI and emits Rejected Signal to close the window
 * \sa  void QEditor::rejected();
 */
void QNoteWidget::performRejected(){
    if(edit){
        note->initialize();
    }
    reset();
    emit rejected();
}

/*! \fn void QNoteWidget::reset()
 * Discards changes made in UI by user and re-establishes the initial conditions.
 */
void QNoteWidget::reset(){
    if(edit){
        note->initialize();
        setValues();
    }
    if(!edit){
        ui->chkNum->setChecked(true);
        ui->txteNoteText->setText("");
        ui->txtleTag->setText("");
        ui->sbxDblNumeric->setValue(0);
    }
}

/*! \fn void QNoteWidget::on_btnEdit_clicked()
 * Edit the note through a new Dialog Window, adds the added note to layout and sets visibility constraints
 * \sa void QActivityWidget::setValues()
 */
void QNoteWidget::on_btnEdit_clicked()
{
    QEditorWidget di(this, note);
    int res = di.exec();
    if(res == QDialog::Accepted){
        note->initialize();
        setValues();
        setViewVisibility();
    }
}

/*! \fn void QNoteWidget::on_btnDelete_clicked()
 * Delete the note object in the widget from the database and emit container refresh signal.
 * \sa void QActivityNotesWidget::addNote(QNoteWidget *note), void QActivityNotesWidget::reInitialize()
 */
void QNoteWidget::on_btnDelete_clicked()
{
    if(note->removeItem()){
        QMessageBox::information(this, "Removed", "Operation Successful");
    }else{
        QMessageBox::information(this, "Removal", "Operation Failed. Please Try Again");
    }
    emit forceRefresh();
}

/*! \fn void QNoteWidget::on_chkNum_toggled(bool checked)
 * Enforces Numeric Inputs availability and validates form
 * \param checked if control was checked
 * \sa void QNoteWidget::validateForm()
 */
void QNoteWidget::on_chkNum_toggled(bool checked)
{
    ui->sbxDblNumeric->setEnabled(checked);
    ui->txtleTag->setEnabled(checked);
    validateForm();
}

/*! \fn void QNoteWidget::on_txteNoteText_textChanged()
 * Validates form upon NoteText change
 * \sa void QNoteWidget::validateForm()
 */
void QNoteWidget::on_txteNoteText_textChanged()
{
    validateForm();
}

/*! \fn void QNoteWidget::on_txtleTag_textChanged()
 * Validates form upon NoteText change
 * \sa void QNoteWidget::validateForm()
 */
void QNoteWidget::on_txtleTag_textChanged(const QString &)
{
    validateForm();
}
