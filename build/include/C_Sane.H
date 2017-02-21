//
// Copyright 2001-2002 by Francesco Bradascio.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fbradasc@katamail.com".
//

#ifndef C_Sane_H
#define C_Sane_H

/*****************************************************************************

                                    SANE FLOW

- sane_init()
  // pick desired device, possibly by using
    - sane_get_devices()
  - sane_open()
  // device setup
    // repeatedly to configure device as desired
    do
      - sane_get_option_description()
      - sane_control_option()
    done
  // image acquisition
    // repeadetly for each frame to acquire
    do
      - sane_start()
      // repeatedly until read returns EOF
      do
        - sane_get_parameters()
        - sane_read()
      done
    done
    - sane_cancel()
  - sane_close()
- sane_exit()

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
    #include <sane/sane.h>
    #include <sane/saneopts.h>
}

typedef bool (*C_Sane_ScanCB)(void *data,int data_len,int mode);

struct C_Sane_Status
{
private:
    SANE_Status status_;
    SANE_Int info_;

public:
    C_Sane_Status();
    C_Sane_Status(C_Sane_Status& s);
    C_Sane_Status(SANE_Status s);
    C_Sane_Status(SANE_Int i);
    operator SANE_String_Const();
    operator SANE_Status();
    operator SANE_Int();
    operator char*();

    void operator =(SANE_Status s);
    void operator =(SANE_Int i);
    void operator =(C_Sane_Status& s);
    bool operator ==(SANE_Status c);
    bool operator ==(SANE_Int c);
    bool operator !=(SANE_Status c);
    bool operator !=(SANE_Int c);
};
class C_Sane_Value
{
private:
    void *value_;

public:
    C_Sane_Value();
    C_Sane_Value(void *value);
    C_Sane_Value(C_Sane_Value& value);

    SANE_Bool toBool();
    SANE_Int toInt();
    double toDouble();
    char *toString();
    void *toVoidP();

    void* operator =(void *value);
    void* operator =(C_Sane_Value& value);

    operator bool   ();
    operator int    ();
    operator double ();
    operator char*  ();
    operator void*  ();
};
class C_Sane_Option
{
private:
    SANE_Int number_;
    const SANE_Option_Descriptor *descriptor_;
    void *value_;
    SANE_Handle handle_;
    C_Sane_Status status_;

    C_Sane_Status control(SANE_Action action);
    void *value(void *val,SANE_Action action);
public:
    C_Sane_Option(SANE_Handle handle=0, SANE_Int opt_num=0);
    ~C_Sane_Option();
    void handle(SANE_Handle hndl);
    SANE_Int number();
    void number(SANE_Int opt_num);
    const SANE_Option_Descriptor *descriptor();
    const SANE_Option_Descriptor *descriptor(SANE_Int opt_num);
    C_Sane_Status status();
    void *value();
    void *value(void *val);
    void *auto_value(void *val);
    char* string_value();
};
class C_Sane
{
private:
    SANE_Handle handle_;
    SANE_Auth_Callback auth_cb_;
    SANE_Int device_;
    char *config_dir_;
    char *hostname_;
    char *devicename_;
    SANE_Int version_;
    C_Sane_Status status_;
    SANE_Parameters params_;
    const SANE_Device **devices_;
    SANE_Int num_devices_;
    C_Sane_Option option_;
    SANE_Int num_options_;

    void devicename(char *dev);

public:
    enum {
        USER_MODE=-1,
        START_FRAME=0,
        END_OF_DATA,
        NEW_DATA,
        DATA_ERROR
    };

    C_Sane(char *devname, char *host=0, SANE_Auth_Callback auth_cb=0);
    ~C_Sane();
    void exit();
    void auth_callback(SANE_Auth_Callback auth_cb);
    void hostname(char *host);
    C_Sane_Status status();
    void init();
    SANE_Handle handle();
    void open(const SANE_Char *devname=0);
    SANE_Int num_options();
    C_Sane_Option *option(SANE_Int opt_num);
    C_Sane_Option *option(char *opt_name);
    SANE_Int num_devices();
    const SANE_Device **devices();
    const SANE_Device *device();
    const SANE_Device *device(char *dev);
    const SANE_Device *device(int index);
    SANE_Parameters *parameters();
    void close();
    void start();
    void cancel();
    SANE_Int get_select_fd();
    void set_non_blocking(SANE_Bool non_blocking);
    SANE_Int read(SANE_Byte *data,SANE_Int max_length);
    SANE_Int read(char *data,int max_length);
    bool scan(char *buff,int buf_len,C_Sane_ScanCB scan_cb);
    bool scanner(char *buff,int buf_len,C_Sane_ScanCB scan_cb);
    void save(FILE *,bool (*)(char*));
    void load(FILE *,bool (*)(char*));
};
#endif /* C_Sane_H */
