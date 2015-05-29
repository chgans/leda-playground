#ifndef SYMBOLPARSER_H
#define SYMBOLPARSER_H

#include <QString>

class Symbol;

struct SymbolReaderPrivate;
class SymbolReader
{
public:
    SymbolReader();

    Symbol *parse(const QString &filename);

    QString errorString() const
    {
        return m_errorString;
    }

private:
    SymbolReaderPrivate *p;
    QString m_errorString;
};

#endif // SYMBOLPARSER_H
