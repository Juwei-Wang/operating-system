//
// recursively find and print all directories and files in the
// current directory
//

#include <stdio.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>

int main()
{
    std::vector<std::string> stack;
    stack.push_back(".");
    long number = 0;
    while( ! stack.empty()) {
        auto dirname = stack.back();
        stack.pop_back();
        number = 0;
        DIR * dir = opendir( dirname.c_str());
        if( dir) {
            while(1) {

                dirent * de = readdir( dir);
                if( ! de) break;

                std::string name = de-> d_name;
                if( name != "." && name != ".."){
                    number++;
                }
                if( name == "." || name == "..") continue;
                std::string path = dirname + "/" + de-> d_name;
                stack.push_back( path);
            }
            //printf("%li\n",number);
            if(number == 0){
                printf("%s\n", dirname.c_str());
            }
            closedir( dir);
        }
    }

    return 0;
}

