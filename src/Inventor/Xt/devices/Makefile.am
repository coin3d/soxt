
INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

noinst_LTLIBRARIES = libSoXtDevices.la

libSoXtDevices_la_SOURCES = \
	SoXtDevice.h \
	SoXtDevice.cpp \
	SoXtInputFocus.h \
	SoXtInputFocus.cpp \
	SoXtKeyboard.h \
	SoXtKeyboard.cpp \
	SoXtMouse.h \
	SoXtMouse.cpp \
	SoXtSpaceball.h \
	SoXtSpaceball.cpp

libSoXtDevicesincdir = $(includedir)/Inventor/Xt/devices

libSoXtDevicesinc_HEADERS = \
	SoXtDevice.h \
	SoXtInputFocus.h \
	SoXtKeyboard.h \
	SoXtMouse.h \
	SoXtSpaceball.h

############################################################################

install-libSoXtDevicesincHEADERS: $(libSoXtDevicesinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libSoXtDevicesincdir)
	@list='$(libSoXtDevicesinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoXtDevicesincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoXtDevicesincdir)/$$f; \
	done

uninstall-libSoXtDevicesincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libSoXtDevicesinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libSoXtDevicesincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libSoXtDevicesincdir)/$$f; \
	done

