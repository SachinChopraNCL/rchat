#include <stdio.h>

namespace rchat {

    void line() { printf("--------------------------------");}
    
    void linebreak() { printf("\n--------------------------------"); }

    void printstart(const char* message) { printf("\n[STARTUP] %s", message); }

    void printsession(const char* message) { printf("\n[IN-SESSION] %s", message);}

    void fprintsession(const char* message, const char* buffer) { printf("\n[IN-SESSION] %s %s", message, buffer);}
    
    void printerror(const char* message) { printf("\n[ERROR] %s", message); }

    void printerrori(const char* message, signed int error) {printf("\n[ERROR] %s: %d", message, error);}

    void printerrorld(const char* message, long error) {printf("\n[ERROR] %s: %ld", message, error);}
} 
