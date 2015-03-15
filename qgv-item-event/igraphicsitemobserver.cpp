#include "igraphicsitemobserver.h"
#include "igraphicsobservableitem.h"

IGraphicsItemObserver::IGraphicsItemObserver():
    m_operationInProgress(false)
{

}

IGraphicsItemObserver::~IGraphicsItemObserver()
{
    foreach(IGraphicsObservableItem *item, m_items) {
        removeObservedItem(item);
    }
}

void IGraphicsItemObserver::beginObservedItemTransaction()
{
    Q_ASSERT(!m_operationInProgress);
    m_operationInProgress = true;
}

void IGraphicsItemObserver::addObservedItem(IGraphicsObservableItem *item)
{
    if (m_items.contains(item))
        return;

    m_items.append(item);
    if (!m_operationInProgress) {
        item->beginItemObserverTransaction();
        item->addItemObserver(this);
        item->endItemObserverTransaction();
    }
}

void IGraphicsItemObserver::removeObservedItem(IGraphicsObservableItem *item)
{
    if (!m_items.contains(item))
        return;

    m_items.removeOne(item);
    if (!m_operationInProgress) {
        item->beginItemObserverTransaction();
        item->removeItemObserver(this);
        item->endItemObserverTransaction();
    }
}

void IGraphicsItemObserver::endObserveredItemTransaction()
{
    Q_ASSERT(m_operationInProgress);
    m_operationInProgress = false;
}
