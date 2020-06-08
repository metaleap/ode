#pragma once
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "utils_std_basics.c"
#include "common.c"
#include "ui_ctl_panel.c"
#include "utils_std_mem.c"


typedef struct termios Termios;


#define term_esc "\x1b["
#define ode_output_screen_max_width 256
#define ode_output_screen_max_height 256
#define ode_input_buf_size (1 * 1024 * 1024)


struct Ode {
    struct Init {
        Strs argv_paths;
        struct Term {
            Termios orig_attrs;
            Bool did_tcsetattr;
        } term;
    } init;
    struct Input {
        Bool exit_requested;
    } input;
    struct Output {
        struct Screen {
            OdeScreenCell real[ode_output_screen_max_width][ode_output_screen_max_height];
            OdeScreenCell prep[ode_output_screen_max_width][ode_output_screen_max_height];
            OdeSize size;
            Str term_esc_cursor_pos[ode_output_screen_max_width][ode_output_screen_max_height];
            Bool resized;
        } screen;
        OdeRgbaColors colors;
    } output;
    struct Ui {
        OdeUiCtlPanel main;
        OdeUiCtlPanel statusbar;
        OdeUiCtlPanel sidebar_left;
        OdeUiCtlPanel sidebar_right;
        OdeUiCtlPanel sidebar_bottom;
        OdeUiCtlPanel editors;
        OdeUiCtlPanel view_explorer;
        OdeUiCtlPanel view_extensions;
        OdeUiCtlPanel view_search;
        OdeUiCtlPanel view_outline;
        OdeUiCtlPanel view_diags;
        OdeUiCtlPanel view_logs;
        OdeUiCtlPanel view_terminals;
    } ui;
} ode;




OdeRgbaColor* rgba(U8 const r, U8 const g, U8 const b, U8 const a) {
    OdeRgbaColor spec = (OdeRgbaColor) {.r = r, .g = g, .b = b, .a = a};
    for (UInt i = 0; i < ode.output.colors.len; i += 1) {
        OdeRgbaColor* const color = &ode.output.colors.at[i];
        if (color->rgba == spec.rgba)
            return &ode.output.colors.at[i];
    }
    spec.ansi_esc =
        str7(strL(";2;", 3), uIntToStr(r, 1, 10), strL(";", 1), uIntToStr(g, 1, 10), strL(";", 1), uIntToStr(b, 1, 10), strL("m", 1));
    ·append(ode.output.colors, spec);
    return ·last(ode.output.colors);
}

static void termClearScreen() {
    write(STDOUT_FILENO, term_esc "2J", 2 + 2);
}

static void termPositionCursorAtTopLeftCorner() {
    write(STDOUT_FILENO, term_esc "H", 2 + 1);
}

static void termClearAndPosTopLeft() {
    termClearScreen();
    termPositionCursorAtTopLeftCorner();
}

static void termRestore() {
    if (ode.init.term.did_tcsetattr) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &ode.init.term.orig_attrs);
        write(STDOUT_FILENO, term_esc "?47l", 2 + 4); // leave alt screen (entered in termInit)
        write(STDOUT_FILENO, term_esc "?25h", 2 + 4); // show cursor (hidden in termInit)
        write(STDOUT_FILENO, term_esc "u", 2 + 1);    // restore cursor pos (stored in termInit)
    }
}

void odeDie(CStr const hint, Bool const got_errno) {
    termRestore();
    if (got_errno)
        perror(hint);
    abortWithBacktraceAndMsg(str(hint));
}

static void updateScreenSize() {
    struct winsize win_size = {.ws_row = 0, .ws_col = 0};
    if ((-1 == ioctl(1, TIOCGWINSZ, &win_size)) || (win_size.ws_row == 0) || (win_size.ws_col == 0))
        odeDie("updateScreenSize: ioctl", true);
    ode.output.screen.size.width = (win_size.ws_col >= ode_output_screen_max_width) ? (ode_output_screen_max_width - 1) : win_size.ws_col;
    ode.output.screen.size.height = (win_size.ws_row >= ode_output_screen_max_height) ? (ode_output_screen_max_height - 1) : win_size.ws_row;
}

static void termOnResized() {
    ode.output.screen.resized = true;
    updateScreenSize();
}

static void termRawOn() {
    if (-1 == tcgetattr(STDIN_FILENO, &ode.init.term.orig_attrs))
        odeDie("termRawOn: tcgetattr", true);

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
        odeDie("termRawOn: tcsetattr", true);
}

static void termInit() {
    atexit(termRestore);
    ode.init.term.did_tcsetattr = false;
    termRawOn();
    ode.init.term.did_tcsetattr = true;

    write(STDOUT_FILENO, term_esc "s", 2 + 1);    // store cursor pos (restored in termRestore)
    write(STDOUT_FILENO, term_esc "?25l", 2 + 4); // hide cursor (recovered in termRestore)
    write(STDOUT_FILENO, term_esc "?47h", 2 + 4); // enter alt screen (left in termRestore)
    termClearAndPosTopLeft();
}

void odeInit() {
    ode.input.exit_requested = false;
    Str const esc = strL(term_esc, 2);
    ode.output.colors = ·listOf(OdeRgbaColor, 0, 8);
    ode.output.screen.resized = false;
    for (UInt x = 0; x < ode_output_screen_max_width; x += 1)
        for (UInt y = 0; y < ode_output_screen_max_height; y += 1) {
            ode.output.screen.real[x][y] = (OdeScreenCell) {.color = {.bg = NULL, .fg = NULL, .ul3 = NULL}};
            ode.output.screen.prep[x][y] = (OdeScreenCell) {.color = {.bg = NULL, .fg = NULL, .ul3 = NULL}};
            ode.output.screen.term_esc_cursor_pos[x][y] =
                str5(esc, uIntToStr(1 + y, 1, 10), strL(";", 1), uIntToStr(1 + x, 1, 10), strL("H", 1));
        }
    updateScreenSize();
    if (signal(SIGWINCH, termOnResized) == SIG_ERR)
        odeDie("odeInit: signal", true);
    termInit();
}
