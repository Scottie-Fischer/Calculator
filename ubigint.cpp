#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <iostream>
#include <string>
#include <iterator>
#include <cmath>
using namespace std;

//#include "debug.h"
#include "ops.h"
#include "ubigint.h"


//Constructor
ubigint::ubigint (unsigned long that){//: uvalue (that) {
   unsigned long x = that;
   if(x == 0){uvalue.push_back(0);}
   while(x >= 1){
      uvalue.push_back(x%10);
      x/=10;
   }
   //DEBUGF ('~', this << " -> " << uvalue[0])
}
//Constructor
ubigint::ubigint (const string& that){//: uvalue(0) {
   //DEBUGF ('~', "that = \"" << that << "\"");

   reverse_iterator rit = that.crbegin();
   for(rit = that.crbegin(); rit < that.crend(); rit++){
      //cout << "Constructing digit:" << *rit << endl;
      char x = *rit;
      uint8_t dig = x;
      //if(dig != 0){cout << "dig not zero" << static_cast<int>(dig);}
      //uvalue.push_back(dig);
      uvalue.push_back(rit[0]-48);
   }
}

//Operator Fields
ubigint ubigint::operator+ (const ubigint& that) const {
   //DEBUGF ('u', *this << "+" << that);
   ubigint result;
   uint8_t carryout = 0;
   uint8_t sum = 0;

   //This is more digits
   if(uvalue.size() > that.uvalue.size()){
     //cout << "this is gtr" << endl;
     for(uint8_t i = 0; i < uvalue.size(); i++){
         //If we run out of digits
         if(i >= that.uvalue.size()){
           //Match digits from this

           result.uvalue.push_back(uvalue[i] + carryout);
           carryout = 0;
         }
         else{
           //Sum is from current digit
           sum = that.uvalue[i] + uvalue[i] + carryout;


           if(sum >= 10){
              sum -= 10;
              carryout = 1;
           }
           else{carryout = 0;}

           result.uvalue.push_back(sum);
         }
      }
      return result;
   }
   //This is less digits (or equal)
   else{
      //cout << "That is goe" << endl;
      for(uint8_t i = 0; i < that.uvalue.size(); i++){
         if(i >= uvalue.size()){

            result.uvalue.push_back(that.uvalue[i] + carryout);
            carryout = 0;
         }
         else{
            sum = carryout + uvalue[i] + that.uvalue[i];

            if(sum >= 10){
               sum -= 10;
               carryout = 1;
            }
            else{carryout = 0;}

            result.uvalue.push_back(sum);
            if((i == that.uvalue.size()-1) && carryout != 0){

               result.uvalue.push_back(carryout);
            }
         }
      }
      return result;
   }
   //ubigint result (uvalue + that.uvalue);
   //DEBUGF ('u', result);
   return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
   //if (*this < that) throw domain_error ("ubigint::operator-(a<b)");

   ubigint result;
//Iterate threw this bc it is assumed to be larger (handled in bigint)
   uint8_t borrow = 0;
   uint8_t i;
   //cout << "Subtracting: " << *this << " (size " << uvalue.size() << ")"
   //<<"-" << that << "(size " << that.uvalue.size() << ")" << endl;
   //
   int sub;
   for(i = 0; i < uvalue.size(); i++){
      int minu = uvalue[i] - borrow;
      //int sub = that.uvalue[i];
      if(i > that.uvalue.size()-1){sub=0;}
      else{sub = that.uvalue[i];}
      if(minu < sub){
         minu += 10;
         borrow = 1;
      }
      else{borrow = 0;}
      //cout << static_cast<int>(i) << "minu: " << minu << " sub: " << sub << endl;
      uint8_t diff = minu - sub;
      result.uvalue.push_back(diff);
   }
   i--;
   while(result.uvalue[i] == 0 && i > 0){
      result.uvalue.pop_back();
      i--;
   }
   return result;

}

ubigint ubigint::operator* (const ubigint& that) const {
   ubigint result;
   int x = 0;
   int sizeM = uvalue.size();
   int sizeN = that.uvalue.size();
   int carry;
   uint8_t digit;
   while(x < sizeM + sizeN){
      result.uvalue.push_back(0);
      x++;
   }

   for(int i = 0; i < sizeM; i++){
      carry = 0;
      for(int j = 0; j < sizeN; j++){
         digit = result.uvalue[i+j] + (uvalue[i] * that.uvalue[j]) + carry;
         result.uvalue[i+j] = digit % 10;
         carry = digit/10;
         //cout << "i,j " << i << "," << j << " dig:" << static_cast<int>(digit) << " res: " << result << endl;
      }
      result.uvalue[i+sizeN] = carry;
   }
   for(int l = result.uvalue.size()-1; l >= 1; l--){
      if(result.uvalue[l] != 0) break;
      result.uvalue.pop_back();
   }
   return result;
}

