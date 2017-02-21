/* Copyright (C) 2003  Francesco Bradascio <fbradasc@katamail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <stdio.h>
#include <string.h>

#include <ctype.h>

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Sorted_Choice.H>

#include "pixmaps/mini_folder.xpm"
#include "pixmaps/mini_ofolder.xpm"
#include "pixmaps/mini_efolder.xpm"

static Fl_Menu_Item terminator = {0};

static int numericstrcmp(const char *a, const char *b, bool cs=true)
{
int ret = 0;
int diff,magdiff;

    for (;;) {
        if (isdigit(*a & 255) && isdigit(*b & 255)) {
            while (*a == '0') {
                a++;
            }
            while (*b == '0') {
                b++;
            }
            while (isdigit(*a & 255) && *a == *b) {
                a++;
                b++;
            }
            diff = (isdigit(*a & 255) && isdigit(*b & 255)) ? *a - *b : 0;
            magdiff = 0;
            while (isdigit(*a & 255)) {
                magdiff++;
                a++;
            }
            while (isdigit(*b & 255)) {
                magdiff--;
                b++;
            }
            if (magdiff) { /* compare # of significant digits*/
                ret = magdiff;
                break;
            }
            if (diff) { /* compare first non-zero digit */
                ret = diff;
                break;
            }
        } else {
            if (cs) {
                /* compare case-sensitive */
                if ((ret = *a-*b)) {
                    break;
                }
            } else {
                /* compare case-insensitve */
                if ((ret = tolower(*a & 255)-tolower(*b & 255))) {
                    break;
                }
            }
            if (!*a) {
                break;
            }
            a++;
            b++;
        }
    }
    return (ret < 0) ? -1 : ( (ret > 0) ? 1 : 0 );
}

Fl_Sorted_Menu_Item::Fl_Sorted_Menu_Item (
    bool ascending_sort,
    const Fl_Menu_Item *mi,
    bool selected,
    Fl_Sorted_Menu_Item *next,
    Fl_Sorted_Menu_Item *prev
) {
    if (next) {
        next->prev(this);
    }
    if (prev) {
        prev->next(this);
    }
    ascending_ = ascending_sort;
    next_      = next;
    prev_      = prev;
    childs_    = 0;
    lastChild_ = 0;
    memcpy (
        (void *)&item_,
        (void *)( (mi) ? mi : &terminator ),
        sizeof(Fl_Menu_Item)
    );
    selected_  = selected;
}

Fl_Sorted_Menu_Item::~Fl_Sorted_Menu_Item()
{
Fl_Sorted_Menu_Item *next;

    for (
        Fl_Sorted_Menu_Item *child = childs_;
        child;
        child = next
    ) {
        next = child->next();
        delete child;
    }
}

Fl_Sorted_Menu_Item *Fl_Sorted_Menu_Item::add_item (
    const Fl_Menu_Item *mi,
    bool sel
) {
int order;
Fl_Sorted_Menu_Item *child, *newChild;

    for (child = childs_; child; child = child->next()) {
        order = numericstrcmp(mi->label(),child->item()->label());
        if (
            (ascending_ && order<0) ||
            (!ascending_ && order>0)
        ) {
            break;
        }
    }
    newChild = new Fl_Sorted_Menu_Item (
        ascending_,
        mi,
        sel,
        child,
        (child) ? child->prev() : lastChild_
    );
    if (!child) {
        if (!lastChild_) {
            childs_ = newChild;
        }
        lastChild_ = newChild;
    } else if (child==childs_) {
        childs_ = newChild;
    }
    return newChild;
}

const Fl_Menu_Item *Fl_Sorted_Menu_Item::add (
    const Fl_Menu_Item *mi,
    const Fl_Menu_Item *mis,
    int level
) {
int llevel;
Fl_Sorted_Menu_Item *smi;

    for (
        llevel  = level;
        llevel != level || mi->label();
        mi++
    ) {
        if (mi->label()) {
            smi = add_item(mi,(mi==mis));
            if (mi->submenu()) {
                llevel++;
                mi = smi->add(&mi[1],mis,llevel);
            }
        } else {
            llevel--;
        }
    }
    return --mi;
}

const Fl_Menu_Item *Fl_Sorted_Menu_Item::dump (
    const Fl_Menu_Item *mi,
    Fl_Menu_ *menu
) {
Fl_Menu_Item *lmi;

    for (
        Fl_Sorted_Menu_Item *child = childs_;
        child;
        child = child->next()
    ) {
        lmi = child->item();
        if (menu && child->selected()) {
            menu->value(mi);
        }
        memcpy((void *)mi++,(void *)lmi,sizeof(Fl_Menu_Item));
        if (lmi->submenu()) {
            mi = child->dump(mi,menu);
            memcpy((void *)mi++,(void *)&terminator,sizeof(Fl_Menu_Item));
        }
    }
    return mi;
}

