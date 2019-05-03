//
// Copyright 2001-2002 by Francesco Bradascio.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fbradasc@katamail.com".
//
//         FlScan is based in part on the work of
//         the FLTK project (http://www.fltk.org).
//

#include <FL/Fl_File_Icon.H>
#include "flscan.h"
#include <string.h>

const char *copyrightText = 
"<HTML><BODY><CENTER>"
"<B>FlScan 1.0.2</B><BR>"
"a new FLTK SANE frontend<BR>"
"Copyright (C) 2001-2002  by Francesco Bradascio</CENTER><P>"
"</P>"
"This program is free software; you can redistribute it and/or modify "
"it under the terms of the <I>GNU General Public License</I> as published by "
"the Free Software Foundation; either version 2 of the License, or "
"(at your option) any later version.<P>"
"</P>"
"This program is distributed in the hope that it will be useful, "
"but WITHOUT ANY WARRANTY; without even the implied warranty of "
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
"GNU General Public License for more details.<P>"
"</P>"
"You should have received a copy of the <I>GNU General Public License</I> "
"along with this program; if not, write to the Free Software "
"Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA<P>"
"</P>"
"<CENTER>Please report bugs to fbradasc@katamail.com.</CENTER></BODY></HTML>";

void loadIcons()
{
static short  plain[] =   // Plain file icon
{
    Fl_File_Icon::COLOR, -1, -1,
        Fl_File_Icon::OUTLINEPOLYGON, 0, FL_GRAY,
            Fl_File_Icon::VERTEX, 2000, 1000,
            Fl_File_Icon::VERTEX, 2000, 9000,
            Fl_File_Icon::VERTEX, 6000, 9000,
            Fl_File_Icon::VERTEX, 8000, 7000,
            Fl_File_Icon::VERTEX, 8000, 1000,
        Fl_File_Icon::END,
        Fl_File_Icon::OUTLINEPOLYGON, 0, FL_GRAY,
            Fl_File_Icon::VERTEX, 6000, 9000,
            Fl_File_Icon::VERTEX, 6000, 7000,
            Fl_File_Icon::VERTEX, 8000, 7000,
        Fl_File_Icon::END,
        Fl_File_Icon::COLOR, 0, FL_BLACK,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 6000, 7000,
            Fl_File_Icon::VERTEX, 8000, 7000,
            Fl_File_Icon::VERTEX, 8000, 1000,
            Fl_File_Icon::VERTEX, 2000, 1000,
        Fl_File_Icon::END,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 3000, 7000,
            Fl_File_Icon::VERTEX, 5000, 7000,
        Fl_File_Icon::END,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 3000, 6000,
            Fl_File_Icon::VERTEX, 5000, 6000,
        Fl_File_Icon::END,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 3000, 5000,
            Fl_File_Icon::VERTEX, 7000, 5000,
        Fl_File_Icon::END,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 3000, 4000,
            Fl_File_Icon::VERTEX, 7000, 4000,
        Fl_File_Icon::END,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 3000, 3000,
            Fl_File_Icon::VERTEX, 7000, 3000,
        Fl_File_Icon::END,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 3000, 2000,
            Fl_File_Icon::VERTEX, 7000, 2000,
        Fl_File_Icon::END,
    Fl_File_Icon::END
};
static short  image[] =   // Image file icon
{
    Fl_File_Icon::COLOR, -1, -1,
        Fl_File_Icon::OUTLINEPOLYGON, 0, FL_GRAY,
            Fl_File_Icon::VERTEX, 2000, 1000,
            Fl_File_Icon::VERTEX, 2000, 9000,
            Fl_File_Icon::VERTEX, 6000, 9000,
            Fl_File_Icon::VERTEX, 8000, 7000,
            Fl_File_Icon::VERTEX, 8000, 1000,
        Fl_File_Icon::END,
        Fl_File_Icon::OUTLINEPOLYGON, 0, FL_GRAY,
            Fl_File_Icon::VERTEX, 6000, 9000,
            Fl_File_Icon::VERTEX, 6000, 7000,
            Fl_File_Icon::VERTEX, 8000, 7000,
        Fl_File_Icon::END,
        Fl_File_Icon::COLOR, 0, FL_BLACK,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 6000, 7000,
            Fl_File_Icon::VERTEX, 8000, 7000,
            Fl_File_Icon::VERTEX, 8000, 1000,
            Fl_File_Icon::VERTEX, 2000, 1000,
        Fl_File_Icon::END,
        Fl_File_Icon::COLOR, 0, FL_RED,
        Fl_File_Icon::POLYGON,
            Fl_File_Icon::VERTEX, 3500, 2500,
            Fl_File_Icon::VERTEX, 3000, 3000,
            Fl_File_Icon::VERTEX, 3000, 4000,
            Fl_File_Icon::VERTEX, 3500, 4500,
            Fl_File_Icon::VERTEX, 4500, 4500,
            Fl_File_Icon::VERTEX, 5000, 4000,
            Fl_File_Icon::VERTEX, 5000, 3000,
            Fl_File_Icon::VERTEX, 4500, 2500,
        Fl_File_Icon::END,
        Fl_File_Icon::COLOR, 0, FL_GREEN,
        Fl_File_Icon::POLYGON,
            Fl_File_Icon::VERTEX, 5500, 2500,
            Fl_File_Icon::VERTEX, 5000, 3000,
            Fl_File_Icon::VERTEX, 5000, 4000,
            Fl_File_Icon::VERTEX, 5500, 4500,
            Fl_File_Icon::VERTEX, 6500, 4500,
            Fl_File_Icon::VERTEX, 7000, 4000,
            Fl_File_Icon::VERTEX, 7000, 3000,
            Fl_File_Icon::VERTEX, 6500, 2500,
        Fl_File_Icon::END,
        Fl_File_Icon::COLOR, 0, FL_BLUE,
        Fl_File_Icon::POLYGON,
            Fl_File_Icon::VERTEX, 4500, 3500,
            Fl_File_Icon::VERTEX, 4000, 4000,
            Fl_File_Icon::VERTEX, 4000, 5000,
            Fl_File_Icon::VERTEX, 4500, 5500,
            Fl_File_Icon::VERTEX, 5500, 5500,
            Fl_File_Icon::VERTEX, 6000, 5000,
            Fl_File_Icon::VERTEX, 6000, 4000,
            Fl_File_Icon::VERTEX, 5500, 3500,
        Fl_File_Icon::END,
    Fl_File_Icon::END
};
static short  dir[] =     // Directory icon
{
    Fl_File_Icon::COLOR, -1, -1,
        Fl_File_Icon::POLYGON,
            Fl_File_Icon::VERTEX, 1000, 1000,
            Fl_File_Icon::VERTEX, 1000, 7500,
            Fl_File_Icon::VERTEX, 9000, 7500,
            Fl_File_Icon::VERTEX, 9000, 1000,
        Fl_File_Icon::END,
        Fl_File_Icon::POLYGON,
            Fl_File_Icon::VERTEX, 1000, 7500,
            Fl_File_Icon::VERTEX, 2500, 9000,
            Fl_File_Icon::VERTEX, 5000, 9000,
            Fl_File_Icon::VERTEX, 6500, 7500,
        Fl_File_Icon::END,
        Fl_File_Icon::COLOR, 0, FL_WHITE,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 1500, 1500,
            Fl_File_Icon::VERTEX, 1500, 7000,
            Fl_File_Icon::VERTEX, 9000, 7000,
        Fl_File_Icon::END,
        Fl_File_Icon::COLOR, 0, FL_BLACK,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 9000, 7500,
            Fl_File_Icon::VERTEX, 9000, 1000,
            Fl_File_Icon::VERTEX, 1000, 1000,
        Fl_File_Icon::END,
        Fl_File_Icon::COLOR, 0, FL_GRAY,
        Fl_File_Icon::LINE,
            Fl_File_Icon::VERTEX, 1000, 1000,
            Fl_File_Icon::VERTEX, 1000, 7500,
            Fl_File_Icon::VERTEX, 2500, 9000,
            Fl_File_Icon::VERTEX, 5000, 9000,
            Fl_File_Icon::VERTEX, 6500, 7500,
            Fl_File_Icon::VERTEX, 9000, 7500,
        Fl_File_Icon::END,
    Fl_File_Icon::END
};
    Fl_File_Icon::load_system_icons();

    // Create the default icons...
    new Fl_File_Icon (
        "*",
        Fl_File_Icon::PLAIN,
        sizeof(plain) / sizeof(plain[0]),
        plain
    );
    new Fl_File_Icon (
        "*.{bm|bmp|bw|gif|jpg|pbm|pcd|pgm|ppm|png|ras|rgb|tif|xbm|xpm}",
        Fl_File_Icon::PLAIN,
        sizeof(image) / sizeof(image[0]),
        image
    );
    new Fl_File_Icon (
        "*",
        Fl_File_Icon::DIRECTORY,
        sizeof(dir) / sizeof(dir[0]),
        dir
    );
}

