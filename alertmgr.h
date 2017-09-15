#ifndef ALERTMGR_H
#define ALERTMGR_H

#include <QString>
#include <QDateTime>

/*! \class QAlertMgr
 * \brief This class manages date time notifications(Utility Class)
 *
 * Provides methods to create text alerts related to due dates and deadlines.
 */
class QAlertMgr
{
public:
    static QString getDateNotification(QDateTime vFrom, QDateTime vTo);
    static QString getDateNotification(QDate vFrom, QDate vTo);

private:
    //! Default Constructor
    //! Private constructor preventing instantiation since only static members are present
    QAlertMgr();
    //! Destructor.
    //! Empty Destructor
    ~QAlertMgr();
};

#endif // ALERTMGR_H
