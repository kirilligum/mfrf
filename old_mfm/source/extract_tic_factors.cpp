#include "extract_tic_factors.hpp"

#include <tuple>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <boost/lexical_cast.hpp>

#include "extract_mnemonics.hpp"

std::map< std::string, std::vector< std::map< std::string, double> > > 
extract_tic_factors (std::vector<std::vector<std::string>> lines, std::vector<std::string> factor) {
  using namespace std;
  map<string,vector<map<string,double>>> tic_factors;
  auto mnemonic = extract_mnemonics(lines.front());///> get names of observables
  for(auto it=lines.begin()+1;it!=lines.end();++it) {
    string tic_name = (*it)[mnemonic["tic"]];
    map<string,double> fs;
    for(auto ifac:factor) {
      try{
        fs.emplace(make_pair(ifac,boost::lexical_cast<double>((*it)[mnemonic[ifac]])));
      } catch (boost::bad_lexical_cast&) {
        fs.emplace(make_pair(ifac,std::numeric_limits<double>::quiet_NaN()));
      }
    }
    get<1>(
        * get<0>(
          tic_factors.emplace(tic_name,vector<map<string,double>>())
          )
        ).push_back(fs);
  }
  return tic_factors;
}
