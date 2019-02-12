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

#include "C_Sane.H"

C_Sane_Status::C_Sane_Status()
{
    status_=SANE_STATUS_GOOD;
    info_=0;
}
C_Sane_Status::C_Sane_Status(C_Sane_Status& s)
{
    info_=(SANE_Int)s;
    status_=(SANE_Status)s;
}
C_Sane_Status::C_Sane_Status(SANE_Status s)
{
    status_=s;
    info_=0;
}
C_Sane_Status::C_Sane_Status(SANE_Int i)
{
    status_=SANE_STATUS_GOOD;
    info_=i;
}
C_Sane_Status::operator SANE_String_Const()
{
    return sane_strstatus(this->status_);
}
C_Sane_Status::operator SANE_Status()
{
    return this->status_;
}
C_Sane_Status::operator SANE_Int()
{
    return this->info_;
}
C_Sane_Status::operator char*()
{
    return (char*)sane_strstatus(this->status_);
}

void C_Sane_Status::operator =(SANE_Status s)
{
    this->status_=s;
}
void C_Sane_Status::operator =(SANE_Int i)
{
    this->info_=i;
}
void C_Sane_Status::operator =(C_Sane_Status& s)
{
    this->info_=(SANE_Int)s;
    this->status_=(SANE_Status)s;
}
bool C_Sane_Status::operator ==(SANE_Status c)
{
    return ( this->status_ == c );
}
bool C_Sane_Status::operator ==(SANE_Int c)
{
    return ( this->info_ == c );
}
bool C_Sane_Status::operator !=(SANE_Status c)
{
    return ( this->status_ != c );
}
bool C_Sane_Status::operator !=(SANE_Int c)
{
    return ( this->info_ != c );
}

C_Sane_Value::C_Sane_Value()
{
    value_=0;
}
C_Sane_Value::C_Sane_Value(void *value)
{
    value_=value;
}
C_Sane_Value::C_Sane_Value(C_Sane_Value& value)
{
    value_=(void*)value;
}

SANE_Bool C_Sane_Value::toBool()
{
    return *(SANE_Bool*)value_;
}
SANE_Int C_Sane_Value::toInt()
{
    return *(SANE_Bool*)value_;
}
double C_Sane_Value::toDouble()
{
    return SANE_UNFIX(*(SANE_Fixed*)value_);
}
char *C_Sane_Value::toString()
{
    return (char*)value_;
}
void *C_Sane_Value::toVoidP()
{
    return value_;
}

void* C_Sane_Value::operator =(void *value)
{
    return (value_=value);
}
void* C_Sane_Value::operator =(C_Sane_Value& value)
{
    return (value_=(void*)value);
}

C_Sane_Value::operator bool()
{
    return this->toBool();
}
C_Sane_Value::operator int()
{
    return this->toInt();
}
C_Sane_Value::operator double()
{
    return this->toDouble();
}
C_Sane_Value::operator char*()
{
    return this->toString();
}
C_Sane_Value::operator void*()
{
    return this->toVoidP();
}

