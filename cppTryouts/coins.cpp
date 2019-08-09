#include <iostream>
#include <vector>
#include <map>
#include <algorithm>    // std::max
#include <bitset>
#include <string>
#include <set>
#include <deque>
#include <unordered_map>
#include <unordered_set>

using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::string;	
using std::multiset;
using std::deque;
using std::unordered_map;
using std::unordered_set;
class Solution {
public:
    int mCount;
    const int inf = std::numeric_limits<int>::infinity();
    unordered_map<int,int> M;
    int change(vector<int>& coins, int amount){
        
       if(amount == 0) return 0;

       int res = inf;

       for(int i = 0 ; i < coins.size(); ++i){
           if(coins[i] <= amount){
               int val = amount - coins[i];
               int subRes = -1;
               if(M.find(val) != M.end()){
                   subRes = M.find(val)->second;
               }
               else {
                    subRes = change(coins, val);
                    M[val] = subRes;
               }
                
               if(subRes != inf && subRes + 1 < res){
                    res = subRes + 1;
               }
           }
       }    
        return res;
    }
    
    
    int coinChange(vector<int>& coins, int amount) {
        return change(coins,amount); 
        
    }
};

int main() {
  vector<int> coins = {186,419,83,408};
  Solution cS;
  cout << cS.coinChange(coins, 6249) << endl;
 

  return 0;
}
