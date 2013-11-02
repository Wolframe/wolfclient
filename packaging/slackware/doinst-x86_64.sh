#!/bin/sh -e

# Author: Andreas Baumann <abaumann@yahoo.com>

( cd usr/lib64 ; rm -rf libskeleton.so.0.0 )
( cd usr/lib64 ; ln -sf libskeleton.so.0.0.1 libskeleton.so.0.0 )
( cd usr/lib64 ; rm -rf libqtwolfclient.so.0 )
( cd usr/lib64 ; ln -sf libqtwolfclient.so.0.0.1 libqtwolfclient.so.0 )
( cd usr/lib64 ; rm -rf libqtwolfclient.so.0.0 )
( cd usr/lib64 ; ln -sf libqtwolfclient.so.0.0.1 libqtwolfclient.so.0.0 )
( cd usr/lib64 ; rm -rf libskeleton.so )
( cd usr/lib64 ; ln -sf libskeleton.so.0.0.1 libskeleton.so )
( cd usr/lib64 ; rm -rf libskeleton.so.0 )
( cd usr/lib64 ; ln -sf libskeleton.so.0.0.1 libskeleton.so.0 )
( cd usr/lib64 ; rm -rf libqtwolfclient.so )
( cd usr/lib64 ; ln -sf libqtwolfclient.so.0.0.1 libqtwolfclient.so )
