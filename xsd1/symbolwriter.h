#ifndef SYMBOLWRITER_H
#define SYMBOLWRITER_H

#include <QString>

class Symbol;

class SymbolWriterPrivate;
class SymbolWriter
{
public:
    SymbolWriter();

    bool write(const QString &filename, const Symbol *symbol);

    QString errorString() const
    {
        return m_errorString;
    }

private:
    SymbolWriterPrivate *p;
    QString m_errorString;
};

#endif // SYMBOLWRITER_H
