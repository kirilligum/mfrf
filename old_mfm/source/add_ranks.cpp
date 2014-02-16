#include "rank_tics.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/lexical_cast.hpp>


std::vector<std::vector<std::pair<std::string,double>>> add_ranks(
    std::vector<std::vector<std::pair<std::string,double>>> as,
    std::vector<std::vector<std::pair<std::string,double>>> bs
    ){
  using namespace std;
  vector<vector<pair<string,double>>> tots;
  std::transform(as.begin(),as.end(),bs.begin(),back_inserter(tots),[](
        vector<pair<string,double>> a,vector<pair<string,double>> b){
      vector<pair<string,double>> tot;
      for(auto i:a) {
        tot.emplace_back(make_pair(i.first,i.second+
            find_if(b.begin(),b.end(),[&](pair<string,double> j) {return (i.first==j.first);})->second));
      }
      return tot;
      });
  return tots;
}
