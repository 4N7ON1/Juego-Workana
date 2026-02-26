#include <windows.h>
#include <winbase.h>
#include <sys/stat.h>

class CMyDib
{
public:
	CMyDib(char *szFilename, unsigned long dwFilePointer);
	~CMyDib();
	void PaintImage(HDC hDC);
	BYTE* GetBits();
	WORD m_wWidthX;
	WORD m_wWidthY;
	WORD m_wColorNums;//bmp
	LPSTR m_lpDib;
	LPBITMAPINFO m_bmpInfo; //bmp
};