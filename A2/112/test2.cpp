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
    //printf("我是112\n");
    std::vector<std::string> fnames;

    typedef std::pair<std::string, std::string> pair1;

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
        std::pair<std::string, std::string> temp;
        temp = std::make_pair(std::string(dig.c_str()),std::string(fnames[i].c_str()));
        shaA.push_back(temp);

    }


    // compare digests
    sort(shaA.begin(), shaA.end());
    // for (int j = 0; j < shaA.size() ; ++j) {
    //     printf("Digest %s: %s\n", shaA[j].second.c_str(), shaA[j].first.c_str());
    //}

    std::vector<std::vector<std::string> > matchingA;
    std::vector<std::string> eachM;
    int m = shaA.size() - 1;
    for (int k = 0; k < m; ++k) {
        //printf("I get into this loop\n");
        //printf("%s\n",shaA[k].first.c_str());
        //printf("%s\n",shaA[k+1].first.c_str());
        if(k < m ){
            if(shaA[k].first  == shaA[k+1].first){
                if (k == (m-1)){
                  //printf("I get into this loop 1a\n");
                  eachM.push_back(shaA[k].second);
                  eachM.push_back(shaA[k+1].second);
                  matchingA.push_back(eachM);
                  eachM.clear();
                }else{
                  //printf("I get into this loop 1b\n");
                  //printf("Digest %s\n", shaA[k].first.c_str());
                  //printf("Digest %s\n", shaA[k+1].first.c_str());
                  eachM.push_back(shaA[k].second);
                }
            } else{
                if (k ==  (m-1)){
                  //printf("I get into this loop 2a\n");
                  eachM.push_back(shaA[k].second);
                  matchingA.push_back(eachM);
                  eachM.clear();
                  eachM.push_back(shaA[k+1].second);
                  matchingA.push_back(eachM);
                  eachM.clear();
                }else{
                  //printf("I get into this loop 2b\n");
                  eachM.push_back(shaA[k].second);
                  matchingA.push_back(eachM);
                  eachM.clear();
                }
            }

        }
    }
   //printf("%i\n",matchingA.size());
   //std::cout << matchingA[0][0] << std::endl;
   //std::cout << matchingA[0][1] << std::endl;
   //std::cout << matchingA[0][2] << std::endl;
   //std::cout << matchingA[0].size() << std::endl
   printf("\n");
   printf("Found %i pairs of matching or just only file\n",matchingA.size());
   for (int l = 0; l < matchingA.size() ; ++l) {
       if(matchingA[l].size() > 1){
         printf("matching %i\n",l);
       }else{
         printf("the only file %i\n",l);
       }
       for (int z = 0; z < matchingA[l].size(); ++z)
         {
            std::cout << matchingA[l][z] << std::endl;
        }
       printf("\n");
    }


    return 0;
}
