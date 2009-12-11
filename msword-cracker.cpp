#include <iostream>
#include <cstdio>
#include "wv.h"

class WordDoc {
public:
    WordDoc(char *filename) {
        int ret = wvInitParser(&ps, filename);
        version = ret & 0x7fff;
        recognized_version =
            version == WORD6 ||
            version == WORD7 ||
            version == WORD8;
        password_protected = ret & 0x8000;
    }
    
    bool check_password(char *password) {
        wvSetPassword(password, &ps);
        switch (version) {
            case WORD6: case WORD7:
                return wvDecrypt95(&ps) == 0;
            case WORD8:
                return wvDecrypt97(&ps) == 0;
            default:
                return false;
        }
    }
    
    bool password_protected;
    bool recognized_version;
    int version;
    
private:
    wvParseStruct ps;
};

int main(int argc, char *argv[]) {
    wvInit();
    
    if (argc != 3) {
        printf("Usage: %s file password\n", argv[0]);
        return 1;
    }
    
    WordDoc doc(argv[1]);
    if (doc.password_protected == false) {
        std::cout << "Success? The document is not encrypted." << std::endl;
        return 1;
    }
    else if (doc.recognized_version == false) {
        std::cout << "File format not recognized" << std::endl;
        return 1;
    }
    else if (doc.check_password(argv[2]) == false) {
        std::cout << "Incorrect" << std::endl;
        return 1;
    }
    else {
        std::cout << "Correct" << std::endl;
        return 0;
    }
}
