#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "listbase.h"

/*! \class QInitializer
 * \brief This class performs some initial functions to set up UI (Utility Class)
 *
 * Intializes and provides all the variables necessary for initial UI creation. Intializes the Lists QList which is used throughout the application.
 */
class QInitializer
{
public:
    static QList<QListBase*> lists; //!< The Default set of lists used in endeavors.
    static void setLists();
    static QList<int> getEndeavors();

private:
    //! Default Constructor
    //! Private constructor preventing instantiation since only static members are present.
    QInitializer();
    //! Destructor.
    //! Empty Destructor, Only static variables are initialized and they should not be destructed.
    ~QInitializer();
};

#endif // INITIALIZER_H


