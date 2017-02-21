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
#ifndef Fl_Tree_Browser_H
#define Fl_Tree_Browser_H

#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Browser_.H>

class Fl_Tree_Browser;

class Fl_Tree_Item
{
protected:
    Fl_Tree_Browser *parent_;

    Fl_Tree_Item *prev_;
    Fl_Tree_Item *next_;
    Fl_Tree_Item *father_;
    Fl_Tree_Item *childs_;

    int level_;

    bool opened_;
    bool selected_;
    bool can_open_;
    bool visible_;

    Fl_Color color_;
    int font_;
    int font_size_;

    char* label_;
    char* tooltip_;
    Fl_Pixmap* pixmap_;

    void* data_;

public:
    Fl_Tree_Item (
        const char      *label     = 0,
        Fl_Pixmap       *pixmap    = 0,
        const char      *tooltip   = 0,
        void            *data      = 0,
        Fl_Color         color     = FL_BLACK,
        int              font      = 0,
        int              font_size = 0,
        bool             can_open  = true,
        bool             opened    = true,
        Fl_Tree_Browser *parent    = 0,
        Fl_Tree_Item    *father    = 0,
        Fl_Tree_Item    *prev      = 0,
        Fl_Tree_Item    *next      = 0
    );

    ~Fl_Tree_Item();

    int height();
    int width();
    void draw(int X,int Y);
    void draw_connection_line(int X,int Y,int H);

    void prev(Fl_Tree_Item *p)   { prev_ = (Fl_Tree_Item *)p; }
    Fl_Tree_Item *prev()         { return prev_  ; }

    void next(Fl_Tree_Item *p)   { next_ = (Fl_Tree_Item *)p; }
    Fl_Tree_Item *next()         { return next_  ; }

    void father(Fl_Tree_Item *p) { father_ = (Fl_Tree_Item *)p; }
    Fl_Tree_Item *father()       { return father_; }

    void childs(Fl_Tree_Item *p) { childs_ = (Fl_Tree_Item *)p; }
    Fl_Tree_Item *childs()       { return childs_; }

    void label(char* p);
    char* label()             { return label_  ; }

    void tooltip(char* p);
    char* tooltip()           { return tooltip_; }

    void pixmap(Fl_Pixmap* p) { pixmap_ = p; }
    Fl_Pixmap* pixmap()       { return pixmap_; }

    void data(void* v)        { data_ = v; }
    void* data() const        { return data_; }

    void can_open (bool b)    { can_open_ = b; }
    bool can_open()           { return can_open_; }

    void selected(bool b)     { selected_ = b; }
    bool selected()           { return selected_; }

    void opened(bool b)       { opened_ = b; }
    bool opened()             { return opened_; }

    void color(Fl_Color c)    { color_ = c; }
    Fl_Color color()          { return color_; }

    void visible(bool b)       { visible_ = b; }
    bool visible()             { return visible_; }

    int level() { return level_; }
};

class Fl_Tree_Browser : public Fl_Browser_
{

private:
    // friend class Fl_Tree_Item;

    Fl_Tree_Item *items_;

    // required routines for Fl_Browser_ subclass:
    void *item_first() const ;
    void *item_next(void *) const ;
    void *item_prev(void *) const ;
    int item_width(void *) const ;
    int item_height(void *) const ;
    void item_draw(void *,int,int,int,int) const ;
    void item_select(void *p,int s=1);
    int item_selected(void *p) const;
    int incr_height() const ;
    int full_height() const ;
    int full_width() const ;

    Fl_Pixmap* open_icon_;
    Fl_Pixmap* closed_icon_;
    Fl_Pixmap* empty_icon_;

    static Fl_Pixmap* s_open_icon_;
    static Fl_Pixmap* s_closed_icon_;
    static Fl_Pixmap* s_empty_icon_;

    Fl_Tree_Item *pushedtitle_;

    bool draw_lines_;
    int label_offset_;
    int pixmap_offset_;
    int level_width_;

public:
    Fl_Tree_Browser(int,int,int,int,const char* = 0);
    ~Fl_Tree_Browser();

    int handle(int e);

    void items(Fl_Tree_Item *i) { clear(); items_ = i; }
    Fl_Tree_Item *items() { return items_; }

    void clear();

    void open_icon(Fl_Pixmap *p);
    Fl_Pixmap *open_icon() { return open_icon_; }

    void closed_icon(Fl_Pixmap *p);
    Fl_Pixmap *closed_icon() { return closed_icon_; }

    void empty_icon(Fl_Pixmap *p);
    Fl_Pixmap *empty_icon() { return empty_icon_; }

    void draw_lines(bool b) { draw_lines_ = b; }
    bool draw_lines() { return draw_lines_; }

    void label_offset(int v) { label_offset_ = v; }
    int label_offset() { return label_offset_; }

    void pixmap_offset(int v) { pixmap_offset_ = v; }
    int pixmap_offset() { return pixmap_offset_; }

    int height() { return full_height(); }
    int width() { return full_width(); }

    void *selected() { return selection(); }

    Fl_Tree_Item *pushedtitle() { return pushedtitle_; }

    int level_width();
};

#endif
