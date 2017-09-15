#include "editactivitywidget.h"
#include "ui_editactivitywidget.h"

#include <QMessageBox>
#include "sprint.h"

QEditActivityWidget::QEditActivityWidget(QWidget *parent, QActivity *actv) :
    QEditor(parent), activity(actv), edit(actv->id > 0),
    ui(new Ui::QEditActivityWidget)
{
    ui->setupUi(this);

    //! Enforce Maximum Sprint available to be defined if containing endeavor has a deadline.
    end = new QProject(this, activity->enId);
    if(!end->deadLine.isNull()){
        ui->sbxSprint->setMaximum(end->getNoOfSprints());
    }
   // ui->chkCreatedDtBased->setVisible(end->enId != 1);
    reset();
}

QEditActivityWidget::~QEditActivityWidget()
{
    delete ui;
}

/*! \fn void QEditActivityWidget::setValues()
 * Sets all the initialized values of the activity object to display controls assigned for them.
 */
void QEditActivityWidget::setValues(){
    ui->txtEdName->setPlainText(activity->text);

    ui->chkNotify->setChecked(!activity->notify.isNull());
    if(ui->dtNotify->isEnabled())
        ui->dtNotify->setDateTime(activity->notify.toDateTime());

    ui->chkPoints->setChecked(!activity->points.isNull());
    if(ui->sbxWeight->isEnabled())
        ui->sbxWeight->setValue(activity->points.toInt());

    ui->sbxSprint->setValue(activity->sprint);
    setSprintText(activity->sprint);
}

/*! \fn void QEditActivityWidget::getValues()
 * Updates existing values of the Activity object by assigning user input values from respective controls
 */
void QEditActivityWidget::getValues(){
    activity->text = ui->txtEdName->toPlainText();
    if(ui->chkNotify->isChecked())
        activity->notify = ui->dtNotify->dateTime();
    else
        activity->notify = QVariant();


    if(ui->sbxWeight->isEnabled())
        activity->points = ui->sbxWeight->value();
    else
        activity->points = QVariant() ;

    if(ui->chkCreatedDtBased->isChecked())
        activity->sprint = end->getSprint(activity->notify.toDateTime());
    else
        activity->sprint = ui->sbxSprint->value();
}

/*! \fn void QEditActivityWidget::performAccepted()
 * Performs final actions on activity dependin on it's context (To add new or edit existing). Emits accepted signal to mark the function as finalized and successful.
 * \sa  void QEditor::accepted();
 */
void QEditActivityWidget::performAccepted(){
    getValues();
    if(edit){
        if(activity->editItem())
            QMessageBox::information(this, "Edited", "Operation Successful");
        else
            QMessageBox::information(this, "Edited", "Operation Failed. Please Try Again");
    }else
    {
        if(activity->addItem())
            QMessageBox::information(this, "Added", "Operation Successful");
        else
            QMessageBox::information(this, "Added", "Operation Failed. Please Try Again");
    }
    emit accepted();
}

/*! \fn void QEditActivityWidget::performRejected()
 * Discards changes made in UI and emits Rejected Signal to close the window
 * \sa  void QEditor::rejected();
 */
void QEditActivityWidget::performRejected(){
    if(edit){
        activity->initialize();
    }
    reset();
    emit rejected();
}


/*! \fn void QEditActivityWidget::reset()
 * Discards changes made in UI by user and re-establishes the initial conditions.
 */
void QEditActivityWidget::reset(){
    ui->dtNotify->setMinimumDateTime(end->createdDate);
    if(edit){
        activity->initialize();
        setValues();
    }
    else{
        ui->txtEdName->setPlainText("");
        ui->dtNotify->setDateTime(ui->dtNotify->minimumDateTime());
        ui->chkNotify->setChecked(true);
        ui->chkPoints->setChecked(true);
        ui->sbxSprint->setValue(end->getCurrentSprint());
        ui->sbxWeight->setValue(ui->sbxSprint->minimum());
        setSprintText(end->getCurrentSprint());
    }
}


/*! \fn void QEditActivityWidget::setSprintText(int spr)
 * Provides information of sprint which the user is assigning the activity to.
 * \param spr The sprint selected by user through UI
 */
void QEditActivityWidget::setSprintText(int spr){
    QSprint sprint(this, end, spr);
    QString spString("Selected Sprint Duration : <b>");
    spString.append(sprint.start.toString("dd MMM")).append("</b> to <b>").append(sprint.end.toString("dd MMM")).append("</b>");
    ui->lblSprint->setText(spString);
}


/*! \fn void QEditActivityWidget::on_chkNotify_toggled(bool checked)
 * Only enables inputting properties that require a Deadline in defition if user acknowledges to add a deadline.
 * \param checked If the control is checked.
 */
void QEditActivityWidget::on_chkNotify_toggled(bool checked)
{
     ui->dtNotify->setEnabled(checked);
     ui->chkCreatedDtBased->setEnabled(checked);
}

/*! \fn void QEditActivityWidget::on_chkPoints_toggled(bool checked)
 * Only enables inputting points if user acknowledges
 * \param checked If the control is checked.
 */
void QEditActivityWidget::on_chkPoints_toggled(bool checked)
{
    ui->sbxWeight->setEnabled(checked);
}

/*! \fn void QEditActivityWidget::on_chkCreatedDtBased_toggled(bool checked)
 * Only enables defining included sprint based on created date if user requires it
 * \param checked If the control is checked.
 */
void QEditActivityWidget::on_chkCreatedDtBased_toggled(bool checked)
{
    ui->sbxSprint->setEnabled(!checked);
    if(checked)
        setSprintText(end->getSprint(ui->dtNotify->dateTime()));
    else
        setSprintText(ui->sbxSprint->value());
}



/*! \fn void QEditActivityWidget::on_sbxSprint_valueChanged(int arg1)
 * \param arg1 Spinbox updated value
 * \sa void QEditActivityWidget::setSprintText(int spr)
 */
void QEditActivityWidget::on_sbxSprint_valueChanged(int arg1)
{
     setSprintText(arg1);
}

/*! \fn void QEditActivityWidget::on_dtNotify_dateTimeChanged(const QDateTime &dateTime)
 * \param dateTime DateTime edit updated value
 * \sa void QEditActivityWidget::setSprintText(int spr), int QProject = ::getSprint(QDateTime activityDeadline)
 */
void QEditActivityWidget::on_dtNotify_dateTimeChanged(const QDateTime &dateTime)
{
    if(ui->chkCreatedDtBased->isChecked())
       setSprintText(end->getSprint(dateTime));
}

/*! \fn void QEditActivityWidget::on_txtEdName_textChanged()
 * Checks user's ability to proceed with the action depending on values input on txtEdName control
 */
void QEditActivityWidget::on_txtEdName_textChanged()
{
    emit formValidated(ui->txtEdName->toPlainText().size() > 0);
    if(ui->txtEdName->toPlainText().size() <= 0)
        ui->lblValidation->setText("You must enter the Activity Text to proceed");
    else
        ui->lblValidation->setText("");
}