void Fl_Sorted_Menu_Item::next(Fl_Sorted_Menu_Item *next)
{
    next_ = next;
}

void Fl_Sorted_Menu_Item::prev(Fl_Sorted_Menu_Item *prev)
{
    prev_ = prev;
}

Fl_Sorted_Menu_Item *Fl_Sorted_Menu_Item::next(void)
{
    return next_;
}

Fl_Sorted_Menu_Item *Fl_Sorted_Menu_Item::prev(void)
{
    return prev_;
}

Fl_Sorted_Menu_Item *Fl_Sorted_Menu_Item::childs(void)
{
    return childs_;
}

Fl_Menu_Item *Fl_Sorted_Menu_Item::item(void)
{
    return &item_;
}

bool Fl_Sorted_Menu_Item::selected(void)
{
    return selected_;
}

void Fl_Sorted_Choice::sort(bool ascending)
{
Fl_Sorted_Menu_Item *items = new Fl_Sorted_Menu_Item(ascending);

    if (menu()) {
        items->add(menu(),mvalue());
        items->dump(menu(),this);
        redraw();
    }
    delete items;
}

void *ListWindow::add (
    Fl_Sorted_Choice *parent,
    const Fl_Tree_Item *father,
    const Fl_Menu_Item *mi,
    int level
) {
int llevel;
Fl_Tree_Item *item=0, *first=0;

    for (
        llevel  = level;
        llevel != level || mi->label();
        mi++
    ) {
        if (mi->label()) {
            item = new Fl_Tree_Item (
                mi->label(),
                (Fl_Pixmap *) (
                    (mi->submenu()) ? parent->folder_pixmap()
                                    : parent->item_pixmap()
                ),
                mi->label(), // (const char *)0,
                (void *)( (mi->submenu()) ? 0 : mi ),
                mi->labelcolor() ? mi->labelcolor()
                                 : (parent) ? parent->textcolor()
                                            : FL_BLACK,
                mi->labelfont()  ? mi->labelfont()
                                 : (parent) ? parent->textfont()
                                            : FL_HELVETICA,
                mi->labelsize()  ? mi->labelsize()
                                 : (parent) ? parent->textsize()
                                            : FL_NORMAL_SIZE,
                true, true,
                tTree,
                (Fl_Tree_Item *)father,
                item,
                0
            );
            if (!first) {
                first = item;
            }
            if (mi->submenu()) {
                llevel++;
                mi = (Fl_Menu_Item *)add(parent,item,&mi[1],llevel);
            }
        } else {
            llevel--;
        }
    }
    return (!father) ? (void *)first : (void *)(--mi);
}

ListWindow::ListWindow(Fl_Sorted_Choice *parent, const Fl_Menu_Item *menu)
    : Fl_Menu_Window(0,0,1,1,0)
{
int H, W;
Fl_Tree_Item *items;

    tTree = new Fl_Tree_Browser(0,0,1,1);
        tTree->box(FL_BORDER_BOX);
        tTree->color(7);
        tTree->draw_lines(false);
        tTree->closed_icon(new Fl_Pixmap(mini_folder_xpm));
        tTree->open_icon(new Fl_Pixmap(mini_ofolder_xpm));
        tTree->empty_icon(new Fl_Pixmap(mini_efolder_xpm));
        tTree->callback(selection_cb, (void *)this);

    end();
    set_modal();
    clear_border();

    tTree->color(parent->color());
    tTree->textfont(parent->textfont());
    tTree->textsize(parent->textsize());

    items = (Fl_Tree_Item *)add(parent,0,menu,0);
    tTree->items(items);

    if (items) {
        H = items->height() * 10;
        W = tTree->width();
        if (tTree->height()>H) {
            W += tTree->scrollbar_width() + 2;
        } else {
            H = tTree->height();
        }
        W = ( W > parent->w() ) ? W : parent->w();
    } else {
        H = parent->h();
        W = parent->w();
    }
    H += 2;
    tTree->resize(tTree->x(),tTree->y(),W,H);
    resize (
        parent->window()->x() + parent->x(),
        parent->window()->y() + parent->y() + parent->h(),
        W,
        H
    );
}

int ListWindow::handle(int e)
{
    switch (e) {
        case FL_KEYBOARD:
            if (Fl::event_key()==FL_Escape) {
                tTree->deselect();
                hide();
                return 1;
            }
        break;
        case FL_PUSH:
            int mx = Fl::event_x_root();
            int my = Fl::event_y_root();
            if (
                (mx < x()) || (mx > (x()+w())) ||
                (my < y()) || (my > (y()+h()))
            ) {
                hide();
                return 1;
            }
        break;
    }
    return Fl_Menu_Window::handle(e);
}

