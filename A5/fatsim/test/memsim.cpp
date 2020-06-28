#include <stdio.h>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

typedef std::vector<std::string> vs_t;
typedef std::vector<std::vector<long>> type1;

// split string p_line into a vector of strings (words)
// the delimiters are 1 or more whitespaces
vs_t
split( const std::string & p_line)
{
  auto line = p_line + " ";
  vs_t res;
  bool in_str = false;
  std::string curr_word = "";
  for( auto c : line) {
    if( isspace(c)) {
      if( in_str)
        res.push_back(curr_word);
      in_str = false;
      curr_word = "";
    }
    else {
      curr_word.push_back(c);
      in_str = true;
    }
  }
  return res;
}

// convert string to long
// if successful, success = True, otherwise success = False
long
str2long( const std::string & s, bool & success)
{
  char * end = 0;
  errno = 0;
  long res = strtol( s.c_str(), & end, 10);
  if( * end != 0 || errno != 0) {
    success = false;
    return -1;
  }
  success = true;
  return res;
}

// read in a line from stdin and parse it
// parsed results are passed back via parameter references
// returns true on EOF, otherwise false
// aborts program on bad input
bool
parse_line( long & n1, long & n2)
{
  char buff[4096];
  vs_t words;
  static long line_no = 0;

  // read in a line and parse it into words
  while(1) { // loop to skip empty lines
    line_no ++;
    if( ! fgets( buff, sizeof(buff), stdin))
      return true; // eof
    words = split(buff);
    if( words.size() != 0) break; // continue if line not empty
  }

  if( words.size() > 2) {
    printf("Error on line %ld - too many words\n", line_no);
    exit(-1);
  }

  // convert first word into number
  bool ok;
  n1 = str2long( words[0], ok);
  if( ! ok) {
    printf("Error on line %ld - not an integer\n", line_no);
    exit(-1);
  }

  if( n1 < 0) {
    if( words.size() != 1) {
      printf("Error on line %ld - too many numbers\n", line_no);
      exit(-1);
    }
  }
  else {
    // convert 2nd number
    n2 = str2long( words[1].c_str(), ok);
    if( ! ok || n2 < 1) {
      printf("Error on line %ld - 2nd paramter not positive integer\n",
             line_no);
      exit(-1);
    }
  }
  return false;
};

// all you need to do is to fill in the implementation of MemSim class
struct MemSim {
  // initializes the memory simulator with a page size
  type1 current;
  long page_size_k = 0;

  MemSim( long page_size) {
      std::vector<long> temp;
      page_size_k = page_size;
      temp.push_back(-1);
      temp.push_back(page_size);
      current.push_back(temp);
      printf("kk = %ld.\n", current[0][0]);
      printf("kk = %ld.\n", current[0][1]);
  }

