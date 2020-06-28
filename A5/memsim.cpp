#include <stdio.h>
#include <string>
#include <vector>
#include <cassert>
#include <map>
// 30053278 juwei wang

typedef std::vector<std::string> vs_t;


//声明一个node类型来创造linkedlist
struct node{
    long tag;
    long length;
    struct node *next;
    node(long Tag, long Length, struct node *node){
        tag = Tag;
        length = Length;
        next = node;
    }

};

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


struct MemSim {
    long requsted_p;
    long page_size_k;
    node *begin;
    node *end_m;


    // initializes the linkedlist
    // 初始化链子
    MemSim( long page_size) {
        requsted_p = 0;
        page_size_k = page_size;
        node *first = new node(-1,page_size_k, nullptr);
        begin = first;
        end_m = first;
        requsted_p ++;
    }


    // free block
    // 让所有tag相同的变成free(-1)
    void free( long Tag) {


        if (begin == nullptr) return;
        node *prev_node = nullptr;
        node *current_node = begin;
        //check every space in the list
        while (current_node != nullptr){

            //if the tag match then set the tag to -1
            if (current_node -> tag == Tag){
                current_node -> tag = -1;

                //merge left
                if(prev_node != nullptr){

                    if (prev_node -> tag == -1){
                        // let the current chunk goes to the prev_node
                        prev_node -> length = prev_node -> length + current_node -> length;
                        prev_node -> next = current_node -> next;
                        current_node = prev_node;
                    }
                }

                //merge right
                if (current_node -> next != nullptr){
                    // check next tag is free
                    if (current_node -> next -> tag == -1){
                        //check if next node is the last node
                        if (current_node -> next == end_m){
                            end_m = current_node;
                        }
                        //Merge the next chunk the current_node chunk
                        current_node -> length = current_node -> length + current_node -> next-> length;
                        current_node -> next = current_node -> next -> next;
                    }
                }
                else{end_m = current_node;}

            }else{
                prev_node = current_node;
                current_node = current_node -> next;
            }
        }
        //printf("loop-----------------------7\n");
        /*node *pp = begin;
        while (pp != nullptr){
            printf("tag %ld , size %ld", pp ->tag, pp->length);
            printf("\n");
            pp = pp -> next;
        }
        printf("\n");
        printf("---------------------------\n");*/
    }

    // allocates a chunk using best-fit
    // if no suitable chunk found, requests new page(s)
    void alloc( long Tag, long size) {
        //printf("current tag%ld \n",Tag);
        //printf("current size%ld \n",size);
        //把指针转向整个list的开端
        node *current_node = begin;
        //node *best_fit = nullptr;
        node *true_node = nullptr;
        bool found = false;
        bool first_found = true;
        long temp_size = 0;

        //开始检测整个链子 for (auto & elem : current)
        while(current_node != nullptr){

            //if (elem[0] == -1 && elem[1] >= size)
            //If node we are looking at is a hole that is at least as big as the requested size, we have found a candidate.
            if (current_node -> tag == -1 && current_node -> length >= size){

                //If we haven't found a fit yet
                found = true;
                if (first_found){
                    //printf("loop 1 \n");
                    temp_size = current_node -> length;
                    true_node = current_node;
                    first_found = false;
                }
                if (current_node -> length < temp_size) {
                    //printf("loop 2 \n");
                    temp_size = current_node->length;
                    true_node = current_node;
                }

            }
            current_node = current_node -> next;
        }

        if(found) {
            //Didn't find a fit, need memory. Add it to the end
            //如果找到了 就判断是size等于还是大于
            //大于就生出多个chunk
            //等于就直接放进size里面
            if (true_node ->length > size) {
                //printf("loop found 1 \n");
                long pre_size = true_node->length;
                node *ptr = true_node->next;
                node *ptr_1 = new node(Tag, size, ptr);
                true_node->tag = Tag;
                true_node->length = size;
                true_node->next = ptr_1;
                ptr_1->next = ptr;
                ptr_1->tag = -1;
                ptr_1->length = pre_size - size;
                if(ptr_1 -> next == nullptr) end_m = ptr_1;
            } else {
                true_node->tag = Tag;
                true_node->length = size;
            }

        }else{
            //printf("end ** tag %ld , end ** size %ld", end_m ->tag, end_m ->length);
            if(end_m -> tag > 0){
                //printf("loop not found 1 \n");
                long size_m;
                bool div_exac = false;
                if(size % page_size_k == 0){
                    size_m = (size / page_size_k);
                    div_exac = true;
                }else {
                    size_m = (size / page_size_k) + 1;
                }
                long add_size = page_size_k * size_m;
                if(!div_exac){
                    node *pt  = new node(-1,add_size - size, nullptr);
                    node *pt1 = new node(Tag,size, pt);
                    end_m -> next = pt1;
                    requsted_p = requsted_p + size_m;
                    end_m = pt;
                } else{
                    node *pt1 = new node(Tag,size, nullptr);
                    end_m -> next = pt1;
                    requsted_p = requsted_p + size_m;
                    end_m = pt1;
                }
            } else{
                long cha_zhi = size - (end_m -> length);
                long size_m;
                bool div_exac = false;
                if(size % page_size_k == 0){
                    size_m = (cha_zhi / page_size_k);
                    div_exac = true;
                } else {
                    size_m = (cha_zhi / page_size_k) + 1;
                }
                long add_size = page_size_k * size_m;
                long total = add_size + (end_m -> length);
                if(!div_exac){
                    end_m -> tag = Tag;
                    end_m -> length = size;
                    node *pt1 = new node (-1, total - size, nullptr);
                    end_m -> next = pt1;
                    requsted_p = requsted_p + size_m;
                    end_m = pt1;
                } else{
                    end_m -> tag = Tag;
                    end_m -> length = size;
                    requsted_p = requsted_p + size_m;
                }
            }

            //printf("end_m length = %ld\n", end_m -> length);
        }//End of modifying best_fit

        //Allocation processed.
        /*node *pp = begin;
        while (pp != nullptr){
            printf("tag %ld , size %ld", pp ->tag, pp->length);
            printf("\n");
            pp = pp -> next;
        }
        printf("\n");
        printf("---------------------------\n");*/

    }


    // returns statistics about the simulation
    // - number of total page requests
    // - the largest available chunk
    void get_report( long & n_req, long & largest) {
        n_req = requsted_p;
        largest = 0;
        node *current_node = begin;
        while (current_node != nullptr){
            if (current_node -> tag == -1 && current_node -> length > largest) largest = current_node -> length;
            current_node = current_node -> next;
        }
    }
private:
};

/*//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////*/
int
main( int argc, char ** argv)
{
    // parse command line arguments
    // ------------------------------
    bool first_alloc = false;
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
        //node *ptr = ms.begin;
        /*
        while (ptr != nullptr){
          printf("|%ld / %ld|, ", ptr -> tag, ptr -> length);
          ptr = ptr -> next;
        }
        printf("\n");
        */
        long n1, n2;
        if( parse_line( n1, n2)) break;
        if( n1 < 0){
            ms.free( -n1);
        }
        else{
            ms.alloc( n1, n2);
            first_alloc = true;
        }
    }

    // report results
    // ------------------------------
    long n_page_requests, largest_free_chunk_size;
    ms.get_report( n_page_requests, largest_free_chunk_size);
    if(!first_alloc){
        n_page_requests = 0;
        largest_free_chunk_size = 0;
    }
    printf("pages requested: %ld\n", n_page_requests);
    printf("largest free chunk: %ld\n", largest_free_chunk_size);

    return 0;
}
