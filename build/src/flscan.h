// generated by Fast Light User Interface Designer (fluid) version 1.0305

#ifndef flscan_h
#define flscan_h
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include "C_Sane.H"
#include "flscan_thread.h"
#include <Threads.H>
extern Trigger running; 
extern C_Sane *sane; 
extern char *sane_host; 
extern const char *copyrightText;

class EatEvent : public Fl_Box {
public:
  EatEvent(int x,int y,int w,int h,const char *l=NULL);
  int handle(int e);
};

class ImageSavingMonitor : public Thread {
public:
  void thread();
  ImageSavingMonitor();
};
void auth_callback(SANE_String_Const domain,SANE_Char username[SANE_MAX_USERNAME_LEN],SANE_Char password[SANE_MAX_PASSWORD_LEN]);
#include <FL/Fl_Double_Window.H>
extern Fl_Double_Window *FlScan;
#include <FL/Fl_Group.H>
extern Fl_Group *FlScanContainer;
#include <FL/Fl_Tree_Browser.H>
extern Fl_Tree_Browser *FlScanScannerOptions;
extern Fl_Group *FlScanSettings;
extern Fl_Group *FlScanMisc;
#include <FL/Fl_Sorted_Choice.H>
extern Fl_Sorted_Choice *FlScanScanFileType;
#include <FL/Fl_Input.H>
extern Fl_Input *FlScanTxFileName;
#include <FL/Fl_Box.H>
extern Fl_Box *FlScanLbSeparator;
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
extern Fl_Button *FlScanPbFileSelector;
extern Fl_Button *FlScanPbPreview;
extern Fl_Button *FlScanPbScan;
extern Fl_Button *FlScanPbReset;
extern Fl_Button *FlScanPbCancel;
extern Fl_Group *FlScanSetOption;
extern Fl_Button *FlScanPbAutoOption;
extern Fl_Button *FlScanPbSetOption;
extern Fl_Group *FlScanBoolGroup;
#include <FL/Fl_Light_Button.H>
extern Fl_Light_Button *FlScanTbBoolType;
extern Fl_Group *FlScanFixedGroup;
#include <FL/Fl_Float_Input.H>
extern Fl_Float_Input *FlScanTxFixedType;
extern Fl_Group *FlScanIntGroup;
#include <FL/Fl_Int_Input.H>
extern Fl_Int_Input *FlScanTxIntType;
extern Fl_Group *FlScanStringGroup;
extern Fl_Input *FlScanTxStringType;
extern Fl_Group *FlScanButtonGroup;
extern Fl_Button *FlScanPbButtonType;
extern Fl_Group *FlScanRangeGroup;
#include <FL/Fl_Slider.H>
extern Fl_Slider *FlScanSlRangeType;
#include <FL/Fl_Value_Input.H>
extern Fl_Value_Input *FlScanViRangeType;
extern Fl_Group *FlScanListGroup;
extern Fl_Sorted_Choice *FlScanMnListType;
extern Fl_Group *FlScanMessage;
extern Fl_Group *FlScanTableGroup;
#include <FL/Fl_RaiseButton.H>
extern Fl_RaiseButton *FlScanRbSplineTable;
extern Fl_RaiseButton *FlScanRbLinearTable;
extern Fl_RaiseButton *FlScanRbFreeTable;
extern Fl_RaiseButton *FlScanPbGammaTable;
extern Fl_Float_Input *FlScanTxGammaTable;
extern Fl_RaiseButton *FlScanPbResetTable;
extern Fl_Group *FlScanGroupTableType;
#include <FL/Fl_Curve.H>
extern Fl_Curve *FlScanTableGammaCurve;
extern Fl_Group *FlScanOutputResolutions;
#include <FL/Fl_Browser.H>
extern Fl_Browser *FlScanLsOutputGeometries;
extern Fl_Sorted_Choice *FlScanOmZoomFilter;
extern Fl_Group *FlScanHistogram;
extern Fl_Button *FlScanHistogramPbIntensity;
extern Fl_Button *FlScanHistogramPbRed;
extern Fl_Button *FlScanHistogramPbBlue;
extern Fl_Button *FlScanHistogramPbGreen;
extern Fl_Button *FlScanHistogramPbLog;
extern Fl_Button *FlScanHistogramPbPixel;
extern Fl_Group *FlScanHistogramCanvas;
extern Fl_Light_Button *FlScanHistogramTgIntensity;
extern Fl_Light_Button *FlScanHistogramTgRed;
extern Fl_Light_Button *FlScanHistogramTgGreen;
extern Fl_Light_Button *FlScanHistogramTgBlue;
extern Fl_Light_Button *FlScanHistogramTgPixel;
extern Fl_Light_Button *FlScanHistogramTgLog;
extern Fl_Group *FlScanSetupJPEG;
extern Fl_Int_Input *FlScanFFmtTxQltJPEG;
extern Fl_Slider *FlScanFFmtSlQltJPEG;
extern Fl_Int_Input *FlScanFFmtTxSmtJPEG;
extern Fl_Slider *FlScanFFmtSlSmtJPEG;
extern Fl_Light_Button *FlScanFFmtTbOptJPEG;
extern Fl_Box *FlScanLbJPEGSeparator;
extern Fl_Int_Input *FlScanFFmtTxCompressionPNG;
extern Fl_Slider *FlScanFFmtSlCompressionPNG;
extern Fl_Group *FlScanSaneLogo;
extern Fl_Button *showCopyright;
extern Fl_Group *FlScanImageSelector;
#include "FL/FileBrowser.h"
extern FileBrowser *FlScanFileBrowser;
#include <FL/Fl_Progress.H>
extern Fl_Progress *FlScanProgressBar;
#include <FL/Fl_Tabs.H>
extern Fl_Tabs *FlScanTabs;
extern Fl_Group *FlScanTabThumbnails;
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>
extern Fl_Pack *FlScanThumbsRows;
extern Fl_Group *FlScanTabPreview;
#include <FL/Fl_Image_Preview.H>
extern Fl_Image_Preview *FlScanPreviewArea;
extern Fl_Group *FlScanTabConversion;
#include <Conversions_List.H>
extern Conversions_List *FlScanConversionArea;
extern Fl_Group *FlScanTabView;
#include <FL/Fl_Scrolled_Image.H>
extern Fl_Scrolled_Image *FlScanSwViewArea;
#include <FL/Fl_Image_View.H>
extern Fl_Image_View *FlScanViewArea;
extern Fl_Group *FlScanToolBar;
extern Fl_Group *FlScanPreviewTools;
extern Fl_RaiseButton *FlScanThumbnailsRotateCCW;
extern Fl_RaiseButton *FlScanThumbnailsRotateCW;
extern Fl_RaiseButton *FlScanThumbnailsSetHorizont;
extern Fl_RaiseButton *FlScanThumbnailsUndoRotate;
extern Fl_RaiseButton *FlScanThumbnailsHorizontalMirror;
extern Fl_RaiseButton *FlScanThumbnailsVerticalMirror;
extern Fl_RaiseButton *FlScanPreviewZoomPlus;
extern Fl_RaiseButton *FlScanPreviewZoomMinus;
extern Fl_RaiseButton *FlScanPreviewZoomScreen;
extern Fl_RaiseButton *FlScanPreviewZoomMinimize;
extern EatEvent *FlScanEeLockView;
extern EatEvent *FlScanEeWaitForSaving;
Fl_Double_Window* makeFlSCAN(bool enable_scan);
extern unsigned char menu_FlScanScanFileType_i18n_done;
extern Fl_Menu_Item menu_FlScanScanFileType[];
#define FlScanScanFileTypeJPEG (menu_FlScanScanFileType+0)
#define FlScanScanFileTypePNG (menu_FlScanScanFileType+1)
#define FlScanScanFileTypePNM (menu_FlScanScanFileType+2)
#define FlScanScanFileTypeRAW (menu_FlScanScanFileType+3)
#define FlScanScanFileTypeTIFF (menu_FlScanScanFileType+4)
extern unsigned char menu_FlScanOmZoomFilter_i18n_done;
extern Fl_Menu_Item menu_FlScanOmZoomFilter[];
void FlScanSetMode(int mode);
void FlScanSliderGlue(Fl_Valuator *slider, Fl_Input *text,bool textToSlider);
void FlScanResize(int how);
void cb_selectoption(Fl_Widget *w, void *v);
void update_tree();
void FlScanSaneSetCB(bool automatic);
void FlScanSelOption();
void FlScanShowOption(Fl_Group *w,const char *l,const char *t);
char * FlScanMenuize(const char *str);
void FlScanChoiceCB(Fl_Widget *w,void *d);
void FlScanTableSetGamma();
void FlScanScannerCB(int fd,void *d);
bool FlScanPreviewCB(void *d,int l,int m);
void FlScanUpdateSelection();
void FlScanUpdateProgress(const char *msg,bool working);
bool FlScanScanCB(void *d,int l,int m);
void dump_sane();
void FlScanSaveProgressCB(int prog);
char* FlScanGetCurrentDeviceConfigFileName(void);
bool FlScanValidateOptionSaving(char *optname);
void FlScanSaveCurrentDevice(void);
void FlScanLoadCurrentDevice(void);
void FlScanUpdateCounterInFilename(char **filename,int skip,int step,int min_counter_len);
void FlScanFillInGeometries(Fl_Browser *o);
void FlScanSaveSettings(void);
void FlScanLoadSettings(void);
void FlScanUpdateDirectory(void);
void FlScanChangeDirectory();
void FlScanShowImage(char *pname);
bool FlScanReadImageCB(unsigned char *row,int w,int d,int c,int m);
bool FlScanCheckForImagesNotSaved();
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Help_View.H>
Fl_Double_Window* make_copyright_window(const char *copyright);
extern Fl_Double_Window *wDevList;
extern Fl_Browser *lsDevList;
void FlScanDeviceSelection(void);
#endif
