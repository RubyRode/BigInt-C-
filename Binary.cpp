
#include "BigInt.h"
const long long FLOOR_SQRT_LLONG_MAX = 3037000499;
BigInt BigInt::operator+(const BigInt &num) const {
    if (this->is_neg && !num.is_neg){
        BigInt lhs = *this; //a, this
        lhs.is_neg = false;
        return num - lhs;
    } else if (!this->is_neg && num.is_neg){
        BigInt rhs = num;
        rhs.is_neg = false;
        return *this - rhs;
    }
    long larg_size;
    string larger, smaller;
    if (*this > num){
        larg_size = (long)this->size();
        tie(larger,smaller) = make_tuple(this->value, num.value);
    }else{
        tie(larger, smaller) = make_tuple(num.value, this->value);
        larg_size = (long)num.size();
    }

    BigInt temp;
    temp.value = "";
    short carry = 0, sum;
    add_lead_zeroes(smaller, larg_size - smaller.size());
    for (int ix = larg_size - 1; ix >= 0; ix--){
        sum = (short)(larger[ix] - '0' + smaller[ix] - '0' + carry);
        temp.value = to_string(sum%10) + temp.value;
        carry = (short)(sum/(short)10);
    }
    if (carry){
        temp.value = to_string(carry) + temp.value;
    }
    if (this->is_neg and temp.value != "0"){
        temp.is_neg = true;
    }
    return temp;

}

BigInt BigInt::operator-(const BigInt& num) const {
    if (!this->is_neg && num.is_neg){
        BigInt rhs = num;
        rhs.is_neg = false;
        return *this + rhs;
    }else if (this->is_neg && !num.is_neg){
        BigInt lhs = *this;
        lhs.is_neg = false;
        return -(lhs + num);
    }
    BigInt result;
    string larger, smaller;
    if (abs(*this) > abs(num)){
        larger = this->value;
        smaller = num.value;
        if (this->is_neg){
            result.is_neg = true;
        }
    }else {
        larger = num.value;
        smaller = this->value;
        if (!num.is_neg){
            result.is_neg = true;
        }
    }
    add_lead_zeroes(smaller, larger.size() - smaller.size());
    result.value = "";
    short difference;
    long i,j;
    for (i = (long)larger.size() - 1; i >= 0; i--){
        difference = short(larger[i] - smaller[i]);
        if (difference < 0){
            for (j = i-1; j >= 0; j--){
                if (larger[j] != '0'){
                    larger[j]--;
                    break;
                }
            }
            j++;
            while(j != i){
                larger[j] = '9';
                j++;
            }
            difference += 10;
        }
        result.value = to_string(difference) + result.value;
    }
    strip_zeroes(result.value);
    if (result.value == "0"){
        result.is_neg = false;
    }
    return result;
}

BigInt BigInt::operator*(const BigInt& num) const {
    if (*this == BigInt(0) or num == BigInt(0))
        return BigInt(0);
    if (*this == BigInt(1))
        return num;
    if (num == BigInt(1))
        return *this;

    BigInt product;
    if (abs(*this) <= FLOOR_SQRT_LLONG_MAX and abs(num) <= FLOOR_SQRT_LLONG_MAX)
        product = std::stoll(this->value) * std::stoll(num.value);
    else if (is_power_of_10(this->value)) {
        product.value = num.value;
        product.value.append(this->value.begin() + 1, this->value.end());
    } else if (is_power_of_10(num.value)) {
        product.value = this->value;
        product.value.append(num.value.begin() + 1, num.value.end());
    } else {
        std::string larger, smaller;
        if (abs(*this) > abs(num)){
            larger = this->value;
            smaller = num.value;
            if (this->is_neg){
                product.is_neg = true;
            }
        }else {
            larger = num.value;
            smaller = this->value;
            if (!num.is_neg){
                product.is_neg = true;
            }
        }
        size_t half_length = larger.size() / 2;
        auto half_length_ceil = (size_t) ceil(larger.size() / 2.0);

        BigInt num1_high, num1_low;
        num1_high = BigInt(larger.substr(0, half_length));
        num1_low = BigInt(larger.substr(half_length));
        add_lead_zeroes(smaller, larger.size() - smaller.size());
        BigInt num2_high, num2_low;
        num2_high = BigInt(smaller.substr(0, half_length));
        num2_low = BigInt(smaller.substr(half_length));

        strip_zeroes(num1_high.value);
        strip_zeroes(num1_low.value);
        strip_zeroes(num2_high.value);
        strip_zeroes(num2_low.value);

        BigInt prod_high, prod_mid, prod_low;
        prod_high = num1_high * num2_high;
        prod_low = num1_low * num2_low;
        prod_mid = (num1_high + num1_low) * (num2_high + num2_low) - prod_high - prod_low;

        add_trail_zeroes(prod_high.value, 2 * half_length_ceil);
        add_trail_zeroes(prod_mid.value, half_length_ceil);

        strip_zeroes(prod_high.value);
        strip_zeroes(prod_mid.value);
        strip_zeroes(prod_low.value);

        product = prod_high + prod_mid + prod_low;
    }
    strip_zeroes(product.value);

    if (this->is_neg == num.is_neg)
        product.is_neg = false;
    else
        product.is_neg = true;

    return product;
}