void ubigint::multiply_by_2(){
   uint8_t prod = 0;
   uint8_t carry = 0;
   ubigint start(*this);
   for(uint8_t i  = 0;i < uvalue.size(); i++){
      prod = (uvalue[i] * 2 ) + carry ;
      if (prod >= 10){
         carry = 1;
         prod -= 10;
      }else{carry = 0;}
      uvalue[i] = prod;
   }
   if(carry != 0){uvalue.push_back(carry);}

   //cout << start << " now: " << *this << endl;
   //uvalue *= 2;
}

//TODO: COMPLETELY UNFUCK THIS FUNCTION
//THIS IS SOME FUCKING GARBAGE CODE
void ubigint::divide_by_2() {
   ubigint start(*this);

   for(uint8_t i = 0; i < uvalue.size(); i++){
      uvalue[i] /= 2;
      if(i < uvalue.size()-1  && uvalue[i+1] %2 == 1){
         uvalue[i] += 5;
      }
   }
   for(int i = uvalue.size()-1; i > 0; i--){
      if(uvalue[i] == 0){
          //cout<<"popping"<<endl;
          uvalue.pop_back();
      }else{break;}
   }
   //cout << start << " -> " << *this << endl;
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, const ubigint& divisor_) {
   // NOTE: udivide is a non-member function.

   ubigint divisor {divisor_};
   ubigint zero {0};
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {1};
   ubigint quotient {0};
   ubigint remainder {dividend}; // left operand, dividend

   //cout << "po2: " << power_of_2 << " quotient " << quotient
   //<< " remainder " << remainder << " div: " << divisor <<  endl;
   //int j = 0;
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   //cout << "po2: " << power_of_2 << " quotient " << quotient
   //<< " remainder " << remainder << " div: " << divisor <<  endl;
   while (power_of_2 > zero) {
      //cout << " /checking " << divisor << " <= " << remainder << endl;
      if (divisor < remainder || divisor == remainder) {
         //cout << "div " << divisor << " <= rem" << remainder << endl;
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   //cout << "after 2nd loop" << endl;
   //DEBUGF ('/', "quotient = " << quotient);
   //DEBUGF ('/', "remainder = " << remainder);
   return {.quotient = quotient, .remainder = remainder};

   //ubigint quotient;
   //ubigint remainder;
   //return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {

   if(uvalue.size() != that.uvalue.size()){

      return false;
   }
   for(uint8_t i = 0; i < uvalue.size(); i++){
      if(uvalue[i] != that.uvalue[i]){
         //cout << "not equal: idx " << static_cast<int>(i) << endl;
         return false;
      }
   }
   //cout << "equal" <<endl;
   return true;
}

bool ubigint::operator< (const ubigint& that) const {
   //cout << " <checking " << *this << " < " << that << " ";
   //that < this
   if(uvalue.size() < that.uvalue.size()){
      //cout << " yes this.size() < that.size()" <<endl;
      return true;
   }
   if(uvalue.size() > that.uvalue.size()){
      //cout << " no this.size() > that.size()" <<endl;
      return false;
   }
   if(*this == that){
      //cout << " no equal values" << endl;
      return false;
   }
   //Equal Size UnEqual Value
   //Iterator Inversely
   for(int i = that.uvalue.size()-1; i >= 0; i--){
      //cout << i << "-> " << static_cast<int>(that.uvalue[i]) << " : " << static_cast<int>(uvalue[i]) << endl;
      if(that.uvalue[i] > uvalue[i]){
         //cout << "no indx " << i << endl;
         return true;
      }
      else if(that.uvalue[i] < uvalue[i]){
         return false;
      }
   }
   //cout << "true eol" << endl;
   return true;

   //return uvalue < that.uvalue;
}

void ubigint::print() const {
   //DEBUGF ('p', this << " -> " << *this);
   for(int i = uvalue.size()-1; i >= 0; i--){
      cout << static_cast<int>(uvalue[i]);
   }
   //for(char c: uvalue){
   //   cout << static_cast<int>(c);
   //}
   //cout << uvalue;
}

ostream& operator<< (ostream& out, const ubigint& that) {
   //out << "ubigint(";
   for(int i = that.uvalue.size()-1; i >= 0; i--){
      cout << static_cast<int>(that.uvalue[i]);
   }
   //cout << ")";
   return out;
}
