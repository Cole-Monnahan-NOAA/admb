include objects.lst

!IF ([cl /? 2>&1 | findstr /C:"Compiler Version 19." > nul] == 0)
CXXVERSION=-cl19
!ENDIF
!IF ([cl /? 2>&1 | findstr /C:" x64" > nul] == 0)
OSNAME=-win64
!ELSEIF ([cl /? 2>&1 | findstr /C:" x86" > nul] == 0)
OSNAME=-win32
!ENDIF

!IF DEFINED(DEBUG)
!IF "$(CXXVERSION)"=="-cl19" && "$(OSNAME)"=="-win64"
OPT=$(OBJECTS: = ..\..\build\objects\win64-cl19-debug\contrib-optlp-gdbprintlib-)
SAF=$(OBJECTS: = ..\..\build\objects\win64-cl19-debug\contrib-saflp-gdbprintlib-)
!ELSEIF "$(CXXVERSION)"=="-cl19" && "$(OSNAME)"=="-win32"
OPT=$(OBJECTS: = ..\..\build\objects\win32-cl19-debug\contrib-optlp-gdbprintlib-)
SAF=$(OBJECTS: = ..\..\build\objects\win32-cl19-debug\contrib-saflp-gdbprintlib-)
!ELSE
OPT=$(OBJECTS: = ..\..\build\objects\win-cl-debug\contrib-optlp-gdbprintlib-)
SAF=$(OBJECTS: = ..\..\build\objects\win-cl-debug\contrib-saflp-gdbprintlib-)
!ENDIF
!ELSE
!IF "$(CXXVERSION)"=="-cl19" && "$(OSNAME)"=="-win64"
OPT=$(OBJECTS: = ..\..\build\objects\win64-cl19\contrib-optlp-gdbprintlib-)
SAF=$(OBJECTS: = ..\..\build\objects\win64-cl19\contrib-saflp-gdbprintlib-)
!ELSEIF "$(CXXVERSION)"=="-cl19" && "$(OSNAME)"=="-win32"
OPT=$(OBJECTS: = ..\..\build\objects\win32-cl19\contrib-optlp-gdbprintlib-)
SAF=$(OBJECTS: = ..\..\build\objects\win32-cl19\contrib-saflp-gdbprintlib-)
!ELSE
OPT=$(OBJECTS: = ..\..\build\objects\win-cl\contrib-optlp-gdbprintlib-)
SAF=$(OBJECTS: = ..\..\build\objects\win-cl\contrib-saflp-gdbprintlib-)
!ENDIF
!ENDIF

!IF DEFINED(SAFE_ONLY)
all: $(SAF)
!ELSE
all: $(OPT) $(SAF)
!ENDIf

$(OPT):: $(@B:contrib-optlp-gdbprintlib-=).cpp
!IF DEFINED(DEBUG)
	..\..\admb -c -g -f $?
!ELSE
	..\..\admb -c -f $?
!ENDIf

	copy $(?:.cpp=.obj) $@

$(SAF):: $(@B:contrib-saflp-gdbprintlib-=).cpp
!IF DEFINED(DEBUG)
	..\..\admb -c -g $?
!ELSE
	..\..\admb -c $?
!ENDIf
	copy $(?:.cpp=.obj) $@

includes: $(HEADERS)
	for %%a in ($(HEADERS)) do copy %%a $(CONTRIB_INCLUDE)

clean:
	del /Q $(OBJECTS)
