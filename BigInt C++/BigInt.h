#pragma once
#ifndef OOP_BIGINT_H
#define OOP_BIGINT_H

#include <string>
#include <iostream>

using namespace std;


class BigInt {
public:
    string value;
    bool is_neg{};

    BigInt();
    BigInt(int);
    explicit BigInt(long long);
    explicit BigInt(string); // бросать исключение std::invalid_argument при ошибке
    BigInt(const BigInt&);
    ~BigInt();

    BigInt& operator=(const BigInt&);  //возможно присваивание самому себе!
    BigInt& operator=(const int&);
    BigInt& operator=(const std::string&);
    BigInt& operator=(const long long&);

    BigInt operator~() const;

    BigInt& operator++();
    const BigInt operator++(int);
    BigInt& operator--();
    const BigInt operator--(int);

    BigInt& operator+=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator/=(const BigInt&);
    BigInt& operator^=(const BigInt&);
    BigInt& operator%=(const BigInt&);
    BigInt& operator&=(const BigInt&);
    BigInt& operator|=(const BigInt&);

    BigInt operator+() const;  // unary +
    BigInt operator-() const; // unary -


    bool operator==(const BigInt&) const;
    bool operator==(const int&) const;
    bool operator==(const long long&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator<(const int&) const;
    bool operator>(const BigInt&) const;
    bool operator>(const int&) const;
    bool operator<=(const BigInt&) const;
    bool operator<=(const long long&) const;
    bool operator>=(const BigInt&) const;

    explicit operator int() const;
    explicit operator long long() const;
    explicit operator string() const;



    [[nodiscard]] size_t size() const;  // size in bytes
    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator*(const BigInt&) const;
    BigInt operator/(const BigInt&) const;
    BigInt operator^(const BigInt&) const;
    BigInt operator%(const BigInt&) const;
    BigInt operator%(const int&) const;
    BigInt operator&(const BigInt&) const;
    BigInt operator|(const BigInt&) const;

};
std::ostream& operator<<(std::ostream&, const BigInt&);
std::istream& operator>>(std::istream& , BigInt& );
void add_lead_zeroes(std::string&, size_t);
void strip_zeroes(std::string&);
tuple<BigInt, BigInt> divide(const BigInt&, const BigInt&);
bool is_valid(const std::string&);
BigInt abs(const BigInt&);
void add_trail_zeroes(std::string&, size_t );
bool is_power_of_10(const std::string& );
std::string int_to_base(BigInt&, int);
BigInt base_to_int(const std::string& , int);
BigInt pow(const BigInt&, const int&);
#endif //OOP_BIGINT_H