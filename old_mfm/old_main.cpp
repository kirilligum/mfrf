#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include<boost/tokenizer.hpp>

int main(int argc, char const *argv[]) {
  using namespace std;
  using namespace boost;
  cout << "Hi\n";
  vector<vector<string>> lines;
  ifstream inf(argv[1]);

  //string st((istreambuf_iterator<char>(inf)),istreambuf_iterator<char>());
  //tokenizer<escaped_list_separator<char> > tok(st, escaped_list_separator<char>('\\', '\n'));
  //boost::copy(tok, ostream_iterator<string>(cout,"  "));

  //tokenizer<escaped_list_separator<char> > tok((istream_iterator<char>(inf)),istream_iterator<char>(), escaped_list_separator<char>('\\', '\n'));
  //boost::copy(tok, ostream_iterator<string>(cout,"  "));

  for(string line;getline(inf, line);) if(!inf.eof()) {
    tokenizer<> tok(line);
    //tokenizer<escaped_list_separator<char> > tok(line);
    istringstream ll(line);
   //for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg){
    lines.emplace_back(tok.begin(),tok.end());
    //lines.emplace_back((istream_iterator<string>(ll)), istream_iterator<string>());
  }

  //boost::copy(lines.front(), ostream_iterator<string>(cout,"  "));

  //for(string line;getline(inf, line);) {
    //istringstream iss(line);
    //vector<double> l;
    //double x;
    //if(!inf.eof()) {
      //while(iss>> x) l.push_back(x);
      //lines.push_back(l);
    //}
  //}

  //ofstream of("ns.dat");
  //cout << "size = " << lines.size() << "\n";
  //for(auto i:lines) { 
    //vector<double> nn = {i[0], 0.5*(i[3]*i[3]+i[4]*i[4])-0.35, 0.5*(i[5]*i[5]+i[6]*i[6])-0.35};
    //boost::copy(nn, ostream_iterator<double>(of,"  ")); 
    //of << "\n";
  //}
  return 0;
}
// CC -std=c++11 -I/usr/common/usg/boost/1.54/gnu/include main.cpp -L/usr/common/usg/boost/1.54/gnu/lib -lboost_filesystem -lboost_system -lboost_mpi -lboost_serialization
