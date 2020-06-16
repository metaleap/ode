#pragma once
#include "utils_std_basics.c"
#include "utils_std_mem.c"
#include "common.c"
#include "core.c"
#include "ui_main.c"

#define ode_input_buf_size (1 * 1024 * 1024)

void odeInitKnownHotKeys() {
    OdeHotKeys k = ode.input.all.hotkeys;
#define ·_ ·append
#define ·__ (OdeHotKey)

    ·_(k, (·__ {.title = str("enter"), .key = ode_key_enter, .esc_seq = str("\x0d")}));
    ·_(k, (·__ {.title = str("backspace"), .key = ode_key_back, .esc_seq = str("\x7f")}));

    ·_(k, (·__ {.title = str("f1"), .key = ode_key_f1, .esc_seq = str("\x1b\x4f\x50")}));
    ·_(k, (·__ {.title = str("f2"), .key = ode_key_f2, .esc_seq = str("\x1b\x4f\x51")}));
    ·_(k, (·__ {.title = str("f3"), .key = ode_key_f3, .esc_seq = str("\x1b\x4f\x52")}));
    ·_(k, (·__ {.title = str("f4"), .key = ode_key_f4, .esc_seq = str("\x1b\x4f\x53")}));
    ·_(k, (·__ {.title = str("f5"), .key = ode_key_f5, .esc_seq = str("\x1b\x5b\x31\x35\x7e")}));
    ·_(k, (·__ {.title = str("f6"), .key = ode_key_f6, .esc_seq = str("\x1b\x5b\x31\x37\x7e")}));
    ·_(k, (·__ {.title = str("f7"), .key = ode_key_f7, .esc_seq = str("\x1b\x5b\x31\x38\x7e")}));
    ·_(k, (·__ {.title = str("f8"), .key = ode_key_f8, .esc_seq = str("\x1b\x5b\x31\x39\x7e")}));
    ·_(k, (·__ {.title = str("f9"), .key = ode_key_f9, .esc_seq = str("\x1b\x5b\x32\x30\x7e")}));
    ·_(k, (·__ {.title = str("f10"), .key = ode_key_f10, .esc_seq = str("\x1b\x5b\x32\x31\x7e")}));
    ·_(k, (·__ {.title = str("f11"), .key = ode_key_f11, .esc_seq = str("\x1b\x5b\x32\x33\x7e")}));
    ·_(k, (·__ {.title = str("f12"), .key = ode_key_f12, .esc_seq = str("\x1b\x5b\x32\x34\x7e")}));
    ·_(k, (·__ {.title = str("left"), .key = ode_key_arr_l, .esc_seq = str("\x1b\x5b\x44")}));
    ·_(k, (·__ {.title = str("down"), .key = ode_key_arr_d, .esc_seq = str("\x1b\x5b\x42")}));
    ·_(k, (·__ {.title = str("up"), .key = ode_key_arr_u, .esc_seq = str("\x1b\x5b\x41")}));
    ·_(k, (·__ {.title = str("right"), .key = ode_key_arr_r, .esc_seq = str("\x1b\x5b\x43")}));
    ·_(k, (·__ {.title = str("end"), .key = ode_key_end, .esc_seq = str("\x1b\x5b\x46")}));
    ·_(k, (·__ {.title = str("home"), .key = ode_key_home, .esc_seq = str("\x1b\x5b\x48")}));
    ·_(k, (·__ {.title = str("insert"), .key = ode_key_ins, .esc_seq = str("\x1b\x5b\x32\x7e")}));
    ·_(k, (·__ {.title = str("delete"), .key = ode_key_del, .esc_seq = str("\x1b\x5b\x33\x7e")}));
    ·_(k, (·__ {.title = str("pageup"), .key = ode_key_pgup, .esc_seq = str("\x1b\x5b\x35\x7e")}));
    ·_(k, (·__ {.title = str("pagedown"), .key = ode_key_pgdn, .esc_seq = str("\x1b\x5b\x36\x7e")}));
    ·_(k, (·__ {.title = str("alt+escape"), .key = ode_key_esc, .alt = true, .esc_seq = str("\x1b\x1b")}));

    static CStr titles[] = {"shift+", "alt+", "alt+shift+", "ctrl+", "ctrl+shift+", "ctrl+alt+", "ctrl+alt+shift+"};
    static CStr fkey_infixes[] = {"\x32", "\x33", "\x34", "\x35", "\x36", "\x37", "\x38"}; // order matches the above
    ·forEach(OdeHotKey, hk, k, {
        if (hk->alt || hk->ctl || hk->shift)
            continue;
        UInt const key = hk->key;
        Bool const is_foo =
            (ode_key_f1 == key || ode_key_f2 == key || ode_key_f3 == key || ode_key_f4 == key || ode_key_end == key || ode_key_home == key
             || ode_key_arr_d == key || ode_key_arr_l == key || ode_key_arr_r == key || ode_key_arr_u == key);
        Bool const is_baz = (ode_key_pgup == key || ode_key_pgdn == key || ode_key_del == key || ode_key_ins == key);
        Bool const is_bar = (is_baz || ode_key_f5 == key || ode_key_f6 == key || ode_key_f7 == key || ode_key_f8 == key || ode_key_f9 == key
                             || ode_key_f10 == key || ode_key_f11 == key || ode_key_f12 == key);
        if (is_foo || is_bar) {
            Str const pref = is_foo ? str("\x1b\x5b\x31\x3b") : str2(NULL, strSub(hk->esc_seq, 0, is_baz ? 3 : 4), str("\x3b"));
            Str const suff = is_foo ? strSub(hk->esc_seq, hk->esc_seq.len - 1, hk->esc_seq.len) : str("\x7e");
            for (UInt i = 0; i <= 6; i += 1)
                ·_(k, (·__ {.title = str2(NULL, str(titles[i]), hk->title),
                            .esc_seq = str3(NULL, pref, str(fkey_infixes[i]), suff),
                            .key = key,
                            .alt = (i == 1 || i == 2 || i == 5 || i == 6),
                            .ctl = (i == 3 || i == 4 || i == 5 || i == 6),
                            .shift = (i == 0 || i == 2 || i == 4 || i == 6)}));
        }
    });
    ·_(k, (·__ {.title = str("alt+enter"), .alt = true, .key = ode_key_enter, .esc_seq = str("\x1b\x0d")}));
    ·_(k, (·__ {.title = str("alt+backspace"), .alt = true, .key = ode_key_back, .esc_seq = str("\x1b\x7f")}));
    ·_(k, (·__ {.title = str("ctrl+backspace"), .ctl = true, .key = ode_key_back, .esc_seq = str("\x08")}));
    ·_(k, (·__ {.title = str("ctrl+alt+backspace"), .ctl = true, .alt = true, .key = ode_key_back, .esc_seq = str("\x1b\x08")}));
    static CStr a_to_z = "abcdefghijklmnopqrstuvwxyz~_@|\\(){}[]^-*+#'.:,;<>!\"$&/=?`0123456789\x25";
    Str ascii_ctl_escs = newStr(NULL, str(a_to_z).len, 0);
    for (UInt i = 0; a_to_z[i] != 0; i += 1) {
        U8 const c = a_to_z[i];
        ascii_ctl_escs.at[i] = (c & 0x1f);
        Str const st = (Str) {.at = (U8*)&c, .len = 1};
        Str const se = (Str) {.at = &ascii_ctl_escs.at[i], .len = 1};
        ·_(k, (·__ {.title = str2(NULL, str("alt+"), st), .alt = true, .key = c, .esc_seq = str2(NULL, str("\x1b"), st)}));
        if (i <= 30 && c != 'i') {
            ·_(k, (·__ {.title = str2(NULL, str("ctrl+"), st), .ctl = true, .key = c, .esc_seq = se}));
            ·_(k,
               (·__ {.title = str2(NULL, str("ctrl+alt+"), st), .ctl = true, .alt = true, .key = c, .esc_seq = str2(NULL, str("\x1b"), se)}));
        }
    }

    { // now reorder such that all \x1b-prefixed hotkeys follow all non-\x1b ones. at runtime lets us short-circuit the search per keypress
        for (UInt i = 0; i < k.len; i += 1)
            if (k.at[i].esc_seq.at[0] == '\x1b') {
                ode.input.hotkeys_idx_esc = i;
                break;
            }
        for (UInt i = ode.input.hotkeys_idx_esc + 1; i < k.len; i += 1)
            if (k.at[i].esc_seq.at[0] != '\x1b') {
                OdeHotKey const copy = k.at[i];
                k.at[i] = k.at[ode.input.hotkeys_idx_esc];
                k.at[ode.input.hotkeys_idx_esc] = copy;
                ode.input.hotkeys_idx_esc += 1;
            }
    }

    ode.input.all.hotkeys = k;
}

