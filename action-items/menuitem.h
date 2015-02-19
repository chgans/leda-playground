#ifndef MENUITEM_H
#define MENUITEM_H

#include <QList>
#include <QString>

class MenuItem;
typedef QList<MenuItem*> MenuItemList;

class MenuItem
{
public:
    MenuItem(const QString &title, MenuItem *parent = 0):
        m_title(title),
        m_parent(parent),
        m_children(MenuItemList())
    {
        if (parent)
            parent->appendChild(this);
    }

    QString title()
    {
        return m_title;
    }

    void setParent(MenuItem *parent)
    {
        m_parent = parent;
    }

    MenuItem *parent()
    {
        return m_parent;
    }

    void setChildren(MenuItemList children)
    {
        foreach(MenuItem *child, children)
            child->setParent(this);
        m_children = children;
    }

    MenuItemList children()
    {
        return m_children;
    }

    void appendChild(MenuItem *item)
    {
        item->setParent(this);
        m_children << item;
    }

private:
    QString m_title;
    MenuItem *m_parent;
    MenuItemList m_children;
};

#endif // MENUITEM_H
