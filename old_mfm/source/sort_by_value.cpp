#include "sort_by_value.hpp"
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
void sort_by_value( std::vector<std::vector<std::pair<std::string,double>>> &vals){ ///> time(tics(tic,fv)))
  using namespace std;
  for(auto &tfs:vals) { ///> BUG some numbers are out of sort
    sort(tfs.begin(), tfs.end(), [](pair<string,double> a, pair<string,double> b) {
        return a.second < b.second; });
  }
}
