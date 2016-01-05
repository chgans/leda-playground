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
 * - align items (H+V)
 * - distribute items
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
 * - change path (rect, ellipse, lins, ...)
 * - change content (text, ...)
 *
 * Commands from an editor point of view:
 *
 * Commands from a model point of view:
 * - Create (objType -> objId)
 * - Read (objIdList -> objList)
 * - Update (objIdList + map<prop, value>)
 * - Delete (objIdList)
 * - list (->objIdList
 */

#endif // COMMAND_H
