#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=USB/usb_device.c "USB/CDC Device Driver/usb_function_cdc.c" usb_descriptors.c main.c D32.c LCDlib.c DataCapture.c ID.c Utilities.c Camera.c ExtIO.c Ports.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/USB/usb_device.o "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o" ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/main.o ${OBJECTDIR}/D32.o ${OBJECTDIR}/LCDlib.o ${OBJECTDIR}/DataCapture.o ${OBJECTDIR}/ID.o ${OBJECTDIR}/Utilities.o ${OBJECTDIR}/Camera.o ${OBJECTDIR}/ExtIO.o ${OBJECTDIR}/Ports.o
POSSIBLE_DEPFILES=${OBJECTDIR}/USB/usb_device.o.d "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o.d" ${OBJECTDIR}/usb_descriptors.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/D32.o.d ${OBJECTDIR}/LCDlib.o.d ${OBJECTDIR}/DataCapture.o.d ${OBJECTDIR}/ID.o.d ${OBJECTDIR}/Utilities.o.d ${OBJECTDIR}/Camera.o.d ${OBJECTDIR}/ExtIO.o.d ${OBJECTDIR}/Ports.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/USB/usb_device.o ${OBJECTDIR}/USB/CDC\ Device\ Driver/usb_function_cdc.o ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/main.o ${OBJECTDIR}/D32.o ${OBJECTDIR}/LCDlib.o ${OBJECTDIR}/DataCapture.o ${OBJECTDIR}/ID.o ${OBJECTDIR}/Utilities.o ${OBJECTDIR}/Camera.o ${OBJECTDIR}/ExtIO.o ${OBJECTDIR}/Ports.o

# Source Files
SOURCEFILES=USB/usb_device.c USB/CDC Device Driver/usb_function_cdc.c usb_descriptors.c main.c D32.c LCDlib.c DataCapture.c ID.c Utilities.c Camera.c ExtIO.c Ports.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX460F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/USB/usb_device.o: USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/USB/usb_device.o.d" -o ${OBJECTDIR}/USB/usb_device.o USB/usb_device.c  
	
${OBJECTDIR}/USB/CDC\ Device\ Driver/usb_function_cdc.o: USB/CDC\ Device\ Driver/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB/CDC Device Driver" 
	@${RM} "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o".d 
	@${RM} "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o" 
	@${FIXDEPS} "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o.d" -o "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o" "USB/CDC Device Driver/usb_function_cdc.c"  
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  
	
${OBJECTDIR}/D32.o: D32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/D32.o.d 
	@${RM} ${OBJECTDIR}/D32.o 
	@${FIXDEPS} "${OBJECTDIR}/D32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/D32.o.d" -o ${OBJECTDIR}/D32.o D32.c  
	
${OBJECTDIR}/LCDlib.o: LCDlib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LCDlib.o.d 
	@${RM} ${OBJECTDIR}/LCDlib.o 
	@${FIXDEPS} "${OBJECTDIR}/LCDlib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/LCDlib.o.d" -o ${OBJECTDIR}/LCDlib.o LCDlib.c  
	
${OBJECTDIR}/DataCapture.o: DataCapture.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DataCapture.o.d 
	@${RM} ${OBJECTDIR}/DataCapture.o 
	@${FIXDEPS} "${OBJECTDIR}/DataCapture.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/DataCapture.o.d" -o ${OBJECTDIR}/DataCapture.o DataCapture.c  
	
${OBJECTDIR}/ID.o: ID.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ID.o.d 
	@${RM} ${OBJECTDIR}/ID.o 
	@${FIXDEPS} "${OBJECTDIR}/ID.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/ID.o.d" -o ${OBJECTDIR}/ID.o ID.c  
	
${OBJECTDIR}/Utilities.o: Utilities.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Utilities.o.d 
	@${RM} ${OBJECTDIR}/Utilities.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/Utilities.o.d" -o ${OBJECTDIR}/Utilities.o Utilities.c  
	
${OBJECTDIR}/Camera.o: Camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Camera.o.d 
	@${RM} ${OBJECTDIR}/Camera.o 
	@${FIXDEPS} "${OBJECTDIR}/Camera.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/Camera.o.d" -o ${OBJECTDIR}/Camera.o Camera.c  
	
${OBJECTDIR}/ExtIO.o: ExtIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ExtIO.o.d 
	@${RM} ${OBJECTDIR}/ExtIO.o 
	@${FIXDEPS} "${OBJECTDIR}/ExtIO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/ExtIO.o.d" -o ${OBJECTDIR}/ExtIO.o ExtIO.c  
	
