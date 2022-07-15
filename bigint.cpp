#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <iostream>
#include <QDebug>
using namespace std;

#include <bigint.h>

//Ctors
bigint::bigint(long that):uvalue(that),is_negative(that < 0){}

bigint::bigint(const ubigint& uvalue_,bool is_negative_):
               uvalue(uvalue_),is_negative(is_negative_){}

bigint::bigint(const string& that){
    is_negative = that.size() > 0 and that[0] == '_';
    uvalue = ubigint (that.substr (is_negative ? 1:0));
}

//Operators
bigint bigint::operator+()const{
    return *this;
}

bigint bigint::operator-()const{
    return {uvalue, not is_negative};
}

bigint bigint::operator+(const bigint& that)const{
    //The signs are the same
    if(is_negative == that.is_negative){
        ubigint result {uvalue + that.uvalue};
        bigint res(result,is_negative);
        return res;
    }
    //The signs are different
    else{
        //This is smaller
        if(uvalue < that.uvalue){
            ubigint result(that.uvalue - uvalue);
            bigint res(result,that.is_negative);
            return res;
        }
        //This is bigger
        if(uvalue == that.uvalue){
            ubigint result(uvalue - that.uvalue);
            bigint res(result,false);
            return res;
        }
        else{
            ubigint result(uvalue - that.uvalue);
            bigint res(result,is_negative);
            return res;
        }
    }
}

bigint bigint::operator-(const bigint& that)const{
    if(is_negative == that.is_negative){
        if(that.uvalue < uvalue){
            ubigint sub{uvalue-that.uvalue};
            bigint res(sub,is_negative);
            return res;
        }
        else{
            ubigint sub{that.uvalue-uvalue};
            bigint res(sub,!that.is_negative);
            if(sub==0){res.is_negative=false;}
            return res;
        }
    }else{
        ubigint sub{uvalue + that.uvalue};
        bigint res(sub,is_negative);
        return res;
    }
}
bigint bigint::operator*(const bigint& that)const{
    bigint res {uvalue * that.uvalue};
    if(is_negative != that.is_negative){
        res.is_negative = true;
    }
    if(uvalue == 0 || that.uvalue == 0){
        res.is_negative = false;
    }
    return res;
}

bigint bigint::operator/(const bigint& that)const{
    bigint res {uvalue / that.uvalue};
    if(is_negative != that.is_negative){
        res.is_negative = true;
    }
    return res;
}
bigint bigint::operator%(const bigint& that)const{
    bigint res{uvalue % that.uvalue};
    return res;
}

bool bigint::operator==(const bigint& that)const{
    return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator<(const bigint& that)const{
    if(is_negative != that.is_negative)return is_negative;
    return is_negative ? uvalue > that.uvalue
                       : uvalue < that.uvalue;
}

void bigint::print()const{
    if(is_negative)cout << "-";
    uvalue.print();
}

//This is to handle the QDebug output
QDebug operator<<(QDebug debug,const bigint& that){
    debug.nospace() << "bigint(" << (that.is_negative ? "-": "+")
                           << "," << that.uvalue << ")";
    return debug;
}

ostream& operator<<(ostream& out,const bigint& that){
    return out << "bigint(" << (that.is_negative ? "-": "+")
               << "," << that.uvalue << ")";
}

