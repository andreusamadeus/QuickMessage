#include "tshifr.h"
#include <QChar>

Tshifr::Tshifr(QString k)
{
    key=k;
}

Symbol_to_Number::Symbol_to_Number(QString k):Tshifr(k)
{}

QString Symbol_to_Number::Decode(QString text)
{
    QString rtext=text;
    QString rkey=getKey();

    QString ctext="";
    for (int i=0;i<rtext.size()/4;i++)
    {
        ctext+= QChar(rtext.mid(i*4+1,4).toInt()-int(rkey[i%rkey.size()].unicode())-1000);
    }

    return ctext;
}

QString Symbol_to_Number::Encode(QString text)
{
    QString rtext=text;
    QString rkey=getKey();

    QString ctext="";
    for (int i=0;i<rtext.size();i++)
    {
        ctext+=QString::number(rtext[i].unicode()+rkey[i%rkey.size()].unicode()+1000);
    }
    return ctext;
}
