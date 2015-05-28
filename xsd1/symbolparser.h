#ifndef SYMBOLPARSER_H
#define SYMBOLPARSER_H

#include <QString>

class Symbol;

struct SymbolParserPrivate;
class SymbolParser
{
public:
    SymbolParser();

    Symbol *parse(const QString &filename);

    QString errorString() const
    {
        return m_errorString;
    }

private:
    SymbolParserPrivate *p;
    QString m_errorString;
};

#endif // SYMBOLPARSER_H
