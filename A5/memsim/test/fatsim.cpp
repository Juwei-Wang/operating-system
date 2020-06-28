#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <queue>

/*
int main() {
    std::vector<int> pair;
    std::map<int, std::vector<int>> colours;

    pair.push_back(5);
    colours.insert(std::pair<int,std::vector<int>>(5,pair));

    std::vector<int> pair1;

    pair1 = colours[5];
    std::cout << pair1[0] << std::endl;

}*/


void
fatsim( const std::vector<long> fat, long & longest_file_blocks, long & unused_blocks){

    // reimplement this
  std::map<long, std::vector<long>> adj_list;
    for (long i = 0; i < fat.size(); ++i) {
        std::vector<long> list;
        std::vector<long> list1;
        adj_list[i];
        list = adj_list[fat[i]];
        if(list.size() == 0){
            list1.push_back(i);
            //adj_list.insert(std::pair<long,std::vector<long>>(fat[i],list1));
            adj_list[fat[i]] = list1;
        }else{
            list1 = adj_list[fat[i]];
            list1.push_back(i);
            adj_list[fat[i]] = list1;
        }

    }

    std::map<long, std::vector<long>> adj_list1;
    adj_list1 = adj_list;
    std::queue<long> q;
    q.push(-1);
    long node;
    long len = 0;
    while (!q.empty()){
        long size = q.size();
        for (long i=0; i< size; ++i) {
            node=q.front();
            q.pop();
            adj_list1.erase(node);
            //std::cout << "erase node"<< node << std::endl;
            for (long x : adj_list[node]) {// check neighbors of node
                //std::cout << x << "/";
                q.push(x);
            }
            //std::cout << "\n" << std::endl;
        }
        ++len;
        //std::cout << "length = "<<len << std::endl;
        //std::cout << "------------------------\n" << std::endl;
    }

    len --;



/*    for (auto it : adj_list ){
        std::cout << it.first << std::endl;
        for (int i = 0; i < it.second.size(); ++i) {
            std::cout << it.second[i] << " " << std::endl;
        }
        std::cout << "--------------------------\n";
    }*/




  longest_file_blocks = len;
  unused_blocks = adj_list1.size();
  //std::cout << "---adj_list_size-"<< adj_list.size() << std::endl;
}

int
main( int argc, char **)
{
  if( argc != 1) {
    printf("Thanks for all the command line arguments.\n"
           "But I don't like any of them... Bye.\n");
    exit(-1);
  }

  std::vector<long> fat;
  long maxnum = 0;
  while ( 1) {
    long num;
    if( 1 != scanf("%ld", & num)) break;
    if( num < -1) {
      printf("I don't like your FAT, it's too negative.\n");
      exit(-1);
    }
    maxnum = std::max( maxnum, num);
    fat.push_back(num);
  }

  if( long(fat.size()) <= maxnum) {
    printf("Some of your FATs are too big.\n");
    exit(-1);
  }

  long count1, count2;
  fatsim( fat, count1, count2);
  printf("blocks in largest file: %ld\n", count1);
  printf("blocks not in any file: %ld\n", count2);

  return 0;
}
