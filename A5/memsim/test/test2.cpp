#include <iostream>
#include <set>
#include <list>
#include <fstream>
#include <cassert>
#include <unordered_map>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
using namespace std;

typedef std::tuple< long, long , std::string > pair1;

int main(){

    std::vector< pair1 > shaA;
    std::string a = std::string("a") + "b";

    pair1 k = std::make_tuple(1,2,"a");
    pair1 b = std::make_tuple(2,3,a);
    shaA.push_back(k);
    shaA.push_back(b);

    long c = std::get<0>(shaA[0]);
    printf("%i \n", c);
    std::get<0>(shaA[0]) = 8;
    c = std::get<0>(shaA[0]);
    printf("%i \n", c);

    long d = std::get<0>(shaA[1]);
    printf("%i \n", d);
    std::get<0>(shaA[1]) = 6;
    c = std::get<0>(shaA[1]);
    printf("%i \n", c);


    std::string e = std::get<2>(shaA[0]);
    std::string f = std::get<2>(shaA[1]);
    printf("%s \n", e.c_str());
    printf("%s \n", f.c_str());


return 0;
}
