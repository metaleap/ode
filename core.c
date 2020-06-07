#pragma once
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "utils_libc_deps_basics.c"
#include "common.c"
#include "ui_ctl_panel.c"


typedef struct termios Termios;


#define term_esc "\x1b["
#define ode_output_screen_max_width 256
#define ode_output_screen_max_height 64
#define ode_output_screen_buf_size (64 * ode_output_screen_max_width * ode_output_screen_max_height)
#define ode_input_buf_size (1 * 1024 * 1024)



typedef struct OdeScreenCell {
    OdeColored color;
    OdeGlyphStyleFlags style;
    U32 rune;
    Bool dirty;
} OdeScreenCell;

typedef struct OdeScreenGrid {
    OdeScreenCell cells[ode_output_screen_max_width][ode_output_screen_max_height];
} OdeScreenGrid;

struct Ode {
    struct Init {
        Strs argv_paths;
        struct {
            Termios orig_attrs;
            Bool orig_attrs_got;
        } term;
    } init;
    struct Input {
        U8 buf[ode_input_buf_size];
        UInt n_bytes_read;
        Bool exit_requested;
    } input;
    struct Output {
        U8 buf[ode_output_screen_buf_size];
        struct Screen {
            OdeScreenGrid dst;
            OdeScreenGrid src;
            OdeSize size;
        } screen;
    } output;
    OdeUiCtlPanel ui;
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

static void termAltEnter() {
    write(1, term_esc "?47h", 2 + 4);
    termClearAndPosTopLeft();
}

static void termAltLeave() {
    write(1, term_esc "?47l", 2 + 4);
    termClearAndPosTopLeft();
}

static void termRawOff() {
    if (ode.init.term.orig_attrs_got)
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &ode.init.term.orig_attrs);
}

void odeDie(CStr const hint) {
    termRawOff();
    termAltLeave();
    perror(hint);
    abort();
}

static void termRawOn() {
    if (-1 == tcgetattr(STDIN_FILENO, &ode.init.term.orig_attrs))
        odeDie("termRawOn: tcgetattr");
    ode.init.term.orig_attrs_got = true;

    Termios new_attrs = ode.init.term.orig_attrs;
    new_attrs.c_iflag &= ~(BRKINT     // disable break-condition-to-signal-raising (Ctl+C)
                           | ICRNL    // disable CR-LF so that Enter/Return key sends \n not \r\n
                           | INPCK    // disable parity-checking (not applicable to software terminals)
                           | ISTRIP   // disable 8th-bit stripping
                           | IXON);   // disable processing of Ctl+S and Ctl+Q so we receive them
    new_attrs.c_oflag &= ~OPOST;      // disable automatic \n-to-\r\n output post-processing
    new_attrs.c_cflag |= CS8;         // set char-size = 8 bits
    new_attrs.c_lflag &= ~(ECHO       // disable automagic echoing of inputs
                           | ICANON   // disable canonical mode (input sent line-by-line instead of byte-by-byte)
                           | ISIG     // disable processing of Ctl+C and Ctl+Z so we receive them
                           | IEXTEN); // disable processing of Ctl+V so we receive it
    new_attrs.c_cc[VMIN] = 0;         // read() returns as soon as any amount of input bytes
    new_attrs.c_cc[VTIME] = 1;        // read() times out after 1/10 sec

    if (-1 == tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_attrs))
        odeDie("termRawOn: tcsetattr");
}

static void updateScreenSize() {
    struct winsize win_size = {.ws_row = 0, .ws_col = 0};
    if ((-1 == ioctl(1, TIOCGWINSZ, &win_size)) || (win_size.ws_row == 0) || (win_size.ws_col == 0))
        odeDie("updateScreenSize: ioctl");
    ode.output.screen.size.width = win_size.ws_col;
    ode.output.screen.size.height = win_size.ws_row;
}

static void termInit() {
    ode.init.term.orig_attrs_got = false;
    termRawOn();
    atexit(termAltLeave);
    atexit(termRawOff);
    updateScreenSize();
    termAltEnter();
}

void odeInit() {
    { // init global shared mutable state `ode`
        ode.input.exit_requested = false;
        for (UInt i = 0; i < ode_output_screen_buf_size; i += 1)
            ode.output.buf[i] = '?';
        for (UInt x = 0; x < ode_output_screen_max_width; x += 1)
            for (UInt y = 0; y < ode_output_screen_max_height; y += 1) {
                ode.output.screen.dst.cells[x][y] =
                    (OdeScreenCell) {.rune = 0,
                                     .dirty = false,
                                     .style = ode_glyphstyle_none,
                                     .color = {.bg = rgba(0x00, 0x00, 0x00, 0xff), .fg = rgba(0x00, 0x00, 0x00, 0xff)}};
                ode.output.screen.src.cells[x][y] =
                    (OdeScreenCell) {.rune = '?',
                                     .dirty = false,
                                     .style = ode_glyphstyle_none,
                                     .color = {.bg = rgba(0x39, 0x32, 0x30, 0xff), .fg = rgba(0xb8, 0xb0, 0xa8, 0xff)}};
            }
    }
    termInit();
}
