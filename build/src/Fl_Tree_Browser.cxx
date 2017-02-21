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
#include <string.h>
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Tree_Browser.H>

#include <FL/pixmaps/open_icon.xpm>
#include <FL/pixmaps/closed_icon.xpm>
#include <FL/pixmaps/empty_icon.xpm>

Fl_Pixmap* Fl_Tree_Browser::s_open_icon_   = 0;
Fl_Pixmap* Fl_Tree_Browser::s_closed_icon_ = 0;
Fl_Pixmap* Fl_Tree_Browser::s_empty_icon_  = 0;

static Fl_Tree_Item *last_item_open(Fl_Tree_Item *item)
{
    if (item->childs() && item->opened()) {
        for (item=item->childs(); item; item = item->next()) {
            if (!item->next()) { // this is the last son
                return last_item_open(item);
            }
        }
    }
    return item;
}

Fl_Tree_Browser::Fl_Tree_Browser(int X,int Y,int W,int H,const char*l)
    : Fl_Browser_(X,Y,W,H,l)
{
    type(FL_HOLD_BROWSER);
    when(FL_WHEN_RELEASE_ALWAYS);
    has_scrollbar(Fl_Browser_::VERTICAL);

    items_ = (Fl_Tree_Item *)0;

    if (s_closed_icon_ == 0) {
        s_closed_icon_ = new Fl_Pixmap(closed_icon_xpm);
    }
    if (s_open_icon_ == 0) {
        s_open_icon_ = new Fl_Pixmap(open_icon_xpm);
    }
    if (s_empty_icon_ == 0) {
        s_empty_icon_ = new Fl_Pixmap(empty_icon_xpm);
    }
    closed_icon_ = 0;
    open_icon_   = 0;
    empty_icon_  = 0;

    level_width_ = 0;

    closed_icon(s_closed_icon_);
    open_icon(s_open_icon_);
    empty_icon(s_empty_icon_);

    pushedtitle_   = 0;
    draw_lines_    = true;
    label_offset_  = 0;
    pixmap_offset_ = 0;
}

Fl_Tree_Browser::~Fl_Tree_Browser()
{
    closed_icon(0);
    open_icon(0);
    empty_icon(0);
    clear();

}

int Fl_Tree_Browser::level_width()
{
    if (level_width_ < 12) {
        level_width_ = 12;
    }
    return level_width_;
}
void Fl_Tree_Browser::clear()
{
Fl_Tree_Item *next;

    for (
        Fl_Tree_Item *item = items_;
        item;
        item = next
    ) {
        next = item->next();
        delete item;
    }
}

void Fl_Tree_Browser::open_icon(Fl_Pixmap *p)
{
int wp, hp;

    if (open_icon_ && open_icon_ != s_open_icon_) {
        delete open_icon_;
    }
    open_icon_ = p;

    if (p && fl_measure_pixmap(p->data(),wp,hp) && wp > level_width_) {
        level_width_ = wp;
    }
}

void Fl_Tree_Browser::closed_icon(Fl_Pixmap *p)
{
int wp, hp;

    if (closed_icon_ && closed_icon_ != s_closed_icon_) {
        delete closed_icon_;
    }
    closed_icon_ = p;

    if (p && fl_measure_pixmap(p->data(),wp,hp) && wp > level_width_) {
        level_width_ = wp;
    }
}

void Fl_Tree_Browser::empty_icon(Fl_Pixmap *p)
{
int wp, hp;

    if (empty_icon_ && empty_icon_ != s_empty_icon_) {
        delete empty_icon_;
    }
    empty_icon_ = p;

    if (p && fl_measure_pixmap(p->data(),wp,hp) && wp > level_width_) {
        level_width_ = wp;
    }
}

void *Fl_Tree_Browser::item_first() const
{
    return (void *)items_;
}

