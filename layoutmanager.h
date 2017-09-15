#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

#include <QLayout>

/*! \class QLayoutManager
 * \brief Abstract Class that defines and supports methods to be implemented by widgets that dynamically add child events.
 *
 * Inherited classes are QListWidget, WiEndavour, QActivityChildrenWidget  and QActivityNotesWidget
 */
class QLayoutManager
{
public:
    //! Constructor.
    QLayoutManager();
    //! Destructor.
    ~QLayoutManager();

protected slots:
     virtual void reInitialize() = 0;

protected:
     virtual void setUp() = 0 ;
     virtual void removeItemsFromLayout() = 0;
     void removeLayoutItemsFromLayout(QLayout *layout);

};

#endif // ACLAYOUTMANAGER_H
