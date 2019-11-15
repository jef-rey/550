/**
 *  p4- the implementing SAIS
 *  Jeff Anderson
 *  CSCI 550
 *  Fall 2019
 * */
#include <iostream>
#include <string>
#include <vector>

#define L 0
#define S 1

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

void find_type(vector<int> &s_bucket, vector<int> &s_bucket2,
               vector<int> &l_bucket, vector<int> &counts);
void set_type(vector<int> &SA, vector<int> &T, vector<int> &s_bucket,
              vector<bool> &type);

void fill_l(vector<int> &SA, vector<int> &T, vector<int> &l_bucket,
            vector<bool> &type);
void fill_s(vector<int> &SA, vector<int> &T, vector<int> &s_bucket2,
            vector<bool> &type);

void SAIS(vector<int> &T, vector<bool> &type, vector<int> &SA);
bool compare_LMS(const vector<int> &T, const vector<bool> &type, int previous,
                 int current);

int main() {
  // string T = "ROADSTARCAT$";
  string input;
  string t = "";
  while (getline(cin, input)) {
    t = t + input;
  }

  vector<int> T; // the vector we will run SAIS on. converted string of ints
                 // where each char is represented by an int.
  for (int i = 0; i < (int)t.size(); i++) {
    T.push_back(t[i]++);
  }
  T.push_back(0); // this is the equivalent of appending $ to the end

  // put print statement here.

  vector<int> SA(T.size(), -1); // suffix array
  vector<bool> type(T.size());

  SAIS(T, type, SA);

  // //BEGINNING OF STEP 2
  // // give each LMS-substring of T a name

  // printing -- commenting out for now
  for (int i = 0; i < (int)SA.size(); i++) {
    cout << SA[i] << " ";
  }
  cout << endl;

  return 0;
}

// function to find L and S types for SA
void find_type(vector<int> &s_bucket, vector<int> &s_bucket2,
               vector<int> &l_bucket, vector<int> &counts) {
  int total_count = 0;
  for (int i = 0; i < (int)counts.size(); i++) {
    if (counts[i] > 0) { // $ is a special case
      l_bucket[i] = total_count;
      total_count += counts[i];
      s_bucket[i] = total_count - 1;
      s_bucket2[i] = total_count - 1;
    }
  }
}

// function to set types for L and S
void set_type(vector<int> &SA, vector<int> &T, vector<int> &s_bucket,
              vector<bool> &type) {
  // bool L = 0;
  // bool S = 1;
  type[T.size() - 1] = S; // we know the $ is at the end.
  for (int i = (int)type.size() - 2; i >= 0; i--) {
    if (T[i] > T[i + 1]) { // check for types
      type[i] = L;
      if (type[i + 1]) { // if l-type
        SA[s_bucket[T[i + 1]]] = i + 1;
        s_bucket[T[i + 1]]--;
      }
    } else if (T[i] < T[i + 1]) {
      type[i] = S;
    } else {
      type[i] = type[i + 1]; // if type is equal to next type, assign same type
    }
  }
}

void fill_l(vector<int> &SA, vector<int> &T, vector<int> &l_bucket,
            vector<bool> &type) {
  for (int i = 0; i < (int)SA.size(); i++) {
    if ((SA[i] > 0) && (type[SA[i] - 1] == L)) {
      SA[l_bucket[T[SA[i] - 1]]] = SA[i] - 1;
      l_bucket[T[SA[i] - 1]]++;
    }
  }
}

void fill_s(vector<int> &SA, vector<int> &T, vector<int> &s_bucket2,
            vector<bool> &type) {
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
}

void SAIS(vector<int> &T, vector<bool> &type, vector<int> &SA) {

  vector<int> counts(256, 0);

  for (int i = 0; i < (int)T.size(); i++) { // detecting numbers in T
    counts[T[i]]++;
  }

  vector<int> s_bucket(257, -1); // bucket for s-type
  vector<int> s_bucket2(
      257, -1); // bucket for s-type so you don't have to keep resetting
  vector<int> l_bucket(257, -1); // bucket for l-type
  find_type(s_bucket, s_bucket2, l_bucket, counts);

  // setting L-type and S-type and buckets
  set_type(SA, T, s_bucket, type);

  // fill l-buckets
  fill_l(SA, T, l_bucket, type);

  // filling S-buckets
  fill_s(SA, T, s_bucket2, type);

  // beginning of step 2
  vector<int> T_1(SA.size());
  vector<int> N(SA.size(), -1);

  // to figure out bool LMS
  vector<bool> LMS(SA.size(), 0);
  for (int i = 0; i < (int)SA.size(); i++) {
    if (SA[i] > 0 && type[SA[i] - 1] == L && type[SA[i]] == S) {
      // we know that LMS can only occur if previous is L type
      // and current is S type. and b/c you cannot compare T[i] at 0 with
      // T[i-1] we know (by definition) that T[0] will never be a LMS
      LMS[i] = 1;
    }
  }

  int name = 0;
  N[SA[0]] = name; // $ -- base case for N
  name++;          // base case for N
  int previous = SA[0];
  // int current = SA[i] IF LMS

  // scan SA from L-to-R
  for (int i = 1; i < (int)SA.size(); i++) {
    // assign lms-substrings names
    if (LMS[i]) {
      // name++;
      if (!compare_LMS(T, type, previous, SA[i])) {
        name++;
      }

      N[SA[i]] = name;
      previous = SA[i];
    }
  }
}

bool compare_LMS(const vector<int> &T, const vector<bool> &type, int previous,
                 int current) {

  // for (int previous)

  return 0;
}