---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../assets/css/copy-button.css" />


# :heavy_check_mark: test/suffix_automaton.test.cpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#098f6bcd4621d373cade4e832627b4f6">test</a>
* <a href="{{ site.github.repository_url }}/blob/master/test/suffix_automaton.test.cpp">View this file on GitHub</a>
    - Last commit date: 2020-08-26 23:10:07+09:00


* see: <a href="https://judge.yosupo.jp/problem/number_of_substrings">https://judge.yosupo.jp/problem/number_of_substrings</a>


## Depends on

* :heavy_check_mark: <a href="../../library/src/suffix_automaton.cpp.html">src/suffix_automaton.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp

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

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "test/suffix_automaton.test.cpp"

#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"

#line 1 "src/suffix_automaton.cpp"

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <queue>
#include <map>

class suffix_automaton {
public:
  using size_type = std::size_t;

private:
  class node_type {
  public:
    size_type link, length;
    std::map<char, size_type> next;

    node_type() = default;
    explicit node_type(const size_type length): 
      link(-1), length(length), next()
    { }
  };

  size_type M_deepest;
  std::vector<node_type> M_node;

public:
  suffix_automaton() { 
    M_node.emplace_back(0); 
    M_deepest = 0;
  }
  explicit suffix_automaton(const std::string &str) {
    M_node.emplace_back(0);
    M_deepest = 0;
    for (const auto c: str) {
      push_back(c);
    }
  }

  void push_back(const char letter) {
    const size_type new_node = M_node.size();
    size_type seek = M_deepest;
    M_deepest = new_node;
    M_node.emplace_back(M_node[seek].length + 1);
    M_node[new_node].link = 0;
    while (~seek && !M_node[seek].next.count(letter)) {
      M_node[seek].next[letter] = new_node;
      seek = M_node[seek].link;
    }
    if (~seek) {
      const size_type current = M_node[seek].next[letter];
      if (M_node[current].length == M_node[seek].length + 1) {
        M_node[new_node].link = current;
      }
      else {
        const size_type split = M_node.size();
        M_node.push_back(M_node[current]);
        M_node[split].length = M_node[seek].length + 1;
        while (~seek && M_node[seek].next[letter] == current) {
          M_node[seek].next[letter] = split;
          seek = M_node[seek].link;
        }
        M_node[current].link = M_node[new_node].link = split;
      }
    }
  }

  std::vector<size_type> sorted() const {
    const size_type size = M_node.size();
    std::vector<size_type> degree(size);
    for (const auto &n: M_node) {
      for (const auto &p: n.next) {
        degree[p.second]++;
      }
    }
    std::vector<size_type> res;
    res.reserve(size);
    std::queue<size_type> que;
    que.push(0);
    while (!que.empty()) {
      const auto cur = que.front();
      que.pop();
      res.push_back(cur);
      for (const auto &p: M_node[cur].next) {
        degree[p.second]--;
        if (degree[p.second] == 0) {
          que.push(p.second);
        }
      }
    }
    return res;
  }

  const node_type &operator [] (const size_type index) const {
    return M_node[index];
  }

};
#line 5 "test/suffix_automaton.test.cpp"

#line 8 "test/suffix_automaton.test.cpp"
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

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

