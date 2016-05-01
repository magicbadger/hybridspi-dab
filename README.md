DAB SPI Encoder
===============

Simple encoder for Hybrid SPI SI.

Currently hardcoded to either DAB Datagroups or packets for a set ensemble (e1.c18c). Will be configurable in future. 

Pass in the SI.xml file to generate the encoded MOT directory bitstream


Building
--------

aclocal
libtoolize
automake --add-missing
configure
make
