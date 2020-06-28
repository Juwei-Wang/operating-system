#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>     // std::cout

typedef std::pair<std::string, int> pair1;

std::vector< pair1 > shaA;

int main () {
    std::pair <std::string,double> product1;                     // default constructor
    std::pair <std::string,double> product2 ("tomatoes",2.30);   // value init
    std::pair <std::string,double> product3 (product2);          // copy constructor

    product1 = std::make_pair(std::string("lightbulbs"),0.99);   // using make_pair (move)

    product2.first = "shoes";                  // the type of first is string
    product2.second = 39.90;                   // the type of second is double

    std::cout << "The price of " << product1.first << " is $" << product1.second << '\n';
    std::cout << "The price of " << product2.first << " is $" << product2.second << '\n';
    std::cout << "The price of " << product3.first << " is $" << product3.second << '\n';



    for (int i = 0; i < 10; ++i) {
        std::string dig = digest( fnames[i]);
        if( dig.empty()) {
            printf("Could not get digest for file %s\n", fnames[i].c_str());
            return -1;
        }
        printf("Digest %i: %s\n", i, dig.c_str());
        std::pair<std::string, int> temp;
        temp = std::make_pair(std::string(dig.c_str()),i);
        shaA.push_back(temp);

    }
    return 0;
}