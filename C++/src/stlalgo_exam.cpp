#include "stlalgo_exam.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <set>

int testF( int a , int b) {
    return a+b;
}

struct randomize {
    std::uniform_int_distribution<int>     i_dist;
    std::uniform_real_distribution<double> r_dist;
    std::normal_distribution<double>       n_dist;

    randomize():
        i_dist(20,40),r_dist(-1,1),n_dist(0,1) {

    }
};

int doGenRand() {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1,100);
    std::vector<int> vrandout;
    for( int n =0; n < 10; ++n) {
        vrandout.push_back( dis(gen) ) ;
    }

    auto minmaxv = std::minmax_element( vrandout.begin(), vrandout.end());

    std::normal_distribution<double>  n_dist(0,1);
    for ( int i = 0; i< 10; ++i) {
        std::cout<< n_dist(gen)<< std::endl;
    }

    return (*minmaxv.first);
}

int count() {
    std::set<int> i_set= { 1, 1, 2, 3};

    int countno ;
    std::count(i_set.begin(), i_set.end(), countno);
    return countno;
}