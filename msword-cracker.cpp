#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "wv.h"

class WordDoc {
public:
    WordDoc(char *filename) {
        int ret = wvInitParser(&ps, filename);
        exists = ret != 3;
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
    bool exists;
    int version;
    
private:
    wvParseStruct ps;
};

#define usage() { \
    printf("Usage: %s -p password file.doc\n", argv[0]); \
    return 1; \
}

int main(int argc, char *argv[]) {
    wvInit();
    
    char action, opt;
    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt) {
            case 'p' : action = opt; break;
            default: usage();
        }
    }
    
    if (optind >= argc - 1) usage();
    char *docfile = argv[++optind];
    WordDoc doc(docfile);
    
    if (doc.exists == false) {
        std::cout << "No such file: " << docfile << std::endl;
    }
    else if (doc.password_protected == false) {
        std::cout << "Success? The document is not encrypted." << std::endl;
        return 1;
    }
    else if (doc.recognized_version == false) {
        std::cout << "File format not recognized" << std::endl;
        return 1;
    }
    else if (action == 'p' && doc.check_password(argv[2]) == false) {
        std::cout << "Incorrect" << std::endl;
        return 1;
    }
    else if (action == 'p') {
        std::cout << "Correct" << std::endl;
        return 0;
    }
}

#undef usage
