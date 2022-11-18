
#include "BigInt.h"


void strip_zeroes(std::string & num){
    size_t i;
    for (i = 0; i < num.size(); i++){
        if (num[i] != '0'){
            break;
        }
    }
    if (i == num.size()){
        num = "0";
    }else {
        num = num.substr(i);
    }
}


void add_lead_zeroes(std::string& num, size_t num_zeroes){
    num = std::string(num_zeroes,'0') + num;
}
void add_trail_zeroes(std::string& num, size_t num_zeroes) {
    num += std::string(num_zeroes, '0');
}

BigInt::operator string() const {
    char tmp;
    if (this->is_neg){
        tmp = '-';
    }else{
        tmp = '+';
    }
    return tmp + this->value;
}

BigInt::operator int() const{
    return std::stoi((string)*this);
}

BigInt::operator long long() const {
    return std::stoll(this->value);
}

bool is_valid(const std::string& num){
    for (char digit : num){
        if (digit < '0' or digit > '9'){
            return false;
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& o, const BigInt& a){
    if (a.value.empty()){
        return o;
    }else if(a.value[a.value.size()] == '0'){
        o << 0;
    }
    if (a.is_neg){
        o << '-';
    }
    for (int i = 0; i < a.size(); i++ ){
        o << a.value[i];
    }
    return o;
}
std::istream& operator>>(std::istream& in, BigInt& num) {
    std::string input;
    in >> input;
    num = BigInt(input);

    return in;
}

tuple<BigInt, BigInt> divide(const BigInt& dividend, const BigInt& divisor){
    BigInt quotient, remainder,temp;
    temp = divisor;
    quotient = 1;
    while(temp < dividend){
        ++quotient;
        temp += divisor;
    }
    if (temp > dividend){
        --quotient;
        remainder = dividend - (temp - divisor);
    }
    return make_tuple(quotient, remainder);
}

BigInt abs(const BigInt& num){
    return num.is_neg ? -num: num;
}
bool is_power_of_10(const std::string& num) {
    if (num[0] != '1')
        return false;
    for (size_t i = 1; i < num.size(); i++)
        if (num[i] != '0')
            return false;

    return true;
}


std::string int_to_base(BigInt& num, int base){
    std::string result;

    BigInt bigint = num;
    if (!num.is_neg) {
        while (bigint > 0) {
            BigInt r = bigint % BigInt(base);

            if (r >= BigInt(10))
                result += (char) ((int) (r / BigInt(10)) + 'A');
            else
                result += (char) ((int) (r) + '0');

            bigint /= BigInt(base);
        }
    }else {

    }

    reverse(result.begin(), result.end());

    return result;
}

BigInt pow(const BigInt& base, const int& exp) {
    if (exp < 0) {
        if (base == 0)
            throw std::logic_error("Cannot divide by zero");
        if (abs(base) == 1){
            return base;
        }else{
            return {0};
        }
    }
    if (exp == 0) {
        if (base == 0)
            throw std::logic_error("Zero cannot be raised to zero");
        return {1};
    }
    int tmp = exp;
    BigInt result = base, result_odd = BigInt(1);
        while (tmp > 1) {
        if (tmp % 2)
            result_odd *= result;
        result *= result;
        tmp /= 2;
    }

    return result * result_odd;
}

BigInt base_to_int(const std::string& str, int base){
    BigInt result = BigInt(0);

    for(int i = 0; i < str.length(); i++){
        BigInt add;

        if (i == 0)
            add += BigInt(str[i] - '0');
        else if ('0' <= str[i] && str[i] <= '9')
            add += BigInt(str[i] - '0') * pow(BigInt(base), i);
        else
            add += BigInt((i - 'A') + 10);

        result += add;
    }

    return result;
}