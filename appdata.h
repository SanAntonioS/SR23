#ifndef APPDATA_H
#define APPDATA_H

#include "mainwindow.h"


class AppData
{
public:
    static int     strHexToDecimal(const QString &strHex);
    static float   getData(QByteArray data);
};

#endif // APPDATA_H