void *Fl_Tree_Browser::item_next(void *l) const
{
Fl_Tree_Item *item = (Fl_Tree_Item *)l;

    if (item->childs() && item->opened()) {
        return (void *)(item->childs());
    } else if (item->next()) {
        return (void *)(item->next());
    } else if (item->father()) { // the last son, go up one level
        for (item = item->father(); item; item = item->father()) {
            if (item->next()) {
                return (void *)(item->next());
            } // else the last son, go up one level
        }
    }
    return (void *)0;
}

void *Fl_Tree_Browser::item_prev(void *l) const
{
Fl_Tree_Item *item = (Fl_Tree_Item *)l;

    if (item->prev()) {
        return (void *)last_item_open(item->prev());
    }
    return (void *)(item->father());
}

int Fl_Tree_Browser::item_height(void *l) const
{
    return ((Fl_Tree_Item *)l)->height();
}

int Fl_Tree_Browser::incr_height() const
{
    return textsize() + 2;
}

void Fl_Tree_Browser::item_draw(void *v, int X, int Y, int, int) const
{
    if (v) {
        ((Fl_Tree_Item *)v)->draw(X,Y);
    }
}

int Fl_Tree_Browser::item_width(void *v) const
{
    return ( v ) ? ((Fl_Tree_Item *)v)->width() : 0;
}

void Fl_Tree_Browser::item_select(void *v, int s)
{
    if (v) {
        ((Fl_Tree_Item *)v)->selected( s ? true : false );
    }
}

int Fl_Tree_Browser::item_selected(void *v) const
{
    return ( v && ((Fl_Tree_Item *)v)->selected() ) ? 1 : 0;
}

int Fl_Tree_Browser::handle(int e)
{
Fl_Tree_Item *l=0;
int X,Y,W,H,WW;
Fl_Pixmap *icon = 0;
  
    bbox(X,Y,W,H);
    switch (e) {
        case FL_KEYBOARD:
        case FL_SHORTCUT:
            switch (Fl::event_key()) {
                case FL_Up:
                    l = (Fl_Tree_Item *) (
                            (selection()) ? item_prev(selection())
                                          : (top()) ? top()
                                                    : item_first()
                    );
                    for (;l;l = (Fl_Tree_Item *)item_prev((void *)l)) {
                        if (item_height((void *)l)>0) {
                            select_only((void *)l,0);
                            break;
                        }
                    }
                    return 1;
                case FL_Down:
                    l = (Fl_Tree_Item *) (
                            (selection()) ? item_next(selection())
                                          : (top()) ? top()
                                                    : item_first()
                    );
                    for (;l;l = (Fl_Tree_Item *)item_next((void *)l)) {
                        if (item_height((void *)l)>0) {
                            select_only((void *)l,0);
                            break;
                        }
                    }
                    return 1;
                case FL_Enter:
                case ' ':
                    l = (Fl_Tree_Item *) (
                            (selection()) ? selection()
                                          : (top()) ? top()
                                                    : item_first()
                    );
                    if (l->childs() && l->can_open()) {
                        l->opened(l->opened() ? false : true);
                        for (Fl_Tree_Item *k=l->childs();k; k=k->next()) {
                            k->visible(l->opened());
                        }
                        redraw();
                    }
                    do_callback();
                break;
            }
            break;
        case FL_MOVE:
            if (!Fl::event_inside(X,Y,W,H)) {
                break;
            }
            l = (Fl_Tree_Item *)find_item(Fl::event_y());
            if (l) {
                Fl::belowmouse();
                Fl_Tooltip::enter_area (
                    this,
                    0, 0, w(), h(),
                    l->tooltip()
                );
            }
            break;
        case FL_PUSH:
            if (!Fl::event_inside(X,Y,W,H)) {
                break;
            }
            l = (Fl_Tree_Item *)find_item(Fl::event_y());
            if (l) {
                if (l->can_open() && l->childs()) {
                    if (!l->next() || l->next()->level() <= l->level()) {
                        if (l->opened()) {
                            icon = closed_icon_;
                        } else {
                            icon = open_icon_;
                        }
                    } else {
                        icon = empty_icon_;
                    }
                }
                WW = (icon) ? icon->w() : 10;
                X += 3 + l->level() * level_width_;
                if (
                    l->childs() && l->can_open() &&
                    Fl::event_x()>X && Fl::event_x()<X+WW
                ) {
                    pushedtitle_ = l;
                    redraw_line(l);
                }
            }
        break;
        case FL_RELEASE:
            if (Fl::event_inside(X,Y,W,H)) {
                l = pushedtitle_;
                pushedtitle_ = 0;
                if (l && l->can_open()) {
                    l->opened(l->opened() ? false : true);
                    for (Fl_Tree_Item *k=l->childs(); k; k=k->next()) {
                        k->visible(l->opened());
                    }
                    redraw();
                }
            }
        break;
    }
    return Fl_Browser_::handle(e);
}

