#include "activitychildrenwidget.h"
#include "ui_activitychildrenwidget.h"
#include "initializer.h"

QActivityChildrenWidget::QActivityChildrenWidget(QWidget *parent, QActivity *act) : QDialog(parent), activity(act),
    ui(new Ui::QActivityChildrenWidget)
{
    ui->setupUi(this);
    ui->hLayoutLists->setAlignment(Qt::AlignLeft);
    this->setWindowTitle(activity->text.left(30).append("..."));
    setUp();
}

QActivityChildrenWidget::~QActivityChildrenWidget()
{
    delete ui;
}

/*! \fn void QActivityChildrenWidget::setUp()
 * Determines which lists to be displayed in regard with the list parent activity belongs to and displays them.
 */
void QActivityChildrenWidget::setUp(){
    QList<int> lists;
    switch(activity->listId){
        case 1:
        case 2:
        case 3:
            lists.append(1);
            lists.append(2);
            lists.append(3);
            break;
        case 7:
        case 8:
            lists.append(7);
            lists.append(8);
            break;
    }

    //! Creates QListWidget widgets using the determined values and their respective QListBase objects and adds them to layout.
    foreach(int x, lists){
        addList(new QListWidget(this, QInitializer::lists.at(x-1), activity->enId, activity->id));
    }
}

/*! \fn void QActivityChildrenWidget::reInitialize()
 * Private slot that refreshes the layout and it's items. Removes all Items from layout and sets up the form again. Called upon a forceRefresh() signal from a QListWidget widget currently in the Horizontal Layout.
 */
void QActivityChildrenWidget::reInitialize(){
    removeItemsFromLayout();
    setUp();
}

/*! \fn void QActivityChildrenWidget::removeItemsFromLayout()
 * Removes all the layout items from Horizontal Layout.
 * \sa void UcLayoutMgr::removeItemsFromLayout(QLayout *layout)
 */
void QActivityChildrenWidget::removeItemsFromLayout(){
   removeLayoutItemsFromLayout(ui->hLayoutLists);
}


/*! \fn void QActivityChildrenWidget::addList(QListWidget *list)
 * Adds the QListWidget widgets passed to the Horizontal Layout of the current widget.
 * \param list QListWidget Widget initialized previously
 */
void QActivityChildrenWidget::addList(QListWidget *list){
    ui->hLayoutLists->addWidget(list);
    //! Connect to reinitialize upon catching forceRefresh from an added QListWidget.
    connect(list, SIGNAL(forceRefresh()), this, SLOT(reInitialize()));
}
