#include "longsublen.h"
#include <map>

namespace leetcode {
  
int SolutionSSL::lengthOfLongestSubstring(std::string s)
{
    int start_pos=0;
    int stop_pos= 1;
    
    int end_pos = s.length();
    int maxlen = 0;
    std::map<char, int> charpos;
    while( stop_pos < end_pos){
      char t = s[stop_pos];
      if( t == s[start_pos] || charpos[t] > start_pos){
	  int len = stop_pos - start_pos;
	  start_pos +=1;
	  if( len > maxlen) maxlen = len;
      } 
      charpos[t] = stop_pos;
      ++stop_pos;
    
    }    
    return maxlen;    
}

}//endof namespace leetcode