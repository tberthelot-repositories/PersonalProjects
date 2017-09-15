#include "sprint.h"

QSprint::QSprint(QObject *parent, QProject *belongsTo, int sprint) : QObject(parent), sprintId(sprint)
{
    //!< Start and end dates are calculates in relavance to Sprint Weeks and created date.
    int sprintDays = belongsTo->sprintWeeks*7;
    start = belongsTo->createdDate.addDays((sprintId-1)*sprintDays);
    end = start.addDays(sprintDays - 1);
    //!< if End date is after the defined deadline, then Deadline of the endeavor is assigned as end date.
    if(!belongsTo->deadLine.isNull()){
        int compare = end.daysTo(belongsTo->deadLine.toDateTime());
        if(compare <= 0)
              end = belongsTo->deadLine.toDateTime();
    }
}