C_Sane_Status C_Sane_Option::control(SANE_Action action)
{
SANE_Int info;

    status_ = SANE_STATUS_INVAL; // save in C_Sane_Status::status_
    if (number_>=0) {
        status_ = sane_control_option (
            handle_,
            number_,
            action,
            value_,
            &info
        );
        status_=info; // save in C_Sane_Status::info_
    }
    return status_;
}
void *C_Sane_Option::value(void *val,SANE_Action action)
{
    if (value_) {
        value_=0;
    }
    status_ = SANE_STATUS_INVAL;
    if (descriptor()) {
        value_ = (void *)new char[descriptor_->size];
        if (action!=SANE_ACTION_GET_VALUE) {
            memcpy(value_,(void*)val,descriptor_->size);
        }
        control(action);
    }
    return value_;
}
C_Sane_Option::C_Sane_Option(SANE_Handle handle, SANE_Int opt_num):
    number_(opt_num),
    handle_(handle),
    value_(0)
{
}
C_Sane_Option::~C_Sane_Option()
{
}
void C_Sane_Option::handle(SANE_Handle hndl)
{
    handle_=hndl;
}
SANE_Int C_Sane_Option::number()
{
    return number_;
}
void C_Sane_Option::number(SANE_Int opt_num)
{
    number_ = opt_num;
}
const SANE_Option_Descriptor *C_Sane_Option::descriptor()
{
    descriptor_ = 0;
    if (number_>=0) {
        descriptor_ = sane_get_option_descriptor (
            handle_,
            number_
        );
    }
    return descriptor_;
}
const SANE_Option_Descriptor *C_Sane_Option::descriptor(SANE_Int opt_num)
{
    number(opt_num);
    return descriptor();
}
C_Sane_Status C_Sane_Option::status()
{
    return status_;
}
void *C_Sane_Option::value()
{
    return value((void*)0,SANE_ACTION_GET_VALUE);
}
void *C_Sane_Option::value(void *val)
{
    return value(val,SANE_ACTION_SET_VALUE);
}
void *C_Sane_Option::auto_value(void *val)
{
    return value(val,SANE_ACTION_SET_AUTO);
}
char* C_Sane_Option::string_value()
{
C_Sane_Value val(value());
static char buff[256];

    buff[0]='\0';
    switch (descriptor_->type) {
        case SANE_TYPE_BOOL:
            sprintf(buff,"%s",((bool)val)?"ON":"OFF");
            break;
        case SANE_TYPE_INT:
            sprintf(buff,"%d",(int)val);
            break;
        case SANE_TYPE_FIXED:
            sprintf(buff,"%g",(double)val);
            break;
        case SANE_TYPE_STRING:
            sprintf(buff,"%s",(char*)val);
            break;
        default:
            break;
    }
    return buff;
}

