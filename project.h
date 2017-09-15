#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include "itemmanager.h"
#include <QDateTime>
#include <QVariant>
#include <QList>

/*! \class QProject = 
 * \brief This class contains properties and methods of an Endeavor (Object Class)
 */

class QProject : public QObject, public QItemManager
{
    Q_OBJECT
public:
    /*! Constructor taking parent and integer arguments and initializing object
     * \param parent Parent QObject
     * \param vId Id of the endeavor object. Default value is 0
     * \sa void QProject = ::setValues(QString vName, QDateTime vCreated, QVariant vDeadline, bool vRecurring, bool vProduct, int vSWeeks),  bool QProject = ::initialize()
     */
    explicit QProject(QObject *parent = 0, int vId = 0);

    int enId; //!< Endeavor ID number
    QString enName; //!< Endeavor name; a short introductory phrase containing 2-3 words (Not constrained)
    QDateTime createdDate; //!< Endeavor Created Date
    QVariant deadLine; //!< Endeavor Deadline. Could be NULL or a Date Time value. Number of sprints in an Endeavor will be defined through this.
    bool hasRecurring; //!< Whether Endeavor has recurring activities user needs to keep track of.  If false wouldn't display the Recurring-List
    bool hasProduct; //!< Whether Endeavor has a product to be delivered at the end. If false wouldn't display Product Backlog and Done Deliverables lists.
    int sprintWeeks; //!< Chosen number of weeks per sprint.
    QList<int> lists; //!< A list of Indexes of Application-Lists (QListBase) the endeavor supports.

    bool addItem();
    bool editItem();
    bool removeItem();
    bool initialize();
    int getSprint(QDateTime deadline);
    int getNoOfSprints();
    int getCurrentSprint();

private:
    void setValues(QString vName, QDateTime vCreated, QVariant vDeadline, bool vRecurring, bool vProduct, int vSWeeks);
    void removeChildren();
    void assignToLists();
    int maximumSprint();
};

#endif // PROJECT_H
