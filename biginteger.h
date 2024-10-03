#ifndef PROJECT12_BIGINTEGER_H
#define PROJECT12_BIGINTEGER_H

#include <iostream>
#include <limits>
#include <cstring>

class BigInteger {
private:
    const static unsigned long long base = 1ll << 32;
    unsigned long long *data = nullptr;
    bool sign = false;
    size_t size = 0;

    bool is_equal(const BigInteger &other) const;

    bool is_less(const BigInteger &other) const;

    void normalize();

    BigInteger &minus_min_abs(const BigInteger &other);

    BigInteger abs() const;

    void set_bit(unsigned long long index, bool value);

    void operator<<=(unsigned long long  shift);

    bool get_bit(unsigned long long index) const;

public:
    BigInteger();

    BigInteger(const long long &);

    explicit BigInteger(const std::string &);

    BigInteger(const BigInteger &);

    BigInteger(BigInteger &&);

    BigInteger &operator=(const BigInteger &);

    BigInteger &operator=(BigInteger &&);

    friend bool operator==(const BigInteger &a1, const BigInteger &a2);

    friend BigInteger operator+(const BigInteger &a, const BigInteger &a1);

    friend BigInteger operator-(const BigInteger &a, const BigInteger &a1);

    friend BigInteger operator*(const BigInteger &a, const BigInteger &a1);

    friend BigInteger operator/(const BigInteger &a, const BigInteger &a1);

    friend BigInteger operator%(const BigInteger &a, const BigInteger &a1);

    friend BigInteger &operator+=(BigInteger &a1, const BigInteger &a2);

    friend BigInteger &operator-=(BigInteger &a1, const BigInteger &a2);

    friend BigInteger &operator*=(BigInteger &a1, const BigInteger &a2);

    friend BigInteger &operator/=(BigInteger &a1, const BigInteger &a2);

    friend BigInteger &operator%=(BigInteger &a1, const BigInteger &a2);

    BigInteger operator-() const;

    BigInteger operator+();

    BigInteger &operator--();

    BigInteger operator--(int);

    BigInteger &operator++();

    BigInteger operator++(int);

    int operator<=>(const BigInteger &) const;

    std::string to_string() const;

    ~BigInteger();

};

std::string to_string(const BigInteger &a);

#endif
