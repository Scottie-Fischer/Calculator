#ifndef OPS_H
#define OPS_H

/*
 * This is for the template of basic operators
 *
 *
*/

#ifdef INCLUDE_TEMPLATE_OPERATOR_NE
template <typename val>
inline bool operator!=(const val& left, const val* right){
    return not(left == right);
}
#endif

template <typename val>
inline bool operator==(const val& left, const val& right){
    return not (left==right);
}

template <typename val>
inline bool operator> (const val& left, const val& right){
    return right < left;
}

template <typename val>
inline bool operator<= (const val& left, const val& right){
    return not (right < left);
}

template <typename val>
inline bool operator>= (const val& left, const val& right){
    return not(left < right);
}
#endif // OPS_H
