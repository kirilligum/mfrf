#pragma once

#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <tuple>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include "file_to_vvst.hpp"
#include "extract_mnemonics.hpp"
#include "extract_tic_factors.hpp"
#include "calc_ey.hpp"
#include "calc_roc.hpp"
#include "get_mnemonic.hpp"

std::tuple<
  cv::Mat,
  cv::Mat,
  cv::Mat,
  std::vector<double>
  > get_data_and_class(std::string file_name) {
  using namespace std;
  using namespace cv;
  bool new_parse=1;
  std::vector<std::vector<std::pair<std::string,double>>> ey;
  std::vector<std::vector<std::pair<std::string,double>>> roc;
  vector<vector<pair<string,double>>> price;
  if(new_parse) {
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
    //ofstream otf("tf.dat");
    //for(auto itic:tf) { 
      //otf << "*** " <<itic.first << " - ";
      //for(auto idate:itic.second) { 
        //for(auto i:idate) {///> erase tics with nan
          //otf << i.first << "-" << i.second <<"  "; 
        //} } 
      //otf << endl;
    //} 
    // get factors:
    // create a vector of pair<tic,f> for each factor
    ey = calc_ey(tf); ///> BUG gives out nan
    roc = calc_roc(tf);
    price = get_mnemonic(tf,"PRCCQ");
    ///////////////////////////////////////////////
    // saving to a serialized archive
    ///////////////////////////////////////////////
    ofstream ofs_ey("filtered_ey.dat");
    boost::archive::binary_oarchive oa_ey(ofs_ey);
    oa_ey << ey ;
    ofstream ofs_roc("filtered_roc.dat");
    boost::archive::binary_oarchive oa_roc(ofs_roc);
    oa_roc << roc ;
    ofstream ofs_price("filtered_price.dat");
    boost::archive::binary_oarchive oa_price(ofs_price);
    oa_price << price ;
  } else {
    /////////////////////////////////////////////////
    /// load from a serialized archive
    /////////////////////////////////////////////////
    std::ifstream ifs_ey("filtered_ey.dat");
    boost::archive::binary_iarchive ia_ey(ifs_ey);
    ia_ey >> ey;
    std::ifstream ifs_roc("filtered_roc.dat");
    boost::archive::binary_iarchive ia_roc(ifs_roc);
    ia_roc >> roc;
    std::ifstream ifs_price("filtered_price.dat");
    boost::archive::binary_iarchive ia_price(ifs_price);
    ia_price >> price;
  }
  ofstream oey("ey.dat"); for (auto i: ey[0]) oey << i.first << " " << i.second << "\n";
  ofstream oroc("roc.dat"); for (auto i: roc[0]) oroc << i.first << " " << i.second << "\n";
  ofstream oprice("price.dat"); for (auto i: price[0]) oprice << i.first << " " << i.second << "\n";
  // get std. div of the price
  // get eta = (price)^alpha/stddiv
  // fill training_data with with change in factors up to the 2 last points and training_class with eta
  //int samples = 3;
  vector<int> bins = {-3,-2,-1,0,1,2,3};
  cout << "ey.size = " << ey.size() << "    ey.front().size()" << ey.front().size() << endl;
  int samples = ey.size()-4;
  int leave_out = 5;
  int tottd = (samples-leave_out)*ey.front().size(); ///> total training data
  cv::Mat training_data(tottd,2,CV_32FC1 );
  //cv::Mat training_class(tottd,1,CV_32FC1 );
  cv::Mat training_class(tottd,1,CV_32SC1 );
  cv::Mat sample_data(ey.front().size(),2,CV_32FC1 );
  vector<double> d_last_price(price.front().size());
  //if(ey.size()!=roc.size()||ey.back().size()!=roc.back().size()) cout << "error: size problems with roc and ey\n";
  for (size_t i = 0; i < samples+1; ++i) { ///> time-wise
    for (size_t j = 0; j < ey[i].size(); ++j) { ///> stock-symbol-wise
  //cv::Mat training_data(2,2,CV_32FC1 );
  //cv::Mat training_class(2,1,CV_32FC1 );
  //for (size_t i = 0; i < 2; ++i) { ///> time-wise
    //for (size_t j = 0; j < 3; ++j) { ///> stock-symbol-wise
      double dey = 0.0;
        dey = ey[i+1][j].second - ey[i][j].second;
      double droc = 0.0;
        droc = roc[i+1][j].second - roc[i][j].second;
      double stddiv=1.0;
      double dprice = 0.0;
      if(price[i+2][j].second!=0.0) 
        dprice = pow((
              price[i+2][j].second - price[i+1][j].second) 
            /price[i+2][j].second
              ,1)/stddiv;
      if(dprice!=dprice) cout << " nan at ("<< i<< ","<< j<<")    " 
        << price[i+2][j].second << "  "
        << price[i+1][j].second << "  "
        << "\n";
      //int label =0;
      //for (size_t l = 0; l < 16+1; ++l) {
        //if(dprice>pow(l-8,2)*0.01 && dprice<pow(l-7,2)*0.01 ) label = l;
      //}
      //cout << (float)dey << " " <<(float)droc << " " <<(float)dprice << "\n";
      //training_data.at<float>(i,0) = 1.3*(i%2+3);
      //training_data.at<float>(i,1) = 1.2*(i%2+4);
      //training_class.at<float>(i,0) = 1.0*(i%2+5);
      //cout << 
        //training_data.at<float>(i,0)  << "  " <<
        //training_data.at<float>(i,1)  << "  " <<
        //training_class.at<float>(i,0) << "  --\n";
      int itd = i*((samples-leave_out)+1)+j;
      if(i<(samples-leave_out)) { ///> so that the last few in time series are not included
        training_data.at<float>(itd,0) = (float)dey;
        training_data.at<float>(itd,1) = (float)droc;
        //training_class.at<float>(itd,0) = (dprice*1e1>0)?1:0;
        //training_class.at<int>(itd,0) = (dprice*1e1>0)?1:0;
        int label = (int)(dprice*1e1);
        if(label <= bins.front()) training_class.at<int>(itd,0) = bins[0];
        for(int j = 1; j<bins.size()-1;++j) 
          if(label>bins[j]-0.5&&label<=bins[j]+0.5) training_class.at<int>(itd,0) = bins[j];
        if(label>bins.back()) training_class.at<int>(itd,0) = bins.back();
        //training_class.at<float>(i,0) = (int)(dprice*1e1);
        //training_class.at<float>(i,0) = (float)dprice;
      } else if (i==samples){
        sample_data.at<float>(j,0) = (float)dey;
        sample_data.at<float>(j,1) = (float)droc;
        d_last_price[j] = dprice;
      }
      //if(i==0) {
      //else if(i==ey.size()-3) {
        //sample_data.at<float>(j,0) = (float)dey;
        //sample_data.at<float>(j,1) = (float)droc;
      //} 
      else {
        //std::cout << "error in get_data_and_class \n";
        //std::cout << i << ".";
      }
    }
  }
  return std::make_tuple(training_data,training_class,sample_data,d_last_price);
}
