#include	<windows.h>



#ifdef	FRENCH_EXPORTS
char	*GameLadLanguage = "Fran�ais";
#endif
#ifdef	SPANISH_EXPORTS
char	*GameLadLanguage = "Espa�ol";
#endif
#ifdef	SWEDISH_EXPORTS
char	*GameLadLanguage = "Svenska";
#endif



BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
	return true;
}

