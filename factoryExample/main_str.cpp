#include "StrVec.hpp"

int main(){


    StrVec sv;

    sv.push_back("Hello");
    sv.push_back("World");
    sv.push_back("!");
    for(auto p = sv.begin(); p != sv.end(); ++p){
        std::cout << *p << " ";
    }
    std::cout << std::endl;
    return 0;
}