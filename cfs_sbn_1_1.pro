TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES += cfs_sbn_1_1.asn
DISTFILES += cfs_sbn_1_1.acn
DISTFILES += cfs_sbn_1_1.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters
#include(handleAsn1AcnBuild.pri)
include(work/taste.pro)
message($$DISTFILES)

