using namespace std;

string hexToBinary(const string& hexString) {
    string binaryString;
    for (char hexChar : hexString) {
        switch (toupper(hexChar)) {
        case '0': binaryString.append("0000"); break;
        case '1': binaryString.append("0001"); break;
        case '2': binaryString.append("0010"); break;
        case '3': binaryString.append("0011"); break;
        case '4': binaryString.append("0100"); break;
        case '5': binaryString.append("0101"); break;
        case '6': binaryString.append("0110"); break;
        case '7': binaryString.append("0111"); break;
        case '8': binaryString.append("1000"); break;
        case '9': binaryString.append("1001"); break;
        case 'A': case 'a': binaryString.append("1010"); break;
        case 'B': case 'b': binaryString.append("1011"); break;
        case 'C': case 'c': binaryString.append("1100"); break;
        case 'D': case 'd': binaryString.append("1101"); break;
        case 'E': case 'e': binaryString.append("1110"); break;
        case 'F': case 'f': binaryString.append("1111"); break;
        default:
            cerr << "Invalid hexadecimal character: " << hexChar << endl;
            return "";
        }
    }
    return binaryString;
}