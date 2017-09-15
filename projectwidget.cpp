#include "projectwidget.h"
#include "ui_projectwidget.h"
#include <QMessageBox>
#include "activity.h"
#include "initializer.h"
#include "alertmgr.h"
#include "editorwidget.h"

QProjectWidget::QProjectWidget(QWidget *parent, QProject *end) :
    QWidget(parent),
    ui(new Ui::QProjectWidget), endeavor(end)
{
    ui->setupUi(this);
    ui->hLayoutLists->setAlignment(Qt::AlignLeft);
    ui->btnEdit->setVisible(endeavor->enId != 1);
    ui->btnDelete->setVisible(endeavor->enId != 1);
    setEnDetails();
    setUp();
}

QProjectWidget::~QProjectWidget()
{
    delete ui;
}

/*! \fn void WiEndavour::setEnDetails()
 * Sets all the initialized values of the Endeavor object to display units assigned for them. Enforce visibility restrictions depending on assigned values.
 */
void QProjectWidget::setEnDetails(){
    ui->lblDeadline->setVisible(!endeavor->deadLine.isNull());
    ui->lblicoDeadline->setVisible(!endeavor->deadLine.isNull());
    ui->lbltxtDeadline->setVisible(!endeavor->deadLine.isNull());
    ui->lblNotify->setVisible(!endeavor->deadLine.isNull());
    if(!endeavor->deadLine.isNull()){
        ui->lblDeadline->setText(endeavor->deadLine.toDate().toString("dd MMM yy"));
        ui->lblNotify->setText(QAlertMgr::getDateNotification(QDate::currentDate(), endeavor->deadLine.toDate()));
    }

    ui->lblAddedDate->setText(endeavor->createdDate.toString("dd MMM yy"));
    ui->lblSprintWeeks->setText(QString::number(endeavor->sprintWeeks));
    sprintComboInitialize(endeavor->getCurrentSprint());
}

/*! \fn void WiEndavour::setUp()
 * Adds the initialized lists of the endeavor to the interface with their respective characteristics.
 * /sa void UcInitializer::setLists()
 */
void QProjectWidget::setUp(){
    removeItemsFromLayout();
    int sprint = ui->cmbSprints->currentIndex();
    if(sprint <= 0)
        sprint = 0;
    foreach(int x, endeavor->lists){
        addList(new QListWidget(this, QInitializer::lists.at(x-1), endeavor->enId, 0 , sprint));
    }
}

/*! \fn void WiEndavour::addList(QListWidget *list)
 * Adds the QListWidget widgets passed to the Horizontal Layout of the current widget.
 * \param list QListWidget Widget initialized previously
 */
void QProjectWidget::addList(QListWidget *list){
    ui->hLayoutLists->addWidget(list);
    connect(list, SIGNAL(forceSprintRefresh(int)), this, SLOT(sprintComboInitialize(int)));
  //  connect(list, SIGNAL(forceRefresh()), this, SLOT(reInitialize()));
}

/*! \fn void WiEndavour::reInitialize()
 * Reinitializes the widgets contained inside the 'this' widget.
 * /sa void WiEndavour::setEnDetails(), void WiEndavour::setUp()
 */
void QProjectWidget::reInitialize(){
    setEnDetails();
    setUp();
}

/*! \fn void WiEndavour::removeItemsFromLayout()
 * Removes all List items from the Horizontal layout.
 * \sa void UcLayoutMgr::removeItemsFromLayout(QLayout *layout)
 */
void QProjectWidget::removeItemsFromLayout(){
   removeLayoutItemsFromLayout(ui->hLayoutLists);
}

/*! \fn void WiEndavour::sprintComboInitialize(int val)
 * Gets number of sprints available to be categorized activities under and adds the sprints to the combo box as a filter.
 * \param val Sets the selected index
 * \sa void WiEndavour::on_cmbSprints_currentIndexChanged(int index)
 */
void QProjectWidget::sprintComboInitialize(int val){
    while(ui->cmbSprints->count() != 1){
         ui->cmbSprints->removeItem(1);
    }
    for(int i = 1; i <= endeavor->getNoOfSprints(); i++)
        ui->cmbSprints->insertItem(i, QString("Sprint %1").arg(i));
    ui->cmbSprints->setCurrentIndex(val);
}

/*! \fn void WiEndavour::setSprintDetails(OcSprint *spr)
 * Sets the details of the selected sprint
 * \param spr OcSprint object with Sprint Details Required
 */
void QProjectWidget::setSprintDetails(QSprint *spr){
      ui->lblSprintFrom->setText(spr->start.toString("dd MMM"));
      ui->lblSprintTo->setText(spr->end.toString("dd MMM"));
      ui->lblSprintNotify->setText(QAlertMgr::getDateNotification(QDate::currentDate(), spr->end.date()));
}

/*! \fn void WiEndavour::on_btnEdit_clicked()
 * Edit the endeavor through a new Dialog Window, emits the signal to refresh container tab.
 */
void QProjectWidget::on_btnEdit_clicked()
{
    QEditorWidget di(this, endeavor);
    int res = di.exec();
    if(res == QDialog::Accepted){
        endeavor->initialize();
        emit forceRefresh(endeavor->enName);
        reInitialize();
    }
}

/*! \fn void WiEndavour::on_btnDelete_clicked()
 * Deletes the Endeavor object in the widget from the database and emit container removeTab signal.
 * \sa void MwDashboard::removeCurrentTab(), void MwDashboard::addEndeavorTab(QProject =  *end)
 */
void QProjectWidget::on_btnDelete_clicked()
{
    if(endeavor->removeItem()){
        QMessageBox::information(this, "Removed", "Operation Successful");
    }else{
        QMessageBox::information(this, "Removal", "Operation Failed. Please Try Again");
    }
    emit removeTab();
}

/*! \fn void WiEndavour::on_btnRefresh_clicked()
 * Reinitializes widget items
 * \sa void WiEndavour::reInitialize()
 */
void QProjectWidget::on_btnRefresh_clicked()
{
    reInitialize();
}

/*! \fn void WiEndavour::on_cmbSprints_currentIndexChanged(int index)
 * Sets the changed details of the currently selected sprint
 * \param index Index of the combo box Id of selected Sprint
 * \sa void WiEndavour::setSprintDetails(OcSprint *spr)
 */
void QProjectWidget::on_cmbSprints_currentIndexChanged(int index)
{
    ui->gbxSprint->setVisible(index > 0);
    if(index > 0){
            QSprint *spr = new QSprint(this, endeavor, index);
            setSprintDetails(spr);
    }
    setUp();
}

