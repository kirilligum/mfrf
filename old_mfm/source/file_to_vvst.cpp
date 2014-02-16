#include "file_to_vvst.hpp"
#include <fstream>
#include <iostream>
#include <vector>
//#include <map>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/tokenizer.hpp>


std::vector<std::vector<std::string>> file_to_vvst(std::string file_name) {
  using namespace std;
  using namespace boost;
  ifstream inf(file_name);
  //ifstream inf(argv[1]);
  vector<vector<string>> lines;
  for(string line;getline(inf, line);) if(!inf.eof()) {
    //boost::char_separator<char> sep(",");
    //boost::tokenizer<boost::char_separator<char> > tok(line, sep);
    boost::tokenizer<boost::escaped_list_separator<char>> tok(line);
    //tokenizer<> tok(line);
    lines.emplace_back(tok.begin(),tok.end());
  }
  return lines;

}
