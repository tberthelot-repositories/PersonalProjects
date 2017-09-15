#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include "itemmanager.h"
#include <QDateTime>
#include <QVariant>

/*! \class QNote
 * \brief This class contains properties and methods of a note that could be added to an activity (Object Class)
 */
class QNote : public QObject, public QItemManager
{
    Q_OBJECT
public:
    /*! Constructor taking parent and integer arguments and initializing object
     * \param parent Parent QObject
     * \param vId Id of the Note
     * \sa bool QNote::initialize(), void QNote::setValues(QString vText , QVariant vNumeric, QString vNumericTag, int vParentId, QDateTime vCreated)
     */
    explicit QNote(QObject *parent = 0, int vId = 0);

    int noteId; //!< Note Id number
    int parentActivityId; //!< Note parent activity Id number
    QString noteText; //!< Note rich text (HTML)
    QVariant numeric; //!< If note has a numeric value attached to it, NULL if not.
    QString numerictag; //!< Tag of the possible numeric value
    QDateTime created; //!< Note created date.

    bool addItem();
    bool editItem();
    bool removeItem();
    bool initialize();

private:
    void setValues(QString vText , QVariant vNumeric, QString vNumericTag, int vParentId, QDateTime vCreated);
};

#endif // NOTE_H
