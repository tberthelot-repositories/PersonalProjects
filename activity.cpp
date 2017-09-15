#include "activity.h"
#include <QDebug>
#include "databasemgr.h"
#include "note.h"

QActivity::QActivity(QObject *parent, int vId): QObject(parent), id(vId){
    //!< If a valid ID exists then the object is initialized through database.
    if(id > 0)
        initialize();
}

/*! \fn void OcActivity::setValues(QString vText, QVariant vNotify, QDateTime vCreated, QDateTime vUpdated, int vOrder, int vParentId, int vEndId, int vListId, QVariant vPoints, int vSprint)
 * Function used to initialize certain Activity object properties.
 * \param vText String Variable for Activity Name
 * \param vNotify QVariant variable for Activity Notification/Deadline date
 * \param vCreated DateTime variable for Created Date
 * \param vUpdated DateTime variable for Updated Date
 * \param vOrder Integer variable for display index of the particular activity
 * \param vParentId Integer variable for a possible parent activity Id
 * \param vEndId Integer variable for Endeavor Containng the activity
 * \param vListId Integer variable for List the activity belongs to
 * \param vPoints Integer variable for Points the activity is worth
 * \param vSprint Integer variable for the sprint activity belongs to
 */
void QActivity::setValues(QString vText, QVariant vNotify, QDateTime vCreated, QDateTime vUpdated, int vOrder, int vParentId, int vEndId, int vListId, QVariant vPoints, int vSprint ){
    text = vText ;
    notify = vNotify;
    created = vCreated;
    updated = vUpdated;
    order = vOrder;
    parentId = vParentId;
    enId = vEndId;
    points = vPoints;
    sprint = vSprint;
    listId = vListId;
}

/*! \fn bool OcActivity::addItem()
 * This method records the activity variable values in a row in database
 * \return if the action was successful
 */
bool QActivity::addItem(){
        QSqlQuery q(QDatabaseMgr::connect());
        QString queryStr("insert into activities(a_text, notify, created_date, a_order, pa_id, e_id, a_list, points, updated_date, sprint_id) values (:text, :notify, :created, :aorder, :paId, :enId, :listId, :points, :updated, :sprint)");
        q.prepare(queryStr);
        q.bindValue(":text", text);
        q.bindValue(":notify", notify);
        created = QDateTime::currentDateTime();
        updated = QDateTime::currentDateTime();
        q.bindValue(":created", created);
        q.bindValue(":aorder", order);
        q.bindValue(":paId", parentId);
        q.bindValue(":enId", enId );
        q.bindValue(":listId", listId);
        q.bindValue(":points", points );
        q.bindValue(":updated", updated );
        q.bindValue(":sprint", sprint);
        QVariant success = q.exec();
        if(success.toBool())
            id =  q.lastInsertId().toInt();
        else
            qDebug() << "Activity Insertion Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
        QDatabaseMgr::close();
        return success.toBool();
}

/*! \fn bool OcActivity::editItem()
 * This method records the updated values of an activity in database
 * \return if the action was successful
 * \sa bool OcActivity::moveToList(int vListId)
 */
bool QActivity::editItem(){
        QSqlQuery q(QDatabaseMgr::connect());
        QString queryStr("update activities set a_text = :text, notify = :notify, a_order = :aorder,  a_list = :listId, points = :points, updated_date = :updated, sprint_id = :sprint where a_id = :id");
        q.prepare(queryStr);
        q.bindValue(":id", id);
        q.bindValue(":text", text);
        q.bindValue(":notify", notify);
        q.bindValue(":aorder", order);
        q.bindValue(":listId", listId );
        q.bindValue(":points", points );
        updated = QDateTime::currentDateTime();
        q.bindValue(":updated", updated );
        q.bindValue(":sprint", sprint);
        QVariant success = q.exec();
        if(!success.toBool())
            qDebug() << "Activity Update Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
        QDatabaseMgr::close();
        return success.toBool();
}

/*! \fn bool OcActivity::removeItem()
 * This method removes the activity row and connected child activities and notes from database.
 * \return if the action was successful
 * \sa void OcActivity::removeChildren() , void OcActivity::removeNotes()
 */
bool QActivity::removeItem(){
        removeNotes();
        removeChildren();
        QSqlQuery q(QDatabaseMgr::connect());
        QString queryStr("delete from activities where a_id = :id");
        q.prepare(queryStr);
        q.bindValue(":id", id);
        QVariant success = q.exec();
        if(!success.toBool())
            qDebug() << "Activity Deletion Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
        QDatabaseMgr::close();
        return success.toBool();
}

/*! \fn bool OcActivity::initialize()
 * This method initializes the Activity object values from the database once the Activity ID was assigned
 * \return True if query was successful, False if id was invalid.
 */
bool QActivity::initialize(){
    if(id > 0){
        QSqlQuery q(QDatabaseMgr::connect());
        QString queryStr("select * from activities where a_id = :id");
        q.prepare(queryStr);
        q.bindValue(":id", id);
        QVariant success = q.exec();
        if (success.toBool() && q.next()) {
            setValues(q.value("a_text").toString(),q.value("notify"),q.value("created_date").toDateTime(),q.value("updated_date").toDateTime(),q.value("a_order").toInt(), q.value("pa_id").toInt(), q.value("e_id").toInt(), q.value("a_list").toInt(), q.value("points"), q.value("sprint_id").toInt());
        }
        if(!success.toBool())
            qDebug() << "Activity Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
        QDatabaseMgr::close();
        return success.toBool();
    }else{
        qDebug() << "Activity Retrieval Error ; Invalid ID ; " << id;
        return false;
    }
}

