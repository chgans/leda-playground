#ifndef LOGGINGWIDGET_H
#define LOGGINGWIDGET_H

#include <QFrame>

namespace Ui {
class LoggingWidget;
}

class LoggingModel;
class LoggingSortFilterProxyModel;
class QMenu;

class LoggingWidget : public QFrame
{
    Q_OBJECT

public:
    explicit LoggingWidget(QWidget *parent = 0);
    ~LoggingWidget();

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
    Ui::LoggingWidget *ui;
    LoggingModel *m_model;
    LoggingSortFilterProxyModel *m_filterModel;
    QMenu *m_categoriesMenu;
};

#endif // LOGGINGWIDGET_H
