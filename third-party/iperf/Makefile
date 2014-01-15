
ifeq ($(ROOT_DIR),)
$(error ROOT_DIR is not set.)
endif

ifeq (arm,$(EMBOX_ARCH))
EMBOX_IPERF_TARGET=arm-none-eabi
else ifeq (x86,$(EMBOX_ARCH))
EMBOX_IPERF_TARGET=i386-pc-elf
else ifeq (sparc,$(EMBOX_ARCH))
EMBOX_IPERF_TARGET=sparc-elf
else
$(error Unsupported architecture: $(EMBOX_ARCH))
endif

EMBOX_TARGET_CC =$(shell which $(EMBOX_CROSS_COMPILE)gcc)
EMBOX_TARGET_CXX=$(shell which $(EMBOX_CROSS_COMPILE)g++)

ifeq (,$(EMBOX_TARGET_CC))
$(error Cannot find C compiler: $(EMBOX_CROSS_COMPILE)gcc)
endif
ifeq (,$(EMBOX_TARGET_CXX))
$(error Cannot find C++ compiler: $(EMBOX_CROSS_COMPILE)g++)
endif



AT=

IPERF_DIR       = $(ROOT_DIR)/build/iperf
IPERF_CMAKE_DIR = $(IPERF_DIR)/cmake
IPERF_LIB       += $(IPERF_DIR)/src/libiperf.a
IPERF_FLAGS     = $(IPERF_CMAKE_DIR)/flags

EMBOX_IMPORTED_CPPFLAGS += -I$(abspath .)/include
#EMBOX_IMPORTED_CPPFLAGS += -save-temps=obj
#EMBOX_IMPORTED_CPPFLAGS += -include $(ROOT_DIR)/third-party/iperf/iperf_embox_compat.h
EMBOX_IMPORTED_CPPFLAGS += $(filter -I%,$(EMBOX_CPPFLAGS))
EMBOX_IMPORTED_CPPFLAGS += $(filter -nostdinc,$(EMBOX_CPPFLAGS))
EMBOX_IMPORTED_CPPFLAGS += $(filter -D__EMBOX__,$(EMBOX_CPPFLAGS))

EMBOX_IMPORTED_CFLAGS   += $(EMBOX_IMPORTED_CPPFLAGS)
EMBOX_IMPORTED_CXXFLAGS += $(EMBOX_IMPORTED_CPPFLAGS)

EMBOX_IMPORTED_CFLAGS   += $(filter -fno-common,$(EMBOX_CFLAGS))
EMBOX_IMPORTED_CFLAGS   += $(filter -march%,$(EMBOX_CFLAGS))
EMBOX_IMPORTED_CFLAGS   += $(filter -m32,$(EMBOX_CFLAGS))
EMBOX_IMPORTED_CFLAGS   += $(filter -fno-stack-protector,$(EMBOX_CFLAGS))
EMBOX_IMPORTED_CFLAGS   += $(filter -std=gnu99,$(EMBOX_CFLAGS))
EMBOX_IMPORTED_CFLAGS   += $(filter -g,$(EMBOX_CFLAGS))

EMBOX_IMPORTED_CXXFLAGS += $(subst .,$(ROOT_DIR),$(filter -I%,$(EMBOX_CXXFLAGS)))
EMBOX_IMPORTED_CXXFLAGS += $(filter -fno-common,$(EMBOX_CXXFLAGS))
EMBOX_IMPORTED_CXXFLAGS += $(filter -march%,$(EMBOX_CXXFLAGS))
EMBOX_IMPORTED_CXXFLAGS += $(filter -m32,$(EMBOX_CXXFLAGS))
EMBOX_IMPORTED_CXXFLAGS += $(filter -fno-stack-protector,$(EMBOX_CXXFLAGS))
EMBOX_IMPORTED_CXXFLAGS += $(filter -fno-rtti,$(EMBOX_CXXFLAGS))
EMBOX_IMPORTED_CXXFLAGS += $(filter -fno-exceptions,$(EMBOX_CXXFLAGS))
EMBOX_IMPORTED_CXXFLAGS += $(filter -fno-threadsafe-statics,$(EMBOX_CXXFLAGS))

#EMBOX_IMPORTED_CFLAGS   += -Wl,--relocatable
#EMBOX_IMPORTED_CXXFLAGS += -Wl,--relocatable

EMBOX_IMPORTED_MAKEFLAGS += $(filter -j,$(EMBOX_MAKEFLAGS))
ifneq ($(filter -j,$(EMBOX_MAKEFLAGS)),)
EMBOX_IMPORTED_MAKEFLAGS += $(shell nproc)
endif
# no wonder the following doesn't work
# EMBOX_IMPORTED_MAKEFLAGS += $(filter --jobserver-fds=%,$(EMBOX_MAKEFLAGS))

EMBOX_DERIVED_CFLAGS    += $(EMBOX_IMPORTED_CFLAGS)
EMBOX_DERIVED_CXXFLAGS  += $(EMBOX_IMPORTED_CXXFLAGS)
EMBOX_DERIVED_MAKEFLAGS += $(EMBOX_IMPORTED_MAKEFLAGS)
EMBOX_DERIVED_TARGET     = $(EMBOX_IPERF_TARGET)
EMBOX_DERIVED_CC         = $(EMBOX_TARGET_CC)
EMBOX_DERIVED_CXX        = $(EMBOX_TARGET_CXX)

EMBOX_DERIVED_LDFLAGS   += -Wl,--relocatable -nostdlib
EMBOX_DERIVED_LDFLAGS   += $(foreach i,$(filter -m elf_i386,$(EMBOX_LDFLAGS)),-Wl,$i)

all:

.PHONY: all $(IPERF_LIB)

$(IPERF_CMAKE_DIR):
	$(AT)mkdir -p $@

$(IPERF_FLAGS): | $(IPERF_CMAKE_DIR)
	-$(AT)rm $@
	$(AT)echo EMBOX_DERIVED_CFLAGS=\"$(EMBOX_DERIVED_CFLAGS)\" >> $@
	$(AT)echo EMBOX_DERIVED_CXXFLAGS=\"$(EMBOX_DERIVED_CXXFLAGS)\" >> $@
	$(AT)echo EMBOX_DERIVED_LDFLAGS=\"$(EMBOX_DERIVED_LDFLAGS)\" >> $@
	$(AT)echo EMBOX_DERIVED_MAKEFLAGS=\"$(EMBOX_DERIVED_MAKEFLAGS)\" >> $@
	$(AT)echo EMBOX_DERIVED_TARGET=\"$(EMBOX_DERIVED_TARGET)\" >> $@
	$(AT)echo EMBOX_DERIVED_CC=\"$(EMBOX_DERIVED_CC)\" >> $@
	$(AT)echo EMBOX_DERIVED_CXX=\"$(EMBOX_DERIVED_CC)\" >> $@
	$(AT)echo EMBOX_ROOT=\"$(ROOT_DIR)\" >> $@

$(IPERF_CMAKE_DIR)/Makefile: CMakeLists.txt $(IPERF_FLAGS) | $(IPERF_CMAKE_DIR)
	$(AT)cd $(IPERF_CMAKE_DIR) && $(shell cat $(IPERF_FLAGS)) cmake -DCMAKE_BUILD_TYPE=Debug $(shell pwd)

$(IPERF_LIB): $(IPERF_CMAKE_DIR)/Makefile
	$(AT)cd $(IPERF_CMAKE_DIR) && $(MAKE) VERBOSE=1 # -j `nproc`

all: $(IPERF_LIB)