#include <stdio.h>
#include <string>
#include <vector>
#include <cassert>

typedef std::vector<std::string> vs_t;
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

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

struct node{
  long tag;
  long start;
  long length;
  struct node *next;
  node(long Tag, long Start, long Length, struct node *node){ 
    tag = Tag;
    start = Start;
    length = Length;
    next = node;
  }

};

// all you need to do is to fill in the implementation of MemSim class
struct MemSim {
  long pages; 
  long PAGESIZE;
  node *head;
  node *tail;


  // initializes the memory simulator with a page size
  MemSim( long page_size) {
    pages = 0;
    PAGESIZE = page_size;
    head = nullptr;
    tail = nullptr;
  }


  // frees all blocks with the given tag and merges adjecent free blocks
  void free( long Tag) {
    if (head == nullptr) return;
    node *current = head;
    node *previous = nullptr;
    //For every chunk in the list
    while (current != nullptr){
      //If it has a matching tag, it is occupied, so free it up
      if (current -> tag == Tag){
        current -> tag = -1;

        if(previous != nullptr){
          if (previous -> tag == -1){
            //Merge previous chunk with this chunk
            previous -> length = previous -> length + current -> length;
            previous -> next = current -> next;
            current = previous;
          }


        if (current -> next != nullptr){
          if (current -> next -> tag == -1){
            if (current -> next == tail) tail = current;
            
            //Merge the next chunk the current chunk
            current -> length = current -> length + current -> next-> length;
            current -> next = current -> next -> next;
          }
        }
        else{tail = current;}
        
        
      }
      //Does not have matching tag
      else{
        previous = current;
        current = current -> next;
      }
    }
  
  }


  // allocates a chunk using best-fit
  // if no suitable chunk found, requests new page(s)
  void alloc( long Tag, long size) {
    node *current = head;
    node *best_fit = nullptr;

    while(current != nullptr){
      
      //If node we are looking at is a hole that is at least as big as the requested size, we have found a candidate.
      if (current -> tag == -1 && current -> length >= size){

        //If we haven't found a fit yet
        if (best_fit == nullptr){
          //Set it to be the best fit
          best_fit = current;
          //Move onto the next node
          current = current -> next;
          continue;
        }
        else{
          //Is the current node we are looking at a BETTER fit that best_fit?
          if (best_fit -> length >= current -> length){
            //If so, we found a new best_fit
            best_fit = current;
            //Move onto the next node
            current = current -> next;
          }
          else
            current = current -> next;
          continue;
        }

      }
      else{ //Current node isn't a candidate. Move to next node
        current = current -> next;
      }
    }//End of while
   
    if(best_fit == nullptr){
      //Didn't find a fit, need memory. Add it to the end
      long mem = 0;
      if (tail != nullptr){
        if (tail -> tag == -1){
          mem = tail->length;
        }
      }
      
      if (mem < size){
        if (size <= PAGESIZE){
          pages++;
          mem += PAGESIZE;
        }
        else{
          long value = size/PAGESIZE+1;
          if ((size+mem)%PAGESIZE == 0) value--;
          if(mem/PAGESIZE == 0 && mem != 0){
            value = size/PAGESIZE;
          }
          value -= mem/PAGESIZE;
          pages+= value;
          mem += PAGESIZE *value;
        }

      }
      //printf("mem = %ld\n", mem);

      //This the first node
      if(head == nullptr){
        //Perfect fit
        if(mem == size){
          head = new node(Tag, 0, size, nullptr);
          tail = head;
        }
        //Imperfect fit
        else{
          tail = new node(-1, size, mem-size, nullptr);
          head = new node(Tag, 0, size, tail);
          
        }
      }
      
      //This is not the first node
      else{
        if(tail -> tag == -1)
          tail -> length = mem;
        if(mem == size){
          //Perfect fit
          if (tail -> tag == -1){
            tail -> tag = Tag;
            tail -> length = mem;
          }
          else{
            node *ptr = new node(Tag, (tail -> start + tail -> length), size, nullptr);
            tail -> next = ptr;
            tail = ptr;
          }
          
        }
        else{
          //Imperfect fit
          if (tail -> tag == -1){
            tail -> length = size;
            tail -> tag = Tag;
            tail -> next = new node(-1, (tail -> start+size), mem-size, nullptr);
            tail = tail ->next;
          }
          else{
            node *ptr = new node(-1, (tail -> start+size), mem-size, nullptr);

            node *ptr1 = new node(Tag, (tail -> start), size, ptr);
            tail -> next = ptr1;
            tail = ptr;
          }

        }
      }
    
      
    }//End of allocating new memory
    
    //We found a fit
    
    else{
      //Perfect fit
      if (best_fit -> length == size){
        best_fit -> tag = Tag;
      }
      //Imperfect fit
      else{

        long tmp = best_fit -> length;
        best_fit -> tag = Tag;
        best_fit -> length = size;
        node *ptr = new node(-1, best_fit -> start + best_fit -> length, tmp-size, best_fit -> next);
        best_fit -> next = ptr;
        
        if ( ptr -> next == nullptr) tail = ptr;
      }
      //printf("Tail length = %ld\n", tail -> length);
    }//End of modifying best_fit

    //Allocation processed.
  }


  // returns statistics about the simulation
  // - number of total page requests
  // - the largest available chunk
  void get_report( long & n_req, long & largest) {
    n_req = pages;
    largest = 0;
    node *current = head;
    while (current != nullptr){
      if (current -> tag == -1 && current -> length > largest) largest = current -> length;
      current = current -> next;
    }
  }
 private:
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
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
    node *ptr = ms.head;
    /*
    while (ptr != nullptr){
      printf("|%ld / %ld|, ", ptr -> tag, ptr -> length);
      ptr = ptr -> next;
    }
    printf("\n");
    */
    lng n1, n2;
    if( parse_line( n1, n2)) break;
    if( n1 < 0){
      ms.free( -n1);
    }
    else{
      ms.alloc( n1, n2);
    }
  }

  // report results
  // ------------------------------
  long n_page_requests, largest_free_chunk_size;
  ms.get_report( n_page_requests, largest_free_chunk_size);
  printf("pages requested: %ld\n", n_page_requests);
  printf("largest free chunk: %ld\n", largest_free_chunk_size);

  return 0;
}

