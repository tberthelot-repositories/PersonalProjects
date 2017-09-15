#ifndef LISTBASE_H
#define LISTBASE_H

#include <QObject>
#include <QPixmap>

/*! \class QListBase
 * \brief This class holds properties and methods of the Application's List object  (Object Class)
 */
class QListBase : public QObject
{
    Q_OBJECT
public:
    /*! Constructor taking parent and integer arguments and initializing object ID
     * \param parent Parent QObject
     * \param vListId Id of the list
     * \param vName Name of the list
     * \param vIcon String image path for the icon to be displayed.
     * \sa bool QListBase::initialize()
     */
    explicit QListBase(QObject *parent = 0, int vListId = 0, QString vName = "", QString vIcon = "");

    int listId; //!< List Id Number ( 1 <= x <= 8)
    QString name; //!< List Name
    QPixmap getIcon();
    QList<int> getChildren(int enId, int paId, int sprintId);

private:
    QString iconName; //!< String file name of the image of List Icon
};

#endif // LISTBASE_H
