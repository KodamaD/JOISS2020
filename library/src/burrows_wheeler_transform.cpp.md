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


# :warning: src/burrows_wheeler_transform.cpp

<a href="../../index.html">Back to top page</a>

* category: <a href="../../index.html#25d902c24283ab8cfbac54dfa101ad31">src</a>
* <a href="{{ site.github.repository_url }}/blob/master/src/burrows_wheeler_transform.cpp">View this file on GitHub</a>
    - Last commit date: 2020-08-25 21:14:55+09:00




## Depends on

* :heavy_check_mark: <a href="suffix_array.cpp.html">src/suffix_array.cpp</a>
* :heavy_check_mark: <a href="suffix_array_induced_sorting.cpp.html">src/suffix_array_induced_sorting.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#pragma once

#include "suffix_array.cpp"

#include <string>

std::string burrows_wheeler_transform(const std::string &str) {
  std::string res;
  res.reserve(str.size() + 1);
  for (const auto x: suffix_array(str)) {
    res.push_back(x == 0 ? '$' : str[x - 1]);
  }
  return res;
}

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 2 "src/burrows_wheeler_transform.cpp"

#line 2 "src/suffix_array.cpp"

#line 2 "src/suffix_array_induced_sorting.cpp"

#include <cstddef>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <utility>

std::vector<std::int32_t> suffix_array_induced_sorting(std::vector<std::int32_t> vec, std::size_t bin_size) {

  const std::size_t size = vec.size();
  vec.push_back(0);

  enum class Type: bool { S, L };
  std::vector<Type> type(size + 1);
  std::vector<std::int32_t> lms;
  type.back() = Type::S;
  for (std::size_t i = size; i-- > 0;) {
    vec[i]++;
    if (vec[i] < vec[i + 1]) {
      type[i] = Type::S;
    }
    else if (vec[i] > vec[i + 1]) {
      type[i] = Type::L;
    }
    else {
      type[i] = type[i + 1];
    }
    if (type[i] == Type::L && type[i + 1] == Type::S) {
      lms.push_back(i + 1);
    }
  }

  bin_size++;
  std::vector<std::size_t> bin(bin_size + 1);
  for (const auto x: vec) {
    bin[x + 1]++;
  }
  for (std::size_t i = 1; i <= bin_size; i++) {
    bin[i] += bin[i - 1];
  }

  std::vector<std::int32_t> array(size + 1);
  const auto induced_sort = [&](const std::vector<std::int32_t> &lms) {
    std::fill(array.begin(), array.end(), -1);
    std::vector<std::size_t> count(bin_size);
    for (const auto x: lms) {
      array[bin[vec[x] + 1] - 1 - count[vec[x]]] = x;
      count[vec[x]]++;
    }
    std::fill(count.begin(), count.end(), 0);
    for (std::size_t i = 0; i <= size; ++i) {
      const std::int32_t x = array[i] - 1;
      if (x >= 0 && type[x] == Type::L) {
        array[bin[vec[x]] + count[vec[x]]] = x;
        count[vec[x]]++;
      }
    }
    std::fill(count.begin(), count.end(), 0);
    for (std::size_t i = size + 1; i-- > 0;) {
      const std::int32_t x = array[i] - 1;
      if (x >= 0 && type[x] == Type::S) {
        array[bin[vec[x] + 1] - 1 - count[vec[x]]] = x;
        count[vec[x]]++;
      }
    }
  };

  induced_sort(lms);
  if (lms.size() > 1) {
    const std::size_t lms_size = lms.size();
    std::vector<std::int32_t> order(size + 1, -1);
    for (std::size_t i = 0; i < lms_size; i++) {
      order[lms[i]] = i;
    }
    std::vector<std::int32_t> lms_idx;
    lms_idx.reserve(lms_size);
    for (const auto x: array) {
      if (order[x] >= 0) {
        lms_idx.push_back(x);
      }
    }
    std::vector<std::int32_t> relabel(lms_size);
    relabel[lms_size - 1 - order[lms_idx[1]]] = 1;
    std::size_t new_bin_size = 1;
    for (std::size_t i = 2; i < lms_size; i++) {
      std::size_t l1 = lms_idx[i];
      std::size_t l2 = lms_idx[i - 1];
      const std::size_t r1 = lms[order[l1] - 1];
      const std::size_t r2 = lms[order[l2] - 1];
      if (r1 - l1 != r2 - l2) {
        new_bin_size++;
      }
      else {
        while (l1 <= r1) {
          if (vec[l1] != vec[l2]) {
            new_bin_size++;
            break;
          }
          l1++;
          l2++;
        }
      }
      relabel[lms_size - 1 - order[lms_idx[i]]] = new_bin_size;
    }
    const auto sa = suffix_array_induced_sorting(std::move(relabel), new_bin_size + 1);
    for (std::size_t i = 1; i <= lms_size; i++) {
      lms_idx[lms_size - i] = lms[lms_size - 1 - sa[i]];
    }
    induced_sort(lms_idx);
  }
  return array;
  
}
#line 4 "src/suffix_array.cpp"

#line 7 "src/suffix_array.cpp"
#include <string>

std::vector<std::size_t> suffix_array(const std::string &str) {
  std::vector<std::int32_t> vec;
  vec.reserve(str.size());
  std::copy(str.cbegin(), str.cend(), std::back_inserter(vec));
  std::vector<std::size_t> res;
  res.reserve(str.size() + 1);
  for (const auto x: suffix_array_induced_sorting(vec, 128)) {
    res.push_back(x);
  }
  return res;
}
#line 4 "src/burrows_wheeler_transform.cpp"

#line 6 "src/burrows_wheeler_transform.cpp"

std::string burrows_wheeler_transform(const std::string &str) {
  std::string res;
  res.reserve(str.size() + 1);
  for (const auto x: suffix_array(str)) {
    res.push_back(x == 0 ? '$' : str[x - 1]);
  }
  return res;
}

```
{% endraw %}

<a href="../../index.html">Back to top page</a>