int Fl_Tree_Browser::full_width() const
{
int w, W=0;

    for (void *item = item_first(); item; item = item_next(item)) {
        w = item_width(item);
        if (w > W) {
            W = w;
        }
    }
    return W;
}

int Fl_Tree_Browser::full_height() const
{
int h=0;

    for (void *item = item_first(); item; item = item_next(item)) {
        h += item_height(item);
    }
    return h;
}

void Fl_Tree_Item::draw_connection_line(int X,int Y,int H)
{
int W;
Fl_Pixmap *icon = 0;

    if (can_open_ && childs_) {
        if (!next_ || next_->level() <= level_) {
            if (opened_) {
                icon = parent_->open_icon();
            } else {
                icon = parent_->closed_icon();
            }
        } else {
            icon = parent_->empty_icon();
        }
    }
    if (next_) {
        X += 3 + level_ * parent_->level_width();
        W = (icon) ? icon->w() : 10;
        fl_xyline(X + (W/2), Y, X + (W/2), Y + H);
    }
}

void Fl_Tree_Item::draw(int X, int Y)
{
int XS, W, WW, H;
Fl_Tree_Item *p;
Fl_Pixmap *icon=0;

    XS = X;
    X += 3 + level_ * parent_->level_width();

    W = width();
    H = height();

    if (selected_) {
        fl_color(fl_contrast(color_,FL_SELECTION_COLOR));
    } else {
        fl_color(color_);
    }
    if (can_open_ && childs_) {
        if (!next_ || next_->level() <= level_) {
            if (opened_ != (this == parent_->pushedtitle())) {
                icon = parent_->open_icon();
            } else {
                icon = parent_->closed_icon();
            }
        } else {
            icon = parent_->empty_icon();
        }
    }
    WW = -2;
    if (parent_->draw_lines()) {
        WW = (icon) ? icon->w() : 10;
        fl_xyline(X+(WW/2), Y+(H/2), X+WW, Y+(H/2));
        fl_xyline(X+(WW/2), Y      , X+(WW/2), Y+((next_) ? H : (H/2)));
        for (p=father_; p; p=p->father()) {
            p->draw_connection_line(XS,Y,H);
        }
    }
    if (icon) {
        icon->draw(X,Y);
        WW = icon->w();
    }
    X += WW + 2;
    if (pixmap_) {
        X += parent_->pixmap_offset();
        pixmap_->draw(X,Y);
        X += pixmap_->w() + 2;
    }
    if (label_) {
        X += parent_->label_offset();
        // W -= X; ???
        fl_font(font_,font_size_);
        fl_draw (
            (const char *)label_,
            X,Y,W,H,
            (Fl_Align)(FL_ALIGN_LEFT | FL_ALIGN_CLIP)
        );
    }
}

