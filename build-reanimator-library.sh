#!/bin/bash

####################
# Script variables #
####################

numberOfCores="$(nproc --all)"
install=false
configArgs=""
rootDir="$(dirname $(realpath $0))"
buildDir="${rootDir}/build"
repositoryDir="${buildDir}/repositories"
installDir="${buildDir}"

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
    --install-dir DIR      Install libraries and binaries to DIR (ignored if --install)
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
        --config-args)
            shift # past argument
            configArgs="$1"
            shift # past value
            ;;
        --install)
            install=true
            shift # past argument
            ;;
        --install-dir)
            shift # past argument
            installDir="$(realpath "$1" || exit $?)"
            shift # past value
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

# --install-dir is ignored if --install is specified
if $install; then
    installDir="/usr/local"
fi

# Set up the build directory
runcmd /bin/rm -rf "${buildDir}"
runcmd mkdir -p "${buildDir}"
runcmd mkdir -p xml
runcmd cd tables
runcmd perl gen-xml-enums.pl
runcmd cd ../
runcmd cp -r ./xml "${buildDir}"
runcmd rm -f include/strace2ds-enums.h
runcmd cp strace2ds-enums.h include
runcmd cd "${buildDir}"

# Cloning all repositories
runcmd mkdir -p "${repositoryDir}"
runcmd cd "${repositoryDir}"
[[ -d "Lintel" ]] || runcmd git clone https://github.com/dataseries/Lintel.git
[[ -d "DataSeries" ]] || runcmd git clone https://github.com/dataseries/DataSeries.git
[[ -d "gperftools" ]] || runcmd git clone https://github.com/gperftools/gperftools.git

# Building Lintel
# ---------------
runcmd cd Lintel
runcmd cmake -DCMAKE_INSTALL_PREFIX="${installDir}" .
if $install; then
    runcmd sudo make -j"${numberOfCores}" install
else
    runcmd make -j"${numberOfCores}" install
fi
runcmd cd "${repositoryDir}"

# Building DataSeries
# -------------------
runcmd cd DataSeries
runcmd cmake -DCMAKE_CXX_STANDARD=11 -DCMAKE_INSTALL_PREFIX="${installDir}" .
if $install; then
    runcmd sudo make -j"${numberOfCores}" install
else
    runcmd make -j"${numberOfCores}" install
fi
runcmd cd "${repositoryDir}"

# Building tcmalloc
# -----------------
runcmd cd gperftools
runcmd ./autogen.sh
runcmd ./configure --prefix="${installDir}" "${configArgs}"
runcmd make -j"${numberOfCores}"
if $install; then
    runcmd sudo make -j"${numberOfCores}" install
else
    runcmd make -j"${numberOfCores}" install
fi
runcmd cd "${repositoryDir}"

# Building strace2ds
# ------------------
runcmd cd "${buildDir}"
runcmd cmake -DCMAKE_INSTALL_PREFIX:PATH="${installDir}" ../
runcmd make clean
runcmd make -j"${numberOfCores}"
if $install; then
    runcmd sudo make -j"${numberOfCores}" install
else
    runcmd make -j"${numberOfCores}" install
fi
