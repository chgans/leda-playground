#ifndef SCHPORT_H
#define SCHPORT_H

#include <QGraphicsItem>

class EdlPort;

class SchItem: public QGraphicsItem
{
public:
    SchItem(QGraphicsItem * parent = 0):
        QGraphicsItem(parent)
    {}
};


/*
 * Schematic Library Editor
 *
 */

class SchPort: public SchItem
{
public:
    SchPort(QGraphicsItem * parent = 0);
    void fromEdl(const EdlPort &port);
    EdlPort toEdL() const;


private:
    QGraphicsItem *mPort;
    QGraphicsSimpleTextItem *mPortId;
    QGraphicsSimpleTextItem *mPortLabel;
    bool mShowPortId;
    bool mShowPortLabel;
};

class SchAttribute: public SchItem
{
public:
    SchAttribute(QGraphicsItem * parent = 0);

private:
    QString mName;
    QString mValue;
    QGraphicsSimpleTextItem *mAttribute;
    bool mShowName;
    bool mShowValue;
};

class SchEntity:  public SchItem
{
public:
    SchEntity(QGraphicsItem * parent = 0);

private:
    QGraphicsSimpleTextItem *mEntityId;
    QGraphicsSimpleTextItem *mEntityLabel;
    QList<SchPort*> mPortList;
    QList<SchAttribute*> mAttributeList;
};

#endif // SCHPORT_H
