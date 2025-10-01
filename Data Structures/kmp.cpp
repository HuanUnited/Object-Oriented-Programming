#include <iostream>
#include <vector>
#include <string>

std::vector<int> computeLPSArray(std::string &pattern) {
    int n = pattern.size();
    std::vector<int> lps(n, 0);
        
    // length of the previous longest prefix suffix
    int len = 0;  
    int i = 1;

    while (i < n) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) { 
                // fall back in the pattern
                len = lps[len - 1];  
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    return lps;
}

int main(){
  std::string s = "babbaaaab";
  std::vector<int> lps = computeLPSArray(s);
  for (auto i : lps){
    std::cout << i <<" ";
  }
  std::cout << '\n';
}