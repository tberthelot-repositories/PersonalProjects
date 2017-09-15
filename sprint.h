#ifndef SPRINT_H
#define SPRINT_H

#include <QObject>
#include <QDateTime>
#include "project.h"

/*! \class QSprint
 * \brief This class calculates basic properties of a sprint in relevance with the Endeavor it belongs to (Object Class)
 */
class QSprint : public QObject
{
    Q_OBJECT
public:
    /*! Constructor taking parent, QProject =  and integer arguments and calculating public properties of item
     * \param parent Parent QObject
     * \param end QProject =  Endeavor of which the sprint details are required.
     * \param id Id of the Sprint
     */
    explicit QSprint(QObject *parent = 0, QProject *end = 0, int id= 0);
    QDateTime start; //!< Date the sprint starts
    QDateTime end; //!< Date the sprint ends on

private:
    int sprintId; //!< Index of the sprint in a given timeline.

};

#endif // SPRINT_H
