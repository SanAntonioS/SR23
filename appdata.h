#ifndef APPDATA_H
#define APPDATA_H

#include "mainwindow.h"


class AppData
{
public:
    static int     strHexToDecimal(const QString &strHex);
    static QString decimalToStrHex(int decimal);
    static int   getTempData(QByteArray data);
    static int   getPowerData(QByteArray data);
    static int   getPIDData(QByteArray data);
};

#endif // APPDATA_H
