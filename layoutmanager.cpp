#include "layoutmanager.h"
#include <QLayoutItem>
#include <QWidget>

QLayoutManager::QLayoutManager(){}
QLayoutManager::~QLayoutManager(){}

/*! \fn void QLayoutManager::removeLayoutItemsFromLayout(QLayout *layout)
 * Clears layout that's passed to the method as a pointer
 * \param layout Pointer to the layout of the UI which you need to clear of its items.
 * \author <a href="http://stackoverflow.com/a/7077340 "> StackOverflow Answer </a> (Sole Author)
 */
void QLayoutManager::removeLayoutItemsFromLayout(QLayout *layout){
    QLayoutItem* item;
    while ((item = layout->takeAt(0) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
}
