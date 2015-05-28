#include "mainwindow.h"
#include "symbol.h"
#include "symbolparser.h"

#include <QApplication>
#include <QMultiMap>
#include <QDir>
#include <QDebug>

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QSvgGenerator>
#include <QPainter>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // This detect copies of files, better work on CSV, since we want to ignore symbol
    // caption and description (only the drawing elements)
    if (argc == 2 && QString::fromLocal8Bit(argv[1]) == "--hash") {
        QMultiMap<uint, QString> fileMap;
        QDir topDir(SYMBOL_DIR);
        topDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (QString dirname, topDir.entryList()) {
            QDir subDir(topDir.absoluteFilePath(dirname));
            subDir.setFilter(QDir::Files);
            subDir.setNameFilters(QStringList() << "*.xml");
            foreach (QFileInfo info, subDir.entryInfoList()) {
                QString path = info.absoluteFilePath();
                QFile file(path);
                file.open(QFile::ReadOnly);
                QByteArray content = file.readAll();
                uint hash = qHash(content);
                file.close();
                fileMap.insert(hash, info.canonicalFilePath());
            }
        }
        qDebug() << "XML files with same hash value:";
        foreach (uint hash, fileMap.uniqueKeys()) {
            if (fileMap.count(hash) > 1) {
                foreach (const QString &path, fileMap.values(hash))
                    qDebug() << hash << path;
            }
        }
        return 0;
    }

    // Create SVG icons
    else if (argc == 2 && QString::fromLocal8Bit(argv[1]) == "--icons") {
        QDir topDir(SYMBOL_DIR);
        topDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (QString dirname, topDir.entryList()) {
            QDir subDir(topDir.absoluteFilePath(dirname));
            subDir.setFilter(QDir::Files);
            subDir.setNameFilters(QStringList() << "*.xml");
            foreach (QFileInfo info, subDir.entryInfoList()) {
                QString path = info.absoluteFilePath();
                SymbolParser *parser = new SymbolParser();
                Symbol *sym = parser->parse(path);
                if (sym == nullptr) {
                    qCritical() << parser->errorString();
                    continue;
                }
                QGraphicsScene scene;
                QGraphicsItemGroup *group = scene.createItemGroup(sym->drawings);
                QSvgGenerator generator;
                QSize size = group->boundingRect().size().toSize();
                QString svg = info.baseName() + ".svg";
                generator.setFileName(subDir.absoluteFilePath(svg));
                qDebug() << "Generating" << generator.fileName() << size;
                generator.setSize(size);
                generator.setViewBox(QRect(0, 0, size.width(), size.height()));
                generator.setTitle(sym->name);
                generator.setDescription(sym->label);
                QPainter painter;
                painter.begin(&generator);
                painter.setRenderHint(QPainter::Antialiasing);
                scene.render(&painter);
                painter.end();
            }
        }
        return 0;
    }

    w.show();
    return a.exec();
}
