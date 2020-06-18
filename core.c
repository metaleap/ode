#pragma once
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"


static void termClearScreen() {
    write(ode.init.term.tty_fileno, term_esc "2J", 2 + 2);
}

static void termPositionCursorAtTopLeftCorner() {
    write(ode.init.term.tty_fileno, term_esc "H", 2 + 1);
}

static void termClearAndPosTopLeft() {
    termClearScreen();
    termPositionCursorAtTopLeftCorner();
}

static void termRestore() {
    if (ode.init.term.did_tcsetattr) {
        tcsetattr(ode.init.term.tty_fileno, TCSAFLUSH, &ode.init.term.orig_attrs);
        write(ode.init.term.tty_fileno, term_esc "?1003l",
              2 + 6); // request to no longer report mouse events
        write(ode.init.term.tty_fileno, term_esc "?2004l",
              2 + 6); // disable bracketed-paste mode
        write(ode.init.term.tty_fileno, term_esc "?47l",
              2 + 4); // leave alt screen (entered in termInit)
        write(ode.init.term.tty_fileno, term_esc "?25h",
              2 + 4); // show cursor (hidden in termInit)
        write(ode.init.term.tty_fileno, term_esc "u",
              2 + 1); // restore cursor pos (stored in termInit)
    }
}

static void odeOnExit() {
    void odeUiCtlDispose(OdeUiCtl * ctl);
    odeUiCtlDispose((OdeUiCtl*)ode.ui.main);
    termRestore();
    if (ode.init.term.tty_fileno > 2)
        close(ode.init.term.tty_fileno);
}

void odeDie(CStr const hint, Bool const got_errno) {
    odeOnExit();
    if (got_errno)
        perror(hint);
    abortWithBacktraceAndMsg(str(hint));
}

static void updateScreenSize() {
    struct winsize win_size = {.ws_row = 0, .ws_col = 0};
    if ((-1 == ioctl(1, TIOCGWINSZ, &win_size)) || (win_size.ws_row == 0)
        || (win_size.ws_col == 0))
        odeDie("updateScreenSize: ioctl", true);
    ode.output.screen.size.width = (win_size.ws_col >= ode_output_screen_max_width)
                                       ? (ode_output_screen_max_width - 1)
                                       : win_size.ws_col;
    ode.output.screen.size.height = (win_size.ws_row >= ode_output_screen_max_height)
                                        ? (ode_output_screen_max_height - 1)
                                        : win_size.ws_row;
}

static void termOnResized() {
    ode.input.screen_resized = true;
    updateScreenSize();
}

static void termRawOn() {
    if (-1 == tcgetattr(ode.init.term.tty_fileno, &ode.init.term.orig_attrs))
        odeDie("termRawOn: tcgetattr", true);

    Termios new_attrs = ode.init.term.orig_attrs;
    new_attrs.c_iflag &=
        ~(BRKINT   // disable break-condition-to-signal-raising (Ctl+C)
          | ICRNL  // disable CR-LF so that Enter/Return key sends \n not \r\n
          | INPCK  // disable parity-checking (not applicable to software terminals)
          | ISTRIP // disable 8th-bit stripping
          | IXON); // disable processing of Ctl+S and Ctl+Q so we receive them
    new_attrs.c_oflag &= ~OPOST; // disable automatic \n-to-\r\n output post-processing
    new_attrs.c_cflag |= CS8;    // set char-size = 8 bits
    new_attrs.c_lflag &=
        ~(ECHO       // disable automagic echoing of inputs
          | ICANON   // disable canonical mode (input sent line-by-line instead of
                     // byte-by-byte)
          | ISIG     // disable processing of Ctl+C and Ctl+Z so we receive them
          | IEXTEN); // disable processing of Ctl+V so we receive it
    new_attrs.c_cc[VMIN] = 0;  // read() returns as soon as any amount of input bytes
    new_attrs.c_cc[VTIME] = 1; // read() times out after 1/10 sec

    if (-1 == tcsetattr(ode.init.term.tty_fileno, TCSAFLUSH, &new_attrs))
        odeDie("termRawOn: tcsetattr", true);
}

static void termInit() {
    atexit(odeOnExit);
    termRawOn();
    ode.init.term.did_tcsetattr = true;

    write(ode.init.term.tty_fileno, term_esc "s",
          2 + 1); // store cursor pos (restored in termRestore)
    write(ode.init.term.tty_fileno, term_esc "?25l",
          2 + 4); // hide cursor (recovered in termRestore)
    write(ode.init.term.tty_fileno, term_esc "?47h",
          2 + 4); // enter alt screen (left in termRestore)
    write(ode.init.term.tty_fileno, term_esc "?2004h",
          2 + 6); // enable bracketed-paste mode
    write(ode.init.term.tty_fileno, term_esc "?1003h",
          2 + 6); // request terminal to report mouse events
    termClearAndPosTopLeft();
}

void odeInit() {
    ode.init.fs.cur_dir_path = str(getcwd((PtrAny)&mem_bss.buf, mem_bss_max));
    if (ode.init.fs.cur_dir_path.at == NULL)
        odeDie("odeInit: getcwd", true);
    mem_bss.pos = ode.init.fs.cur_dir_path.len;
    {
        UInt env_count = 0;
        for (UInt i = 0; __environ[i] != NULL; i += 1)
            env_count += 1;
        ode.init.env.names = ·sliceOf(Str, NULL, 0, env_count);
        ode.init.env.values = ·sliceOf(Str, NULL, 0, env_count);
        for (UInt i = 0; i < env_count; i += 1) {
            Str env_pair = str(__environ[i]);
            ºUInt idx_eq = strIndexOf(env_pair, strL("=", 1));
            if (idx_eq.got) {
                ·push(ode.init.env.names, strSub(env_pair, 0, idx_eq.it));
                ·push(ode.init.env.values,
                      strSub(env_pair, 1 + idx_eq.it, env_pair.len));
            }
        }
        ode.init.fs.home_dir_path = odeEnv(str("HOME"));
        while (
            ode.init.fs.home_dir_path.len > 0
            && '/' == ode.init.fs.home_dir_path.at[ode.init.fs.home_dir_path.len - 1])
            ode.init.fs.home_dir_path.len -= 1;
    }
    ode.init.term.tty_fileno = open("/dev/tty", O_RDWR | O_NOCTTY);
    if (ode.init.term.tty_fileno == -1)
        odeDie("odeInit: open(/dev/tty)", true);

    ode.input.mouse.pos = pos(255, 255);
    ode.input.all.commands = ·listOf(OdeCmd, NULL, 0, 16);
    ode.input.all.hotkeys = ·listOf(OdeHotKey, NULL, 0, 320);
    void odeInitKnownHotKeys();
    odeInitKnownHotKeys();
    void odeInitCommands();
    odeInitCommands();

    ode.output.colors = ·listOf(OdeRgbaColor, NULL, 0, 16);
    Str const esc = str(term_esc);
    for (UInt x = 0; x < ode_output_screen_max_width; x += 1)
        for (UInt y = 0; y < ode_output_screen_max_height; y += 1)
            ode.output.screen.term_esc_cursor_pos[x][y] =
                str5(NULL, esc, uIntToStr(NULL, 1 + y, 1, 10), strL(";", 1),
                     uIntToStr(NULL, 1 + x, 1, 10), strL("H", 1));
    updateScreenSize();
    if (signal(SIGWINCH, termOnResized) == SIG_ERR)
        odeDie("odeInit: signal", true);
    termInit();
}
