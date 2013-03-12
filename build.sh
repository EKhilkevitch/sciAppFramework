#!/bin/sh

# =========================================

die() {
  echo ""
  echo "build failure: $*"
  echo ""
  exit 1
}

# =========================================

ROOT_DIR=`dirname $0`
BUILD_DIR="build"

# =========================================

cd "$ROOT_DIR" || die "Can not change dir to ${ROOT_DIR}"
mkdir -p "$BUILD_DIR" || die "Can not build dir ${BUILD_DIR}"
cd "$BUILD_DIR" || die "Can not change dir to ${BUILD_DIR}"
cmake .. $* || die "Error while running cmake command"
make || die "Error while running make"

echo ""
echo "Build successfully"
echo ""

exit 0

# =========================================

