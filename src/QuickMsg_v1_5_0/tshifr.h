#ifndef TSHIFR_H
#define TSHIFR_H

#include <Qstring>

class Tshifr
{
private:
    QString key;

public:

    Tshifr(){}
    Tshifr( QString k);

    void setKey(QString k){ key=k;}

    QString getKey(){ return key;}

    virtual QString Encode(QString text) = 0;
    virtual QString Decode(QString text) = 0;

};

class none: public Tshifr
{
public:
    none(){}
    none( QString k):Tshifr(k){}
    QString Encode(QString text){return text;}
    QString Decode(QString text){return text;}
};

class Symbol_to_Number: public Tshifr
{
public:
    Symbol_to_Number(){}
    Symbol_to_Number( QString k);
    QString Encode(QString text);
    QString Decode(QString text);
};


#endif // TSHIFR_H
