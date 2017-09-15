#include "databasemgr.h"
#include "databasesettings.h"

QString QDatabaseMgr::conName("qt_sql_personalprojects_db_con");
QString QDatabaseMgr::dbName = QDatabaseSettings::getDatabaseLoc();
QSqlDatabase QDatabaseMgr::db = QSqlDatabase::database(conName);

QDatabaseMgr::QDatabaseMgr(){}
QDatabaseMgr::~QDatabaseMgr(){}

/*! \fn void UcDb::establishCon()
 * This method links the database in the drive with memory with the connection name specified.
 * \warning Only has to be called once
 * \sa void UcDb::connect(), void UcDb::removeCon()
 */
void QDatabaseMgr::establishCon(){
     if (!QSqlDatabase::drivers().contains("QSQLITE")){
         qDebug() << "Database Connection Error ;" <<  "SQlite Driver not found";
     }
     else{
         db = QSqlDatabase::addDatabase("QSQLITE", conName);
         db.setDatabaseName(QDatabaseMgr::dbName);
     }
}

/*! \fn void UcDb::removeCon()
 * This method removes the linked database from memory.
 * \warning Should be called at application exit. Currently not explicitly used.
 * \sa UcDb::establishCon()
 */
void QDatabaseMgr::removeCon(){
    QSqlDatabase::removeDatabase(conName);
}

/*! \fn QSqlDatabase UcDb::connect()
 * This method opens the linked database safely and returns it to be used in query executions
 * \return Connected database
 * \sa UcDb::establishCon(), UcDb::close()
 */
QSqlDatabase QDatabaseMgr::connect(){
    db = QSqlDatabase::database(conName);
    if (!db.open()){
        establishCon();
        if(!db.open()){
          qDebug() << "Database Connection Error ;" <<  db.lastError().text() << ";" << db.lastError().driverText();
        }
    }
    return db;
}

/*! \fn void UcDb::close()
 * Safely closes the opened connection to the database.
 * \sa UcDb::connect()
 */
void QDatabaseMgr::close(){
    if(db.isOpen()){
        db.commit();
        db.close();
    }
}

