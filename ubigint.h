#ifndef UBIGINT_H
#define UBIGINT_H

#include <exception>
#include <limits>
#include <utility>
#include <vector>
#include <cstdint>
#include <iostream>
using namespace std;

class ubigint{
    friend ostream& operator <<(ostream&, const ubigint&);

    private:
        using ubigvalue_t = vector<uint8_t>;
        ubigvalue_t uvalue{};
    public:
        void multiply_by_2();
        void divide_by_2();

        ubigint() = default;
        ubigint(unsigned long);
        ubigint(const string&);

        ubigint operator+(const ubigint&)const;
        ubigint operator-(const ubigint&)const;
        ubigint operator*(const ubigint&)const;
        ubigint operator/(const ubigint&)const;
        ubigint operator%(const ubigint&)const;

        bool operator==(const ubigint&)const;
        bool operator<(const ubigint&)const;

        void print() const;

};

#endif // UBIGINT_H
