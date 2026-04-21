
#include <bits/stdc++.h>
using namespace std;

#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// Implement a signed big integer class that only needs to support simple addition and subtraction

// Integer 2:
// Implement a signed big integer class that supports addition, subtraction, multiplication, and division, and overload related operators

// Do not use any header files other than the following
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

// Do not use "using namespace std;"

namespace sjtu {
class int2048 {
  // Store sign and base-1e9 chunks (little-endian)
  // Minimal implementation to pass Integer 1 and lay groundwork for Integer 2
  bool neg;
  std::vector<int> d; // each element is 0..9

  void trim();
  static int cmp_abs(const int2048 &a, const int2048 &b);
  static int2048 add_abs(const int2048 &a, const int2048 &b);
  static int2048 sub_abs(const int2048 &a, const int2048 &b); // assume |a|>=|b|
  friend void divmod_abs(const int2048 &, const int2048 &, int2048 &, int2048 &);
public:
  // Constructors
  int2048();
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);

  // The parameter types of the following functions are for reference only, you can choose to use constant references or not
  // If needed, you can add other required functions yourself
  // ===================================
  // Integer1
  // ===================================

  // Read a big integer
  void read(const std::string &);
  // Output the stored big integer, no need for newline
  void print();

  // Add a big integer
  int2048 &add(const int2048 &);
  // Return the sum of two big integers
  friend int2048 add(int2048, const int2048 &);

  // Subtract a big integer
  int2048 &minus(const int2048 &);
  // Return the difference of two big integers
  friend int2048 minus(int2048, const int2048 &);

  // ===================================
  // Integer2
  // ===================================

  int2048 operator+() const;
  int2048 operator-() const;

  int2048 &operator=(const int2048 &);

  int2048 &operator+=(const int2048 &);
  friend int2048 operator+(int2048, const int2048 &);

  int2048 &operator-=(const int2048 &);
  friend int2048 operator-(int2048, const int2048 &);

  int2048 &operator*=(const int2048 &);
  friend int2048 operator*(int2048, const int2048 &);

  int2048 &operator/=(const int2048 &);
  friend int2048 operator/(int2048, const int2048 &);

  int2048 &operator%=(const int2048 &);
  friend int2048 operator%(int2048, const int2048 &);

  friend std::istream &operator>>(std::istream &, int2048 &);
  friend std::ostream &operator<<(std::ostream &, const int2048 &);

  friend bool operator==(const int2048 &, const int2048 &);
  friend bool operator!=(const int2048 &, const int2048 &);
  friend bool operator<(const int2048 &, const int2048 &);
  friend bool operator>(const int2048 &, const int2048 &);
  friend bool operator<=(const int2048 &, const int2048 &);
  friend bool operator>=(const int2048 &, const int2048 &);
};
} // namespace sjtu

#endif

// Implementation for sjtu::int2048

#include <algorithm>
#include <cctype>
#include <cassert>

