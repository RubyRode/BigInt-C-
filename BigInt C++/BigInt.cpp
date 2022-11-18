#include <string>
#include <cmath>
#include <climits>

#include "BigInt.h"


BigInt::BigInt(): is_neg(false) {}


BigInt::BigInt(int num) {
    value = std::to_string(std::abs(num));
    if (num < 0){
        is_neg = true;
    }else{
        is_neg = false;
    }
}

BigInt::BigInt(std::string num) {
    if (num[0] == '+' or num[0] == '-'){
        std::string mag = num.substr(1);
        if (is_valid(mag)){
            value = mag;
            if (num[0] == '+') {
                is_neg = false;
            }else{
                is_neg = true;
            }
        }else{
            throw std::invalid_argument("Integer expected, got \'" + num + "\'");
        }
    }else{
        if (is_valid(num)){
            value = num;
            is_neg = false;
        }else{
            throw std::invalid_argument("Integer expected, got \'" + num +"\'");
        }
    }
    strip_zeroes(value);
}

BigInt::BigInt(const BigInt & num) {
    value = num.value;
    is_neg = num.is_neg;
}

bool BigInt::operator==(const BigInt & num) const {
    return (is_neg == num.is_neg) and (value == num.value);
}

bool BigInt::operator!=(const BigInt & num) const {
    return !(*this == num);
}

bool BigInt::operator<(const BigInt & num) const {
    if (this->value.size() < num.value.size() and this->is_neg == num.is_neg) {
        return true;
    }
    else if (this->value.size() == num.value.size()){
        int cnt = 0;
        int i;
        for (i = 0; i < this->value.size(); i++){
            if (this->value[i] < num.value[i]){
                cnt ++;
            }
        }
        if (cnt == i){
            return true;
        }
    }
    return false;
}

bool BigInt::operator>(const BigInt & num) const {
    if ((*this != num) && !(*this < num)){
        return true;
    }
    return false;
}

bool BigInt::operator<=(const BigInt & a) const {
    if ((*this == a) || (*this < a)){
        return true;
    }
    return false;
}

bool BigInt::operator<=(const long long& a) const {
    bool tmp = *this > BigInt(a);
    return !(*this > BigInt(a));
}

bool BigInt::operator>=(const BigInt & a) const {
    if ((*this == a) || (*this > a)){
        return true;
    }
    return false;
}


BigInt &BigInt::operator+=(const BigInt & num) {
    *this = *this + num;
    return *this;
}

BigInt &BigInt::operator*=(const BigInt &num) {
    *this = *this * num;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& num) {
    *this = *this - num;
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& num) {
    *this = *this / num;
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& num) {
    *this = *this % num;
    return *this;
}

BigInt &BigInt::operator++() {

    return *this += BigInt(1);
}

const BigInt BigInt::operator++(int){
    BigInt temp = *this;
    ++(*this);
    return temp;
}

BigInt &BigInt::operator--() {
    *this -= BigInt(1);
    return *this;
}

const BigInt BigInt::operator--(int){
    BigInt temp = *this;
    --(*this);
    return temp;
}

BigInt BigInt::operator+() const {
    return *this;
}

BigInt BigInt::operator-() const {
    BigInt temp;
    temp.value = value;
    if (value != "0") {
        if (is_neg)
            temp.is_neg = false;
        else
            temp.is_neg = true;
    }
    return temp;
}

size_t BigInt::size() const {
    return this->value.size();
}

BigInt &BigInt::operator=(const int & num) {
    BigInt temp(num);
    value = temp.value;
    is_neg = temp.is_neg;
    return *this;
}

BigInt::BigInt(long long int num) {
    value = std::to_string(std::abs(num));
    if (num < 0)
        is_neg = true;
    else
        is_neg = false;
}

BigInt &BigInt::operator=(const long long int & num) {
    BigInt temp(num);
    value = temp.value;
    is_neg = temp.is_neg;

    return *this;
}

bool BigInt::operator==(const long long int &num) const {
    return *this == BigInt(num);
}

bool BigInt::operator<(const int &num) const {
    return *this < BigInt(num);
}

bool BigInt::operator==(const int &num) const {
    return *this == BigInt(num);
}

bool BigInt::operator>(const int &num) const {
    return *this > BigInt(num);
}



BigInt BigInt::operator ~() const {
    std::string s1 = this->value;

    for (char & i : s1) {
        bool b1 = i == '1';
        i = b1 ? '0' : '1';
    }
    strip_zeroes(s1);
    return BigInt(s1);
}

BigInt &BigInt::operator^=(const BigInt &num) {
    *this = *this ^ num;
    return *this;
}

BigInt &BigInt::operator&=(const BigInt &num) {
    *this = *this & num;
    return *this;
}

BigInt &BigInt::operator|=(const BigInt &num) {
    *this = *this | num;
    return *this;
}

BigInt &BigInt::operator=(const string &num) {
    BigInt temp = BigInt(num);
    *this = temp;
    return *this;
}



BigInt & BigInt::operator=(const BigInt & a)= default;

BigInt::~BigInt() = default;
