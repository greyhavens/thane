PROGRAMS += shell

shell_BASENAME = avmthane
shell_INCLUDES = -I$(srcdir) -I$(topsrcdir)/extensions
shell_DEFINES = -DAVMPLUS_SHELL
shell_STATIC_LIBRARIES = MMgc avmplus
shell_DIR := $(curdir)/
shell_EXTRA_CPPFLAGS := $(AVMSHELL_CPPFLAGS)
shell_EXTRA_LDFLAGS := $(AVMSHELL_LDFLAGS)

ifdef ENABLE_THANE
shell_BUILD_ALL = 1
endif

shell_CXXSRCS := $(shell_CXXSRCS) \
  $(curdir)/avmthane.cpp \
  $(curdir)/ConsoleOutputStream.cpp \
  $(curdir)/DataIO.cpp \
  $(curdir)/DomainClass.cpp \
  $(curdir)/FileInputStream.cpp \
  $(curdir)/StringBuilderClass.cpp \
  $(curdir)/SystemClass.cpp \
  $(curdir)/as3src/flash/net/SocketGlue.cpp \
  $(curdir)/as3src/flash/utils/ByteArrayGlue.cpp \
  $(curdir)/as3src/flash/utils/DictionaryGlue.cpp \
  $(NULL)
