#include "editprojectwidget.h"
#include "ui_editprojectwidget.h"

#include <QMessageBox>

QEditProjectWidget::QEditProjectWidget(QWidget *parent, QProject *en) :
    QEditor(parent), end(en), edit(en->enId > 0),
    ui(new Ui::QEditProjectWidget)
{
    ui->setupUi(this);
    reset();
}

QEditProjectWidget::~QEditProjectWidget()
{
    delete ui;
}

/*! \fn void QEditProjectWidget::setValues()
 * Sets all the initialized values of the endeavor object to display controls assigned for them.
 */
void QEditProjectWidget::setValues(){
    ui->dtNotify->setMinimumDateTime(end->createdDate);
    ui->txtName->setText(end->enName);

    ui->chkNotify->setChecked(!end->deadLine.isNull());
    if(!end->deadLine.isNull())
        ui->dtNotify->setDateTime(end->deadLine.toDateTime());

    ui->chkProduct->setChecked(end->hasProduct);
    ui->chkRecurring->setChecked(end->hasRecurring);

    ui->sbxSprintWeeks->setValue(end->sprintWeeks);

}

/*! \fn void QEditProjectWidget::getValues()
 * Updates existing values of the Endeavor object by assigning user input values from respective controls
 */
void QEditProjectWidget::getValues(){
    end->enName = ui->txtName->text();

    if(ui->chkNotify->isChecked())
        end->deadLine = ui->dtNotify->dateTime();
    else
        end->deadLine = QVariant();

    end->hasProduct = ui->chkProduct->isChecked();
    end->hasRecurring = ui->chkRecurring->isChecked();
    end->sprintWeeks = ui->sbxSprintWeeks->value();
}

/*! \fn void QEditProjectWidget::performAccepted()
 * Performs final actions on endeavor dependin on its context (To add new or edit existing). Emits accepted signal to mark the function as finalized and successful.
 * \sa  void QEditor::accepted();
 */
void QEditProjectWidget::performAccepted(){
    getValues();
    if(edit){
        if(end->editItem())
            QMessageBox::information(this, "Edited", "Operation Successful");
        else
            QMessageBox::information(this, "Edited", "Operation Failed. Please Try Again");
    }else
    {
        if(end->addItem())
            QMessageBox::information(this, "Added", "Operation Successful");
        else
            QMessageBox::information(this, "Added", "Operation Failed. Please Try Again");
    }
    emit accepted();
}

/*! \fn void QEditProjectWidget::performRejected()
 * Discards changes made in UI and emits Rejected Signal to close the window
 * \sa  void QEditor::rejected();
 */
void QEditProjectWidget::performRejected(){
    if(edit){
        end->initialize();
    }
    reset();
    emit rejected();
}

/*! \fn void QEditProjectWidget::reset()
 * Discards changes made in UI by user and re-establishes the initial conditions.
 */
void QEditProjectWidget::reset(){
    if(edit){
        end->initialize();
        setValues();
    }
    else{
        ui->txtName->setText("");
        ui->dtNotify->setMinimumDateTime(QDateTime::currentDateTime());
        ui->dtNotify->setDateTime(ui->dtNotify->minimumDateTime());
        ui->chkNotify->setChecked(false);
        ui->chkProduct->setChecked(false);
        ui->chkRecurring->setChecked(false);
        ui->sbxSprintWeeks->setValue(ui->sbxSprintWeeks->minimum());
    }
}

/*! \fn void QEditProjectWidget::on_chkNotify_toggled(bool checked)
 * Enables user to add a deadline to endeavor upon acknowledgement of existance of a deadline
 * \param checked If the control is checked.
 */
void QEditProjectWidget::on_chkNotify_toggled(bool checked)
{
     ui->dtNotify->setEnabled(checked);
}


/*! \fn void QEditProjectWidget::on_txtName_textChanged(const QString &arg1)
 * Checks user's ability to proceed with the action depending on values input on txtName control
 * \param arg1 Current Text of the txtName control
 */
void QEditProjectWidget::on_txtName_textChanged(const QString &arg1)
{
    emit formValidated(arg1.size() > 0);
    if(arg1.size() <= 0)
        ui->lblValidation->setText("You must enter a valid Endeavor Name");
    else
        ui->lblValidation->setText("");
}
