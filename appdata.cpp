#include "appdata.h"

int AppData::strHexToDecimal(const QString &strHex)
{
    bool ok;
    return strHex.toInt(&ok, 16);
}

QString AppData::decimalToStrHex(int decimal)
{
    QString temp = QString::number(decimal, 16);
    if (temp.length() == 1) {
        temp = "0" + temp;
    }

    return temp;
}

float AppData::getData(QByteArray data)
{
    data = data.left(data.indexOf('\x03')).right(data.indexOf(",") - 3);
    float temp = strHexToDecimal(data) * 0.1;

    return temp;
}
