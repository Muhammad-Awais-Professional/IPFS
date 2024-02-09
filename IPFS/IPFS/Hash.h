#include <sha.h>
#include <filters.h>
#include <hex.h>
#include "Conversion.h"
using namespace std;

class HashMap
{
public:
    HashMap() {

    }

    BigBin HashingFunction(string name, int id_space) {
        CryptoPP::SHA1 sha1;
        string source = name;  
        string hash = "";
        CryptoPP::StringSource(source, true, new CryptoPP::HashFilter(sha1, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
        string binHash = hexToBinary(hash);
        int binHash_len = binHash.size() - id_space;

        string str = "";
        for (int i = 0; i < id_space; i++) {
            str += " ";
        }
        int j = 0;
        for (int i = binHash_len; binHash[i] != '\0'; i++) {
            str[j] = binHash[i];
            j++;
        }

        BigBin BinaryHash;
        BinaryHash.binary = str;

        return BinaryHash;
    }

    ~HashMap() {

    }

};