Bool odeProcessInput() {
    static U8 buf[ode_input_buf_size];
    static OdeInput inputs_buf[ode_input_buf_size];
    static MemHeap bracketed_paste = (MemHeap) {.cap = (4 * 1024), .kind = mem_heap_pages_malloc};
    static U8 last7[7];

    MemHeap mem_tmp = (MemHeap) {.kind = mem_heap_pages_malloc, .cap = 4 * 1024};
    Int n_bytes_read = read(ode.init.term.tty_fileno, buf, ode_input_buf_size);
    if (n_bytes_read < 0) {
        if (errno == EAGAIN)
            return false;
        else
            odeDie("odeProcessInput: read", true);
    }
    OdeInputs inputs = {.at = &inputs_buf[0], .len = 0};
#define ·isEsc(¹the_str__, ²the_len__, ¹the_min_len__)                                                                                       \
    (((i + (¹the_min_len__)) <= ((UInt)n_bytes_read)) && strEq((¹the_str__), strL(&buf[i], (²the_len__)), (²the_len__)))
    if (n_bytes_read > 0) {
        for (UInt i = 0, max = (UInt)n_bytes_read; i < max; i += 1) {
            Bool const is_esc = (buf[i] == 0x1b);
            for (UInt j = 1; j < 7; j += 1)
                last7[j - 1] = last7[j];
            last7[6] = buf[i];

            if (bracketed_paste.ptr != NULL) {
                if (bracketed_paste.len == bracketed_paste.cap) {
                    memHeapAlloc(&bracketed_paste, 1);
                    bracketed_paste.len -= 1;
                }
                bracketed_paste.ptr[bracketed_paste.len] = buf[i];
                bracketed_paste.len += 1;
                if (strEq(&last7[1], strL(term_esc "201~", 6), 6)) {
                    ·assert(mem_tmp.ptr == NULL);
                    mem_tmp.cap = 4096 + memHeapSize(&bracketed_paste, true);
                    U8* const ptr = memHeapAlloc(&mem_tmp, memHeapSize(&bracketed_paste, false));
                    UInt const len = memHeapCopy(&bracketed_paste, ptr);
                    memHeapFree(&bracketed_paste);
                    bracketed_paste.ptr = NULL;
                    ·push(inputs, ((OdeInput) {
                                      .kind = ode_input_str,
                                      .of = {.string = (Str) {.len = len - 6, .at = ptr}},
                                  }));
                }
            } else if (is_esc && ·isEsc(term_esc "200~", 6, 6)) {
                memHeapAlloc(&bracketed_paste, 1);
                bracketed_paste.len -= 1;
                i += 5;
            } else if (is_esc && ·isEsc(term_esc "M", 3, 6)) {
                OdeMouseState const old = ode.input.mouse;
                OdeMouseState new = old;
                new.pos = (OdePos) {.x = buf[i + 4] - 33, .y = buf[i + 5] - 33};
                Bool const m_pos_eq = posEql(old.pos, new.pos);

                U8 const m = buf[i + 3];
                Bool const m_scroll_up =
                    (m == 0x61) || (m == 0x71) || (m == 0x69) || (m == 0x65) || (m == 0x75) || (m == 0x79) || (m == 0x6d) || (m == 0x7d);
                Bool const m_scroll_down =
                    (m == 0x60) || (m == 0x70) || (m == 0x68) || (m == 0x64) || (m == 0x74) || (m == 0x78) || (m == 0x6c) || (m == 0x7c);
                // Bool const m_any_up = // not currently needed
                //     (m == 0x23) || (m == 0x33) || (m == 0x2b) || (m == 0x27) || (m == 0x37) || (m == 0x3b) || (m == 0x2f) || (m == 0x3f);
                // Bool const m_any_move = // not currently needed
                //     (m == 0x43) || (m == 0x53) || (m == 0x4b) || (m == 0x47) || (m == 0x57) || (m == 0x5b) || (m == 0x4f) || (m == 0x5f);
                Bool const m_down_l =
                    (m == 0x20) || (m == 0x30) || (m == 0x28) || (m == 0x24) || (m == 0x34) || (m == 0x38) || (m == 0x2c) || (m == 0x3c);
                Bool const m_drag_l =
                    (m == 0x40) || (m == 0x50) || (m == 0x48) || (m == 0x44) || (m == 0x54) || (m == 0x58) || (m == 0x4c) || (m == 0x5c);
                Bool const m_down_m =
                    (m == 0x21) || (m == 0x31) || (m == 0x29) || (m == 0x25) || (m == 0x35) || (m == 0x39) || (m == 0x2d) || (m == 0x3d);
                Bool const m_drag_m =
                    (m == 0x41) || (m == 0x51) || (m == 0x49) || (m == 0x45) || (m == 0x55) || (m == 0x59) || (m == 0x4d) || (m == 0x5d);
                Bool const m_down_r =
                    (m == 0x22) || (m == 0x32) || (m == 0x2a) || (m == 0x26) || (m == 0x36) || (m == 0x3a) || (m == 0x2e) || (m == 0x3e);
                Bool const m_drag_r =
                    (m == 0x42) || (m == 0x52) || (m == 0x4a) || (m == 0x46) || (m == 0x56) || (m == 0x5a) || (m == 0x4e) || (m == 0x5e);
                new.dragging = m_drag_l || m_drag_m || m_drag_r;
                new.btn_down.left = m_down_l || m_drag_l;
                new.btn_down.mid = m_down_m || m_drag_m;
                new.btn_down.right = m_down_r || m_drag_r;
                OdeInput evt = (OdeInput) {
                    .kind = ode_input_mouse,
                    .of = {.mouse = {.scroll = m_scroll_down || m_scroll_up,
                                     .down = m_scroll_down || m_down_l || m_down_m || m_down_r,
                                     .btn_l = m_down_l || m_drag_l,
                                     .btn_m = m_down_m || m_drag_m,
                                     .btn_r = m_down_r || m_drag_r,
                                     .did_drop = old.dragging && !new.dragging,
                                     .did_move = !m_pos_eq,
                                     .did_click =
                                         m_pos_eq && (old.btn_down.left || old.btn_down.mid || old.btn_down.right)
                                         && !(new.btn_down.left || new.btn_down.mid || new.btn_down.right || old.dragging || new.dragging)}},
                    .mod_key = {
                        .ctl = ((m == 0x71 || m == 0x75 || m == 0x79 || m == 0x7d) || (m == 0x70 || m == 0x74 || m == 0x78 || m == 0x7c))
                               || (m >= 0x30 && m <= 0x3f) || (m >= 0x50 && m <= 0x5f),
                        .alt = ((m == 0x69 || m == 0x79 || m == 0x6d || m == 0x7d) || (m == 0x68 || m == 0x78 || m == 0x6c || m == 0x7c))
                               || (m >= 0x28 && m <= 0x2f) || (m >= 0x38 && m <= 0x3f) || (m >= 0x48 && m <= 0x4f)
                               || (m >= 0x58 && m <= 0x5f),
                        .shift =
                            ((m == 0x65 || m == 0x75 || m == 0x6d || m == 0x7d) || (m == 0x64 || m == 0x74 || m == 0x6c || m == 0x7c))
                            || ((m >= 0x24 && m <= 0x27) || (m >= 0x34 && m <= 0x37) || (m >= 0x44 && m <= 0x47) || (m >= 0x54 && m <= 0x57))
                            || ((m >= 0x2c && m <= 0x2f) || (m >= 0x3c && m <= 0x3f) || (m >= 0x4c && m <= 0x4f)
                                || (m >= 0x5c && m <= 0x5f))}};
                if (evt.of.mouse.did_click || evt.of.mouse.did_drop) {
                    evt.of.mouse.btn_l = old.btn_down.left;
                    evt.of.mouse.btn_m = old.btn_down.mid;
                    evt.of.mouse.btn_r = old.btn_down.right;
                }
                ·push(inputs, evt);
                ode.input.mouse = new;
                i += 5;
            } else {
                OdeHotKey* hotkey = NULL;
                ·forEach(OdeHotKey, hk, ode.input.all.hotkeys, {
                    if (iˇhk >= ode.input.hotkeys_idx_esc && !is_esc)
                        break;
                    if (·isEsc(hk->esc_seq.at, hk->esc_seq.len, hk->esc_seq.len)) {
                        hotkey = hk;
                        break;
                    }
                });
                if (hotkey != NULL) {
                    ·push(inputs, ((OdeInput) {.kind = ode_input_hotkey,
                                               .of = {.key = hotkey->key},
                                               .mod_key = {.ctl = hotkey->ctl, .alt = hotkey->alt, .shift = hotkey->shift}}));
                    i += (hotkey->esc_seq.len - 1);
                    if (hotkey->ctl && hotkey->key == 'q' && !(hotkey->alt || hotkey->shift))
                        ode.input.exit_requested = true;
                } else
                    ·push(inputs, ((OdeInput) {.kind = ode_input_str, .of = {.string = (Str) {.at = &buf[i], .len = 1}}}));
            }
        }
    }

    Bool ret_dirty =
        (!ode.input.exit_requested) && (inputs.len > 0) && ode.ui.main->ui_panel.base.on.input(&ode.ui.main->ui_panel.base, &mem_tmp, inputs);
    memHeapFree(&mem_tmp);
    return ret_dirty;
}
