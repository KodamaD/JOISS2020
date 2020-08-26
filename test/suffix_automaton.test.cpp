
#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"

#include "../src/suffix_automaton.cpp"

#include <cstdint>
#include <string>
#include <iostream>

int main() {
  std::string S;
  std::cin >> S;
  const suffix_automaton sa(S);
  const auto idx = sa.sorted();
  std::vector<std::uint32_t> dp(idx.size());
  dp[idx.front()] = 1;
  std::uint64_t ans = 0;
  for (const auto i: idx) {
    ans += dp[i];
    for (const auto &p: sa[i].next) {
      dp[p.second] += dp[i];
    }
  }
  std::cout << ans - 1 << '\n';
  return 0;
}
