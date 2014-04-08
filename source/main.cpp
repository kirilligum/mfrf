#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <cstdlib>
//#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include "get_data_and_class.hpp"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[]) {
  cout << " hi \n";
  cout << "Creating train data and responses Mat\n";
  Mat training_data, training_class;
  Mat sample_data;
  std::vector<double> d_last_price;
  tie(training_data,training_class,sample_data,d_last_price) = get_data_and_class("sp500.csv");
  ofstream("training_data.txt") << training_data <<endl;;
  ofstream("training_class.txt") << training_class <<endl;;
  ofstream("sample_data.txt") << sample_data <<endl;;
  int ATTRIBUTES_PER_SAMPLE = training_data.cols;
  cout << "done\n";
  cout << "set parameters for random forest\n";
  CvRTParams params = CvRTParams(
      25, // max depth
      5, // min sample count
      0, // regression accuracy: N/A here
      false, // compute surrogate split, no missing data
      5, // max number of categories (use sub-optimal algorithm for larger numbers)
      0, // the array of priors
      //priors, // the array of priors
      false,  // calculate variable importance
      4,       // number of variables randomly selected at node and used to find the best split(s).
      1000,	 // max number of trees in the forest
      0.00001f,				// forrest accuracy
      CV_TERMCRIT_ITER |	CV_TERMCRIT_EPS // termination cirteria
      );
  cout << "run random forest training \n";
  Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
  var_type.setTo(Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical
  CvRTrees* rtree = new CvRTrees;
  rtree->train(training_data, CV_ROW_SAMPLE, training_class, 
      Mat(), Mat(), Mat(), Mat(), params);
      //Mat(), Mat(), var_type, Mat(), params);
  //cout << " get_tree_count " << rtree->get_tree_count() << endl;
  //rtree->get_tree(0)->save("tree00.yml");
  cout << "make a prediction\n";
  //for (size_t i = 0; i < training_class.size(); ++i) {
  //for (size_t i = 0; i < training_data.rows; ++i) {
    ////Mat sample(1,2,CV_32FC1 );
    ////cout << training_data.row(i) << "  ";
    ////sample.row(0) = training_data.row(i);
    //Mat sample = training_data.row(i);
    //cout << sample << "  ";
    //cout << training_class.row(i) << "  ";
    ////cout << (rtree->predict(sample)>0.5)? 0:1;
    //cout << rtree->predict(sample);
    //cout << endl;
  //}
  //cout << rtree->predict(sample_data);
  std::vector<double> quarter_profit(7,0.0);
  vector<int> scount(7,0);
  for(int i = 0 ; i< sample_data.rows ; ++i){
    double signal = rtree->predict(sample_data.row(i));
    double dlast = d_last_price[i];
    //cout << signal << "=" << dlast << "  ";
    for(int i=0;i<7;++i) {
      if(signal==i-3) {
        quarter_profit[i]+=dlast;
        ++scount[i];
      }
    }
    //if(signal==2) { quarter_profit+=dlast; ++scount;}
  }
  cout << endl;
  for(int i=0;i<7;++i) {
    double qp = 0.0;
    if(scount[i]!=0) qp=quarter_profit[i]/scount[i];
    cout << qp << setw(20) << " profit with signal "<<i-3<<"   stocks with signal=" << scount[i] << " \n";
  }
  //cout << quarter_profit/scount << " profit with signal 2 \n";
  cout << "calculate the error for the price\n";
  //cout << "make a distribution of errors\n";
  return 0;
}
