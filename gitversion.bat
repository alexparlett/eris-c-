@ECHO OFF
cd %1
git describe --long --tags --dirty --always > gitversion.tmp
SET /p version= < gitversion.tmp
DEL gitversion.tmp
FOR /F "tokens=1-5 delims=.-" %%A IN ("%version%") DO (
	SET MAJOR=%%A&SET MINOR=%%B&SET REVISION=%%C&SET BUILD=%%D&SET DIRTY=%%E
)
SET /a MINOR+=1
IF DEFINED DIRTY GOTO DIRTYBUILD
:CLEANBUILD 
(ECHO #define FILEVER %MAJOR%,%MINOR%,%REVISION%,0)>Source/gitversion.h
(ECHO #define PRODUCTVER %MAJOR%,%MINOR%,%REVISION%,0)>>Source/gitversion.h
(ECHO #define FILEVERSTR %MAJOR%.%MINOR%.%REVISION%.%BUILD%)>>Source/gitversion.h
(ECHO #define PRODUCTVERSTR %MAJOR%.%MINOR%.%REVISION%.%BUILD%)>>Source/gitversion.h
(ECHO.)>>Source/gitversion.h
(ECHO const char* ERIS_VERSION = "%MAJOR%.%MINOR%.%REVISION%.%BUILD%";)>>Source/gitversion.h
GOTO CLEANUP
:DIRTYBUILD
(ECHO #define FILEVER %MAJOR%,%MINOR%,%REVISION%,0)>Source/gitversion.h
(ECHO #define PRODUCTVER %MAJOR%,%MINOR%,%REVISION%,0)>>Source/gitversion.h
(ECHO #define FILEVERSTR "%MAJOR%.%MINOR%.%REVISION%.%BUILD%M")>>Source/gitversion.h
(ECHO #define PRODUCTVERSTR "%MAJOR%.%MINOR%.%REVISION%.%BUILD%M")>>Source/gitversion.h
(ECHO.)>>Source/gitversion.h
(ECHO const char* ERIS_VERSION = "%MAJOR%.%MINOR%.%REVISION%.%BUILD%M";)>>Source/gitversion.h
GOTO CLEANUP
:CLEANUP
cd %2