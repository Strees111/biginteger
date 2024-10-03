#include <iostream>
#include "biginteger.h"

BigInteger::BigInteger() : data(new unsigned long long[1]{}), size(1) {}

BigInteger::BigInteger(const long long &a) {
    size_t count = 0;
    unsigned long long temp = 0;
    unsigned long long temp1 = 0;
    if (a >= 0) {
        temp = a;
    } else {
        sign = true;
        temp = -a;
    }
    temp1 = temp;
    do {
        ++count;
        temp /= base;
    } while (temp > 0);
    data = new unsigned long long[count]{};
    size = count;
    for (size_t i = 0; i < count; ++i) {
        data[i] = temp1 % base;
        temp1 /= base;
    }
    normalize();
}

BigInteger::BigInteger(const std::string &a) {
    if (a.empty()) {
        throw std::invalid_argument("String empty");
    }
    size_t startIndex = 0;
    if (a[0] == '-') {
        sign = true;
        startIndex = 1;
    } else if (a[0] == '+') {
        startIndex = 1;
    }
    if (startIndex == a.size()) {
        throw std::invalid_argument("String must have digits");
    }
    for (size_t i = startIndex; i < a.size(); ++i) {
        if (!isdigit(a[i])) {
            throw std::invalid_argument("Not a digit");
        }
    }
    int tempSize = a.size() - startIndex;
    unsigned long long *tempDigits = new unsigned long long[tempSize]{};
    for (int i = 0; i < tempSize; ++i) {
        tempDigits[i] = 0;
    }
    for (size_t i = startIndex; i < a.size(); ++i) {
        tempDigits[i - startIndex] = a[i] - '0';
    }

    int tempDataSize = 0;
    unsigned long long *tempData = new unsigned long long[tempSize]{};
    while (tempSize > 0) {
        unsigned long long remainder = 0;
        int newDigitsSize = 0;
        unsigned long long *newDigits = new unsigned long long[tempSize]{};
        for (int i = 0; i < tempSize; ++i) {
            unsigned long long currentValue = remainder * 10 + tempDigits[i];
            if (newDigitsSize > 0 || currentValue >= base) {
                newDigits[newDigitsSize++] = currentValue / base;
            }
            remainder = currentValue % base;
        }
        tempData[tempDataSize++] = remainder;
        delete[] tempDigits;
        tempDigits = newDigits;
        tempSize = newDigitsSize;
    }

    size = tempDataSize;
    data = new unsigned long long[size]{};
    for (size_t i = 0; i < size; ++i) {
        data[i] = tempData[i];
    }
    delete[] tempData;
    delete[] tempDigits;
    normalize();
}


BigInteger::BigInteger(const BigInteger &a) : data(new unsigned long long[a.size]{}), sign(a.sign), size(a.size) {
    memcpy(data, a.data, size * sizeof(unsigned long long));
    normalize();
}

BigInteger::BigInteger(BigInteger &&a) : data(a.data), sign(a.sign), size(a.size) {normalize();}

BigInteger &BigInteger::operator=(const BigInteger &a) {
    if (this != &a) {
        delete[] data;
        data = new unsigned long long[a.size]{};
        sign = a.sign;
        size = a.size;
        memcpy(data, a.data, size * sizeof(unsigned long long));
    }
    normalize();
    return *this;
}

BigInteger &BigInteger::operator=(BigInteger &&a) {
    if (this != &a) {
        delete[] data;
        sign = a.sign;
        size = a.size;
        data = a.data;
        a.data = nullptr;
    }
    normalize();
    return *this;
}

bool operator==(const BigInteger &a1, const BigInteger &a2) {
    return a1.is_equal(a2);
}

BigInteger operator+(const BigInteger &a, const BigInteger &a1) {
    BigInteger res = a;
    res += a1;
    res.normalize();
    return res;
}

BigInteger operator-(const BigInteger &a, const BigInteger &a1) {
    BigInteger res = a;
    res -= a1;
    res.normalize();
    return res;
}

BigInteger operator*(const BigInteger &a, const BigInteger &a1) {
    BigInteger res = a;
    res *= a1;
    res.normalize();
    return res;
}

BigInteger operator/(const BigInteger &a, const BigInteger &a1) {
    BigInteger res = a;
    res /= a1;
    res.normalize();
    return res;
}

BigInteger operator%(const BigInteger &a, const BigInteger &a1) {
    BigInteger res = a;
    res %= a1;
    res.normalize();
    return res;
}

