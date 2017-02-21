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
#ifndef Fl_Sorted_Choice_H
#define Fl_Sorted_Choice_H

#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Tree_Browser.H>

class Fl_Sorted_Menu_Item;

class Fl_Sorted_Menu_Item
{
public:
    Fl_Sorted_Menu_Item (
        bool ascending_sort=true,
        const Fl_Menu_Item *mi=0,
        bool selected=false,
        Fl_Sorted_Menu_Item *next=0,
        Fl_Sorted_Menu_Item *prev=0
    );
    ~Fl_Sorted_Menu_Item();

    Fl_Sorted_Menu_Item *add_item(const Fl_Menu_Item *mi, bool selected=false);
    const Fl_Menu_Item *add (
        const Fl_Menu_Item *mi,
        const Fl_Menu_Item *mis=0,
        int level=0
    );
    const Fl_Menu_Item *dump(const Fl_Menu_Item *mi, Fl_Menu_ *menu=0);

    void next(Fl_Sorted_Menu_Item *next);
    void prev(Fl_Sorted_Menu_Item *prev);
    Fl_Sorted_Menu_Item *next(void);
    Fl_Sorted_Menu_Item *prev(void);
    Fl_Sorted_Menu_Item *childs(void);
    Fl_Menu_Item *item(void);
    bool selected(void);

private:
    bool ascending_;
    Fl_Sorted_Menu_Item *prev_;
    Fl_Sorted_Menu_Item *next_;
    Fl_Sorted_Menu_Item *childs_;
    Fl_Sorted_Menu_Item *lastChild_;
    Fl_Menu_Item item_;
    bool selected_;
};

class Fl_Sorted_Choice;

class ListWindow : public Fl_Menu_Window
{
private:
    Fl_Tree_Browser *tTree;

    static void selection_cb(Fl_Widget *w, void *d);
    void selection_cb(Fl_Widget *w);

    void *add (
        Fl_Sorted_Choice *parent,
        const Fl_Tree_Item *father,
        const Fl_Menu_Item *mi,
        int level
    );

public:
    ListWindow(Fl_Sorted_Choice *parent, const Fl_Menu_Item *menu);

    int handle(int);

    Fl_Menu_Item *selected(void);
};

class Fl_Sorted_Choice : public Fl_Choice
{
private:
    Fl_Pixmap* folder_pixmap_;
    Fl_Pixmap* item_pixmap_;

    const Fl_Menu_Item *pulldown(void);

    const Fl_Menu_Item *prev_item(const Fl_Menu_Item *mi);
    const Fl_Menu_Item *next_item(const Fl_Menu_Item *mi);

public:
    Fl_Sorted_Choice(int x,int y,int w,int h,const char *l=0);
    void sort(bool ascending=true);
    int handle(int e);

    void folder_pixmap(Fl_Pixmap *p) { folder_pixmap_ = p; }
    void item_pixmap(Fl_Pixmap *p)   { item_pixmap_ = p; }

    Fl_Pixmap *folder_pixmap(void) { return folder_pixmap_; }
    Fl_Pixmap *item_pixmap(void)   { return item_pixmap_; }
};

#endif
