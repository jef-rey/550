/**
 *  p5- the implementing SAIS
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

// void SAIS(vector<int> &T, vector<bool> &type, vector<int> &SA);
void SAIS(vector<int> &T, vector<int> &SA);
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

  vector<int> alphabet(256, 0);
  for (int i = 0; i <(int)t.size(); i++){
    alphabet[t[i]] = 1;
  }

  int name = 0;
  vector<int> r_alphabet;// reverse alphabet
  r_alphabet.push_back(-1);//garbage -- 0 is not a valid pos to look at
  for (int i = 0; i < (int)alphabet.size(); i++){
    if (alphabet[i] > 0){
      alphabet[i] = ++name;
      r_alphabet.push_back(i);
    }
  }


  //  for (int i = 0; i < (int)T.size(); i++) { // detecting numbers in T
  //    counts[T[i]]++;
  //  }


  for (int i = 0; i < (int)t.size(); i++) {
    T.push_back(alphabet[t[i]]);

  }
  T.push_back(0); // this is the equivalent of appending $ to the end

  // put print statement here.

  vector<int> SA(T.size(), -1); // suffix array
  // vector<bool> type(T.size());

  // SAIS(T, type, SA);
  SAIS(T, SA);

  // //BEGINNING OF STEP 2
  // // give each LMS-substring of T a name


  // printing BWT
  for (int i = 0; i < (int)SA.size(); i++) {
    if(SA[i] >0){
      //{cout << (char)T[SA[i] -1];}
      cout <<    ( char)r_alphabet[T[SA[i]-1]];
    }
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

void SAIS(vector<int> &T, vector<int> &SA) {

  //vector<int> counts(256, 0);
  int largest_char = 0; 
  // largest char is going to represent the largest character in the alphabet
  // of T, which at this point (in p5) could be greater than the original 256
  // character count. 
  for (int i = 0; i < (int)T.size() ; i++){
    if(T[i] > largest_char){
      largest_char = T[i];
    }
  }

  vector<int> counts(largest_char+1, 0);
  vector<bool> type(T.size());

  for (int i = 0; i < (int)T.size(); i++) { // detecting numbers in T
    counts[T[i]]++;
  }

  int alphabet_size = 0;
  for (int i = 0; i < (int)counts.size(); i ++){
    if(counts[i] > 0){
      alphabet_size++;
    }
  }

  if (alphabet_size  == (int)T.size()) {
    for (int i = 0; i < (int)T.size(); i++) {
      SA[T[i]] = i;
    }
    return;
  }

  //vector<int> s_bucket(257, -1); // bucket for s-type
  vector<int> s_bucket(largest_char+1, -1); // bucket for s-type
  //vector<int> s_bucket2(   257, -1); // bucket for s-type so you don't have to keep resetting
  vector<int> s_bucket2(largest_char+1, -1); // bucket for s-type so you don't have to keep resetting
  //vector<int> l_bucket(257, -1); // bucket for l-type
  vector<int> l_bucket(largest_char+1, -1); // bucket for l-type
  find_type(s_bucket, s_bucket2, l_bucket, counts);

  // setting L-type and S-type and buckets
  set_type(SA, T, s_bucket, type);

  // fill L-buckets
  fill_l(SA, T, l_bucket, type);

  // filling S-buckets
  fill_s(SA, T, s_bucket2, type);

  // beginning of step 2
  vector<int> Tone;
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
  int previous = SA[0];
  // int current = SA[i] IF LMS

  // scan SA from L-to-R
  for (int i = 1; i < (int)SA.size(); i++) {
    // assign lms-substrings names
    if (LMS[i]) {
      if (!compare_LMS(T, type, previous, SA[i])) {
        name++;
      }
      N[SA[i]] = name;
      previous = SA[i];
    }
  }

  for (int i = 0; i < (int)N.size(); i++) {
    if (N[i] != -1) {
      Tone.push_back(N[i]);
    }
  }


  vector<int> SAone(Tone.size());

  SAIS(Tone, SAone);

  find_type(s_bucket, s_bucket2, l_bucket, counts);
  for (int i = 0; i < (int)SA.size(); i++){
    SA[i] = -1;
  }

  int iterator = 0;
  for(int j = 1; j < (int)type.size(); j++){
    if(type[j] == S && type[j-1] == L){
      Tone[iterator] = j;
      iterator++;
    }
  }

  for (int j = (int)SAone.size()-1; j >= 0; j--){
    int p = Tone[SAone[j]];
    SA[s_bucket[T[p]]] = p;
    // cout << "SA[" << s_bucket[T[p]] << "] = " << SA[s_bucket[T[p]]] << endl;
    s_bucket[T[p]]--;
    // cout << "SA in that loop " << SA[s_bucket[T[p]]] << endl;
    // cout << "p: " << p << endl;
    // cout << "j: " << j << endl;
    // cout << "SAone[" << j << "] = " << SAone[j] << endl;
  }

  fill_l(SA, T, l_bucket, type);

  // filling S-buckets
  fill_s(SA, T, s_bucket2, type);
}

bool isStartLMS(int start, int prev) { return start == S && prev == L; }

bool compare_LMS(const vector<int> &T, const vector<bool> &type, int previous,
    int current) {

  // for (int previous)
  // if T[current] and t[prev] are not the same, dump out
  // if T[previous+1] and T[current+1]
  // continue looping  while both previous and current are < T.size
  // inside the loop, we compare both substrings and if they a != immediately
  // return
  // if one of them is the start of a new LMS and other one isnt
  bool flag = true;
  if (T[current] != T[previous]) {
    flag = false;
  }
  for (int i = previous + 1, j = current + 1;
      flag && i < (int)T.size() && j < (int)T.size(); i++, j++) {
    if (T[i] != T[j]) {
      flag = false;
    }
    if (isStartLMS(type[i], type[i - 1]) && isStartLMS(type[j], type[j - 1])) {
      break; // flag is correctly set, and this is where to stop looking
    }
    if (isStartLMS(type[i], type[i - 1]) && !isStartLMS(type[j], type[j - 1])) {
      flag = false;
    }
    if (!isStartLMS(type[i], type[i - 1]) && isStartLMS(type[j], type[j - 1])) {
      flag = false;
    }
  }
  return flag;
}