/*! \fn void OcActivity::removeChildren()
 * This method removes the any child activities the activity might have had from database.
 *  \sa bool OcActivity::removeItem(), QList<int> OcActivity::getChildren(), void OcActivity::removeNotes()
 */
void QActivity::removeChildren(){
    foreach(int i, getChildren()){
        QActivity actv(this, i);
        actv.removeItem();
    }
}

/*! \fn void OcActivity::removeNotes()
 * This method removes the any notes associated with the activity from database
 * \sa bool OcActivity::removeItem(), QList<int> OcActivity::getNotes(), void OcActivity::removeChildren()
 */
void QActivity::removeNotes(){
    foreach(int i, getNotes()){
        QNote note(this, i);
        note.removeItem();
    }
}

/*! \fn QList<int> OcActivity::getChildren()
 * This method retrieves the child Activity Ids of the activity from which the method is called. They are ordered by the order they are arranged in view.
 * \return An Integer list of sub-activity (OcActivity) IDs
 * \sa void OcActivity::removeChildren()
 */
QList<int> QActivity::getChildren(){
    QList<int> children;
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("select a_id from activities where pa_id = :id order by a_order asc");
    q.prepare(queryStr);
    q.bindValue(":id", id);
    QVariant success = q.exec();
    if (success.toBool()){
        while (q.next()) {
            children.append(q.value("a_id").toInt());
     }
    }
    if(!success.toBool())
        qDebug() << "Activity Children Retrieval Error ; " << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return children;
}

/*! \fn QList<int> OcActivity::getNotes()
 * This method retrieves the IDs of notes added to the particular activity. They are arranged in the order they were added.
 * \return An Integer list of Note (OcNote) IDs
 * \sa void OcActivity::removeNotes()
 */
QList<int> QActivity::getNotes(){
    QList<int> noteList;
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("select n_id from notes where a_id = :id order by date(created)");
    q.prepare(queryStr);
    q.bindValue(":id", id);
    QVariant success = q.exec();
    if (success.toBool()){
        while (q.next()) {
            noteList.append(q.value("n_id").toInt());
     }
    }
    if(!success.toBool())
        qDebug() << "Activity Note Retrieval Error ; " << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return noteList;
}

/*! \fn int OcActivity::countNotes()
 * Retrieves the number of notes in the activity
 * \return Number of Notes in the activity
 * \sa int OcActivity::countChildren()
 */
int QActivity::countNotes(){
    int count = 0;
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("select count(*) from notes where a_id = :id ");
    q.prepare(queryStr);
    q.bindValue(":id", id);
    QVariant success = q.exec();
    if (success.toBool() && q.next()){
           count = q.value(0).toInt();
    }
    if(!success.toBool())
        qDebug() << "Activity Note Count Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return count;
}

/*! \fn int OcActivity::countChildren()
 * Retrieves the number of child activities to which this activity is parent to.
 * \return Number of child activities.
 * \sa int OcActivity::countDoneChildren() , int OcActivity::countNotes()
 */
int QActivity::countChildren(){
    int count = 0;
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("select count(*) from activities where pa_id = :id");
    q.prepare(queryStr);
    q.bindValue(":id", id);
    QVariant success = q.exec();
    if (success.toBool() && q.next()){
           count = q.value(0).toInt();
    }
    if(!success.toBool())
        qDebug() << "Activity Children Count Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return count;
}

/*! \fn int OcActivity::countDoneChildren()
 * Retrieves the number of child activities of this activity that are marked as completed.
 * \return Number of completed child activities.
 * \sa int OcActivity::countChildren()
 */
int QActivity::countDoneChildren(){
    int count = 0;
    int done;

    /*! If the parent activity is in Product Backlog List (7) , then it's Done Children is counted
     * from Done Deliverables List. (8). If it's not (Ideally in the ToDo-list or Doing-list),
     * then Completed Children is counted from Done-list
    */
    if(listId == 7)
        done = 8;
    else
        done = 3;

    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("select count(*) from activities where pa_id = :id and a_list = :donelist");
    q.prepare(queryStr);
    q.bindValue(":id", id);
    q.bindValue(":donelist", done);
    QVariant success = q.exec();
    if (success.toBool() && q.next()){
           count = q.value(0).toInt();
    }
    if(!success.toBool())
        qDebug() << "Activity Children Completed Count Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return count;
}

/*! \fn bool OcActivity::moveToList(int vListId)
 * Records the list change of the activity as it's moved. Called upon drag and drop.
 * \param vListId of the target list to which the activity is being moved to.
 * \return If action was successfully recorded in database
 * \sa bool OcActivity::editItem()
 */
bool QActivity::moveToList(int vListId){
    listId = vListId;
    updated = QDateTime::currentDateTime();
    switch(listId){
        case 3:
        case 8:
            foreach(int i, getChildren()){
                QActivity actv(this, i);
                actv.moveToList(listId);
            }
            break;
    }
    return editItem();
}
