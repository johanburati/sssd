#!/bin/bash
#
# DO NOT RUN THIS MANUALLY
#

sssd_source="/shared/sssd"
artifacts_dir="/shared/artifacts"

archive-artifacts() {
    echo "Archiving artifacts..."

    cp -f $sssd_source/ci-*.log $artifacts_dir
    cp -f $sssd_source/ci-build-debug/ci-*.log $artifacts_dir
    cp -f $sssd_source/ci-build-debug/test-suite.log $artifacts_dir
}

success-or-die() {
    ret=$1
    msg=$2
    if [ $ret -eq 0 ]; then
        return 0
    fi

    echo $msg
    archive-artifacts

    exit $ret
}

cd $sssd_source

echo "[1/1] Running Continuous Integration Tests"
./contrib/ci/run --moderate --no-deps
success-or-die $? "CI Failed!"

archive-artifacts
exit 0