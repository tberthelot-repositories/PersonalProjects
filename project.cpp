#include "project.h"
#include "databasemgr.h"
#include "activity.h"
#include "listbase.h"
#include "sprint.h"

QProject::QProject(QObject *parent, int vId): QObject(parent), enId(vId){
    //!< If a valid ID exists then the object is initialized through database.
    if(enId > 0)
        initialize();
}


/*! \fn void QProject = ::setValues(QString vName, QDateTime vCreated, QVariant vDeadline, bool vRecurring, bool vProduct, int vSWeeks)
 * Function used to initialize certain Endeavor object properties.
 * \param vName String Variable for Endeavor Name
 * \param vCreated DateTime variable for Created Date
 * \param vDeadline QVariant variable for Endeavor Deadline date
 * \param vRecurring Boolean value for whether Endeavor has recurring activities
 * \param vProduct Boolean value for whether Endeavor has a product to be delivered.
 * \param vSWeeks Integer variable for chosen number of weeks per sprint
 */
void QProject::setValues(QString vName, QDateTime vCreated, QVariant vDeadline, bool vRecurring, bool vProduct, int vSWeeks){
    enName = vName;
    createdDate = vCreated;
    deadLine = vDeadline;
    hasRecurring = vRecurring;
    hasProduct = vProduct;
    sprintWeeks = vSWeeks;
}

/*! \fn bool QProject = ::addItem()
 * This method records the Endeavor variable values in a row in database
 * \return if the action was successful
 */
bool QProject::addItem(){
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("insert into endeavors(e_name, has_recurring, has_product, deadline, created_date, sprint_weeks) values (:text, :recur, :product, :deadline, :created, :sprintWs)");
    q.prepare(queryStr);
    q.bindValue(":text", enName);
    q.bindValue(":recur", hasRecurring);
    q.bindValue(":product", hasProduct);
    q.bindValue(":deadline", deadLine);
    createdDate = QDateTime::currentDateTime();
    q.bindValue(":created",createdDate );
    q.bindValue(":sprintWs", sprintWeeks);
    QVariant success = q.exec();
    if(success.toBool())
        enId =  q.lastInsertId().toInt();
    else
        qDebug() << "Endeavor Insertion Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return success.toBool();
}

/*! \fn bool QProject = ::editItem()
 * This method records the updated values of an endeavor in database
 * \return if the action was successful
 */
bool QProject::editItem(){
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("update endeavors set e_name = :text, has_recurring = :recur, has_product = :product, deadline = :deadline, sprint_weeks = :sprintWs where e_id = :id");
    q.prepare(queryStr);
    q.bindValue(":id", enId);
    q.bindValue(":text", enName);
    q.bindValue(":recur", hasRecurring);
    q.bindValue(":product", hasProduct);
    q.bindValue(":deadline", deadLine);
    q.bindValue(":sprintWs", sprintWeeks);
    QVariant success = q.exec();
    if(!success.toBool())
        qDebug() << "Endeavor Update Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return success.toBool();
}

/*! \fn bool QProject = ::removeItem()
 * This method removes the endeavor row and activities of the endeavor from database
 * \return if the action was successful
 * \sa void QProject = ::removeChildren()
 */
bool QProject::removeItem(){
    removeChildren();
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("delete from endeavors where e_id = :id");
    q.prepare(queryStr);
    q.bindValue(":id", enId);
    QVariant success = q.exec();
    if(!success.toBool())
        qDebug() << "Endeavor Deletion Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return success.toBool();
}

/*! \fn bool QProject = ::initialize()
 * This method initializes the Endeavor object values from the database once the Endeavor ID was assigned.
 * \return True if query was successful, False if id was invalid.
 * \sa void QProject = ::assignToLists()
 */
bool QProject::initialize(){
    if(enId > 0){
        QSqlQuery q(QDatabaseMgr::connect());
        QString queryStr("select * from endeavors where e_id = :id  limit 0,1");
        q.prepare(queryStr);
        q.bindValue(":id", enId);
        QVariant success = q.exec();
        if (success.toBool() && q.next()) {
            setValues(q.value("e_name").toString(), q.value("created_date").toDateTime(),q.value("deadline"), q.value("has_recurring").toBool(),q.value("has_product").toBool() ,q.value("sprint_weeks").toInt()  );
        }
        if(!success.toBool())
            qDebug() << "Endeavor Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
        QDatabaseMgr::close();
        assignToLists();
        return success.toBool();
    }else{
        qDebug() << "Endeavor Retrieval Error ; Invalid ID";
        return false;
    }
}