int handle(int e)
{
    // this is used to stop Esc from exiting the program:
    return ((e==FL_SHORTCUT)&&(Fl::event_key()==FL_Escape));
}

int main(int argc, char **argv)
{
Fl_Window *w;
bool enable_scan=true;

    FL_LOCK_BEGIN()
    FL_TOGGLE_THREAD()
    Fl::scheme("gtk+"); 
    sane=nullptr;
    sane_host=nullptr;
    loadIcons();
    Fl::add_handler(handle);
    if (argc>2 && argv[2] && strlen(argv[2])) {
        sane_host=strdup(argv[2]);
    }
    sane = new C_Sane (
        (char*)(argc>1) ? argv[1] : 0,
        (char*)sane_host,
        (SANE_Auth_Callback)auth_callback
    );
    if (sane_host) {
      free(sane_host);
    }
    if (sane) {
        if (sane->status()!=SANE_STATUS_GOOD) {
            fprintf (
                stderr,
                "Error opening SANE: %s\n",
                (SANE_String_Const)sane->status()
            );
            enable_scan=false;
        }
        FlScanDeviceSelection();
        w=makeFlSCAN(enable_scan);
        FlScanLoadSettings();
        FlScanResize(0);
        w->show();
    } else {
        fprintf (stderr,"Cannot create the SANE object\n");
    }
    Fl::run();
    FlScanSaveSettings();
    FlScanSaveCurrentDevice();
    delete sane;
    return 0;
}
