#include "alertmgr.h"
#include <math.h>

QAlertMgr::QAlertMgr(){}
QAlertMgr::~QAlertMgr(){}


/*! \fn QString QAlertMgr::getDateNotification(QDateTime from, QDateTime to)
 * This method compares the date time values passed as parameters and provides a text which presents the comparison in a relevant manner.
 * \param from DateTime from which the comparison should start
 * \param to DateTime up to which the comparison should be considered
 * \return Alert Text
 * \sa QString QAlertMgr::getDateNotification(QDate from, QDate to)
 */
QString QAlertMgr::getDateNotification(QDateTime from, QDateTime to){
    QString val;
    int days = from.daysTo(to);
    if (days == 0){
        int seconds = from.time().secsTo(to.time());
        int hours = floor(seconds%3600);
        if(hours > 0){
            val = QString("Due in %1 hours").arg(hours);
        }else{
            int mins = floor(seconds%60);
            if(mins > 0){
                val = QString("Due in %1 mins").arg(mins);
            }
            else{
                val = QString("Due now");
            }
        }
    }else if(days > 30) {
        val = QString(to.toString("dd MMM"));
    }else{
        val = getDateNotification(from.date(), to.date());
    }
    return val;
}

/*! \fn QString QAlertMgr::getDateNotification(QDate from, QDate to)
 * This method compares the dates passed as parameters and provides a text which presents the comparison in a relevant manner.
 * \param from Date from which the comparison should start
 * \param to Date up to which the comparison should be considered
 * \return Alert Text
 * \sa QString QAlertMgr::getDateNotification(QDateTime from, QDateTime to)
 */
QString QAlertMgr::getDateNotification(QDate from, QDate to){
    QString val;
    int days = from.daysTo(to);
    if(days < 0){
        val = QString("Overdue by %1 days").arg(-days);
    }else if(days ==0){
        val = QString("Due today");
    }
    else{
        val = QString("Due in %1 days").arg(days);
    }
    return val;
}

