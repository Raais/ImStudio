#!/bin/bash

# Author : Eric Bachard
# License MIT
# Created : friday 17 déc. 2021 21:27:36 CET

# Linux ONLY, but you can add your arch, improve, and so on

BUILD_DIR=build

# other case is LEGACY if nothing works for you
OPENGL_GL_PREFERENCE=GLVND

S_UNAME=`uname -s`

if test X${S_UNAME} = "XLinux"
    then
        if ! test -d ${BUILD_DIR}
           then
                mkdir ${BUILD_DIR}
        fi

        # push the current build directory
        pushd .

        cd ${BUILD_DIR}

        # build
        cmake -DOpenGL_GL_PREFERENCE=${OPENGL_GL_PREFERENCE} \
        ..
        make -j4

        # install. Uncomment me if you have admin rights
        # sudo make install
        # sudo ldconfig
        popd
fi

exit


