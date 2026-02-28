/* LegacyStdioFix.c
   Compatibilidad para libs compiladas con MSVC antiguo (jpeg.lib, CxImage.lib)
   que referencian __iob_func / _iob, removidos en VS2015+
*/
#include <stdio.h>

#if _MSC_VER >= 1900
FILE* __cdecl __iob_func(void)
{
    return NULL;
}
#endif
