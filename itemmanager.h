#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

/*! \class QItemManager
 * \brief Abstract Class that defines methods to be implemented by Object Classes that perform CRUD operations on the database
 *
 * Inherited classes are OcActivity, QProject =  and OcNote
 */
class QItemManager
{
public:
    //! Constructor.
    QItemManager();
    //! Destructor.
    ~QItemManager();
    virtual bool addItem() = 0;
    virtual bool editItem()= 0;
    virtual bool removeItem()= 0;
    virtual bool initialize()= 0;
};

#endif // ITEMMANAGER_H