BigInt BigInt::operator/(const BigInt& num) const {
    BigInt abs_dividend = abs(*this);
    BigInt abs_divisor = abs(num);

    if (num == 0)
        throw std::logic_error("Attempted division by zero");
    if (abs_dividend < abs_divisor)
        return BigInt(0);
    if (num == 1)
        return *this;
    if (num == -1)
        return -(*this);

    BigInt quotient;
    if (abs_dividend <= LLONG_MAX and abs_divisor <= LLONG_MAX)
        quotient = std::stoll(abs_dividend.value) / std::stoll(abs_divisor.value);
    else if (abs_dividend == abs_divisor)
        quotient = 1;
    else if (is_power_of_10(abs_divisor.value)) { // if divisor is a power of 10 do optimised calculation
        size_t digits_in_quotient = abs_dividend.value.size() - abs_divisor.value.size() + 1;
        quotient.value = abs_dividend.value.substr(0, digits_in_quotient);
    }
    else {
        quotient.value = "";    // the value is cleared as digits will be appended
        BigInt chunk, chunk_quotient, chunk_remainder;
        size_t chunk_index = 0;
        chunk_remainder.value = abs_dividend.value.substr(chunk_index, abs_divisor.value.size() - 1);
        chunk_index = abs_divisor.value.size() - 1;
        while (chunk_index < abs_dividend.value.size()) {
            chunk.value = chunk_remainder.value.append(1, abs_dividend.value[chunk_index]);
            chunk_index++;
            while (chunk < abs_divisor) {
                quotient.value += "0";
                if (chunk_index < abs_dividend.value.size()) {
                    chunk.value.append(1, abs_dividend.value[chunk_index]);
                    chunk_index++;
                }
                else
                    break;
            }
            if (chunk == abs_divisor) {
                quotient.value += "1";
                chunk_remainder = 0;
            }
            else if (chunk > abs_divisor) {
                strip_zeroes(chunk.value);
                std::tie(chunk_quotient, chunk_remainder) = divide(chunk, abs_divisor);
                quotient.value += chunk_quotient.value;
            }
        }
    }
    strip_zeroes(quotient.value);

    if (this->is_neg == num.is_neg)
        quotient.is_neg = false;
    else
        quotient.is_neg = true;

    return quotient;
}

BigInt BigInt::operator%(const BigInt &num) const {
    BigInt abs_dividend = abs(*this);
    BigInt abs_divisor = abs(num);

    if (abs_divisor == 0)
        throw std::logic_error("Attempted division by zero");
    if (abs_divisor == 1 or abs_divisor == abs_dividend)
        return BigInt(0);

    BigInt remainder;
    if (abs_dividend <= LLONG_MAX and abs_divisor <= LLONG_MAX)
        remainder = std::stoll(abs_dividend.value) % std::stoll(abs_divisor.value);
    else if (abs_dividend < abs_divisor)
        remainder = abs_dividend;
    else if (is_power_of_10(num.value)){
        size_t no_of_zeroes = num.value.size() - 1;
        remainder.value = abs_dividend.value.substr(abs_dividend.value.size() - no_of_zeroes);
    }
    else {
        BigInt quotient = abs_dividend / abs_divisor;
        remainder = abs_dividend - quotient * abs_divisor;
    }
    strip_zeroes(remainder.value);


    remainder.is_neg = this->is_neg;
    if (remainder.value == "0")
        remainder.is_neg = false;

    return remainder;
}

BigInt BigInt::operator%(const int &num) const {
    return *this % BigInt(num);
}


BigInt BigInt::operator^(const BigInt& num) const{
    BigInt tmp = *this;
    std::string first = int_to_base(tmp, 2);
    tmp = num;
    std::string second = int_to_base(tmp, 2);

    unsigned int n = std::max(first.size(), second.size());

    reverse(first.begin(), first.end());
    reverse(second.begin(), second.end());

    std::string result(n, '~');

    for (int i = (int)n-1; i >=0 ; i--) {
        if (first.size() <= i || second.size() <= i) {
            if (first.size() > i) {
                if (first[i] == '0')
                    result[i] = '0';
                else
                    result[i] = '1';
            }

            if (second.size() > i) {
                if (second[i] == '0')
                    result[i] = '0';
                else
                    result[i] = '1';
            }

            continue;
        }

        if (first[i] == second[i])
            result[i] = '0';
        else
            result[i] = '1';
    }

    BigInt res = BigInt(base_to_int(result, 2));
    return res;
}

BigInt BigInt::operator&(const BigInt &second_) const {
    BigInt tmp = *this;
    std::string first = int_to_base(tmp, 2);
    tmp = second_;
    std::string second = int_to_base(tmp, 2);

    unsigned int n = std::min(first.size(), second.size());

    reverse(first.begin(), first.end());
    reverse(second.begin(), second.end());

    std::string result(n, '~');

    for(int i = (int)n - 1; i >= 0; i--){
        if(first[i] == '1' && second[i] == '1')
            result[i] = '1';
        else
            result[i] = '0';
    }

    BigInt res = BigInt(base_to_int(result, 2));
    return res;
}

BigInt BigInt::operator|(const BigInt& second_) const{
    BigInt tmp = *this;
    std::string first = int_to_base(tmp, 2);
    tmp = second_;
    std::string second = int_to_base(tmp, 2);

    unsigned int n = std::max(first.size(), second.size());

    reverse(first.begin(), first.end());
    reverse(second.begin(), second.end());

    std::string result(n, '~');

    for(int i = (int)n-1; i >= 0; i--){
        if(first.size() <= i || second.size() <= i){
            if(first.size() > i) result[i] = first[i];
            if(second.size() > i) result[i] = second[i];

            continue;
        }

        if(first[i] == '1' || second[i] == '1')
            result[i] = '1';
        else
            result[i] = '0';
    }

    BigInt res = BigInt(base_to_int(result, 2));
    return res;
}
