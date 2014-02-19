#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

int main(int argc, char const *argv[]) {
  cout << " hi \n";
  int p = 2;
  int d = 2;
  float trainingData[p][d];
  for (size_t i = 0; i < p/2; ++i) {
    for (size_t j = 0; j < d; ++j) {
      trainingData[i][j] = 0.0;
    }
  }
  for (size_t i = p/2; i < p; ++i) {
    for (size_t j = 0; j < d; ++j) {
      trainingData[i][j] = 1.0;
    }
  }
  Mat training_data(p, d, CV_32FC1, trainingData);
  cout << training_data << endl;
  float trainingClass[p];
  for (size_t i = 0; i < p/2; ++i) {
    trainingClass[i] = 0;
  }
  for (size_t i = p/2; i < p; ++i) {
    trainingClass[i] = 1;
  }
  Mat training_class(p, 1, CV_32FC1, trainingClass);
  cout << training_class << endl;
  CvRTrees rtrees;
  CvRTParams  params( 4, // max_depth,
                      1, // min_sample_count,
                      0.f, // regression_accuracy,
                      false, // use_surrogates,
                      2, // max_categories,
                      0, // priors,
                      false, // calc_var_importance,
                      1, // nactive_vars,
                      5, // max_num_of_trees_in_the_forest,
                      0, // forest_accuracy,
                      CV_TERMCRIT_ITER // termcrit_type
                     );

  rtrees.train(training_data, CV_ROW_SAMPLE, training_class, Mat(), Mat(), Mat(), Mat(), params );
  //rtrees.train(training_data, CV_ROW_SAMPLE, training_class);
  rtrees.save("rtrees.yml");
  float sampleData[d];
  for (size_t i = 0; i < d; ++i) {
    sampleData[i]=1.0;
  }
  Mat sample_data(1, d, CV_32FC1, sampleData);
  cout << sample_data << endl;
  cout << rtrees.predict(sample_data) << "  <-- predict\n";
  return 0;
}
