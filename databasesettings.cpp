#include "databasesettings.h"
#include <QTextStream>
#include <QDir>
#include <QFile>

QDatabaseSettings::QDatabaseSettings(){}
QDatabaseSettings::~QDatabaseSettings(){}


/*! \fn bool UcDbSettings::backupWorkingDatabase(QString dir)
 * Copies the database file to a location selected by user.
 * \param dir Target folder location string
 * \return If copy operation was successful.
 */
bool QDatabaseSettings::backupWorkingDatabase(QString dir){
    QDir target(dir);
    QFile existingDb(getDatabaseLoc());
    bool val=  existingDb.copy(target.filePath("personalprojects.db"));
    if(val)
        existingDb.setPermissions(QFile::WriteUser);
    return val;
}

/*! \fn QString UcDbSettings::getDatabaseLoc()
 * Determines the location of the database file.
 * \return The location of the database.
 * \sa bool UcDbSettings::moveDatabase(QString dir)
 */
QString QDatabaseSettings::getDatabaseLoc(){
    //! Default location of the database file.
    QString dbLoc = QDir::home().filePath("personalprojectsapp/personalprojects.db");

    /*! Checks if a different location for the database has been specified in usersettings.txt file. If so reads it and returns the file path as database location instead of default location. */
    QFile settingsTxt(QDir::home().filePath("personalprojectsapp/usersettings.txt"));
    if(settingsTxt.exists()){
        settingsTxt.open(QFile::ReadOnly | QIODevice::Text);
        QTextStream readSt(&settingsTxt);
        if (!readSt.atEnd()) {
            dbLoc = readSt.readLine();
        }
        settingsTxt.close();
    }
    return dbLoc;
}


/*! \fn bool UcDbSettings::moveDatabase(QString dir)
 * Moves the database file to a location specified by the user. This locatino is then written to usersettings.txt file in the default application directory in User's Home Folder.
 * \param dir Target folder location string
 * \return If copy operation was successful.
 * \sa QString UcDbSettings::getDatabaseLoc()
 */
bool QDatabaseSettings::moveDatabase(QString dir){
    QDir target(dir);
    QString targetFilePath = target.filePath("personalprojects.db");
    QFile existingDb(getDatabaseLoc());

    bool copied = existingDb.copy(targetFilePath);
    QString targetTextPath = QDir::home().filePath("personalprojectsapp/usersettings.txt");
    if(copied){
        existingDb.setPermissions(QFile::WriteUser);
        QFile settingsTxt(targetTextPath);
        settingsTxt.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream writeSt(&settingsTxt);
        writeSt << targetFilePath;
        settingsTxt.close();
        existingDb.remove();
        return true;
    }else{
        return false;
    }
}

/* Archived Methods. Refer to header file for more information. */
/*! \fn bool UcDbSettings::establishDatabase()
 * Writes the clean database file to a folder in User's Home Folder. This method should not depend on anything else.
 * \return If folder creation and file copy operations were successful
 * \warning Method Should not depend on anything else except for bool UcDbSettings::restoreDefaultDatabase().
 * \warning Not used in application due to Write Access depending on OS, file system or folder location.
 * \sa bool UcDbSettings::restoreDefaultDatabase()
 */
bool QDatabaseSettings::establishDatabase(){
    bool returnVal = false;
    QDir appDir(QDir::home().filePath("personalprojectsapp"));
    QFile dbFile(appDir.filePath("personalprojects.db"));
    QFile settingsTxt(appDir.filePath("usersettings.txt"));
    if(dbFile.exists() || settingsTxt.exists()){
        returnVal =  true;
    }else if(appDir.exists()){
         return returnVal = restoreDefaultDatabase();
    }else{
        QDir::home().mkdir("personalprojectsapp");
        return returnVal = restoreDefaultDatabase();
    }
    return returnVal;
}

/*! \fn bool UcDbSettings::restoreDefaultDatabase()
 * Writes the clean database file to application specifc folder created at time of initial installation.
 * \return If file copy operation was successful
 * \warning  Method Should not depend on anything else.
 * \warning Not used in application due to Write Access depending on OS, file system or folder location.
 * \sa bool UcDbSettings::establishDatabase()
 */
bool QDatabaseSettings::restoreDefaultDatabase(){
    QFile resourceDb(":/db/personalprojects.db");
    QDir appDir(QDir::home().filePath("personalprojectsapp"));
    QFile settingsTxt(appDir.filePath("usersettings.txt"));
    if(settingsTxt.exists()){
      settingsTxt.remove();
    }
    bool val = resourceDb.copy(appDir.filePath("personalprojects.db"));
    if(val)
        resourceDb.setPermissions(QFile::WriteUser);
    return val;
}

/*! \fn bool UcDbSettings::checkDbExists()
 * Check if database exists
 * \return True if database exists, false if not
 */
bool QDatabaseSettings::checkDbExists(){
   return QFile(getDatabaseLoc()).exists();
}
