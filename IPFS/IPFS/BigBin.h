#pragma once
#include <iostream>
#include <string>

using namespace std;

class BigBin {
public:
    string binary;

    string addBinary(const string& a, const string& b) {
        string result = "";
        int sum = 0;
        int i = a.size() - 1, j = b.size() - 1;

        while (i >= 0 || j >= 0 || sum == 1) {
            sum += ((i >= 0) ? a[i] - '0' : 0);
            sum += ((j >= 0) ? b[j] - '0' : 0);
            result = char(sum % 2 + '0') + result;
            sum /= 2;
            i--; j--;
        }
        return result;
    }

    string subtractBinary(const string& a, const string& b) {
        string result = "";
        int borrow = 0;
        int i = a.size() - 1, j = b.size() - 1;

        while (i >= 0 || j >= 0) {
            int sub = ((i >= 0) ? a[i] - '0' : 0) - ((j >= 0) ? b[j] - '0' : 0) - borrow;
            if (sub < 0) {
                sub += 2;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
            result = char(sub + '0') + result;
            i--; j--;
        }

        string::size_type it = result.find_first_not_of("0");
        return it != string::npos ? result.substr(it) : "0";
    }

    string multiplyBinary(const string& a, const string& b) {
        string result = "0";

        for (int i = b.length() - 1; i >= 0; --i) {
            if (b[i] == '1') {
                string shiftedA = a;
                for (int j = 0; j < b.length() - 1 - i; ++j) {
                    shiftedA.push_back('0');
                }
                result = addBinary(result, shiftedA);
            }
        }

        string::size_type it = result.find_first_not_of("0");
        return it != string::npos ? result.substr(it) : "0";
    }

public:
    BigBin() : binary("") {}

    BigBin(const string& bin) : binary(bin) {}

    BigBin powerOfTwo(int n) {
        string binary = "1";
        for (int i = 0; i < n; ++i) {
            binary.push_back('0');
        }
        return BigBin(binary);
    }

    BigBin operator+(BigBin& other) {
        return BigBin(addBinary(binary, other.binary));
    }

    BigBin operator-(const BigBin& other) {
        BigBin result = BigBin(subtractBinary(binary, other.binary));

        if (result < BigBin()) {
            BigBin max_Limit = BigBin().powerOfTwo(binary.length());
            result = max_Limit + result;
        }

        return result;
    }

    BigBin operator*(const BigBin& other) {
        return BigBin(multiplyBinary(this->binary, other.binary));
    }
    BigBin& operator=(const BigBin& other) {
        if (this != &other) {
            this->binary = other.binary;
        }
        return *this;
    }
    BigBin& operator=(const std::string& str) {
        this->binary = str;
        return *this;
    }

    bool operator==(const BigBin& other) const {
        return this->binary == other.binary;
    }

    bool operator!=(const BigBin& other) const {
        return !(*this == other);
    }

    bool operator<(const BigBin& other) const {
        std::string this_binary = this->binary;
        std::string other_binary = other.binary;

        if (this_binary.length() < other_binary.length()) {
            this_binary.insert(this_binary.begin(), other_binary.length() - this_binary.length(), '0');
        }
        else if (other_binary.length() < this_binary.length()) {
            other_binary.insert(other_binary.begin(), this_binary.length() - other_binary.length(), '0');
        }

        return this_binary < other_binary;
    }

    bool operator>(const BigBin& other) const {
        return other < *this;
    }

    bool operator<=(const BigBin& other) const {
        return !(*this > other);
    }

    bool operator>=(const BigBin& other) const {
        return !(*this < other);
    }


    friend ostream& operator<<(ostream& out, const BigBin& BigBin) {
        out << BigBin.binary;
        return out;
    }

    friend istream& operator>>(istream& in, BigBin& BigBin) {
        in >> BigBin.binary;
        return in;
    }
};