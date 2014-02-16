#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <map>

std::map< std::string, std::vector< std::map< std::string, double> > > 
extract_tic_factors (std::vector<std::vector<std::string>> lines, std::vector<std::string> factor) ;
