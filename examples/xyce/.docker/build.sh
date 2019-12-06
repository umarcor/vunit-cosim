#!/usr/bin/env sh

set -e

export XYCE_OUTDIR=/usr/local/
export PATH="${PATH}:/tmp${XYCE_OUTDIR}bin"
export LIBRARY_PATH="${LIBRARY_PATH}:/tmp${XYCE_OUTDIR}lib"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/tmp${XYCE_OUTDIR}lib"
export CPATH="${CPATH}:/tmp${XYCE_OUTDIR}include"

mkdir -p /tmp/build/Xyce
cd /tmp/build/

# Get Xyce
XYCE_VERSION=6.12
curl -fsSL https://xyce.sandia.gov/downloads/_assets/documents/Xyce-6.12.tar.gz | tar xz -C Xyce --strip-components=1

# Get Trilinos
mkdir -p Trilinos/trilinos-source
curl -fsSL https://github.com/trilinos/Trilinos/archive/trilinos-release-12-12-1.tar.gz | tar xz -C Trilinos/trilinos-source --strip-components=1

mkdir -p /tmp/usr

# Build Trilinos
cd Trilinos
FLAGS="-O3 -fPIC"

cmake \
-G "Unix Makefiles" \
-DCMAKE_CXX_FLAGS="$FLAGS" \
-DCMAKE_C_FLAGS="$FLAGS" \
-DCMAKE_Fortran_FLAGS="$FLAGS" \
-DCMAKE_INSTALL_PREFIX="$XYCE_OUTDIR" \
-DCMAKE_MAKE_PROGRAM="make" \
-DBUILD_SHARED_LIBS=ON \
-DTrilinos_ENABLE_EXPLICIT_INSTANTIATION=ON \
-DTrilinos_ENABLE_NOX=ON \
 -DNOX_ENABLE_LOCA=ON \
-DTrilinos_ENABLE_EpetraExt=ON \
 -DEpetraExt_BUILD_BTF=ON \
 -DEpetraExt_BUILD_EXPERIMENTAL=ON \
 -DEpetraExt_BUILD_GRAPH_REORDERINGS=ON \
-DTrilinos_ENABLE_TrilinosCouplings=ON \
-DTrilinos_ENABLE_Ifpack=ON \
-DTrilinos_ENABLE_Isorropia=ON \
-DTrilinos_ENABLE_AztecOO=ON \
-DTrilinos_ENABLE_Belos=ON \
-DTrilinos_ENABLE_Triutils=ON \
-DTrilinos_ENABLE_Teuchos=ON \
-DTeuchos_ENABLE_COMPLEX=ON \
-DTrilinos_ENABLE_Amesos=ON \
 -DAmesos_ENABLE_KLU=ON \
 -DAmesos_ENABLE_UMFPACK=ON \
-DTrilinos_ENABLE_Sacado=ON \
-DTrilinos_ENABLE_Kokkos=OFF \
-DTrilinos_ENABLE_ALL_OPTIONAL_PACKAGES=OFF \
-DTPL_ENABLE_AMD=ON \
-DAMD_LIBRARY_DIRS="/usr/lib" \
-DTPL_AMD_INCLUDE_DIRS="/usr/include/suitesparse" \
-DTPL_ENABLE_UMFPACK=ON \
-DUMFPACK_LIBRARY_DIRS="/usr/lib" \
-DTPL_UMFPACK_INCLUDE_DIRS="/usr/include/suitesparse" \
-DTPL_ENABLE_BLAS=ON \
-DTPL_ENABLE_LAPACK=ON \
./trilinos-source

#-DTrilinos_ENABLE_Fortran=OFF \
#-DTrilinos_CXX11_FLAGS="c++11" \
#-DCMAKE_Fortran_FLAGS="$FLAGS" \

#-DTrilinos_ENABLE_ShyLU=ON \
#-DTrilinos_ENABLE_Zoltan=ON \


#cmake \
#-G "Unix Makefiles" \
#-DCMAKE_CXX_FLAGS="$FLAGS" \
#-DCMAKE_C_FLAGS="$FLAGS" \
#-DCMAKE_Fortran_FLAGS="$FLAGS" \
#-DCMAKE_INSTALL_PREFIX="$XYCE_OUTDIR" \
#-DCMAKE_MAKE_PROGRAM="make" \
#-DTrilinos_ENABLE_NOX=ON \
#-DNOX_ENABLE_LOCA=ON \
#-DTrilinos_ENABLE_EpetraExt=ON \
#-DEpetraExt_BUILD_BTF=ON \
#-DEpetraExt_BUILD_EXPERIMENTAL=ON \
#-DEpetraExt_BUILD_GRAPH_REORDERINGS=ON \
#-DTrilinos_ENABLE_TrilinosCouplings=ON \
#-DTrilinos_ENABLE_Ifpack=ON \
#-DTrilinos_ENABLE_Isorropia=ON \
#-DTrilinos_ENABLE_AztecOO=ON \
#-DTrilinos_ENABLE_Belos=ON \
#-DTeuchos_ENABLE_COMPLEX=ON \
#-DTrilinos_ENABLE_Amesos=ON \
#-DAmesos_ENABLE_KLU=ON \
#-DAmesos_ENABLE_UMFPACK=ON \
#-DTrilinos_ENABLE_Sacado=ON \
#-DTrilinos_ENABLE_Kokkos=OFF \
#-DTrilinos_ENABLE_Zoltan=ON \
#-DTrilinos_ENABLE_ALL_OPTIONAL_PACKAGES=OFF \
#-DTPL_ENABLE_AMD=ON \
#-DAMD_LIBRARY_DIRS="/usr/lib" \
#-DTPL_AMD_INCLUDE_DIRS="/usr/include/suitesparse" \
#-DTPL_ENABLE_UMFPACK=ON \
#-DUMFPACK_LIBRARY_DIRS="/usr/lib" \
#-DTPL_UMFPACK_INCLUDE_DIRS="/usr/include/suitesparse" \
#-DTPL_ENABLE_BLAS=ON \
#-DTPL_ENABLE_LAPACK=ON \
#./trilinos-source

#-DTrilinos_ENABLE_ShyLU=ON \
#-DTrilinos_ENABLE_Teuchos=ON \

#make
make DESTDIR=/tmp -j$(nproc) install

cd ..

# Build Xyce
mkdir xyce-build
cd xyce-build

../Xyce/configure \
--disable-reaction_parser \
--disable-verbose_linear \
--disable-verbose_nonlinear \
--disable-verbose_time \
--enable-shared \
--enable-xyce-shareable \
ADMS_CXXFLAGS="-O1" \
CXXFLAGS="-O3 -std=c++11" \
LDFLAGS="-Wl,-rpath=./utils/XyceCInterface -Wl,-rpath=./lib" \
CPPFLAGS="-I/tmp/usr/local/include/suitesparse"

#make
make DESTDIR=/tmp -j$(nproc) install

git clone https://github.com/ghdl/ghdl/
cd ghdl
./dist/ci-run.sh -c -b llvm -p xyce_ghdl build
tar xzvf xyce_ghdl.tgz -C /tmp
