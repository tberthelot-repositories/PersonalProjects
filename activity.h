#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QObject>
#include "itemmanager.h"
#include <QDateTime>
#include <QVariant>
#include <QList>

/*! \class QActivity
 * \brief This class contains properties and methods of an Activity Object (Object Class)
 */
class QActivity : public QObject, public QItemManager
{
    Q_OBJECT
public:
    /*! Constructor taking parent and integer arguments and initializing object
     * \param parent Parent QObject
     * \param vId Id of the activity
     * \sa void QActivity::setValues(QString vText, QVariant vNotify, QDateTime vCreated, QDateTime vUpdated, int vOrder, int vParentId, int vEndId, int vListId, QVariant vPoints, int vSprint ), bool QActivity::initialize()
     */
    explicit QActivity(QObject *parent = 0, int vId = 0);

    int id; //!< Activity ID number
    QString text; //!< Activity name; A Self explanatory phrase
    QVariant notify; //!< Activity notify DateTime, Sometimes referred to as a Deadline for the activity. Could be NULL or a Date Time Value
    QDateTime created; //!< Activity Created Date Time
    QDateTime updated; //!< Activity Updated Date Time. Referred to find out if the activity is in a final list.
    int order; //!< Index of the activity when displayed the list view. Could cause conflicts in "All Sprints View"
    int parentId; //!< Parent Activity Id. If 0, Activity is a top activity. Considered Endeavor Activities
    int enId; //!< Endeavor to which the activity belongs to
    int listId; //!< List number to which the activity belongs to
    QVariant points; //!< Weight, or the worth of the activity. Could be NULL or an integer value
    int sprint; //!< Sprint id to which the activity would belong. Either calculated or manually input.

    bool addItem();
    bool editItem();
    bool moveToList(int list);
    bool removeItem();
    bool initialize();
    int countChildren();
    int countDoneChildren();
    int countNotes();
    QList<int> getChildren();
    QList<int> getNotes();

private:
    void setValues(QString vText, QVariant vNotify, QDateTime vCreated, QDateTime vUpdated, int vOrder, int vParentId, int vEndId, int vListId, QVariant vPoints, int vSprint );
    void removeChildren();
    void removeNotes();

};

#endif // ACTIVITY_H
