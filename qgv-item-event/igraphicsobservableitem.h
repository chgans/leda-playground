#ifndef IGRAPHICSOBSERVABLEITEM_H
#define IGRAPHICSOBSERVABLEITEM_H

#include <QList>

class IGraphicsItemObserver;

class IGraphicsObservableItem
{
public:
    IGraphicsObservableItem();
    virtual ~IGraphicsObservableItem();

    void beginItemObserverTransaction();
    void addItemObserver(IGraphicsItemObserver *observer);
    void removeItemObserver(IGraphicsItemObserver *observer);
    void endItemObserverTransaction();
    QList<IGraphicsItemObserver *> itemObservers() const;

protected:
    void notifyObservers();

private:
    QList<IGraphicsItemObserver *> m_observers;
    bool m_transactionInProgress;
};

#endif // IGRAPHICSOBSERVABLEITEM_H
