#include "extract_tic.hpp"

#include <tuple>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "extract_mnemonics.hpp"

//std::tuple<
  //std::map<std::string,std::vector<std::vector<std::string>>>
  //,
  ////std::map<std::string,std::vector<std::vector<double>>> 
//> extract_tic (std::vector<std::vector<std::string>> lines) {
std::map<std::string,std::vector<std::vector<std::string>>> extract_tic (std::vector<std::vector<std::string>> lines) {
  using namespace std;
  auto mnemonic = extract_mnemonics(lines.front());///> get names of observables
  map<string,vector<vector<string>>> info_by_tic;
  //map<string,vector<vector<double>>> data_by_tic;
  for(auto it=lines.begin()+1;it!=lines.end();++it) {
    string tic_name = (*it)[mnemonic["tic"]];
    //cout << tic_name << " ";
    //vector<double> time_data;
    //std::transform(begin(*it)+15, end(*it),back_inserter(time_data), [] (string st) { cout << st << " * "; return stod(st); }); 
    //get<1>(*get<0>(data_by_tic.emplace(make_pair(tic_name,vector<vector<double>>())))).push_back(time_data);
    get<1>(*get<0>(info_by_tic.emplace(make_pair(tic_name,vector<vector<string>>())))).emplace_back(begin(*it), end(*it));
  }
  return info_by_tic;
  //return make_tuple(info_by_tic,data_by_tic);
}
