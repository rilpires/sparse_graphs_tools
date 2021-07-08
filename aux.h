
#ifndef MAX_PARTITIONS
#define MAX_PARTITIONS 500U
#define MAX_SUM 500U
#endif

#define uint64_t unsigned long long int

#include <vector>
#include <unordered_map>
#include <map>

struct state {
    uint64_t sum;
    unsigned short partitions;
    std::vector<uint64_t> v;
    
    state( std::vector<uint64_t> );
    state( uint64_t p_sum , uint64_t p_partitions );

    bool    go_to_next();
};

int count( int sum , int partitions ){
    if(partitions==0) return 0;
    if(partitions==1) return 1;
    if(sum<=1) return 1;
    
    static int mem_count[MAX_SUM][MAX_PARTITIONS];

    if( mem_count[sum][partitions] == 0 ){
        int ret = 0;
        int mod = sum % partitions;
        int i;
        for( i = 0 ; i*partitions + mod <= sum ; i++ )
            ret += count( i*partitions + mod , partitions-1 );
        mem_count[sum][partitions] = ret;
    }

    return mem_count[sum][partitions];
}


state::state( std::vector<uint64_t> p_v ){
    v = p_v;
    partitions = v.size();
    sum = 0;
    for( int i = 0 ; i < partitions ; i++ ) sum += (i+1)*v[i];
}

state::state( uint64_t p_sum , uint64_t p_partitions ){
    sum = p_sum;
    partitions = p_partitions;
    v.resize(partitions);
    uint64_t remainder = sum;
    for( int i = 0 ; i < partitions ; i++ ){
        v[partitions-i-1] = remainder / (partitions-i);
        remainder -= v[partitions-i-1] * ( partitions-i );  
    }
}

bool    state::go_to_next(){
    switch (partitions) {
        case 1:{ 
            return false;
        }
        case 2:{
            if( v[1] > 0 ){
                v[1]--;
                v[0]+=2;
                return true;
            } else {
                return false;
            }
        }
        default:{
            std::vector<uint64_t> subv = std::vector<uint64_t>( v.begin() , v.end()-1 );
            state substate = state(subv);
            if( substate.go_to_next() ){
                for( int i = 0 ; i < partitions-1 ; i++ ){
                    v[i] = substate.v[i];
                }
                return true;
            } else if (v[v.size()-1] > 0) {
                v[ v.size()-1 ]--;
                substate = state( substate.sum + partitions , partitions-1 );
                for( int i = 0 ; i < partitions-1 ; i++ ){
                    v[i] = substate.v[i];
                }
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

uint64_t sum_distances_m_m( uint64_t m1 , uint64_t m2 , uint64_t a , uint64_t b , uint64_t c , uint64_t d ){
    if( m2 > m1 ) return sum_distances_m_m( m2 , m1 , a , c , b , d );

    using namespace std;
    uint64_t ret = 0 ;
    uint64_t i1 = 0 , i2 = 0;
    for( i2 = 0 ; i2 < m2 ; i2++ ){
        uint64_t toptop = a+1+i2;
        uint64_t topbot = b+m2-i2;
        uint64_t bottop = c+1+i2;
        uint64_t botbot = d+m2-i2;
        uint64_t best_top_exit = min(toptop,topbot);
        uint64_t best_bot_exit = min(bottop,botbot);
        for( i1 = 0 ; i1 < m1 ; i1++ )
            ret += min( 1+i1+best_top_exit , m1-i1+best_bot_exit );
    }
    return ret;
}
uint64_t sum_distances_m_internal( uint64_t m ){
    using namespace std;
    uint64_t ret = 0;
    for( uint64_t i = 0 ; i < m ; i ++ )
        ret += (m-i-1)*(m-i)/2;
    return ret;
}

/* =========================
--------- m = n+1 ----------
============================ */
uint64_t sum_distances_h_m_theta( uint64_t m1 , uint64_t m2 , uint64_t m3 ){
    uint64_t ret = 0;
    for( auto p : {m1,m2,m3} )
        ret += p*(p+1);
    return ret;
}
uint64_t sum_distances_h_h_theta( uint64_t m1 , uint64_t m2 , uint64_t m3 ){
    return 1+m3;
}
uint64_t sum_distances_m_m_theta( uint64_t m1 , uint64_t m2 , uint64_t m3 ){
    return \
        sum_distances_m_m(m1,m2,0,m3,m3,0)+
        sum_distances_m_m(m2,m3,0,m1,m1,0)+
        sum_distances_m_m(m3,m1,0,m2,m2,0)+
        sum_distances_m_internal(m1)+
        sum_distances_m_internal(m2)+
        sum_distances_m_internal(m3);
}
uint64_t sum_distances_theta( uint64_t m1 , uint64_t m2 , uint64_t m3 ){
    return  sum_distances_m_m_theta(m1,m2,m3) + 
            sum_distances_h_m_theta(m1,m2,m3) + 
            sum_distances_h_h_theta(m1,m2,m3);
}


/* =========================
------ m = n+2 (THETA) -----
============================ */
uint64_t sum_distances_h_m_theta( uint64_t m1 , uint64_t m2 , uint64_t m3 , uint64_t m4 ){
    uint64_t ret = 0;
    for( auto p : {m1,m2,m3,m4} )
        ret += p*(p+1);
    return ret;
}
uint64_t sum_distances_h_h_theta( uint64_t m1 , uint64_t m2 , uint64_t m3 , uint64_t m4 ){
    return 1+m4;
}
uint64_t sum_distances_m_m_theta( uint64_t m1 , uint64_t m2 , uint64_t m3 , uint64_t m4 ){
    return \
        sum_distances_m_m(m1,m2,0,m4,m4,0)+
        sum_distances_m_m(m1,m3,0,m4,m4,0)+
        sum_distances_m_m(m1,m4,0,m3,m3,0)+
        sum_distances_m_m(m2,m3,0,m4,m4,0)+
        sum_distances_m_m(m2,m4,0,m3,m3,0)+
        sum_distances_m_m(m3,m4,0,m2,m2,0)+
        sum_distances_m_internal(m1)+
        sum_distances_m_internal(m2)+
        sum_distances_m_internal(m3)+
        sum_distances_m_internal(m4);
}
uint64_t sum_distances_theta( uint64_t m1 , uint64_t m2 , uint64_t m3 , uint64_t m4 ){
    return  sum_distances_m_m_theta(m1,m2,m3,m4) + 
            sum_distances_h_m_theta(m1,m2,m3,m4) + 
            sum_distances_h_h_theta(m1,m2,m3,m4);
}

