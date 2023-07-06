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

int AppData::getTempData(QByteArray data)
{
    data = data.left(data.indexOf('\x03')).right(data.indexOf(",") - 2);
    int temp = strHexToDecimal(data);

    return temp;
}

int AppData::getPowerData(QByteArray data)
{
    data = data.left(data.indexOf('\x03')).right(data.indexOf(",") - 3);
    int temp = strHexToDecimal(data);

    return temp;
}

int AppData::getPIDData(QByteArray data)
{
    data = data.left(data.indexOf('\x03')).right(data.indexOf(",") - 3);

    int temp = strHexToDecimal(data);

    if (temp > 32767) {
        qDebug() << data;
        temp = temp - 0x01;
        temp = ~int16_t(temp);

        temp = - temp;
    }

    return temp;
}
