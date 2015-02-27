#ifndef MESSAGELOGVIEWER_H
#define MESSAGELOGVIEWER_H

#include <QFrame>

namespace Ui {
class MessageLogViewer;
}

class LoggingModel;
class LoggingSortFilterProxyModel;
class QMenu;

// TODO: move navigation, clear and save to IOutputPane
class MessageLogViewer : public QFrame
{
    Q_OBJECT

public:
    explicit MessageLogViewer(QWidget *parent = 0);
    ~MessageLogViewer();

    LoggingModel *model();
    void setModel(LoggingModel *model);

    bool canNavigate() const;
    bool canGoToNext() const;
    bool canGoToPrevious() const;

protected slots:
    void updateNavigationState();
    void goToNext();
    void goToPrevious();
    void updateCategoryList(const QList<const char *> categories);
    void filterCategoriesTriggered(QAction *action);

protected:
    void setCategoryVisibility(const char *categoryName, bool visible);

private:
    Ui::MessageLogViewer *ui;
    LoggingModel *m_model;
    LoggingSortFilterProxyModel *m_filterModel;
    QMenu *m_categoriesMenu;
};

#endif // MESSAGELOGVIEWER_H
