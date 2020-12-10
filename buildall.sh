#!/bin/bash
# If you want to build strace2ds library, just run this script.
# If you want to build and install, run this script with 'install' argument. Ex: ./buildall install

function runcmd
{
    echo "CMD: $@"
    sleep 0.2
    $@
    ret=$?
    if test $ret -ne 0 ; then
	exit $ret
    fi
}

numberOfCores="$(nproc --all)"
runcmd /bin/rm -rf build
runcmd mkdir -p build
runcmd mkdir -p xml
runcmd cd tables
runcmd perl gen-xml-enums.pl
runcmd cd ../
runcmd cp -r ./xml build
runcmd cd build
runcmd cmake ../
runcmd make clean
runcmd make -j"${numberOfCores}"
# If all you're changing is C/C++ code, just "cd build" and run next two commands.
if [ "$#" -eq 1 ] && [ "$1" = "install" ]; then
    runcmd sudo make -j"${numberOfCores}" install
fi
