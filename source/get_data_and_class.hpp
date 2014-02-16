#pragma once

#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <tuple>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

#include "file_to_vvst.hpp"
#include "extract_mnemonics.hpp"
#include "extract_tic_factors.hpp"
#include "calc_ey.hpp"
#include "calc_roc.hpp"
#include "get_mnemonic.hpp"

std::tuple<cv::Mat,cv::Mat> get_data_and_class(std::string file_name) {
  using namespace std;
  using namespace cv;
  cv::Mat training_data, training_class;
  auto lines = file_to_vvst(file_name); ///> the first line are names of the columns
  auto mnemonic = extract_mnemonics(lines.front());///> get names of observables
  auto tf = extract_tic_factors(lines,vector<string>{ "OIADPQ" ,"PRCCQ" ,"CSHOQ" ,"AJEXQ" ,"PSTKQ" ,"DLTTQ" ,"DLCQ" ,"CHEQ" ,"PPENTQ" ,"ACTQ" ,"LCTQ" });
  cout << "tf size: " << tf.size() << "\n";
  vector<string> nan_tics;
  for(auto itic:tf) { for(auto idate:itic.second) { for(auto i:idate) {///> erase tics with nan
        if(!(i.second==i.second)) nan_tics.push_back(itic.first); } } } 
        //if(!(i.second==i.second)) tf.erase(itic.first); } } } 
  for(auto i:nan_tics) tf.erase(i);
  cout << "tf size: " << tf.size() << "\n";
  ofstream otf("tf.dat");
  for(auto itic:tf) { 
    otf << "*** " <<itic.first << "\n";
    for(auto idate:itic.second) { 
      for(auto i:idate) {///> erase tics with nan
        otf << i.first << "-" << i.second <<"\n"; 
      } } } 

  // get factors:
  // create a vector of pair<tic,f> for each factor
  auto ey = calc_ey(tf); ///> BUG gives out nan
  ofstream oey("ey.dat"); for (auto i: ey[0]) oey << i.first << " " << i.second << "\n";
  auto roc = calc_roc(tf);
  ofstream oroc("roc.dat"); for (auto i: roc[0]) oroc << i.first << " " << i.second << "\n";
  // get price
  vector<vector<pair<string,double>>> price = get_mnemonic(tf,"PRCCQ");
  ofstream oprice("price.dat"); for (auto i: price[0]) oprice << i.first << " " << i.second << "\n";
  // get std. div of the price
  // get eta = (price)^alpha/stddiv
  // fill training_data with with change in factors up to the 2 last points and training_class with eta
  if(ey.size()!=roc.size()||ey.back().size()!=roc.back().size()) cout << "error: size problems with roc and ey\n";
  for (size_t i = 0; i < ey.size()-2; ++i) { ///> time-wise
    for (size_t j = 0; j < ey[i].size(); ++j) { ///> stock-symbol-wise
      double dey = ey[i+1][j].second - ey[i][j].second;
      double droc = roc[i+1][j].second - roc[i][j].second;
      double stddiv=1.0;
      double dprice = pow((
            price[i+2][j].second - price[i+1][j].second)/price[i+2][j].second
              ,1)/stddiv;
      //int label =0;
      //for (size_t l = 0; l < 16+1; ++l) {
        //if(dprice>pow(l-8,2)*0.01 && dprice<pow(l-7,2)*0.01 ) label = l;
      //}
      training_data.at<float>(i,0) = dey;
      training_data.at<float>(i,1) = droc;
      training_class.at<float>(i,0) = dprice;
    }
  }
  return std::make_tuple(training_data,training_class);
}