std::string BigInteger::to_string() const {
    std::string result;
    BigInteger temp(*this);
    temp.sign = false;

    while (temp.size > 1 || temp.data[0] != 0) {
        unsigned long long remainder = 0;
        BigInteger newTemp;
        delete[] newTemp.data;
        newTemp.data = new unsigned long long[temp.size]{};
        newTemp.size = temp.size;
        newTemp.sign = false;

        for (int i = static_cast<int>(temp.size - 1); i >= 0; --i) {
            unsigned long long currentValue = (remainder << 32) + temp.data[i];
            newTemp.data[i] = currentValue / 10;
            remainder = currentValue % 10;
        }
        newTemp.normalize();
        temp = newTemp;
        result.push_back(static_cast<char>(remainder + '0'));
    }
    if (size == 1 && data[0] == 0) {
        return "0";
    }
    std::string temp1;
    for (size_t i = 0; i < result.size(); ++i) {
        temp1.push_back(result[result.size() - i - 1]);
    }
    if (sign) {
        temp1.insert(temp1.begin(), '-');
    }
    return temp1;
}

BigInteger BigInteger::operator-() const {
    BigInteger temp = *this;
    temp.sign = !sign;
    temp.normalize();
    return temp;
}

std::string to_string(const BigInteger &a) {
    return a.to_string();
}

BigInteger BigInteger::operator+() {
    BigInteger temp = *this;
    temp.normalize();
    return temp;
}

BigInteger &BigInteger::operator--() {
    *this -= 1;
    normalize();
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger temp = *this;
    *this -= 1;
    temp.normalize();
    return temp;
}

BigInteger &BigInteger::operator++() {
    *this += 1;
    normalize();
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger temp = *this;
    *this += 1;
    temp.normalize();
    return temp;
}

int BigInteger::operator<=>(const BigInteger &a) const {
    if (this->is_less(a)) {
        return -1;
    }
    if (this->is_equal(a)) {
        return 0;
    }
    return 1;
}

BigInteger::~BigInteger() {
    delete[] data;
}

BigInteger &operator+=(BigInteger &a1, const BigInteger &a2) {
    if (a1.data == nullptr || a2.data == nullptr){
        throw "Error";
    }
    if (a1.sign == a2.sign) {
        size_t max = (a1.size > a2.size) ? a1.size : a2.size;
        unsigned long long *result = new unsigned long long[max + 1]{};
        unsigned long long carry = 0;
        size_t i = 0;
        for (; i < max; ++i) {
            unsigned long long sum = carry;
            if (i < a1.size) { sum += a1.data[i]; }
            if (i < a2.size) { sum += a2.data[i]; }
            result[i] = sum % BigInteger::base;
            carry = sum / BigInteger::base;
        }
        if (carry > 0) {
            result[i] = carry;
            ++i;
        }
        delete[] a1.data;
        a1.size = i;
        a1.data = result;
    } else {
        if (a1.abs() >= a2.abs()) {
            a1.minus_min_abs(a2);
        } else {
            BigInteger tmp(a2);
            a1 = tmp.minus_min_abs(a1);
        }
    }
    a1.normalize();
    return a1;
}

BigInteger &operator-=(BigInteger &a1, const BigInteger &a2) {
    a1 += -a2;
    a1.normalize();
    return a1;
}

BigInteger &operator*=(BigInteger &a1, const BigInteger &a2) {
    unsigned long long *result = new unsigned long long[a1.size + a2.size + 1]{};
    for (size_t i = 0; i < a1.size; ++i) {
        unsigned long long carry = 0;
        for (size_t j = 0; j < a2.size; ++j) {
            unsigned long long sum = result[i + j] + a1.data[i] * a2.data[j] + carry;
            result[i + j] = sum % BigInteger::base;
            carry = sum / BigInteger::base;
        }
        result[i + a2.size] += carry;
    }
    delete[] a1.data;
    a1.data = result;
    a1.size = a1.size + a2.size + 1;
    a1.sign = (a1.sign != a2.sign);
    a1.normalize();
    return a1;
}

BigInteger &operator/=(BigInteger &a1, const BigInteger &a2) {
    if (a2.size == 1 && a2.data[0] == 0) {
        throw "Division by zero";
    }

    BigInteger abs_dividend = a1;
    BigInteger abs_divisor = a2;

    bool sign_a1 = a1.sign;
    bool sign_a2 = a2.sign;

    abs_divisor.sign = false;
    abs_dividend.sign = false;

    BigInteger quotient;
    BigInteger remainder;

    size_t bit_count = abs_dividend.size * 32;
    for (int i = static_cast<int>(bit_count) - 1; i >= 0; --i) {
        remainder <<= 1;
        if (abs_dividend.get_bit(i)) {
            remainder.set_bit(0, true);
        }
        if (remainder >= abs_divisor) {
            remainder -= abs_divisor;
            quotient.set_bit(i, true);
        }
    }
    a1 = quotient;
    a1.sign = (sign_a1 != sign_a2);
    a1.normalize();
    return a1;
}

