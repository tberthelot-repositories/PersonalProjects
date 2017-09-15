#ifndef DATABASEMGR_H
#define DATABASEMGR

#include <QtSql>

/*! \class QDatabaseMgr
 * \brief This class manages Database connection (Utility Class)
 *
 * Provides object classes static methods to initiate a safe connection and close after it's
 * been used.
 */
class QDatabaseMgr
{
public:
    static QSqlDatabase connect();
    static void close();
    static void removeCon();

private:
    //! Default Constructor
    //! Private constructor preventing instantiation since only static members are present.
    QDatabaseMgr();
    //! Destructor.
    //! Empty Destructor, Only static variables are initialized and they should not be destructed.
    ~QDatabaseMgr();
    static QString conName; //!< Database connection name ; to recognize it from multiple connections initiated by application
    static QString dbName;  //! Database connection string ; Location to the database in hard disk
    static QSqlDatabase db; //! static QSqlDatabase returned to each connection

    static void establishCon();

};

#endif // DATABASEMGR