${OBJECTDIR}/Ports.o: Ports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Ports.o.d 
	@${RM} ${OBJECTDIR}/Ports.o 
	@${FIXDEPS} "${OBJECTDIR}/Ports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/Ports.o.d" -o ${OBJECTDIR}/Ports.o Ports.c  
	
else
${OBJECTDIR}/USB/usb_device.o: USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB" 
	@${RM} ${OBJECTDIR}/USB/usb_device.o.d 
	@${RM} ${OBJECTDIR}/USB/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/USB/usb_device.o.d" -o ${OBJECTDIR}/USB/usb_device.o USB/usb_device.c  
	
${OBJECTDIR}/USB/CDC\ Device\ Driver/usb_function_cdc.o: USB/CDC\ Device\ Driver/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB/CDC Device Driver" 
	@${RM} "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o".d 
	@${RM} "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o" 
	@${FIXDEPS} "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o.d" -o "${OBJECTDIR}/USB/CDC Device Driver/usb_function_cdc.o" "USB/CDC Device Driver/usb_function_cdc.c"  
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  
	
${OBJECTDIR}/D32.o: D32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/D32.o.d 
	@${RM} ${OBJECTDIR}/D32.o 
	@${FIXDEPS} "${OBJECTDIR}/D32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/D32.o.d" -o ${OBJECTDIR}/D32.o D32.c  
	
${OBJECTDIR}/LCDlib.o: LCDlib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LCDlib.o.d 
	@${RM} ${OBJECTDIR}/LCDlib.o 
	@${FIXDEPS} "${OBJECTDIR}/LCDlib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/LCDlib.o.d" -o ${OBJECTDIR}/LCDlib.o LCDlib.c  
	
${OBJECTDIR}/DataCapture.o: DataCapture.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DataCapture.o.d 
	@${RM} ${OBJECTDIR}/DataCapture.o 
	@${FIXDEPS} "${OBJECTDIR}/DataCapture.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/DataCapture.o.d" -o ${OBJECTDIR}/DataCapture.o DataCapture.c  
	
${OBJECTDIR}/ID.o: ID.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ID.o.d 
	@${RM} ${OBJECTDIR}/ID.o 
	@${FIXDEPS} "${OBJECTDIR}/ID.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/ID.o.d" -o ${OBJECTDIR}/ID.o ID.c  
	
${OBJECTDIR}/Utilities.o: Utilities.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Utilities.o.d 
	@${RM} ${OBJECTDIR}/Utilities.o 
	@${FIXDEPS} "${OBJECTDIR}/Utilities.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/Utilities.o.d" -o ${OBJECTDIR}/Utilities.o Utilities.c  
	
${OBJECTDIR}/Camera.o: Camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Camera.o.d 
	@${RM} ${OBJECTDIR}/Camera.o 
	@${FIXDEPS} "${OBJECTDIR}/Camera.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/Camera.o.d" -o ${OBJECTDIR}/Camera.o Camera.c  
	
${OBJECTDIR}/ExtIO.o: ExtIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ExtIO.o.d 
	@${RM} ${OBJECTDIR}/ExtIO.o 
	@${FIXDEPS} "${OBJECTDIR}/ExtIO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/ExtIO.o.d" -o ${OBJECTDIR}/ExtIO.o ExtIO.c  
	
${OBJECTDIR}/Ports.o: Ports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Ports.o.d 
	@${RM} ${OBJECTDIR}/Ports.o 
	@${FIXDEPS} "${OBJECTDIR}/Ports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/Pulse Sampling Thermography/Software/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items" -I"../../../../../../Documents and Settings/Aaron/Desktop/QFI Material/PST Items/PST Firmware/Include" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include/USB" -I"../../../../../../Documents and Settings/Aaron/Desktop/PST Items/PST Firmware/Include" -I"." -I"../microchip/include" -I"." -I"Include" -I"Include/USB" -MMD -MF "${OBJECTDIR}/Ports.o.d" -o ${OBJECTDIR}/Ports.o Ports.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-L"../../../../../../Program Files/Microchip/MPLAB C30/lib",-L".",-Map="${DISTDIR}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.map" 
else
dist/${CND_CONF}/${IMAGE_TYPE}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-L"../../../../../../Program Files/Microchip/MPLAB C30/lib",-L".",-Map="${DISTDIR}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/PST_Firmware_MPLab_X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
