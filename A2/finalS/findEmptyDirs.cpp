//
// recursively find and print all directories and files in the
// current directory
// 30053278 juwei wang

#include <stdio.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>

int main()
{
    std::vector<std::string> stack;
    stack.push_back(".");  // every path start from a doc
    long number = 0;
    // recursively check the dir
    while( ! stack.empty()) {
        auto dirname = stack.back();
        stack.pop_back();  //delete the stack  when finishing searching
        number = 0; // count the files found
        // open the next path
        DIR * dir = opendir( dirname.c_str());
        if( dir) {
            while(1) {

                dirent * de = readdir( dir); // read file name
                if( ! de) break;

                std::string name = de-> d_name; // covert file name
                if( name != "." && name != ".."){  //check if there are something except for . and ..
                    number++;
                }
                if( name == "." || name == "..") continue; //if check the . and .., then continue
                std::string path = dirname + "/" + de-> d_name;
                stack.push_back( path);
            }
            //printf("%li\n",number);
            //if there is nothing other than . and .. , then this is a empty dir
            if(number == 0){
                printf("%s\n", dirname.c_str());
            }
            closedir( dir);
        }
    }

    return 0;
}

