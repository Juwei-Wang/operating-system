#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cassert>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>
#include <algorithm>

// read line from fp
// returns False on EOF
//         True  on success
// 读取文件中的数据的function
bool get_line( std::string & line, FILE * fp = stdin)
{
    line.clear();
    while(1) {
        int c = fgetc(fp);
        if( c == '\n') return true;
        if( c == EOF) return ! line.empty();
        line.push_back(c);
    }
}


// returns SHA256 digest of the contents of file fname
// on failure returns empty string
// uses popen() to call sha256sum binary
// 得到文件里面数据的　sha256
std::string
digest( const std::string & fname)
{
    std::string cmdline = "sha256sum " + fname + " 2> /dev/null";
    FILE * fp = popen( cmdline.c_str(), "r");
    if( fp == NULL) return "";
    std::string output;
    if( ! get_line(output, fp)) return "";
    if( pclose(fp) != 0) return "";
    std::string result;
    for( auto c : output)
        if( isspace(c)) break ; else result.push_back(c);
    return result;
}


int main()
{
    // read a list of filenames from stdin
    //push_back在尾部加入一个数据
    std::vector<std::string> fnames;

    typedef std::pair<std::string, int> pair1;

    std::vector< pair1 > shaA;

    std::string line;
    while(1) {
        if( ! get_line(line)) break;
        fnames.push_back(line);
    }

    // for debugging purposes print out the filenames
    //打印读取了多少个文件
    printf("Read %lu filenames:\n", fnames.size());

    //打印所有读取文件的文件名
    for( const auto & fname : fnames)
        printf("  '%s'\n", fname.c_str());


    if( fnames.size() < 2) {
        printf("I could have worked if you gave me 2+ filenames... :(\n");
        return -1;
    }


    //计算全部文件的sha dig
    for (int i = 0; i < fnames.size(); ++i) {
        std::string dig = digest( fnames[i]);
        if( dig.empty()) {
            printf("Could not get digest for file %s\n", fnames[i].c_str());
            return -1;
        }
        //printf("Digest %i: %s\n", i, dig.c_str());
        std::pair<std::string, int> temp;
        temp = std::make_pair(std::string(dig.c_str()),i);
        shaA.push_back(temp);

    }


    // compare digests
    sort(shaA.begin(), shaA.end());
    for (int j = 0; j < shaA.size() ; ++j) {
        printf("Digest %i: %s\n", shaA[j].second, shaA[j].first.c_str());
    }

    std::vector<std::vector<int> > matchingA;
    std::vertor<int> eachM;
    int m = shaA.size() - 1;
    for (int k = 0; k < shaA.size(); ++k) {
        if(k == m ){
            if(shaA[k] == shaA[k+1]){
                eachM.push_back(shaA[k].second);
            } else{
                eachM.push_back(shaA[k].second);
                matchingA.push_back(eachM);
                eachM.clear();
            }

        }



    }





    return 0;
}
