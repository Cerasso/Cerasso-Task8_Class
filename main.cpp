#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cctype> 

using namespace std;

bool isNumber(const string& s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return !s.empty();
}

int main() {
    string s;
    getline(cin, s);
    
    vector<string> vec;
    string temp;
    for (int i = 0; i < s.size(); ++i){
         if(s[i] == ' '){
             vec.push_back(temp);
             temp.clear();
         }else{
             
             temp += s[i];                
         }
    }
    
    if (!temp.empty()) {
        vec.push_back(temp);
    }
    
    std::sort(vec.begin(), vec.end() , [](const std::string& a, const std::string& b)
                                  {
                                    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(),  
                                    [](char c1, char c2) {
                                            return tolower(c1) < tolower(c2);
                                        });
                                  });
    
    for (int i = 0; i < vec.size(); i++){
        if(!isNumber(vec[i]))
            cout << vec[i] << ' ';
        }
}