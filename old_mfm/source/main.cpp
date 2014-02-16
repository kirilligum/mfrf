#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/lexical_cast.hpp>

#include "file_to_vvst.hpp"
#include "extract_mnemonics.hpp"
//#include "extract_tic.hpp"
#include "extract_tic_factors.hpp"
#include "calc_ey.hpp"
#include "calc_roc.hpp"
#include "rank_tics.hpp"
#include "sort_by_value.hpp"
#include "add_ranks.hpp"
#include "get_mnemonic.hpp"

struct trading_step {
  std::vector<std::pair<std::string,double>> current_portfolio;///> time(tic,num_of_share)
  double operator()(
      std::vector<std::pair<std::string,double>> new_portfolio,
      std::vector<std::pair<std::string,double>> prices
      ) { 
    using namespace std;
    double funds =0.0;
    // 1. cash out
    //cout << "current_portfolio: ";
    //for(auto i:current_portfolio) cout << i.first << "-" << i.second << " ";
    //cout << "\n";
    for(auto i:current_portfolio) 
      funds += i.second*
            find_if(prices.begin(),prices.end(),[&](pair<string,double> j) {return (i.first==j.first);})->second;
    // 2. buy new top
    transform(new_portfolio.begin(),new_portfolio.begin()+current_portfolio.capacity(),begin(current_portfolio),[&](pair<string,double> np){
        return make_pair(np.first,funds/current_portfolio.capacity()/
            find_if(prices.begin(),prices.end(),[&](pair<string,double> j) {return (np.first==j.first);})->second
          );
      });
    return funds;
  }
};

int main(int argc, char const *argv[]) {
  using namespace std;

  //string file_name_csv_data(argv[1]);
  string file_name_csv_data = "in_data/sp500.csv";
  auto lines = file_to_vvst(file_name_csv_data); ///> the first line are names of the columns

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
  ////
  // rank stocks by factors create a data-struct
  sort_by_value(ey);///> BUG some numbers are out of sort
    ofstream oeys("eys.dat"); for (auto i: ey[0]) oeys << i.first << " " << i.second << "\n";
  sort_by_value(roc);
    ofstream orocs("rocs.dat"); for (auto i: roc[0]) orocs << i.first << " " << i.second << "\n";
  // use find to get tic and distance for rank
  //vector<vector<pair<string,double>>> eyr;///> time/tic_by_ey_rank
  // make a vector of pait<tic,vector<rank>>
  auto eyr = rank_tics(ey); ofstream oeyr("eyr.dat"); for (auto i: eyr[0]) oeyr << i.first << " " << i.second << "\n";
  auto rocr = rank_tics(roc); ofstream orocr("rocr.dat"); for (auto i: rocr[0]) orocr << i.first << " " << i.second << "\n";
  // rank stock by the sum of ranks in factors
  auto tr = add_ranks(ey,roc);///> time/tic_by_total_rank
  ofstream otr("tr.dat"); for (auto i: tr[0]) otr << i.first << " " << i.second << "\n";
  sort_by_value(tr); ofstream otrs("trs.dat"); for (auto i: tr[0]) otrs << i.first << " " << i.second << "\n";
  // profit. 

  // backtest:
  // 1. buy top20 (copy into a portfolio vetor)
  // 2. time step
  // 3. sell portfolio vector
  // 4. clear portfolio vector
  // 5. go to 1
  vector<vector<pair<string,double>>> prices = get_mnemonic(tf,"PRCCQ");
    ofstream oprices("prices.dat"); for (auto i: prices[0]) oprices << i.first << " " << i.second << "\n";
  int ntop=20;
  double initial_funds=1e1;
  vector<double> portfolio_value;
  std::vector<std::pair<std::string,double>> current_portfolio;///> time(tic,num_of_share)
  copy(tr.front().end()-ntop,tr.front().end(),back_inserter(current_portfolio));
  cout << "current_portfolio size: " << current_portfolio.size() << "\n";
  for(auto &i: current_portfolio) i.second = initial_funds/ntop;
    ofstream ocp("cp.dat"); for (auto i: current_portfolio) ocp << i.first << " " << i.second << "\n";
  transform(tr.begin(),tr.end(),prices.begin(),back_inserter(portfolio_value),trading_step{current_portfolio});
    //ofstream opv("pv.dat"); for (auto i: portfolio_value) opv << i << "\n";
  vector<double> short_portfolio_value;
  std::vector<std::pair<std::string,double>> short_current_portfolio;///> time(tic,num_of_share)
  copy(tr.front().begin(),tr.front().begin()+ntop,back_inserter(short_current_portfolio));
  for(auto &i: short_current_portfolio) i.second = -initial_funds/ntop;
    ofstream oscp("scp.dat"); for (auto i: short_current_portfolio) oscp << i.first << " " << i.second << "\n";
  transform(tr.begin(),tr.end(),prices.begin(),back_inserter(short_portfolio_value),trading_step{short_current_portfolio});
    ofstream opv("pv.dat"); 
    for (size_t i = 0; i < portfolio_value.size()-1; ++i) {
      opv << portfolio_value[i] 
        << "  " << short_portfolio_value[i] 
        << "  " << portfolio_value[i] - short_portfolio_value[i] 
        << "  " << (portfolio_value[i] - short_portfolio_value[i])/(portfolio_value[0] - short_portfolio_value[0]) 
        <<"\n";
    }
  //
  // pair<date,portfolio_value>
  // pick 50 best stocks. by keep them
  // sell the old portfolio
  // buy the new portfolio
  // do for each timestep
  // plot

  // web interface:
  // ? takes the list of stocks
  // build correlations between stocks and draw an edge-vertice graph
  // do a slider that lights up stocks that are good



  //boost::copy(mnemonic, ostream_iterator<string>(cout,"  "));
  //boost::copy(lines.front(), ostream_iterator<string>(cout,"  "));

  return 0;
}
// CC -std=c++11 -I/usr/common/usg/boost/1.54/gnu/include main.cpp -L/usr/common/usg/boost/1.54/gnu/lib -lboost_filesystem -lboost_system -lboost_mpi -lboost_serialization
