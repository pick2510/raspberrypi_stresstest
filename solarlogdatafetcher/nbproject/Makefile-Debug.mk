#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/Inverter.o \
	${OBJECTDIR}/src/Modulestring.o \
	${OBJECTDIR}/src/Plant.o \
	${OBJECTDIR}/src/libstrebel.o \
	${OBJECTDIR}/src/md5.o \
	${OBJECTDIR}/src/solarlogdatafetcher.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=/usr/local/Cellar/cpp-netlib/0.12.0_1/lib/libcppnetlib-client-connections.a /usr/local/Cellar/cpp-netlib/0.12.0_1/lib/libcppnetlib-server-parsers.a /usr/local/Cellar/cpp-netlib/0.12.0_1/lib/libcppnetlib-uri.a /usr/local/Cellar/boost/1.60.0_2/lib/libboost_system-mt.a /usr/local/Cellar/openssl/1.0.2h_1/lib/libssl.a /usr/local/Cellar/openssl/1.0.2h_1/lib/libcrypto.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher: /usr/local/Cellar/cpp-netlib/0.12.0_1/lib/libcppnetlib-client-connections.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher: /usr/local/Cellar/cpp-netlib/0.12.0_1/lib/libcppnetlib-server-parsers.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher: /usr/local/Cellar/cpp-netlib/0.12.0_1/lib/libcppnetlib-uri.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher: /usr/local/Cellar/boost/1.60.0_2/lib/libboost_system-mt.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher: /usr/local/Cellar/openssl/1.0.2h_1/lib/libssl.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher: /usr/local/Cellar/openssl/1.0.2h_1/lib/libcrypto.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher ${OBJECTFILES} ${LDLIBSOPTIONS} -lpthread

${OBJECTDIR}/src/Inverter.o: src/Inverter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Inverter.o src/Inverter.cpp

${OBJECTDIR}/src/Modulestring.o: src/Modulestring.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Modulestring.o src/Modulestring.cpp

${OBJECTDIR}/src/Plant.o: src/Plant.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Plant.o src/Plant.cpp

${OBJECTDIR}/src/libstrebel.o: src/libstrebel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/libstrebel.o src/libstrebel.cpp

${OBJECTDIR}/src/md5.o: src/md5.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/md5.o src/md5.cpp

${OBJECTDIR}/src/solarlogdatafetcher.o: src/solarlogdatafetcher.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/solarlogdatafetcher.o src/solarlogdatafetcher.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/solarlogdatafetcher

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
