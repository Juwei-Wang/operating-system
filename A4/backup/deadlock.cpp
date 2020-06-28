#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>
#include <set>
#include <list>
//juwei wang 30053278

typedef std::vector<std::string> vs_t;

enum Edge {
    Allocation, Request
};

// split string p_line into vector of strings (words)
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

// read in a line from stdin and parse it
// parsed results are passed back via parameter references
// return true on success, false on EOF
// aborts program on bad input (should t    A8S 8DY         hrow exception)
bool
parseLine( std::string & proc_name, std::string & res_name, Edge & etype)
{
    char buff[4096];
    vs_t words;

    // read in a line and parse it into words
    while(1) { // loop to skip empty lines
        if( ! fgets( buff, sizeof(buff), stdin))
            return false;
        words = split(buff);
        if( words.size() != 0) break;
    }

    if( words.size() != 3) {
        printf("Bad line: %s\n", buff);
        exit( -1); // exception would be nicer
    }

    proc_name = words[0];
    res_name = words[2];
    etype = Edge::Request;

    if( words[1] == "->") {
        return true;
    }
    if( words[1] == "<-") {
        etype = Edge::Allocation;
        return true;
    }

    printf("Bad line: %s\n", buff);
    exit( -1); // exception would be nicer
};

// utility class you can use to convert words to unique integers
// get(word) returns the same number given the same word
//           the numbers will start at 0, then 1, 2, ...

// HINT: this "may" help you get a bit more performance
// in your cycle finding algorithm, since indexed arrays are faster
// than hash tables... if you don't know what that means,
// safely ignore this hint :)
struct Word2Int {
    int
    get(const std::string & w) {
        auto f = myset.find(w);
        if( f == myset.end()) {
            myset[w] = counter;
            counter ++;
            return counter - 1;
        }
        return f-> second;
    }
private:
    int counter = 0;
    std::unordered_map<std::string,int> myset;
};

struct Graph {

    // add your own members here
    // ...
    std::set<std::string> nodes_wo;
    std::unordered_map<std::string, std::list<std::string>> adj_list_copy;
    std::unordered_map<std::string, int> out_counts_copy;
    std::set<std::string> all_nodes_copy;

    //  std::unordered_map<std::string, vs_t> adj_list;
    std::unordered_map<std::string, std::list<std::string>> adj_list;
    std::unordered_map<std::string, int> out_counts;
    std::set<std::string> all_nodes;
    std::set<std::string> deadlock_c;

    Graph() {
        // reimplement this (probably)
    }

    // add edge n1 -> n2
    void
    add( std::string n1 , std::string n2) {
        // reimplement this
        all_nodes.insert(n1);
        all_nodes.insert(n2);
        adj_list[n2].push_back(n1);
        out_counts[n1] ++;
    }

    // run cycle detection

    bool
    deadlock() {

        // fine the nodes with 0 outgoing degree
        // 找到没有outgoing degree 的结点
        for( const std::string & n : all_nodes) {
            out_counts[n];
        }
        adj_list_copy = adj_list;     //copy list
        out_counts_copy = out_counts;  //copy list
        all_nodes_copy = all_nodes;  //copy list
        nodes_wo.clear();  //copy list



        //node_wo已经找出了为0的集合
        //if we already find the set that stores the nodes with 0 outgoing degree
        bool check_count = true;

        while (check_count){
            check_count = false;
            std::set<std::string> temp_nodes;
            temp_nodes.clear();


            //移除掉原来count为0的结点
            //remove the nodes with 0 outgoing degree
            for( const auto & e : out_counts_copy) {
                //printf("%10s (%2d)\n",e.first.c_str(),e.second);
                if( e.second == 0){
                    nodes_wo.insert(e.first.c_str());
                    temp_nodes.insert(e.first.c_str());
                    all_nodes_copy.erase(e.first.c_str());
                    // if no more nodes in the graph , break and no deadlock detected
                    if(all_nodes_copy.size() == 0){
                        break;
                    }

                }
            }

            //移除掉原来count为0的结点
            //remove the nodes with 0 outgoing degree
            for (const std::string & n : temp_nodes){
              if(out_counts_copy.find(n.c_str()) != out_counts_copy.end()){
                 out_counts_copy.erase(n.c_str());
              }
      
            }

            // if no more nodes in the graph , break and no deadlock detected
            if(all_nodes_copy.size() == 0){
                break;
            }

            //check all the incoming nodes when we remove a node
            //找出了所有out为0结点的incoming nodes
            for (const std::string & n : temp_nodes) {
                //printf("%10s ::", n.c_str());
                for( const auto & s : adj_list_copy[n]) {
                    //printf("%s , ", s.c_str());
                    out_counts_copy[s] --;
                }
                //printf("\n");
            }


            // check if there are nodes in gragh with 0 outgoing degree
            for( const std::string & n : all_nodes_copy) {
                //printf("%10s (%2d)\n",n.c_str(),out_counts_copy[n]);
                if (out_counts_copy[n] == 0){
                    check_count = true;

                };
            }

            // if there is a node in graph and no nodes can be removed, a deadlock detect
            if(!check_count){
                for( const std::string & n : all_nodes_copy){
                    deadlock_c.insert(n.c_str());
                }
                printf("There exits a deadlock\n");
                return true;

            }

        }

        return false;
    }

    // report cycle
    //n
    void
    report() {
        // reimplement this
        //printf("Not sure if there is deadlock...\n"
        //       "but the outdegrees + adjacency list (reversed) looks like this:\n");

        //for( const std::string & n : all_nodes) {
        //    printf("%10s (%2d) : ", n.c_str(), out_counts[n]);
        //    for( const auto & s : adj_list[n]) {
        //        printf("%s , ", s.c_str());
        //    }
        //    printf("\n");
        //}

        // list of nodes with no outgoing arrows:
        //printf("Nodes without outgoing edges... these are not waiting on anything..\n");
        //for( const auto & e : out_counts) {
        //    if( e.second == 0){
                //nodes_wo.insert(e.first.c_str());
        //        printf("* %s \n", e.first.c_str());
        //    }
            //printf("%s \n",e.first.c_str());
        //}
        //printf("\n");


        //nodes_wo.insert("hello");
        //printf("nodes_wO size is %id\n", nodes_wo.size());
        //printf("Here are the nodes with 0 outgoing degree \n");
        //for (const std::string & k : nodes_wo) {
        //    printf("%10s (%2d) : \n", k.c_str());
        //}

        //print the outcome
        if(deadlock_c.size() == 0){
          printf("No deadlock\n");
        }
        printf("Deadlocked processes\n");
        for( const std::string & n : deadlock_c){
            if(n.c_str()[0] == 'p'){
            printf("%s ", n.c_str());
            }
        }
        printf("\n");

    }
};

int
main()
{

    Graph g;

    while(true) {
        std::string pname, rname;
        Edge e;
        std::string edge_p;
        if( ! parseLine(pname, rname, e)) break;

        if( e == Edge::Request){
            g.add( "p_" + pname, "r_" + rname);
           // edge_p = "P_";//+ pname  + "->" + "r_" + rname;
        }
        else{
            g.add( "r_" + rname, "p_" + pname);
           // edge_p = "P_"; //+ pname  + "<-" + "r_" + rname;
        }
        if( g.deadlock()){
            printf("p_%s", pname.c_str());
            if(e  == Edge::Request){
              printf(" --> ");
            }else{
              printf(" <-- ");
            }
            printf("r_%s \n", rname.c_str());
            break;
        }
    }

    g.report();

    return 0;
}