BigInteger &operator%=(BigInteger &a1, const BigInteger &a2) {
    if (a2.size == 1 && a2.data[0] == 0) {
        throw "Division by zero";
    }

    BigInteger abs_dividend = a1;
    BigInteger abs_divisor = a2;

    bool sign_a1 = a1.sign;
    bool sign_a2 = a2.sign;

    abs_divisor.sign = false;
    abs_dividend.sign = false;

    BigInteger quotient;
    BigInteger remainder;

    size_t bit_count = abs_dividend.size * 32;
    for (int i = static_cast<int>(bit_count) - 1; i >= 0; --i) {
        remainder <<= 1;
        if (abs_dividend.get_bit(i)) {
            remainder.set_bit(0, true);
        }
        if (remainder >= abs_divisor) {
            remainder -= abs_divisor;
            quotient.set_bit(i, true);
        }
    }
    a1 = remainder;
    a1.sign = (sign_a1 && !sign_a2);
    a1.normalize();
    return a1;
}

bool BigInteger::is_equal(const BigInteger &other) const {
    bool answer = (sign == other.sign) && (size == other.size);
    for (size_t i = 0; i < size && answer; ++i) {
        if (data[i] != other.data[i]) {
            answer = false;
        }
    }
    return answer;
}

bool BigInteger::is_less(const BigInteger &other) const {
    if (sign && !other.sign) {
        return true;
    }
    if (other.sign && !sign) {
        return false;
    }
    if (sign) {
        return (-other).is_less(-BigInteger(*this));
    }
    if (size < other.size) {
        return true;
    }
    if (other.size < size) {
        return false;
    }

    for (int i = static_cast<int>(size) - 1; i >= 0; --i) {
        if (data == nullptr){
            throw 1;
        }
        if (other.data[i] < data[i]) {

            return false;
        }
        if (data[i] < other.data[i]) {
            return true;
        }
    }
    return false;
}

void BigInteger::normalize() {
    while (size > 1 && data[size - 1] == 0) {
        --size;
    }
    if (size == 1 && data[0] == 0) {
        sign = false;
    }
}

BigInteger &BigInteger::minus_min_abs(const BigInteger &other) {
    int carry = 0;
    size_t max = (size > other.size) ? size : other.size;
    for (size_t i = 0; i < max; ++i) {
        unsigned long long temp = (i < size) ? data[i] : 0;
        unsigned long long temp1 = (i < other.size) ? other.data[i] : 0;
        if (temp < temp1 + carry) {
            data[i] = BigInteger::base - temp1 + temp - carry;
            carry = 1;
        } else {
            data[i] = temp - (temp1 + carry);
            carry = 0;
        }
    }
    normalize();
    return *this;
}

BigInteger BigInteger::abs() const { return sign ? -BigInteger(*this) : BigInteger(*this); }

void BigInteger::set_bit(unsigned long long index, bool value) {
    size_t digit_index = index / 32;
    size_t bit_index = index % 32;

    if (digit_index >= size) {
        unsigned long long *new_data = new unsigned long long[digit_index + 1]{};
        std::copy(data, data + size, new_data);
        delete[] data;
        data = new_data;
        size = digit_index + 1;
    }

    if (value) {
        data[digit_index] |= (1ull << bit_index);
    } else {
        data[digit_index] &= ~(1ull << bit_index);
    }
    normalize();
}

void BigInteger::operator<<=(unsigned long long shift) {
    if (shift == 0) return;

    static const unsigned long long BITS_PER_DIGIT = 32;
    size_t digit_shift = shift / BITS_PER_DIGIT;
    unsigned long long bit_shift = shift % BITS_PER_DIGIT;

    size_t new_size = size + digit_shift + 2;
    unsigned long long *new_data = new unsigned long long[new_size]{};

    for (size_t i = 0; i <= size; ++i) {
        new_data[i + digit_shift] = ((i < size) ? data[i] : 0) << bit_shift;
        if (bit_shift > 0 && (i + digit_shift + 1) < new_size) {
            new_data[i + digit_shift + 1] = ((i < size) ? data[i] : 0) >> (BITS_PER_DIGIT - bit_shift);
        }
    }
    for (size_t i = 0; i < new_size; ++i) {
        if (new_data[i] >= base) {
            if (i + 1 < new_size) {
                new_data[i + 1] += new_data[i] / base;
            }
            new_data[i] %= base;
        }
    }
    delete[] data;
    data = new_data;
    size = new_size;
    normalize();
}


bool BigInteger::get_bit(unsigned long long index) const {
    size_t digit_index = index / 32ull;
    unsigned long long bit_index = index % 32ull;
    if (digit_index >= size) return false;
    return (data[digit_index] >> bit_index) & 1ull;
}