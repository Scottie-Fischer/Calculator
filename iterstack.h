#ifndef ITERSTACK_H
#define ITERSTACK_H
#include <vector>
using namespace std;

template <typename value_t, typename container = vector<value_t>>
class iterstack{
    //Adding iterator functions for our better stack class
    public:
        using value_type = value_t;
        using const_iterator = typename container::const_reverse_iterator;
        using size_type = typename container::size_type;
    //Utilizing the stack object
    private:
        container stack;
    //Inline functions for iterstack
    public:
        void clear(){stack.clear();}
        bool empty() const {return stack.empty();}
        size_type size() const{return stack.size();}
        const_iterator begin() {return stack.crbegin();}
        const_iterator end(){return stack.crend();}
        void push(const value_type& val) {stack.push_back(val);}
        void pop(){stack.pop_back();}
        const value_type& top() const{return stack.back();}
};


#endif // ITERSTACK_H
