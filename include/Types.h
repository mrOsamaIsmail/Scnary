#ifndef TYPES_H
#define TYPES_H


#include <string>
#include <list>
#include <map>
#include <array>

#define prntln(x) std::cout<< x <<"\n" 

template <typename type,typename key>
using dictionary = std::map<key,type>;

template <typename type,std::size_t count>
using Array = std::array<type, count>;

using string = std::string;

template <typename T>
using list  = std::list<T>; 

using uint = unsigned int;

#endif