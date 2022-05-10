@echo on

REM param : "$PROJ_DIR$" $CONFIG_NAME$ 


C:


SET ASTYLE_EXE="AStyle.exe --style=allman --indent=spaces=4 --indent-switches --indent-preprocessor --indent-col1-comments --break-blocks --unpad-paren --pad-header --pad-oper --fill-empty-lines --add-brackets --convert-tabs --align-pointer=type --recursive --suffix=none --min-conditional-indent=0"

REM cd "D:\Professional\Projects\SPM_DLMS_V1.xx.xx_Wrapper -IPV6\FIRM\Sources\"

REM=============================================================================
"%ASTYLE_EXE%" *.c *.h

pause

