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

typedef struct OdeRgbaColor {
    U8 r;
    U8 g;
    U8 b;
    U8 a;
} OdeRgbaColor;

typedef struct OdeScreenCell {
    U32 rune;
    struct Color {
        OdeRgbaColor bg;
        OdeRgbaColor fg;
    } color;
    struct StyleBits {
        Bool bold : 1;
        Bool italic : 1;
        Bool underline : 1;
        Bool strikethru : 1;
    } style;
    Bool dirty;
} OdeScreenCell;

typedef struct OdeScreenGrid {
    OdeScreenCell cells[ode_output_screen_max_width][ode_output_screen_max_height];
} OdeScreenGrid;

struct Ode {
    struct Output {
        U8 buf[ode_output_screen_buf_size];
        struct Screen {
            OdeScreenGrid dst;
            OdeScreenGrid src;
            struct {
                UInt width;
                UInt height;
            } size;
        } screen;
    } output;

    struct Input {
        U8 buf[ode_input_buf_size];
        Bool exit_requested;
    } input;

    struct Init {
        Strs argv_paths;
        struct {
            Termios orig_attrs;
            Bool orig_attrs_got;
        } term;
    } init;
} ode;




OdeRgbaColor rgba(U8 const r, U8 const g, U8 const b, U8 const a) {
    return (OdeRgbaColor) {.r = r, .g = g, .b = b, .a = a};
}

Bool rgbaEql(OdeRgbaColor const* const c1, OdeRgbaColor const* const c2) {
    return (c1->r == c2->r) && (c1->g == c2->g) && (c1->b == c2->b) && (c1->a == c2->a);
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
    if (ode.init.term.orig_attrs_got)
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &ode.init.term.orig_attrs);
}

void odeDie(CStr const hint) {
    termRawOff();
    termClearAndPosTopLeft();
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
    atexit(termClearAndPosTopLeft);
    atexit(termRawOff);
    updateScreenSize();
}

Bool odeProcessInput() {
    Int n_bytes_read = read(STDIN_FILENO, ode.input.buf, ode_input_buf_size);
    if (n_bytes_read < 0) {
        if (errno == EAGAIN)
            n_bytes_read = 0;
        else
            odeDie("odeProcessInput: read");
    }

    if (n_bytes_read != 0) {
        fprintf(stdout, "%zd ", n_bytes_read);
        fflush(stdout);
    }

    for (UInt i = 0, max = n_bytes_read; i < max; i += 1)
        if (ode.input.buf[i] == (0x1f & 'q'))
            ode.input.exit_requested = true;

    return false;
}

void odeRenderOutput() {
    Bool got_dirty_cells = false;

    for (UInt x = 0; x < ode_output_screen_max_width; x += 1)
        for (UInt y = 0; y < ode_output_screen_max_height; y += 1) {
            OdeScreenCell* dst = &ode.output.screen.dst.cells[x][y];
            OdeScreenCell* src = &ode.output.screen.src.cells[x][y];
            dst->dirty = (src->rune != dst->rune) || (!rgbaEql(&src->color.bg, &dst->color.bg)) || (!rgbaEql(&src->color.fg, &dst->color.fg));
            got_dirty_cells |= dst->dirty;
        }

    if (got_dirty_cells) {
        Str buf = (Str) {.len = 0, .at = &ode.output.buf[0]};
        for (UInt x = 0; x < ode_output_screen_max_width; x += 1)
            for (UInt y = 0; y < ode_output_screen_max_height; y += 1)
                if (ode.output.screen.dst.cells[x][y].dirty) {
                    // TODO: append positioning / formatting escapes and the cell's rune
                }
        if (buf.len > 0) {
            Int const n_written = write(STDOUT_FILENO, buf.at, buf.len);
            if (n_written < 0 || ((UInt)n_written) != buf.len)
                odeDie("odeRenderOutput: write");
        }
    }
}

void odeInit() {
    { // init global shared mutable state `ode`
        ode.input.exit_requested = false;
        for (UInt i = 0; i < ode_output_screen_buf_size; i += 1)
            ode.output.buf[i] = '?';
        for (UInt x = 0; x < ode_output_screen_max_width; x += 1)
            for (UInt y = 0; y < ode_output_screen_max_height; y += 1) {
                ode.output.screen.dst.cells[x][y] =
                    (OdeScreenCell) {.dirty = false, .color = {.bg = rgba(0, 0, 0, 255), .fg = rgba(0, 0, 0, 255)}, .rune = 0};
                ode.output.screen.src.cells[x][y] = (OdeScreenCell) {
                    .dirty = false, .color = {.bg = rgba(0xef, 0xe9, 0xe8, 255), .fg = rgba(0x68, 0x64, 0x61, 255)}, .rune = '?'};
            }
    }
    termInit();
}
