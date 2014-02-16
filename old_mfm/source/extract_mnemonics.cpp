#include "extract_mnemonics.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <iterator>

std::map<std::string,int> extract_mnemonics(std::vector<std::string> line ){ ///> basically names of observables
  using namespace std;
  map<string,int> mnemonic; 
  for(auto it=line.begin();it!=line.end();++it) {
    mnemonic.emplace(make_pair(*it, distance(line.begin(),it)));
  }
  return mnemonic;
}
