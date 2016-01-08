#ifndef COMMAND_H
#define COMMAND_H


class Editor;

class Command
{
public:
    Command();
    virtual ~Command();

    virtual void redo(Editor *editor) = 0;
    virtual void undo(Editor *editor) = 0;
};

#include <QPen>

class ChangePenCommand: public Command
{
public:
    ChangePenCommand();
    ~ChangePenCommand();

    void redo(Editor *editor);
    void undo(Editor *editor);

private:
    QPen m_oldPen;
    QPen m_newPen;
};

#include <QBrush>

class ChangeBrushCommand: public Command
{
public:
    ChangeBrushCommand();
    ~ChangeBrushCommand();

    void redo(Editor *editor);
    void undo(Editor *editor);

private:
    QBrush m_oldPen;
    QBrush m_newPen;
};

#include <QPointF>

class MoveItemsCommand: public Command
{
public:
    MoveItemsCommand();
    ~MoveItemsCommand();

    void redo(Editor *editor);
    void undo(Editor *editor);

private:
    QPointF m_vector;
};

class RotateItemsCommand: public Command
{
public:
    RotateItemsCommand();
    ~RotateItemsCommand();

    void redo(Editor *editor);
    void undo(Editor *editor);

private:
    QPointF m_center;
    qreal m_angle;
};

class MirrorCommand
{
public:
    typedef enum {
        VerticalMirroring = 0,
        HorizontalMirroring
    } Orientation;

    MirrorCommand();
    ~MirrorCommand();

    void redo(Editor *editor);
    void undo(Editor *editor);

private:
    bool m_wasMirrored;
    Orientation m_orientation;
};


/*
 * Commands from an author point of view:
 *
 * - add item
 * - delete items
 * - clone items (copy/paste)
 *
 * - move items
 * - rotate items
 * - mirror items (H+V)
 * - align items (H: left/center/right, V: top/center/bottom)
 * - distribute items (H: left/center/right/space, V: top/center/bottom/space)
 * - TBD: align/distribute for text
 *
 * - change visibility
 * - change opacity
 *
 * - bring to front
 * - send to back
 * - raise
 * - lower
 *
 * - group
 * - ungroup
 *
 * - lock
 * - unlock
 *
 * - change pen
 * - change brush
 *
 * - change item (rect, ellipse, line, text, ...)
 *
 * Commands from an editor point of view:
 *
 * - add item
 * - remove item
 * - group items
 * - ungroup items
 * - set position
 * - set rotation
 * - set mirrored
 * - set visible
 * - set opacity
 * - set z-value
 * - set locked
 * - set pen
 * - set brush
 *
 *
 * Commands from a model point of view:
 * - Create (objType -> objId)
 * - Read (objIdList -> objList)
 * - Update (objIdList + map<prop, value>)
 * - Delete (objIdList)
 * - Query (->objIdList)
 */

#endif // COMMAND_H