void ListWindow::selection_cb(Fl_Widget *w, void *d)
{
    if (d) {
        ((ListWindow*)d)->selection_cb(w);
    }
}

void ListWindow::selection_cb(Fl_Widget *w)
{
    if (selected()) {
        hide();
    }
}

Fl_Menu_Item *ListWindow::selected(void)
{
    return (
        tTree->selected() &&
        ((Fl_Tree_Item *)(tTree->selected()))->data()
    ) ? (Fl_Menu_Item*)((Fl_Tree_Item *)(tTree->selected()))->data()
      : 0;
}

Fl_Sorted_Choice::Fl_Sorted_Choice(int x,int y,int w,int h,const char *l)
    : Fl_Choice(x,y,w,h,l)
{
    folder_pixmap_=0;
    item_pixmap_=0;
}

const Fl_Menu_Item *Fl_Sorted_Choice::prev_item(const Fl_Menu_Item *mi)
{
    mi = (!mi) ? menu() : mi;
    while (menu() && mi>menu()) {
        mi--;
        if (mi->label() && !mi->submenu()) {
            return mi;
        }
    }
    return 0;
}

const Fl_Menu_Item *Fl_Sorted_Choice::next_item(const Fl_Menu_Item *mi)
{
const Fl_Menu_Item *m = menu();

    mi = (!mi) ? m : mi;
    while (size()>0 && mi<&(m[size()-1])) {
        mi++;
        if (mi->label() && !mi->submenu()) {
            return mi;
        }
    }
    return 0;
}

int Fl_Sorted_Choice::handle(int e)
{
const Fl_Menu_Item* v = 0;

    if (!menu() || !menu()->text) {
        return 0;
    }
    switch (e) {
        case FL_ENTER:
        case FL_LEAVE:
            return 1;

        case FL_MOUSEWHEEL:
            if (Fl::focus() == this) {
                if ( Fl::e_dy < 0 ) {
                    if ((v = prev_item(mvalue()))) {
                        redraw();
                        picked(v);
                    } else {
                        fl_beep(FL_BEEP_MESSAGE);
                    }
                    return 1;
                } else if ( Fl::e_dy > 0 ) {
                    if ((v = next_item(mvalue()))) {
                        redraw();
                        picked(v);
                    } else {
                        fl_beep(FL_BEEP_MESSAGE);
                    }
                    return 1;
                }
            }
            break;
        case FL_KEYBOARD:
            if ( Fl::event_key() == FL_Up ) {
                if ((v = prev_item(mvalue()))) {
                    redraw();
                    picked(v);
                } else {
                    fl_beep(FL_BEEP_MESSAGE);
                }
                return 1;
            } else if (Fl::event_key() == FL_Down ) {
                if ((v = next_item(mvalue()))) {
                    redraw();
                    picked(v);
                } else {
                    fl_beep(FL_BEEP_MESSAGE);
                }
                return 1;
            } else if (
                ( Fl::event_key() != ' ' && Fl::event_key() != FL_Enter ) ||
                ( Fl::event_state() & (FL_SHIFT | FL_CTRL | FL_ALT | FL_META) )
            ) {
                return 0;
            }
        case FL_PUSH:
            if (Fl::visible_focus()) {
                Fl::focus(this);
            }
            Fl::event_is_click(0);
            if (
                e==FL_PUSH &&
                Fl::event_x() < x()+w()-h()
            ) { // click on the text field
                redraw();
                return 1;
            }
        J1:
            v = pulldown();
            if (!v || v->submenu()) {
                return 1;
            }
            if (v != mvalue()) {
                redraw();
            }
            picked(v);
            return 1;
        case FL_SHORTCUT:
            if (Fl_Widget::test_shortcut()) {
                goto J1;
            }
            v = menu()->test_shortcut();
            if (!v) {
                return 0;
            }
            if (v != mvalue()) {
                redraw();
            }
            picked(v);
            return 1;
        case FL_FOCUS:
        case FL_UNFOCUS:
            if (Fl::visible_focus()) {
                redraw();
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
    return 0;
}

const Fl_Menu_Item *Fl_Sorted_Choice::pulldown(void)
{
    Fl_Group::current(0);

    ListWindow wBrwsr(this,menu());

    Fl::grab(wBrwsr);
 
    wBrwsr.show();

    while (wBrwsr.visible()) {
        Fl::wait();
    }
    Fl::release();

    return wBrwsr.selected() ? wBrwsr.selected() : mvalue();
}

