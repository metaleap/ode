#pragma once
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "utils_libc_deps_basics.c"



#define term_esc "\x1b["
#define ode_output_screen_max_width 256
#define ode_output_screen_max_height 64
#define ode_output_screen_buf_size (64 * ode_output_screen_max_width * ode_output_screen_max_height)
#define ode_input_buf_size (1 * 1024 * 1024)



typedef struct termios Termios;

typedef struct OdeRgbColor {
    U8 r;
    U8 g;
    U8 b;
} OdeRgbColor;

typedef struct OdeScreenCell {
    struct {
        OdeRgbColor bg;
        OdeRgbColor fg;
    } color;
    U32 content;
} OdeScreenCell;

typedef struct OdeScreenGrid {
    OdeScreenCell cells[ode_output_screen_max_width][ode_output_screen_max_height];
} OdeScreenGrid;

struct {
    struct {
        Termios orig_attrs;
        Bool orig_attrs_got;
        U8 out_buf[ode_output_screen_buf_size];
    } term;
    struct {
        OdeScreenGrid last;
        OdeScreenGrid next;
        struct {
            UInt width;
            UInt height;
        } size;
    } screen;
    struct {
        U8 buf[ode_input_buf_size];
        Bool exit_requested;
    } input;
} ode;




OdeRgbColor rgb(U8 r, U8 g, U8 b) {
    return (OdeRgbColor) {.r = r, .g = g, .b = b};
}

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

static void termRawOff() {
    if (ode.term.orig_attrs_got)
        tcsetattr(0, TCSAFLUSH, &ode.term.orig_attrs);
}

void odeDie(CStr const hint) {
    termRawOff();
    termClearAndPosTopLeft();
    perror(hint);
    abort();
}

static void termRawOn() {
    if (-1 == tcgetattr(0, &ode.term.orig_attrs))
        odeDie("termRawOn: tcgetattr");
    ode.term.orig_attrs_got = true;

    Termios new_attrs = ode.term.orig_attrs;
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

    if (-1 == tcsetattr(0, TCSAFLUSH, &new_attrs))
        odeDie("termRawOn: tcsetattr");
}

static void updateScreenSize() {
    struct winsize win_size = {.ws_row = 0, .ws_col = 0};
    if ((-1 == ioctl(1, TIOCGWINSZ, &win_size)) || (win_size.ws_row == 0) || (win_size.ws_col == 0))
        odeDie("updateScreenSize: ioctl");
    ode.screen.size.width = win_size.ws_col;
    ode.screen.size.height = win_size.ws_row;
}

static void termInit() {
    ode.term.orig_attrs_got = false;
    termRawOn();
    atexit(termClearAndPosTopLeft);
    atexit(termRawOff);
    updateScreenSize();
}

Bool odeProcessInput() {
    Int num_bytes_last_read = read(0, ode.input.buf, ode_input_buf_size);
    if (num_bytes_last_read < 0) {
        if (errno == EAGAIN)
            num_bytes_last_read = 0;
        else
            odeDie("odeProcessInput: read");
    }

    if (num_bytes_last_read != 0) {
        fprintf(stdout, "%zd ", num_bytes_last_read);
        fflush(stdout);
    }

    for (UInt i = 0, max = num_bytes_last_read; i < max; i += 1)
        if (ode.input.buf[i] == (0x1f & 'q'))
            ode.input.exit_requested = true;

    return false;
}

void odeRenderOutput() {
}

void odeInit() {
    { // init global shared mutable state `ode`
        ode.input.exit_requested = false;
        for (UInt i = 0; i < ode_output_screen_buf_size; i += 1)
            ode.term.out_buf[i] = '?';
        for (UInt x = 0; x < ode_output_screen_max_width; x += 1)
            for (UInt y = 0; y < ode_output_screen_max_height; y += 1) {
                ode.screen.last.cells[x][y] = (OdeScreenCell) {.color = {.bg = rgb(0, 0, 0), .fg = rgb(0, 0, 0)}, .content = 0};
                ode.screen.next.cells[x][y] =
                    (OdeScreenCell) {.color = {.bg = rgb(0xef, 0xe9, 0xe8), .fg = rgb(0x68, 0x64, 0x61)}, .content = '?'};
            }
    }
    termInit();
}
