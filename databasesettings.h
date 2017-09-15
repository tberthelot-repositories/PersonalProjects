#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QString>

/*! \class QDatabaseSettings
 * \brief This class manages SQLite Database File Location in the machine (Utility Class)
 *
 * Provides ways to back up the database, move the database to a seperate location (i.e Allowing the file to be syncronized using a seperate tool)
 *
 * \attention  static bool establishDatabase(),
    static bool restoreDefaultDatabase() methods are technically correct. However, they write a Read Only database file from application resources to user's disk and attempt to change write access to it. The QT capability of changing file permissions is platform and file system location dependent. Therefore these methods have been marked private, but kept for referential purposes. <a href="http://doc.qt.io/qt-5/qfile.html#setPermissions">More information</a>
*/
class QDatabaseSettings
{
public:
    static bool backupWorkingDatabase(QString);
    static QString getDatabaseLoc();
    static bool moveDatabase(QString);
    static bool checkDbExists();

private:
    //! Default Constructor
    //! Private constructor preventing instantiation since only static members are present.
    QDatabaseSettings();
    //! Destructor.
    //! Empty Destructor
    ~QDatabaseSettings();

    static bool establishDatabase();
    static bool restoreDefaultDatabase();
};

#endif // DATABASESETTINGS_H
