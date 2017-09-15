#include "activitywidget.h"
#include "ui_activitywidget.h"
#include <QString>
#include <QMessageBox>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsOpacityEffect>
#include "alertmgr.h"
#include "editorwidget.h"
#include "activitychildrenwidget.h"
#include "activitynoteswidget.h"

QActivityWidget::QActivityWidget(QWidget *parent, QActivity *activ) :
    QWidget(parent),
    ui(new Ui::QActivityWidget), activity(activ)
{
    ui->setupUi(this);
//    activity->initialize();
    setValues();

    //! Assign visibility characteristics based on containing list
    switch(activity->listId){
        case 4:
        case 5:
        case 6:
        case 8:
            ui->btnViewChildren->setVisible(false);
            ui->lblicoChildren->setVisible(false);
            ui->lblChildren->setVisible(false);
            break;
    }
}

QActivityWidget::~QActivityWidget()
{
    delete ui;
}

/*! \fn void QActivityWidget::setValues()
 * Sets all the initialized values of the activity object to display units assigned for them. Enforce visibility restrictions depending on container list or availablity of a certain property.
 */
void QActivityWidget::setValues(){
    ui->lblActivity->setText(activity->text);
    ui->lblNotify->setVisible(!activity->notify.isNull() && activity->listId != 3);
    ui->lblAddedDate->setText(activity->created.toString("dd MMM yy"));
    ui->lblSprintNo->setText(QString::number(activity->sprint));
    ui->lblNotes->setText(QString::number(activity->countNotes()));

    if(!activity->notify.isNull()){
        ui->lblNotify->setToolTip(activity->notify.toDateTime().toString("dd MMM yy HH:mm ap"));
        ui->lblNotify->setText(QAlertMgr::getDateNotification(QDateTime::currentDateTime(), activity->notify.toDateTime()));
    }

    ui->lblicoWeight->setVisible(!activity->points.isNull());
    ui->lblWeight->setVisible(!activity->points.isNull());
    if(!activity->points.isNull()){
        ui->lblWeight->setText(activity->points.toString());
    }

    int activityChildren = activity->countChildren();
    if(activityChildren > 0){
        QString children(QString::number(activity->countDoneChildren()));
        children.append("/").append(QString::number(activityChildren));
        ui->lblChildren->setText(children);
    }
}

/*! \fn void QActivityWidget::setFrameOpacity(float opacity)
 * Sets the transparency of the widget.
 * \param opacity Transparency of the widget
 * \author <a href="http://stackoverflow.com/a/7077340 "> StackOverflow Answer </a> (Sole Author)
 */
void QActivityWidget::setFrameOpacity(qreal opacity){
    QGraphicsOpacityEffect *transparency = new QGraphicsOpacityEffect(ui->frame);
    transparency->setOpacity(opacity);
    ui->frame->setGraphicsEffect(transparency);
}

/*! \fn void QActivityWidget::mousePressEvent(QMouseEvent *event)
 * Initialize the Drag and Drop Event upon Left button mouse press event inside Label Move.
 *  \param event Mouse Event
 * \sa void QListWidget::dragEnterEvent(QDragEnterEvent *event), void QListWidget::dropEvent(QDropEvent *event)
 */
void QActivityWidget::mousePressEvent(QMouseEvent *event)
{
     if (event->button() == Qt::LeftButton && ui->lblMove->geometry().contains(event->pos())) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        QString str("");
        bool copyac = false;


//! Define the copy characteristics depending on the list type contained in through mime data.
        switch(activity->listId){
         case 1:
         case 2:
         case 3:
            str.append("application/dnd-activity-todos");
            break;
         case 4:
         case 5:
         case 6:
            copyac = true;
            str.append("application/dnd-activity-copy");
            break;
         case 7:
         case 8:
            str.append("application/dnd-activity-products");
            break;
        }

        //! Set data, which is a data stream of the Activity Id
        mimeData->setData(str, QString::number(activity->id).toStdString().c_str());
        drag->setMimeData(mimeData);

        //! Set Pixamp (Image displayed while drag and drop is going on and the hot spot, where in the image the mouse would be displayed.
        drag->setPixmap(this->grab());
        drag->setHotSpot(event->pos());

        setFrameOpacity(0.7);
        if(copyac){
            drag->exec(Qt::CopyAction);
            setFrameOpacity(1);
        }
        else{
            //! If a Move Action is defined, remove the origin of the action.
            if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
                delete this;
            else{
                setFrameOpacity(1);
            }
        }
    }
}


/*! \fn void QActivityWidget::setOrders(int val)
 * If list position differs from Activity Order, update activity order.
 *  \param val Position of container list.
 * \sa void QListWidget::addItemToLayout(int pos, QActivityWidget *act)
 */
void QActivityWidget::setOrders(int val){
    if( activity->order != val){
        activity->order = val;
        activity->editItem();
    }
//  ui->btnUp->setDisabled(activity->order == 0);
}


/*! \fn void QActivityWidget::on_btnEdit_clicked()
 * Edit the activity through a new Dialog Window and refresh container display values if item was edited.
 * \sa void QActivityWidget::setValues()
 */
void QActivityWidget::on_btnEdit_clicked()
{
    QEditorWidget di(this, activity);
    di.setWindowFlags(Qt::Window);
    int res = di.exec();
    if(res == QDialog::Accepted){
        activity->initialize();
        setValues();
    }
}

/*! \fn void QActivityWidget::on_btnDelete_clicked()
 * Delete the activity object in the widget from the database and emit container refresh signal.
 * \sa void QActivityWidget::forceRefresh()
 */
void QActivityWidget::on_btnDelete_clicked()
{
    if(activity->removeItem()){
        QMessageBox::information(this, "Removed", "Operation Successful");
    }else{
        QMessageBox::information(this, "Removal", "Operation Failed. Please Try Again");
    }
    emit forceRefresh();
}

/*! \fn void QActivityWidget::on_btnUp_clicked()
 *  Emit moveUp on button click.
 * \sa void moveUp(int val) , void QActivityWidget::on_btnDown_clicked()
 */
void QActivityWidget::on_btnUp_clicked()
{
    emit moveUp(activity->order);
}

/*! \fn void QActivityWidget::on_btnDown_clicked()
 * Emit moveDown on button click.
 * \sa void moveDown(int val) , void QActivityWidget::on_btnUp_clicked()
 */
void QActivityWidget::on_btnDown_clicked()
{
    emit moveDown(activity->order);
}

/*! \fn void QActivityWidget::on_btnViewChildren_clicked()
 * Display all child activities of this activity in a new Dialog Window.
 * \sa void QActivityWidget::on_btnShowNotes_clicked()
 */
void QActivityWidget::on_btnViewChildren_clicked()
{
    QActivityChildrenWidget di(this, activity);
    di.exec();
    setValues();
}

/*! \fn void QActivityWidget::on_btnShowNotes_clicked()
 * Display all notes of this activity in a new Dialog Window.
 * \sa void QActivityWidget::on_btnViewChildren_clicked()
 */
void QActivityWidget::on_btnShowNotes_clicked()
{
    QActivityNotesWidget di(this, activity);
    di.exec();
    setValues();
}
