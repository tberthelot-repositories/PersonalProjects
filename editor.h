#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>

/*! \class QEditor
 * \brief Abstract Class that defines slots and signals for a widget that could be included in DiEditor form.
 *
 * This Class directly connects with DiEditor Class. Inherited classes are QEditActivityWidget, QEditProjectWidget or QNoteWidget
 */
class QEditor : public QWidget
{
    Q_OBJECT
public:
    /*! Constructor
     * \param parent Parent QWidget - Widget/Window/Dialog which has the widget.
     */
    explicit QEditor(QWidget *parent = 0);
    //! Destructor.
    //! Virtual Destructor ; Due to the class being abstract
    virtual ~QEditor(){}

signals:
    /*! \fn void QEditor::rejected()
     * Emitted after QEditor::performRejected() is processed
*/
    void rejected();
    /*! \fn void QEditor::accepted()
     * Emitted after QEditor::performAccepted() is processed
*/
    void accepted();
    /*! \fn void QEditor::formValidated(bool)
     * Emitted once the form is validated
     * \param val Whether values are properly added and if it's safe to proceed.
*/
    void formValidated(bool val);

public slots:
    /*! \fn virtual void QEditor::performAccepted()
     * Called when Save button of the DiEditor's Button box is clicked.
     */
    virtual void performAccepted() = 0;
    /*! \fn virtual void QEditor::performRejected()
     * Called when Discard button of the DiEditor's Button box is clicked.
     */
    virtual void performRejected() = 0;
    /*! \fn virtual void QEditor::reset()
     * Called when Reset button of the DiEditor's Button box is clicked.
     */
    virtual void reset() = 0;

protected:
    virtual void setValues() = 0;
    virtual void getValues() = 0;

};

#endif // EDITOR_H