  // frees all blocks with the given tag and merges adjecent free blocks
  void free( long tag) {
      printf("free loop \n");
      printf("tag = %ld \n",tag);
      long true_index = 0;

      for(long k = 0; k < current.size(); ++k){

          printf("free large loop \n");
          printf("current %ld, current current size %ld \n",k,current.size()); 

          if(current[true_index][0] == tag){
              printf("free match loop \n");
              if(true_index == 0){
                  printf("free 0 loop \n");
                  //check right
                  if(current.size() >= 2){
                      if(current[1][0] < 0){
                          printf("free 0 loop 1 \n");
                          std::vector<std::vector<long>>::iterator it1;
                          it1 = current.begin();
                          long temp_size = current[1][1];
                          long temp_size1 = current[0][1];
                          current.erase(it1);
                          current.erase(it1);
                          std::vector<long> temp_v;
                          temp_v.push_back(-1);
                          temp_v.push_back(temp_size + temp_size1);
                          current.insert(it1,temp_v);
                          //k++;

                          for (long k = 0; k < current.size(); ++k){
                              for (long i = 0; i < current[0].size(); ++i) {
                                  printf("%ld ", current[k][i]);
                              }
                              printf("\n");
                          }

                      } else{
                          printf("free 0 loop 2 \n");
                          current[0][0] = -1;
                      }
                  } else{
                      printf("free 0 loop 3 \n");
                      current[0][0] = -1;
                  }
              }

              if(true_index == (current.size() - 1)){
                  printf("free size() loop \n");
                  //check left

                  if(current.size() >= 2){
                      if(current[current.size() - 2][0] < 0){
                          printf("free size() loop 1\n");
                          std::vector<std::vector<long>>::iterator it1;
                          it1 = current.end() - 1;
                          long temp_size = current[current.size() - 1][1];
                          long temp_size1 = current[current.size() - 2][1];
                          current.erase(it1);
                          current.erase(it1);
                          std::vector<long> temp_v;
                          temp_v.push_back(-1);
                          temp_v.push_back(temp_size + temp_size1);
                          current.insert(it1,temp_v);
                          //k++;

                          for (long k = 0; k < current.size(); ++k){
                              for (long i = 0; i < current[0].size(); ++i) {
                                  printf("%ld ", current[k][i]);
                              }
                              printf("\n");
                          }

                      } else{
                          printf("free size() loop 2\n");
                          current[current.size() - 1][0] = -1;
                      }
                  } else{
                      printf("free size() loop 3\n");
                      current[current.size() - 1][0] = -1;
                  }
              }

              if((true_index < (current.size() - 1)) && (true_index > 0)){
                  printf("free between loop \n");
                  bool merge_left = false;
                  // check left
                  if(current[true_index - 1][0] < 0){
                      printf("free between loop 1\n");
                      std::vector<std::vector<long>>::iterator it1;
                      it1 = current.begin() + true_index - 1;
                      long temp_size = current[true_index][1];
                      long temp_size1 = current[true_index - 1][1];
                      current.erase(it1);
                      current.erase(it1);
                      std::vector<long> temp_v;
                      temp_v.push_back(-1);
                      temp_v.push_back(temp_size + temp_size1);
                      current.insert(it1,temp_v);
                      merge_left = true;
                      //k++;
                      for (long k = 0; k < current.size(); ++k){
                          for (long i = 0; i < current[0].size(); ++i) {
                              printf("%ld ", current[k][i]);
                          }
                          printf("-----------------\n");
                      }

                  }

                  if(merge_left){
                      if(current[true_index][0] < 0){
                          printf("free between loop 2\n");
                          std::vector<std::vector<long>>::iterator it1;
                          it1 = current.begin() + true_index - 1;
                          long temp_size = current[true_index][1];
                          long temp_size1 = current[true_index - 1][1];
                          current.erase(it1);
                          current.erase(it1);
                          std::vector<long> temp_v;
                          temp_v.push_back(-1);
                          temp_v.push_back(temp_size + temp_size1);
                          current.insert(it1,temp_v);
                          true_index--;
                          for (long k = 0; k < current.size(); ++k){
                              for (long i = 0; i < current[0].size(); ++i) {
                                  printf("%ld ", current[k][i]);
                              }
                              printf("---------------------\n");
                          }
                      }
                  } else{
                      if(current[true_index+1] [0] < 0){
                          printf("free between loop 3\n");
                          std::vector<std::vector<long>>::iterator it1;
                          it1 = current.begin() + true_index;
                          long temp_size = current[true_index][1];
                          long temp_size1 = current[true_index + 1][1];
                          current.erase(it1);
                          current.erase(it1);
                          std::vector<long> temp_v;
                          temp_v.push_back(-1);
                          temp_v.push_back(temp_size + temp_size1);
                          current.insert(it1,temp_v);
                          //k++;
                          for (long k = 0; k < current.size(); ++k){
                              for (long i = 0; i < current[0].size(); ++i) {
                                  printf("%ld ", current[k][i]);
                              }
                              printf("----------------\n");
                          }
                      } else{
                          printf("free between loop 4\n");
                          current[true_index][0] = -1;

                          for (long k = 0; k < current.size(); ++k){
                              for (long i = 0; i < current[0].size(); ++i) {
                                  printf("%ld ", current[k][i]);
                              }
                              printf("---------------------\n");
                          }
                      }
                  }

              }
          }
          true_index ++;

      }
      for (long k = 0; k < current.size(); ++k){
          for (long i = 0; i < current[0].size(); ++i) {
              printf("%ld ", current[k][i]);
          }
          printf("\n");
      }
      printf("\n");
      printf("\n");

    // implement this
  }
  // allocates a chunk using best-fit
  // if no suitable chunk found, requests new page(s)
  void alloc( long tag, long size) {
      printf("current tag%ld \n",tag);
      printf("current size%ld \n",size);
      // implement this
      long index = 0;
      long true_index = 0;
      long temp_size = 0;
      bool first_found = true;
      bool found = false;
      // Iterate over all elements in Vector
      for (auto & elem : current)
      {
          if (elem[0] == -1 && elem[1] >= size)
          {
              found = true;
              if(first_found){
                  printf("loop 1 \n");
                  temp_size = elem[1];   // 20 40 25    p:21
                  true_index = index;
                  first_found = false;
              }
              if(elem[1] < temp_size){
                  printf("loop 2 \n");
                  temp_size = elem[1];
                  true_index = index;
              }
              std::cout << true_index << std::endl;
              //break;
          }
          index ++ ;
      }

      if(found){
          //如果找到了 就判断是size等于还是大于
          //大于就生出多个chunk
          //等于就直接放进size里面
          if(current[true_index][1] > size){
              printf("loop found 1 \n");
              std::vector<std::vector<long>>::iterator it1;
              std::vector<long> temp_v;
              it1 = current.begin() + true_index;
              long pre_size = current[true_index][1];
              current.erase(it1);
              temp_v.push_back(-1);
              temp_v.push_back(pre_size - size);
              current.insert(it1, temp_v);
              temp_v.clear();
              temp_v.push_back(tag);
              temp_v.push_back(size);
              current.insert(it1, temp_v);
          } else{
              printf("loop found 2 \n");
              std::vector<std::vector<long>>::iterator it1;
              std::vector<long> temp_v;
              it1 = current.begin() + true_index;
              current.erase(it1);
              temp_v.push_back(tag);
              temp_v.push_back(size);
              current.insert(it1, temp_v);
          }
      } else{
          // 没有找到 看最后一个block是不是空的
          if(current[current.size() - 1][0] > 0){
               printf("loop not found 1 \n");
               long size_m = (size / page_size_k) + 1;
               long add_size = page_size_k * size_m;
               std::vector<long> temp_v;
               temp_v.push_back(tag);
               temp_v.push_back(size);
               current.push_back(temp_v);
               temp_v.clear();
               temp_v.push_back(-1);
               temp_v.push_back(add_size - size);
               current.push_back(temp_v);
          }else{
              printf("loop not found 2 \n");
              printf("size %ld", size);
              long cha_zhi = size - current[current.size() - 1][1];
              printf("cha_zhi %ld \n", cha_zhi);
              long size_m = (cha_zhi / page_size_k) + 1;
              printf("size %ld \n", size);
              long add_size = page_size_k * size_m;
              printf("%ld add_size \n", add_size);
              long total = add_size + current[current.size() - 1][1];
              printf("%ld total \n", total);
              std::vector<std::vector<long>>::iterator it1;
              it1 = current.end() - 1;
              current.erase(it1);
              std::vector<long> temp_v;
              temp_v.push_back(tag);
              temp_v.push_back(size);
              current.push_back(temp_v);
              temp_v.clear();
              temp_v.push_back(-1);
              temp_v.push_back(total - size);
              current.push_back(temp_v);
          }
      }

      for (long k = 0; k < current.size(); ++k){
         for (long i = 0; i < current[0].size(); ++i) {
              printf("%ld ", current[k][i]);
         }
         printf("\n");
      }
     printf("\n");
     printf("\n");
  }
  // returns statistics about the simulation
  // - number of total page requests
  // - the largest available chunk
  void get_report( long & n_req, long & largest) {
    // implement this
    n_req = 0;
    largest = 0;
  }
 private:
};

int
main( int argc, char ** argv)
{
  // parse command line arguments
  // ------------------------------
  long page_size = 1000;
  if( argc != 2) {
    printf("Assuming page size = %ld.\n", page_size);
  }
  else {
    bool ok;
    page_size = str2long( argv[1], ok);
    if( !ok || page_size < 1) {
      printf("Bad page size '%s'.\n", argv[1]);
      exit(-1);
    }
  }

  // process every line
  // ------------------------------
  MemSim ms( page_size);
  while(true) {
    long n1, n2;
    if( parse_line( n1, n2)) break;
    if( n1 < 0)
      ms.free( -n1);
    else
      ms.alloc( n1, n2);
  }

  // report results
  // ------------------------------
  long n_page_requests, largest_free_chunk_size;
  ms.get_report( n_page_requests, largest_free_chunk_size);
  printf("pages requested: %ld\n", n_page_requests);
  printf("largest free chunk: %ld\n", largest_free_chunk_size);

  return 0;
}