/*! \fn void QProject = ::removeChildren()
 * This method removes the any child activities the endeavor might have therefore recursively deletes their child activities and notes.
 *  \sa bool QProject = ::removeItem()
 */
void QProject::removeChildren(){
    foreach(int listId, lists){
        QListBase normal(this, listId);
        foreach(int actvId, normal.getChildren(enId, 0 , 0)){
            QActivity actv(this, actvId);
            actv.removeItem();
        }
    }
}

/*! \fn int QProject = ::getNoOfSprints()
 * This method retrieves the number of sprints in the particular endeavor.
 * \return Number of sprints
 * \sa int QProject = ::getCurrentSprint(), int QProject = ::maximumSprint()
 */
int QProject::getNoOfSprints(){
    //! If deadline is null the method returns the user assigned maximum sprint of the endeavor
    if(deadLine.isNull())
        return maximumSprint();
    else{
     //! If deadline exists the method calculates the number of days between created date and deadline, divides it from number of weeks assigned per sprint and 7 (for days in a week) and returns the rounded up value.
        int days = createdDate.daysTo(deadLine.toDateTime());
        float noOfSprints = (float)days/ (float)(sprintWeeks * 7);
        return ceil(noOfSprints);
    }
}


/*! \fn int QProject = ::getCurrentSprint()
 * This method retrieves the current sprint
 * \return Current Sprint Number
 * \sa int QProject = ::getNoOfSprints(), int QProject = ::maximumSprint()
 */
int QProject::getCurrentSprint(){
    int val = 0;
    /*! Calculates number of days until today. */
    int days = createdDate.daysTo(QDateTime::currentDateTime());
    /*! If a deadline exists, checks if current day is past deadline. If so returns days until deadline (so the last sprint would be selected as current sprint */
    if(!deadLine.isNull()){
        if(QDateTime::currentDateTime().daysTo(deadLine.toDateTime()) < 0){
           days = createdDate.daysTo(deadLine.toDateTime());
        }
    }
    /*! Divides days from number of weeks assigned per sprint and 7 (for days in a week) and returns the rounded up value. */
    float currentSprint = (float)days/ (float)(sprintWeeks * 7);
    val = ceil(currentSprint);
    if(deadLine.isNull()){
        if(val > maximumSprint())
            val = maximumSprint();
    }
    return val;
}

/*! \fn int QProject = ::maximumSprint()
 * This method retrieves the maximum sprint the user has defined for the acitivities in a paticular endeavor. If there is none then a default 1 sprint would be returned.
 * \return Maximum Sprint Number
 * \sa int QProject = ::getNoOfSprints()
 */
int QProject::maximumSprint(){
    int count = 0;
    QSqlQuery q(QDatabaseMgr::connect());
    QString queryStr("select max(sprint_id) from activities where e_id = :id");
    q.prepare(queryStr);
    q.bindValue(":id", enId);
    QVariant success = q.exec();
    if (success.toBool() && q.next()){
           count = q.value(0).toInt();
    }
    if(!success.toBool())
        qDebug() << "Endeavor Null Deadline Sprint Count Retrieval Error ;" << q.lastError().text() << ";" << q.lastError().driverText();
    QDatabaseMgr::close();
    return count;
}


/*! \fn int QProject = ::getSprint(QDateTime activityDeadline)
 * This method returns the ID number of the sprint for an activity deadline that is passed as the variable.
 * \param activityDeadline QDateTime variable for a given deadline
 * \return Sprint number the deadline would fall into
 * \sa int QProject = ::getNoOfSprints()
 */
int QProject::getSprint(QDateTime activityDeadline){
    int days = createdDate.daysTo(activityDeadline);
    float sprint = (float)days/ (float)(sprintWeeks * 7);
    return ceil(sprint);
}


/*! \fn void QProject = ::assignToLists()
 * This method initializes the lists property of an endeavor.
 *  Based on the values intialized it determines which lists should be displayed and which shouldn't be. Adds them to the public QList list variable.
 * \sa bool QProject = ::initialize()
 */
void QProject::assignToLists(){
    lists.clear();
    lists.append(1); //To Do
    lists.append(2); //Doing
    lists.append(3); // Done
    if(hasRecurring){
        lists.append(4); //Recurring
    }
    lists.append(5);//Research
    lists.append(6);//Ideas
    if(hasProduct){
        lists.append(7); //Product Backlog
        lists.append(8); //Done Deliverables
    }
}




