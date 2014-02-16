#include "calc_ey.hpp"
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

std::vector<std::vector<std::pair<std::string,double>>> calc_ey( ///> time(tics(tic,fv)))
std::map< std::string, std::vector< std::map< std::string, double> > > tmd) {
  using namespace std;
  vector<vector<pair<string,double>>> ey
    ((*tmd.begin()).second.size());
  for(auto itic:tmd) {
    for(auto &eyitime:ey) {
      eyitime.push_back(make_pair(itic.first,0.0));
    }
  }
  for(auto itic:tmd) {
    boost::for_each(itic.second,ey,[&](map<string,double> m, vector<pair<string,double>> &facs) {
        double OIADPQ= m["OIADPQ"];
        double PRCCQ = m["PRCCQ"]; 
        double CSHOQ = m["CSHOQ"];
        double AJEXQ = m["AJEXQ"];
        double PSTKQ = m["PSTKQ"];
        double DLTTQ = m["DLTTQ"];
        double DLCQ  = m["DLCQ"];  
        double CHEQ  = m["CHEQ"];  
        double ey_val = OIADPQ*4/(PRCCQ*CSHOQ*AJEXQ + PSTKQ + DLTTQ + DLCQ - CHEQ);
        find_if(facs.begin(),facs.end(),[&](pair<string,double> fac) {return (itic.first==fac.first);})->second=ey_val;
        });
  }
  return ey;
}
  //Earnings Yield = ((OIADPQ_t + OIADPQ_t-1 +OIADPQ_t-2 +OIADPQ_t-3)/((PRCCD*CSHOQ*AJEXQ) + PSTQ +DLTTQ + DLCQ - CHEQ)
  //OIADPQ -- Operating Income After Depreciation - Quarterly
  //PRCCQ -- Price Close - Quarter
  //CSHOQ -- Common Shares Outstanding
  //AJEXQ -- Adjustment Factor (Company) - Cumulative by Ex-Date
  //PSTKQ -- Preferred/Preference Stock (Capital) - Total
  //DLTTQ -- Long-Term Debt - Total
  //DLCQ -- Debt in Current Liabilities
  //CHEQ -- Cash and Short-Term Investments
  //string tic = "AMD";
  //cout << info_by_tic["AMD"].size();
  //cout << "\n";
  //cout << info_by_tic["AMD"].front().size();
  //cout << "\n";
  //for(auto i: info_by_tic) cout << i.first << "  " ;
  ////string  OIADPQ = info_by_tic[tic].front().front();
  //cout << "\n";
  //for(auto i : info_by_tic[tic]) {
    //cout << i.at(mnemonic["OIADPQ"]) << "  ";
  //}
  //string  OIADPQ = info_by_tic[tic].front().at(mnemonic["OIADPQ"]);
  //cout << "OIADPQ " << OIADPQ << "\n";
  //cout << "\n";
  ////
  //ROTCE = ((OIADPQ_t + OIADPQ_t-1 +OIADPQ_t-2 +OIADPQ_t-3)/(PPENTQ + ACTQ - CHEQ - LCTQ +DLCQ)
  //OIADPQ -- Operating Income After Depreciation - Quarterly
  //PPENTQ -- Property Plant and Equipment - Total (Net)
  //ACTQ -- Current Assets - Total
  //CHEQ -- Cash and Short-Term Investments
  //LCTQ -- Current Liabilities - Total
  //DLCQ -- Debt in Current Liabilities
