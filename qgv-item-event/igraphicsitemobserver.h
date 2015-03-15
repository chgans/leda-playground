#ifndef IGRAPHICSITEMOBSERVER_H
#define IGRAPHICSITEMOBSERVER_H

#include <QList>

class IGraphicsObservableItem;

class IGraphicsItemObserver
{
public:
    IGraphicsItemObserver();
    virtual ~IGraphicsItemObserver();

    virtual void itemNotification(IGraphicsObservableItem *item) = 0;

    void beginObservedItemTransaction();
    void addObservedItem(IGraphicsObservableItem *item);
    void removeObservedItem(IGraphicsObservableItem *item);
    void endObserveredItemTransaction();

    QList<IGraphicsObservableItem *> observedItems() const;

private:
    QList<IGraphicsObservableItem *> m_items;
    bool m_operationInProgress;
};
#endif // IGRAPHICSITEMOBSERVER_H
