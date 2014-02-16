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


std::vector<std::vector<std::pair<std::string,double>>> rank_tics(
    std::vector<std::vector<std::pair<std::string,double>>> bvs){
  using namespace std;
  vector<vector<pair<string,double>>> brs;
  transform(bvs.begin(),bvs.end(),back_inserter(brs), [](vector<pair<string,double>> byv){
      vector<pair<string,double>> byr;
      for(auto it=byv.begin();it!=byv.end();++it){
        byr.emplace_back(make_pair(it->first,distance(byv.begin(),it)));
      }
      return byr;
      });
  return brs;
}
