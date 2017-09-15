#include "listwidget.h"
#include "ui_listwidget.h"
#include <QMessageBox>
#include <QPalette>
#include <QDrag>
#include <QMimeData>
#include "editorwidget.h"

QListWidget::QListWidget(QWidget *parent, QListBase *vList, int enId, int paId, int sprintId) :
    QWidget(parent), list(vList), endId(enId), parentActivityId(paId), sprint(sprintId),
    ui(new Ui::QListWidget)
{
    ui->setupUi(this);
    ui->lblListName->setText(list->name);
    ui->lblImg->setPixmap(list->getIcon());
    ui->vLayoutActivities->setAlignment(Qt::AlignTop);
    noOfItems = -1;

    setUp();

    //! Set up background colors for each list item to hint user about their possible functionality.
    QPalette pal(palette());
    switch(list->listId){
    //! ToDo, Doing, Done lists are same color
     case 1:
     case 2:
     case 3:
        pal.setColor(QPalette::Window, QColor(203, 143, 129, 30));
        break;
     case 4:
        pal.setColor(QPalette::Window, QColor(200, 160, 145, 30));
        break;
     case 5:
        pal.setColor(QPalette::Window, QColor(193, 151, 145, 30));
        break;
     case 6:
        pal.setColor(QPalette::Window, QColor(190, 150, 140, 30));
        break;
        //! Product backlog and Done Deliverables lists are same color
     case 7:
     case 8:
        pal.setColor(QPalette::Window, QColor(241, 140, 85, 30));
        break;
    }

    setPalette(pal);
    setAutoFillBackground(true);

    //! Disable adding child activities to completed activities
    if(list->listId == 3 || list->listId == 8){
        ui->btnAddChild->setVisible(false);
    }
    else if(paId > 0){
        int inclList = QActivity(0, paId).listId;
        ui->btnAddChild->setVisible(!(inclList == 3 || inclList == 8));
    }
}

QListWidget::~QListWidget()
{
    delete ui;
}

/*! \fn void QListWidget::setUp()
 * Adds the initialized activities of the list to the interface with their respective characteristics.
 * /sa QList<int> QListBase::getChildren(int enId, int paId, int sprintId), void QListWidget::reInitialize()
 */
void QListWidget::setUp(){
    removeItemsFromLayout();
    foreach(int item, list->getChildren(endId, parentActivityId, sprint))
    {
        connectToLayout(new QActivityWidget(this, new QActivity(this, item)));
    }
}

/*! \fn void QListWidget::removeItemsFromLayout()
 * Removes all List items from the Vertical layout.
 * \sa void UcLayoutMgr::removeItemsFromLayout(QLayout *layout)
 */
void QListWidget::removeItemsFromLayout(){
    removeLayoutItemsFromLayout(ui->vLayoutActivities);
}

/*! \fn void QListWidget::moveItemUp(int pos)
 * Moves the layout item in position specified upwards and the widget that was previously in it's place down.
 * \param pos Position of item
 * \sa void QActivityWidget::moveUp(int val)
 */
void QListWidget::moveItemUp(int pos){
    if(pos > 0){
        QActivityWidget *lower = (QActivityWidget*) ui->vLayoutActivities->itemAt(pos)->widget();
        QActivityWidget *upper = (QActivityWidget*) ui->vLayoutActivities->itemAt(pos-1)->widget();
        ui->vLayoutActivities->removeWidget(upper);
        ui->vLayoutActivities->removeWidget(lower);
        addItemToLayout(pos-1, lower);
        addItemToLayout(pos, upper);
    }else{
         QMessageBox::warning(0, "Warning", "You cannot move up from here");
    }
}

/*! \fn void QListWidget::moveItemDown(int pos)
 * Moves the layout item in position specified downwards and the widget that was previously in it's place upwards.
 * \param pos Position of item
 * \sa void QActivityWidget::moveDown(int val)
 */
void QListWidget::moveItemDown(int pos){
    if(pos < noOfItems){
        QActivityWidget *upper = (QActivityWidget*) ui->vLayoutActivities->itemAt(pos)->widget();
        QActivityWidget *lower = (QActivityWidget*) ui->vLayoutActivities->itemAt(pos+1)->widget();
        ui->vLayoutActivities->removeWidget(upper);
        ui->vLayoutActivities->removeWidget(lower);
        addItemToLayout(pos, lower);
        addItemToLayout(pos+1, upper);
    }
    else{
        QMessageBox::warning(0, "Warning", "You cannot move down from here");
    }
}


/*! \fn void QListWidget::dragEnterEvent(QDragEnterEvent *event)
 * Process the ability to drop the object when a drag and drop object is being moved to the list.
 * \param event QDragEnterEvent
 * \sa void QActivityWidget::mousePressEvent(QMouseEvent *event), void QListWidget::dropEvent(QDropEvent *event),
 */
void QListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    bool acceptance = false;
    //! Enable drop only if the list is allowed recieve by checking mime data format
    switch(list->listId){
     case 1:
     case 2:
     case 3:
        if (event->mimeData()->hasFormat("application/dnd-activity-todos") || event->mimeData()->hasFormat("application/dnd-activity-copy") )
            acceptance= true;
        break;
     case 7:
     case 8:
        if (event->mimeData()->hasFormat("application/dnd-activity-products"))
            acceptance= true;
        break;
    }
    if(acceptance)
        event->acceptProposedAction();

}


/*! \fn void QListWidget::dropEvent(QDropEvent *event)
 * Process the Drop Action by checking mime data format and therein the nature of drop, whether to copy or move.
 *  \param event QDropEvent
 * \sa void QActivityWidget::mousePressEvent(QMouseEvent *event), void QListWidget::dragEnterEvent(QDragEnterEvent *event)
 */
void QListWidget::dropEvent(QDropEvent *event)
{
    bool acceptance = false;
    bool copyac = false;
    QString id;
    switch(list->listId){
     case 1:
     case 2:
     case 3:
        if (event->mimeData()->hasFormat("application/dnd-activity-todos")){
            acceptance= true;
            id = QString(event->mimeData()->data("application/dnd-activity-todos"));
        }
        if (event->mimeData()->hasFormat("application/dnd-activity-copy")){
            acceptance= true;
            copyac = true;
            id = QString(event->mimeData()->data("application/dnd-activity-copy"));
        }
        break;
     case 7:
     case 8:
        if (event->mimeData()->hasFormat("application/dnd-activity-products")){
            acceptance= true;
            id = QString(event->mimeData()->data("application/dnd-activity-products"));
        }
        break;
    }
    if(acceptance && !copyac){
        //! If Drop action is to move, move it to the containing list, connect it to layout, declare and accept the drop action.
        QActivity *activity = new QActivity(0, id.toInt());
        connectToLayout(new QActivityWidget(0, activity));
        activity->moveToList(list->listId);
        event->setDropAction(Qt::MoveAction);
        event->acceptProposedAction();
    }
    else if(acceptance && copyac){
        //! If Drop action is to copy, add a new activity with the changed listId and upon success, connect it to layout, define and accept the declare action.
        QActivity *activity = new QActivity(0, id.toInt());
        activity->initialize();
        activity->id = 0;
        activity->listId = list->listId;
        activity->enId =  endId;
        activity->parentId = 0;
        bool success = activity->addItem();
        if(success)
            connectToLayout(new QActivityWidget(this, activity));
        event->setDropAction(Qt::CopyAction);
        event->acceptProposedAction();
    }else{
            QMessageBox::warning(this, "Drag And Drop", "Dropping in this list isn't possible. Please stick to the color code of Lists");
    }
}


/*! \fn void QListWidget::connectToLayout(QActivityWidget *act)
 * Initialize connections on the QActivityWidget that was is to the List
 *  \param act QActivityWidget Widget initialized previously
 * \sa void QListWidget::addItemToLayout(int pos, QActivityWidget *act)
 */
void QListWidget::connectToLayout(QActivityWidget *act){
    noOfItems++;
    addItemToLayout(noOfItems, act);
    connect(act, SIGNAL(moveUp(int)), this, SLOT(moveItemUp(int)));
    connect(act, SIGNAL(moveDown(int)), this, SLOT(moveItemDown(int)));
    connect(act, SIGNAL(forceRefresh()), this, SLOT(reInitialize()));
}

/*! \fn void QListWidget::addItemToLayout(int pos, QActivityWidget *act)
 * Adds the QActivityWidget widgets passed to the Vertical Layout of the list widget.
 * \param pos position to insert the widget at
 * \param act QActivityWidget Widget initialized previously
 * \sa void QListWidget::connectToLayout(QActivityWidget *act), void QActivityWidget::setOrders(int val)
 */
void QListWidget::addItemToLayout(int pos, QActivityWidget *act){
    ui->vLayoutActivities->insertWidget(pos, act);
    int position = ui->vLayoutActivities->indexOf(act);
    act->setOrders(position);
}

/*! \fn void QListWidget::reInitialize()
 * Reinitializes the widgets contained inside the 'this' widget.
 * /sa void QListWidget::setUp()
 */
void QListWidget::reInitialize(){
    setUp();
}

/*! \fn void QListWidget::on_btnAddChild_clicked()
 * Displays the Dialog to add a new activity, if returned as accepted and if the activity was added to same sprint that's on display, adds a QActivityWidget widget to the QListWidget.
 */
void QListWidget::on_btnAddChild_clicked()
{
    QActivity *actv = new QActivity(this, 0);
    actv->listId = list->listId;
    actv->enId =  endId;
    actv->parentId = parentActivityId;
    QEditorWidget *di = new QEditorWidget(this, actv);
    int res = di->exec();
    if(res == QDialog::Accepted){
        if(!(actv->sprint != sprint && actv->parentId == 0))
            connectToLayout(new QActivityWidget(this, actv));
        if(actv->parentId == 0)
            emit forceSprintRefresh(actv->sprint);
    }
}
