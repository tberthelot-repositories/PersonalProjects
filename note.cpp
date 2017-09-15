#include "note.h"
#include "databasemgr.h"

QNote::QNote(QObject *parent, int vId) : QObject(parent), noteId(vId){
   //!< If a valid ID exists then the object is initialized through database.
    if(noteId > 0)
        initialize();
}

/*! \fn void OcNote::setValues(QString vText , QVariant vNumeric, QString vNumTag, int vParentId, QDateTime vCreated )
 * Function used to initialize note object properties.
 * \param vText String HTML text of the note
 * \param vNumeric If note has a numeric value attached to it, NULL if not
 * \param vNumTag String tag of the numeric value (if exists), else empty string.
 * \param vParentId Integer variable for a possible parent activity Id
 * \param vCreated Note created DateTime.
 */
void QNote::setValues(QString vText , QVariant vNumeric, QString vNumTag, int vParentId, QDateTime vCreated ){
   parentActivityId = vParentId;
   noteText = vText;
   numeric = vNumeric;
   numerictag = vNumTag;
   created = vCreated;
}


/*! \fn bool OcNote::addItem()
 * This method records the note variable values in a row in database
 * \return if the action was successful
 */
bool QNote::addItem(){
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("insert into notes(n_text, numeric, num_tag, created, a_id) values (:text, :numeric, :numtag, :created, :activityId)");
    q.prepare(queryStr);
    q.bindValue(":text", noteText);
    q.bindValue(":numeric", numeric);
    q.bindValue(":numtag", numerictag);
    created = QDateTime::currentDateTime();
    q.bindValue(":created", created);
    q.bindValue(":activityId", parentActivityId);
    QVariant success = q.exec();
    if(success.toBool())
        noteId =  q.lastInsertId().toInt();
    else
        qDebug() << "Note Insertion Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return success.toBool();
}

/*! \fn bool OcActivity::editItem()
 * This method records the updated values of an activity in database
 * \return if the action was successful
 */
bool QNote::editItem(){
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("update notes set n_text = :text, numeric = :numeric, num_tag = :numtag where n_id = :id");
    q.prepare(queryStr);
    q.bindValue(":id", noteId);
    q.bindValue(":text", noteText);
    q.bindValue(":numeric", numeric);
    q.bindValue(":numtag", numerictag);
    QVariant success = q.exec();
    if(!success.toBool())
        qDebug() << "Note Update Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return success.toBool();
}

/*! \fn bool OcNote::removeItem()
 * This method removes the note row from database.
 * \return if the action was successful
 */
bool QNote::removeItem(){
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("delete from notes where n_id = :id");
    q.prepare(queryStr);
    q.bindValue(":id", noteId);
    QVariant success = q.exec();
    if(!success.toBool())
        qDebug() << "Note Deletion Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return success.toBool();
}


/*! \fn bool OcNote::initialize()
 * This method initializes the note object values from the database once the note ID was assigned
 * \return True if query was successful, False if id was invalid.
 */
bool QNote::initialize(){
    if(noteId > 0){
        QSqlQuery q(QDatabaseMgr::connect());
        QString queryStr("select * from notes where n_id = :id limit 0,1");
        q.prepare(queryStr);
        q.bindValue(":id", noteId);
        QVariant success = q.exec();
        if (success.toBool() && q.next()) {
            setValues(q.value("n_text").toString(), q.value("numeric") , q.value("num_tag").toString(), q.value("a_id").toInt(), q.value("created").toDateTime());
        }
        if(!success.toBool())
            qDebug() << "Note Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
        QDatabaseMgr::close();
        return success.toBool();
    }else{
        qDebug() << "Note Retrieval Error ; Invalid ID";
        return false;
    }
}