int Fl_Tree_Item::width()
{
int wl=0, hl=0;
int W=0;
Fl_Pixmap *icon=0;

    if (visible_) {

        if (childs_) { // room for the [+], [-] or [X] icon
            if (!next_ || next_->level() <= level_) {
                if (opened_ != (this == parent_->pushedtitle())) {
                    icon = parent_->open_icon();
                } else {
                    icon = parent_->closed_icon();
                }
            } else {
                icon = parent_->empty_icon();
            }
            if (icon) {
                W += icon->w() + 2;
            }
        } else if (parent_->draw_lines()) {
            W += 12;
        }
        if (pixmap_) { // room for the item's pixmap
            W += ( pixmap_->w() + parent_->pixmap_offset() + 2 );
        }
        if (label_) { // room for the item's label
            fl_font(font_,font_size_);
            fl_measure(label_,wl,hl);
            W += wl + parent_->label_offset();
        }
        W += 6 + level_ * parent_->level_width();
    }
    return W;
}

int Fl_Tree_Item::height(void)
{
int wl=0, hl=0, hp=0, hi=0;
int H=0;
Fl_Pixmap *icon=0;

    if (visible_) {
        if (label_) {
            fl_font(font_,font_size_);
            fl_measure(label_,wl,hl);
        }
        if (childs_) {
            if (!next_ || next_->level() <= level_) {
                if (opened_ != (this == parent_->pushedtitle())) {
                    icon = parent_->open_icon();
                } else {
                    icon = parent_->closed_icon();
                }
            } else {
                icon = parent_->empty_icon();
            }
            if (icon) {
                hi = icon->h();
            }
        }
        if (pixmap_) {
            hp=pixmap_->h();
        }
        if (hi>hp) {
            hp = hi;
        }
        if (hl>0 && (hl>hp || hl==hp)) {
            H = hl + 1;
        } else if (hp>0 && hp>hl) {
            H = hp + 1;
        } else {
            H = 17;
        }
    }
    return H;
}

void Fl_Tree_Item::tooltip(char* ptr)
{
    if (tooltip_) {
        delete tooltip_;
    }
    if (ptr && strlen(ptr)) {
        tooltip_ = strdup(ptr);
    }
}

void Fl_Tree_Item::label(char* ptr)
{
    if (label_) {
        delete label_;
    }
    if (ptr && strlen(ptr)) {
        label_ = strdup(ptr);
    }
}

Fl_Tree_Item::~Fl_Tree_Item(void)
{
Fl_Tree_Item *next;

    if (label_) {
        delete label_;
    }
    if (tooltip_) {
        delete tooltip_;
    }
    if (next_) {
        next_->prev(prev_);
    }
    if (prev_) {
        prev_->next(next_);
    }
    for (
        Fl_Tree_Item *child = childs_;
        child;
        child = next
    ) {
        next = child->next();
        delete child;
    }
}

Fl_Tree_Item::Fl_Tree_Item (
    const char      *label,
    Fl_Pixmap       *pixmap,
    const char      *tooltip,
    void            *data,
    Fl_Color         color,
    int              font,
    int              font_size,
    bool             can_open,
    bool             opened,
    Fl_Tree_Browser *parent,
    Fl_Tree_Item    *father,
    Fl_Tree_Item    *prev,
    Fl_Tree_Item    *next
) {
    if (next) {
        next->prev(this);
    }
    if (prev) {
        prev->next(this);
    }
    prev_      = prev;
    next_      = next;
    father_    = father;
    parent_    = parent;

    childs_    = 0;

    level_     = 0;

    if (father) {
        level_ = father->level() + 1;
        if (!father->childs()) {
            father->childs(this);
        }
    }

    visible_   = true;

    selected_  = false;

    opened_    = opened;

    color_     = color;
    font_      = font;
    font_size_ = font_size;

    can_open_  = can_open;

    data_      = data;

    pixmap_    = pixmap;
    label_     = 0;
    tooltip_   = 0;

    if (label && strlen(label)) {
        label_   = strdup(label);
    }
    if (tooltip && strlen(tooltip)) {
        tooltip_ = strdup(tooltip);
    }
}
