#pragma once
#ifndef PARSEUTIL_H
#define PARSEUTIL_H

#include "heallocation.h"
#include "log.h"

#include <QString>
#include <QList>
#include <QMap>
#include <QRegularExpression>

enum TokenClass {
    Number,
    Operator,
    Error,
};

class Token {
public:
    Token(QString value = "", QString type = "") {
        this->value = value;
        this->type = TokenClass::Operator;
        if (type == "decimal" || type == "hex") {
            this->type = TokenClass::Number;
            this->operatorPrecedence = -1;
        } else if (type == "operator") {
            this->operatorPrecedence = precedenceMap[value];
        } else if (type == "error") {
            this->type = TokenClass::Error;
        }
    }
    static QMap<QString, int> precedenceMap;
    QString value;
    TokenClass type;
    int operatorPrecedence; // only relevant for operator tokens
};

class ParseUtil {
public:
    ParseUtil(){};
    void set_root(const QString& dir);
    static QString readTextFile(const QString& path);
    static int textFileLineCount(const QString& path);
    QList<QStringList> parseAsm(const QString& filename);
    int evaluateDefine(const QString&, const QMap<QString, int>&);
    QStringList readCArray(const QString& text, const QString& label);
    QMap<QString, QString> readNamedIndexCArray(const QString& text, const QString& label);
    QString readCIncbin(const QString& text, const QString& label);
    QMap<QString, int> readCDefines(const QString& filename, const QStringList& prefixes, QMap<QString, int> = {});
    QStringList readCDefinesSorted(const QString&, const QStringList&, const QMap<QString, int>& = {});
    QList<QStringList> getLabelMacros(const QList<QStringList>&, const QString&);
    QStringList getLabelValues(const QList<QStringList>&, const QString&);
    bool tryParseJsonFile(QJsonDocument* out, const QString& filepath);
    bool ensureFieldsExist(const QJsonObject& obj, const QList<QString>& fields);

    // Returns the 1-indexed line number for the definition of scriptLabel in the scripts file at filePath.
    // Returns 0 if a definition for scriptLabel cannot be found.
    static int getScriptLineNumber(const QString& filePath, const QString& scriptLabel);
    static int getRawScriptLineNumber(QString text, const QString& scriptLabel);
    static int getPoryScriptLineNumber(QString text, const QString& scriptLabel);
    static QString removeStringLiterals(QString text);
    static QString removeLineComments(QString text, const QString& commentSymbol);
    static QString removeLineComments(QString text, const QStringList& commentSymbols);

    static QStringList splitShellCommand(QStringView command);

private:
    QString root;
    QString text;
    QString file;
    QList<Token> tokenizeExpression(QString expression, const QMap<QString, int>& knownIdentifiers);
    QList<Token> generatePostfix(const QList<Token>& tokens);
    int evaluatePostfix(const QList<Token>& postfix);
    void error(const QString& message, const QString& expression);
};

#endif // PARSEUTIL_H