void C_Sane::devicename(char *dev)
{
    if (devicename_) {
      free(devicename_);
    }
    if (dev && strlen(dev)) {
        devicename_=strdup(dev);
    }
}
C_Sane::C_Sane(char *devname, char *host, SANE_Auth_Callback auth_cb)
{
    handle_      = 0;
    auth_cb_     = 0;
    device_      = 0;
    config_dir_  = 0;
    hostname_    = 0;
    devicename_  = 0;
    version_     = 0;
    devices_     = 0;
    num_devices_ = 0;
    num_options_ = 0;
    hostname(host);
    auth_callback(auth_cb);
    init();
    open(devname);
}
C_Sane::~C_Sane()
{
    if (hostname_) {
      free(hostname_);
    }
    sane_exit();
}
void C_Sane::exit()
{
    cancel();
    close();
    sane_exit();
    device_=-1;
    num_devices_=0;
    num_options_=0;
}
void C_Sane::auth_callback(SANE_Auth_Callback auth_cb)
{
    auth_cb_=auth_cb;
}
void C_Sane::hostname(char *host)
{
char envbuf[1024];

    if (hostname_) {
        free(hostname_);
        hostname_=0;
    }
    if (host) {
        hostname_=strdup(host);
    }
    if (hostname_) {
        sprintf(envbuf,"SANE_NET_HOST=%s",hostname_);
    } else {
        sprintf(envbuf,"SANE_NET_HOST=");
    }
    putenv(envbuf);
}
C_Sane_Status C_Sane::status()
{
    return status_;
}
void C_Sane::init()
{
    devices_=0;
    num_devices_=0;
    status_ = SANE_STATUS_INVAL;
    if (auth_cb_) {
        status_ = sane_init(&version_,auth_cb_);
        if (status_ == SANE_STATUS_GOOD) {
            status_ = sane_get_devices(&devices_,SANE_FALSE);
            if (status_ == SANE_STATUS_GOOD) {
                for (
                    num_devices_=0;
                    devices_[num_devices_];
                    ++num_devices_
                );
                if (devices_ && devices_[0]) {
                    //
                    // Set the first device found as default device
                    //
                    devicename((devices_[0]) ? (char*)devices_[0]->name : 0);
                } else {
                    status_ = SANE_STATUS_INVAL;
                }
            }
        }
    }
}
SANE_Handle C_Sane::handle()
{
    return handle_;
}
void C_Sane::open(const SANE_Char *devname)
{
    status_ = SANE_STATUS_INVAL;
    exit();
    init();
    if (status_ == SANE_STATUS_GOOD) {
        if (devname && strlen(devname)) {
            for (int i=0; devices_[i]; i++) {
                if (!strcmp(devname,devices_[i]->name)) {
                    devicename((char*)devname);
                    break;
                }
            }
        }
        if (devicename_) {
            status_ = sane_open(devicename_,&handle_);
            if (status_ != SANE_STATUS_GOOD) {
                handle_=0;
            }
            option_.handle(handle_);
            if (option_.descriptor(0)) {
                num_options_=(SANE_Int)C_Sane_Value(option_.value());
            }
/*
            for (
                num_options_=0;
                option_.descriptor(num_options_);
                num_options_++
            ) {
                if (option_.status() != SANE_STATUS_GOOD) {
                    break;
                }
            }
*/
        }
    }
}
SANE_Int C_Sane::num_options()
{
    return num_options_;
}
C_Sane_Option *C_Sane::option(SANE_Int opt_num)
{
C_Sane_Option *opt=0;

    status_ = SANE_STATUS_INVAL;
    if (num_options_ && opt_num>=0 && opt_num<num_options_) {
        option_.number(opt_num);
        opt = &option_;
    }
    return opt;
}
C_Sane_Option *C_Sane::option(char *opt_name)
{
int i,last_opt;
SANE_String_Const name;
C_Sane_Option *opt=0;

    if (opt_name && strlen(opt_name)) {
        last_opt=option_.number();
        status_ = SANE_STATUS_INVAL;
        for (i=0;i<num_options_ && option_.descriptor(i);i++) {
            name = option_.descriptor()->name;
            if (name && !strcmp(name,opt_name)) {
                opt = &option_;
                break;
            }
        }
        if (!opt) {
            option_.descriptor(last_opt);
        }
    }
    return opt;
}
SANE_Int C_Sane::num_devices()
{
    return num_devices_;
}
const SANE_Device **C_Sane::devices()
{
    return devices_;
}
const SANE_Device *C_Sane::device()
{
    return device(devicename_);
}
const SANE_Device *C_Sane::device(char *dev)
{
    if (devices_ && dev) {
        for (int i=0;devices_[i];++i) {
            if (!strncmp(devices_[i]->name,dev,strlen(dev))) {
                device_=i;
                break;
            }
        }
    }
    return device(device_);
}
const SANE_Device *C_Sane::device(int index)
{
const SANE_Device *dev=0;

    status_ = SANE_STATUS_GOOD;
    if (
        !devices_              ||
        index<0                ||
        index>num_devices_     ||
        !devices_[index]       ||
        !devices_[index]->name
    ) {
        status_ = SANE_STATUS_INVAL;
    } else {
        device_=index;
        dev=devices_[index];
    }
    return dev;
}
SANE_Parameters *C_Sane::parameters()
{
SANE_Parameters *params=0;

    status_ = SANE_STATUS_INVAL;
    if (handle_) {
        status_ = sane_get_parameters (
            handle_,
            &params_
        );
        if (status_==SANE_STATUS_GOOD) {
            params=&params_;
        }
    }
    return params;
}
void C_Sane::close()
{
    if (handle_) {
        sane_close(handle_);
    }
    handle_=0;
}
void C_Sane::start()
{
    status_ = SANE_STATUS_INVAL;
    if (handle_) {
        status_ = sane_start(handle_);
    }
}
void C_Sane::cancel()
{
    if (handle_) {
        sane_cancel(handle_);
    }
}
SANE_Int C_Sane::get_select_fd()
{
SANE_Int fd=0;

    status_ = SANE_STATUS_INVAL;
    if (handle_) {
        status_ = sane_get_select_fd(handle_,&fd);
    }
    return fd;
}
void C_Sane::set_non_blocking(SANE_Bool non_blocking)
{
    status_ = SANE_STATUS_INVAL;
    if (handle_) {
        status_ = sane_set_io_mode(handle_,non_blocking);
    }
}
SANE_Int C_Sane::read(SANE_Byte *data,SANE_Int max_length)
{
SANE_Int bytes;

    status_ = sane_read(handle_,data,max_length,&bytes);
    return bytes;
}
SANE_Int C_Sane::read(char *data,int max_length)
{
    return read((SANE_Byte *)data,(SANE_Int)max_length);
}
bool C_Sane::scan(char *buff,int buf_len,C_Sane_ScanCB scan_cb)
{
SANE_Int fd;
bool ok=true;

    if (scan_cb) {
        start();
        if (status_!=SANE_STATUS_GOOD) {
            cancel();
            return false;
        }
        parameters();
        if (status_!=SANE_STATUS_GOOD) {
            cancel();
            return false;
        }
        fd=0;
        set_non_blocking(true);
        if (status_==SANE_STATUS_GOOD) {
            fd=get_select_fd();
            if (status_!=SANE_STATUS_GOOD) {
                fd=0;
            }
        }
        if (!scan_cb((void*)&params_,fd,C_Sane::START_FRAME)) {
            ok=false;
        }
    }
    return ok;
}
bool C_Sane::scanner(char *buff,int buf_len,C_Sane_ScanCB scan_cb)
{
SANE_Int len;
bool ok=true;

    if (scan_cb) {
        while (1) {
            len=read(buff,buf_len);
            if (status_!=SANE_STATUS_GOOD) {
                if (status_==SANE_STATUS_EOF) {
                    if (params_.last_frame) {
                        scan_cb(0,0,END_OF_DATA);
                    } else {
                        // recursively get next frame
                        if (!scan(buff,buf_len,scan_cb)) {
                            scan_cb(0,0,C_Sane::DATA_ERROR);
                        }
                        break;
                    }
                } else {
                    ok=false;
                    scan_cb(0,0,C_Sane::DATA_ERROR);
                }
                cancel();
                break;
            }
            if (!len) {
                break;
            }
            if (!scan_cb(buff,len,C_Sane::NEW_DATA)) {
                ok=false;
                break;
            }
        }
    }
    return ok;
}
void C_Sane::save(FILE *pfcfg, bool (*validateCb)(char*))
{
int i;
C_Sane_Option *opt;
const SANE_Option_Descriptor *sod;

    if (pfcfg) {
        for (i = 0; i<num_options() && (opt=option(i)); i++) {
            if (
                (sod=opt->descriptor())             &&
                sod->name                           &&
                SANE_OPTION_IS_SETTABLE(sod->cap)   &&
                /* SANE_OPTION_IS_ACTIVE(sod->cap)     && */
                sod->type != SANE_TYPE_BUTTON       &&
                !(sod->cap & SANE_CAP_AUTOMATIC)    &&
                opt->value()                        &&
                (validateCb && validateCb((char*)sod->name))
            ) {
                fprintf(pfcfg,"%s,%d\n",sod->name,sod->size);
                fwrite((void*)opt->value(),1,sod->size,pfcfg);
            }
        }
    }
}
void C_Sane::load(FILE *pfcfg, bool (*validateCb)(char*))
{
char optname[256],*pch;
int optsize;
void *data;
C_Sane_Option *opt;
const SANE_Option_Descriptor *sod;

    while (pfcfg && !feof(pfcfg)) {
        fgets(optname,256,pfcfg);
        pch=strchr(optname,',');
        optsize=0;
        if (pch) {
            *pch='\0';
            sscanf(pch+1,"%d\n",&optsize);
        }
        if (optsize>0) {
            data=malloc(optsize);
            if (data) {
                fread((void*)data,1,optsize,pfcfg);
                opt=option(optname);
                if (
                    opt                                 &&
                    (sod=opt->descriptor())             &&
                    sod->name                           &&
                    SANE_OPTION_IS_SETTABLE(sod->cap)   &&
                    /* SANE_OPTION_IS_ACTIVE(sod->cap)     && */
                    sod->type != SANE_TYPE_BUTTON       &&
                    !(sod->cap & SANE_CAP_AUTOMATIC)    &&
                    (validateCb && validateCb((char*)sod->name))
                ) {
                    opt->value(data);
                }
                free(data);
            } else {
                /* Skip data block */
                for (int i=0;i<optsize && !feof(pfcfg);i++) {
                    fgetc(pfcfg);
                }
            }
        }
    }
}
