#include "calc_ey.hpp"
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

std::vector<std::vector<std::pair<std::string,double>>> calc_roc( ///> time(tics(tic,fv)))
std::map< std::string, std::vector< std::map< std::string, double> > > tmd) {
  using namespace std;
  vector<vector<pair<string,double>>> roc
    ((*tmd.begin()).second.size());
  for(auto itic:tmd) {
    for(auto &rocitime:roc) {
      rocitime.push_back(make_pair(itic.first,0.0));
    }
  }
  for(auto itic:tmd) {
    boost::for_each(itic.second,roc,[&](map<string,double> m, vector<pair<string,double>> &facs) {
        double OIADPQ= m["OIADPQ"];
        double PPENTQ= m["PPENTQ"]; 
        double ACTQ  = m["ACTQ"];
        double CHEQ  = m["CHEQ"];  
        double LCTQ  = m["LCTQ"];
        double DLCQ  = m["DLCQ"];  
        double roc_val = OIADPQ*4/(PPENTQ + ACTQ - CHEQ - LCTQ + DLCQ);
        find_if(facs.begin(),facs.end(),[&](pair<string,double> fac) {return (itic.first==fac.first);})->second=roc_val;
        });
  }
  return roc;
}
