
#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_B"

#include "../src/fm_index.cpp"

#include <string>
#include <iostream>

int main() {
  std::string S;
  std::cin >> S;
  const fm_index fm(S);
  std::string T;
  std::cin >> T;
  auto answer = fm.locate(T);
  std::sort(answer.begin(), answer.end());
  for (const auto x: answer) {
    std::cout << x << '\n';
  }
  return 0;
}
