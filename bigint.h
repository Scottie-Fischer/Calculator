#ifndef BIGINT_H
#define BIGINT_H
#include <exception>
#include <limits>
#include <utility>
#include <iostream>
#include <QDebug>
using namespace std;
#include "ops.h"
#include "ubigint.h"

class bigint{
    friend ostream& operator<< (ostream&, const bigint&);
    friend QDebug operator<<(QDebug,const bigint&);
    private:
        ubigint uvalue {};
        bool is_negative{false};
    public:
        //Ctors
        bigint() = default;
        bigint(long);
        bigint(const ubigint&, bool is_negative = false);
        explicit bigint (const string&);

        //Operators
        bigint operator+()const;
        bigint operator-()const;

        bigint operator+(const bigint&)const;
        bigint operator-(const bigint&)const;
        bigint operator*(const bigint&)const;
        bigint operator/(const bigint&)const;
        bigint operator%(const bigint&)const;

        bool operator==(const bigint&)const;
        bool operator<(const bigint&)const;


        void print() const;

};

#endif // BIGINT_H
