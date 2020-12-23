#!/bin/bash

####################
# Script variables #
####################
install=false
customDataseriesDir=false
# TODO: Find a sensible default install directory
installDir="./build"
dataseriesDir=""

####################
# Script functions #
####################

# Wrapper function for running commands with feedback
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

# Output usage string (invoked on -h or --help)
function printUsage
{
    (
    cat << EOF
Usage: $0 [options...]
Options:
    --config-args ARGS     Append ARGS to every ./configure command
    --install              Install libraries and binaries under /usr/local
    --install-packages     Automatically use apt-get to install missing packages
    -h, --help             Print this help message
EOF
    ) >&2
    exit 0
}

##################
# Script startup #
##################

while [[ $# -gt 0 ]]; do
    key="$1"

    case "${key}" in
        --dataseries-dir)
            customDataseriesDir=true
            shift # past argument
            dataseriesDir="$1"
            shift # past value
            ;;
        --install)
            install=true
            shift # past argument
            ;;
        --install-dir)
            shift # past argument
            installDir="$1"
            shift # past argument
            ;;
        -h|--help)
            printUsage
            shift # past argument
            ;;
        *)
            shift # past argument
            ;;
    esac
done

numberOfCores="$(nproc --all)"
runcmd /bin/rm -rf build
runcmd mkdir -p build
runcmd mkdir -p xml
runcmd cd tables
runcmd perl gen-xml-enums.pl
runcmd cd ../
runcmd cp -r ./xml build
runcmd cd build

if [[ "${install}" == true ]]; then
    if [[ "${customDataseriesDir}" == true ]]; then
        runcmd cmake -DCMAKE_INSTALL_PREFIX:PATH="${installDir}"/strace2ds -DDATASERIES_DIR:PATH="${dataseriesDir}" ../
    else
        runcmd cmake -DCMAKE_INSTALL_PREFIX:PATH="${installDir}"/strace2ds ../
    fi
else
    if [[ "${customDataseriesDir}" == true ]]; then
        runcmd -DDATASERIES_DIR:PATH="${dataseriesDir}" ../
    else
        runcmd cmake ../
    fi
fi

runcmd make clean
runcmd make -j"${numberOfCores}"

if [[ "${install}" == true ]]; then
    runcmd sudo make -j"${numberOfCores}" install
else
    runcmd make -j"${numberOfCores}" install
fi
