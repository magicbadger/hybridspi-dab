ACLOCAL_AMFLAGS = -I config 

SUBDIRS	= doc 

AM_CPPFLAGS = \
    -I$(top_builddir)/include \
    -I$(top_srcdir)/include

bin_PROGRAMS = src/dabspiencoder

src_dabspiencoder_SOURCES = src/encoder.cpp src/downloader.cpp
#src_dabspiencoder_CPPFLAGS = -Wp,-v -I${LIBMOT_CFLAGS} -I${LIBMSC_CFLAGS} -I${LIBHYBRIDSPI_CFLAGS} ${TINYXML2_CFLAGS} -I../src
src_dabspiencoder_CPPFLAGS = -I${LIBMOT_CFLAGS} -I${LIBMSC_CFLAGS} -I${LIBHYBRIDSPI_CFLAGS} ${TINYXML2_CFLAGS} -I../src
#src_dabspiencoder_LDFLAGS = -v
src_dabspiencoder_LDADD = -L${LIBMOT_LIBS} -lmot -L${LIBMSC_LIBS} -lmsc ${LIBCURL_LIBS} -lcurl ${TINYXML2_LIBS} -L${LIBHYBRIDSPI_LIBS} -lhybridspi -Lsrc
