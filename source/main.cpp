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
  tie(training_data,training_class,sample_data) = get_data_and_class("sp500.csv");
  int ATTRIBUTES_PER_SAMPLE = 2;
  cout << "done\n";
  cout << "set parameters for random forest\n";
        //float priors[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
        //float *priors = vector<float>(16,1.0).data();  // weights of each classification for classes (all equal as equal samples of each digit)

        //CvRTParams params = CvRTParams(25, // max depth
                                       //5, // min sample count
                                       //0, // regression accuracy: N/A here
                                       //false, // compute surrogate split, no missing data
                                       //15, // max number of categories (use sub-optimal algorithm for larger numbers)
                                       //priors, // the array of priors
                                       //false,  // calculate variable importance
                                       //4,       // number of variables randomly selected at node and used to find the best split(s).
                                       //100,	 // max number of trees in the forest
                                       //0.01f,				// forrest accuracy
                                       //CV_TERMCRIT_ITER |	CV_TERMCRIT_EPS // termination cirteria
                                      //);

  cout << "run random forest training \n";
    //Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
    //var_type.setTo(Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical
        CvRTrees* rtree = new CvRTrees;
        //const char *var_type="ord";
        rtree->train(training_data, CV_ROW_SAMPLE, training_class);
        //rtree->train(training_data, CV_ROW_SAMPLE, training_class,
                     //Mat(), Mat(), var_type, Mat(), params);
                     //Mat(), Mat(), var_type, Mat(), params);
  //cout << "make a prediction\n";
  //cout << "calculate the error for the price\n";
  //cout << "make a distribution of errors\n";
  return 0;
}
