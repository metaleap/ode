#pragma once
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "utils_libc_deps_basics.c"



#define term_esc "\x1b["



typedef struct termios Termios;


struct {
    struct {
        Termios orig_attrs;
    } term;
} ode;




static void termClearScreen() {
    write(1, term_esc "2J", 2 + 2);
}

static void termPositionCursorAtTopLeftCorner() {
    write(1, term_esc "H", 2 + 1);
}

static void termClearAndPosTopLeft() {
    termClearScreen();
    termPositionCursorAtTopLeftCorner();
}

void odeDie(CStr const hint) {
    termClearAndPosTopLeft();
    perror(hint);
    abort();
}

static void termRawOff() {
    tcsetattr(0, TCSAFLUSH, &ode.term.orig_attrs);
}

static void termRawOn() {
    if (-1 == tcgetattr(0, &ode.term.orig_attrs))
        odeDie("termRawOn: tcgetattr");

    Termios term_attrs = ode.term.orig_attrs;
    term_attrs.c_iflag &= ~(BRKINT     // disable break-condition-to-signal-raising (Ctl+C)
                            | ICRNL    // disable CR-LF so that Enter/Return key sends \n not \r\n
                            | INPCK    // disable parity-checking (not applicable to modern terminals)
                            | ISTRIP   // disable 8th-bit stripping
                            | IXON);   // disable processing of Ctl+S and Ctl+Q so we receive them
    term_attrs.c_oflag &= ~OPOST;      // disable automatic \n-to-\r\n output post-processing
    term_attrs.c_cflag |= CS8;         // set char-size = 8 bits
    term_attrs.c_lflag &= ~(ECHO       // disable automagic echoing of inputs
                            | ICANON   // disable canonical mode (input sent line-by-line instead of byte-by-byte)
                            | ISIG     // disable processing of Ctl+C and Ctl+Z so we receive them
                            | IEXTEN); // disable processing of Ctl+V so we receive it
    term_attrs.c_cc[VMIN] = 0;         // read() returns as soon as an input byte arrives
    term_attrs.c_cc[VTIME] = 1;        // read() times out after 1/10 sec

    if (-1 == tcsetattr(0, TCSAFLUSH, &term_attrs))
        odeDie("termRawOn: tcsetattr");
}

static void termInit() {
    termRawOn();
    atexit(termRawOff);
}

void odeInit() {
    termInit();
}
