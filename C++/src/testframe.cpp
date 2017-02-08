#include "stlalgo_exam.h"
#include <gtest/gtest.h>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>

TEST(TestAddFunction , HandleAddOpr) {
    EXPECT_EQ( 2, testF(1,1));
    EXPECT_EQ( 100, testF(55,45));

    EXPECT_LT( 0, doGenRand() );
}


TEST(TestSetAlgorithm, CountV) {
    std::set<int> i_set= { 1, 1, 2, 3};

    int countno=0 ;
    countno = std::count_if(i_set.begin(), i_set.end(),
                            [](int v)->bool{ return v ==1;});

    EXPECT_EQ(1,countno);

    std::multiset<int> ml_i_set = { 1,2,5,6,5,6,9};
    countno = std::count_if(ml_i_set.begin(), ml_i_set.end(),
                            [](int v)->bool{ return v ==5;});

    EXPECT_EQ(2,countno);


    std::set<int> i_set_one = { 7,2,3};
    std::set<int> i_set_two = { 2,3,100};

    std::vector<int> v(3);

    auto diffend = std::set_difference( i_set_one.begin(), i_set_one.end(),
                                        i_set_two.begin(), i_set_two.end(),
                                        v.begin()
                                      );
    v.resize( diffend - v.begin());

    EXPECT_EQ(1, v.size());
    EXPECT_EQ(7, v[0]);

    v.resize(3);
    diffend = std::set_difference(
                  i_set_two.begin(), i_set_two.end(),
                  i_set_one.begin(), i_set_one.end(),
                  v.begin()
              );

    v.resize( diffend - v.begin());

    EXPECT_EQ(1, v.size());
    EXPECT_EQ(100, v[0]);
    v.resize(3);
    diffend = std::set_symmetric_difference(
                  i_set_two.begin(), i_set_two.end(),
                  i_set_one.begin(), i_set_one.end(),
                  v.begin()
              );

    v.resize( diffend - v.begin());

    EXPECT_EQ(2, v.size());
    EXPECT_EQ(7, v[0]);
    EXPECT_EQ(100, v[1]);

}



TEST( TestIteration, TestSwapValue){

    std::vector<int> v = {1,2,4,5};

    //std::iter_swap( v.begin(), prev(v.end()));

    // EXPECT_EQ( 5,v[0]);
    // EXPECT_EQ( 1,v[3]);

    std::vector<int> v2= { 9,6,7,8};


    std::iter_swap( v.begin(), v2.begin());
    EXPECT_EQ( 9,v[0]);
    EXPECT_EQ( 1,v2[0]);
}

template < class T>
bool gLess( T a, T b){
    return a<=b;
}

template < class T> 
bool _greater(T a, T b){ return a>=b;}

TEST( _TestMapV, _TestMapKeyV){

    std::map<int,int> mapII;
    mapII[0] = 100;
    mapII[2] = 200;
    mapII[4] = 400;

    std::vector< double >  vd = {1, 2, 1, 0.5, 1.3, 2,4 };

    std::sort( vd.begin(), vd.end(), gLess<double> );
    for( auto each : vd){
     std::cout<< each << " ";
    } 
       // std::cout<<'\n';
    std::set<double,std::greater<double> > sd = { 1, 0.5, 100,0.4, 7.3 , 2.7, 8.5,1.8} ;
    //std::sort( sd.begin(), sd.end(), gLess<double> );
    for( auto each : sd){ std::cout<< each << " ";} std::cout<<'\n';

    EXPECT_EQ(mapII[0], 100);
}


TEST ( _ListDemo , _ListCheck){

    std::list<int>  rlist = { 1, 2, 5, -1, -1,-2, 3};
    for( auto v: rlist) { 
        std::cout<< v<< " ";
    }
    std::cout<< std::endl;

    EXPECT_EQ( rlist.front() , 1);
    EXPECT_EQ(rlist.back(), 3);

    rlist.sort();

    for( auto v: rlist) { 
        std::cout<< v<< " ";
    }

    rlist.unique();
    std::cout<<std::endl;
    for( auto v: rlist) { 
        std::cout<< v<< " ";
    }


}


TEST(mapKV, mapKVTEST){

    std::map<int, int>  mapII;
    mapII[0] = 1; 
    mapII[2] = 2;
    mapII[3] =1;


    std::multimap<int,int> mmap;
    mmap.insert(std::make_pair( 0, 1));
    mmap.insert(std::make_pair(  0, 2));
    mmap.insert(std::make_pair(  0, 3));
    mmap.insert(std::make_pair(  0, 4));
    mmap.insert( std::make_pair( 5, 1));
    mmap.insert( std::make_pair( 5, 2));


    // mmap[0] = 1;
    // mmap[0] = 2;
    // mmap[0] = 3;
    // mmap[2] = 1;
    // mmap[3] = 1;
    // std::cout<< mmap.count(1)<< std::endl;

    EXPECT_EQ(4, mmap.count(0));

    std::pair< std::multimap<int,int>::iterator, std::multimap<int,int>::iterator> range;

    range = mmap.equal_range(0);
    for( auto v= range.first; v !=range.second; ++v){
        std::cout<< v->second << " ";
    }
    std::cout<<std::endl;
}

TEST( _1, _2 ){
    std::map< int, int>    imap={{0,1},{1,0},{2,-0},{3,1}}; 
    for( auto ieach:imap){
        std::cout<< ieach.second << " ";
    }
    std::cout<< '\n';

    EXPECT_EQ( 1,imap.count(0));
}