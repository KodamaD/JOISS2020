
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
