// proj4
// jeff anderson
#include <iostream>
#include <string>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

// using namespace std;

int main() {
  // string T = "ROADSTARCAT$";
  string input;
  string t= "";
  while (getline(cin, input)) {
    t = t + input;
 }
  vector<int> counts(257, 0);
  vector<int> T;// the vector we will run SAIS on
  for (int i = 0; i < (int)t.size(); i++){
    T.push_back(t[i]++);
  }
  // T.append("\0");
  T.push_back(0);
  // string Str("");
  for (int i = 0; i < (int)T.size(); i++) { // detecting numbers in T
    counts[T[i]]++;
    // Str = counts[T[i]];
  }

  vector<int> s_bucket(257, -1);
  vector<int> s_bucket2(257, -1);
  vector<int> l_bucket(257, -1);
  //   int i = 0;
  int total_count = 0; // switch to 1 if newnumber++
  for (int i = 0; i < (int)counts.size(); i++) {
    if (counts[i] > 0) { // $ is a special case
      l_bucket[i] = total_count;
      total_count += counts[i];
      s_bucket[i] = total_count - 1;
      s_bucket2[i] = total_count - 1;
    }
  }
  vector<int> SA(T.size(), -1);

  // setting L-type and S-type and buckets
  vector<bool> type(T.size());
  bool L = 0;
  bool S = 1;
  type[T.size() - 1] = S; // we know the $ is at the end.
  for (int i = (int)type.size() - 2; i >= 0; i--) {
    if (T[i] > T[i + 1]) {
      type[i] = L;
      if (type[i + 1]) {
        SA[s_bucket[T[i + 1]]] = i + 1;
        s_bucket[T[i + 1]]--;
      }
    } else if (T[i] < T[i + 1]) {
      type[i] = S;
    } else {
      type[i] = type[i + 1];
    }
  }

  for (int i = 0; i < (int)SA.size(); i++) {
    if ((SA[i] > 0) && (type[SA[i] - 1] == L)) {
      SA[l_bucket[T[SA[i] - 1]]] = SA[i] - 1;
      l_bucket[T[SA[i] - 1]]++;
    }
  }

  for (int i = (int)SA.size() - 1; i >= 0; i--) {
    if (SA[i] == 0) {
      if (type[T.size() - 1] == S) {
        SA[s_bucket2[T[T.size() - 1]]] = T.size() - 1;
        s_bucket2[T[T.size() - 1]]--;
      }
    } else if (SA[i] != -1) {
      if (type[SA[i] - 1] == S) {
        SA[s_bucket2[T[SA[i] - 1]]] = SA[i] - 1;
        s_bucket2[T[SA[i] - 1]]--;
      }
    }
  }

  for (int i = 0; i < (int)SA.size(); i++) {
    cout << SA[i] << " ";
  }
  cout << endl;

  return 0;
}

