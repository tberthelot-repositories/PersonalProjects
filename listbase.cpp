#include "listbase.h"
#include "databasemgr.h"

QListBase::QListBase(QObject *parent, int vListId, QString vName, QString vIcon) : QObject(parent), listId(vListId), name(vName), iconName(vIcon){}

/*! \fn QPixmap QListBase::getIcon()
 * Creates a Pixamp Image using iconName given
 * \return Pixamp image of List Icon.
 */
QPixmap QListBase::getIcon(){
    QString ico(":/images/lists/");
    ico.append(iconName);
    ico.append(".png");
    return QIcon(ico).pixmap(QSize(20,20));
}

/*! \fn QList<int> QListBase::getChildren(int enId, int paId, int sprintId)
 * Retrieves activities that belongs to this particular list, its endeavor, a parent activity and if not 0, then the given sprint. They are ordered by the sprint they belong to and the order they were arranged in view.
 * \param enId Integer Id of Endeavor
 * \param paId integer Id of Parent Activity.If 0 it would select the topmost activities.
 * \param sprintId Integer id of the sprint
 * \return List of integer Ids of activities that belongs to the list.
 */
QList<int> QListBase::getChildren(int enId, int paId, int sprintId){
    QList<int> children;
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("select a_id from activities where e_id = :eid and a_list = :listId and pa_id = :parent ");
    if(sprintId > 0)
        queryStr.append("and sprint_id = :sprint ");
    queryStr.append(" order by sprint_id, a_order asc");
    q.prepare(queryStr);
    q.bindValue(":eid", enId);
    q.bindValue(":listId", listId);
    q.bindValue(":parent", paId);    
    if(sprintId > 0)
        q.bindValue(":sprint", sprintId);
    QVariant success = q.exec();
    if (success.toBool()){
        while (q.next()) {
            children.append(q.value("a_id").toInt());
        }
    }
    if(!success.toBool()){
        qDebug() << "List Children Retrieval Error" << q.lastError().text() << ";" << q.lastError().driverText();
        if(sprintId > 0)
            qDebug() << "Sprint Retrieval";
    }
    QDatabaseMgr::close();
    return children;
}

/* Archived Code *//*
bool QListBase::initialize(){
    if(listId > 0){
        QSqlQuery q(UcDb::connect());
        QString queryStr("select * from lists where l_id = :id limit 0,1");
        q.prepare(queryStr);
        q.bindValue(":id", listId);
        QVariant success = q.exec();
        if (success.toBool() && q.next()) {
           setValues(q.value("l_name").toString(), q.value("l_img").toString());
        }
        if(!success.toBool())
            qDebug() << "List Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
        UcDb::close();
        return success.toBool();
    }else{
        qDebug() << "List Retrieval Error ; Invalid ID ; " << listId;
        return false;
    }
}
*/
