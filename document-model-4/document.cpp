#include "document.h"
#include "documentmodel.h"
#include "documentcommand.h"
#include "documentstupidobject.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDebug>
#include <QPointer>
#include <QUndoStack>

Document::Document(QObject *parent) :
    QObject(parent), m_isModified(false),
    m_model(new DocumentModel(this)),
    m_undoStack(new QUndoStack(this))
{
}

QUrl Document::url() const
{
    return QUrl(m_file.fileName());
}

void Document::setUrl(const QUrl &url)
{
    m_file.setFileName(url.toLocalFile());
    emit urlChanged(url);
}

bool Document::open()
{
    if (isOpen())
        close();

    bool ok = m_file.open(QFile::ReadWrite| QFile::Text);
    if (!ok){
        m_errorString = m_file.errorString();
        return false;
    }

    QJsonParseError jsonError;
    m_document.fromJson(m_file.readAll(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        m_errorString = jsonError.errorString();
        return false;
    }

    if (!m_document.isObject()) {
        m_errorString = QString("Document is not a JSON object");
        return false;
    }

    QJsonObject obj = m_document.object();
    if (obj.value("type").toString() != "stupidObject" ||
            !obj.contains("stupidProperty") ||
            obj.value("stupidProperty").isString()) {
        m_errorString = QString("Document doesn't contains a (valid) stupid object!");
        return false;
    }

    DocumentStupidObject *dso = new DocumentStupidObject();
    dso->setStupidProperty(obj.value("stupidProperty").toString());
    m_model->setRootObject(dso);
    return true;
}

bool Document::save()
{
    return true;
}

bool Document::saveAs(const QUrl &url)
{
    setUrl(url);
    m_file.close();
    bool ok = m_file.open(QFile::ReadWrite| QFile::Text);
    if (!ok){
        m_errorString = m_file.errorString();
        return false;
    }
    return save();
}

void Document::close()
{
    if (!isOpen())
        return;
    m_file.close();
    clear();
}

void Document::clear()
{
    setUrl(QUrl());
    m_model->clear();
    m_errorString.clear();
    m_undoStack->clear();
}

bool Document::isModified() const
{
    return m_isModified;
}

bool Document::isOpen() const
{
    return m_file.isOpen();
}

DocumentModel *Document::model()
{
    return m_model;
}

QUndoStack *Document::undoStack()
{
    return m_undoStack;
}

void Document::executeCommand(DocumentCommand *command)
{
    command->setModel(m_model);
    m_undoStack->push(command);
}

QString Document::errorString()
{
    return m_errorString;
}
