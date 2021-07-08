
#include <iostream>

#include "aux.h"

#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;

int main(){
    int sum,partitions;
    
    state s = state(200,4);
    char str[128];

    map<string,uint64_t> results;
    map<uint64_t,vector<string> > ordered_results;

    do {
        uint64_t m1 = s.v[3]+s.v[2]+s.v[1]+s.v[0];
        uint64_t m2 = s.v[3]+s.v[2]+s.v[1];
        uint64_t m3 = s.v[3]+s.v[2];
        uint64_t m4 = s.v[3];
        uint64_t wiener_bigsum = sum_distances_theta(m1,m2,m3,m4);

        // sprintf( str , "%llu \t %llu \t %llu \t => \t %llu" , m1 , m2 , m3 , wiener_bigsum );
        // std::cout << str << std::endl;

        char key[128];
        sprintf( key , "%llu|%llu|%llu|%llu" , m1 , m2 , m3 , m4 );
        results[ key ] = wiener_bigsum;

        ordered_results[wiener_bigsum].push_back(key);

    } 
    while ( s.go_to_next() );

    for( auto p : ordered_results ){
        for( auto s : p.second ){
            cout << p.first << "\t=>\t" << s << endl;
        }
    }
    cout << "Done" << endl;
}