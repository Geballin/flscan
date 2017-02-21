ADD_EXECUTABLE ( flscan

    ../src/Fl_RaiseButton.cxx

    ../src/Flv_CStyle.cxx
    ../src/Flv_List.cxx
    ../src/Flv_Style.cxx
    ../src/Flv_Table.cxx

    ../src/Fl_Sorted_Choice.cxx
    ../src/Fl_Tree_Browser.cxx

    ../src/C_Sane.cxx

    ../src/Conversions_List.cxx
#    ../src/FileIcon.cxx
    ../src/Fl_Curve.cxx
    ../src/Fl_Image_Preview.cxx
    ../src/Fl_Scrolled_Image.cxx
    ../src/Fl_Image_View.cxx
    ../src/Output_File.cxx
    ../src/Image_Stream.cxx
    ../src/JPEG.cxx
    ../src/PNG.cxx
    ../src/zoom.cxx

    ../src/flscan.cxx

    ../src/main_flscan.cxx
)

INSTALL_TARGETS( /bin flscan )
