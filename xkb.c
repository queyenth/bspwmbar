#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

#include "bspwmbar.h"
#include "util.h"

static Display *display;

/* functions */
static void x_open_display();

void
x_open_display()
{
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        die("Cannot open display\n");
    }
}

void
xkb(draw_context_t *dc, module_option_t *opts)
{
    if (display == NULL) {
        x_open_display();
    }

    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);

    XkbRF_VarDefsRec vd;
    XkbRF_GetNamesProp(display, NULL, &vd);

    char *tok = strtok(vd.layout, ",");

    for (int i = 0; i < state.group; i++) {
        tok = strtok(NULL, ",");
        if (tok == NULL) {
            die("Something goes wrong\n");
        }
    }

    draw_text(dc, tok);
}
