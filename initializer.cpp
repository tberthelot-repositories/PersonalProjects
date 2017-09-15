#include "initializer.h"
#include <QDebug>
#include "databasemgr.h"
#include "project.h"

QList<QListBase*> QInitializer::lists;

QInitializer::QInitializer(){}
QInitializer::~QInitializer(){}

/*! \fn void UcInitializer::setLists()
 * This method loads the list parameters from database, instantiates all Lists  from database and assigns it to UcInitializer::lists
 */
void QInitializer::setLists(){
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("select * from lists");
    q.prepare(queryStr);
    QVariant success = q.exec();
    if (success.toBool()) {
        while (q.next()) {
            lists.append(new QListBase(0, q.value("l_id").toInt(), q.value("l_name").toString(),q.value("l_img").toString()));
        }
    }
    if(!success.toBool())
        qDebug() << "Complete-List Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
}

/*! \fn QList<int> UcInitializer::getEndeavors()
 * This methods retrieves all IDs of endeavors the user have added to the database and return them as a list.
 * \return A List of IDs of all endeavors in database
 */
QList<int> QInitializer::getEndeavors(){
    QSqlQuery q(QDatabaseMgr::connect());
    QList<int> endeavors;
    QString queryStr("select e_id from endeavors order by date(created_date)");
    q.prepare(queryStr);
    QVariant success = q.exec();
    if (success.toBool()) {
        while (q.next()) {
            endeavors.append(q.value(0).toInt());
        }
    }
    if(!success.toBool())
        qDebug() << "Endeavors (All) Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return endeavors;
}



/* Archived Code *//*
QList<int> UcInitializer::getAllActivities(int listId){
    QSqlQuery q(UcDb::connect());
    QList<int> activities;
    QString queryStr("select a_id from activities where a_list = :list");
    q.prepare(queryStr);
    q.bindValue(":list", listId);
    QVariant success = q.exec();
    if (success.toBool()) {
        while (q.next()) {
            activities.append(q.value(0).toInt());
        }
    }
    if(!success.toBool())
        qDebug() << "Activity List Retrieval Error ;" << listId << q.lastError().text() << ";" << q.lastError().driverText();
    UcDb::close();
    return activities;
} */


