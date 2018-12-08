# pkg-config.mk -- build tools for Click
# Eddie Kohler
#
# Copyright (c) 2006 Regents of the University of California
# Copyright (c) 2008 Meraki, Inc.
# Copyright (c) 2011 Eddie Kohler
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, subject to the conditions
# listed in the Click LICENSE file. These conditions include: you must
# preserve this copyright notice, and you cannot mention the copyright
# holders in advertising related to the Software without their permission.
# The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
# notice is a summary of the Click LICENSE file; the license in that file is
# legally binding.

CLICKCC = arm-linux-gnueabi-gcc
CLICKCPP = arm-linux-gnueabi-gcc -E
CLICKCXX = arm-linux-gnueabi-g++ -D __i386__
CLICKCXXCPP = arm-linux-gnueabi-g++ -D __i386__ -E

CLICKAR_CREATE = arm-linux-gnueabi-ar cru
CLICKRANLIB = arm-linux-gnueabi-ranlib
CLICKSTRIP = arm-linux-gnueabi-strip

CLICKKERNEL_CC = arm-linux-gnueabi-gcc
CLICKKERNEL_CXX = arm-linux-gnueabi-g++ -D __i386__
CLICKLINUX_BUILDDIR = NONE
CLICKLINUX_SRCDIR = NONE
CLICKLINUX_MAKEARGS = 
CLICKLINUX_FIXINCLUDES_PROGRAM = 

CLICKCPPFLAGS = 
CLICKCFLAGS = -g -O2 -W -Wall
CLICKKERNEL_CFLAGS =  -O2 -Wno-undef
CLICKCXXFLAGS = -g -O2 -W -Wall
CLICKKERNEL_CXXFLAGS =  -O2 -fno-exceptions -fno-rtti -fpermissive -Wno-undef -Wno-pointer-arith
CLICKDEPCFLAGS = -MD -MP

CLICKDEFS = -DHAVE_CONFIG_H
CLICKINCLUDES = -I$(clickbuild_includedir) -I$(clickbuild_srcdir) 
CLICKLDFLAGS = 
CLICKLDMODULEFLAGS = 

CLICKAUTOCONF = autoconf
CLICKGMAKE = make
CLICKINSTALL = /usr/bin/install -c

