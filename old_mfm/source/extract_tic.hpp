#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <map>

//std::tuple<
  //std::map<std::string,std::vector<std::vector<std::string>>>,
  //std::map<std::string,std::vector<std::vector<double>>> 
//> extract_tic (std::vector<std::vector<std::string>> lines);
std::map<std::string,std::vector<std::vector<std::string>>> extract_tic (std::vector<std::vector<std::string>> lines) ;