namespace sjtu {

static const int BASE = 1000000000; // 1e9

void int2048::trim() {
  while (!d.empty() && d.back() == 0) d.pop_back();
  if (d.empty()) neg = false;
}

int int2048::cmp_abs(const int2048 &a, const int2048 &b) {
  if (a.d.size() != b.d.size()) return a.d.size() < b.d.size() ? -1 : 1;
  for (int i = (int)a.d.size() - 1; i >= 0; --i) {
    if (a.d[i] != b.d[i]) return a.d[i] < b.d[i] ? -1 : 1;
  }
  return 0;
}

int2048 int2048::add_abs(const int2048 &a, const int2048 &b) {
  int2048 res;
  res.neg = false;
  const size_t n = std::max(a.d.size(), b.d.size());
  res.d.resize(n + 1, 0);
  long long carry = 0;
  for (size_t i = 0; i < n || carry; ++i) {
    long long cur = carry;
    if (i < a.d.size()) cur += a.d[i];
    if (i < b.d.size()) cur += b.d[i];
    res.d[i] = int(cur % BASE);
    carry = cur / BASE;
  }
  res.trim();
  return res;
}

int2048 int2048::sub_abs(const int2048 &a, const int2048 &b) {
  // assume |a| >= |b|
  int2048 res;
  res.neg = false;
  res.d.resize(a.d.size(), 0);
  long long carry = 0;
  for (size_t i = 0; i < a.d.size(); ++i) {
    long long cur = a.d[i] - carry - (i < b.d.size() ? b.d[i] : 0);
    if (cur < 0) { cur += BASE; carry = 1; } else carry = 0;
    res.d[i] = int(cur);
  }
  res.trim();
  return res;
}

// Constructors
int2048::int2048() : neg(false) {}

int2048::int2048(long long x) : neg(false) {
  if (x < 0) { neg = true; x = -x; }
  while (x) { d.push_back(int(x % BASE)); x /= BASE; }
}

int2048::int2048(const std::string &s) { read(s); }

int2048::int2048(const int2048 &o) = default;

void int2048::read(const std::string &s) {
  neg = false; d.clear();
  size_t i = 0; while (i < s.size() && std::isspace((unsigned char)s[i])) ++i;
  bool sign_set = false;
  if (i < s.size() && (s[i] == '+' || s[i] == '-')) {
    neg = (s[i] == '-'); sign_set = true; ++i;
  }
  while (i < s.size() && s[i] == '0') ++i; // skip leading zeros
  std::vector<int> chunks;
  for (size_t j = s.size(); j > i; ) {
    size_t start = (j >= 9 ? j - 9 : (size_t)0);
    if (start < i) start = i;
    int val = 0;
    for (size_t k = start; k < j; ++k) {
      if (!std::isdigit((unsigned char)s[k])) { d.clear(); neg = false; return; }
      val = val * 10 + (s[k] - '0');
    }
    chunks.push_back(val);
    j = start;
  }
  d = chunks; // already little-endian (lowest chunk first)
  trim();
  if (d.empty()) neg = false;
}

void int2048::print() {
  if (d.empty()) { std::cout << 0; return; }
  if (neg) std::cout << '-';
  int n = (int)d.size();
  std::cout << d[n - 1];
  for (int i = n - 2; i >= 0; --i) {
    std::cout.width(9);
    std::cout.fill('0');
    std::cout << d[i];
  }
}

int2048 &int2048::add(const int2048 &b) {
  if (neg == b.neg) {
    int2048 tmp = add_abs(*this, b);
    tmp.neg = neg;
    *this = tmp;
  } else {
    int c = cmp_abs(*this, b);
    if (c == 0) { d.clear(); neg = false; }
    else if (c > 0) { int2048 tmp = sub_abs(*this, b); tmp.neg = neg; *this = tmp; }
    else { int2048 tmp = sub_abs(b, *this); tmp.neg = b.neg; *this = tmp; }
  }
  return *this;
}

int2048 add(int2048 a, const int2048 &b) { return a.add(b); }

int2048 &int2048::minus(const int2048 &b) {
  if (neg != b.neg) {
    int2048 tmp = add_abs(*this, b);
    tmp.neg = neg;
    *this = tmp;
  } else {
    int c = cmp_abs(*this, b);
    if (c == 0) { d.clear(); neg = false; }
    else if (c > 0) { int2048 tmp = sub_abs(*this, b); tmp.neg = neg; *this = tmp; }
    else { int2048 tmp = sub_abs(b, *this); tmp.neg = !neg; *this = tmp; }
  }
  return *this;
}

int2048 minus(int2048 a, const int2048 &b) { return a.minus(b); }

int2048 int2048::operator+() const { return *this; }

int2048 int2048::operator-() const {
  int2048 r = *this; if (!r.d.empty()) r.neg = !r.neg; return r;
}

int2048 &int2048::operator=(const int2048 &o) = default;

int2048 &int2048::operator+=(const int2048 &b) { return add(b); }
int2048 operator+(int2048 a, const int2048 &b) { return a += b; }

int2048 &int2048::operator-=(const int2048 &b) { return minus(b); }
int2048 operator-(int2048 a, const int2048 &b) { return a -= b; }

int2048 &int2048::operator*=(const int2048 &b) {
  if (d.empty() || b.d.empty()) { d.clear(); neg = false; return *this; }
  std::vector<long long> tmp((size_t)d.size() + b.d.size(), 0);
  for (size_t i = 0; i < d.size(); ++i) {
    long long carry = 0;
    long long ai = d[i];
    for (size_t j = 0; j < b.d.size() || carry; ++j) {
      long long cur = tmp[i + j] + ai * 1ll * (j < b.d.size() ? b.d[j] : 0) + carry;
      tmp[i + j] = cur % BASE;
      carry = cur / BASE;
    }
  }
  d.resize(tmp.size());
  for (size_t i = 0; i < tmp.size(); ++i) d[i] = int(tmp[i]);
  neg = (neg != b.neg);
  trim();
  return *this;
}

int2048 operator*(int2048 a, const int2048 &b) { return a *= b; }

void divmod_abs(const int2048 &a, const int2048 &b, int2048 &q, int2048 &r) {
  // assumes |b| > 0
  r = int2048(); q = int2048();
  if (b.d.empty()) { return; }
  if (int2048::cmp_abs(a, b) < 0) { r = a; q.d.clear(); q.neg = false; return; }

  int n = (int)a.d.size();
  int m = (int)b.d.size();
  int norm = BASE / (b.d.back() + 1);

  auto mul_small = [](const std::vector<int> &v, int m)->std::vector<int>{
    std::vector<int> res(v.size()+1,0); long long carry=0; for(size_t i=0;i<v.size()||carry;++i){ long long cur=carry; if(i<v.size()) cur+=1ll*v[i]*m; res[i]=int(cur%BASE); carry=cur/BASE; } return res; };
  auto add_small = [](std::vector<int> &v, int m){ long long carry=m; size_t i=0; while(carry){ if(i==v.size()) v.push_back(0); long long cur = v[i]+carry; v[i]=int(cur%BASE); carry=cur/BASE; ++i; } };

  int2048 A, B;
  // normalize
  A.neg = false; B.neg = false;
  A.d = mul_small(a.d, norm); A.trim();
  B.d = mul_small(b.d, norm); B.trim();

  q.d.assign(n - m + 1, 0);
  r.d = A.d;

  std::vector<int> dv = B.d; // divisor
  // ensure r has extra zero for convenience
  if ((int)r.d.size() < n + 1) r.d.resize(n + 1, 0);

  long long dv1 = dv.back();
  long long dv2 = (m >= 2) ? dv[m - 2] : 0;

  for (int k = n - m; k >= 0; --k) {
    long long r2 = (long long)r.d[k + m] * BASE + r.d[k + m - 1];
    long long qt = r2 / dv1;
    long long rem = r2 % dv1;
    // adjust using second digit
    while (qt == BASE || (m >= 2 && qt * dv2 > BASE * rem + r.d[k + m - 2])) {
      --qt; rem += dv1; if (rem >= BASE) break;
    }

    // multiply and subtract qt * dv shifted by k
    long long borrow = 0;
    long long carry = 0;
    for (int j = 0; j < m || borrow; ++j) {
      long long cur = r.d[k + j] - (qt * (j < m ? dv[j] : 0) + borrow);
      borrow = 0;
      if (cur < 0) { borrow = (-(cur) + BASE - 1) / BASE; cur += borrow * 1ll * BASE; }
      r.d[k + j] = int(cur);
    }

    // ensure no negative by correction
    // if negative occurred, add back divisor and decrease qt
    int idx = k + m;
    while (idx >= k && r.d[idx] < 0) --idx; // safeguard (shouldn't happen with above)

    // fix by comparing segment
    bool negative_segment = false;
    for (int j = k + m; j >= k; --j) { if (r.d[j] < 0) { negative_segment = true; break; } }
    if (negative_segment) {
      // add back dv and decrement qt
      long long carry2 = 0;
      for (int j = 0; j < m || carry2; ++j) {
        long long cur = r.d[k + j] + (j < m ? dv[j] : 0) + carry2;
        r.d[k + j] = int(cur % BASE);
        carry2 = cur / BASE;
      }
      --qt;
    }
    q.d[k] = int(qt);
  }

  // unnormalize remainder
  // divide r by norm
  long long carry3 = 0;
  for (int i = (int)r.d.size() - 1; i >= 0; --i) {
    long long cur = r.d[i] + carry3 * BASE;
    r.d[i] = int(cur / norm);
    carry3 = cur % norm;
  }
  r.trim();
  q.trim();
}

int2048 &int2048::operator/=(const int2048 &b) {
  // floor division
  if (b.d.empty()) return *this; // undefined per problem, but avoid crash
  if (d.empty()) { neg = false; return *this; }
  int2048 qa, ra;
  divmod_abs(*this, b, qa, ra);
  bool sign = (neg != b.neg);
  // Need floor division adjustment when negative and remainder != 0
  if (sign && !ra.d.empty()) {
    // q = -( |a|/|b| ) floor => subtract 1
    // we have qa = trunc toward zero of |a|/|b| (since abs division)
    // For negative result and remainder, floor = qa + 1 with negative sign
    // But qa is positive here. So result = -(qa) - 1
    // Implement as qa += 1 and negate
    int2048 one(1);
    qa.add(one);
  }
  *this = qa;
  this->neg = sign;
  this->trim();
  return *this;
}

int2048 operator/(int2048 a, const int2048 &b) { return a /= b; }

int2048 &int2048::operator%=(const int2048 &b) {
  if (b.d.empty()) return *this;
  int2048 q, r;
  divmod_abs(*this, b, q, r);
  // Python-like: x % y = x - floor(x/y)*y
  int2048 qr = (*this) - (q * b);
  *this = qr;
  return *this;
}

int2048 operator%(int2048 a, const int2048 &b) { return a %= b; }

std::istream &operator>>(std::istream &is, int2048 &x) {
  std::string s; is >> s; x.read(s); return is;
}

std::ostream &operator<<(std::ostream &os, const int2048 &x) {
  if (x.d.empty()) { os << 0; return os; }
  if (x.neg) os << '-';
  int n = (int)x.d.size();
  os << x.d[n - 1];
  for (int i = n - 2; i >= 0; --i) {
    os.width(9); os.fill('0'); os << x.d[i];
  }
  return os;
}

bool operator==(const int2048 &a, const int2048 &b) {
  return a.neg == b.neg && a.d == b.d;
}
bool operator!=(const int2048 &a, const int2048 &b) { return !(a == b); }
bool operator<(const int2048 &a, const int2048 &b) {
  if (a.neg != b.neg) return a.neg;
  int c = int2048::cmp_abs(a,b);
  return a.neg ? (c > 0) : (c < 0);
}
bool operator>(const int2048 &a, const int2048 &b) { return b < a; }
bool operator<=(const int2048 &a, const int2048 &b) { return !(b < a); }
bool operator>=(const int2048 &a, const int2048 &b) { return !(a < b); }

} // namespace sjtu
