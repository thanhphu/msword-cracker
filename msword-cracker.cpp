#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cstring>
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
        int offset = wvStream_tell(ps.tablefd);
        wvSetPassword(password, &ps);
        bool r = false;
        switch (version) {
            case WORD6: case WORD7:
                r = wvDecrypt95(&ps) == 0; break;
            case WORD8:
                r = wvDecrypt97(&ps) == 0; break;
        }
        gsf_input_seek(ps.tablefd->stream.gsf_stream, offset, G_SEEK_SET);
        return r;
    }
    
    bool password_protected;
    bool recognized_version;
    bool exists;
    int version;
    
private:
    wvParseStruct ps;
};

#define usage() { printf( \
        "Usage: %s { -p password | -w wordlist | -c charset } file.doc\n", \
        argv[0] \
    ); \
    return 1; \
}

int main(int argc, char *argv[]) {
    wvInit();
    
    char action, opt;
    char *arg;
    while ((opt = getopt(argc, argv, "p:w:c:")) != -1) {
        switch (opt) {
            case 'p': case 'w': case 'c':
                action = opt;
                arg = optarg;
                break;
            default: usage();
        }
    }
    
    if (optind >= argc) usage();
    char *docfile = argv[optind];
    WordDoc doc(docfile);
    
    if (doc.exists == false) {
        std::cout << "Failed to parse file: " << docfile << std::endl;
    }
    else if (doc.password_protected == false) {
        std::cout << "Success? The document is not encrypted." << std::endl;
        return 1;
    }
    else if (doc.recognized_version == false) {
        std::cout << "File format not recognized" << std::endl;
        return 1;
    }
    else if (action == 'p') {
        if (doc.check_password(arg)) {
            std::cout << "Correct password" << std::endl;
            return 0;
        }
        else {
            std::cout << "Incorrect password" << std::endl;
            return 1;
        }
    }
    else if (action == 'w') {
        std::ifstream fh(arg);
        if (fh == NULL) {
            printf("Failed to open file: %s\n", arg);
            return 1;
        }
        
        std::string line;
        while (std::getline(fh, line)) {
            if (*line.end() == '\n')
                line.resize(line.size() - 1); // chomp
            if (*line.end() == '\r')
                line.resize(line.size() - 1); // chomp again
            
            if (doc.check_password((char *) line.c_str())) {
                std::cout << "The password is: " << line << std::endl;
                return 0;
            }
        }
        std::cout << "Wordlist exausted" << std::endl;
        return 1;
    }
    else if (action == 'c') {
        char *charset = arg;
        int charlen = strlen(charset);
        char offsets[16] = { 0 };
        char password[17] = { 0 };
        while (1) {
            for (int i = 0; ++offsets[i] > charlen; i++)
                offsets[i] = 1;
            
            for (int i = 0; offsets[i]; i++)
                password[i] = charset[offsets[i] - 1];
            
            if (doc.check_password(password)) {
                std::cout << "The password is: " << password << std::endl;
                return 0;
            }
        }
        std::cout << "Passwords only go up to 16 characters "
            "in Microsoft Word documents" << std::endl;
        return 1;
    }
}

#undef usage
