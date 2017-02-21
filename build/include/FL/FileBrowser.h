//
// Include necessary header files...
//

#ifndef _GUI_FILEBROWSER_H_
#  define _GUI_FILEBROWSER_H_

#  include <FL/Fl_File_Browser.H>


//
// FileBrowser class...
//

class FileBrowser : public Fl_File_Browser
{
  const char	*cur_directory_;

public:
  FileBrowser(int x, int y, int w, int h, const char *l = 0)
  : Fl_File_Browser(x,y,w,h,l)
  {
      cur_directory_ = (char*)0;
  }
  int load(const char *directory, Fl_File_Sort_F *sort = fl_numericsort)
  {
      cur_directory_ = directory;
      this->Fl_File_Browser::load(directory,sort);
  }
  const char *directory() const { return (cur_directory_); };
};

#endif // !_GUI_FILEBROWSER_H_

//
// End of "$Id: FileBrowser.h,v 1.10 2000/01/04 13:45:51 mike Exp $".
//
