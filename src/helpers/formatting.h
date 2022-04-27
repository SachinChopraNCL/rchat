#ifndef FORMATTING_H 
#define FORMATTING_H

#include <stdio.h>


namespace rchat {
    
    inline void line() { printf("--------------------------------");}
    
    inline void linebreak() { printf("\n--------------------------------"); }

    inline void printconsole(const char* message) { printf("[CONSOLE] %s\n", message); }

    inline void printstart(const char* message) { printf("\n[STARTUP] %s", message); }

    inline void printsession(const char* message) { printf("\n[IN-SESSION] %s", message);}

    inline void fprintsession(const char* message, const char* buffer) { printf("\n[IN-SESSION] %s %s", message, buffer);}
    
    inline void printerror(const char* message) { printf("[ERROR] %s", message); }

    inline void printerrori(const char* message, signed int error) {printf("\n[ERROR] %s: %d", message, error);}

    inline void printerrorld(const char* message, long error) {printf("\n[ERROR] %s: %ld", message, error);}

} 

#endif
