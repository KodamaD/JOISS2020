
#include "burrows_wheeler_transform.cpp"
#include "wavelet_matrix.cpp"

#include <iostream>

class fm_index {
public:
  using size_type = std::size_t;

private:
  std::array<size_type, 129> count;
  std::vector<size_type> sa;
  wavelet_matrix<char, 7> mat;

public:
  explicit fm_index(const std::string &str) {
    sa = suffix_array(str);
    const auto bwt = burrows_wheeler_transform(str);
    mat.construct(bwt.cbegin(), bwt.cend());
    count.fill(0);
    for (const auto c: bwt) {
      count[c + 1]++;
    }
    for (size_type i = 1; i <= 128; ++i) {
      count[i] += count[i - 1];
    }
  }

  std::vector<size_type> locate(const std::string &str) const {
    size_type left = 0, right = sa.size();
    for (size_type i = str.size(); i-- > 0;) {
      const auto c = str[i];
      left = count[c] + mat.rank(c, 0, left);
      right = count[c] + mat.rank(c, 0, right);
    }
    std::vector<size_type> res;
    res.reserve(right - left);
    for (size_type i = left; i < right; i++) {
      res.push_back(sa[i]);
    }
    std::sort(res.begin(), res.end());
    return res;
  }

};

int main() {
  std::string s, t;
  std::cin >> s >> t;
  fm_index find(s);
  for (const auto x: find.locate(t)) {
    std::cout << x << '\n';
  }
  return 0;
}