// Primero aseguramos que no haya warnings de WinSock al usar funciones deprecated
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Incluir Winsock y sus dependencias primero
#include <winsock2.h>        // **Primero**
#include <ws2tcpip.h>        // Necesario para el uso de sockets TCP/IP

// Incluir el header de Windows y de IP helpers después
#include <windows.h>         // Incluir siempre después de Winsock
#include <iphlpapi.h>        // Para funciones relacionadas con las interfaces de red

// Luego los otros headers de tu proyecto
#include "../Headers/Game.h"           // Tu header personalizado
#include "../Headers/Winmain.h"        // Otro header personalizado
#include "../Language/lan_eng.h"       // Idioma y localización

// Agregar header de resolución
#include "..\Resolution\Resolution.h"

// Los demás headers como mmsystem, etc., los puedes poner después
#include <mmsystem.h>           // Para funciones relacionadas con el sonido
#include <stdio.h>              // Para printf, sprintf, etc.
#include <stdlib.h>             // Funciones estándar como malloc, free, etc.
#include <algorithm>            // Para sort, swap, etc.
#include <thread>               // Para usar hilos en C++
#include <string>               // Cadenas de texto en C++
#include <cctype>               // Para funciones de manipulación de caracteres
#include <map>                  // Para trabajar con mapas (hashmaps)
#include <iomanip>              // Para manipular flujos de salida (formato de número)
#include <iostream>             // Para std::cout, std::cin, etc.
#include <sstream>              // Para convertir tipos a cadenas de texto
#include <ctime>                // Para funciones relacionadas con tiempo

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "winmm.lib") // Necesario para usar funciones de la biblioteca multimedia

extern class cResolution * c_reso;
extern char G_cSpriteAlphaDegree;

extern char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];
extern class XSocket * G_pCalcSocket;
extern BOOL G_bIsCalcSocketConnected;
extern DWORD G_dwCalcSocketTime, G_dwCalcSocketSendTime;
extern HWND	G_hWnd, G_hEditWnd;
extern HINSTANCE G_hInstance;

extern bool _tmp_bSpriteOmit;

char _cDrawingOrder2[] = { 0, 1, 0, 0, 0, 0, 0, 1, 1 };
char _cMantleDrawingOrder2[] = { 0, 1, 1, 1, 0, 0, 0, 2, 2 };
char _cMantleDrawingOrderOnRun2[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1 };

static char __cSpace[] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 7, 8, 8, 9, 10, 9, 7, 8, 8, 8, 8, 8, 8, 8,
15, 16, 12, 17, 14, 15, 14, 16, 10, 13, 19, 10, 17, 17, 15, 14, 15, 16, 13, 17, 16, 16, 20, 17, 16, 14,
8, 8, 8, 8, 8, 8, 8, 6, 7, 8, 7, 7, 7, 7, 4, 7, 7, 4, 11, 7, 8, 8, 7, 8, 6, 5, 8, 9, 14, 8, 9, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8 };

void CGame::DrawShadowMenu(short x, short y, short x2, short y2)
{
	m_DDraw.DrawShadowBox(x, y, x2, y2);
	m_DDraw.DrawShadowBox(x, y, x2, y2);
}

void CGame::DrawDialogShadowPixels(int mode, int count, short x, short y)
{
	switch (mode)
	{
	case 1:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x + (d), y, 73, 58, 42);
		}
		break;
	}

	case 2:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x, y + (d), /*152, 123, 54);*/73, 58, 42);
		}
		break;
	}
	}
}

void CGame::DrawShadowMenu_DialogBorders(short sX, short sY, short szX, short szY)
{
	DrawDialogShadowPixels(1, szX, sX, sY);
	DrawDialogShadowPixels(1, szX, sX, sY + szY);
	DrawDialogShadowPixels(1, szX, sX, sY + 23);
	DrawDialogShadowPixels(2, szY, sX, sY);
	DrawDialogShadowPixels(2, szY + 1, sX + szX, sY);
}

void CGame::DrawShadowMenu_DialogBordersNoTitle(short sX, short sY, short szX, short szY)
{
	DrawDialogShadowPixels(1, szX, sX, sY);
	DrawDialogShadowPixels(1, szX, sX, sY + szY);
	//DrawDialogShadowPixels(1, szX, sX, sY + 23);
	DrawDialogShadowPixels(2, szY, sX, sY);
	DrawDialogShadowPixels(2, szY + 1, sX + szX, sY);
}

void CGame::DrawNewDialogBox2(int cType, int sX, int sY, int iFrame, BOOL bIsNoColorKey, BOOL bIsTrans)
{
	DWORD dwTime = G_dwGlobalTime;

	if (m_pSprite[cType] == NULL) return;
	if (bIsNoColorKey == FALSE)
	{
		if (bIsTrans == TRUE)
			m_pSprite[cType]->PutTransSprite2(sX, sY, iFrame, dwTime);
		else m_pSprite[cType]->PutSpriteFast(sX, sY, iFrame, dwTime);
	}
	else m_pSprite[cType]->PutSpriteFastNoColorKey(sX, sY, iFrame, dwTime);
}

void CGame::DrawNewDialogBox3(int cType, int sX, int sY, int iFrame, BOOL bIsNoColorKey, BOOL bIsTrans)
{
	DWORD dwTime = G_dwGlobalTime;

	if (m_pSprite[cType] == NULL) return;
	if (bIsNoColorKey == FALSE)
	{
		if (bIsTrans == TRUE)
			m_pSprite[cType]->PutTransSprite2(sX, sY, iFrame, dwTime);
		else m_pSprite[cType]->PutSpriteRGB(sX, sY, iFrame, m_wWR[4] - m_wR[0], m_wWG[4] - m_wG[0], m_wWB[4] - m_wB[0],dwTime);
	}
	else m_pSprite[cType]->PutSpriteFastNoColorKey(sX, sY, iFrame, dwTime);
}


void CGame::PutCenterString_SpriteFont(int iX1, int iX2, int iY, const char * pStr, short sR, short sG, short sB)
{
	int iXpos, iR, iG, iB;
	DWORD iCnt;
	DWORD dwTime = G_dwGlobalTime;
	char  cTmpStr[200];

	m_DDraw.ColorTransferRGB(RGB(sR, sG, sB), &iR, &iG, &iB);

	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);

	int textWidth = 0;
	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
		if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122)) {
			textWidth += __cSpace[cTmpStr[iCnt] - 33];
		}
		else {
			textWidth += 5;
		}
	}

	int areaWidth = iX2 - iX1;
	int initialOffset = (areaWidth - textWidth) / 2;

	iXpos = iX1 + initialOffset;

	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
		if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122)) {
			if ((sR == 0) && (sG == 0) && (sB == 0)) {
				m_pSprite[DEF_SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
			}
			else {
				m_pSprite[DEF_SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 33, iR, iG, iB, dwTime);
			}
			iXpos += __cSpace[cTmpStr[iCnt] - 33];
		}
		else {
			iXpos += 5;
		}
	}
}

void CGame::DrawEnhancement_Tab(short sX, short sY, short msX, short msY, char * text, bool active)
{
	int isize = 128;

	//if (!active) DrawNewDialogBox2(SPRID_ENHANCEMENT_TAB, sX, sY, 0, FALSE, FALSE);

	auto mouse_over = (msX >= sX) && (msX <= sX + isize) && (msY >= sY) && (msY <= sY + 23);

	if (active)
		PutCenterString_SpriteFont(sX, sX + isize, sY + 2, text, 37, 199, 47);
	else if (mouse_over)
		PutCenterString_SpriteFont(sX, sX + isize, sY + 2, text, 255, 200, 0);
	else
		PutCenterString_SpriteFont(sX, sX + isize, sY + 2, text, 255, 0, 0);
}

void CGame::DrawMenu_EnhBox(short sX, short sY, short width, short height)
{
	DrawDialogShadowPixels(1, width, sX, sY);
	DrawDialogShadowPixels(1, width, sX, sY + height);
	DrawDialogShadowPixels(2, height, sX, sY);
	DrawDialogShadowPixels(2, height + 1, sX + width, sY);

	for (short y = 1; y < height; ++y) {
		for (short x = 1; x < width; ++x) {
			m_DDraw.PutTransparentPixel(sX + x, sY + y, 128, 128, 128);
		}
	}
}

void CGame::DrawDialogShadowPixelsEnc(int mode, int count, short x, short y)
{
	switch (mode)
	{
	case 1:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x + (d), y, 73, 58, 42);
		}
		break;
	}

	case 2:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x, y + (d), 73, 58, 42);
		}
		break;
	}
	}
}

void CGame::DrawShadowMenu_ExtMenuBorders(short sX, short sY, short szX, short szY)
{
	DrawDialogShadowPixelsEnc(1, szX, sX, sY);
	DrawDialogShadowPixelsEnc(1, szX, sX, sY + szY);
	DrawDialogShadowPixelsEnc(2, szY, sX, sY);
	DrawDialogShadowPixelsEnc(2, szY + 1, sX + szX, sY);
}

void CGame::DrawComboBoxEnc(short x, short y, short width, short height, const std::vector<std::string>& options, int selectedIndex, bool isOpen, short msX, short msY, bool isEnabled) {
	// Dibujar el menú principal con bordes reducidos
	DrawShadowMenu(x - 1, y - 1, x + width + 1, y + height + 1);

	// Determinar la opción seleccionada o mensaje a mostrar
	std::string selectedOption;
	if (!isEnabled) {
		selectedOption = m_sLang == LANG_ESP ? "Por favor, introduce un ítem" : "Please enter an item";
	}
	else if (options.empty()) {
		selectedOption = m_sLang == LANG_ESP ? "Nada para seleccionar" : "Nothing to select";
	}
	else {
		selectedOption = (selectedIndex == -1) ?
			(m_sLang == LANG_ESP ? "Selecciona una estadística" : "Select a statistic") :
			options[selectedIndex];
	}

	// Dibujar la opción seleccionada
	PutString2(x + 5, y + 3, (char*)selectedOption.c_str(), 255, 255, 255);

	if (isEnabled && !options.empty()) {
		// Dibujar la flecha del ComboBox
		if (isOpen) {
			DrawNewDialogBox2(SPRID_ARROWS + 1, x + width - 15, y + 5, 0);
		}
		else {
			DrawNewDialogBox2(SPRID_ARROWS, x + width - 15, y + 5, 0);
		}

		// Dibujar el menú extendido si está abierto
		if (isOpen) {
			// Calcular el alto del menú desplegable para abarcar todas las opciones
			short extMenuHeight = 20 * (options.size() - (selectedIndex == -1 ? 0 : 1));

			// Dibujar el shadow menu para todo el menú extendido
			DrawShadowMenu(x - 1, y + height - 1, x + width + 1, y + height + extMenuHeight + 1);

			// Dibujar cada opción en el menú desplegable
			int optionIndex = 0;
			for (size_t i = 0; i < options.size(); ++i) {
				if (i == selectedIndex) continue;

				// Calcular la posición Y de cada opción
				short optionY = y + height + (optionIndex * 20);

				// Cambiar color si el mouse está sobre la opción
				if (msX >= x && msX <= x + width && msY >= optionY && msY <= optionY + 20) {
					PutString2(x + 5, optionY, (char*)options[i].c_str(), 255, 165, 0); // Hover
				}
				else {
					PutString2(x + 5, optionY, (char*)options[i].c_str(), 200, 200, 200); // Normal
				}
				optionIndex++;
			}
		}
	}
}


void CGame::HandleComboBoxClick(short msX, short msY, short comboX, short comboY, short comboWidth, short comboHeight, const std::vector<std::string>& options, char cLB, int dnumber, bool isEnabled) {
	if (!isEnabled || options.empty()) return;

	if (cLB != 0 && iGetTopDialogBoxIndex() == dnumber && !isClickProcessed) {
		if (msX >= comboX && msX <= comboX + comboWidth && msY >= comboY && msY <= comboY + comboHeight) {
			isComboBoxOpen = !isComboBoxOpen;
			PlaySound('E', 14, 5);
			isClickProcessed = true;
		}
		else if (isComboBoxOpen) {
			int optionIndex = 0;
			for (size_t i = 0; i < options.size(); ++i) {
				if (i == selectedOptionIndex) continue;
				short optionY = comboY + comboHeight + (optionIndex * 20);
				if (msX >= comboX && msX <= comboX + comboWidth && msY >= optionY && msY <= optionY + 20) {
					selectedOptionIndex = i;
					isComboBoxOpen = false;
					PlaySound('E', 14, 5);
					isClickProcessed = true;
					selectedType = (selectedOptionIndex == 0) ? 1 : 2;
					break;
				}
				optionIndex++;
			}
		}
	}
	else if (cLB == 0) {
		isClickProcessed = false;
	}
}

void CGame::DrawMenu_SmallButton(short sX, short sY, short msX, short msY, short x, short y, char * text)
{
	if ((msX >= sX + x) && (msX <= sX + x + 76) && (msY >= sY + y) && (msY <= sY + y + 22)){
		DrawNewDialogBox2(SPRID_SMALLBUTTONS + 1, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 76, sY + y + 2, text, 255, 240, 0);
	}
	else
	{
		DrawNewDialogBox2(SPRID_SMALLBUTTONS, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 76, sY + y + 2, text, 255, 240, 255);
	}
}


void CGame::DrawNewPanelButton(short sX, short sY, short msX, short msY, short x, short y, char * text, bool istrue)
{
	if ((msX >= sX + x) && (msX <= sX + x + 86) && (msY >= sY + y) && (msY <= sY + y + 24) || istrue){
		DrawNewDialogBox2(SPRID_NEWPANELBUTS + 1, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 86, sY + y + 3, text, 255,255,0);
	}
	else
	{
		DrawNewDialogBox2(SPRID_NEWPANELBUTS, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 86, sY + y + 3, text, 255, 200, 0);
	}
}

void CGame::draw_enchanting(short msX, short msY, short msZ, char cLB) {
	short sX, sY, sizeX, sizeY;
	int dnumber = 72;
	int iValue, i;
	char cItemColor, cStr1[120], cStr2[120], cStr3[120], cStr4[120], cStr5[120], cStr6[120];
	DWORD dwTime = timeGetTime();
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	std::string selectedOptionText;
/*	DrawShadowMenu(sX, sY, sX + sizeX, sY + sizeY);
	DrawShadowMenu_DialogBorders(sX, sY, sizeX, sizeY);*/

	DrawShadowDialog2(sX, sY, dnumber);
//	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Rebirth", 255, 255, 255);
	// title


	DrawNewDialogBox2(SPRID_FLAGSLANG, sX + 195 + 80, sY + 13, 0, FALSE, true);
	DrawNewDialogBox2(SPRID_FLAGSLANG + 1, sX + 195 + 28 + 80, sY + 13, 0, FALSE, true);

	//PutCenterString_SpriteFont(sX, sX + sizeX, sY + 13, "Enchanting", 255, 200, 0);

	bool tab1 = mode == 0;
	bool tab2 = mode == 1;

	if (mode == 0 || mode == 1)
	{
		DrawEnhancement_Tab(sX + 44 + 2, sY + 24 + 6, msX, msY, m_sLang == LANG_ESP ? "Extraer" : "Extract", tab1);
		DrawEnhancement_Tab(sX + 44 + 128 + 1, sY + 24 + 6, msX, msY, m_sLang == LANG_ESP ? "Mejorar" : "Upgrade", tab2);
	}

	if (mode == 2)
	{
		PutAlignedString2(sX, sX + sizeX, sY + 50, m_sLang == LANG_ESP ? "Confirmar Extraccion" : "Confirm Extraction", 255, 0, 0);
		PutAlignedString2(sX + 20, sX + sizeX - 20, sY + 75, m_sLang == LANG_ESP ? "¡ADVERTENCIA: Los item seleccionados se perderan!" : "WARNING: The selected items will be lost!", 255, 0, 0);
	}
	else if (mode == 3)
	{
		PutAlignedString2(sX, sX + sizeX, sY + 55, m_sLang == LANG_ESP ? "Mejorar estadistica del item" : "Upgrade item stat", 255, 0, 0);
	}

	switch (mode)
	{
	case 0:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Enchanting", 240, 240, 240);

		int iTotalLines = 0;
		for (int b = 0; b < 20; b++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1)
			{
				iTotalLines++;
			}
		}

		if (iTotalLines == 0)
		{
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 95, m_sLang == LANG_ESP ? "Por favor ingresa un item o mas." : "Please enter one or more items.", 255, 255, 255);
		}

		int iPointerLoc;
		double d1, d2, d3;
		int maxlines = 13;

		if (iTotalLines > maxlines)
		{
			d1 = static_cast<double>(m_stDialogBoxInfo[72].sView);
			d2 = static_cast<double>(iTotalLines - maxlines);
			d3 = (275.0f * d1) / d2;
			iPointerLoc = static_cast<int>(d3 + 0.5);
			//DrawNewDialogBox2(SPRID_NEWSCROLL, sX + sizeX - 15, sY + 44, 0);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + sizeX - 13, sY + 44 + iPointerLoc, 7);
		}
		else
		{
			iPointerLoc = 0;
		}

		int tempindex = -1;

		for (int i = 0; i < maxlines; i++)
		{
			if ((i + m_stDialogBoxInfo[72].sView) < iTotalLines)
			{
				int enchantingItemIndex = m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i + m_stDialogBoxInfo[72].sView];
				if (enchantingItemIndex != -1)
				{
					char cTemp[512] = {};
					strcpy(cTemp, m_pItemList[enchantingItemIndex]->m_cName);
					if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
					{
						PutAlignedString(sX + 10, sX + sizeX - 10, sY + i * 18 + 75, cTemp, 255, 255, 255);
						tempindex = i + m_stDialogBoxInfo[72].sView;
					}
					else
					{
						PutAlignedString(sX + 10, sX + sizeX - 10, sY + i * 18 + 75, cTemp, 255, 200, 0);
					}
				}
			}
		}

		if (tempindex != -1 && (msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + (tempindex - m_stDialogBoxInfo[72].sView) * 18 + 75) && (msY <= sY + (tempindex - m_stDialogBoxInfo[72].sView) * 18 + 89))
		{
			DrawMarketItem(m_pItemList[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[tempindex]], msX + 50, msY);
		}

		// Manejo de clic del botón izquierdo
		if (cLB != 0 && iTotalLines > maxlines)
		{
			if (iGetTopDialogBoxIndex() == 72)
			{
				if ((msX >= sX + sizeX - 13) && (msX <= sX + sizeX + 12) && (msY >= sY + 44) && (msY <= sY + 374))
				{
					d1 = static_cast<double>(msY - (sY + 44));
					d2 = static_cast<double>(iTotalLines - maxlines);
					d3 = (d1 * d2) / 275.0f;
					m_stDialogBoxInfo[72].sView = static_cast<int>(d3 + 0.5);
				}
			}
		}
		else
		{
			m_stDialogBoxInfo[72].bIsScrollSelected = FALSE;
		}

		// Manejo del scroll del mouse
		if (iGetTopDialogBoxIndex() == 72 && msZ != 0)
		{
			m_stDialogBoxInfo[72].sView -= msZ / 120;  // Ajustar este valor para un desplazamiento más controlado
			if (m_stDialogBoxInfo[72].sView < 0)
			{
				m_stDialogBoxInfo[72].sView = 0;
			}
			if (m_stDialogBoxInfo[72].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[72].sView = iTotalLines - maxlines;
			}
			m_DInput.m_sZ = 0;  // Reiniciar el valor del scroll
		}

		// Limitar la vista dentro de los valores válidos
		if (iTotalLines > maxlines && m_stDialogBoxInfo[72].sView > iTotalLines - maxlines)
		{
			m_stDialogBoxInfo[72].sView = iTotalLines - maxlines;
		}
		if (m_stDialogBoxInfo[72].sView < 0 || iTotalLines <= maxlines)
		{
			m_stDialogBoxInfo[72].sView = 0;
		}

		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Extraer" : "Extract");
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Cerrar" : "Close");
		
		break;
	}
	case 1:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Enchanting", 240, 240, 240);
		DrawMenu_EnhBox(sX + 44 + 94, sY + 132 + 20, 80, 114);
		std::vector<std::string> enhancementOptions;

		bool isEnabled = false;
		bool optionSelected = false;
		std::string selectedStatText;
		std::string showselectedStatText;
		int selectedValue = 0;
		int istype = -1;

		// Llenar el combo box con las variables globales
		char optionBuffer[200];

		sprintf(optionBuffer, "Light: %d shards", shard_light);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Strong: %d shards", shard_strong);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Hit Prob: %d fragments", fragment_hp);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Defense Ratio: %d fragments", fragment_dr);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "HP Recovery: %d fragments", fragment_hprec);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "SP Recovery: %d fragments", fragment_sprec);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "MP Recovery: %d fragments", fragment_mprec);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Magic Resist: %d fragments", fragment_mr);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Physical Absorption: %d fragments", fragment_pa);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Magic Absorption: %d fragments", fragment_ma);
		enhancementOptions.push_back(optionBuffer);

		sprintf(optionBuffer, "Exp: %d fragments", fragment_exp);
		enhancementOptions.push_back(optionBuffer);

		// Dibujar el sprite del ítem primero
		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
			short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
			short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
			short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

			char ctype1[200] = { 0 };
			char ctype2[200] = { 0 };
			char tmpctype1[200] = { 0 };
			char tmpctype2[200] = { 0 };
			int multiplier = 0;

			i = m_stDialogBoxInfo[dnumber].sV1;
			cItemColor = m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cItemColor;
			if ((m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_LHAND)
				|| (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_RHAND)
				|| (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND)) {
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSprite]->PutSpriteRGB(sX + 44 + 134, sY + 182 + 20, m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
			}
			else {
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSprite]->PutSpriteRGB(sX + 44 + 134, sY + 182 + 20, m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
			}

			ZeroMemory(cStr1, sizeof(cStr1));
			ZeroMemory(cStr2, sizeof(cStr2));
			ZeroMemory(cStr3, sizeof(cStr3));
			GetItemName(m_pItemList[m_stDialogBoxInfo[dnumber].sV1], cStr1, cStr2, cStr3);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 230 + 20 + 20, cStr1, 255, 165, 0);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 245 + 20 + 20, cStr2, 255, 255, 255);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 260 + 20 + 20, cStr3, 255, 255, 255);

			isEnabled = true;
		}

		// Mostrar el combo box después del sprite
		DrawComboBoxEnc(sX + 17, sY + 28 + 25, sizeX - 36, 20, enhancementOptions, selectedOptionIndex, isComboBoxOpen, msX, msY, !enhancementOptions.empty());
		if (!enhancementOptions.empty()) {
			HandleComboBoxClick(msX, msY, sX + 44 + 3, sY + 28 + 25, sizeX - 6, 20, enhancementOptions, cLB, dnumber, !enhancementOptions.empty());
		}

		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
			short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
			short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
			short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

			char ctype1[200] = { 0 };
			char ctype2[200] = { 0 };
			char tmpctype1[200] = { 0 };
			char tmpctype2[200] = { 0 };
			int multiplier = 0;

			// Dibujar el texto del combo box sobre el sprite
			if (selectedOptionIndex == -1 && !isComboBoxOpen)
				PutAlignedString(sX + 10, sX + sizeX, sY + 120, m_sLang == LANG_ESP ? "Por favor, selecciona una estadistica valida." : "Please select valid stat.", 255, 0, 0);
			else if (m_stDialogBoxInfo[dnumber].sV1 != -1 && selectedOptionIndex != -1 && !isComboBoxOpen) {


				seltype = 0;
				SelectedType1 = -1;
				SelectedType2 = -1;

				// Verificar la opción seleccionada
				if (selectedOptionIndex == 0) {
					seltype = 1;
					SelectedType1 = 6;
				}
				else if (selectedOptionIndex == 1) {
					seltype = 1;
					SelectedType1 = 8;
				}
				else if (selectedOptionIndex == 2) {
					seltype = 2;
					SelectedType2 = 2;
				}
				else if (selectedOptionIndex == 3) {
					seltype = 2;
					SelectedType2 = 3;
				}
				else if (selectedOptionIndex == 4) {
					seltype = 2;
					SelectedType2 = 4;
				}
				else if (selectedOptionIndex == 5) {
					seltype = 2;
					SelectedType2 = 5;
				}
				else if (selectedOptionIndex == 6) {
					seltype = 2;
					SelectedType2 = 6;
				}
				else if (selectedOptionIndex == 7) {
					seltype = 2;
					SelectedType2 = 7;
				}
				else if (selectedOptionIndex == 8) {
					seltype = 2;
					SelectedType2 = 8;
				}
				else if (selectedOptionIndex == 9) {
					seltype = 2;
					SelectedType2 = 9;
				}
				else if (selectedOptionIndex == 10) {
					seltype = 2;
					SelectedType2 = 11;
				}

				int costPoints = 0;

				if (seltype != 0)
				{
					if (seltype == 1) {
						if (SelectedType1 == type1 || type1 == 0) {
							costPoints = (val1 + 1) * 400;
						}
					}
					else if (seltype == 2) {
						if (SelectedType2 == type2 || type2 == 0) {
							costPoints = (val2 + 1) * 400;
						}
					}
				}


				ZeroMemory(costMessage, sizeof(costMessage));

				if (costPoints == 0)
					sprintf(costMessage, m_sLang == LANG_ESP ? "No se puede mejorar este item con la estadística seleccionada." : "Can't upgrade this item with the selected stat.");
				else
					sprintf(costMessage, m_sLang == LANG_ESP ? "Costara %d puntos subir el item al siguiente nivel." : "It will cost %d points to enhance to the next level.", costPoints);

				PutAlignedString(sX + 10, sX + sizeX, sY + 120, costMessage, 255, 165, 0);
			}
		}

		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Mejorar" : "Upgrade");
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Cerrar" : "Close");

		break;
	}

	case 2:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, m_sLang == LANG_ESP ? "Confirmar extrac." : "Confirm Extract", 240, 240, 240);
		
		// Mapa para almacenar las estadísticas acumuladas
		std::map<std::string, int> statMap;

		for (int i = 0; i < 20; i++) {
			if (m_stDialogBoxInfo[72].m_iEnchantingItem[i] != -1) {
				auto item = m_pItemList[m_stDialogBoxInfo[72].m_iEnchantingItem[i]];
				if (!item) continue;

				short type1 = (item->m_dwAttribute & 0x00F00000) >> 20;
				short val1 = (item->m_dwAttribute & 0x000F0000) >> 16;
				short type2 = (item->m_dwAttribute & 0x0000F000) >> 12;
				short val2 = (item->m_dwAttribute & 0x00000F00) >> 8;

				if (PermitedType1(type1) && val1 != 0) {
					std::string statName = GetStatNameType1(type1);
					statMap[statName] += val1 * 10;
				}

				if (PermitedType2(type2) && val2 != 0) {
					std::string statName = GetStatNameType2(type2);
					statMap[statName] += val2 * 10;
				}
			}
		}

		PutAlignedString(sX + 10, sX + sizeX, sY + 100, m_sLang == LANG_ESP ? "Recibiras: " : "You will get: ", 255, 200, 0);

		int offsetY = sY + 115;
		for (const auto& stat : statMap) {
			char temp[100];
			wsprintf(temp, m_sLang == LANG_ESP ? "%d %s fragmentos" : "%d %s fragments", stat.second, (char*)stat.first.c_str());
			PutAlignedString(sX + 10, sX + sizeX, offsetY, temp, 255, 255, 255);
			offsetY += 15; 
		}
			
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Si" : "Yes");
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "No" : "No");

		break;
	}

	case 3:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, m_sLang == LANG_ESP ? "Confirmar mejora" : "Confirm Upgrade", 240, 240, 240);

		char texting[500];
		if (selectedOptionIndex == 0)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Light Shards: %d" : "Your Light Shards: %d", shard_light);
		}
		else if (selectedOptionIndex == 1)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Strong Shards: %d" : "Your Strong Shards: %d", shard_strong);
		}
		else if (selectedOptionIndex == 2)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de HP: %d" : "Your HP Fragments: %d", fragment_hp);
		}
		else if (selectedOptionIndex == 3)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de DR: %d" : "Your DR Fragments: %d", fragment_dr);
		}
		else if (selectedOptionIndex == 4)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de HP Rec: %d" : "Your HP Rec Fragments: %d", fragment_hprec);
		}
		else if (selectedOptionIndex == 5)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de SP Rec: %d" : "Your SP Rec Fragments: %d", fragment_sprec);
		}
		else if (selectedOptionIndex == 6)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de MP Rec: %d" : "Your MP Rec Fragments: %d", fragment_mprec);
		}
		else if (selectedOptionIndex == 7)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de MR: %d" : "Your MR Fragments: %d", fragment_mr);
		}
		else if (selectedOptionIndex == 8)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de Absorcion Fisica: %d" : "Your PA Fragments: %d", fragment_pa);
		}
		else if (selectedOptionIndex == 9)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de Absorcion Magica: %d" : "Your MA Fragments: %d", fragment_ma);
		}
		else if (selectedOptionIndex == 10)
		{
			wsprintf(texting, m_sLang == LANG_ESP ? "Tus Fragmentos de Exp: %d" : "Your EXP Fragments: %d", fragment_exp);
		}

		PutAlignedString(sX + 15, sX + sizeX - 15, sY + 83, texting, 255, 255, 20);

		PutAlignedString(sX + 15, sX + sizeX - 15, sY + 110, costMessage, 240, 240, 240);

		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
			short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
			short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
			short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

			char ctype1[200] = { 0 };
			char ctype2[200] = { 0 };
			char tmpctype1[200] = { 0 };
			char tmpctype2[200] = { 0 };
			int multiplier = 0;

			i = m_stDialogBoxInfo[dnumber].sV1;
			cItemColor = m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cItemColor;
			if ((m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_LHAND)
				|| (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_RHAND)
				|| (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND)) {
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSprite]->PutSpriteRGB(sX + 44 + 134, sY + 182 + 20, m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSpriteFrame, m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], dwTime);
			}
			else {
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSprite]->PutSpriteRGB(sX + 44 + 134, sY + 182 + 20, m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_sSpriteFrame, m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], dwTime);
			}

			ZeroMemory(cStr1, sizeof(cStr1));
			ZeroMemory(cStr2, sizeof(cStr2));
			ZeroMemory(cStr3, sizeof(cStr3));
			GetItemName(m_pItemList[m_stDialogBoxInfo[dnumber].sV1], cStr1, cStr2, cStr3);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 230 + 20 + 20, cStr1, 255, 165, 0);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 245 + 20 + 20, cStr2, 255, 255, 255);
			PutAlignedString(sX + 44 + 24, sX + 44 + 248, sY + 260 + 20 + 20, cStr3, 255, 255, 255);

		}

		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "Si" : "Yes");
		DrawMenu_SmallButton(sX + 44, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY + 65, m_sLang == LANG_ESP ? "No" : "No");
		break;
	}

	default:
		break;
	}

	//DrawDialogCord(sX, sY, msX, msY);
}


// Función para obtener el nombre de la estadística
string CGame::GetStatNameType1(short type) {
	switch (type) {
	case 6: return "Light";
	case 8: return "Strong";
	default: return "Unknown";
	}
}

string CGame::GetStatNameType2(short type) {
	switch (type) {
	case 2: return "Hitting Probability";
	case 3: return "Defense Ratio";
	case 4: return "HP Recovery";
	case 5: return "SP Recovery";
	case 6: return "MP Recovery";
	case 7: return "Magic Resistance";
	case 8: return "Physical Absorption";
	case 9: return "Magic Absorption";
	case 11: return "Exp";
	default: return "Unknown";
	}
}


bool CGame::PermitedType1(short type)
{
	switch (type)
	{
	case 6: //Light
	case 8: //Strong
		return true;
		break;

	default: return false; break;
	}

	return false;
}

bool CGame::PermitedType2(short type)
{
	switch (type)
	{
	case 2:	//hitting probability
	case 3:	//defense ratio
	case 4: //HP Recovery
	case 5: //SP Recovery
	case 6: //MP Recovery
	case 7: //Magic Resistance
	case 8: //Physical Absorption
	case 9: //Magic Absorption
	case 11: //Exp
		return true;
		break;
	default: return false; break;
	}

	return false;
}

void CGame::click_enchanting(short msX, short msY)
{
	short sX, sY, szX;
	int dnumber = 72;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	szX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	auto tab1 = (msX >= sX + 44 + 2) && (msX <= sX + 44 + 2 + 128) && (msY >= sY + 24) && (msY <= sY + 24 + 25);
	auto tab2 = (msX >= sX + 44 + 2 + 128 + 1) && (msX <= sX + 44 + 2 + 128 + 1 + 128) && (msY >= sY + 24) && (msY <= sY + 24 + 25);

	auto eng = (msX >= sX + 276) && (msX <= sX + 299) && (msY >= sY + 13) && (msY <= sY + 29);
	auto esp = (msX >= sX + 304) && (msX <= sX + 326) && (msY >= sY + 13) && (msY <= sY + 29);

	if (eng) {
		PlaySound('E', 14, 5); m_sLang = LANG_ENG;
	}
	if (esp) {
		PlaySound('E', 14, 5); m_sLang = LANG_ESP;
	}


	if (tab1)
	{
		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV1] = FALSE;
			m_stDialogBoxInfo[dnumber].sV1 = -1;
		}

		if (m_stDialogBoxInfo[dnumber].sV2 != -1) {
			m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV2] = FALSE;
			m_stDialogBoxInfo[dnumber].sV2 = -1;
		}
		
		for (int b = 0; b < 20; b++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1)
			{
				m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]] = FALSE;
				m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] = -1;
			}
		}

		isComboBoxOpen = false;
		selectedOptionIndex = -1;
		isClickProcessed = false;
		selectedType = 0;

		m_stDialogBoxInfo[dnumber].cMode = 0;
		PlaySound('E', 14, 5);
		return;
	}

	if (tab2)
	{

		if (m_stDialogBoxInfo[dnumber].sV1 != -1) {
			m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV1] = FALSE;
			m_stDialogBoxInfo[dnumber].sV1 = -1;
		}

		if (m_stDialogBoxInfo[dnumber].sV2 != -1) {
			m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV2] = FALSE;
			m_stDialogBoxInfo[dnumber].sV2 = -1;
		}
		
		for (int b = 0; b < 20; b++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1)
			{
				m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]] = FALSE;
				m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] = -1;
			}
		}

		isComboBoxOpen = false;
		selectedOptionIndex = -1;
		isClickProcessed = false;
		selectedType = 0;

		m_stDialogBoxInfo[dnumber].cMode = 1;
		PlaySound('E', 14, 5);
		return;
	}

	switch (mode)
	{
	case 0:
	{
		auto extract = (msX >= sX + 44 + 30) && (msX <= sX + 44 + 105) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);
		auto cancel = (msX >= sX + 44 + 153) && (msX <= sX + 44 + 230) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);

		if (cancel)
		{
			DisableDialogBox(72);
			PlaySound('E', 14, 5);
			return;
		}

		if (extract)
		{
			// Verificar si todos los ítems están en estado -1
			bool allItemsInvalid = true;
			for (int b = 0; b < 20; b++)
			{
				if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1 && m_pItemList[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]])
				{
					allItemsInvalid = false;
					break;
				}
			}

			// Si todos los ítems son inválidos, no continuar
			if (allItemsInvalid) return;


			m_stDialogBoxInfo[dnumber].cMode = 2;
			PlaySound('E', 14, 5);
		}
		break;
	}
	case 1:
	{
		auto fusion = (msX >= sX + 44 + 30) && (msX <= sX + 44 + 105) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);
		auto cancel = (msX >= sX + 44 + 153) && (msX <= sX + 44 + 230) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);

		if (cancel)
		{
			DisableDialogBox(72);
			PlaySound('E', 14, 5);
			return;
		}

		if (!m_pItemList[m_stDialogBoxInfo[dnumber].sV1] || m_stDialogBoxInfo[dnumber].sV1 == -1) return;
		short destItem = m_stDialogBoxInfo[dnumber].sV1;

		if (fusion)
		{
			if (m_stDialogBoxInfo[dnumber].sV1 != -1 && seltype != 0)
			{

				short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
				short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
				short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
				short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

				if (seltype == 1)
				{
					if (SelectedType1 == type1 || type1 == 0) {
						m_stDialogBoxInfo[dnumber].cMode = 3;
					}
				}
				else if (seltype == 2)
				{
					if (SelectedType2 == type2 || type2 == 0) {
						m_stDialogBoxInfo[dnumber].cMode = 3;
					}
				}

			}

			PlaySound('E', 14, 5);

			return;
		}
		break;
	}

	case 2:
	{
		auto yes = (msX >= sX + 44 + 30) && (msX <= sX + 44 + 105) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);
		auto no = (msX >= sX + 44 + 153) && (msX <= sX + 44 + 230) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);


		if (yes)
		{
			// Verificar si todos los ítems están en estado -1
			int count = 0;
			for (int b = 0; b < 20; b++)
			{
				if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1 && m_pItemList[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]])
				{					
					count++;
				//	break;
				}
			}

			// Si todos los ítems son inválidos, no continuar
			if (count == 0) return;


			char data[512];
			auto * cp = data;

			Push(cp, (u32)ENHANCEMENT_EXTRACT);
			Push(cp, (u16)0);

			Push(cp, count);

			for (int b = 0; b < 20; b++)
			{
				if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1 && m_pItemList[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]])
				{
					Push(cp, m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]);
				}
			}
			
			m_pGSock->iSendMsg(data, cp - data);

			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[dnumber].cMode = 0;
			return;
		}

		if (no)
		{
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[dnumber].cMode = 0;
			return;
		}

		break;
	}

	case 3:
	{
		auto yes = (msX >= sX + 44 + 30) && (msX <= sX + 44 + 105) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);
		auto no = (msX >= sX + 44 + 153) && (msX <= sX + 44 + 230) && (msY >= sY + 293 + 65) && (msY <= sY + 145 + 313 + 65);

		if (yes)
		{
			if (!m_pItemList[m_stDialogBoxInfo[dnumber].sV1] || m_stDialogBoxInfo[dnumber].sV1 == -1) return;
			short destItem = m_stDialogBoxInfo[dnumber].sV1;

			short type1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00F00000) >> 20;
			short val1 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x000F0000) >> 16;
			short type2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x0000F000) >> 12;
			short val2 = (m_pItemList[m_stDialogBoxInfo[dnumber].sV1]->m_dwAttribute & 0x00000F00) >> 8;

			if (seltype == 1)
			{
				if (SelectedType1 == type1 || type1 == 0) {
					char data[50];
					auto * cp = data;

					Push(cp, (u32)ENHANCEMENT_FUSION);
					Push(cp, (u16)0);

					Push(cp, destItem);
					Push(cp, seltype);
					Push(cp, SelectedType1);

					m_pGSock->iSendMsg(data, cp - data);
					m_stDialogBoxInfo[dnumber].sV1 = -1;
					for (int i = 0; i < 20; i++)
					{
						m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] = -1;
					}
					seltype = 0;
					m_bIsItemDisabled[destItem] = FALSE;

					isComboBoxOpen = false;
					selectedOptionIndex = -1;
					isClickProcessed = false;
					selectedType = 0;

					seltype = 0;
					SelectedType1 = -1;
					SelectedType2 = -1;

					PlaySound('E', 14, 5);
					m_stDialogBoxInfo[dnumber].cMode = 1;
				}
			}
			else if (seltype == 2)
			{
				if (SelectedType2 == type2 || type2 == 0) {
					char data[50];
					auto * cp = data;

					Push(cp, (u32)ENHANCEMENT_FUSION);
					Push(cp, (u16)0);

					Push(cp, destItem);
					Push(cp, seltype);
					Push(cp, SelectedType2);

					m_pGSock->iSendMsg(data, cp - data);
					m_stDialogBoxInfo[dnumber].sV1 = -1;
					for (int i = 0; i < 20; i++)
					{
						m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] = -1;
					}
					seltype = 0;
					m_bIsItemDisabled[destItem] = FALSE;

					isComboBoxOpen = false;
					selectedOptionIndex = -1;
					isClickProcessed = false;
					selectedType = 0;

					seltype = 0;
					SelectedType1 = -1;
					SelectedType2 = -1;

					PlaySound('E', 14, 5);
					m_stDialogBoxInfo[dnumber].cMode = 1;
				}
			}
			return;
		}

		if (no)
		{
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[dnumber].cMode = 1;
			return;
		}

		break;
	}

	default:
		break;
	}
}

void CGame::bItemDrop_enchanting()
{
	int dnumber = 72;
	char cItemID;
	cItemID = (char)m_stMCursor.sSelectedObjectID;

	if (m_bIsItemDisabled[cItemID] == TRUE) return;
	if (m_cCommand < 0) return;

	short type1 = (m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
	short val1 = (m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;
	short type2 = (m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
	short val2 = (m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;

	switch (m_stDialogBoxInfo[dnumber].cMode) {
	case 0:
	{
		if (m_pItemList[cItemID]->m_cEquipPos == DEF_EQUIPPOS_NONE) return;

		if (val1 == 0 && val2 == 0) return;

		int itemCount = 0;
		for (int i = 0; i < 20; i++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] != -1)
			{
				itemCount++;
			}
		}

		if (itemCount >= 20)
		{
			AddEventList("No hay espacio para más items.");
			return;
		}

		for (int i = 0; i < 20; i++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] == -1)
			{
				m_stDialogBoxInfo[dnumber].m_iEnchantingItem[i] = cItemID;
				m_bIsItemDisabled[cItemID] = TRUE;
				PlaySound('E', 29, 0);
				return;
			}
		}
		break;
	}
	case 1:

		if (m_pItemList[cItemID]->m_cEquipPos == DEF_EQUIPPOS_NONE) return;
		m_bIsItemDisabled[m_stDialogBoxInfo[dnumber].sV1] = FALSE;
		m_stDialogBoxInfo[dnumber].sV1 = cItemID;
		m_bIsItemDisabled[cItemID] = TRUE;
		PlaySound('E', 29, 0);

		break;
	}
}

void CGame::getCommandHandler(char * data)
{
	char command[30] = {};
	int val1 = 0;
	int val2 = 0;

	ZeroMemory(command, sizeof(command));
	Pop(data, command, 30);
	Pop(data, val1);
	Pop(data, val2);

	procCommandHandler(command, val1, val2);
}

void CGame::startReceiver()
{
	// Crear un hilo para ejecutar receiveMessage en paralelo
	std::thread receiverThread(&CGame::receiveMessageFromServer, this);

	// Opcional: Separar el hilo para que funcione de forma independiente
	receiverThread.detach(); // Permite que el hilo funcione sin necesidad de unirse
}

void CGame::procCommandHandler(char * command, int val1, int val2)
{
	//HeatoN rebirth
	if (strcmp(command, "/rebirthlevel") == 0)
	{
		m_iRebirthLevel = val1;
	}//heaton fast teleport free command for fast moving when teleport
	else if (strcmp(command, "/freecmd") == 0)
	{
		/*std::thread timerThread([this]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(180));
			this->m_bCommandAvailable = true;
			this->m_dwCommandTime = 0;
		});
		timerThread.detach();*/
		reqfree = true;
		return;
	}
	/*else if (strcmp(command, "/eventstatus") == 0)
	{
		bool status = val2 ? 1 : 0;
		g_ev.NotifyEventStatus((EventID)val1, status, 1);
		PlaySound('E', 25, 0, 0);
	}
	else if (strcmp(command, "/logineventstatus") == 0)
	{
		bool status = val2 ? 1 : 0;
		g_ev.NotifyEventStatus((EventID)val1, status, 2);
	}*/
	else if (strcmp(command, "/coins") == 0)
	{
		m_iCoins = val1;
	}
	else if (strcmp(command, "/rep") == 0)
	{
		m_iReputation = val1;
	}
	else if (strcmp(command, "/majs") == 0)
	{
		m_iGizonItemUpgradeLeft = val1;
	}
	else if (strcmp(command, "/rebirthstatus") == 0)
	{
		m_sRebirthStatus = val1;
	}
	else if (strcmp(command, "/rebirthenabled") == 0)
	{
		m_sRebirthEnabled = val1;
	}
	else if (strcmp(command, "/handle") == 0)
	{
		/*std::thread timerThread([this]() {
			UDPrequest_ping();
		});
		timerThread.detach();*/
		
		/*udpPort = 8888 + val1;

		const std::string SERVER_IP = "127.0.0.1"; // Replace with your server's IP address
		const int SERVER_PORT = udpPort;

		startUDPClient(SERVER_PORT);
		startReceiver();
		sendMessageToServer(SOCKMSG_WELCOME);*/
	}
	else if (strcmp(command, "/hp") == 0)
	{
		m_iHP = val1;
	}
	else if (strcmp(command, "/mp") == 0)
	{
		m_iMP = val1;
	}
	else if (strcmp(command, "/sp") == 0)
	{
		m_iSP = val1;
	}
	else if (strcmp(command, "/playerexp") == 0)
	{
		m_iExp = val1;
	}
	else if (strcmp(command, "/statspoints") == 0)
	{
		m_iLU_Point = val1;
	}
	else if (strcmp(command, "/playerlevel") == 0)
	{
		m_iLevel = val1;
	}
	else if (strcmp(command, "/playerexp") == 0)
	{
		m_iExp = val1;
	}
	else if (strcmp(command, "/str") == 0)
	{
		m_iStr = val1;
	}
	else if (strcmp(command, "/dex") == 0)
	{
		m_iDex = val1;
	}
	else if (strcmp(command, "/vit") == 0)
	{
		m_iVit = val1;
	}
	else if (strcmp(command, "/int") == 0)
	{
		m_iInt = val1;
	}
	else if (strcmp(command, "/mag") == 0)
	{
		m_iMag = val1;
	}
	else if (strcmp(command, "/chr") == 0)
	{
		m_iCharisma = val1;
	}
	else if (string(command) == "/contrib") {
		m_iContribution = val1;
	}
	else if (string(command) == "/enableitem") {
		for (int i = 0; i < 20; i++)
		{
			if (m_stDialogBoxInfo[72].m_iEnchantingItem[i] != -1 && val1 == i)
			{
				m_bIsItemDisabled[m_stDialogBoxInfo[72].m_iEnchantingItem[i]] = FALSE;
				m_stDialogBoxInfo[72].m_iEnchantingItem[i] = -1;
			}
		}		
	}
	else if (string(command) == "/acomodarcandys")
	{
		AcomodarCandys();
	}
	else if (string(command) == "/battlewinner")
	{
		// Activar la visualización del sprite y registrar el tiempo actual
		g_bShowWinnerSprite = true;
		g_dwWinnerSpriteStartTime = timeGetTime();
		PlaySound('E', 23, 0);
		AddEventList("You win Battle royale Round.", 10);
	}
	else if (string(command) == "/addnewitem")
	{	
		PlaySound('E', 20, 0);
	}
	else if (string(command) == "/maxhp")
	{
		m_iHP = (3 * m_iVit) + (2 * m_iLevel) + ((m_iStr + m_sRankAddStr + m_iAngelicStr) / 2);
	}
	else if (string(command) == "/cleartop5")
	{
		clearTopFive();
	}
	else if (string(command) == "/brfullhp")
	{
		m_iHP = (3 * m_iVit) + (2 * m_iLevel) + ((m_iStr + m_sRankAddStr + m_iAngelicStr) / 2);
	}
	else if (string(command) == "/closerevivepanel")
	{
		DisableDialogBox(74);
	}
	else if (string(command) == "/closeenchantingpanel")
	{
		m_bIsItemDisabled[m_stDialogBoxInfo[72].sV1] = FALSE;
		m_stDialogBoxInfo[72].sV1 = -1;
		for (int i = 0; i < 20; i++)
		{
			if (m_stDialogBoxInfo[72].m_iEnchantingItem[i] != -1)
			{
				m_bIsItemDisabled[m_stDialogBoxInfo[72].m_iEnchantingItem[i]] = FALSE;
				m_stDialogBoxInfo[72].m_iEnchantingItem[i] = -1;
			}
		}
		m_stDialogBoxInfo[72].dwV1 = NULL;
		isComboBoxOpen = false; // Bandera para controlar si el combobox está abierto o cerrado
		selectedOptionIndex = -1; // Índice de la opción seleccionada
		 // Bandera para controlar si el clic ya fue procesado
		selectedType = 0;
		isClickProcessed = false;
		//DisableDialogBox(72);
	}
	
	else if (string(command) == "/relicstatus") {
		m_sRelicStatus = val1;
		//if (val1 == 1) m_bRelicCaptured = true; else m_bRelicCaptured = false;
	}
	else if (string(command) == "/isadmin") {
		m_bAdmin = true;
	}
	else if (string(command) == "/playerside") {
		m_sSide = val1;
	}
	
	else if (string(command) == "/purgeteam") {
		m_spurgeteam = val1;
	}
	else if (string(command) == "/ctrfinishcount")
	{
		m_iCTRFinishCount = val1;
		finishmode = val2;
	}
	else if (string(command) == "/relicholder") {
		PlaySound('C', 25, 0);
		AddEventList("You have captured the relic.", 10);
	}
	else if (string(command) == "/rewsound") {
		PlaySound('E', 52, 0);
	}
	else if (string(command) == "/shards") {
		shard_light = val1;
		shard_strong = val2;
	}
	else if (string(command) == "/fragment_hp") {
		fragment_hp = val1;
	}
	else if (string(command) == "/fragment_dr") {
		fragment_dr = val1;
	}
	else if (string(command) == "/fragment_hprec") {
		fragment_hprec = val1;
	}
	else if (string(command) == "/fragment_sprec") {
		fragment_sprec = val1;
	}
	else if (string(command) == "/fragment_mprec") {
		fragment_mprec = val1;
	}
	else if (string(command) == "/fragment_mr") {
		fragment_mr = val1;
	}
	else if (string(command) == "/fragment_pa") {
		fragment_pa = val1;
	}
	else if (string(command) == "/fragment_ma") {
		fragment_ma = val1;
	}
	else if (string(command) == "/fragment_exp") {
		fragment_exp = val1;
	}
	else if (string(command) == "/additemcount") 
	{
		if (m_pItemList[val1])
		{
			m_pItemList[val1]->m_dwCount += val2;
		}
	}
	else if (string(command) == "/disablemanudialog")
	{
		DisableDialogBox(86);
	}
	else if (string(command) == "/newitemcount")
	{
		if (m_pItemList[val1])
		{
			m_pItemList[val1]->m_dwCount = val2;
		}
	}
	else if (string(command) == "/newsv3")
	{
		if (m_pItemList[val1])
		{
			m_pItemList[val1]->m_sItemSpecEffectValue3 = val2;
		}
	}
	else if (strcmp(command, "/lightningsegments") == 0)
	{
		m_iLightingSegments = val1;
	}
	else if (strcmp(command, "/icesegments") == 0)
	{
		m_iIceSegments = val1;
	}
	else if (strcmp(command, "/firesegments") == 0)
	{
		m_iFireSegments = val1;
	}
	else if (strcmp(command, "/firetalentlvl") == 0)
	{
		m_sFireTalentLvl = val1;
	}
	else if (strcmp(command, "/icetalentlvl") == 0)
	{
		m_sIceTalentLvl = val1;
	}
	else if (strcmp(command, "/lightningtalentlvl") == 0)
	{
		m_sLightningTalentLvl = val1;
	}

	else if (strcmp(command, "/firepoints") == 0)
	{
		m_iFirePoints = val1;
	}
	else if (strcmp(command, "/icepoints") == 0)
	{
		m_iIcePoints = val1;
	}
	else if (strcmp(command, "/lightningpoints") == 0)
	{
		m_iLightningPoints = val1;
	}

	else if (strcmp(command, "/talent") == 0)
	{
		m_sTalent = val1;
	}
	else if (strcmp(command, "/eks") == 0)
	{
		m_iEnemyKillCount = val1;
	}
	else if (strcmp(command, "/dkcitykills") == 0)
	{
		m_idkareskills = val1; 
		m_idkelvkills = val2;
	}

	else if (strcmp(command, "/evil_laught") == 0)
	{
		PlaySound('E', 62, 0);

		if (val1 > 0)
		{
			string slaught = "-" + to_string(val1) + "!";

			_RemoveChatMsgListByObjectID(m_sPlayerObjectID);
			for (int i = 1; i < DEF_MAXCHATMSGS; i++)
				if (m_pChatMsgList[i] == NULL) {
					m_pChatMsgList[i] = new class CMsg(23, (char*)slaught.c_str(), m_dwCurTime);
					m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;

					if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == FALSE) {
						delete m_pChatMsgList[i];
						m_pChatMsgList[i] = NULL;
					}
					break;
				}
		}		
	}
	else if (strcmp(command, "/showmenu") == 0)
	{
		ChangeGameMode(DEF_GAMEMODE_ONMAINMENU);
	}
	else if (strcmp(command, "/mazecount") == 0)
	{
		maze_mobs = val1;
	}
	else if (strcmp(command, "/eventpoints") == 0)
	{
		m_iEventPoints = val1;
	}
	else if (strcmp(command, "/fishoff") == 0)
	{
		m_bFishing = false;
	}
	else if (strcmp(command, "/purgeroundtime") == 0)
	{
		purge_newround = val1;
	}
	
	else if (strcmp(command, "/tokens") == 0)
	{
		m_iTokens = val1;
	}
	else if (strcmp(command, "/clearcharekquests") == 0)
	{
		vec_charekquest.clear();
	}
	else if (strcmp(command, "/responseping") == 0)
	{
		endTime = std::chrono::high_resolution_clock::now();
	}
	else if (strcmp(command, "/revive") == 0)
	{
		if (!m_bIsDialogEnabled[74]) {
			EnableDialogBox(74, NULL, NULL, NULL);
		}
		return;
	}
}

// Heaton Rebirth
void CGame::draw_rebirth(short msX, short msY) {
	short sX, sY, sizeX, sizeY;
	int dnumber = 73;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	int fix_x = 142;


	DrawShadowDialog2(sX, sY, dnumber);
	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Rebirth", 255, 255, 255);


	DrawNewDialogBox2(SPRID_FLAGSLANG, sX + 195 + 80, sY + 13, 0, FALSE, true);
	DrawNewDialogBox2(SPRID_FLAGSLANG + 1, sX + 195 + 28 + 80, sY + 13, 0, FALSE, true);

	/*PutString_SprFont2(sX + fix_x, sY + 13, "Rebirth", 255, 200, 0);*/
	
	// Textos dinámicos según el idioma
	const char* txtRebirth = "Rebirth";
	const char* txtSystemDesc = (m_sLang == LANG_ESP) ?
		"El sistema de rebirth te otorga 3 puntos adicionales de estadísticas por cada renacer." :
		"The rebirth system grants you 3 additional stat points for every rebirth.";
	const char* txtModeDesc = (m_sLang == LANG_ESP) ?
		"Iniciar un nuevo rebirth te pondrá en Modo Rebirth y te llevará al Nivel 1." :
		"Starting a new rebirth will enter Rebirth Mode, granting you one Rebirth Level and resetting you to Level 1.";
	const char* txtSkillsDesc = (m_sLang == LANG_ESP) ?
		"No perderás tus habilidades, pero no podrás usarlas hasta tener suficientes estadísticas." :
		"You will not lose your skills or spells, but you will be unable to use them until you have enough stats.";
	const char* txtPenalty = (m_sLang == LANG_ESP) ?
		"En Modo Rebirth, tendrás una penalización del -20%% de experiencia hasta alcanzar el Nivel %d nuevamente." :
		"While in Rebirth mode, you will have a -20%% experience penalty for each Rebirth Level until you reach Level %d again.";
	const char* txtToggle = (m_sLang == LANG_ESP) ?
		"Puedes alternar entre Modo Rebirth y Modo Normal (Nivel %d) en cualquier momento por un pequeño precio." :
		"You can toggle between Rebirth Mode and Normal Mode (Level %d) at any time for a small price.";
	const char* txtRequirements = (m_sLang == LANG_ESP) ? "Requisitos:" : "Requirements:";
	const char* txtStartRebirth = (m_sLang == LANG_ESP) ? "Iniciar Nuevo Rebirth" : "Start New Rebirth";
	const char* txtSwitchMode = (m_sLang == LANG_ESP) ? "Cambiar Modo Rebirth" : "Switch Rebirth Mode";
	const char* txtContribution = (m_sLang == LANG_ESP) ? "%d Contribución" : "%d Contribution";
	const char* txtGold = (m_sLang == LANG_ESP) ? "%d Oro" : "%d Gold";
	const char* txtfree = (m_sLang == LANG_ESP) ? "El primer Rebirth es gratis!" : "First rebirth is free!";

	const char* txt1 = (m_sLang == LANG_ESP) ?
		"Estas seguro que deseas realizar esta operacion?" :
		"you sure want to perform this operation?";

	switch (mode)
	{
	case 0:
	{
		char temp[5000];
		int maxlevel = m_iPlayerMaxLevel;

		// Descripciones
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 40, (char*)txtSystemDesc, 240, 240, 240);
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 75, (char*)txtModeDesc, 240, 240, 240);
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 110, (char*)txtSkillsDesc, 240, 240, 240);

		wsprintf(temp, txtPenalty, maxlevel);
		PutAlignedString(sX + 10, sX + sizeX - 20, sY + 145, temp, 240, 240, 240);

		ZeroMemory(temp, sizeof(temp));
		wsprintf(temp, txtToggle, maxlevel);
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 198, temp, 240, 240, 240);

		// Requisitos
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 250, (char*)txtRequirements, 255, 200, 0);

		if (m_iLevel == m_iPlayerMaxLevel && m_sRebirthStatus == 0 && m_sRebirthEnabled == 0) {
			// Mostrar Nivel y Contribución requerida

			int cost = m_iRebirthLevel * 5000;
			if (cost != 0)
			{
				wsprintf(G_cTxt, txtContribution, m_iRebirthLevel * 5000);
				PutAlignedString(sX + 10, sX + sizeX - 10, sY + 270, G_cTxt, 2, 220, 5);
			}
			else
			{
				wsprintf(G_cTxt, txtContribution, m_iRebirthLevel * 5000);
				PutAlignedString(sX + 20, sX + sizeX - 20, sY + 270, (char*)txtfree, 25, 203, 12);
			}

			// Botón para iniciar renacer
			DrawMenu_LongBut(sX, sY, msX, msY, 36, 360, (char*)txtStartRebirth);
		}
		else {
			// Mostrar costo en oro para cambiar de modo
			wsprintf(G_cTxt, txtGold, 5000);
			PutAlignedString(sX + 20, sX + sizeX - 20, sY + 270, G_cTxt, 25, 203, 12);

			// Botón para cambiar de modo
			DrawMenu_LongBut(sX, sY, msX, msY, 36, 360, (char*)txtSwitchMode);
		}
		break;
	}
	case 1:
	{
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 75, (char*)txt1, 240, 240, 240);
		DrawMenu_LongBut(sX, sY, msX, msY, 36, 360, (char*)txtStartRebirth);
		break;
	}
	case 2:
	{
		PutAlignedString(sX + 20, sX + sizeX - 20, sY + 75, (char*)txt1, 240, 240, 240);
		DrawMenu_LongBut(sX, sY, msX, msY, 36, 360, (char*)txtSwitchMode);
		break;
	}
	default:
		break;
	}

	DrawDialogCord(sX, sY, msX, msY);
}



void CGame::DrawDialogCord(short sX, short sY, short msX, short msY)
{
	return;
	string cords = to_string(msX - sX) + "/" + to_string(msY - sY);
	PutString2(msX + 10, msY + 25, (char*)cords.c_str(), 255, 255, 255);
}

void CGame::click_rebirth(short msX, short msY)
{
	short sX, sY, szX;
	int dnumber = 73;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	szX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	
	auto eng = (msX >= sX + 276) && (msX <= sX + 299) && (msY >= sY + 13) && (msY <= sY + 29);
	auto esp = (msX >= sX + 304) && (msX <= sX + 326) && (msY >= sY + 13) && (msY <= sY + 29);

	if (eng) {
		PlaySound('E', 14, 5); m_sLang = LANG_ENG;
	}
	if (esp) {
		PlaySound('E', 14, 5); m_sLang = LANG_ESP;
	}


	int fix_y = 360;

	auto rebirth_click = (msX >= sX + 36) && (msX <= sX + 36 + 278) && (msY >= sY + fix_y) && (msY <= sY + fix_y + 22);

	switch (mode)
	{
	case 0:
	{
		if (m_iLevel == m_iPlayerMaxLevel && m_sRebirthStatus == 0 && m_sRebirthEnabled == 0)
		{
			if (rebirth_click)
			{
				m_stDialogBoxInfo[dnumber].cMode = 1;
				PlaySound('E', 14, 5);
			}
		}
		else
		{
			if (rebirth_click)
			{
				m_stDialogBoxInfo[dnumber].cMode = 2;
				PlaySound('E', 14, 5);
			}
		}
		break;
	}
	case 1: 
	{
		if (rebirth_click)
		{
			if (m_iLevel != m_iPlayerMaxLevel)
			{
				wsprintf(G_cTxt, "You need to be level %d to be reborn!", m_iPlayerMaxLevel);
				AddEventList(G_cTxt);
				PlaySound('E', 14, 5);
				return;
			}

			SendChat("/reqrebirth");
			PlaySound('E', 14, 5);
			DisableDialogBox(73);
		}
		break; 
	}
	case 2:
	{ 
		if (rebirth_click)
		{
			SendChat("/reqswitch");
			PlaySound('E', 14, 5);
			DisableDialogBox(73);
		}
		break; 
	}
	default:
		break;
	}
}


void CGame::DrawMenu_LongBut(short sX, short sY, short msX, short msY, short x, short y, const char* text)
{
	if ((msX >= sX + x) && (msX <= sX + x + 250) && (msY >= sY + y) && (msY <= sY + y + 22)) {
		DrawNewDialogBox2(SPRID_LONGBUTTONS + 1, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 278, sY + y + 2, (char*)text, 255, 240, 0);
	}
	else
	{
		DrawNewDialogBox2(SPRID_LONGBUTTONS, sX + x, sY + y, 0, FALSE, FALSE);
		PutAlignedString2(sX + x, sX + x + 278, sY + y + 2, (char*)text, 255, 255, 255);
	}
}

// heaton send chat
void CGame::SendChat(char * str)
{
	bSendCommand(CLIENT_REQUEST_COMMAND_CHATMSG, NULL, NULL, NULL, NULL, NULL, str);
}

// draw enable events names heaton
void CGame::DrawActiveEvents()
{
	string s;

	s.append("");

	for (int i = 0; i < (int)EventID::Max; i++)
	{

		auto is = g_ev.Is((EventID)i);

		if (!is)
			continue;

		eventcount++;

		s.append(" - ");
		s.append(getEventName((EventID)i));
	}

	if (eventcount >= 1)
	{
		s.append(" -");
		eventcount = 0;
	}
	if (c_reso->IsResolution() == C1024x768)
	{
		PutAlignedString(1, 1023, 40, (char*)s.c_str(), 255, 255, 255);
	}
	else if (c_reso->IsResolution() == C800x600)
	{
		PutAlignedString(1, 799, 40, (char*)s.c_str(), 255, 255, 255);
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		PutAlignedString(1, 639, 40, (char*)s.c_str(), 255, 255, 255);
	}
}

void CGame::click_requestrevive(short msX, short msY)
{
	short sX, sY;
	int dnumber = 74;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;

	auto yes = (msX >= sX + 15) && (msX <= sX + 15 + 76) && (msY >= sY + 50) && (msY <= sY + 50 + 22);
	auto no = (msX >= sX + 163) && (msX <= sX + 163 + 76) && (msY >= sY + 50) && (msY <= sY + 50 + 22);

	if (isInMap("battle"))
	{
		if (yes)
		{
			char data[10];
			auto * cp = data;

			Push(cp, (u32)REVIVE_YES);
			Push(cp, (u16)0);

			m_pGSock->iSendMsg(data, cp - data);

			PlaySound('E', 14, 5);
			DisableDialogBox(74);
			return;
		}

		if (no)
		{
			char data[10];
			auto * cp = data;

			Push(cp, (u32)REVIVE_NO);
			Push(cp, (u16)0);

			m_pGSock->iSendMsg(data, cp - data);
			PlaySound('E', 14, 5);
			DisableDialogBox(74);
			return;
		}
		return;
	}

	if (yes)
	{
		SendChat("/reviveyes");
		PlaySound('E', 14, 5);
		DisableDialogBox(74);
		return;
	}

	if (no)
	{
		SendChat("/reviveno");
		PlaySound('E', 14, 5);
		DisableDialogBox(74);
		return;
	}
}

void CGame::draw_requestrevive(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 74;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;

	
	DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 6);
	

	PutAlignedString(sX, sX + sizeX, sY + 10, "Do you want revive in event?", 255, 255, 255);

	DrawMenu_SmallButton(sX, sY, msX, msY, 15, 50, "Yes");
	DrawMenu_SmallButton(sX, sY, msX, msY, 163, 50, "No");
}

//heaton
void CGame::request_ranking(short mode)
{
	switch (mode)
	{
	case DEF_TOPDM: vec_fighters.clear(); SendChat("/reqtopdm"); break;
	case DEF_TOPGG: vec_ggfighters.clear(); SendChat("/reqtopgg"); break;
	default: break;
	}
}

void CGame::click_top10(short msX, short msY)
{
	short sX, sY;
	int dnumber = 75;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;

}

void CGame::draw_top10(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 75;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;

	DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX, sY, 0);

	if (isInMap("gg"))
	{
		PutAlignedString(sX, sX + sizeX, sY + 10, "GunGame Top 10", 255, 255, 255);
		PutAlignedString(sX, sX + sizeX + 1, sY + 10, "GunGame Top 10", 255, 255, 255);

		PutString(sX + 20, sY + 52, "Pos", RGB(255, 200, 0));
		PutString(sX + 50, sY + 52, "Name", RGB(255, 200, 0));
		PutString(sX + 140, sY + 52, "Kills", RGB(255, 200, 0));

		for (int i = 0; i < vec_ggfighters.size(); i++)
		{
			auto y = sY + 75 + (i * 15);
			string pos = to_string(i + 1);
			pos.append(")");

			PutString(sX + 20, y, (char*)pos.c_str(), RGB(255, 200, 0));
			PutString(sX + 50, y, vec_ggfighters[i].fighter, RGB(255, 255, 255));
			PutString(sX + 140, y, (char*)to_string(vec_ggfighters[i].kills).c_str(), RGB(255, 255, 255));
			
		}

		return;
	}

	if (isInMap("dm"))
	{
		PutAlignedString(sX, sX + sizeX, sY + 10, "Deathmatch Top 10", 255, 255, 255);
		PutAlignedString(sX, sX + sizeX + 1, sY + 10, "Deathmatch Top 10", 255, 255, 255);

		PutString(sX + 20, sY + 52, "Pos", RGB(255, 200, 0));
		PutString(sX + 50, sY + 52, "Name", RGB(255, 200, 0));
		PutString(sX + 140, sY + 52, "Kills", RGB(255, 200, 0));

		
		PutString(sX + 200, sY + 52, "Deaths", RGB(255, 200, 0));
		

		for (int i = 0; i < vec_fighters.size(); i++)
		{
			auto y = sY + 75 + (i * 15);
			string pos = to_string(i + 1);
			pos.append(")");

			PutString(sX + 20, y, (char*)pos.c_str(), RGB(255, 200, 0));
			PutString(sX + 50, y, vec_fighters[i].fighter, RGB(255, 255, 255));
			PutString(sX + 140, y, (char*)to_string(vec_fighters[i].kills).c_str(), RGB(255, 255, 255));
			
			PutString(sX + 200, y, (char*)to_string(vec_fighters[i].deaths).c_str(), RGB(255, 255, 255));
			
		}

		return;
	}	
}

void CGame::get_guildbankdata(char* data)
{
	Pop(data, m_iGuildBankTokens);
	Pop(data, m_iGuildBankContribution);
	Pop(data, m_iGuildBankMajestics);
}

void CGame::getNewItemBankAtr(char* data)
{
	short destItem;
	DWORD destItemAtr;
	Pop(data, destItem);
	Pop(data, destItemAtr);

	if (!m_pBankList[destItem]) return;
	m_pBankList[destItem]->m_dwAttribute = destItemAtr;
}

void CGame::getNewItemAtr(char* data)
{
	short destItem;
	DWORD destItemAtr;
	Pop(data, destItem);
	Pop(data, destItemAtr);

	if (!m_pItemList[destItem]) return;
	m_pItemList[destItem]->m_dwAttribute = destItemAtr;
}

void CGame::deleteBankItem(char* data)
{
	short destItem;
	DWORD destItemAtr;
	Pop(data, destItem);

	if (!m_pBankList[destItem]) return;

	delete m_pBankList[destItem];
	m_pBankList[destItem] = NULL;
}

void CGame::getNewItemWeight(char* data)
{
	short destItem;
	WORD destItemweight;
	Pop(data, destItem);
	Pop(data, destItemweight);

	if (!destItem) return;
	m_pItemList[destItem]->m_wWeight = destItemweight;
}

void CGame::getNewItemEndurance(char* data)
{
	short destItem;
	int destItemEndu;
	int destItemMaxEndu;

	Pop(data, destItem);
	Pop(data, destItemEndu);
	Pop(data, destItemMaxEndu);

	if (!destItem) return;
	//alrevez no es? nook porq aca lo hace 
	m_pItemList[destItem]->m_wCurLifeSpan = destItemEndu;
	m_pItemList[destItem]->m_wMaxLifeSpan = destItemMaxEndu;
}

void CGame::get_top_deathmatch(char * data)
{
	short fighters = 0;
	char name[12] = {};
	int kills = 0;
	int deaths = 0;

	Pop(data, fighters);

	for (int i = 0; i < fighters; i++)
	{
		Pop(data, name, 12);
		Pop(data, kills);
		Pop(data, deaths);

		st_fighters u;

		ZeroMemory(u.fighter, sizeof(u.fighter));
		strcpy(u.fighter, name);
		u.kills = kills;
		u.deaths = deaths;

		vec_fighters.push_back(u);
	}	
}

void CGame::get_top_gungame(char * data)
{
	short fighters = 0;
	char name[12] = {};
	int kills = 0;

	Pop(data, fighters);

	for (int i = 0; i < fighters; i++)
	{
		Pop(data, name, 12);
		Pop(data, kills);

		st_ggfighters u;

		ZeroMemory(u.fighter, sizeof(u.fighter));
		strcpy(u.fighter, name);
		u.kills = kills;

		vec_ggfighters.push_back(u);
	}
}

void CGame::getBrStatus(char * data)
{
	Pop(data, m_sBrEventStatus);
}

void CGame::getBrRound(char * data)
{
	Pop(data, m_sBrRound);
}

void CGame::getBrTimeRemaining(char * data)
{
	ZeroMemory(cBrTimeRemaining, sizeof(cBrTimeRemaining));
	Pop(data, cBrTimeRemaining, 20);
}


void CGame::getBrFreezeTimeRemaining(char * data)
{
	ZeroMemory(cBrFreezeTimeRemaining, sizeof(cBrFreezeTimeRemaining));
	Pop(data, cBrFreezeTimeRemaining, 20);
}


void CGame::getBrSafeTimeStatus(char * data)
{
	short status;

	Pop(data, status);
	if (status == 0) m_bSafeTime = false; else m_bSafeTime = true;
	ZeroMemory(cBrSafeTimeRemaining, sizeof(cBrSafeTimeRemaining));
	Pop(data, cBrSafeTimeRemaining, 20);
}


void CGame::getBrRoundTime(char * data)
{
	ZeroMemory(cBrRoundTime, sizeof(cBrRoundTime));
	Pop(data, cBrRoundTime, 20);
}


void CGame::getBrRadius(char * data)
{
	Pop(data, g_radius);
	if (isInMap("battle")) m_bIsRedrawPDBGS = TRUE;
}

// draw cords on game
void CGame::DrawCords(short x, short y)
{
	return;
	DWORD dwTime = timeGetTime();
	WORD  wR, wG, wB;
	string s = std::to_string(x) + "/" + std::to_string(y);
	PutString2(x + 10, y + 25, (char*)s.c_str(), 255, 255, 255);
}

void CGame::DrawCenteredBoxWithPixels(int playerX, int playerY, int viewX, int viewY, int width, int height, int r, int g, int b)
{
	const int TILE_SIZE = 32;
	const int SCREEN_CENTER_X = 512; // 1024/2
	const int SCREEN_CENTER_Y = 384; // 768/2
	const int COMP_X = -512;
	const int COMP_Y = -437;

	// Convertir posición del jugador a pantalla
	int screenX = (playerX * TILE_SIZE) - viewX + SCREEN_CENTER_X + COMP_X;
	int screenY = (playerY * TILE_SIZE) - viewY + SCREEN_CENTER_Y + COMP_Y;

	// Calcular esquinas del recuadro
	int left = screenX - (width / 2);
	int top = screenY - (height / 2);
	int right = left + width;
	int bottom = top + height;

	// Dibujar bordes horizontales
	for (int x = left; x <= right; x++) {
		m_DDraw.PutPixel(x, top, r, g, b);    // Borde superior
		m_DDraw.PutPixel(x, bottom, r, g, b); // Borde inferior
	}

	// Dibujar bordes verticales
	for (int y = top; y <= bottom; y++) {
		m_DDraw.PutPixel(left, y, r, g, b);   // Borde izquierdo
		m_DDraw.PutPixel(right, y, r, g, b);  // Borde derecho
	}
}



void CGame::DrawMarketItem(CItem* item, int x, int y, int mx, int my)
{
	short msX, msY, msZ;
	char cLB, cRB;// , cMB;
	auto mousex = mx;
	char cStr1[64], cStr2[64], cStr3[64], cStr4[64], cStr5[64], cStr6[64], cItemsTotal[64], cStr7[64];
	int  iLoc, iLocStr2 = 0, iLocStr3 = 0, iLocStr4 = 0, iLocStr5 = 0, iLocStr6 = 0, iLocStr7 = 0, ItemsTotal = 0;
	int  iStrLen1 = 0, iStrLen2 = 0, iStrLen3 = 0, iStrLen4 = 0, iStrLen5 = 0, iStrLen6 = 0;
	short msY1 = 0, msX1 = 0, msK = 0;

	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);

	int posx = x == 0 ? msX - m_stMCursor.sDistX : x;
	int posy = y == 0 ? msY - m_stMCursor.sDistY : y;
	auto cItemColor = item->m_cItemColor;

	int magicspr = DEF_SPRID_ITEMPACK_PIVOTPOINT;

	if (cItemColor != 0) {
		if ((item->m_cEquipPos == DEF_EQUIPPOS_LHAND) ||
			(item->m_cEquipPos == DEF_EQUIPPOS_RHAND) ||
			(item->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
		{
			m_pSprite[magicspr + item->m_sSprite]->PutSpriteRGB(posx, posy,
				item->m_sSpriteFrame,
				m_wWR[cItemColor] - m_wR[0], m_wWG[cItemColor] - m_wG[0], m_wWB[cItemColor] - m_wB[0], 1);
		}
		else
		{
			m_pSprite[magicspr + item->m_sSprite]->PutSpriteRGB(posx, posy,
				item->m_sSpriteFrame,
				m_wR[cItemColor] - m_wR[0], m_wG[cItemColor] - m_wG[0], m_wB[cItemColor] - m_wB[0], 1);
		}
	}
	else
	{
		m_pSprite[magicspr + item->m_sSprite]->PutSpriteFast(posx, posy, item->m_sSpriteFrame, 1);
	}


	GetMarketItemName(item, cStr1, cStr2, cStr3, cStr7);

	//AddEventList(cStr7);

	if (mx != 0 && my != 0)
	{
		msX = mx;
		msY = my;
	}

	iLoc = 0;
	if (strlen(cStr1) != 0)
	{
		if (m_bIsSpecial)
		{
			if (!ShadowsBox)
				PutString(msX, msY + 25, cStr1, RGB(0, 255, 50), FALSE, 1);
		}
		else {
			if (!ShadowsBox)
				PutString(msX, msY + 25, cStr1, RGB(255, 255, 255), FALSE, 1);
		}
		iLoc += 15;
	}
	if (strlen(cStr2) != 0) 
	{
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr2, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr2 += 15;
	}
	if (strlen(cStr3) != 0)
	{
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr3, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr3 += 15;
	}
	/**/
	if ((item->m_sLevelLimit != 0) && ((item->m_dwAttribute & 0x00000001) == 0))
	{
		wsprintf(cStr4, "%s: %d", DRAW_DIALOGBOX_SHOP24, item->m_sLevelLimit);//"���� ����: %d"
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr4, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr4 += 15;
	}
	if ((item->m_cEquipPos != DEF_EQUIPPOS_NONE) && (item->m_wWeight >= 1100))
	{
		int	_wWeight = 0;
		if (item->m_wWeight % 100) _wWeight = 1;
		wsprintf(cStr5, DRAW_DIALOGBOX_SHOP15, item->m_wWeight / 100 + _wWeight);
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr5, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr5 += 15;
	}
	if (item->m_cEquipPos != DEF_EQUIPPOS_NONE || !strcmp(item->m_cName, "ZemstoneofSacrifice"))
	{
		wsprintf(cStr6, UPDATE_SCREEN_ONGAME10, item->m_wCurLifeSpan);
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr6, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr6 += 15;
	}

	if (strlen(cStr7) != 0)
	{
		if (!ShadowsBox)
			PutString(msX, msY + 25 + iLoc, cStr7, RGB(150, 150, 150), FALSE, 1);
		iLoc += 15;
		iLocStr7 += 15;
	}

	if (iLoc == 15)
	{
		iLoc = 0;
		for (int iTmp = 0; iTmp < DEF_MAXITEMS; iTmp++)
		{
			if (item != NULL)
			{
				if (strcmp(item->m_cName, item->m_cName) == 0) iLoc++;
				ItemsTotal = 1;
			}
		}
		if (iLoc > 1)
		{
		}
	}
	//Coloco el recuadro - LaloRamos
	if (ShadowsBox)
	{
		if (ItemsTotal > 1)
			iLoc = 30;
		else if (ItemsTotal == 1)
			iLoc = 13;

		//msY1 = msY;
		if (c_reso->IsResolution() == C800x600)
		{
			if ((msY + iLoc + 28) > 599)
				msY1 = 599;
			else msY1 = msY + iLoc + 28;
		}
		else if (c_reso->IsResolution() == C1024x768)
		{
			if ((msY + iLoc + 28) > 767)
				msY1 = 767;
			else msY1 = msY + iLoc + 28;
		}
		else if (c_reso->IsResolution() == C640x480)
		{
			if ((msY + iLoc + 28) > 479)
				msY1 = 479;
			else msY1 = msY + iLoc + 28;
		}

		msK = SpaceChar(cStr1);
		if (msK < SpaceChar(cStr2)) msK = SpaceChar(cStr2);
		if (msK < SpaceChar(cStr3)) msK = SpaceChar(cStr3);
		if (msK < SpaceChar(cStr4)) msK = SpaceChar(cStr4);
		if (msK < SpaceChar(cStr5)) msK = SpaceChar(cStr5);
		if (msK < SpaceChar(cStr6)) msK = SpaceChar(cStr6);

		msX1 = msX + msK;

		if (c_reso->IsResolution() == C800x600)
		{
			if (msX1 > 799)
				msX1 = 799;
		}
		else if (c_reso->IsResolution() == C1024x768)
		{
			if (msX1 > 1023)
				msX1 = 1023;
		}
		else if (c_reso->IsResolution() == C640x480)
		{
			if (msX1 > 639)
				msX1 = 639;
		}

		m_DDraw.DrawShadowBox(msX, msY + 25, msX1, msY1);
		PutAlignedString(msX, msX + msK, msY + 25, cStr1, 234, 176, 51);

		if (ItemsTotal > 1)
			PutAlignedString(msX, msX + msK, msY + 25 + 15, cItemsTotal, 255, 255, 255);
		if (iLocStr2 > 0)
		{	//Purity
			if (memcmp(GET_ITEM_NAME1, cStr2, 6) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 255, 255); // Blanco
			//Completion
			else if (memcmp(GET_ITEM_NAME2, cStr2, 10) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 255, 255); // Blanco
			//Critical
			else if (memcmp(GET_ITEM_NAME3, cStr2, 8) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 221, 203, 49); // Amarillo
			// Poisoning
			else if (memcmp(GET_ITEM_NAME4, cStr2, 9) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 209, 61); // Verde
			// Righteous
			else if (memcmp(GET_ITEM_NAME5, cStr2, 9) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 119, 119, 119); // 
			// Agile
			else if (memcmp(GET_ITEM_NAME6, cStr2, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 137, 185, 195); // 
			// Light
			else if (memcmp(GET_ITEM_NAME7, cStr2, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 235, 104); // 
			// Sharp
			else if (memcmp(GET_ITEM_NAME8, cStr2, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 41, 70, 218); // 
			// Strong
			else if (memcmp(GET_ITEM_NAME9, cStr2, 6) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 113, 113, 113); //
			// Ancient
			else if (memcmp(GET_ITEM_NAME10, cStr2, 7) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 195, 77, 147); //
			// Special
			else if (memcmp(GET_ITEM_NAME11, cStr2, 7) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 223, 148, 3); //
			// Mana Converting
			else if (memcmp(GET_ITEM_NAME12, cStr2, 15) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 113, 169, 226); //
			// Critical
			else if (memcmp(GET_ITEM_NAME13, cStr2, 8) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 221, 203, 49); //
			// Critical Hit Damage
			else if (memcmp(GET_ITEM_NAME14, cStr2, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 221, 203, 49); //
			// Poison Damage
			else if (memcmp(GET_ITEM_NAME15, cStr2, 13) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 209, 61); // Verde
			// Attack Speed -1
			else if (memcmp(GET_ITEM_NAME16, cStr2, 15) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 137, 185, 195); //
			// light
			else if (memcmp(GET_ITEM_NAME17, cStr2, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 235, 104); //
			// Damage added
			else if (memcmp(GET_ITEM_NAME18, cStr2, 12) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 206, 18, 115); //
			// Endurance
			else if (memcmp(GET_ITEM_NAME19, cStr2, 9) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 139, 139, 139); //
			// Extra Damage added
			else if (memcmp(GET_ITEM_NAME20, cStr2, 18) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 228, 184, 67); //
			// Magic Casting Probability
			else if (memcmp(GET_ITEM_NAME21, cStr2, 25) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 214, 72, 72); //
			// Replace %d%% damage to mana
			else if (memcmp(GET_ITEM_NAME22, cStr2, 7) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 113, 169, 226); //
			// Crit Increase Chance
			else if (memcmp(GET_ITEM_NAME23, cStr2, 20) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 221, 203, 49); //
			// Poison Resistance
			else if (memcmp(GET_ITEM_NAME24, cStr2, 17) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 209, 61); // Verde
			// Hitting Probability
			else if (memcmp(GET_ITEM_NAME25, cStr2, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 200, 165, 3); //
			// Defense Ratio
			else if (memcmp(GET_ITEM_NAME26, cStr2, 13) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 170, 127, 174); //
			// HP recovery
			else if (memcmp(GET_ITEM_NAME27, cStr2, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 216, 38, 38); // Rojo
			// SP recovery
			else if (memcmp(GET_ITEM_NAME28, cStr2, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 209, 61); // Verde
			// MP recovery
			else if (memcmp(GET_ITEM_NAME29, cStr2, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 113, 169, 226); // Azul
			// Magic Resistance
			else if (memcmp(GET_ITEM_NAME30, cStr2, 16) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 145, 187, 87); // 
			// Physical Absorption
			else if (memcmp(GET_ITEM_NAME31, cStr2, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 38, 122, 118); // 
			// Magic Absorption
			else if (memcmp(GET_ITEM_NAME32, cStr2, 16) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 175, 26, 100); //
			// Consecutive Attack Damage
			else if (memcmp(GET_ITEM_NAME33, cStr2, 25) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 228, 184, 67); //
			// Experience
			else if (memcmp(GET_ITEM_NAME34, cStr2, 10) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 168, 74, 140); //
			// Gold
			else if (memcmp(GET_ITEM_NAME35, cStr2, 4) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 223, 148, 3); //
			else
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr2, cStr2, 255, 255, 255);
			if (iLocStr3 > 0)
				iLocStr3 += iLocStr2;
			else if (iLocStr4 > 0)
				iLocStr4 += iLocStr2;
			else if (iLocStr5 > 0)
				iLocStr5 += iLocStr2;
			else if (iLocStr6 > 0)
				iLocStr6 += iLocStr2;
			else if (iLocStr7 > 0)
				iLocStr7 += iLocStr2;
		}
		if (iLocStr3 > 0)
		{
			//Purity
			if (memcmp(GET_ITEM_NAME1, cStr3, 6) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 255, 255, 255); // Blanco
			//Completion
			else if (memcmp(GET_ITEM_NAME2, cStr3, 10) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 255, 255, 255); // Blanco
			// Critical Hit Damage
			else if (memcmp(GET_ITEM_NAME14, cStr3, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 221, 203, 49); //
			// Poison Damage
			else if (memcmp(GET_ITEM_NAME15, cStr3, 13) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 145, 209, 61); // Verde
			// Attack Speed -1
			else if (memcmp(GET_ITEM_NAME16, cStr3, 15) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 137, 185, 195); //
			// light
			else if (memcmp(GET_ITEM_NAME17, cStr3, 5) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 255, 235, 104); //
			// Damage added
			else if (memcmp(GET_ITEM_NAME18, cStr3, 12) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 206, 18, 115); //
			// Endurance
			else if (memcmp(GET_ITEM_NAME19, cStr3, 9) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 139, 139, 139); //
			// Extra Damage added
			else if (memcmp(GET_ITEM_NAME20, cStr3, 18) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 228, 184, 67); //
			// Magic Casting Probability
			else if (memcmp(GET_ITEM_NAME21, cStr3, 25) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 214, 72, 72); //
			// Replace %d%% damage to mana
			else if (memcmp(GET_ITEM_NAME22, cStr3, 7) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 113, 169, 226); //
			// Crit Increase Chance
			else if (memcmp(GET_ITEM_NAME23, cStr3, 20) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 221, 203, 49); //
			// Poison Resistance
			else if (memcmp(GET_ITEM_NAME24, cStr3, 17) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 145, 209, 61); // Verde
			// Hitting Probability
			else if (memcmp(GET_ITEM_NAME25, cStr3, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 200, 165, 3); //
			// Defense Ratio
			else if (memcmp(GET_ITEM_NAME26, cStr3, 13) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 170, 127, 174); //
			// HP recovery
			else if (memcmp(GET_ITEM_NAME27, cStr3, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 216, 38, 38); // Rojo
			// SP recovery
			else if (memcmp(GET_ITEM_NAME28, cStr3, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 145, 209, 61); // Verde
			// MP recovery
			else if (memcmp(GET_ITEM_NAME29, cStr3, 11) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 113, 169, 226); // Azul
			// Magic Resistance
			else if (memcmp(GET_ITEM_NAME30, cStr3, 16) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 145, 187, 87); // 
			// Physical Absorption
			else if (memcmp(GET_ITEM_NAME31, cStr3, 19) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 38, 122, 118); // 
			// Magic Absorption
			else if (memcmp(GET_ITEM_NAME32, cStr3, 16) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 175, 26, 100); //
			// Consecutive Attack Damage
			else if (memcmp(GET_ITEM_NAME33, cStr3, 25) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 228, 184, 67); //
			// Experience
			else if (memcmp(GET_ITEM_NAME34, cStr3, 10) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 168, 74, 140); //
			// Gold
			else if (memcmp(GET_ITEM_NAME35, cStr3, 4) == 0)
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 223, 148, 3); //
			else
				PutAlignedString(msX, msX + msK, msY + 25 + iLocStr3, cStr3, 250, 250, 250);
			if (iLocStr4 > 0)
				iLocStr4 += iLocStr3;
			else if (iLocStr5 > 0)
				iLocStr5 += iLocStr3;
			else if (iLocStr6 > 0)
				iLocStr6 += iLocStr3;
			else if (iLocStr7 > 0)
				iLocStr7 += iLocStr3;
		}
		if (iLocStr4 > 0)
		{
			PutAlignedString(msX, msX + msK, msY + 25 + iLocStr4, cStr4, 255, 255, 255);
			if (iLocStr5 > 0)
				iLocStr5 += iLocStr4;
			else if (iLocStr6 > 0)
				iLocStr6 += iLocStr4;
			else if (iLocStr7 > 0)
				iLocStr7 += iLocStr4;
		}
		if (iLocStr5 > 0)
		{
			PutAlignedString(msX, msX + msK, msY + 25 + iLocStr5, cStr5, 255, 255, 255);
			if (iLocStr6 > 0)
				iLocStr6 += iLocStr5;
			else if (iLocStr7 > 0)
				iLocStr7 += iLocStr5;
		}
		if (iLocStr6 > 0)
		{
			PutAlignedString(msX, msX + msK, msY + 25 + iLocStr6, cStr6, 255, 255, 255);

			if (iLocStr7 > 0)
				iLocStr7 += iLocStr6;
		}	
		if (iLocStr7 > 0)
			PutAlignedString(msX, msX + msK, msY + 25 + iLocStr7, cStr7, 255, 255, 255);
	}
}


bool CGame::isCountableItem(string itemname)
{
	return string(itemname) == "RepBall" || string(itemname) == "ContribBall" || string(itemname) == "Tokens" || string(itemname) == "CoinBall" ||
		string(itemname) == "LightShards" || string(itemname) == "StrongShards" || string(itemname) == "HitProbFragments" || string(itemname) == "DRFragments" ||
		string(itemname) == "HPRecFragments" || string(itemname) == "SPRecFragments" || string(itemname) == "MPRecFragments" || string(itemname) == "MRFragments" ||
		string(itemname) == "PaFragments" || string(itemname) == "MaFragments" || string(itemname) == "ExpFragments" || string(itemname) == "MajBall" ||
		string(itemname) == "Iron" || string(itemname) == "Copper" || string(itemname) == "Tin" || string(itemname) == "Bronze" || string(itemname) == "Silver" ||
		string(itemname) == "Steel" || string(itemname) == "Platinum" || string(itemname) == "Obsidian" || string(itemname) == "Cobalt" ||
		string(itemname) == "Mithril" || string(itemname) == "Adamantite" || string(itemname) == "Orichalcum" || string(itemname) == "Titanium" ||
		string(itemname) == "Darksteel" || string(itemname) == "Palladium" || string(itemname) == "Aetherium" || string(itemname) == "Dragonite" ||
		string(itemname) == "Fish" || string(itemname) == "RedFish" || string(itemname) == "GreenFish" || string(itemname) == "YellowFish" ||
		string(itemname) == "GoldFish" || string(itemname) == "ClownFish" || string(itemname) == "AngelFish" || string(itemname) == "BettaFish" ||
		string(itemname) == "ParrotFish" || string(itemname) == "LionFish" || string(itemname) == "PufferFish" || string(itemname) == "BluefinFish" ||
		string(itemname) == "Shark" || string(itemname) == "Eclipsium" || string(itemname) == "Voidsteel" || string(itemname) == "Astronyx" || string(itemname) == "Netherite" ||

		string(itemname) == "Bloodstone" || string(itemname) == "Ashbone" ||
		string(itemname) == "Ironshade" || string(itemname) == "Oblivionite" ||
		string(itemname) == "WhiteShark" || string(itemname) == "BlueShark" ||
		string(itemname) == "LemonShark" || string(itemname) == "Sunfish" ||
		string(itemname) == "Blobfish" || string(itemname) == "Viperfish" ||


		string(itemname) == "Ferronyx" || string(itemname) == "Lunacite" ||
		string(itemname) == "Gravitanium" || string(itemname) == "Pyrosite" ||
		string(itemname) == "Crytherium" || string(itemname) == "Voltrium" ||

		string(itemname) == "TetraFish" || string(itemname) == "TangFish" ||
		string(itemname) == "ButterFish" || string(itemname) == "CopperFish" ||
		string(itemname) == "ReefFish" ||

		string(itemname) == "Solvaris" ||
		string(itemname) == "Cryovale" ||
		string(itemname) == "Lumivor" ||
		string(itemname) == "Emberveil" ||
		string(itemname) == "Pyrelith" ||

		string(itemname) == "SkyOctopus" ||
		string(itemname) == "PinkOctopus" ||
		string(itemname) == "RedOctopus" ||
		string(itemname) == "SkyHorse" ||
		string(itemname) == "OrangeHorse" ||
		string(itemname) == "Megalodon" ||

		// Nuevos ítems añadidos
		string(itemname) == "CyclopsEye" || string(itemname) == "CyclopsHandEdge" || string(itemname) == "CyclopsHeart" || string(itemname) == "CyclopsMeat" ||
		string(itemname) == "CyclopsLeather" || string(itemname) == "HelboundHeart" || string(itemname) == "HelboundLeather" || string(itemname) == "HelboundTail" ||
		string(itemname) == "HelboundTeeth" || string(itemname) == "HelboundClaw" || string(itemname) == "HelboundTongue" || string(itemname) == "LumpofClay" ||
		string(itemname) == "OrcMeat" || string(itemname) == "OrcLeather" || string(itemname) == "OrcTeeth" || string(itemname) == "OgreHair" ||
		string(itemname) == "OgreHeart" || string(itemname) == "OgreMeat" || string(itemname) == "OgreLeather" || string(itemname) == "OgreTeeth" ||
		string(itemname) == "OgreClaw" || string(itemname) == "ScorpionPincers" || string(itemname) == "ScorpionMeat" || string(itemname) == "ScorpionSting" ||
		string(itemname) == "ScorpionSkin" || string(itemname) == "SkeletonBones" || string(itemname) == "SlimeJelly" || string(itemname) == "StoneGolemPiece" ||
		string(itemname) == "TrollHeart" || string(itemname) == "TrollMeat" || string(itemname) == "TrollLeather" || string(itemname) == "TrollClaw" ||
		string(itemname) == "DemonEye" || string(itemname) == "DemonHeart" || string(itemname) == "DemonMeat" || string(itemname) == "DemonLeather" ||
		string(itemname) == "UnicornHeart" || string(itemname) == "UnicornHorn" || string(itemname) == "UnicornMeat" || string(itemname) == "UnicornLeather" ||
		string(itemname) == "WerewolfHeart" || string(itemname) == "WerewolfNail" || string(itemname) == "WerewolfMeat" || string(itemname) == "WerewolfTail" ||
		string(itemname) == "WerewolfTeeth" || string(itemname) == "WerewolfLeather" || string(itemname) == "WerewolfClaw" ||
		string(itemname) == "IceSegments" ||
		string(itemname) == "LightningSegments" ||
		string(itemname) == "FireSegments";
}
// drop item in market
bool CGame::bItemDrop_Market()
{
	auto sX = m_stDialogBoxInfo[76].sX;
	auto sY = m_stDialogBoxInfo[76].sY;
	auto szx = m_stDialogBoxInfo[76].sSizeX;

	short msX, msY, msZ;
	char cLB, cRB, cMB;
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);

	int item_index = m_stMCursor.sSelectedObjectID;
	if (!m_pItemList[item_index])
		return false;

	if (m_pItemList[item_index]->m_cItemType == DEF_ITEMTYPE_CONSUME ||
		m_pItemList[item_index]->m_cItemType == DEF_ITEMTYPE_ARROW)
	{
		if (!isCountableItem(m_pItemList[item_index]->m_cName))
		{
			AddEventList("Cannot place a countable item");
			return false;
		}		
	}
	
	if (m_stDialogBoxInfo[76].cMode != 1)
		return false;

	market_item = m_pItemList[item_index];
	m_bIsItemDisabled[item_index] = TRUE;
	market_item_index = item_index;
	EnableDialogBox(17, 1, NULL, NULL);
	m_stDialogBoxInfo[17].sV11 = m_stMCursor.sSelectedObjectID + 1;
	return true;
}


// draw market dialog
void CGame::DrawDialogBox_Market(short msX, short msY, short msZ, char cLB)
{
	auto sX = m_stDialogBoxInfo[76].sX;
	auto sY = m_stDialogBoxInfo[76].sY;
	auto szx = m_stDialogBoxInfo[76].sSizeX;
	int r = 0;
	int g = 0;
	int b = 0;

	int r2 = 0;
	int g2 = 0;
	int b2 = 0;

	r = 255; g = 255; b = 0;

	int addy = 35;

	DrawShadowDialog2(sX, sY, 76);

	if (m_sLang == LANG_ENG)
	{
		PutCenterString_SpriteFont(sX, sX + szx, sY + 3, "Market Place", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + szx, sY + 4, "Market Place", 255, 200, 0);
	}
	else if (m_sLang == LANG_ESP)
	{
		PutCenterString_SpriteFont(sX, sX + szx, sY + 3, "Mercado", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + szx, sY + 4, "Mercado", 255, 200, 0);
	}


	if (m_sLang == LANG_ENG)
	{

	}
	else if (m_sLang == LANG_ESP)
	{

	}
	string s;

	DrawNewDialogBox2(SPRID_FLAGSLANG, sX + 195 + 80 - 80, sY + 13, 0, FALSE, true);
	DrawNewDialogBox2(SPRID_FLAGSLANG + 1, sX + 195 + 28 + 80 - 80, sY + 13, 0, FALSE, true);

	switch (m_stDialogBoxInfo[76].cMode)
	{
	case 0:

		if (m_sLang == LANG_ENG)
		{
			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
			{
				PutAlignedString(sX, sX + szx, sY + 70, "Post an Item", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 70, "Post an Item", 255, 255, 255);
			}

			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
			{
				PutAlignedString(sX, sX + szx, sY + 95, "Search for an Item", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 95, "Search for an Item", 255, 255, 255);
			}

			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
			{
				PutAlignedString(sX, sX + szx, sY + 120, "My posted Items", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 120, "My posted Items", 255, 255, 255);
			}

			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Close", 250, 250, 250);
			else PutString_SprFont2(sX + 195, sY + 290 + addy, "Close", 255, 200, 0);
		}
		else if (m_sLang == LANG_ESP)
		{
			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
			{
				PutAlignedString(sX, sX + szx, sY + 70, "Publicar un item", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 70, "Publicar un item", 255, 255, 255);
			}

			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
			{
				PutAlignedString(sX, sX + szx, sY + 95, "Buscar un item", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 95, "Buscar un item", 255, 255, 255);
			}

			if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
			{
				PutAlignedString(sX, sX + szx, sY + 120, "Mis items publicados", 255, 200, 0);
			}
			else
			{
				PutAlignedString(sX, sX + szx, sY + 120, "Mis items publicados", 255, 255, 255);
			}

			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Cerrar", 250, 250, 250);
			else PutString_SprFont2(sX + 195, sY + 290 + addy, "Cerrar", 255, 200, 0);
		}

		break;

	case 1:
		if (!market_item)
		{
			if (m_sLang == LANG_ENG)
			{
				PutAlignedString(sX + 30, sX + szx - 30, sY + 175, "Drag an Item from your bag and set the price in tokens", 255, 255, 255);
			}
			else if (m_sLang == LANG_ESP)
			{
				PutAlignedString(sX + 30, sX + szx - 30, sY + 175, "Arrastra un item desde tu bolsa y establece el precio en tokens", 255, 255, 255);
			}
		}
		else
		{
			DrawMarketItem(market_item, sX + 60, sY + 70, sX + 60, sY + 70);
			//DrawItemInfo(market_item, sX + 60, sY + 70, sX + 60, sY + 70);

			if (market_tokens > 0)
			{
				wsprintf(G_cTxt, "Tokens: %d", market_tokens);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);


				if (m_sLang == LANG_ENG)
				{
					DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Yes");
					DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");

				}
				else if (m_sLang == LANG_ESP)
				{
					DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Si");
					DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");

				}

				break;
			}
		}

		if (m_sLang == LANG_ENG)
		{
			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Back", 250, 250, 250);
			else PutString_SprFont2(sX + 195, sY + 290 + addy, "Back", 255, 200, 0);

		}
		else if (m_sLang == LANG_ESP)
		{
			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Atras", 250, 250, 250);
			else PutString_SprFont2(sX + 195, sY + 290 + addy, "Atras", 255, 200, 0);

		}
		break;

		case 2:
		{
			int iTotalLines = static_cast<int>(market_names.size());

			int iPointerLoc;
			double d1, d2, d3;
			int maxlines = 13;

			if (iTotalLines > maxlines)
			{
				d1 = static_cast<double>(m_stDialogBoxInfo[76].sView);
				d2 = static_cast<double>(iTotalLines - maxlines);
				d3 = (275.0f * d1) / d2;
				iPointerLoc = static_cast<int>(d3 + 0.5);
				//DrawNewDialogBox2(SPRID_NEWSCROLL, sX + szx - 15, sY + 44, 0);
				DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + szx - 13, sY + 44 + iPointerLoc, 7);
			}
			else
			{
				iPointerLoc = 0;
			}

			// Mostrar texto según el idioma
			if (m_sLang == LANG_ENG) {
				PutAlignedString(sX + 19, sX + 237, sY + 55, "Character Name", 255, 255, 255);//60
			}
			else if (m_sLang == LANG_ESP) {
				PutAlignedString(sX + 19, sX + 237, sY + 55, "Nombre del personaje", 255, 255, 255);
			}

			// Dibujar nombres de mercado
			for (int i = 0; i < 13; i++) {
				if ((i + m_stDialogBoxInfo[76].sView) < market_names.size()) {
					char cTemp[512] = {};
					strcpy(cTemp, market_names[i + m_stDialogBoxInfo[76].sView].c_str());
					if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89)) {
						PutAlignedString(sX + 19, sX + 237, sY + i * 18 + 75, cTemp, 255, 255, 255);
					}
					else {
						PutAlignedString(sX + 19, sX + 237, sY + i * 18 + 75, cTemp, r, g, b);
					}
				}
			}

			// Manejo de clic del botón izquierdo
			if (cLB != 0 && iTotalLines > maxlines)
			{
				if (iGetTopDialogBoxIndex() == 76)
				{
					if ((msX >= sX + szx - 13) && (msX <= sX + szx + 12) && (msY >= sY + 44) && (msY <= sY + 374))
					{
						d1 = static_cast<double>(msY - (sY + 44));
						d2 = static_cast<double>(iTotalLines - maxlines);
						d3 = (d1 * d2) / 275.0f;
						m_stDialogBoxInfo[76].sView = static_cast<int>(d3 + 0.5);
					}
				}
			}
			else
			{
				m_stDialogBoxInfo[76].bIsScrollSelected = FALSE;
			}

			// Manejo del scroll del mouse
			if (iGetTopDialogBoxIndex() == 76 && msZ != 0)
			{
				m_stDialogBoxInfo[76].sView -= msZ / 120;  // Ajustar este valor para un desplazamiento más controlado
				if (m_stDialogBoxInfo[76].sView < 0)
				{
					m_stDialogBoxInfo[76].sView = 0;
				}
				if (m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
				{
					m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
				}
				m_DInput.m_sZ = 0;  // Reiniciar el valor del scroll
			}

			// Limitar la vista dentro de los valores válidos
			if (iTotalLines > maxlines && m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
			}
			if (m_stDialogBoxInfo[76].sView < 0 || iTotalLines <= maxlines)
			{
				m_stDialogBoxInfo[76].sView = 0;
			}

			// Mostrar mensajes adicionales según el idioma
			if (m_sLang == LANG_ENG) {
				if (iTotalLines == 0) {
					PutAlignedString(sX + 19, sX + 237, sY + 165, "No operations available in Market Place", 255, 255, 255);
				}

				if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy)) {
					PutString_SprFont2(sX + 195, sY + 290 + addy, "Back", 250, 250, 250);
				}
				else {
					PutString_SprFont2(sX + 195, sY + 290 + addy, "Back", 255, 200, 0);
				}
			}
			else if (m_sLang == LANG_ESP) {
				if (iTotalLines == 0) {
					PutAlignedString(sX + 19, sX + 237, sY + 165, "No hay operaciones disponibles en el mercado.", 255, 255, 255);
				}

				if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy)) {
					PutString_SprFont2(sX + 195, sY + 290 + addy, "Atras", 250, 250, 250);
				}
				else {
					PutString_SprFont2(sX + 195, sY + 290 + addy, "Atras", 255, 200, 0);
				}
			}

			break;
		}


	case 3:
	{
		int iTotalLines = static_cast<int>(market_items.size());

		int iPointerLoc;
		double d1, d2, d3;
		int maxlines = 13;

		if (m_sLang == LANG_ENG)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Tokens", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Tokens", 255, 255, 255);
		}
		else if (m_sLang == LANG_ESP)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Tokens", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Tokens", 255, 255, 255);
		}

		if (iTotalLines > maxlines)
		{
			d1 = static_cast<double>(m_stDialogBoxInfo[76].sView);
			d2 = static_cast<double>(iTotalLines - maxlines);
			d3 = (275.0f * d1) / d2;
			iPointerLoc = static_cast<int>(d3 + 0.5);
			//DrawNewDialogBox2(SPRID_NEWSCROLL, sX + szx - 15, sY + 44, 0);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + szx - 13, sY + 44 + iPointerLoc, 7);
		}
		else
		{
			iPointerLoc = 0;
		}


		int tempindex = -1;

		for (int i = 0; i < maxlines; i++)
		{
			if ((i + m_stDialogBoxInfo[76].sView) < iTotalLines)
			{
				char cTemp[512] = {};
				strcpy(cTemp, market_items[i + m_stDialogBoxInfo[76].sView].item.m_cName);
				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cTemp, 255, 255, 255);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(market_items[i + m_stDialogBoxInfo[76].sView].tokens).c_str(), 255, 255, 255);
					tempindex = i + m_stDialogBoxInfo[76].sView;
				}
				else
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cTemp, r, g, b);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(market_items[i + m_stDialogBoxInfo[76].sView].tokens).c_str(), r, g, b);
				}
			}
		}//para mira esto me olvide

		if (tempindex != -1)
		{
			DrawMarketItem(&market_items[tempindex].item, msX + 50, msY);
		}

		// Manejo de clic del botón izquierdo
		if (cLB != 0 && iTotalLines > maxlines)
		{
			if (iGetTopDialogBoxIndex() == 76)
			{
				if ((msX >= sX + szx - 13) && (msX <= sX + szx + 12) && (msY >= sY + 44) && (msY <= sY + 374))
				{
					d1 = static_cast<double>(msY - (sY + 44));
					d2 = static_cast<double>(iTotalLines - maxlines);
					d3 = (d1 * d2) / 275.0f;
					m_stDialogBoxInfo[76].sView = static_cast<int>(d3 + 0.5);
				}
			}
		}
		else
		{
			m_stDialogBoxInfo[76].bIsScrollSelected = FALSE;
		}

		// Manejo del scroll del mouse
		if (iGetTopDialogBoxIndex() == 76 && msZ != 0)
		{
			m_stDialogBoxInfo[76].sView -= msZ / 120;  // Ajustar este valor para un desplazamiento más controlado
			if (m_stDialogBoxInfo[76].sView < 0)
			{
				m_stDialogBoxInfo[76].sView = 0;
			}
			if (m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
			}
			m_DInput.m_sZ = 0;  // Reiniciar el valor del scroll
		}

		// Limitar la vista dentro de los valores válidos
		if (iTotalLines > maxlines && m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
		{
			m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
		}
		if (m_stDialogBoxInfo[76].sView < 0 || iTotalLines <= maxlines)
		{
			m_stDialogBoxInfo[76].sView = 0;
		}

		if (m_sLang == LANG_ENG)
		{
			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Back", 250, 250, 250);
			else
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Back", 255, 200, 0);
		}
		else if (m_sLang == LANG_ESP)
		{
			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Atras", 250, 250, 250);
			else
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Atras", 255, 200, 0);
		}

		break;
	}


	case 4:
		DrawMarketItem(&market_item_ptr->item, sX + 60, sY + 70, sX + 60, sY + 70);


		if (m_sLang == LANG_ENG)
		{
			if (string(m_cPlayerName) == market_name)
			{

				wsprintf(G_cTxt, "Withdraw item from market?", market_tokens);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}
			else
			{

				wsprintf(G_cTxt, "Tokens: %d", market_item_ptr->tokens);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}


			DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Yes");
			DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");
		}
		else if (m_sLang == LANG_ESP)
		{
			if (string(m_cPlayerName) == market_name)
			{

				wsprintf(G_cTxt, "¿Retirar item del mercado?", market_tokens);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}
			else
			{

				wsprintf(G_cTxt, "Tokens: %d", market_item_ptr->tokens);
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}


			DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "si");
			DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");
		}

		break;

	case 5:
	{

		int iTotalLines = static_cast<int>(market_items.size());

		int iPointerLoc;
		double d1, d2, d3;
		int maxlines = 13;

		if (iTotalLines > maxlines)
		{
			d1 = static_cast<double>(m_stDialogBoxInfo[76].sView);
			d2 = static_cast<double>(iTotalLines - maxlines);
			d3 = (275.0f * d1) / d2;
			iPointerLoc = static_cast<int>(d3 + 0.5);
			//DrawNewDialogBox2(SPRID_NEWSCROLL, sX + szx - 15, sY + 44, 0);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + szx - 13, sY + 44 + iPointerLoc, 7);
		}
		else
		{
			iPointerLoc = 0;
		}

		int tempindex = -1;

		if (m_sLang == LANG_ENG)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Tokens", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Tokens", 255, 255, 255);
		}
		else if (m_sLang == LANG_ESP)
		{
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 23, sX + 166, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Tokens", 255, 255, 255);
			PutAlignedString(sX + 154, sX + 251, sY + 55, "Tokens", 255, 255, 255);
		}

		for (int i = 0; i < 13; i++)
			if (((i + m_stDialogBoxInfo[76].sView) < market_items.size()))
			{
				char cTemp[512] = {};
				strcpy(cTemp, market_items[i + m_stDialogBoxInfo[76].sView].item.m_cName);

				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cTemp, 255, 255, 255);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(market_items[i + m_stDialogBoxInfo[76].sView].tokens).c_str(), 255, 255, 255);
					//DrawMarketItem(&market_items[i + m_stDialogBoxInfo[76].sView].item, msX + 50, msY);
					tempindex = i + m_stDialogBoxInfo[76].sView;
				}
				else
				{
					PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cTemp, r, g, b);
					PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75, (char*)std::to_string(market_items[i + m_stDialogBoxInfo[76].sView].tokens).c_str(), r, g, b);
				}
			}

		if (tempindex != -1)
		{
			DrawMarketItem(&market_items[tempindex].item, msX + 50, msY);
		}

		// Manejo de clic del botón izquierdo
		if (cLB != 0 && iTotalLines > maxlines)
		{
			if (iGetTopDialogBoxIndex() == 76)
			{
				if ((msX >= sX + szx - 13) && (msX <= sX + szx + 12) && (msY >= sY + 44) && (msY <= sY + 374))
				{
					d1 = static_cast<double>(msY - (sY + 44));
					d2 = static_cast<double>(iTotalLines - maxlines);
					d3 = (d1 * d2) / 275.0f;
					m_stDialogBoxInfo[76].sView = static_cast<int>(d3 + 0.5);
				}
			}
		}
		else
		{
			m_stDialogBoxInfo[76].bIsScrollSelected = FALSE;
		}

		// Manejo del scroll del mouse
		if (iGetTopDialogBoxIndex() == 76 && msZ != 0)
		{
			m_stDialogBoxInfo[76].sView -= msZ / 120;  // Ajustar este valor para un desplazamiento más controlado
			if (m_stDialogBoxInfo[76].sView < 0)
			{
				m_stDialogBoxInfo[76].sView = 0;
			}
			if (m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
			}
			m_DInput.m_sZ = 0;  // Reiniciar el valor del scroll
		}

		// Limitar la vista dentro de los valores válidos
		if (iTotalLines > maxlines && m_stDialogBoxInfo[76].sView > iTotalLines - maxlines)
		{
			m_stDialogBoxInfo[76].sView = iTotalLines - maxlines;
		}
		if (m_stDialogBoxInfo[76].sView < 0 || iTotalLines <= maxlines)
		{
			m_stDialogBoxInfo[76].sView = 0;
		}

		if (m_sLang == LANG_ENG)
		{
			if (iTotalLines == 0)
			{
				PutAlignedString(sX + 19, sX + 237, sY + 165, "No have items in Market Place", 255, 255, 255);
			}


			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Back", 250, 250, 250);
			else PutString_SprFont2(sX + 195, sY + 290 + addy, "Back", 255, 200, 0);
		}
		else if (m_sLang == LANG_ESP)
		{
			if (iTotalLines == 0)
			{
				PutAlignedString(sX + 19, sX + 237, sY + 165, "No hay items en el mercado.", 255, 255, 255);
			}


			if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
				PutString_SprFont2(sX + 195, sY + 290 + addy, "Atras", 250, 250, 250);
			else PutString_SprFont2(sX + 195, sY + 290 + addy, "Atras", 255, 200, 0);
		}

		break;
	}

	case 6:

		DrawMarketItem(&market_item_ptr->item, sX + 60, sY + 70, sX + 60, sY + 70);

		if (m_sLang == LANG_ENG)
		{
			if (string(m_cPlayerName) == market_name)
			{

				wsprintf(G_cTxt, "Delete you item from market?");
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}


			DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Yes");
			DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");
		}
		else if (m_sLang == LANG_ESP)
		{
			if (string(m_cPlayerName) == market_name)
			{

				wsprintf(G_cTxt, "¿Deseas eliminar este item del mercado?");
				PutAlignedString(sX, sX + szx, sY + 200, G_cTxt, 255, 255, 255);

			}


			DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Si");
			DrawMenu_SmallButton(sX, sY, msX, msY, 150 + 15, 296, "No");
		}



		break;

	}

	DrawDialogCord(sX, sY, msX, msY);
}


// click market dialog
void CGame::DlgBoxClick_Market(short msX, short msY)
{
	auto sX = m_stDialogBoxInfo[76].sX;
	auto sY = m_stDialogBoxInfo[76].sY;
	auto szx = m_stDialogBoxInfo[76].sSizeX;

	int addy = 35;

	if (m_stDialogBoxInfo[76].cMode == 0 && (msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
	{
		PlaySound('E', 14, 5);
		DisableDialogBox(76);
		return;
	}
	else if (m_stDialogBoxInfo[76].cMode == 1 || m_stDialogBoxInfo[76].cMode == 2 || m_stDialogBoxInfo[76].cMode == 3 || m_stDialogBoxInfo[76].cMode == 5)
	{
		if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + addy) && (msY < sY + 310 + addy))
		{
			if (m_stDialogBoxInfo[76].cMode == 1) m_stDialogBoxInfo[76].cMode = 0;
			else if (m_stDialogBoxInfo[76].cMode == 2) m_stDialogBoxInfo[76].cMode = 0;
			else if (m_stDialogBoxInfo[76].cMode == 3) m_stDialogBoxInfo[76].cMode = 2;
			else if (m_stDialogBoxInfo[76].cMode == 5) m_stDialogBoxInfo[76].cMode = 0;

			PlaySound('E', 14, 5);
			return;
		}
	}

	auto btneng = (msX > sX + 196) && (msX < sX + 219) && (msY > sY + 13) && (msY < sY + 29);
	auto btnesp = (msX > sX + 225) && (msX < sX + 246) && (msY > sY + 13) && (msY < sY + 29);

	if (btneng)
	{
		m_sLang = LANG_ENG;
		PlaySound('E', 14, 5);
		return;
	}

	if (btnesp)
	{
		m_sLang = LANG_ESP;
		PlaySound('E', 14, 5);
		return;
	}

	switch (m_stDialogBoxInfo[76].cMode)
	{
	case 0:

		if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 70) && (msY < sY + 95))
		{
			market_tokens = 0;
			market_item = nullptr;
			m_stDialogBoxInfo[76].cMode = 1;
			PlaySound('E', 14, 5);
		}

		if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 95) && (msY < sY + 120))
		{
			bSendCommand(REQUEST_MARKETNAMES, NULL, NULL, NULL, NULL, NULL, NULL);
			market_names.clear();
			m_stDialogBoxInfo[76].cMode = 2;
			PlaySound('E', 14, 5);
		}

		if ((msX > sX + 35) && (msX < sX + 220) && (msY > sY + 120) && (msY < sY + 145))
		{
			market_name = m_cPlayerName;
			market_items.clear();
			bSendCommand(REQUEST_MARKETITEMS, NULL, NULL, NULL, NULL, NULL, NULL);

			m_stDialogBoxInfo[76].cMode = 5;
			PlaySound('E', 14, 5);
		}

		break;

	case 1:

		if (!market_item)
		{

		}

		if (market_tokens == 0)
			break;


		if ((msX >= sX + 25) && (msX <= sX + 25 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			bSendCommand(REQUEST_SETMARKETITEM, NULL, NULL, NULL, NULL, NULL, NULL);
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[76].cMode = 0;
			/*DisableDialogBox(76);*/

		}


		if ((msX >= sX + 150 + 15) && (msX <= sX + 150 + DEF_BTNSZX + 15) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			//DisableDialogBox(76);
			m_bIsItemDisabled[market_item_index] = FALSE;
			m_stDialogBoxInfo[76].cMode = 0;
			PlaySound('E', 14, 5);
		}
		break;

	case 2:
		for (int i = 0; i < 13; i++)
			if (((i + m_stDialogBoxInfo[76].sView) < market_names.size()))
			{
				char cTemp[512] = {};
				strcpy(cTemp, market_names[i + m_stDialogBoxInfo[76].sView].c_str());
				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					market_name = cTemp;
					market_items.clear();
					m_stDialogBoxInfo[76].cMode = 3;
					bSendCommand(REQUEST_MARKETITEMS, NULL, NULL, NULL, NULL, NULL, NULL);
					PlaySound('E', 14, 5);
				}
			}

		break;

	case 3:
		for (int i = 0; i < 13; i++)
			if (((i + m_stDialogBoxInfo[76].sView) < market_items.size()))
			{
				auto ptr = &market_items[i + m_stDialogBoxInfo[76].sView];
				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					m_stDialogBoxInfo[76].cMode = 4;
					market_item_ptr = ptr;

					PlaySound('E', 14, 5);
				}
			}


		break;

	case 4:
		if ((msX >= sX + 25) && (msX <= sX + 25 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			bSendCommand(REQUEST_GETMARKETITEM, NULL, NULL, NULL, NULL, NULL, NULL);
			PlaySound('E', 14, 5);
			//DisableDialogBox(76);
			m_stDialogBoxInfo[76].cMode = 0;
		}


		if ((msX >= sX + 150 + 15) && (msX <= sX + 150 + DEF_BTNSZX + 15) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			m_stDialogBoxInfo[76].cMode = 0;
			//DisableDialogBox(76);
			PlaySound('E', 14, 5);
		}
		break;

	case 5:
		for (int i = 0; i < 13; i++)
			if (((i + m_stDialogBoxInfo[76].sView) < market_items.size()))
			{
				auto ptr = &market_items[i + m_stDialogBoxInfo[76].sView];
				if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
				{
					m_stDialogBoxInfo[76].cMode = 6;
					market_item_ptr = ptr;
					PlaySound('E', 14, 5);
				}
			}


		break;

	case 6:
		if ((msX >= sX + 25) && (msX <= sX + 25 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			bSendCommand(REQUEST_GETMARKETITEM, NULL, NULL, NULL, NULL, NULL, NULL);
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[76].cMode = 5;
			market_items.clear();
			bSendCommand(REQUEST_MARKETITEMS, NULL, NULL, NULL, NULL, NULL, NULL);
		}


		if ((msX >= sX + 150 + 15) && (msX <= sX + 150 + DEF_BTNSZX + 15) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
		{
			m_stDialogBoxInfo[76].cMode = 5;
			PlaySound('E', 14, 5);
			market_items.clear();
			bSendCommand(REQUEST_MARKETITEMS, NULL, NULL, NULL, NULL, NULL, NULL);
		}
		break;
	}
}

void CGame::draw_questlist(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 77;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	int tmpindex = -1;
	//DrawNewDialogBox2(SPRID_QUESTLIST, sX, sY, 0);

	DrawShadowDialog2(sX, sY, 77);

	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Quest List", 0, 0, 0);
	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Quest List", 250, 250, 250);
	
	
	switch (mode)
	{
	case 0:
	{
		PutString2(sX + 23, sY + 33, "Quest list", 228, 161, 6);
		PutString2(sX + 23, sY + 55, "Mob", 228, 161, 6);
		PutString2(sX + 145, sY + 55, "Kills", 228, 161, 6);
		PutString2(sX + 220, sY + 55, "Map", 228, 161, 6);

		int iTotalLines, iPointerLoc;
		double d1, d2, d3;

		iTotalLines = 0;
		for (int i = 0; i < vec_questlist.size(); i++)
		{
			iTotalLines++;
		}

		if (iTotalLines > maxlines)
		{			
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);			
		}
		if (iGetTopDialogBoxIndex() == dnumber && msZ != 0)
		{
			m_stDialogBoxInfo[dnumber].sView = m_stDialogBoxInfo[dnumber].sView - msZ / 60;
			m_DInput.m_sZ = 0;
		}
		if (m_stDialogBoxInfo[dnumber].sView < 0) m_stDialogBoxInfo[dnumber].sView = 0;
		if (iTotalLines > maxlines && m_stDialogBoxInfo[dnumber].sView > iTotalLines - maxlines) m_stDialogBoxInfo[dnumber].sView = iTotalLines - maxlines;

		if (iTotalLines > maxlines) {
			d1 = (double)m_stDialogBoxInfo[dnumber].sView;
			d2 = (double)(iTotalLines - maxlines);
			d3 = (274.0f * d1) / d2;
			iPointerLoc = (int)(d3 + 0.5);			
			//DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);			
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242 + 100, sY + 35 + iPointerLoc, 7);
		}
		else iPointerLoc = 0;

		for (int i = 0; i < maxlines; i++)
			if (i + m_stDialogBoxInfo[dnumber].sView < iTotalLines) {

				auto & quest = vec_questlist[i + m_stDialogBoxInfo[dnumber].sView];
				int y = sY + 75 + (15 * i);

				if ((msX > sX + 20) && (msX < sX + 327) && (msY > y) && (msY < y + 15))
				{
					PutString2(sX + 23, y, quest.m_cNpcName, 228, 161, 6);
					PutString2(sX + 145, y, (char*)to_string(quest.m_iMaxKills).c_str(), 228, 161, 6);
					PutString2(sX + 220, y, quest.m_cRealMapName, 228, 161, 6);
					tmpindex = i + m_stDialogBoxInfo[dnumber].sView;

				}
				else
				{
					PutString2(sX + 23, y, quest.m_cNpcName, 255, 255, 255);
					PutString2(sX + 145, y, (char*)to_string(quest.m_iMaxKills).c_str(), 255, 255, 255);
					PutString2(sX + 220, y, quest.m_cRealMapName, 255, 255, 255);
				}
			}

		if (tmpindex != -1)
		{
			auto & quest = vec_questlist[tmpindex];
			int iAdd = 0;
			bool show1 = false;
			bool show2 = false;
			bool show3 = false;

			if (string(quest.m_cQuestReward1) != "None")
			{
				show1 = true;
				iAdd = iAdd + 20;
			}

			if (string(quest.m_cQuestReward2) != "None")
			{
				show2 = true;
				iAdd = iAdd + 20;
			}

			if (string(quest.m_cQuestReward3) != "None")
			{
				show3 = true;
				iAdd = iAdd + 20;
			}

			int startY = msY - 21;

			DrawShadowMenu(msX, startY, msX + 200, startY + iAdd);
			DrawMenu_Box(msX, startY, 200, iAdd);

			if (show1)
			{
				PutAlignedString(msX, msX + 200, startY + 2, quest.m_cQuestReward1, 240, 240, 240);
			}

			if (show2)
			{
				PutAlignedString(msX, msX + 200, startY + 2 + 20, quest.m_cQuestReward2, 240, 240, 240);
			}

			if (show3)
			{
				PutAlignedString(msX, msX + 200, startY + 2 + 40, quest.m_cQuestReward3, 240, 240, 240);
			}
		}

		if (cLB != 0 && iTotalLines > maxlines) {
			if ((iGetTopDialogBoxIndex() == dnumber)) {
				if ((msX >= sX + 334) && (msX <= sX + 346) && (msY >= sY + 40) && (msY <= sY + 320)) {
					d1 = (double)(msY - (sY + 35));
					d2 = (double)(iTotalLines - maxlines);
					d3 = (d1 * d2) / 274.0f;
					iPointerLoc = (int)d3;
					if (iPointerLoc > iTotalLines - maxlines) iPointerLoc = iTotalLines - maxlines;
					m_stDialogBoxInfo[dnumber].sView = iPointerLoc;
				}
			}
		}
		else m_stDialogBoxInfo[dnumber].bIsScrollSelected = FALSE;

		break;
	}
	case 1:
	{
		//HeatoN PRO CODE no dummy (kazin)
		wsprintf(G_cTxt, "Rewards");
		PutAlignedString(sX, sX + sizeX, sY + 50, G_cTxt, 228, 161, 6);

		string rews;
		rews.append(vec_questlist[tmpquest].m_cQuestReward1);
		rews.append(" - ");
		rews.append(vec_questlist[tmpquest].m_cQuestReward2);
		rews.append(" - ");
		rews.append(vec_questlist[tmpquest].m_cQuestReward3);

		wsprintf(G_cTxt, "%s", (char*)rews.c_str());
		PutAlignedString(sX, sX + sizeX, sY + 75, G_cTxt, 240, 240, 240);

		PutAlignedString(sX, sX + sizeX, sY + 100, "(At the end of the Quest, you can choose only one reward)", 240, 240, 240);

		/*if (string(vec_questlist[tmpquest].m_cNpcName) == "EnemyPlayers")
		wsprintf(G_cTxt, "Quest: %s", vec_questlist[tmpquest].m_cNpcName);
		else*/ wsprintf(G_cTxt, "Mob: %s", vec_questlist[tmpquest].m_cNpcName);

		PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 240, 240, 240);

		wsprintf(G_cTxt, "Map: %s", vec_questlist[tmpquest].m_cRealMapName);
		PutAlignedString(sX, sX + sizeX, sY + 175, G_cTxt, 240, 240, 240);

		wsprintf(G_cTxt, "Kills: %d", vec_questlist[tmpquest].m_iMaxKills);
		PutAlignedString(sX, sX + sizeX, sY + 200, G_cTxt, 240, 240, 240);

		DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Accept");
		DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "Cancel");
		break;
	}

	default:
		break;
	}

	DrawDialogCord(sX, sY, msX, msY);
}


void CGame::click_questlist(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 77;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	switch (mode)
	{
	case 0:
	{
		int cnt = 0;
		for (int i = 0; i < vec_questlist.size(); i++)
		{
			cnt++;
		}

		for (int i = 0; i < maxlines; i++)
		{
			if (i + m_stDialogBoxInfo[dnumber].sView < cnt)
			{
				auto & quest = vec_questlist[i + m_stDialogBoxInfo[dnumber].sView];
				int y = sY + 75 + (15 * i);
				if ((msX > sX + 20) && (msX < sX + 327) && (msY > y) && (msY < y + 15))
				{
					tmpquest = i + m_stDialogBoxInfo[dnumber].sView;
					m_stDialogBoxInfo[dnumber].cMode = 1;
					PlaySound('E', 14, 5);
				}
			}
		}
		break;
	}

	case 1:
	{
		auto quest = vec_questlist[tmpquest];

		auto cancel = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 290) && (msY <= sY + 311);
		auto accept = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 290) && (msY <= sY + 311);

		if (cancel)
		{
			m_stDialogBoxInfo[dnumber].cMode = 0;
			PlaySound('E', 14, 5);
		}

		if (accept)
		{
			char data[256];
			auto * cp = data;

			Push(cp, (u32)START_QUEST);
			Push(cp, (u16)0);

			Push(cp, quest.m_sQuestID);
			m_pGSock->iSendMsg(data, cp - data);

			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[dnumber].cMode = 0;
		}
		break;
	}
	default:break;
	}
}


void CGame::draw_charquests(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 78;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	DrawShadowDialog2(sX, sY, 78);

	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Character Quest's", 0, 0, 0);
	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Character Quest's", 250, 250, 250);
	int tmpindex = -1;
	switch (mode)
	{
	case 0:
	{
		PutString2(sX + 23, sY + 55, "Mob", 228, 161, 6);
		PutString2(sX + 145, sY + 55, "Kills", 228, 161, 6);
		PutString2(sX + 220, sY + 55, "Map", 228, 161, 6);

		int iTotalLines, iPointerLoc;
		double d1, d2, d3;

		iTotalLines = 0;
		for (int i = 0; i < vec_charquest.size(); i++)
		{
			iTotalLines++;
		}

		if (iTotalLines > maxlines){
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);
		}
		if (iGetTopDialogBoxIndex() == dnumber && msZ != 0)
		{
			m_stDialogBoxInfo[dnumber].sView = m_stDialogBoxInfo[dnumber].sView - msZ / 60;
			m_DInput.m_sZ = 0;
		}
		if (m_stDialogBoxInfo[dnumber].sView < 0) m_stDialogBoxInfo[dnumber].sView = 0;
		if (iTotalLines > maxlines && m_stDialogBoxInfo[dnumber].sView > iTotalLines - maxlines) m_stDialogBoxInfo[dnumber].sView = iTotalLines - maxlines;

		if (iTotalLines > maxlines) {
			d1 = (double)m_stDialogBoxInfo[dnumber].sView;
			d2 = (double)(iTotalLines - maxlines);
			d3 = (274.0f * d1) / d2;
			iPointerLoc = (int)(d3 + 0.5);
			//DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);			
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 242 + 100, sY + 35 + iPointerLoc, 7);
		}
		else iPointerLoc = 0;

		for (int i = 0; i < maxlines; i++)
			if (i + m_stDialogBoxInfo[dnumber].sView < iTotalLines) {

				auto & quest = vec_charquest[i + m_stDialogBoxInfo[dnumber].sView];
				int y = sY + 80 + (15 * i);

				if ((msX > sX + 20) && (msX < sX + 327) && (msY > y) && (msY < y + 15))
				{
					PutString2(sX + 23, y, quest.m_cNpcName, 228, 161, 6);

					string skills;
					skills.append(to_string(quest.m_iKills));
					skills.append("/");
					skills.append(to_string(quest.m_iMaxKills));
					PutString2(sX + 145, y, (char*)skills.c_str(), 228, 161, 6);

					PutString2(sX + 220, y, quest.m_cRealMapName, 228, 161, 6);


					tmpindex = i + m_stDialogBoxInfo[dnumber].sView;
				}
				else
				{
					PutString2(sX + 23, y, quest.m_cNpcName, 255, 255, 255);

					string skills;
					skills.append(to_string(quest.m_iKills));
					skills.append("/");
					skills.append(to_string(quest.m_iMaxKills));
					PutString2(sX + 145, y, (char*)skills.c_str(), 255, 255, 255);

					PutString2(sX + 220, y, quest.m_cRealMapName, 255, 255, 255);
				}
			}

		if (tmpindex != -1)
		{
			auto & quest = vec_charquest[tmpindex];
			int iAdd = 0;
			bool show1 = false;
			bool show2 = false;
			bool show3 = false;

			if (string(quest.m_cQuestReward1) != "None")
			{
				show1 = true;
				iAdd = iAdd + 20;
			}

			if (string(quest.m_cQuestReward2) != "None")
			{
				show2 = true;
				iAdd = iAdd + 20;
			}

			if (string(quest.m_cQuestReward3) != "None")
			{
				show3 = true;
				iAdd = iAdd + 20;
			}

			int startY = msY - 21;

			DrawShadowMenu(msX, startY, msX + 200, startY + iAdd);
			DrawMenu_Box(msX, startY, 200, iAdd);

			if (show1)
			{
				PutAlignedString(msX, msX + 200, startY + 2, quest.m_cQuestReward1, 240, 240, 240);
			}

			if (show2)
			{
				PutAlignedString(msX, msX + 200, startY + 2 + 20, quest.m_cQuestReward2, 240, 240, 240);
			}

			if (show3)
			{
				PutAlignedString(msX, msX + 200, startY + 2 + 40, quest.m_cQuestReward3, 240, 240, 240);
			}
		}

		if (cLB != 0 && iTotalLines > maxlines) {
			if ((iGetTopDialogBoxIndex() == dnumber)) {
				if ((msX >= sX + 334) && (msX <= sX + 346) && (msY >= sY + 40) && (msY <= sY + 320)) {
					d1 = (double)(msY - (sY + 35));
					d2 = (double)(iTotalLines - maxlines);
					d3 = (d1 * d2) / 274.0f;
					iPointerLoc = (int)d3;
					if (iPointerLoc > iTotalLines - maxlines) iPointerLoc = iTotalLines - maxlines;
					m_stDialogBoxInfo[dnumber].sView = iPointerLoc;
				}
			}
		}
		else m_stDialogBoxInfo[dnumber].bIsScrollSelected = FALSE;

		break;
	}
	case 1:
	{
		//HeatoN PRO CODE no dummy (kazin)
		wsprintf(G_cTxt, "Rewards");
		PutAlignedString(sX, sX + sizeX, sY + 50, G_cTxt, 228, 161, 6);

		string rews;
		rews.append(vec_charquest[tmpquest].m_cQuestReward1);
		rews.append(" - ");
		rews.append(vec_charquest[tmpquest].m_cQuestReward2);
		rews.append(" - ");
		rews.append(vec_charquest[tmpquest].m_cQuestReward3);

		wsprintf(G_cTxt, "%s", (char*)rews.c_str());
		PutAlignedString(sX, sX + sizeX, sY + 75, G_cTxt, 240, 240, 240);

		PutAlignedString(sX, sX + sizeX, sY + 100, "(At the end of the Quest, you can choose only one reward)", 240, 240, 240);

		wsprintf(G_cTxt, "Mob: %s", vec_charquest[tmpquest].m_cNpcName);
		PutAlignedString(sX, sX + sizeX, sY + 125, G_cTxt, 240, 240, 240);

		wsprintf(G_cTxt, "Map: %s", vec_charquest[tmpquest].m_cRealMapName);
		PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 240, 240, 240);

		wsprintf(G_cTxt, "Kills: %d", vec_charquest[tmpquest].m_iMaxKills);
		PutAlignedString(sX, sX + sizeX, sY + 175, G_cTxt, 240, 240, 240);

		string skills = "Current Kills: ";
		skills.append(to_string(vec_charquest[tmpquest].m_iKills));
		skills.append("/");
		skills.append(to_string(vec_charquest[tmpquest].m_iMaxKills));

		PutAlignedString(sX, sX + sizeX, sY + 200, (char*)skills.c_str(), 255, 255, 255);

		string s_status = "Status: ";
		if (vec_charquest[tmpquest].m_sCompleted == 1)
		{
			s_status.append("Completed");
			PutAlignedString(sX, sX + sizeX, sY + 225, (char*)s_status.c_str(), 255, 255, 255);
		}
		else if (vec_charquest[tmpquest].m_sStatus == 1){
			s_status.append("Active");
			PutAlignedString(sX, sX + sizeX, sY + 225, (char*)s_status.c_str(), 255, 255, 255);
		}
		else {
			s_status.append("Paused");
			PutAlignedString(sX, sX + sizeX, sY + 225, (char*)s_status.c_str(), 255, 255, 255);
		}

		if (vec_charquest[tmpquest].m_sCompleted == 0)
			DrawMenu_SmallButton(sX, sY, msX, msY, 246, 33, "Dismiss");

		if (vec_charquest[tmpquest].m_sCompleted == 1)
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Get reward");
		else if (vec_charquest[tmpquest].m_sStatus == 1)
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Disable");
		else
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Enable");

		DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "Back");
		break;
	}

	case 2:
	{
		PutAlignedString(sX, sX + sizeX, sY + 35, "Dismiss quest", 228, 161, 6);

		wsprintf(G_cTxt, "Mob: %s", vec_charquest[tmpquest].m_cNpcName);
		PutAlignedString(sX, sX + sizeX, sY + 50 + 15, G_cTxt, 255, 255, 255);

		wsprintf(G_cTxt, "Map: %s", vec_charquest[tmpquest].m_cRealMapName);
		PutAlignedString(sX, sX + sizeX, sY + 75 + 15, G_cTxt, 255, 255, 255);

		string skills = "Current Kills: ";
		skills.append(to_string(vec_charquest[tmpquest].m_iKills));
		skills.append("/");
		skills.append(to_string(vec_charquest[tmpquest].m_iMaxKills));

		PutAlignedString(sX, sX + sizeX, sY + 100 + 15, (char*)skills.c_str(), 255, 255, 255);

		wsprintf(G_cTxt, "Rewards");
		PutAlignedString(sX, sX + sizeX, sY + 125 + 15, G_cTxt, 228, 161, 6);

		string rews;
		rews.append(vec_charquest[tmpquest].m_cQuestReward1);
		rews.append(" - ");
		rews.append(vec_charquest[tmpquest].m_cQuestReward2);
		rews.append(" - ");
		rews.append(vec_charquest[tmpquest].m_cQuestReward3);

		wsprintf(G_cTxt, "%s", (char*)rews.c_str());
		PutAlignedString(sX, sX + sizeX, sY + 150 + 15, G_cTxt, 240, 240, 240);

		PutAlignedString(sX, sX + sizeX, sY + 135 + 75, "Are you sure to cancel the quest?", 255, 255, 255);
		PutAlignedString(sX, sX + sizeX, sY + 135 + 90, "All data from this quest will be lost.", 255, 255, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Yes");
		DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "No");
		break;
	}

	case 3:
	{
		PutAlignedString(sX, sX + sizeX, sY + 35, "Select one Reward", 228, 161, 6);

		if (string(vec_charquest[tmpquest].m_cQuestReward1) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 80) && (msY <= sY + 80 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 80, vec_charquest[tmpquest].m_cQuestReward1, 228, 161, 6);
			else
				PutAlignedString(sX, sX + sizeX, sY + 80, vec_charquest[tmpquest].m_cQuestReward1, 255, 255, 255);
		}

		if (string(vec_charquest[tmpquest].m_cQuestReward2) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 100) && (msY <= sY + 100 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 100, vec_charquest[tmpquest].m_cQuestReward2, 228, 161, 6);
			else PutAlignedString(sX, sX + sizeX, sY + 100, vec_charquest[tmpquest].m_cQuestReward2, 255, 255, 255);

		}

		if (string(vec_charquest[tmpquest].m_cQuestReward3) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 120) && (msY <= sY + 120 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 120, vec_charquest[tmpquest].m_cQuestReward3, 228, 161, 6);
			else PutAlignedString(sX, sX + sizeX, sY + 120, vec_charquest[tmpquest].m_cQuestReward3, 255, 255, 255);
		}

		switch (selectedoption)
		{
		case 1:
			wsprintf(G_cTxt, "Selected: %s", vec_charquest[tmpquest].m_cQuestReward1);
			PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 228, 161, 6);
			break;
		case 2:
			wsprintf(G_cTxt, "Selected: %s", vec_charquest[tmpquest].m_cQuestReward2);
			PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 228, 161, 6);
			break;
		case 3:
			wsprintf(G_cTxt, "Selected: %s", vec_charquest[tmpquest].m_cQuestReward3);
			PutAlignedString(sX, sX + sizeX, sY + 150, G_cTxt, 228, 161, 6);
			break;
		default:

			break;
		}



		DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Obtain");
		DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "Back");
		break;
	}

	default:
		break;
	}

	DrawDialogCord(sX, sY, msX, msY);
}


void CGame::click_charquests(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 78;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	switch (mode)
	{
	case 0:
	{
		int cnt = 0;
		for (int i = 0; i < vec_charquest.size(); i++)
		{
			cnt++;
		}

		for (int i = 0; i < maxlines; i++)
		{
			if (i + m_stDialogBoxInfo[dnumber].sView < cnt)
			{
				auto & quest = vec_charquest[i + m_stDialogBoxInfo[dnumber].sView];
				int y = sY + 80 + (15 * i);
				if ((msX > sX + 20) && (msX < sX + 327) && (msY > y) && (msY < y + 15))
				{
					tmpquest = i + m_stDialogBoxInfo[dnumber].sView;
					m_stDialogBoxInfo[dnumber].cMode = 1;
					PlaySound('E', 14, 5);
				}
			}
		}
		break;
	}

	case 1:
	{
		auto quest = vec_charquest[tmpquest];

		auto back = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 290) && (msY <= sY + 311);
		auto dismiss = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 33) && (msY <= sY + 33 + 21);
		auto button = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 290) && (msY <= sY + 311);

		if (button)
		{
			if (vec_charquest[tmpquest].m_sCompleted == 1)
			{
				selectedoption = 0;
				m_stDialogBoxInfo[dnumber].cMode = 3;
				PlaySound('E', 14, 5);
			}

			else if (vec_charquest[tmpquest].m_sStatus == 1)
			{
				char data[256];
				auto * cp = data;
				Push(cp, (u32)PAUSE_QUEST);
				Push(cp, (u16)0);
				Push(cp, tmpquest);
				m_pGSock->iSendMsg(data, cp - data);
				PlaySound('E', 14, 5);
			}
			else
			{
				char data[256];
				auto * cp = data;
				Push(cp, (u32)ACTIVE_QUEST);
				Push(cp, (u16)0);
				Push(cp, tmpquest);
				m_pGSock->iSendMsg(data, cp - data);
				PlaySound('E', 14, 5);
			}
		}

		if (back)
		{
			m_stDialogBoxInfo[dnumber].cMode = 0;
			PlaySound('E', 14, 5);
		}

		if (dismiss)
		{
			if (vec_charquest[tmpquest].m_sCompleted == 0){
				m_stDialogBoxInfo[dnumber].cMode = 2;
				PlaySound('E', 14, 5);
			}
		}
		break;
	}

	case 2:
	{
		auto quest = vec_charquest[tmpquest];

		auto no = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 290) && (msY <= sY + 311);
		auto yes = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 290) && (msY <= sY + 311);


		if (no)
		{
			m_stDialogBoxInfo[dnumber].cMode = 1;
			PlaySound('E', 14, 5);
		}

		if (yes)
		{
			if (vec_charquest[tmpquest].m_sCompleted == 0){
				char data[256];
				auto * cp = data;
				Push(cp, (u32)DISMISS_QUEST);
				Push(cp, (u16)0);
				Push(cp, tmpquest);
				m_pGSock->iSendMsg(data, cp - data);
				m_stDialogBoxInfo[dnumber].cMode = 0;
				PlaySound('E', 14, 5);
			}
		}
		break;
	}

	case 3:
	{

		auto quest = vec_charquest[tmpquest];

		auto no = (msX >= sX + 246) && (msX <= sX + 321) && (msY >= sY + 290) && (msY <= sY + 311);
		auto yes = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 290) && (msY <= sY + 311);



		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 80) && (msY <= sY + 80 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 80, vec_charquest[tmpquest].m_cQuestReward1, 228, 161, 6);
			else
				PutAlignedString(sX, sX + sizeX, sY + 80, vec_charquest[tmpquest].m_cQuestReward1, 255, 255, 255);
		}

		if (string(vec_charquest[tmpquest].m_cQuestReward2) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 100) && (msY <= sY + 100 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 100, vec_charquest[tmpquest].m_cQuestReward2, 228, 161, 6);
			else PutAlignedString(sX, sX + sizeX, sY + 100, vec_charquest[tmpquest].m_cQuestReward2, 255, 255, 255);

		}

		if (string(vec_charquest[tmpquest].m_cQuestReward3) != "None")
		{
			if ((msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 120) && (msY <= sY + 120 + 12))
				PutAlignedString(sX, sX + sizeX, sY + 120, vec_charquest[tmpquest].m_cQuestReward3, 228, 161, 6);
			else PutAlignedString(sX, sX + sizeX, sY + 120, vec_charquest[tmpquest].m_cQuestReward3, 255, 255, 255);
		}

		auto option1 = (msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 80) && (msY <= sY + 80 + 12);
		auto option2 = (msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 100) && (msY <= sY + 100 + 12);
		auto option3 = (msX >= sX + 110) && (msX <= sX + 230) && (msY >= sY + 120) && (msY <= sY + 120 + 12);

		if (string(vec_charquest[tmpquest].m_cQuestReward1) != "None")
		{
			if (option1)
			{
				selectedoption = 1;
				PlaySound('E', 14, 5);
			}
		}

		if (string(vec_charquest[tmpquest].m_cQuestReward2) != "None")
		{
			if (option2)
			{
				selectedoption = 2;
				PlaySound('E', 14, 5);
			}
		}


		if (string(vec_charquest[tmpquest].m_cQuestReward3) != "None")
		{
			if (option3)
			{
				selectedoption = 3;
				PlaySound('E', 14, 5);
			}
		}


		if (no)
		{
			m_stDialogBoxInfo[dnumber].cMode = 1;
			PlaySound('E', 14, 5);
		}

		if (yes)
		{
			if (vec_charquest[tmpquest].m_sCompleted == 1){

				if (selectedoption == 0)
				{
					AddEventList("Por favor no seas estupido y selecciona una opcion. Gracias.");
					PlaySound('E', 14, 5);
					return;
				}

				char data[260];
				auto * cp = data;
				Push(cp, (u32)REWARD_QUEST);
				Push(cp, (u16)0);
				Push(cp, tmpquest);
				Push(cp, selectedoption);
				m_pGSock->iSendMsg(data, cp - data);
				m_stDialogBoxInfo[dnumber].cMode = 0;
				PlaySound('E', 14, 5);
			}
		}
		break;
	}

	default:break;
	}
}
/*

void CGame::Draw_NetGraph()
{
	int fix_x, fix_y; // Coordenadas iniciales que ajustarás según la resolución

	if (c_reso->IsResolution() == C1024x768)
	{
		fix_x = 1024 - 100;
		fix_y = 768 - 30;
	}
	else if (c_reso->IsResolution() == C800x600)
	{
		fix_x = 800 - 100;
		fix_y = 600 - 30;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		fix_x = 640 - 100;
		fix_y = 480 - 30;
	}

	if (BigBar == 0)
	{
		fix_y = fix_y - 54;
	}

	m_dPing = calculatePing();
	char pingMessage[50]; // Buffer suficiente para almacenar el mensaje
	if (m_dPing > 0.0)
		sprintf(pingMessage, "Ping: %.2f ms", m_dPing);
	else
		sprintf(pingMessage, "Ping: calculating...");

	// Mostrar el mensaje en la posición calculada
	PutString2(fix_x, fix_y, pingMessage, 55, 255, 255);
}
*/
void CGame::Draw_NetGraph()
{
	const int CHAR_WIDTH = 5; // Ajusta según el tamaño real de tu fuente

	int fix_x, fix_y; // Coordenadas iniciales que ajustarás según la resolución

	if (c_reso->IsResolution() == C1024x768)
	{
		fix_x = 1024 - 100;
		fix_y = 768 - 55;
	}
	else if (c_reso->IsResolution() == C800x600)
	{
		fix_x = 800 - 100;
		fix_y = 600 - 55;
	}
	else if (c_reso->IsResolution() == C640x480)
	{
		fix_x = 640 - 100;
		fix_y = 480 - 55;
	}

	if (BigBar == 0)
	{
		fix_y = fix_y - 54;
	}

	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);


	//time
	char timeLabel[10] = "Time:";
	char timeValue[50];
	wsprintf(timeValue, "  %d", SysTime.wSecond);
	PutString2(fix_x, fix_y - 30, timeLabel, 255, 200, 0);
	int timelabelWidth = strlen(timeLabel) * CHAR_WIDTH;
	PutString2(fix_x + timelabelWidth, fix_y - 30, timeValue, 255, 255, 255);

	//fps
	char fpsLabel[10] = "Fps:";
	char fpsValue[50];         
	wsprintf(fpsValue, " %d", m_sFPS + 713);
	PutString2(fix_x, fix_y - 15, fpsLabel, 255, 200, 0);
	int fpslabelWidth = strlen(fpsLabel) * CHAR_WIDTH;
	PutString2(fix_x + fpslabelWidth, fix_y - 15, fpsValue, 255, 255, 255);

	//ping
	//m_dPing = calculatePing();
	char pingLabel[10] = "Ping:";
	char pingValue[50];

	/*if (m_dPing > 130.0) {
		m_dPing -= 60.0;  // Descontamos 60 si el ping es mayor a 130
	}*/

	if (m_dPing > 0.0)
		sprintf(pingValue, " %.2f ms", m_dPing);
	else
		sprintf(pingValue, " request...");

	PutString2(fix_x, fix_y, pingLabel, 255, 200, 0);
	int labelWidth = strlen(pingLabel) * CHAR_WIDTH;
	PutString2(fix_x + labelWidth, fix_y, pingValue, 255, 255, 255);
}

void CGame::DrawQuest()
{
	if (isInMap("dkcity") || isInMap("dkshop1") || isInMap("dkshop2")) return;
	int fix_x = 0;
	const int offset_x = 15;

	if (c_reso->IsResolution() == C640x480)
		fix_x = 540 - offset_x;
	else if (c_reso->IsResolution() == C800x600)
		fix_x = 700 - offset_x;
	else if (c_reso->IsResolution() == C1024x768)
		fix_x = 924 - offset_x;

	st_charquest* mainQuest = nullptr;
	CharacterEk_Quests* ekQuest = nullptr;

	for (auto& quest : vec_charquest)
	{
		if (quest.m_sStatus == 1)
		{
			mainQuest = &quest;
			break;
		}
	}

	for (auto& quest : vec_charekquest)
	{
		if (quest.status)
		{
			ekQuest = &quest;
			break;
		}
	}
	int baseY = 0;
	if (isInMap("dk"))
	{
		baseY = 50;
	}
	else
	{
		baseY = 200 - 28 + 70;
	}


	if (mainQuest)
	{
		PutString2(fix_x - 10, baseY, "Mob Quest:", 255, 200, 0);
		wsprintf(G_cTxt, "%s: %d/%d", mainQuest->m_cNpcName, mainQuest->m_iKills, mainQuest->m_iMaxKills);
		PutString2(fix_x - 10, baseY + 15, G_cTxt, 255, 255, 255);
		baseY += 35; // solo se suma si hubo Mob Quest
	}
	else
	{
		baseY += 10; // dejar un pequeño margen si no hay Mob Quest
	}

	if (ekQuest)
	{
		PutString2(fix_x - 10, baseY, "EK Quest:", 255, 200, 0);
		wsprintf(G_cTxt, "%s: %d/%d", ekQuest->m_cShowMapName, ekQuest->m_iKills, ekQuest->m_iMaxKills);
		PutString2(fix_x - 10, baseY + 15, G_cTxt, 255, 255, 255);
		wsprintf(G_cTxt, "%s", ekQuest->m_cRewardName);
		PutString2(fix_x - 10, baseY + 30, G_cTxt, 255, 255, 255);
	}
}

void CGame::DrawMenu_Box(short sX, short sY, short x, short y)
{
	DrawDialogShadowPixels(1, x, sX, sY);
	DrawDialogShadowPixels(1, x, sX, sY + y);
	DrawDialogShadowPixels(2, y, sX, sY);
	DrawDialogShadowPixels(2, y + 1, sX + x, sY);
}

void CGame::get_questlist(char * data)
{
	vec_questlist.clear();
	short size = 0;

	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_quest u;
		Pop(data, u.m_sQuestID);
		ZeroMemory(u.m_cNpcName, sizeof(u.m_cNpcName));
		Pop(data, u.m_cNpcName, 21);
		ZeroMemory(u.m_cMapName, sizeof(u.m_cMapName));
		Pop(data, u.m_cMapName, 12);
		ZeroMemory(u.m_cRealMapName, sizeof(u.m_cRealMapName));
		Pop(data, u.m_cRealMapName, 12);
		Pop(data, u.m_iMaxKills);
		Pop(data, u.m_iRewardContribution);


		ZeroMemory(u.m_cQuestReward1, sizeof(u.m_cQuestReward1));
		Pop(data, u.m_cQuestReward1, 21);
		ZeroMemory(u.m_cQuestReward2, sizeof(u.m_cQuestReward2));
		Pop(data, u.m_cQuestReward2, 21);
		ZeroMemory(u.m_cQuestReward3, sizeof(u.m_cQuestReward3));
		Pop(data, u.m_cQuestReward3, 21);


		vec_questlist.push_back(u);
	}
}

void CGame::get_magicstatus(char * data)
{
	int index;
	int status;

	Pop(data, index);
	Pop(data, status);

	m_cMagicMastery[index] = status;
}

void CGame::get_dropitems(char * data)
{
	m_vDropItemList.clear();

	short size = 0;

	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_dropitems u;
			
		ZeroMemory(u.m_cItemName, sizeof(u.m_cItemName));
		Pop(data, u.m_cItemName, 21);
		Pop(data, u.itemcount);
		Pop(data, u.attr);
		 
		m_vDropItemList.push_back(u);
	}
}

void CGame::get_charquests(char * data)
{
	vec_charquest.clear();
	short size = 0;

	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_charquest u;

		Pop(data, u.m_sParentQuestID);
		ZeroMemory(u.m_cNpcName, sizeof(u.m_cNpcName));
		Pop(data, u.m_cNpcName, 21);
		ZeroMemory(u.m_cMapName, sizeof(u.m_cMapName));
		Pop(data, u.m_cMapName, 12);
		ZeroMemory(u.m_cRealMapName, sizeof(u.m_cRealMapName));
		Pop(data, u.m_cRealMapName, 12);
		Pop(data, u.m_iKills);
		Pop(data, u.m_iMaxKills);
		Pop(data, u.m_iRewardContribution);

		ZeroMemory(u.m_cQuestReward1, sizeof(u.m_cQuestReward1));
		Pop(data, u.m_cQuestReward1, 21);
		ZeroMemory(u.m_cQuestReward2, sizeof(u.m_cQuestReward2));
		Pop(data, u.m_cQuestReward2, 21);
		ZeroMemory(u.m_cQuestReward3, sizeof(u.m_cQuestReward3));
		Pop(data, u.m_cQuestReward3, 21);

		Pop(data, u.m_sStatus);
		Pop(data, u.m_sCompleted);

		vec_charquest.push_back(u);
	}
}

void CGame::get_charquests_byindex(char * data)
{
	int index = -1;

	Pop(data, index);

	for (int i = 0; i < vec_charquest.size(); i++)
	{
		if (index != i) continue;

		Pop(data, vec_charquest[i].m_iKills);
		Pop(data, vec_charquest[i].m_sStatus);
		Pop(data, vec_charquest[i].m_sCompleted);
	}
}

void CGame::getEventList(char * data)
{
	char msg[200];
	int eventmode = 0;
	ZeroMemory(msg, sizeof(msg));

	Pop(data, msg, 200);
	Pop(data, eventmode);
	
	if (eventmode == 0)	AddEventList(msg);		
	else AddEventList(msg, eventmode);

	if (string(msg) == "Has asestado un golpe critico.")
	{
		char cTxt[200];
		strcpy(cTxt, "Outstanding!");
		PlaySound('E', 61, 0);

		_RemoveChatMsgListByObjectID(m_sPlayerObjectID);
		for (int i = 1; i < DEF_MAXCHATMSGS; i++)
			if (m_pChatMsgList[i] == NULL) {
				strcpy(cTxt, cTxt);
				m_pChatMsgList[i] = new class CMsg(23, cTxt, m_dwCurTime);
				m_pChatMsgList[i]->m_iObjectID = m_sPlayerObjectID;

				if (m_pMapData->bSetChatMsgOwner(m_sPlayerObjectID, -10, -10, i) == FALSE) {
					delete m_pChatMsgList[i];
					m_pChatMsgList[i] = NULL;
				}
				break;
			}
	}
	
}

void CGame::get_charekquests_byindex(char * data)
{
	int index = -1;

	Pop(data, index);

	for (int i = 0; i < vec_charekquest.size(); i++)
	{
		if (index != i) continue;

		Pop(data, vec_charekquest[i].m_iKills);
		short sStatus;
		Pop(data, sStatus);
		vec_charekquest[i].status = sStatus != 0;
		Pop(data, vec_charekquest[i].m_sCompleted);
	}
}


static bool startsWith(const std::string& str, const std::string& prefix) {
	return str.rfind(prefix, 0) == 0; // Verifica si `prefix` es el inicio de `str`
}

// Añade esta función en algún lugar de tu código (puede ser al inicio del archivo)
const char* stristr(const char* str1, const char* str2) {
	if (!*str2) return str1;
	for (; *str1; ++str1) {
		if (tolower(*str1) == tolower(*str2)) {
			const char *h = str1, *n = str2;
			while (*h && *n && tolower(*h) == tolower(*n)) {
				++h;
				++n;
			}
			if (!*n) return str1;
		}
	}
	return nullptr;
}

void CGame::draw_trades(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 79;
	int maxlines = 18;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;	

	DrawShadowDialog2(sX, sY, 79);

	if (m_stDialogBoxInfo[dnumber].cMode == 50)
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Take Fragments", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Take Fragments", 250, 250, 250);
		
		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 60, "Light", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 60, "Light", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 80, "Endurance", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 80, "Endurance", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 100, "Hit Prob", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 100, "Hit Prob", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 120, "Defense Ratio", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 120, "Defense Ratio", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 140) && (msY <= sY + 140 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 140, "HP Recovery", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 140, "HP Recovery", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 160, "SP Recovery", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 160, "SP Recovery", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 180) && (msY <= sY + 180 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 180, "MP Recovery", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 180, "MP Recovery", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 200) && (msY <= sY + 200 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 200, "Magic Resist", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 200, "Magic Resist", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 220) && (msY <= sY + 220 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 220, "Physical Absorption", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 220, "Physical Absorption", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 240) && (msY <= sY + 240 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 240, "Magic Absorption", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 240, "Magic Absorption", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 260) && (msY <= sY + 260 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 260, "Exp", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 260, "Exp", 255, 200, 0);

		/*if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 280) && (msY <= sY + 280 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 260, "Tokens", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 260, "Tokens", 255, 200, 0);*/

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		return;
	}

	if (m_stDialogBoxInfo[dnumber].cMode == 51)
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Take Segments", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Take Segments", 250, 250, 250);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 60, "Fire", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 60, "Fire", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 80, "Lightning", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 80, "Lightning", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 100, "Ice", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 100, "Ice", 255, 200, 0);

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		return;
	}


	switch (trade_mode)
	{
	case DEF_NOTSELECTED:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Items for sale", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Items for sale", 250, 250, 250);
		//255, 200, 0

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
		PutAlignedString(sX, sX + sizeX, sY + 60, "Trade ek", 255, 255, 255); 
		else PutAlignedString(sX, sX + sizeX, sY + 60, "Trade ek", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
		PutAlignedString(sX, sX + sizeX, sY + 80, "Trade tokens", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 80, "Trade tokens", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 100, "Trade contribution", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 100, "Trade contribution", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 120, "Trade majestics", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 120, "Trade majestics", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 140) && (msY <= sY + 140 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 140, "Trade balls", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 140, "Trade balls", 255, 200, 0);

		/*if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 160, "Trade EventPoints", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 160, "Trade EventPoints", 255, 200, 0);*/

		/*if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 160, "Starter items", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 160, "Starter items", 255, 200, 0);
*/

		/*if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 180) && (msY <= sY + 180 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 180, "Trade tokens", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 180, "Trade tokens", 255, 200, 0);*/

		return;
		break;
	}
	case DEF_TRADEEK:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Ek", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Ek", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Ek Count: %d", m_iEnemyKillCount);
		PutAlignedString(sX, sX + sizeX, sY + 33, temp, 255, 255, 255);
		break;
	}
	case DEF_TRADEBALLS:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Balls", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Balls", 250, 250, 250);
		//255, 200, 0

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 60, "Contribution", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 60, "Contribution", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 80, "Tokens", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 80, "Tokens", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 100, "Reputation", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 100, "Reputation", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 120, "Majestics", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 120, "Majestics", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 140) && (msY <= sY + 140 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 140, "Fragments", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 140, "Fragments", 255, 200, 0);

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 160, "Segments", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 160, "Segments", 255, 200, 0);

/*

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 160, "Tokens", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 160, "Tokens", 255, 200, 0);
		*/

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		return;
		break;
	}
	case DEF_STARTERPACK:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Get Starter items", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Get Starter items", 250, 250, 250);
		//255, 200, 0

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 60, "War items", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 60, "War items", 255, 200, 0);


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
			PutAlignedString(sX, sX + sizeX, sY + 80, "Mage items", 255, 255, 255);
		else PutAlignedString(sX, sX + sizeX, sY + 80, "Mage items", 255, 200, 0);


		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		return;
		break;
	}
	case DEF_TRADECOIN:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Coin", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Coin", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Coin count: %d", m_iCoins);
		PutString2(sX + 20, sY + 42, temp, 255, 255, 255);
		break;
	}
	case DEF_TRADETOKENS:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Token", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Token", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Token count: %d", m_iTokens);
		PutString2(sX + 20, sY + 42, temp, 255, 255, 255);
		break;
	}

	case DEF_TRADEEVENTPOINTS:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Event Points", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Event Points", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Event Points: %d", m_iEventPoints);
		PutString2(sX + 20, sY + 42, temp, 255, 255, 255);
		break;
	}

	case DEF_TRADECONTRIB:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Contrib", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Contrib", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Contrib Count: %d", m_iContribution);
		PutString2(sX + 20, sY + 42, temp, 255, 255, 255);
		break;
	}
	case DEF_TRADEMAJS:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trade Majestics", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trade Majestics", 250, 250, 250);
		char temp[50];
		wsprintf(temp, "Majestics Count: %d", m_iGizonItemUpgradeLeft);
		PutString2(sX + 20, sY + 42, temp, 255, 255, 255);
		break;
	}
	default:
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Trades", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Trades", 250, 250, 250);
		break;
	}

	switch (mode)
	{
	case 0:
	{
		const int iSearchBoxX = sX + 70;
		const int iSearchBoxY = sY + 23;
		const int iSearchBoxWidth = 150;
		const int iSearchBoxHeight = 18;
		const int maxlines = 11;

		// Dibujar cuadro de búsqueda
		PutString2(sX + 20, sY + 25, "Search:", 255, 255, 255);
		DrawShadowMenu(iSearchBoxX, iSearchBoxY, iSearchBoxX + iSearchBoxWidth, iSearchBoxY + iSearchBoxHeight);
		DrawShadowMenu_DialogBordersNoTitle(iSearchBoxX, iSearchBoxY, iSearchBoxWidth, iSearchBoxHeight);
		PutString2(iSearchBoxX + 2, iSearchBoxY + 2, searchFilter, 255, 255, 255);

		// Generar índice filtrado
		std::vector<int> filtered_indices;
		if (strlen(searchFilter)) {
			for (int i = 0; i < vec_trades.size(); ++i) {
				if (stristr(vec_trades[i].itemname, searchFilter) != nullptr)
					filtered_indices.push_back(i);
			}
		}
		else {
			for (int i = 0; i < vec_trades.size(); ++i)
				filtered_indices.push_back(i);
		}

		int iTotalLines = static_cast<int>(filtered_indices.size());
		int iPointerLoc;
		double d1, d2, d3;

		// Títulos
		if (m_sLang == LANG_ENG) {
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Item Name", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Cost", 255, 255, 255);
		}
		else if (m_sLang == LANG_ESP) {
			PutAlignedString(sX + 22, sX + 165, sY + 55, "Nombre del item", 255, 255, 255);
			PutAlignedString(sX + 153, sX + 250, sY + 55, "Valor", 255, 255, 255);
		}

		if (iTotalLines > maxlines) {
			d1 = static_cast<double>(m_stDialogBoxInfo[dnumber].sView);
			d2 = static_cast<double>(iTotalLines - maxlines);
			d3 = (275.0f * d1) / d2;
			iPointerLoc = static_cast<int>(d3 + 0.5);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + sizeX - 13, sY + 44 + iPointerLoc, 7);
		}
		else {
			iPointerLoc = 0;
		}

		for (int i = 0; i < maxlines; i++) {
			int viewIndex = i + m_stDialogBoxInfo[dnumber].sView;
			if (viewIndex < iTotalLines) {
				int realIndex = filtered_indices[viewIndex];
				auto& item = vec_trades[realIndex];

				char cNameStr[120], cSubStr1[120], cSubStr2[120];
				GetItemName(item.itemname, 0, cNameStr, cSubStr1, cSubStr2, 0);

				bool hovered = (msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89);
				int r = hovered ? 255 : 255;
				int g = hovered ? 255 : 200;
				int b = hovered ? 255 : 0;

				PutAlignedString(sX + 10, sX + 190, sY + i * 18 + 75, cNameStr, r, g, b);
				PutAlignedString(sX + 148, sX + 260, sY + i * 18 + 75,
					(char*)std::to_string(item.cost).c_str(), r, g, b);
			}
		}

		// Scroll con mouse
		if (iGetTopDialogBoxIndex() == dnumber && msZ != 0) {
			m_stDialogBoxInfo[dnumber].sView -= msZ / 120;
			if (m_stDialogBoxInfo[dnumber].sView < 0)
				m_stDialogBoxInfo[dnumber].sView = 0;
			if (m_stDialogBoxInfo[dnumber].sView > iTotalLines - maxlines)
				m_stDialogBoxInfo[dnumber].sView = iTotalLines - maxlines;
			m_DInput.m_sZ = 0;
		}

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		break;
	}


	case 1:
	{	
		char cTemp[255];
		auto dwTime = timeGetTime();		
		
		auto index = tmptrade;

		string reward = vec_trades[index].itemname;
		string contrib_text = "Contrib+";
		string maj_text = "Majestic+";
		string rep_text = "Rep+";

		if (startsWith(reward, contrib_text))
		{
			m_stDialogBoxInfo[dnumber].sV3 = 1;
		}
		else if (startsWith(reward, maj_text))
		{
				m_stDialogBoxInfo[dnumber].sV3 = 1;
		}
		else if (startsWith(reward, rep_text))
		{
			m_stDialogBoxInfo[dnumber].sV3 = 1;
		}
		else
		{


			m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 156, sY + 219, 19, dwTime);
			m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 170, sY + 219, 19, dwTime);
			PutString2(sX + 80, sY + 235 - 10, DRAW_DIALOGBOX_SHOP27, 228, 161, 6); // "Quantity:"

			if (iGetTopDialogBoxIndex() == dnumber && msZ != 0)
			{
				if (msZ > 0) {
					m_stDialogBoxInfo[dnumber].sV3 += 1;
				}
				else if (msZ < 0) {
					m_stDialogBoxInfo[dnumber].sV3 -= 1;
				}

			}

			if (m_stDialogBoxInfo[dnumber].sV3 > (50 - _iGetTotalItemNum()))
				m_stDialogBoxInfo[dnumber].sV3 = (50 - _iGetTotalItemNum());

			if (m_stDialogBoxInfo[dnumber].sV3 < 1)
				m_stDialogBoxInfo[dnumber].sV3 = 1;

			if (m_stDialogBoxInfo[dnumber].sV3 >= 10)
			{
				ZeroMemory(cTemp, sizeof(cTemp));
				_itoa(m_stDialogBoxInfo[dnumber].sV3, cTemp, 10);
				cTemp[1] = NULL;
				PutString2(sX - 35 + 186, sY - 10 + 235, cTemp, 228, 161, 6);
				ZeroMemory(cTemp, sizeof(cTemp));
				_itoa(m_stDialogBoxInfo[dnumber].sV3, cTemp, 10);
				PutString2(sX - 35 + 200, sY - 10 + 235, (cTemp + 1), 228, 161, 6);
			}
			else
			{
				PutString2(sX - 35 + 186, sY - 10 + 235, "0", 228, 161, 6);
				ZeroMemory(cTemp, sizeof(cTemp));
				_itoa(m_stDialogBoxInfo[dnumber].sV3, cTemp, 10);
				PutString2(sX - 35 + 200, sY - 10 + 235, (cTemp), 228, 161, 6);
			}
			m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 156, sY + 244, 20, dwTime);
			m_pSprite[DEF_SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 170, sY + 244, 20, dwTime);
		}
		
		auto quant = m_stDialogBoxInfo[dnumber].sV3;
		auto icost = vec_trades[index].cost * quant;
		string sitem = vec_trades[index].itemname;
		char * item = (char*)sitem.c_str();
		string scost = "Value: ";

		scost.append(to_string(icost));
		
		switch (trade_mode)
		{
		case DEF_TRADEEK:
		{			
			scost.append(" (Enemy Kills)");
			break;
		}
		case DEF_TRADECOIN:
		{			
			scost.append(" (Coins)");
			
			break;
		}
		case DEF_TRADETOKENS:
		{
			scost.append(" (Tokens)");

			break;
		}

		case DEF_TRADEEVENTPOINTS:
		{
			scost.append(" (EventPoints)");

			break;
		}

		case DEF_TRADECONTRIB:
		{
			scost.append(" (Contribution)");
			break;
		}
		case DEF_TRADEMAJS:
		{
			scost.append(" (Majestics)");
			break;
		}
		default:

			break;
		}

		char * cost = (char*)scost.c_str();		

		PutAlignedString(sX + 60, sX + 200, sY + 60, item, 255, 255, 255);

		if (IsFragment(item))
		{
			PutAlignedString(sX + 5, sX + sizeX - 5, sY + 77, "When the item is a fragment it is multiplied by 150.", 228, 161, 6);
		}

		PutAlignedString(sX + 60, sX + 200, sY + 180, cost, 255, 255, 255);

		if (startsWith(reward, contrib_text))
		{
		}
		else if (startsWith(reward, maj_text))
		{
		}
		else
		{
			if (vec_trades[index].itemcolor != 0)
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + vec_trades[index].sprite]->PutSpriteRGB(sX + 130, sY + 133, vec_trades[index].spriteframe, m_wWR[vec_trades[index].itemcolor] - m_wR[0], m_wWG[vec_trades[index].itemcolor] - m_wG[0], m_wWB[vec_trades[index].itemcolor] - m_wB[0], dwTime);
			else {
				if (string(vec_trades[index].itemname) == "PandoraBox(Small)" ||
					string(vec_trades[index].itemname) == "PandoraBox(Medium)" ||
					string(vec_trades[index].itemname) == "PandoraBox(Big)")
					m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + vec_trades[index].sprite]->PutSpriteFast(sX + 130 - 30, sY + 133 - 40, vec_trades[index].spriteframe, dwTime);
				else m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + vec_trades[index].sprite]->PutSpriteFast(sX + 130, sY + 133, vec_trades[index].spriteframe, dwTime);
			}
		}
		
		PutAlignedString(sX, sX + sizeX, sY + 262, DRAW_DIALOGBOX_CITYHALL_MENU46A, 255, 255, 255);
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY, "Yes");
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "No");
		DrawDialogCord(sX, sY, msX, msY);

		break;
	}

	default:
		break;
	}
}

bool CGame::IsFragment(char* itname)
{
	return strcmp(itname, "LightShards") == 0 ||
		strcmp(itname, "StrongShards") == 0 ||
		strcmp(itname, "HitProbFragments") == 0 ||
		strcmp(itname, "DRFragments") == 0 ||
		strcmp(itname, "HPRecFragments") == 0 ||
		strcmp(itname, "SPRecFragments") == 0 ||
		strcmp(itname, "MPRecFragments") == 0 ||
		strcmp(itname, "MRFragments") == 0 ||
		strcmp(itname, "PaFragments") == 0 ||
		strcmp(itname, "MaFragments") == 0 ||
		strcmp(itname, "ExpFragments") == 0;
}

bool CGame::IsSegment(char* itname)
{
	return strcmp(itname, "IceSegments") == 0 ||
		strcmp(itname, "LightningSegments") == 0 ||
		strcmp(itname, "FireSegments") == 0;
}

bool CGame::IsSpecialWeapon(char* itname)
{
	return strcmp(itname, "BlazingSword") == 0 ||
		strcmp(itname, "RapierofEmbers") == 0 ||
		strcmp(itname, "AsteroidHammer") == 0 ||

		strcmp(itname, "LightningSword") == 0 ||
		strcmp(itname, "HammerOfThor") == 0 ||
		strcmp(itname, "ThunderRapier") == 0 ||

		strcmp(itname, "IceExecutor") == 0 ||
		strcmp(itname, "FrozenHammer") == 0 ||
		strcmp(itname, "FireWizardStaff") == 0 ||
		strcmp(itname, "LightningWizStaff") == 0 ||
		strcmp(itname, "IceWizardStaff") == 0 ||

		strcmp(itname, "IceBladeOfAby") == 0 ||
		strcmp(itname, "FireBladeOfAby") == 0 ||
		strcmp(itname, "LightBladeOfAby") == 0 ||

		strcmp(itname, "IceStaffOfAby") == 0 ||
		strcmp(itname, "FireStaffOfAby") == 0 ||
		strcmp(itname, "LightStaffOfAby") == 0 ||

		strcmp(itname, "IceHammerOfAby") == 0 ||
		strcmp(itname, "FireHammerOfAby") == 0 ||
		strcmp(itname, "LightHammerOfAby") == 0 ||

		strcmp(itname, "EsterkOfFrost") == 0;
}

void CGame::click_trades(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 79;
	int maxlines = 18;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	switch (mode)
	{
	case 0:
	{

		if (trade_mode == DEF_NOTSELECTED)
		{
			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
			{

				isSearchActive = false;
				ZeroMemory(searchFilter, sizeof(searchFilter));

				m_stDialogBoxInfo[dnumber].sV3 = 0;
				bSendCommand(CLIENT_REQUEST_FULLDATACHAR, NULL, NULL, NULL, NULL, NULL, NULL);
				trade_mode = DEF_TRADEEK;
				SendChat("/tradeek");
				PlaySound('E', 14, 5);
				return;
			}

			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
			{

				isSearchActive = false;
				ZeroMemory(searchFilter, sizeof(searchFilter));

				m_stDialogBoxInfo[dnumber].sV3 = 0;
				trade_mode = DEF_TRADETOKENS;
				SendChat("/tradetokens");
				PlaySound('E', 14, 5);
				return;
			}


			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
			{

				isSearchActive = false;
				ZeroMemory(searchFilter, sizeof(searchFilter));

				m_stDialogBoxInfo[dnumber].sV3 = 0;
				bSendCommand(CLIENT_REQUEST_FULLDATACHAR, NULL, NULL, NULL, NULL, NULL, NULL);
				trade_mode = DEF_TRADECONTRIB;
				SendChat("/tradecontrib");
				PlaySound('E', 14, 5);
				return;
			}

			//add
			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
			{

				isSearchActive = false;
				ZeroMemory(searchFilter, sizeof(searchFilter));

				m_stDialogBoxInfo[dnumber].sV3 = 0;
				bSendCommand(CLIENT_REQUEST_FULLDATACHAR, NULL, NULL, NULL, NULL, NULL, NULL);
				trade_mode = DEF_TRADEMAJS;
				SendChat("/trademajs");
				PlaySound('E', 14, 5);
				return;
			}

			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 140) && (msY <= sY + 140 + 15))
			{
				m_stDialogBoxInfo[dnumber].sV3 = 0;
				trade_mode = DEF_TRADEBALLS;
				PlaySound('E', 14, 5);
				return;
			}
/*

			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
			{

				isSearchActive = false;
				ZeroMemory(searchFilter, sizeof(searchFilter));

				m_stDialogBoxInfo[dnumber].sV3 = 0;
				trade_mode = DEF_TRADEEVENTPOINTS;
				SendChat("/tradeeventpoints");
				PlaySound('E', 14, 5);
				return;
			}*/

		/*	if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
			{
				m_stDialogBoxInfo[dnumber].sV3 = 0;
				trade_mode = DEF_STARTERPACK;
				PlaySound('E', 14, 5);
				return;
			}

			if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 180) && (msY <= sY + 180 + 15))
			{
				m_stDialogBoxInfo[dnumber].sV3 = 0;
				trade_mode = DEF_TRADETOKENS;
				SendChat("/tradetokens");
				PlaySound('E', 14, 5);
				return;
			}
*/

			return;
		}


	if (trade_mode == DEF_TRADEBALLS)
	{
		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 1;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}
			
		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
		{
			/*EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 2;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);*/
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 16;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}
			
		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 3;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}
			
		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 4;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 140) && (msY <= sY + 140 + 15))
		{
			m_stDialogBoxInfo[dnumber].cMode = 50;
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
		{
			m_stDialogBoxInfo[dnumber].cMode = 51;
			PlaySound('E', 14, 5);
		}

		/*if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 16;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}*/

		auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back)
		{


			isSearchActive = false;
			ZeroMemory(searchFilter, sizeof(searchFilter));

			EndInputString();

			trade_mode = DEF_NOTSELECTED;
			PlaySound('E', 14, 5);
		}
	}

	if (trade_mode == DEF_STARTERPACK)
	{
		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
		{
			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_STARTERPACK);
			Push(cp, (u16)0);

			short type = 1;
			Push(cp, type);

			m_pGSock->iSendMsg(data, cp - data);

			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
		{
			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_STARTERPACK);
			Push(cp, (u16)0);

			short type = 2;
			Push(cp, type);

			m_pGSock->iSendMsg(data, cp - data);

			PlaySound('E', 14, 5);
		}
	}
	const int iSearchBoxX = sX + 70;
	const int iSearchBoxY = sY + 23;
	const int iSearchBoxWidth = 150;
	const int maxlines = 11;

	// Click en cuadro de búsqueda
	if ((msX >= iSearchBoxX) && (msX <= iSearchBoxX + iSearchBoxWidth) &&
		(msY >= iSearchBoxY) && (msY <= iSearchBoxY + 18))
	{
		isSearchActive = true;
		StartInputString(iSearchBoxX + 2, iSearchBoxY + 2, sizeof(searchFilter) - 1, searchFilter, FALSE);
		return;
	}

	// Generar índice filtrado
	std::vector<int> filtered_indices;
	if (strlen(searchFilter)) {
		for (int i = 0; i < vec_trades.size(); ++i) {
			if (stristr(vec_trades[i].itemname, searchFilter) != nullptr)
				filtered_indices.push_back(i);
		}
	}
	else {
		for (int i = 0; i < vec_trades.size(); ++i)
			filtered_indices.push_back(i);
	}

	// Click en ítems filtrados
	for (int i = 0; i < maxlines; i++) {
		int viewIndex = i + m_stDialogBoxInfo[79].sView;
		if (viewIndex < filtered_indices.size()) {
			if ((msX >= sX + 20) && (msX <= sX + 220) &&
				(msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
			{


				isSearchActive = false;
				ZeroMemory(searchFilter, sizeof(searchFilter));

				EndInputString();
				tmptrade = filtered_indices[viewIndex]; // índice original
				m_stDialogBoxInfo[79].cMode = 1;
				PlaySound('E', 14, 5);
				return;
			}
		}
	}

		

		if (trade_mode != DEF_NOTSELECTED)
		{
			auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
			if (back)
			{


				isSearchActive = false;
				ZeroMemory(searchFilter, sizeof(searchFilter));

				EndInputString();

				trade_mode = DEF_NOTSELECTED;
				PlaySound('E', 14, 5);
			}
		}
		
		break;
	}

	case 50:
	{

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 5;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 6;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 7;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 120) && (msY <= sY + 120 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 8;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 140) && (msY <= sY + 140 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 9;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 160) && (msY <= sY + 160 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 10;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 180) && (msY <= sY + 180 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 11;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 200) && (msY <= sY + 200 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 12;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 220) && (msY <= sY + 220 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 13;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 240) && (msY <= sY + 240 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 14;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 260) && (msY <= sY + 260 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 15;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		/*if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 280) && (msY <= sY + 280 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 16;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}*/
	
		auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back)
		{


			isSearchActive = false;
			ZeroMemory(searchFilter, sizeof(searchFilter));

			EndInputString();

			trade_mode = DEF_NOTSELECTED;
			m_stDialogBoxInfo[dnumber].cMode = 0;
			PlaySound('E', 14, 5);
		}
		

		break;
	}

	case 51:
	{

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 60) && (msY <= sY + 60 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 17;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 80) && (msY <= sY + 80 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 18;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}


		if ((msX >= sX + 40) && (msX <= sX + sizeX - 40) && (msY >= sY + 100) && (msY <= sY + 100 + 15))
		{
			EnableDialogBox(80, NULL, NULL, NULL, NULL);
			confirmoperation = 19;
			StartInputString(m_stDialogBoxInfo[80].sX + 70, m_stDialogBoxInfo[80].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
		}

		auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back)
		{


			isSearchActive = false;
			ZeroMemory(searchFilter, sizeof(searchFilter));

			EndInputString();

			trade_mode = DEF_NOTSELECTED;
			m_stDialogBoxInfo[dnumber].cMode = 0;
			PlaySound('E', 14, 5);
		}
		break;
	}
	case 1:
	{

		auto yes = (msX >= sX + 30) && (msX <= sX + 105) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		auto no = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);

		if (yes)
		{
			tmpquant = m_stDialogBoxInfo[dnumber].sV3;

			char data[256];
			auto * cp = data;
			
			Push(cp, (u32)REQ_TRADE);
			Push(cp, (u16)0);

			Push(cp, trade_mode);
			Push(cp, tmptrade);
			Push(cp, tmpquant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(79);
			PlaySound('E', 14, 5);
		}

		if (no)
		{
			m_stDialogBoxInfo[79].cMode = 0;
			PlaySound('E', 14, 5);
		}

		break;
	}
	default:
		break;
	}
}


void CGame::DrawShadowMenu_Borders2(short sX, short sY, short szX, short szY)
{
	return;
	DrawShadowPixels2(1, szX, sX, sY);
	DrawShadowPixels2(1, szX, sX, sY + szY);
	DrawShadowPixels2(2, szY, sX, sY);
	DrawShadowPixels2(2, szY + 1, sX + szX, sY);
}


// draw pixels
void CGame::DrawShadowPixels2(int mode, int count, short x, short y)
{
	switch (mode)
	{
	case 1:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x + (d), y, 78, 45, 14);
		}
		break;
	}

	case 2:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x, y + (d), 78, 45, 14);
		}
		break;
	}
	}
}


void CGame::get_trade_ek(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trades u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}


void CGame::get_trade_contrib(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trades u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}

void CGame::get_trade_tradeeventpoints(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trades u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}

void CGame::get_trade_coin(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trades u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}

void CGame::get_trade_majs(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trades u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}

void CGame::get_trade_tokens(char * data)
{
	vec_trades.clear();
	int size = 0;
	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		st_trades u;
		ZeroMemory(u.itemname, sizeof(u.itemname));
		Pop(data, u.itemname, 21);
		Pop(data, u.cost);
		Pop(data, u.sprite);
		Pop(data, u.spriteframe);
		Pop(data, u.itemcolor);
		vec_trades.push_back(u);
	}
}

void CGame::DrawShadowPixels(int mode, int count, short x, short y)
{
	switch (mode)
	{
	case 1:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x + (d), y, 73, 58, 42);
		}
		break;
	}

	case 2:
	{
		for (int d = 0; d < count; d++)
		{
			m_DDraw.PutPixel(x, y + (d), 73, 58, 42);
		}
		break;
	}
	}
}

void CGame::DrawMenu_TextBox(short sX, short sY, short x, short y)
{
	DrawShadowPixels(1, x, sX, sY);
	DrawShadowPixels(1, x, sX, sY + y);
	DrawShadowPixels(2, y, sX, sY);
	DrawShadowPixels(2, y + 1, sX + x, sY);
}

void CGame::draw_confirm(short msX, short msY, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 80;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	DrawShadow_Box(sX, sY, sX + sizeX, sY + sizeY);
	DrawShadowMenu_DialogBorders(sX, sY, sizeX, sizeY);

	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Confirm Operation", 230, 230, 255);
	
	switch (confirmoperation)
	{

	case 1:
	{
		DrawMenu_TextBox(sX + 65, sY + 45, 120, 22);
		PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Contribution Ball", 230, 230, 255);

		PutString2(sX + 70, sY + 50, m_cAmmount, 230, 230, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 15, 90, "Get");
		DrawMenu_SmallButton(sX, sY, msX, msY, 163, 90, "Cancel");
		break;
	}
	case 2:
	{
		DrawMenu_TextBox(sX + 65, sY + 45, 120, 22);
		PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Coin Ball", 230, 230, 255);

		PutString2(sX + 70, sY + 50, m_cAmmount, 230, 230, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 15, 90, "Get");
		DrawMenu_SmallButton(sX, sY, msX, msY, 163, 90, "Cancel");
		break;
	}
	case 3:
	{
		DrawMenu_TextBox(sX + 65, sY + 45, 120, 22);
		PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Reputation Ball", 230, 230, 255);

		PutString2(sX + 70, sY + 50, m_cAmmount, 230, 230, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 15, 90, "Get");
		DrawMenu_SmallButton(sX, sY, msX, msY, 163, 90, "Cancel");
		break;
	}
	case 4:
	{
		DrawMenu_TextBox(sX + 65, sY + 45, 120, 22);
		PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Majestic Ball", 230, 230, 255);

		PutString2(sX + 70, sY + 50, m_cAmmount, 230, 230, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 15, 90, "Get");
		DrawMenu_SmallButton(sX, sY, msX, msY, 163, 90, "Cancel");
		break;
	}


	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	{
		DrawMenu_TextBox(sX + 65, sY + 45, 120, 22);

		switch (confirmoperation)
		{
		case 5: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Light Shards", 230, 230, 255); break;
		case 6: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Strong Shards", 230, 230, 255); break;
		case 7: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "HP Fragments", 230, 230, 255); break;
		case 8: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "DR Fragments", 230, 230, 255); break;
		case 9: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "HP Rec Fragments", 230, 230, 255); break;
		case 10: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "SP Rec Fragments", 230, 230, 255); break;
		case 11: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "MP Rec Fragments", 230, 230, 255); break;
		case 12: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "MR Fragments", 230, 230, 255); break;
		case 13: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Pa Fragments", 230, 230, 255); break;
		case 14: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Ma Fragments", 230, 230, 255); break;
		case 15: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Exp Fragments", 230, 230, 255); break;
		case 16: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Tokens", 230, 230, 255); break;

		case 17: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Fire Segments", 230, 230, 255); break;
		case 18: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Lightning Segments", 230, 230, 255); break;
		case 19: PutAlignedString(sX + 5, sX + sizeX - 5, sY + 27, "Ice Segments", 230, 230, 255); break;
		default: break;
		}

		PutString2(sX + 70, sY + 50, m_cAmmount, 230, 230, 255);

		DrawMenu_SmallButton(sX, sY, msX, msY, 15, 90, "Get");
		DrawMenu_SmallButton(sX, sY, msX, msY, 163, 90, "Cancel");
		break;
	}
	default: break;
	}

	DrawDialogCord(sX, sY, msX, msY);

}

void CGame::click_confirm(short msX, short msY)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 80;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	switch (confirmoperation)
	{
	case 1:
	{
		auto strinput = (msX >= sX + 65) && (msX <= sX + 65 + 120) && (msY >= sY + 45) && (msY <= sY + 65);
		if (strinput)
		{
			StartInputString(m_stDialogBoxInfo[dnumber].sX + 70, m_stDialogBoxInfo[dnumber].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
			return;
		}

		auto give = (msX >= sX + 15) && (msX <= sX + 90) && (msY >= sY + 90) && (msY <= sY + 110);
		auto cancel = (msX >= sX + 163) && (msX <= sX + 239) && (msY >= sY + 90) && (msY <= sY + 110);

		if (give)
		{
			EndInputString();

			if (!isValidAmount(m_cAmmount))
			{
				AddEventList("Error. Invalid Contribution amount.");
				return;
			}

			int quant = atoi(m_cAmmount);

			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_TRADE_BALL);
			Push(cp, (u16)0);

			Push(cp, (short)BALL_CONTRIB);
			Push(cp, quant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}

		if (cancel)
		{
			EndInputString();
			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}
	case 2:
	{
		auto strinput = (msX >= sX + 65) && (msX <= sX + 65 + 120) && (msY >= sY + 45) && (msY <= sY + 65);
		if (strinput)
		{
			StartInputString(m_stDialogBoxInfo[dnumber].sX + 70, m_stDialogBoxInfo[dnumber].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
			return;
		}

		auto give = (msX >= sX + 15) && (msX <= sX + 90) && (msY >= sY + 90) && (msY <= sY + 110);
		auto cancel = (msX >= sX + 163) && (msX <= sX + 239) && (msY >= sY + 90) && (msY <= sY + 110);

		if (give)
		{
			EndInputString();

			if (!isValidAmount(m_cAmmount))
			{
				AddEventList("Error. Invalid Contribution amount.");
				return;
			}

			int quant = atoi(m_cAmmount);

			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_TRADE_BALL);
			Push(cp, (u16)0);

			Push(cp, (short)BALL_COIN);
			Push(cp, quant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}

		if (cancel)
		{
			EndInputString();
			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}

	case 3:
	{
		auto strinput = (msX >= sX + 65) && (msX <= sX + 65 + 120) && (msY >= sY + 45) && (msY <= sY + 65);
		if (strinput)
		{
			StartInputString(m_stDialogBoxInfo[dnumber].sX + 70, m_stDialogBoxInfo[dnumber].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
			return;
		}

		auto give = (msX >= sX + 15) && (msX <= sX + 90) && (msY >= sY + 90) && (msY <= sY + 110);
		auto cancel = (msX >= sX + 163) && (msX <= sX + 239) && (msY >= sY + 90) && (msY <= sY + 110);

		if (give)
		{
			EndInputString();

			if (!isValidAmount(m_cAmmount))
			{
				AddEventList("Error. Invalid Contribution amount.");
				return;
			}

			int quant = atoi(m_cAmmount);

			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_TRADE_BALL);
			Push(cp, (u16)0);

			Push(cp, (short)BALL_REP);
			Push(cp, quant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}

		if (cancel)
		{
			EndInputString();
			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}
	case 4:
	{
		auto strinput = (msX >= sX + 65) && (msX <= sX + 65 + 120) && (msY >= sY + 45) && (msY <= sY + 65);
		if (strinput)
		{
			StartInputString(m_stDialogBoxInfo[dnumber].sX + 70, m_stDialogBoxInfo[dnumber].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
			return;
		}

		auto give = (msX >= sX + 15) && (msX <= sX + 90) && (msY >= sY + 90) && (msY <= sY + 110);
		auto cancel = (msX >= sX + 163) && (msX <= sX + 239) && (msY >= sY + 90) && (msY <= sY + 110);

		if (give)
		{
			EndInputString();

			if (!isValidAmount(m_cAmmount))
			{
				AddEventList("Error. Invalid Contribution amount.");
				return;
			}

			int quant = atoi(m_cAmmount);

			char data[256];
			auto * cp = data;

			Push(cp, (u32)REQ_TRADE_BALL);
			Push(cp, (u16)0);

			Push(cp, (short)BALL_MAJS);
			Push(cp, quant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}

		if (cancel)
		{
			EndInputString();
			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}

	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	{
		auto strinput = (msX >= sX + 65) && (msX <= sX + 65 + 120) && (msY >= sY + 45) && (msY <= sY + 65);
		if (strinput)
		{
			StartInputString(m_stDialogBoxInfo[dnumber].sX + 70, m_stDialogBoxInfo[dnumber].sY + 50, 8, m_cAmmount);
			PlaySound('E', 14, 5);
			return;
		}

		auto give = (msX >= sX + 15) && (msX <= sX + 90) && (msY >= sY + 90) && (msY <= sY + 110);
		auto cancel = (msX >= sX + 163) && (msX <= sX + 239) && (msY >= sY + 90) && (msY <= sY + 110);

		if (give)
		{
			EndInputString();

			if (!isValidAmount(m_cAmmount))
			{
				AddEventList("Error. Invalid Contribution amount.");
				return;
			}

			int quant = atoi(m_cAmmount);

			char data[256];
			auto * cp = data;

			
			Push(cp, (u32)REQ_TRADE_BALL);
			Push(cp, (u16)0);
	
			switch (confirmoperation)
			{
				case 5: Push(cp, (short)BALL_LIGHT); break;
				case 6: Push(cp, (short)BALL_STRONG); break;
				case 7: Push(cp, (short)BALL_HITPROB); break;
				case 8: Push(cp, (short)BALL_DR); break;
				case 9: Push(cp, (short)BALL_HPREC); break;
				case 10: Push(cp, (short)BALL_SPREC); break;
				case 11: Push(cp, (short)BALL_MPREC); break;
				case 12: Push(cp, (short)BALL_MR); break;
				case 13: Push(cp, (short)BALL_PA); break;
				case 14: Push(cp, (short)BALL_MA); break;
				case 15: Push(cp, (short)BALL_EXP); break;
				case 16: Push(cp, (short)BALL_MASTERY); break;

				case 17: Push(cp, (short)BALL_FIRESEGMENT); break;
				case 18: Push(cp, (short)BALL_LIGHTNINGSEGMENT); break;
				case 19: Push(cp, (short)BALL_ICESEGMENT); break;
			}
			
			Push(cp, quant);

			m_pGSock->iSendMsg(data, cp - data);

			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}

		if (cancel)
		{
			EndInputString();
			DisableDialogBox(dnumber);
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}

	default: break;
	}
}

bool CGame::isValidAmount(const std::string& str) {
	// La cadena vacía no es un número válido
	if (str.empty()) {
		return false;
	}

	if (string(str) == "0") return false;

	// Comprobar que la longitud no sea mayor a 8
	if (str.length() > 8) {
		return false;
	}

	// Comprobar que todos los caracteres son dígitos
	for (char c : str) {
		if (!std::isdigit(c)) {
			return false;
		}
	}

	// Comprobar que no tenga ceros a la izquierda
	if (str.size() > 1 && str[0] == '0') {
		return false;
	}

	return true;
}

double CGame::calculatePing() {
	std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
	return elapsed.count(); // Retorna el tiempo en milisegundos
}

void CGame::startUDPClient(int port) {
	initializeWinsock();
	createSocket();
	setupServerAddress(DEF_SERVER_IP, port);
}

void CGame::destroyUDPClient() {
	closesocket(clientSocket);
	WSACleanup();
}

void CGame::initializeWinsock() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Failed to initialize Winsock." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void CGame::setupServerAddress(const std::string& serverIp, int port) {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
	serverAddr.sin_port = htons(port);
}

void CGame::createSocket() {
	clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Failed to create socket." << std::endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
}

void CGame::sendMessageToServer(int msgid) {
	return;
	char data[500];
	char* cp = data;

	bool bsend = false;

	Push(cp, msgid);
	switch (msgid)
	{
	case SOCKMSG_WELCOME:
	{
		char message[5] = "hola";
		Push(cp, message, 5); // Pasa el tamaño real de la cadena
		bsend = true;
		break;
	}
	case SOCKMSG_PING:
	{
		startTime = std::chrono::high_resolution_clock::now();
		bsend = true;
		break;
	}
	}

	if (!bsend) return;

	// Calcular el tamaño total del mensaje
	int totalSize = cp - data;

	// Enviar el mensaje
	int bytesSent = sendto(clientSocket, data, totalSize, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (bytesSent == SOCKET_ERROR) {
		std::cerr << "Error sending custom message to client." << std::endl;
	}
	else {
		std::cout << "Custom message sent to client: Size = " << totalSize << std::endl;
	}
}

void CGame::receiveMessageFromServer()
{
	while (true) {
		char buffer[500];
		sockaddr_in fromAddr;
		int fromAddrLen = sizeof(fromAddr);
		
		int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&fromAddr, &fromAddrLen);
		if (bytesReceived == SOCKET_ERROR) {
			continue;
		}

		char* cp = buffer;

		int msgid;
		Pop(cp, msgid);

		switch (msgid)
		{
		case SOCKMSG_WELCOME:
		{
			char test3[4];
			Pop(cp, test3, sizeof(test3));
			break;
		}
		case SOCKMSG_PING:
		{
			endTime = std::chrono::high_resolution_clock::now();
			break;
		}
		case SOCKMSG_OBJECTCOMMAND:
		{/*
			short owner_type;
			short command;
			Pop(cp, owner_type);
			Pop(cp, command);
			
			if (owner_type == DEF_OWNERTYPE_NPC && command == ACTION_OBJECTMOVE)
			{
				int owner;
				short sx;
				short sy;
				short type;
				char dir;
				char cName[5];
				short Appr2;
				int status;
				char killed;
				short sAppr1, sAppr3, sAppr4, sV1, sV2, sV3, sPrevAppr2;
				int iApprColor, iApprColor2, iLoc;
				WORD wEventType;
				char AdminLevel;

				Pop(cp, owner);
				Pop(cp, sx);
				Pop(cp, sy);
				Pop(cp, type);
				Pop(cp, dir);
				Pop(cp, cName, 5);
				Pop(cp, Appr2);
				Pop(cp, status);
				Pop(cp, killed);


				m_pMapData->bSetOwner(owner, sx, sy, type, dir, sAppr1, Appr2, sAppr3, sAppr4, iApprColor, iApprColor2, status, cName, (char)wEventType, sV1, sV2, sV3, iLoc, 0, AdminLevel);

			}*/

			break;
		}
		default:
			break;
		}
	}
}

void CGame::request_ping()
{
	char data[10];
	auto * cp = data;

	Push(cp, (u32)NOTIFY_PING);
	Push(cp, (u16)0);

	m_pGSock->iSendMsg(data, cp - data);
}

/*


int CGame::UDPrequest_ping() {
	while (running) {

		char buffer[UDP_BUFFER_SIZE];
		int serverAddrSize = sizeof(serverAddr);

		// Inicializar Winsock
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cerr << "Error al iniciar Winsock" << std::endl;
			return -1;
		}

		// Crear socket
		clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (clientSocket == INVALID_SOCKET) {
			std::cerr << "Error al crear socket" << std::endl;
			WSACleanup();
			return -1;
		}

		// Configurar dirección del servidor
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
		serverAddr.sin_port = htons(SERVER_PORT);

		// Preparar mensaje
		buffer[0] = 'P'; // Mensaje mínimo (1 byte)

		// Registrar tiempo de envío
		auto startTime = std::chrono::high_resolution_clock::now();

		// Enviar mensaje
		if (sendto(clientSocket, buffer, UDP_BUFFER_SIZE, 0, (sockaddr*)&serverAddr, serverAddrSize) == SOCKET_ERROR) {
			std::cerr << "Error al enviar mensaje" << std::endl;
			closesocket(clientSocket);
			WSACleanup();
			return -1;
		}

		// Recibir respuesta
		int receivedBytes = recvfrom(clientSocket, buffer, UDP_BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &serverAddrSize);
		if (receivedBytes == SOCKET_ERROR) {
			std::cerr << "Error al recibir respuesta" << std::endl;
		}
		else {
			auto endTime = std::chrono::high_resolution_clock::now();
			// Calcular ping
			auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime); // Usar 'double' para precisión decimal
			m_dPingSocket = duration.count() * 1000.0; // Convertir a milisegundos

			// Mostrar ping con 2 decimales
			std::cout << "Ping: " << std::fixed << std::setprecision(2) << m_dPingSocket << " ms" << std::endl;
		}

		closesocket(clientSocket);
		WSACleanup();

		if (!running) break;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Esperar antes de enviar de nuevo
	}

	return 0;
}
*/

void CGame::draw_map(short msX, short msY, char cLB)
{
	int dnumber = 81;
	short sX, sY, szX, szY;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	szX = m_stDialogBoxInfo[dnumber].sSizeX;
	szY = m_stDialogBoxInfo[dnumber].sSizeY;

	DrawShadowDialog2(sX, sY, dnumber);
	
	PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Map Guide", 255, 255, 0);

	if (isInMap("city"))
	{
		DrawGuideMap(0, sX + 30, sY + 30);
	}
	else if (isInMap("2ndmiddle"))
	{
		DrawGuideMap(1, sX + 30, sY + 30);		
	}
	else if (isInMap("minezone"))
	{
		DrawGuideMap(2, sX + 30, sY + 30);
	}
	else if (isInMap("rfarm"))
	{
		DrawGuideMap(3, sX + 30, sY + 30);
	}
	else if (isInMap("icemap"))
	{
		DrawGuideMap(4, sX + 30, sY + 30);
	}
	else if (isInMap("dungeon"))
	{
		DrawGuideMap(5, sX + 30, sY + 30);
	}
	else if (isInMap("dungeon2"))
	{
		DrawGuideMap(6, sX + 30, sY + 30);
	}
	else if (isInMap("aresden"))
	{
		DrawGuideMap(7, sX + 30, sY + 30);
	}
	else if (isInMap("elvine"))
	{
		DrawGuideMap(8, sX + 30, sY + 30);
	}

	DrawDialogCord(sX, sY, msX, msY);
	
	if ((msX >= sX + 30) && (msX <= sX + 106) && (msY >= sY + 437) && (msY <= sY + 437 + 12))	
		PutString2(sX + 30, sY + 437, showpits ? "Show Pits: On" : "Show Pits: Off", 255, 255, 255);	
	else	
		PutString2(sX + 30, sY + 437, showpits ? "Show Pits: On" : "Show Pits: Off", 0, 255, 0);

	if ((msX >= sX + 173) && (msX <= sX + 285) && (msY >= sY + 437) && (msY <= sY + 437 + 12))
		PutString2(sX + 173, sY + 437, showteleports ? "Show teleports: On" : "Show teleports: Off", 255, 255, 255);
	else
		PutString2(sX + 173, sY + 437, showteleports ? "Show teleports: On" : "Show teleports: Off", 2255, 0, 0);

	if ((msX >= sX + szX - 113) && (msX <= sX + szX - 113 + 75) && (msY >= sY + 437) && (msY <= sY + 437 + 12))
		PutString2(sX + szX - 113, sY + 437, showsafes ? "Show safes: On" : "Show safes: Off", 255, 255, 255);
	else
		PutString2(sX + szX - 113, sY + 437, showsafes ? "Show safes: On" : "Show safes: Off", 255, 255, 0);
}


void CGame::DrawLog(string str)
{
	AddEventList((char*)str.c_str(), 10);
}

void CGame::DrawGuideMap(short smap, short sX, short sY)
{
	short sPlayerXOnMap;
	short sPlayerYOnMap;
	short sSafeXOnMap;
	short sSafeYOnMap;

	if (smap == 0) //300 x 300
	{
		sPlayerXOnMap = m_sPlayerX * 1.333f;
		sPlayerYOnMap = m_sPlayerY * 1.333f;
	}
	else if (smap == 1)
	{
		sPlayerXOnMap = m_sPlayerX * 1.6;
		sPlayerYOnMap = m_sPlayerY * 1.6;
	}
	else if (smap == 2)
	{
		sPlayerXOnMap = m_sPlayerX * 2.0;
		sPlayerYOnMap = m_sPlayerY * 2.0;
	}
	else if (smap == 3) //250 x 250
	{
		sPlayerXOnMap = m_sPlayerX * 1.6;
		sPlayerYOnMap = m_sPlayerY * 1.6;
	}
	else if (smap == 4) //200x200
	{
		sPlayerXOnMap = m_sPlayerX * 2.0;
		sPlayerYOnMap = m_sPlayerY * 2.0;
	}
	else if (smap == 5) //200x200
	{
		sPlayerXOnMap = m_sPlayerX * 2.0;
		sPlayerYOnMap = m_sPlayerY * 2.0;
	}
	else if (smap == 6) //200x200
	{
		sPlayerXOnMap = m_sPlayerX * 2.0;
		sPlayerYOnMap = m_sPlayerY * 2.0;
	}
	else if (smap == 7) //300 x 300
	{
		sPlayerXOnMap = m_sPlayerX * 1.333f;
		sPlayerYOnMap = m_sPlayerY * 1.333f;
	}
	else if (smap == 8) //300 x 300
	{
		sPlayerXOnMap = m_sPlayerX * 1.333f;
		sPlayerYOnMap = m_sPlayerY * 1.333f;
	}
	else return;

	// Dibujar el mapa
	DrawNewDialogBox2(SPRID_GUIDEMAP + smap, sX, sY, 0, TRUE);
	//DrawNewDialogBox2(SPRID_RELIC, sX, sY, 0, FALSE, FALSE);
	//return;

	// Dibujar la posición del jugador en el mapa
	DrawPlayerIcon(sX + sPlayerXOnMap, sY + sPlayerYOnMap);

	if (smap == 0)
	{
		if (showpits)
		{
			PutString2(sX + 48 * 1.333f, sY + 125 * 1.333f, "DireBoar", 0, 255, 0);
			PutString2(sX + 50 * 1.333f, sY + 205 * 1.333f, "Ettin", 0, 255, 0);
			PutString2(sX + 245 * 1.333f, sY + 181 * 1.333f, "Orc", 0, 255, 0);
			PutString2(sX + 38 * 1.333f, sY + 85 * 1.333f, "Orc", 0, 255, 0);
			PutString2(sX + 78 * 1.333f, sY + 237 * 1.333f, "Rudolph", 0, 255, 0);
			PutString2(sX + 208 * 1.333f, sY + 48 * 1.333f, "Skeleton", 0, 255, 0);
			PutString2(sX + 158 * 1.333f, sY + 41 * 1.333f, "Zombie", 0, 255, 0);
			PutString2(sX + 108 * 1.333f, sY + 239 * 1.333f, "Unicorn", 0, 255, 0);
			PutString2(sX + 59 * 1.333f, sY + 175 * 1.333f, "Cyclops", 0, 255, 0);
			PutString2(sX + 176 * 1.333f, sY + 72 * 1.333f, "Troll", 0, 255, 0);
			PutString2(sX + 119 * 1.333f, sY + 198 * 1.333f, "Frost", 0, 255, 0);
			PutString2(sX + 150 * 1.333f, sY + 113 * 1.333f, "Frost", 0, 255, 0);
			PutString2(sX + 78 * 1.333f, sY + 101 * 1.333f, "Scorpion", 0, 255, 0);
			PutString2(sX + 160 * 1.333f, sY + 204 * 1.333f, "Giant-Ant", 0, 255, 0);
			PutString2(sX + 228 * 1.333f, sY + 108 * 1.333f, "Cyclops", 0, 255, 0);
			PutString2(sX + 199 * 1.333f, sY + 177 * 1.333f, "Giant-Frog", 0, 255, 0);
			PutString2(sX + 91 * 1.333f, sY + 134 * 1.333f, "Orge", 0, 255, 0);
			PutString2(sX + 142 * 1.333f, sY + 232 * 1.333f, "Beholder", 0, 255, 0);
			PutString2(sX + 181 * 1.333f, sY + 136 * 1.333f, "Giant-Plant", 0, 255, 0);
			PutString2(sX + 224 * 1.333f, sY + 163 * 1.333f, "Stone-Golem", 0, 255, 0);
			PutString2(sX + 85 * 1.333f, sY + 207 * 1.333f, "Stalker", 0, 255, 0);
			PutString2(sX + 112 * 1.333f, sY + 89 * 1.333f, "Clay-Golem", 0, 255, 0);
			PutString2(sX + 184 * 1.333f, sY + 92 * 1.333f, "WereWolf", 0, 255, 0);
			PutString2(sX + 122 * 1.333f, sY + 175 * 1.333f, "WereWolf", 0, 255, 0);
			PutString2(sX + 228 * 1.333f, sY + 79 * 1.333f, "Ice-Golem", 0, 255, 0);
			PutString2(sX + 187 * 1.333f, sY + 163 * 1.333f, "Hellbound", 0, 255, 0);
			PutString2(sX + 261 * 1.333f, sY + 64 * 1.333f, "Ettin", 0, 255, 0);
		}

		if (showsafes)
		{
			sSafeXOnMap = 92 * 1.333f;
			sSafeYOnMap = 58 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 223 * 1.333f;
			sSafeYOnMap = 236 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 155 * 1.333f;
			sSafeYOnMap = 154 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);
		}


		if (showteleports)
		{
		
			sSafeXOnMap = 247 * 1.333f;
			sSafeYOnMap = 232 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Aresden Shop");

			sSafeXOnMap = 107 * 1.333f;
			sSafeYOnMap = 48 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Elvine Shop");

			// City Teleports
			// Minezone Elvine Side
			sSafeXOnMap = 42 * 1.333f;
			sSafeYOnMap = 74 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Dungeon L1");

			// Minezone Aresden Side
			sSafeXOnMap = 175 * 1.333f;
			sSafeYOnMap = 259 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Dungeon L1");

		}
	}
	else if (smap == 1)
	{
		if (showpits)
		{
			// Wyvern / Fire-Wyvern / Tigerworm / Hellclaw
			PutString2(sX + 218 * 1.6, sY + 170 * 1.6, "Wyvern", 0, 255, 0);
			PutString2(sX + 208 * 1.6, sY + 186 * 1.6, "Fire-Wyvern", 0, 255, 0);
			PutString2(sX + 218 * 1.6, sY + 217 * 1.6, "Tigerworm", 0, 255, 0);
			PutString2(sX + 184 * 1.6, sY + 52 * 1.6, "Hellclaw", 0, 255, 0);

			// Ettin / Cyclops / Beholder / Stalker / Demon / Liche
			PutString2(sX + 39 * 1.6, sY + 195 * 1.6, "Ettin", 0, 255, 0);
			PutString2(sX + 57 * 1.6, sY + 168 * 1.6, "Cyclops", 0, 255, 0);
			PutString2(sX + 40 * 1.6, sY + 154 * 1.6, "Beholder", 0, 255, 0);
			PutString2(sX + 43 * 1.6, sY + 133 * 1.6, "Stalker", 0, 255, 0);
			PutString2(sX + 76 * 1.6, sY + 199 * 1.6, "Demon", 0, 255, 0);
			PutString2(sX + 114 * 1.6, sY + 199 * 1.6, "Liche", 0, 255, 0);

			// Wyvern / Fire-Wyvern / Tigerworm / Hellclaw (2)
			PutString2(sX + 202 * 1.6, sY + 38 * 1.6, "Wyvern", 0, 255, 0);
			PutString2(sX + 217 * 1.6, sY + 49 * 1.6, "Fire-Wyvern", 0, 255, 0);
			PutString2(sX + 180 * 1.6, sY + 71 * 1.6, "Tigerworm", 0, 255, 0);
			PutString2(sX + 190 * 1.6, sY + 162 * 1.6, "Hellclaw", 0, 255, 0);

			// Ettin / Cyclops / Beholder / Stalker / Demon / Liche (2)
			PutString2(sX + 37 * 1.6, sY + 68 * 1.6, "Ettin", 0, 255, 0);
			PutString2(sX + 66 * 1.6, sY + 74 * 1.6, "Cyclops", 0, 255, 0);
			PutString2(sX + 44 * 1.6, sY + 97 * 1.6, "Beholder", 0, 255, 0);
			PutString2(sX + 55 * 1.6, sY + 89 * 1.6, "Stalker", 0, 255, 0);
			PutString2(sX + 141 * 1.6, sY + 42 * 1.6, "Demon", 0, 255, 0);
			PutString2(sX + 143 * 1.6, sY + 72 * 1.6, "Liche", 0, 255, 0);
			
			PutString2(sX + 106 * 1.6, sY + 142 * 1.6, "Dark Elf", 0, 255, 0);
			PutString2(sX + 119 * 1.6, sY + 108 * 1.6, "Dark Elf", 0, 255, 0);
		}
		
		if (showteleports)
		{
			sSafeXOnMap = 140 * 1.6;
			sSafeYOnMap = 228 * 1.6;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Elvine City");

			sSafeXOnMap = 125 * 1.6;
			sSafeYOnMap = 21 * 1.6;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Aresden City");
		}
	}

	else if (smap == 2)
	{
		if (showteleports)
		{
			// Dungeone Elvine Side to Elvine
			sSafeXOnMap = 28 * 2.0;
			sSafeYOnMap = 95 * 2.0;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Elvine");

			// Dungeon Ares Side to Aresden
			sSafeXOnMap = 163 * 2.0;
			sSafeYOnMap = 97 * 2.0;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Aresden");

			// Dungeon Zone to Dungeon L2
			sSafeXOnMap = 95 * 2.0;
			sSafeYOnMap = 35 * 2.0;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Dungeon L2");
		}
	}
	else if (smap == 3)
	{
		if (showpits)
		{
			PutString2(sX + 51 * 1.6, sY + 172 * 1.6, "Scorpion", 0, 255, 0);
			PutString2(sX + 139 * 1.6, sY + 41 * 1.6, "Scorpion", 0, 255, 0);

			PutString2(sX + 80 * 1.6, sY + 177 * 1.6, "Skeleton", 0, 255, 0);
			PutString2(sX + 113 * 1.6, sY + 42 * 1.6, "Skeleton", 0, 255, 0);

			PutString2(sX + 99 * 1.6, sY + 206 * 1.6, "Orc", 0, 255, 0);
			PutString2(sX + 82 * 1.6, sY + 36 * 1.6, "Orc", 0, 255, 0);

			PutString2(sX + 93 * 1.6, sY + 160 * 1.6, "Zombie", 0, 255, 0);
			PutString2(sX + 48 * 1.6, sY + 40 * 1.6, "Zombie", 0, 255, 0);

			PutString2(sX + 88 * 1.6, sY + 125 * 1.6, "Clay-Golem", 0, 255, 0);
			PutString2(sX + 167 * 1.6, sY + 66 * 1.6, "Clay-Golem", 0, 255, 0);

			PutString2(sX + 197 * 1.6, sY + 176 * 1.6, "Stone-Golem", 0, 255, 0);
			PutString2(sX + 195 * 1.6, sY + 60 * 1.6, "Stone-Golem", 0, 255, 0);

			PutString2(sX + 128 * 1.6, sY + 131 * 1.6, "Orge", 0, 255, 0);
			PutString2(sX + 113 * 1.6, sY + 66 * 1.6, "Orge", 0, 255, 0);

			PutString2(sX + 172 * 1.6, sY + 194 * 1.6, "Troll", 0, 255, 0);
			PutString2(sX + 64 * 1.6, sY + 72 * 1.6, "Troll", 0, 255, 0);

		}
	}
	else if (smap == 4)
	{
		if (showpits)
		{
			/*// Nuevos pits añadidos
			PutString2(sX + 49 * 1.6, sY + 173 * 1.6, "WereWolf", 0, 255, 0);
			PutString2(sX + 78 * 1.6, sY + 180 * 1.6, "Frost", 0, 255, 0);
			PutString2(sX + 97 * 1.6, sY + 211 * 1.6, "Troll", 0, 255, 0);
			PutString2(sX + 170 * 1.6, sY + 195 * 1.6, "Ettin", 0, 255, 0);
			PutString2(sX + 195 * 1.6, sY + 177 * 1.6, "Hellbound", 0, 255, 0);
			PutString2(sX + 90 * 1.6, sY + 162 * 1.6, "Orge", 0, 255, 0);
			PutString2(sX + 84 * 1.6, sY + 128 * 1.6, "Ice-Golem", 0, 255, 0);

			PutString2(sX + 136 * 1.6, sY + 46 * 1.6, "WereWolf", 0, 255, 0);
			PutString2(sX + 162 * 1.6, sY + 69 * 1.6, "Frost", 0, 255, 0);
			PutString2(sX + 82 * 1.6, sY + 36 * 1.6, "Troll", 0, 255, 0);
			PutString2(sX + 111 * 1.6, sY + 68 * 1.6, "Ettin", 0, 255, 0);
			PutString2(sX + 44 * 1.6, sY + 41 * 1.6, "Hellbound", 0, 255, 0);
			PutString2(sX + 64 * 1.6, sY + 70 * 1.6, "Orge", 0, 255, 0);
			PutString2(sX + 157 * 1.6, sY + 100 * 1.6, "Ice-Golem", 0, 255, 0);*/
		}
	}
	else if (smap == 5)
	{
		if (showteleports)
		{
			// Dungeone Elvine Side to Elvine
			sSafeXOnMap = 164 * 2.0;
			sSafeYOnMap = 97 * 2.0;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Elvine");

			// Dungeon Ares Side to Aresden
			sSafeXOnMap = 35 * 2.0;
			sSafeYOnMap = 36 * 2.0;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Aresden");
		}
	}

	else if (smap == 6) // dungeon2
	{
		if (showteleports)
		{
			sSafeXOnMap = 78 * 2.0;
			sSafeYOnMap = 29 * 2.0;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Dungeon L1");
		}

		if (showpits)
		{
			PutString2(sX + 65 * 2.0, sY + 81 * 2.0, "Hellclaw", 0, 255, 0);
			PutString2(sX + 115 * 2.0, sY + 94 * 2.0, "Wyvern", 0, 255, 0);
			PutString2(sX + 134 * 2.0, sY + 38 * 2.0, "Fire-Wyvern", 0, 255, 0);
			PutString2(sX + 126 * 2.0, sY + 68 * 2.0, "Tigerworm", 0, 255, 0);
			PutString2(sX + 43 * 2.0, sY + 43 * 2.0, "Nizie", 0, 255, 0);
			PutString2(sX + 91 * 2.0, sY + 32 * 2.0, "Gagoyle", 0, 255, 0);
		}
	}

	else if (smap == 7) // dungeon2
	{
		if (showsafes)
		{
			sSafeXOnMap = 112 * 1.333f;
			sSafeYOnMap = 244 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 138 * 1.333f;
			sSafeYOnMap = 204 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 170 * 1.333f;
			sSafeYOnMap = 145 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 65 * 1.333f;
			sSafeYOnMap = 124 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 138 * 1.333f;
			sSafeYOnMap = 48 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 250 * 1.333f;
			sSafeYOnMap = 78 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 250 * 1.333f;
			sSafeYOnMap = 155 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 252 * 1.333f;
			sSafeYOnMap = 236 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);
		}



		if (showteleports)
		{

			sSafeXOnMap = 101 * 1.333f;
			sSafeYOnMap = 183 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "WareHouse");

			sSafeXOnMap = 126 * 1.333f;
			sSafeYOnMap = 166 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Shop");

			sSafeXOnMap = 26 * 1.333f;
			sSafeYOnMap = 279 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Promiseland");

			sSafeXOnMap = 78 * 1.333f;
			sSafeYOnMap = 211 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Dungeon L1");

			sSafeXOnMap = 139 * 1.333f;
			sSafeYOnMap = 117 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Elvine");


			sSafeXOnMap = 184 * 1.333f;
			sSafeYOnMap = 88 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Elvine");


		}

		if (showpits)
		{
			PutString2(sX + 37 * 1.333f, sY + 172 * 1.333f, "WereWolf", 0, 255, 0);
			PutString2(sX + 28 * 1.333f, sY + 187 * 1.333f, "Rudolph", 0, 255, 0);
			PutString2(sX + 37 * 1.333f, sY + 217 * 1.333f, "Troll", 0, 255, 0);
			PutString2(sX + 80 * 1.333f, sY + 237 * 1.333f, "Orge", 0, 255, 0);
			PutString2(sX + 127 * 1.333f, sY + 239 * 1.333f, "Giant-Plant", 0, 255, 0);
			PutString2(sX + 186 * 1.333f, sY + 233 * 1.333f, "Skeleton", 0, 255, 0);
			PutString2(sX + 216 * 1.333f, sY + 229 * 1.333f, "Zombie", 0, 255, 0);
			PutString2(sX + 230 * 1.333f, sY + 204 * 1.333f, "Clay-Golem", 0, 255, 0);
			PutString2(sX + 251 * 1.333f, sY + 199 * 1.333f, "Stone-Golem", 0, 255, 0);
			PutString2(sX + 259 * 1.333f, sY + 186 * 1.333f, "Ice-Golem", 0, 255, 0);
			PutString2(sX + 225 * 1.333f, sY + 65 * 1.333f, "Orc", 0, 255, 0);
			PutString2(sX + 155 * 1.333f, sY + 40 * 1.333f, "Scorpion", 0, 255, 0);
			PutString2(sX + 166 * 1.333f, sY + 28 * 1.333f, "Unicorn", 0, 255, 0);
			PutString2(sX + 95 * 1.333f, sY + 35 * 1.333f, "Frost", 0, 255, 0);
			PutString2(sX + 45 * 1.333f, sY + 64 * 1.333f, "Hellbound", 0, 255, 0);
		}
	}

	else if (smap == 8) // dungeon2
	{
		if (showteleports)
		{

			sSafeXOnMap = 197 * 1.333f;
			sSafeYOnMap = 127 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "WareHouse");

			sSafeXOnMap = 225 * 1.333f;
			sSafeYOnMap = 151 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Shop");

			sSafeXOnMap = 258 * 1.333f;
			sSafeYOnMap = 83 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Dungeon L1");

			sSafeXOnMap = 218 * 1.333f;
			sSafeYOnMap = 20 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Promiseland");

			sSafeXOnMap = 142 * 1.333f;
			sSafeYOnMap = 122 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Aresden");

			sSafeXOnMap = 130 * 1.333f;
			sSafeYOnMap = 72 * 1.333f;
			DrawTeleportIcon(sX + sSafeXOnMap, sY + sSafeYOnMap, "Aresden");

		}

		if (showsafes)
		{
			sSafeXOnMap = 170 * 1.333f;
			sSafeYOnMap = 145 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 60 * 1.333f;
			sSafeYOnMap = 46 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 80 * 1.333f;
			sSafeYOnMap = 198 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 222 * 1.333f;
			sSafeYOnMap = 219 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 155 * 1.333f;
			sSafeYOnMap = 57 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 157 * 1.333f;
			sSafeYOnMap = 248 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 222 * 1.333f;
			sSafeYOnMap = 219 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);

			sSafeXOnMap = 241 * 1.333f;
			sSafeYOnMap = 129 * 1.333f;
			DrawSafeIcon(sX + sSafeXOnMap, sY + sSafeYOnMap);
		}


		if (showpits)
		{
			PutString2(sX + 253 * 1.333f, sY + 183 * 1.333f, "WereWolf", 0, 255, 0);
			PutString2(sX + 124 * 1.333f, sY + 234 * 1.333f, "Rudolph", 0, 255, 0);
			PutString2(sX + 85 * 1.333f, sY + 254 * 1.333f, "Troll", 0, 255, 0);
			PutString2(sX + 28 * 1.333f, sY + 129 * 1.333f, "Orge", 0, 255, 0);
			PutString2(sX + 248 * 1.333f, sY + 193 * 1.333f, "Giant-Plant", 0, 255, 0);
			PutString2(sX + 122 * 1.333f, sY + 29 * 1.333f, "Skeleton", 0, 255, 0);
			PutString2(sX + 152 * 1.333f, sY + 30 * 1.333f, "Zombie", 0, 255, 0);
			PutString2(sX + 39 * 1.333f, sY + 178 * 1.333f, "Clay-Golem", 0, 255, 0);
			PutString2(sX + 42 * 1.333f, sY + 196 * 1.333f, "Stone-Golem", 0, 255, 0);
			PutString2(sX + 44 * 1.333f, sY + 213 * 1.333f, "Ice-Golem", 0, 255, 0);
			PutString2(sX + 148 * 1.333f, sY + 42 * 1.333f, "Orc", 0, 255, 0);
			PutString2(sX + 241 * 1.333f, sY + 211 * 1.333f, "Scorpion", 0, 255, 0);
			PutString2(sX + 139 * 1.333f, sY + 261 * 1.333f, "Unicorn", 0, 255, 0);
			PutString2(sX + 39 * 1.333f, sY + 82 * 1.333f, "Frost", 0, 255, 0);
			PutString2(sX + 50 * 1.333f, sY + 58 * 1.333f, "Hellbound", 0, 255, 0);
		}
	}


	/*if (g_ev.Is(EventID::Shinning) && ShowShin == true || g_ev.Is(EventID::ThePurge) && ShowShin == true && isInMap("purgecity")) //ShowShin / *OPTION PANEL ACA* /
	{
		for (auto & u : m_minimap.list)
		{
			auto * p = &u;

			short shX;
			short shY;

			if (smap == 0)
			{
				shX = p->x * 0.8;
				shY = p->y * 0.8;
			}
			else if (smap == 1)
			{
				shX = p->x * 1.6;
				shY = p->y * 1.6;
			}

			if (m_dwCurTime - p->time > 25000)
				continue;

			int r = 277, g = 46, b = 104;

			DrawShinningPlayerIcon(sX + shX, sY + shY);
		}
	}*/
}

void CGame::DrawTeleportIcon(short sSafeXOnMap, short sSafeYOnMap, const std::string& text)
{
	// Diámetro del círculo
	const int PLAYER_ICON_DIAMETER = 10;

	// Recorrer los píxeles del círculo y establecer el color azul
	for (int x = sSafeXOnMap - PLAYER_ICON_DIAMETER / 2; x < sSafeXOnMap + PLAYER_ICON_DIAMETER / 2; x++)
	{
		for (int y = sSafeYOnMap - PLAYER_ICON_DIAMETER / 2; y < sSafeYOnMap + PLAYER_ICON_DIAMETER / 2; y++)
		{
			// Comprobar si el píxel está dentro del círculo
			if ((x - sSafeXOnMap) * (x - sSafeXOnMap) + (y - sSafeYOnMap) * (y - sSafeYOnMap) <= PLAYER_ICON_DIAMETER * PLAYER_ICON_DIAMETER / 4)
			{
				m_DDraw.PutPixel(x, y, 255, 30, 20); // Azul (R, G, B)
			}
		}
	}

	// Dibujar el texto centrado sobre el círculo
	const int TEXT_WIDTH = 76; // Ajusta según el ancho máximo del texto
	const int TEXT_HEIGHT_OFFSET = 2; // Offset vertical para ajustar la posición del texto

	int textStartX = sSafeXOnMap - TEXT_WIDTH / 2;
	int textStartY = sSafeYOnMap - PLAYER_ICON_DIAMETER / 2 - TEXT_HEIGHT_OFFSET;

	PutAlignedString2(textStartX, textStartX + TEXT_WIDTH, textStartY + 10, (char*)text.c_str(), 255, 30, 20); // Amarillo (R, G, B)
}

void CGame::DrawPlayerIcon(short sPlayerXOnMap, short sPlayerYOnMap)
{
	// Diámetro del círculo
	const int PLAYER_ICON_DIAMETER = 10;

	// Recorrer los píxeles del círculo y establecer el color azul
	for (int x = sPlayerXOnMap - PLAYER_ICON_DIAMETER / 2; x < sPlayerXOnMap + PLAYER_ICON_DIAMETER / 2; x++)
	{
		for (int y = sPlayerYOnMap - PLAYER_ICON_DIAMETER / 2; y < sPlayerYOnMap + PLAYER_ICON_DIAMETER / 2; y++)
		{
			// Comprobar si el píxel está dentro del círculo
			if ((x - sPlayerXOnMap) * (x - sPlayerXOnMap) + (y - sPlayerYOnMap) * (y - sPlayerYOnMap) <= PLAYER_ICON_DIAMETER * PLAYER_ICON_DIAMETER / 4)
			{
				m_DDraw.PutPixel(x, y, 0, 0, 255); // Azul (R, G, B)
			}
		}
	}
}

void CGame::DrawShinningPlayerIcon(short sPlayerXOnMap, short sPlayerYOnMap)
{
	// Diámetro reducido del círculo
	const int PLAYER_ICON_DIAMETER = 6; // Reducido de 10 a 6

	// Recorrer los píxeles del círculo y establecer el color azul
	for (int x = sPlayerXOnMap - PLAYER_ICON_DIAMETER / 2; x < sPlayerXOnMap + PLAYER_ICON_DIAMETER / 2; x++)
	{
		for (int y = sPlayerYOnMap - PLAYER_ICON_DIAMETER / 2; y < sPlayerYOnMap + PLAYER_ICON_DIAMETER / 2; y++)
		{
			// Comprobar si el píxel está dentro del círculo
			if ((x - sPlayerXOnMap) * (x - sPlayerXOnMap) + (y - sPlayerYOnMap) * (y - sPlayerYOnMap) <= PLAYER_ICON_DIAMETER * PLAYER_ICON_DIAMETER / 4)
			{
				m_DDraw.PutPixel(x, y, 227, 46, 104); // red (R, G, B)
			}
		}
	}
}

void CGame::DrawSafeIcon(short sSafeXOnMap, short sSafeYOnMap)
{
	// Diámetro del círculo
	const int PLAYER_ICON_DIAMETER = 10;

	// Recorrer los píxeles del círculo y establecer el color azul
	for (int x = sSafeXOnMap - PLAYER_ICON_DIAMETER / 2; x < sSafeXOnMap + PLAYER_ICON_DIAMETER / 2; x++)
	{
		for (int y = sSafeYOnMap - PLAYER_ICON_DIAMETER / 2; y < sSafeYOnMap + PLAYER_ICON_DIAMETER / 2; y++)
		{
			// Comprobar si el píxel está dentro del círculo
			if ((x - sSafeXOnMap) * (x - sSafeXOnMap) + (y - sSafeYOnMap) * (y - sSafeYOnMap) <= PLAYER_ICON_DIAMETER * PLAYER_ICON_DIAMETER / 4)
			{
				m_DDraw.PutPixel(x, y, 255, 255, 0); // Azul (R, G, B)
			}
		}
	}
}

void CGame::click_map(short msX, short msY)
{
	int dnumber = 81;
	short sX, sY, szX, szY;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	szX = m_stDialogBoxInfo[dnumber].sSizeX;
	szY = m_stDialogBoxInfo[dnumber].sSizeY;

	if ((msX >= sX + 30) && (msX <= sX + 106) && (msY >= sY + 437) && (msY <= sY + 437 + 12))
	{
		showpits = !showpits;
		PlaySound('E', 14, 5);
		return;
	}

	if ((msX >= sX + 173) && (msX <= sX + 285) && (msY >= sY + 437) && (msY <= sY + 437 + 12))
	{
		showteleports = !showteleports;
		PlaySound('E', 14, 5);
		return;
	}

	if ((msX >= sX + szX - 113) && (msX <= sX + szX - 113 + 75) && (msY >= sY + 437) && (msY <= sY + 437 + 12))
	{
		showsafes = !showsafes;
		PlaySound('E', 14, 5);
		return;
	}
}

void CGame::ReproducirMP3(const std::string& nombreArchivo) {
	std::string comando;

	// Cerrar cualquier archivo MP3 abierto previamente
	mciSendString("close mp3", NULL, 0, NULL);

	// Abrir el archivo MP3 (sin especificar el tipo)
	comando = "open \"" + nombreArchivo + "\" type mpegaudio alias mp3";
	if (mciSendString(comando.c_str(), NULL, 0, NULL) != 0) {
		char txt[100];
		wsprintf(txt, "Error al abrir el archivo: %s", nombreArchivo.c_str());
		AddEventList(txt, 10);
		return;
	}

	// Reproducir el archivo MP3
	comando = "play mp3";
	if (mciSendString(comando.c_str(), NULL, 0, NULL) != 0) {
		char txt[100];
		wsprintf(txt, "Error al reproducir el archivo: %s", nombreArchivo.c_str());
		AddEventList(txt, 10);
		return;
	}

	AddEventList("Reproduciendo archivo MP3...", 10);
}

void CGame::draw_selectonteleport(short msX, short msY, char cLB)
{
	int dnumber = 82;
	short sX, sY, szX, szY;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	szX = m_stDialogBoxInfo[dnumber].sSizeX;
	szY = m_stDialogBoxInfo[dnumber].sSizeY;

	DrawShadowDialog2(sX, sY, dnumber);

	if (g_openmode == 1)
	{
		PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Teleport Gungame", 255, 255, 0);

		// Mensajes en amarillo
		PutString2(sX + 35, sY + 50 - 17, "Please choose the combat mode", 255, 255, 0);
		PutString2(sX + 35, sY + 120 - 17, "Please choose the angel", 255, 255, 0);

		// Dibujar opciones de Combat Type (con aún menos espacio)
		PutString2(sX + 35, sY + 50, "Warrior", g_combattype == 1 ? 0 : 255, 255, g_combattype == 1 ? 0 : 255);
		PutString2(sX + 35, sY + 65, "Mage", g_combattype == 2 ? 0 : 255, 255, g_combattype == 2 ? 0 : 255);

		// Dibujar opciones de Angelic (con aún menos espacio)
		const char* angelics[] = { "AngelicPendant(STR)", "AngelicPendant(DEX)", "AngelicPendant(INT)", "AngelicPendant(MAG)" };
		for (int i = 0; i < 4; i++) {
			PutString2(sX + 35, sY + 120 + (i * 20), (char*)angelics[i], g_angelic == i + 1 ? 0 : 255, 255, g_angelic == i + 1 ? 0 : 255);
		}

		// Botón de confirmación
		DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Teleport");
	}
	else if (g_openmode == 2)
	{
		PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Teleport Battle Royale", 255, 255, 0);

		// Mensajes en amarillo
		PutString2(sX + 35, sY + 50 - 17, "Please choose the combat mode", 255, 255, 0);
		PutString2(sX + 35, sY + 120 - 17, "Please choose the angel", 255, 255, 0);

		// Dibujar opciones de Combat Type (con aún menos espacio)
		PutString2(sX + 35, sY + 50, "Warrior", g_combattype == 1 ? 0 : 255, 255, g_combattype == 1 ? 0 : 255);
		PutString2(sX + 35, sY + 65, "Mage", g_combattype == 2 ? 0 : 255, 255, g_combattype == 2 ? 0 : 255);

		// Dibujar opciones de Angelic (con aún menos espacio)
		const char* angelics[] = { "AngelicPendant(STR)", "AngelicPendant(DEX)", "AngelicPendant(INT)", "AngelicPendant(MAG)" };
		for (int i = 0; i < 4; i++) {
			PutString2(sX + 35, sY + 120 + (i * 20), (char*)angelics[i], g_angelic == i + 1 ? 0 : 255, 255, g_angelic == i + 1 ? 0 : 255);
		}

		// Botón de confirmación
		DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Teleport");
	}
	else if (g_openmode == 3)
	{
		PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Teleport Eternal Balance", 255, 255, 0);

		// Mensajes en amarillo
		PutString2(sX + 35, sY + 50 - 17, "Please choose the combat mode", 255, 255, 0);
		PutString2(sX + 35, sY + 120 - 17, "Please choose the angel", 255, 255, 0);

		// Dibujar opciones de Combat Type (con aún menos espacio)
		PutString2(sX + 35, sY + 50, "Warrior", g_combattype == 1 ? 0 : 255, 255, g_combattype == 1 ? 0 : 255);
		PutString2(sX + 35, sY + 65, "Mage", g_combattype == 2 ? 0 : 255, 255, g_combattype == 2 ? 0 : 255);

		// Dibujar opciones de Angelic (con aún menos espacio)
		const char* angelics[] = { "AngelicPendant(STR)", "AngelicPendant(DEX)", "AngelicPendant(INT)", "AngelicPendant(MAG)" };
		for (int i = 0; i < 4; i++) {
			PutString2(sX + 35, sY + 120 + (i * 20), (char*)angelics[i], g_angelic == i + 1 ? 0 : 255, 255, g_angelic == i + 1 ? 0 : 255);
		}

		// Botón de confirmación
		DrawMenu_SmallButton(sX, sY, msX, msY, 25, 296, "Teleport");
	}
	else return;
	
}

void CGame::click_selectonteleport(short msX, short msY)
{
	int dnumber = 82;
	short sX, sY, szX, szY;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	szX = m_stDialogBoxInfo[dnumber].sSizeX;
	szY = m_stDialogBoxInfo[dnumber].sSizeY;

	// Manejar selección de Combat Type
	if ((msX >= sX + 30) && (msX <= sX + 130) && (msY >= sY + 50) && (msY <= sY + 60)) {
		g_combattype = 1; // Warrior
		PlaySound('E', 14, 5);
		return;
	}
	if ((msX >= sX + 30) && (msX <= sX + 130) && (msY >= sY + 65) && (msY <= sY + 75)) {
		g_combattype = 2; // Mage
		PlaySound('E', 14, 5);
		return;
	}

	// Manejar selección de Angelic
	for (int i = 0; i < 4; i++) {
		if ((msX >= sX + 30) && (msX <= sX + 230) && (msY >= sY + 120 + (i * 20)) && (msY <= sY + 140 + (i * 20))) {
			g_angelic = i + 1;
			PlaySound('E', 14, 5);
			return;
		}
	}

	// Confirmación al hacer clic en el botón "Teleport"
	if ((msX >= sX + 25) && (msX <= sX + 25 + DEF_BTNSZX) && (msY >= sY + DEF_BTNPOSY) && (msY <= sY + DEF_BTNPOSY + DEF_BTNSZY))
	{
		if (g_combattype != 0 && g_angelic != 0) {
			char data[100];
			auto* cp = data;

			if (g_openmode == 1)
			{
				Push(cp, (u32)NOTIFY_GUNGAMESELECTOR);
			}
			else if (g_openmode == 2)
			{
				Push(cp, (u32)NOTIFY_BATTLESELECTOR);
			}
			else if (g_openmode == 3)
			{
				Push(cp, (u32)NOTIFY_DKSELECTOR);
			}
			else return;

			Push(cp, (u16)0);


			Push(cp, g_combattype);
			Push(cp, g_angelic);

			m_pGSock->iSendMsg(data, cp - data);
			PlaySound('E', 14, 5);
		}
	}
}

void CGame::get_ctrstatus(char * data)
{
	Pop(data, m_sRelicStatus);
	Pop(data, m_sElvineRelics);
	Pop(data, m_sAresdenRelics);
}


void CGame::get_ctrplayerposition(char * data)
{
	Pop(data, m_sPlayerRelicX);
	Pop(data, m_sPlayerRelicY);
}

void CGame::get_ctrfloorposition(char * data)
{
	Pop(data, m_sFloorRelicX);
	Pop(data, m_sFloorRelicY);
}

void CGame::get_gungameleader(char * data)
{
	ZeroMemory(m_cGungameLeaders, sizeof(m_cGungameLeaders));
	Pop(data, m_cGungameLeaders, 240);
}

void CGame::clearTopFive()
{
	g_topPlayers.clear();
}

void CGame::ProcessTop5Message(char* message) {
	// Limpiar el vector antes de agregar nuevos datos
	g_topPlayers.clear();

	// Extraer el tamaño del mensaje (si es necesario)
	short messageSize;
	Pop(message, messageSize);

	// Extraer los datos del top five
	for (int i = 0; i < messageSize; i++) {
		TopPlayerInfo player;

		// Extraer la posición en el ranking
		Pop(message, player.rank);

		// Extraer el nombre del jugador
		Pop(message, player.name, 12);

		// Extraer la cantidad de kills
		Pop(message, player.kills);

		// Agregar el jugador al vector
		g_topPlayers.push_back(player);
	}
}
void CGame::DisplayTop5()
{
	if (!isInMap("dk")) return;

	int resX = 640;
	int resY = 480;

	// Determinar la resolución actual
	if (c_reso->IsResolution() == C800x600)
	{
		resX = 800;
		resY = 600;
	}
	else if (c_reso->IsResolution() == C1024x768) // Asumiendo que existe esta constante
	{
		resX = 1024;
		resY = 768;
	}

	// Coordenadas iniciales para mostrar el texto (ajustadas para 1024x768)
	int startX = resX - 220;  // Posición X inicial (más a la izquierda para pantallas anchas)
	int startY = 200;         // Posición Y inicial (un poco más abajo para mejor equilibrio)
	int lineHeight = 22;      // Espacio entre líneas (ligeramente mayor)

	// Color del texto principal (amarillo brillante)
	int textR = 255, textG = 255, textB = 0;
	int text2R = 255, text2G = 255, text2B = 255;

	// Color de la sombra (negro)
	int shadowR = 0, shadowG = 0, shadowB = 0;

	// Mostrar el título del ranking con sombra
	PutString2(startX + 1, startY + 1, "=== TOP 5 JUGADORES ===", shadowR, shadowG, shadowB); // Sombra
	PutString2(startX, startY, "=== TOP 5 JUGADORES ===", textR, textG, textB); // Texto principal

	// Verificar si hay datos en el vector
	if (g_topPlayers.empty()) {
		return;
	}

	// Mostrar el top five
	for (size_t i = 0; i < g_topPlayers.size(); i++) {
		const auto& player = g_topPlayers[i];

		// Crear el texto a mostrar
		char playerInfo[128];
		sprintf(playerInfo, "%d. %s - %d kills", player.rank, player.name, player.kills);

		// Dibujar el texto con sombra
		PutString2(startX + 1, startY + (i + 1) * lineHeight + 1, playerInfo, shadowR, shadowG, shadowB); // Sombra
		PutString2(startX, startY + (i + 1) * lineHeight, playerInfo, text2R, text2G, text2B); // Texto principal
	}
}

void CGame::getNewsContents()
{
	NewContents.clear();
	// Ruta al archivo
	std::string filePath = "CONTENTS\\News.txt";

	// Abre el archivo
	std::ifstream file(filePath);

	// Verifica si el archivo se abrió correctamente
	if (!file.is_open()) {
		std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
		return;
	}

	// Lee el contenido del archivo en un string
	std::stringstream buffer;
	buffer << file.rdbuf();
	NewContents = buffer.str();

	// Cierra el archivo
	file.close();
}

void CGame::getDropListContents()
{
	DropContents.clear();
	// Ruta al archivo
	std::string filePath = "CONTENTS\\DropList.txt";

	// Abre el archivo
	std::ifstream file(filePath);

	// Verifica si el archivo se abrió correctamente
	if (!file.is_open()) {
		
		return;
	}

	// Lee el contenido del archivo en un string
	std::stringstream buffer;
	buffer << file.rdbuf();
	DropContents = buffer.str();

	// Cierra el archivo
	file.close();
}


void CGame::DrawDropList(short sX, short sY, short msX, short msY, short msZ, char cLB)
{
	AddEventList("!asdasdasdasd.");
	int i, iTotalLines, iPointerLoc, iMaxLines, FinScroll;
	double d1, d2, d3;
	
	if (DropContents.empty()) {
		AddEventList("DropContents está vacío. No hay nada que dibujar.");
		return;
	}
	else
	{
		AddEventList((char*)DropContents.c_str(), 10);
	}

	iMaxLines = 23;

	// Contar las líneas en DropContents
	iTotalLines = 0;
	size_t pos = 0;
	while ((pos = DropContents.find('<', pos)) != string::npos) {
		iTotalLines++;
		pos++;
	}
	iTotalLines++; // La última línea no tiene '<'

	if (iGetTopDialogBoxIndex() == 52 && msZ != 0)
	{
		m_stDialogBoxInfo[52].sView = m_stDialogBoxInfo[52].sView - msZ / 60;
		m_DInput.m_sZ = 0;
	}
	if (m_stDialogBoxInfo[52].sView < 0) m_stDialogBoxInfo[52].sView = 0;
	if (iTotalLines > iMaxLines && m_stDialogBoxInfo[52].sView > iTotalLines - iMaxLines) m_stDialogBoxInfo[52].sView = iTotalLines - iMaxLines;
	if (iTotalLines > iMaxLines) {
		d1 = (double)m_stDialogBoxInfo[52].sView;
		d2 = (double)(iTotalLines - iMaxLines);
		d3 = (317.0f * d1) / d2;
		iPointerLoc = (int)(d3 + 0.5);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 232, sY - 20 + iPointerLoc, 7);
	}
	else iPointerLoc = 0;

	// Dividir el contenido en líneas usando '<' como separador
	vector<string> lines;
	size_t start = 0;
	pos = 0;
	while ((pos = DropContents.find('<', start)) != string::npos) {
		lines.push_back(DropContents.substr(start, pos - start));
		start = pos + 1;
	}
	lines.push_back(DropContents.substr(start)); // Última línea

	// Mostrar las líneas en el panel
	for (i = 0; i < iMaxLines; i++)
	{
		if (i + m_stDialogBoxInfo[52].sView < lines.size())
		{
			string line = lines[i + m_stDialogBoxInfo[52].sView];
			if (!line.empty())
			{
				switch (line[0])
				{
					// Blanco
				case '-': PutAlignedString(sX, sX + 235, sY + i * 13, (char*)line.substr(1).c_str(), 255, 255, 255); break;
					// Amarillo
				case ';': PutAlignedString(sX, sX + 235, sY + i * 13, (char*)line.substr(1).c_str(), 244, 173, 1); break;
					// Rojo
				case '@': PutAlignedString(sX, sX + 235, sY + i * 13, (char*)line.substr(1).c_str(), 218, 43, 43); break;
					// Azul
				case '&': PutAlignedString(sX, sX + 235, sY + i * 13, (char*)line.substr(1).c_str(), 23, 132, 163); break;
					// Verde
				case '$': PutAlignedString(sX, sX + 235, sY + i * 13, (char*)line.substr(1).c_str(), 92, 163, 23); break;
					// Marrón
				case '<': PutAlignedString(sX, sX + 235, sY + i * 13, (char*)line.substr(1).c_str(), 161, 120, 66); break;
					// Por defecto (gris)
				default: PutAlignedString(sX, sX + 235, sY + i * 13, (char*)line.c_str(), 203, 197, 197); break;
				}
			}
		}
	}

	if (cLB != 0 && iTotalLines > iMaxLines) {
		if ((iGetTopDialogBoxIndex() == 52)) {
			FinScroll = 304;
			if ((msX >= sX + 225) && (msX <= sX + 239) && (msY >= sY) && (msY <= sY + FinScroll)) {
				d1 = (double)(msY - (sY + 98));
				d2 = (double)(iTotalLines - iMaxLines);
				d3 = (d1 * d2) / 321.0f;
				iPointerLoc = (int)d3;
				if (iPointerLoc > iTotalLines - iMaxLines) iPointerLoc = iTotalLines - iMaxLines;
				m_stDialogBoxInfo[52].sView = iPointerLoc;
			}
		}
	}
	else m_stDialogBoxInfo[52].bIsScrollSelected = FALSE;
}

void CGame::getEventStatusBattleRoyale(char * data)
{
	Pop(data, m_sBattleRoyaleStatus);
}

void CGame::getRegisterTimeBattleRoyale(char * data)
{
	Pop(data, m_iRegisterTime);
}

void CGame::getFreezeTimeBattleRoyale(char * data)
{
	Pop(data, m_iFreezeTime);
}

void CGame::getRoundTimeBattleRoyale(char * data)
{
	Pop(data, m_iRoundTime);
}

void CGame::getBattleRoyaleRound(char * data)
{
	Pop(data, m_sBattleRoyaleRound);
}

void CGame::getSafeTimeBattleRoyale(char * data)
{
	Pop(data, m_iSafeTime);
}

void CGame::draw_masterydialog(short msX, short msY, char cLB) {
	int dnumber = 83;
	short sX = m_stDialogBoxInfo[dnumber].sX;
	short sY = m_stDialogBoxInfo[dnumber].sY;
	short szX = m_stDialogBoxInfo[dnumber].sSizeX;
	short szY = m_stDialogBoxInfo[dnumber].sSizeY;

	DrawShadowDialog2(sX, sY, dnumber);

	switch (m_stDialogBoxInfo[dnumber].cMode) {
	case 0: drawMainMenu(sX, sY, szX, szY, msX, msY); break;
	case 1: drawUpgradeMenu(sX, sY, szX, szY, msX, msY); break;
	case 2: drawActivateMenu(sX, sY, szX, szY, msX, msY); break;
	case 3: drawUpgradeConfirmation(sX, sY, szX, szY, msX, msY); break;
	case 4: drawActivateConfirmation(sX, sY, szX, szY, msX, msY); break;
	case 5: drawMasteryInfo(sX, sY, szX, szY, msX, msY); break; // Nuevo modo para mostrar información
	default: break;
	}
}

void CGame::drawMasteryInfo(short sX, short sY, short szX, short szY, short msX, short msY) {
	PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Mastery Information", 255, 255, 0);

	const char* masteryNames[] = {
		"Npc Damage", "Regeneration", "Vitality", "Defense"
	};

	const char* masteryDescriptions[] = {
		"Increases damage against NPCs by 10% points per level.",
		"Reduces regeneration time by 2 seconds per level (Lvl1 base: 15s).",
		"Increases vitality by 10% per level.",
		"Increases dr and mr by 25 points per level."
	};

	short levels[] = { m_sNpcDamageLevel, m_sRegenLevel, m_sVitLevel, m_sDefenseLevel };

	for (int i = 0; i < 4; ++i) {
		wsprintf(G_cTxt, "%s (Level %d): %s", masteryNames[i], levels[i], masteryDescriptions[i]);
		PutAlignedString(sX + 20, sX + szX - 20, sY + 45 + i * 55, G_cTxt, 255, 255, 255);
	}

	drawBackButton(sX, sY, msX, msY);
}
	
void CGame::drawMainMenu(short sX, short sY, short szX, short szY, short msX, short msY) {
	PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Mastery System", 255, 255, 0);

	drawClickableText(sX, sY + 55, szX, 15, msX, msY, "Upgrade mastery levels");
	drawClickableText(sX, sY + 80, szX, 15, msX, msY, "Select active mastery");
	drawClickableText(sX, sY + 105, szX, 15, msX, msY, "View mastery information"); // Nueva opción
}

void CGame::drawUpgradeMenu(short sX, short sY, short szX, short szY, short msX, short msY) {
	PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Upgrade Mastery", 255, 255, 0);

	const char* options[] = {
		"Npc Damage (Current Level %d)",
		"Regeneration (Current Level %d)", "Vitality (Current Level %d)",
		"Defense (Current Level %d)"
	};
	short levels[] = { m_sNpcDamageLevel, m_sRegenLevel, m_sVitLevel, m_sDefenseLevel };

	for (int i = 0; i < 4; ++i) {
		wsprintf(G_cTxt, options[i], levels[i]);
		drawClickableText(sX, sY + 40 + i * 20, szX, 15, msX, msY, G_cTxt);
	}

	drawBackButton(sX, sY, msX, msY);
}

void CGame::drawActivateMenu(short sX, short sY, short szX, short szY, short msX, short msY) {
	PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Activate Mastery", 255, 255, 0);

	const char* options[] = {
		"Npc Damage (Level %d)",
		"Regeneration (Level %d)", "Vitality (Level %d)",
		"Defense (Level %d)"
	};
	short levels[] = { m_sNpcDamageLevel, m_sRegenLevel, m_sVitLevel, m_sDefenseLevel };

	for (int i = 0; i < 4; ++i) {
		wsprintf(G_cTxt, options[i], levels[i]);
		drawClickableText(sX, sY + 40 + i * 20, szX, 15, msX, msY, G_cTxt);
	}

	drawBackButton(sX, sY, msX, msY);
}

void CGame::drawUpgradeConfirmation(short sX, short sY, short szX, short szY, short msX, short msY) {
	PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Confirm Upgrade", 255, 255, 0);

	wsprintf(G_cTxt, "Tokens: %d", m_iTokens);
	PutAlignedString(sX, sX + szX, sY + 25, G_cTxt, 255, 255, 255);

	const char* options[] = {
		"NPC Damage", "Regeneration", "Vitality", "Defense"
	};
	short levels[] = { m_sNpcDamageLevel, m_sRegenLevel, m_sVitLevel, m_sDefenseLevel };

	PutAlignedString(sX, sX + szX, sY + 45, (char*)options[selectedUpgradeOption - 1], 255, 255, 0);
	wsprintf(G_cTxt, "Current Level: %d", levels[selectedUpgradeOption - 1]);
	PutAlignedString(sX, sX + szX, sY + 65, G_cTxt, 255, 255, 0);
	wsprintf(G_cTxt, "Next Level: %d", levels[selectedUpgradeOption - 1] + 1);
	PutAlignedString(sX, sX + szX, sY + 85, G_cTxt, 255, 255, 0);

	int masteryTokenCost = (levels[selectedUpgradeOption - 1] + 1) * 200;
	wsprintf(G_cTxt, "Cost: %d Tokens", masteryTokenCost);
	PutAlignedString(sX, sX + szX, sY + 105, G_cTxt, 255, 0, 0);

	DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY, "Upgrade");
	DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Cancel");
}

void CGame::drawActivateConfirmation(short sX, short sY, short szX, short szY, short msX, short msY) {
	PutCenterString_SpriteFont(sX, sX + szX, sY + 3, "Confirm Activated Mastery", 255, 255, 0);

	wsprintf(G_cTxt, "Tokens: %d", m_iTokens);
	PutAlignedString(sX, sX + szX, sY + 25, G_cTxt, 255, 255, 255);

	const char* options[] = { "NPC Damage", "Regeneration", "Vitality", "Defense" };
	PutAlignedString(sX, sX + szX, sY + 45, (char*)options[selectedMastery - 1], 255, 255, 0);

	int activationCost = 50;
	wsprintf(G_cTxt, "Cost: %d Tokens", activationCost);
	PutAlignedString(sX, sX + szX, sY + 65, G_cTxt, 255, 0, 0);

	DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY, "Active");
	DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Cancel");
}

void CGame::drawClickableText(short sX, short sY, short width, short height, short msX, short msY, const char* text) {
	if (msX >= sX && msX <= sX + width && msY >= sY && msY <= sY + height) {
		PutAlignedString(sX, sX + width, sY, (char*)text, 255, 255, 0);
	}
	else {
		PutAlignedString(sX, sX + width, sY, (char*)text, 240, 240, 240);
	}
}

void CGame::drawBackButton(short sX, short sY, short msX, short msY) {
	if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290) && (msY < sY + 310)) {
		PutString_SprFont2(sX + 195, sY + 290, "Back", 255, 255, 255);
	}
	else {
		PutString_SprFont2(sX + 195, sY + 290, "Back", 255, 255, 0);
	}
}

void CGame::click_masterydialog(short msX, short msY) {
	int dnumber = 83;
	short sX = m_stDialogBoxInfo[dnumber].sX;
	short sY = m_stDialogBoxInfo[dnumber].sY;
	short szX = m_stDialogBoxInfo[dnumber].sSizeX;
	short szY = m_stDialogBoxInfo[dnumber].sSizeY;

	switch (m_stDialogBoxInfo[dnumber].cMode) {
	case 0: handleMainMenuClick(sX, sY, szX, szY, msX, msY); break;
	case 1: handleUpgradeMenuClick(sX, sY, szX, szY, msX, msY); break;
	case 2: handleActivateMenuClick(sX, sY, szX, szY, msX, msY); break;
	case 3: handleUpgradeConfirmationClick(sX, sY, msX, msY); break;
	case 4: handleActivateConfirmationClick(sX, sY, msX, msY); break;
	case 5: handleMasteryInfoClick(sX, sY, msX, msY); break; // Manejo del clic en la pantalla de información
	default: break;
	}
}
void CGame::handleMainMenuClick(short sX, short sY, short szX, short szY, short msX, short msY) {
	if (isClicked(sX, sY + 55, szX, 15, msX, msY)) {
		PlaySound('E', 14, 5);
		m_stDialogBoxInfo[83].cMode = 1;
	}
	else if (isClicked(sX, sY + 80, szX, 15, msX, msY)) {
		PlaySound('E', 14, 5);
		m_stDialogBoxInfo[83].cMode = 2;
	}
	else if (isClicked(sX, sY + 105, szX, 15, msX, msY)) { // Nueva opción
		PlaySound('E', 14, 5);
		m_stDialogBoxInfo[83].cMode = 5;
	}
}
void CGame::handleUpgradeMenuClick(short sX, short sY, short szX, short szY, short msX, short msY) {
	for (int i = 0; i < 4; ++i) {
		if (isClicked(sX, sY + 40 + i * 20, szX, 15, msX, msY)) {
			selectedUpgradeOption = i + 1;
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[83].cMode = 3;
			return;
		}
	}

	if (isClicked(sX + 190, sY + 290, 50, 20, msX, msY)) {
		PlaySound('E', 14, 5);
		m_stDialogBoxInfo[83].cMode = 0;
	}
}

void CGame::handleActivateMenuClick(short sX, short sY, short szX, short szY, short msX, short msY) {
	for (int i = 0; i < 4; ++i) {
		if (isClicked(sX, sY + 40 + i * 20, szX, 15, msX, msY)) {
			selectedMastery = i + 1;
			PlaySound('E', 14, 5);
			m_stDialogBoxInfo[83].cMode = 4;
			return;
		}
	}

	if (isClicked(sX + 190, sY + 290, 50, 20, msX, msY)) {
		PlaySound('E', 14, 5);
		m_stDialogBoxInfo[83].cMode = 0;
	}
}

void CGame::handleUpgradeConfirmationClick(short sX, short sY, short msX, short msY) {
	if (isClicked(sX + 30, sY + 292, 76, 22, msX, msY)) {
		requestUpgradeMastery();
		PlaySound('E', 14, 5);
		m_stDialogBoxInfo[83].cMode = 1;
	}
	else if (isClicked(sX + 154, sY + 292, 76, 22, msX, msY)) {
		PlaySound('E', 14, 5);
		m_stDialogBoxInfo[83].cMode = 1;
	}
}

void CGame::handleActivateConfirmationClick(short sX, short sY, short msX, short msY) {
	if (isClicked(sX + 30, sY + 292, 76, 22, msX, msY)) {
		requestActivateMastery();
		PlaySound('E', 14, 5);
		m_stDialogBoxInfo[83].cMode = 2;
	}
	else if (isClicked(sX + 154, sY + 292, 76, 22, msX, msY)) {
		PlaySound('E', 14, 5);
		m_stDialogBoxInfo[83].cMode = 2;
	}
}

void CGame::handleMasteryInfoClick(short sX, short sY, short msX, short msY) {
	// Verificar si se hizo clic en el botón "Back"
	if (isClicked(sX + 190, sY + 290, 50, 20, msX, msY)) {
		PlaySound('E', 14, 5); // Reproducir sonido de clic
		m_stDialogBoxInfo[83].cMode = 0; // Volver al menú principal
	}
}

bool CGame::isClicked(short x, short y, short width, short height, short msX, short msY) {
	return msX >= x && msX <= x + width && msY >= y && msY <= y + height;
}

void CGame::requestUpgradeMastery() {
	char data[256];
	auto* cp = data;

	Push(cp, (u32)REQ_UPGRADE_MASTERY);
	Push(cp, (u16)0);
	Push(cp, selectedUpgradeOption);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::requestActivateMastery() {
	char data[256];
	auto* cp = data;

	Push(cp, (u32)REQ_ACTIVATE_MASTERY);
	Push(cp, (u16)0);
	Push(cp, selectedMastery);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::DrawDialogBox_DropItems(short msX, short msY, short msZ, char cLB)
{
	auto sX = m_stDialogBoxInfo[84].sX;
	auto sY = m_stDialogBoxInfo[84].sY;
	auto szx = m_stDialogBoxInfo[84].sSizeX;
	int r = 255, g = 255, b = 255; // Color blanco por defecto

	// Dibujar el fondo del diálogo
	DrawShadowDialog2(sX, sY, 84);

	PutCenterString_SpriteFont(sX, sX + szx, sY + 3, "Dropped Items", 0, 0, 0);
	PutCenterString_SpriteFont(sX, sX + szx, sY + 4, "Dropped Items", 250, 250, 250);

	// Dibujar la lista de ítems
	int iTotalLines = static_cast<int>(m_vDropItemList.size());
	int maxlines = 13; // Máximo de líneas que caben en el diálogo

	if (iTotalLines > maxlines)
	{
		double d1 = static_cast<double>(m_stDialogBoxInfo[84].sView);
		double d2 = static_cast<double>(iTotalLines - maxlines);
		double d3 = (275.0f * d1) / d2;
		int iPointerLoc = static_cast<int>(d3 + 0.5);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + szx - 13, sY + 44 + iPointerLoc, 7);
	}

	PutString2(sX + 10, sY + 75 - 20, "Item", 255, 200, 0);
	PutString2(sX + m_stDialogBoxInfo[84].sSizeX - 60, sY + 75 - 20, "Count", 255, 200, 0);

	// Dibujar los nombres de los ítems
	for (int i = 0; i < maxlines; i++)
	{
		if ((i + m_stDialogBoxInfo[84].sView) < iTotalLines)
		{
			char cTemp[512] = {};
			strcpy(cTemp, m_vDropItemList[i + m_stDialogBoxInfo[84].sView].m_cItemName);
			char cTemp2[512] = {};
			strcpy(cTemp2, (char*)to_string(m_vDropItemList[i + m_stDialogBoxInfo[84].sView].itemcount).c_str());

			if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
			{
				PutString2(sX + 10, sY + i * 18 + 75, cTemp, 255, 200, 0); // Resaltar el ítem seleccionado
				PutString2(sX + m_stDialogBoxInfo[84].sSizeX - 60, sY + i * 18 + 75, cTemp2, 255, 200, 0); // Resaltar el ítem seleccionado
			}
			else
			{
				PutString2(sX + 10, sY + i * 18 + 75, cTemp, r, g, b);
				PutString2(sX + m_stDialogBoxInfo[84].sSizeX - 60,  sY + i * 18 + 75, cTemp2, r, g, b);
			}
		}
	}

	if (iTotalLines > maxlines)
	{
		// Manejo del scroll del mouse
		if (iGetTopDialogBoxIndex() == 84 && msZ != 0)
		{
			m_stDialogBoxInfo[84].sView -= msZ / 120;
			if (m_stDialogBoxInfo[84].sView < 0)
			{
				m_stDialogBoxInfo[84].sView = 0;
			}
			if (m_stDialogBoxInfo[84].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[84].sView = iTotalLines - maxlines;
			}
			m_DInput.m_sZ = 0; // Reiniciar el valor del scroll
		}
	}
	
	if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + 60) && (msY < sY + 310 + 60))
		PutString_SprFont2(sX + 195, sY + 290 + 60, "Close", 250, 250, 250);
	else
		PutString_SprFont2(sX + 195, sY + 290 + 60, "Close", 255, 200, 0);
	
	DrawDialogCord(sX, sY, msX, msY);
}

void CGame::DlgBoxClick_DropItems(short msX, short msY)
{
	auto sX = m_stDialogBoxInfo[84].sX;
	auto sY = m_stDialogBoxInfo[84].sY;
	auto szx = m_stDialogBoxInfo[84].sSizeX;

	// Obtener el tiempo actual
	DWORD dwCurrentTime = GetTickCount();

	// Manejar el clic en el botón de cerrar
	if ((msX > sX + 190) && (msX < sX + 240) && (msY > sY + 290 + 60) && (msY < sY + 310 + 60))
	{
		PlaySound('E', 14, 5);
		DisableDialogBox(84);
		return;
	}

	// Manejar el clic en un ítem de la lista
	for (int i = 0; i < 13; i++)
	{
		if ((msX >= sX + 20) && (msX <= sX + 220) && (msY >= sY + i * 18 + 75) && (msY <= sY + i * 18 + 89))
		{
			// Evitar clicks más rápidos de 1.5 segundos
			if (dwCurrentTime - m_dwLastDropClickTime < 1500)
			{
				AddEventList("Solo puedes extraer 1 item cada 1.5 segundos.");
				return; // Ignorar el clic si es demasiado rápido
			}

			int itemIndex = i + m_stDialogBoxInfo[84].sView; // Índice real en el vector

			if (itemIndex < m_vDropItemList.size())
			{
				// Obtener el ítem seleccionado usando el índice calculado
				auto selectedItem = &m_vDropItemList[itemIndex];				

				withdrawDropItem(itemIndex);

				PlaySound('E', 14, 5);
				return;
			}			
		}		
	}
}

void CGame::withdrawDropItem(int itemIndex)
{
	char data[12];
	auto* cp = data;

	Push(cp, (u32)REQ_WITHDRAW_DROPITEM);
	Push(cp, (u16)0);
	Push(cp, itemIndex);

	m_pGSock->iSendMsg(data, cp - data);
}

string CGame::obtenerHoraActual() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	std::ostringstream oss;
	oss << setfill('0') << std::setw(2) << now->tm_hour << ":"
		<< std::setfill('0') << std::setw(2) << now->tm_min << ":"
		<< std::setfill('0') << std::setw(2) << now->tm_sec;

	return oss.str();
}



void CGame::getAddedStats(char * pData)
{
	Pop(pData, m_iAddedPA);
	Pop(pData, m_iAddedMA);
	Pop(pData, m_iAddedMR);
	Pop(pData, m_iAddedDR);
	Pop(pData, m_iAddedPhysDmg);
	Pop(pData, m_iAddedPhysHit);
	Pop(pData, m_iAddedMagDmg);
	Pop(pData, m_iAddedMagHit);
	Pop(pData, m_iAddedMS);
}
void CGame::getDropLog(char * pData)
{
	short dropmode;
	char cTxtMsg[200];

	Pop(pData, dropmode);
	Pop(pData, cTxtMsg, 200);

	if (m_vDropLogList.size() >= 500)
	{
		m_vDropLogList.erase(m_vDropLogList.begin()); 
	}

	st_droplog u;
	u.time = obtenerHoraActual();
	u.dropmode = dropmode;
	u.textlog = cTxtMsg;

	AddEventList((char*)u.textlog.c_str(), 10);

	m_vDropLogList.push_back(u);
}

void CGame::draw_droplog(short msX, short msY, short msZ, char cLB)
{
	auto sX = m_stDialogBoxInfo[85].sX;
	auto sY = m_stDialogBoxInfo[85].sY;
	auto szx = m_stDialogBoxInfo[85].sSizeX;
	int r = 255, g = 255, b = 255; 

	DrawShadowDialog2(sX, sY, 85);

	PutCenterString_SpriteFont(sX, sX + szx, sY + 3, "Manufacture and Drop Logs", 0, 0, 0);
	PutCenterString_SpriteFont(sX, sX + szx, sY + 4, "Manufacture and Drop Logs", 250, 250, 250);

	int iTotalLines = static_cast<int>(m_vDropLogList.size());
	int maxlines = 16;

	if (iTotalLines > maxlines)
	{
		double d1 = static_cast<double>(m_stDialogBoxInfo[85].sView);
		double d2 = static_cast<double>(iTotalLines - maxlines);
		double d3 = (275.0f * d1) / d2;
		int iPointerLoc = static_cast<int>(d3 + 0.5);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + szx - 13, sY + 44 + iPointerLoc, 7);
	}

	for (int i = 0; i < maxlines; i++)
	{
		if ((i + m_stDialogBoxInfo[85].sView) < iTotalLines)
		{
			const auto& log = m_vDropLogList[i + m_stDialogBoxInfo[85].sView];
			string logEntry = log.time + " - " + log.textlog;

			int r = 255, g = 255, b = 255; 
			switch (log.dropmode)
			{
			case drop_common:      r = 200; g = 200; b = 200; break; 
			case drop_lesscommon:  r = 0;   g = 255; b = 0;   break; 
			case drop_rare:        r = 0;   g = 100; b = 255; break; 
			case drop_epic:        r = 255; g = 0;   b = 0; break; 
			case drop_legendary:   r = 255; g = 215; b = 0;   break; 
			}

			PutString2(sX + 10, sY + i * 18 + 40, (char*)logEntry.c_str(), r, g, b);
		}
	}

	if (iTotalLines > maxlines)
	{
		if (iGetTopDialogBoxIndex() == 85 && msZ != 0)
		{
			m_stDialogBoxInfo[85].sView -= msZ / 120;
			if (m_stDialogBoxInfo[85].sView < 0)
			{
				m_stDialogBoxInfo[85].sView = 0;
			}
			if (m_stDialogBoxInfo[85].sView > iTotalLines - maxlines)
			{
				m_stDialogBoxInfo[85].sView = iTotalLines - maxlines;
			}
			m_DInput.m_sZ = 0;
		}
	}
}

void CGame::click_droplog(short msX, short msY)
{
	auto sX = m_stDialogBoxInfo[85].sX;
	auto sY = m_stDialogBoxInfo[85].sY;
	auto szx = m_stDialogBoxInfo[85].sSizeX;

	
}

/*
char data[512];
auto * cp = data;

Push(cp, (u32)ENHANCEMENT_EXTRACT);
Push(cp, (u16)0);

Push(cp, count);

for (int b = 0; b < 20; b++)
{
if (m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b] != -1 && m_pItemList[m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]])
{
Push(cp, m_stDialogBoxInfo[dnumber].m_iEnchantingItem[b]);
}
}

m_pGSock->iSendMsg(data, cp - data);
*/

enum ManufactureMode {
	MANUFACTURE_MODE_SELECT_RECIPE = 0,  // Modo para seleccionar una receta
	MANUFACTURE_MODE_CONFIRM_RECIPE = 1, // Modo para confirmar la receta seleccionada
	MANUFACTURE_MODE_COMPLETE = 2        // Modo para indicar que la fabricación está completa
};

void CGame::HandleItemDropForManufacturing(short mouseX, short mouseY)
{
	int dialogNumber = 86;
	char itemID;
	itemID = (char)m_stMCursor.sSelectedObjectID;

	// Verificar si el ítem está deshabilitado o si no hay un comando válido
	if (m_bIsItemDisabled[itemID] == TRUE) return;
	if (m_cCommand < 0) return;

	switch (m_stDialogBoxInfo[dialogNumber].cMode) {
	case MANUFACTURE_MODE_CONFIRM_RECIPE:
	{
		// Verificar si hay una receta seleccionada válida
		if (m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex < 0 ||
			m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex >= recipes.size())
		{
			AddEventList(m_sLang == LANG_ESP ? "No hay una receta seleccionada." : "No recipe selected.");
			return;
		}

		// Obtener la receta seleccionada
		const Recipe &selectedRecipe = recipes[m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex];

		// Verificar si el ítem está en la lista de ingredientes de la receta
		bool isItemInRecipe = false;
		int requiredCount = 0;
		for (const auto &ingredient : selectedRecipe.ingredients) {
			if (strcmp(ingredient.first.c_str(), m_pItemList[itemID]->m_cName) == 0) {
				isItemInRecipe = true;
				requiredCount = ingredient.second;
				break;
			}
		}

		if (!isItemInRecipe) {
			AddEventList(m_sLang == LANG_ESP ? "Este item no es necesario para la receta." : "This item is not needed for the recipe.");
			return;
		}

		// Verificar si el ítem ya ha sido agregado (por ID)
		for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
			if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] == itemID) {
				AddEventList(m_sLang == LANG_ESP ? "Este item ya ha sido agregado (mismo ID)." : "This item has already been added (same ID).");
				return;
			}
		}

		// Verificar si el ítem ya ha sido agregado (por nombre)
		bool isItemNameAlreadyAdded = false;
		for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
			if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] != -1) {
				auto it = m_pItemList[m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i]];
				if (it != NULL && strcmp(it->m_cName, m_pItemList[itemID]->m_cName) == 0) {
					isItemNameAlreadyAdded = true;
					break;
				}
			}
		}

		if (isItemNameAlreadyAdded) {
			AddEventList(m_sLang == LANG_ESP ? "Este item ya ha sido agregado (mismo tipo)." : "This item has already been added (same type).");
			return;
		}

		// Verificar si la cantidad del ítem es suficiente
		if (m_pItemList[itemID]->m_dwCount < requiredCount) {
			AddEventList(m_sLang == LANG_ESP ? "No tienes suficiente cantidad de este item." : "You don't have enough of this item.");
			return;
		}

		// Verificar si hay espacio en la lista de ítems agregados
		int itemCount = 0;
		for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
			if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] != -1) {
				itemCount++;
			}
		}

		if (itemCount >= MAX_MANUFACTURE_ITEMS) {
			AddEventList(m_sLang == LANG_ESP ? "No hay espacio para mas items." : "There is no space for more items.");
			return;
		}

		// Agregar el ítem a la lista
		for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
			if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] == -1) {
				m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] = itemID;
				m_bIsItemDisabled[itemID] = TRUE;
				AddEventList(m_sLang == LANG_ESP ? "Item agregado a la lista." : "Item added to the list.");
				PlaySound('E', 29, 0);
				return;
			}
		}

		break;
	}
	}
}
void CGame::RenderManufactureDialog(short mouseX, short mouseY, short mouseZ, char leftButton)
{
	short dialogX, dialogY, dialogSizeX, dialogSizeY;
	int dialogNumber = 86;
	int maxLines = 14;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;
	auto & mode = m_stDialogBoxInfo[dialogNumber].cMode;

	DrawShadowDialog2(dialogX, dialogY, dialogNumber);

	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 3, m_sLang == LANG_ESP ? "Manufacture" : "Manufacture", 0, 0, 0);
	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 4, m_sLang == LANG_ESP ? "Manufacture" : "Manufacture", 250, 250, 250);

	DrawNewDialogBox2(SPRID_FLAGSLANG, dialogX + 195 + 80, dialogY + 13, 0, FALSE, true);
	DrawNewDialogBox2(SPRID_FLAGSLANG + 1, dialogX + 195 + 28 + 80, dialogY + 13, 0, FALSE, true);

	switch (mode)
	{
	case MANUFACTURE_MODE_SELECT_RECIPE:
	{
		int totalLines, pointerLocation;
		double d1, d2, d3;  

		PutAlignedString(dialogX, dialogX + dialogSizeX, dialogY + 35, m_sLang == LANG_ESP ? "Selecciona un elemento de la lista para fabricarlo." : "Select an item from the list to craft it.", 228, 161, 6);

		totalLines = recipes.size();

		if (totalLines > maxLines) {
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, dialogX + 100, dialogY, 3);
		}

		if (iGetTopDialogBoxIndex() == dialogNumber && mouseZ != 0) {
			m_stDialogBoxInfo[dialogNumber].sView = m_stDialogBoxInfo[dialogNumber].sView - mouseZ / 60;
			m_DInput.m_sZ = 0;
		}

		if (m_stDialogBoxInfo[dialogNumber].sView < 0) m_stDialogBoxInfo[dialogNumber].sView = 0;
		if (totalLines > maxLines && m_stDialogBoxInfo[dialogNumber].sView > totalLines - maxLines)
			m_stDialogBoxInfo[dialogNumber].sView = totalLines - maxLines;

		if (totalLines > maxLines) {
			d1 = (double)m_stDialogBoxInfo[dialogNumber].sView;
			d2 = (double)(totalLines - maxLines);
			d3 = (274.0f * d1) / d2;
			pointerLocation = (int)(d3 + 0.5);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, dialogX + 242 + 100, dialogY + 35 + pointerLocation, 7);
		}
		else {
			pointerLocation = 0;
		}

		for (int i = 0; i < maxLines; i++) {
			if (i + m_stDialogBoxInfo[dialogNumber].sView < totalLines) {
				auto & recipe = recipes[i + m_stDialogBoxInfo[dialogNumber].sView];
				int y = dialogY + 80 + (15 * i);

				if ((mouseX > dialogX + 50) && (mouseX < dialogX + dialogSizeX - 50) && (mouseY > y) && (mouseY < y + 15)) {
					PutAlignedString(dialogX, dialogX + dialogSizeX, y, (char*)recipe.itemResult.c_str(), 228, 161, 6);
				}
				else {
					PutAlignedString(dialogX, dialogX + dialogSizeX, y, (char*)recipe.itemResult.c_str(), 255, 255, 255);
				}
			}
		}

		if (leftButton != 0 && totalLines > maxLines) {
			if ((iGetTopDialogBoxIndex() == dialogNumber)) {
				if ((mouseX >= dialogX + 334) && (mouseX <= dialogX + 346) && (mouseY >= dialogY + 40) && (mouseY <= dialogY + 320)) {
					d1 = (double)(mouseY - (dialogY + 35));
					d2 = (double)(totalLines - maxLines);
					d3 = (d1 * d2) / 274.0f;
					pointerLocation = (int)d3;
					if (pointerLocation > totalLines - maxLines) pointerLocation = totalLines - maxLines;
					m_stDialogBoxInfo[dialogNumber].sView = pointerLocation;
				}
			}
		}
		else {
			m_stDialogBoxInfo[dialogNumber].bIsScrollSelected = FALSE;
		}

		break;
	}
	case MANUFACTURE_MODE_CONFIRM_RECIPE:
	{
		// Verificar si hay una receta seleccionada válida
		if (m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex >= 0 &&
			m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex < recipes.size())
		{
			// Obtener la receta seleccionada
			const Recipe &selectedRecipe = recipes[m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex];
			int y = dialogY + 50;

			// Formatear el título con wsprintf
			char title[256];
			wsprintf(title, "%s", selectedRecipe.itemResult.c_str());
			PutString2(dialogX + 10, y, title, 228, 161, 6);

			if (selectedRecipe.itemColor != 0)
			{
				// Dibujar el sprite del ítem resultante
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + selectedRecipe.itemSpr]->PutSpriteRGB(
					dialogX + 10 + 130, y + 45, selectedRecipe.itemSprFrame,
					m_wWR[selectedRecipe.itemColor] - m_wR[0],
					m_wWG[selectedRecipe.itemColor] - m_wG[0],
					m_wWB[selectedRecipe.itemColor] - m_wB[0], 0);
			}
			else
			{
				// Dibujar el sprite del ítem resultante
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + selectedRecipe.itemSpr]->PutSpriteFast(
					dialogX + 10 + 130, y + 45, selectedRecipe.itemSprFrame, 0);
			}
			

			y += 20;

			// Formatear el título de los ingredientes
			char ingtitle[256];
			wsprintf(ingtitle, m_sLang == LANG_ESP ? "Ingredientes" : "Ingredients");
			PutString2(dialogX + 10, y, ingtitle, 228, 161, 6);
			y += 20;

			// Mostrar cada ingrediente
			for (const auto &ingredient : selectedRecipe.ingredients) {
				char ingredientText[256];
				bool isIngredientComplete = false;

				// Verificar si el ingrediente está en la lista de ítems ingresados
				for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
					if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] != -1) {
						auto it = m_pItemList[m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i]];
						if (it != NULL && it->m_cName == ingredient.first && it->m_dwCount >= ingredient.second) {
							isIngredientComplete = true;
							break;
						}
					}
				}

				// Dibujar el texto en verde si el ingrediente está completo, de lo contrario en rojo
				if (isIngredientComplete) {
					wsprintf(ingredientText, "%s: %d", ingredient.first.c_str(), ingredient.second);
					PutString2(dialogX + 10, y, ingredientText, 0, 255, 0); // Verde
				}
				else {
					wsprintf(ingredientText, "%s: %d", ingredient.first.c_str(), ingredient.second);
					PutString2(dialogX + 10, y, ingredientText, 255, 0, 0); // Rojo
				}
				y += 15;
			}

			// Dibujar una línea vertical en el centro del diálogo (dialogX + dialogSizeX / 2)
			int midX = dialogX + (dialogSizeX / 2); // Punto medio del diálogo
			for (int i = dialogY + 50; i < dialogY + dialogSizeY - 30; i++) { // Línea comienza en dialogY + 50
				DrawWhiteLine(midX, i, midX, i + 1, 255, 255, 255); // Línea gris
			}

			// Mostrar los ítems ingresados en el lado derecho
			y = dialogY + 50; // Reiniciar la posición Y para el lado derecho
			char itemTitle[256];
			wsprintf(itemTitle, m_sLang == LANG_ESP ? "Items agregados" : "Items Added");
			PutString2(midX + 10, y, itemTitle, 228, 161, 6); // Ajustar la posición X para el lado derecho
			y += 20;

			// Recorrer la lista de ítems ingresados
			for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
				if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] != -1) {
					char itemText[256];

					auto it = m_pItemList[m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i]];
					if (it != NULL)
					{
						// Mostrar el nombre del ítem y la cantidad agregada entre paréntesis
						wsprintf(itemText, "%s (%d)", it->m_cName, it->m_dwCount);

						// Verificar si el mouse está sobre el texto del ítem
						if ((mouseX >= midX + 10) && (mouseX <= midX + dialogSizeX - 10) && (mouseY >= y) && (mouseY <= y + 15)) {
							// Dibujar el texto con el color resaltado (255, 215, 0)
							PutString2(midX + 10, y, itemText, 255, 215, 0);
						}
						else {
							// Dibujar el texto con el color normal (255, 255, 255)
							PutString2(midX + 10, y, itemText, 255, 255, 255);
						}

						y += 15;
					}
				}
			}

			// Informar al jugador sobre el descuento de ítems
			char infoText[256];
			wsprintf(infoText, m_sLang == LANG_ESP ? "Si excede la cantidad requerida, solo se descuenta lo justo." : "If you enter more than required, only necessary will be used.");
			PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + dialogSizeY - 20, infoText, 200, 200, 200); // Texto centrado en gris
		}

		// Dibujar el botón "Back"
		DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 10, 290, m_sLang == LANG_ESP ? "Confirmar" : "Confirm");
		DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 266, 290, m_sLang == LANG_ESP ? "Cancelar" : "Cancel");
		break;
	}
	case MANUFACTURE_MODE_COMPLETE:
	{
		// ... (código existente para MANUFACTURE_MODE_COMPLETE)
		break;
	}
	default:
		break;
	}
}

void CGame::HandleManufactureClick(short mouseX, short mouseY)
{
	short dialogX, dialogY, dialogSizeX;
	int dialogNumber = 86;
	int maxLines = 16;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	auto &mode = m_stDialogBoxInfo[dialogNumber].cMode;

	auto eng = (mouseX >= dialogX + 276) && (mouseX <= dialogX + 299) && (mouseY >= dialogY + 13) && (mouseY <= dialogY + 29);
	auto esp = (mouseX >= dialogX + 304) && (mouseX <= dialogX + 326) && (mouseY >= dialogY + 13) && (mouseY <= dialogY + 29);

	if (eng) {
		PlaySound('E', 14, 5); m_sLang = LANG_ENG;
	}
	if (esp) {
		PlaySound('E', 14, 5); m_sLang = LANG_ESP;
	}

	switch (mode)
	{
	case MANUFACTURE_MODE_SELECT_RECIPE:
	{		
		// Verificar si se hizo clic en un ítem de la lista
		for (int i = 0; i < maxLines; i++) {
			if (i + m_stDialogBoxInfo[dialogNumber].sView < recipes.size()) {
				int y = dialogY + 80 + (15 * i);
				if ((mouseX > dialogX + 50) && (mouseX < dialogX + dialogSizeX - 50) && (mouseY > y) && (mouseY < y + 15)) {
					m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex = i + m_stDialogBoxInfo[dialogNumber].sView;
					mode = MANUFACTURE_MODE_CONFIRM_RECIPE;
					PlaySound('E', 14, 5);
					break;
				}
			}
		}
		
		break;
	}
	case MANUFACTURE_MODE_CONFIRM_RECIPE:
	{
		if ((mouseX >= dialogX + 266) && (mouseX <= dialogX + 341) && (mouseY >= dialogY + 290) && (mouseY <= dialogY + 311))
		{
			ResetManufactureDialog();
			mode = MANUFACTURE_MODE_SELECT_RECIPE;
			PlaySound('E', 14, 5);
		}
		else if ((mouseX >= dialogX + 10) && (mouseX <= dialogX + 85) && (mouseY >= dialogY + 290) && (mouseY <= dialogY + 311))
		{
			// Verificar si se cumplen los requisitos de la receta
			if (m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex >= 0 &&
				m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex < recipes.size())
			{
				const Recipe &selectedRecipe = recipes[m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex];
				bool allIngredientsComplete = true;

				// Verificar si todos los ingredientes están completos
				for (const auto &ingredient : selectedRecipe.ingredients) {
					bool isIngredientComplete = false;
					for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
						if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] != -1) {
							auto it = m_pItemList[m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i]];
							if (it != NULL && it->m_cName == ingredient.first && it->m_dwCount >= ingredient.second) {
								isIngredientComplete = true;
								break;
							}
						}
					}
					if (!isIngredientComplete) {
						allIngredientsComplete = false;
						break;
					}
				}

				// Si todos los ingredientes están completos, enviar el mensaje al servidor
				if (allIngredientsComplete) {
					// Preparar el mensaje para enviar al servidor

					char data[512];
					auto *cp = data;

					// Agregar la cantidad de ítems en la lista
					int itemCount = 0;
					for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
						if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] != -1) {
							itemCount++;
						}
					}
					
					if (itemCount != 0)
					{
						// Agregar el identificador de la operación (por ejemplo, MANUFACTURE_CONFIRM)
						Push(cp, (u32)MANUFACTURE_CONFIRM);
						Push(cp, (u16)0);

						// Agregar el índice de la receta seleccionada
						Push(cp, m_stDialogBoxInfo[dialogNumber].selectedRecipeIndex);

						Push(cp, itemCount);

						// Agregar cada ítem de la lista
						for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
							if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] != -1) {
								Push(cp, m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i]);
							}
						}

						// Enviar el mensaje al servidor
						m_pGSock->iSendMsg(data, cp - data);

						AddEventList(m_sLang == LANG_ESP ? "Receta enviada al servidor." : "Recipe sent to the server.");
					}					
				}
				else {
					AddEventList(m_sLang == LANG_ESP ? "Faltan ingredientes para completar la receta." : "Missing ingredients to complete the recipe.");
				}
			}
		}
		else
		{
			// Verificar si se hizo clic en un ítem de la lista de ítems agregados
			int y = dialogY + 50 + 20; // Posición inicial de los ítems agregados (título + espacio + título de los ítems)
			for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++) {
				if (m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] != -1) {
					// Calcular el área de clic para el ítem actual
					if ((mouseX >= dialogX + (dialogSizeX / 2) + 10) && (mouseX <= dialogX + dialogSizeX - 10) && (mouseY >= y) && (mouseY <= y + 15)) {
						// Eliminar el ítem de la lista
						char itemID = m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i];
						m_stDialogBoxInfo[dialogNumber].m_iManuItemList[i] = -1; // Liberar el espacio
						m_bIsItemDisabled[itemID] = FALSE; // Habilitar el ítem
						AddEventList(m_sLang == LANG_ESP ? "Item eliminado de la lista." : "Item removed from the list.");
						PlaySound('E', 29, 0); // Reproducir sonido
						return; // Salir del bucle
					}
					y += 15; // Mover a la siguiente posición Y
				}
			}
		}
		break;
	}
	case MANUFACTURE_MODE_COMPLETE:
	{
		// ... (código existente para MANUFACTURE_MODE_COMPLETE)
		break;
	}
	default:
		break;
	}
}

void CGame::ResetManufactureDialog()
{
	for (int i = 0; i < MAX_MANUFACTURE_ITEMS; i++)
	{
		m_bIsItemDisabled[m_stDialogBoxInfo[86].m_iManuItemList[i]] = FALSE;
		m_stDialogBoxInfo[86].m_iManuItemList[i] = -1;
	}

	m_stDialogBoxInfo[86].selectedRecipeIndex = -1;
}

void CGame::InitializeCraftingRecipes()
{
	recipes = {

		{ "DarkExecutor", 1, 24, 0, {
			// Peces
			{ "BluefinFish", 40 },
			{ "Shark", 35 },
			// Minerales
			{ "Palladium", 40 },
			{ "Eclipsium", 35 },
			{ "Astronyx", 30 },
			// Drops de criaturas
			{ "TrollHeart", 70 },
			{ "OgreClaw", 60 },
			{ "WerewolfNail", 50 },
			// Piedras mágicas
			{ "AbyssalSapphire", 1 },
			{ "CelestialDiamond", 1 }
		} },

		{ "MagicWand(MS30-LLF)", 17, 0, 0, {
			// Peces
			{ "ClownFish", 40 },
			{ "ParrotFish", 35 },
			// Minerales
			{ "Voidsteel", 40 },
			{ "Netherite", 35 },
			{ "Orichalcum", 30 },
			// Drops de criaturas
			{ "CyclopsEye", 70 },
			{ "UnicornHorn", 60 },
			{ "WerewolfTeeth", 50 },
			// Piedras mágicas
			{ "DivineAmethyst", 1 },
			{ "LuminousQuartz", 1 }
		} }

		/*{ "RingoftheAbaddon", 16, 26, 0, {
			// Peces
			{ "ClownFish", 25 },
			{ "ParrotFish", 35 },
			// Minerales
			{ "Orichalcum", 30 },
			{ "Palladium", 40 },
			// Drops de criaturas
			{ "DemonEye", 30 },
			{ "SkeletonBones", 25 },
			{ "StoneGolemPiece", 20 },
			{ "TrollHeart", 15 },
			{ "WerewolfNail", 10 },
			// Piedras mágicas
			{ "SolarZircon", 1 }
		} },

		{ "RingofArcmage", 16, 49, 0, {
			// Peces
			{ "BettaFish", 60 },
			// Minerales
			{ "Cobalt", 20 },
			{ "Darksteel", 25 },
			{ "Mithril", 30 },
			// Drops de criaturas
			{ "CyclopsMeat", 30 },
			{ "HelboundClaw", 25 },
			{ "OgreHair", 20 },
			{ "ScorpionSkin", 15 },
			{ "UnicornHeart", 10 },
			// Piedras mágicas
			{ "TwilightOpal", 1 }
		} },

		{ "MagicNecklace(MS22)", 16, 7, 0, {
			// Peces
			{ "AngelFish", 30 },
			{ "GreenFish", 30 },
			// Minerales
			{ "Palladium", 20 },
			{ "Silver", 25 },
			// Drops de criaturas
			{ "CyclopsLeather", 40 },
			{ "OrcTeeth", 30 },
			{ "UnicornMeat", 25 },
			{ "WerewolfTail", 15 },
			// Piedras mágicas
			{ "MythicAlexandrite", 1 },
			/ *{ "InfernalRuby", 1 }* /
			{ "ContribBall", 30000 }
		} },

		{ "WarNecklace(HitP25)", 16, 64, 0, {
			// Peces
			{ "PufferFish", 20 },
			{ "RedFish", 25 },
			// Minerales
			{ "Steel", 20 },
			{ "Titanium", 25 },
			// Drops de criaturas
			{ "CyclopsMeat", 40 },
			{ "OgreHeart", 30 },
			{ "OgreTeeth", 25 },
			{ "OrcLeather", 15 },
			{ "ScorpionSting", 10 },
			// Piedras mágicas
			{ "EclipseTanzanite", 1 }
		} },

		{ "MagNecklace(HitP25)", 16, 65, 0, {
			// Peces
			{ "BluefinFish", 15 },
			{ "GreenFish", 20 },
			{ "LionFish", 25 },
			// Minerales
			{ "Darksteel", 25 },
			{ "Obsidian", 30 },
			// Drops de criaturas
			{ "LumpofClay", 40 },
			{ "ScorpionMeat", 30 },
			{ "TrollMeat", 20 },
			{ "UnicornLeather", 15 },
			// Piedras mágicas
			{ "EtherealEmerald", 1 }*/
		/*} }*/
	};
}


bool CGame::IsPowerHeroItem(char * itemname)
{
	return string(itemname) == "PowerHeroHauberk(W)" ||
		string(itemname) == "PowerHeroLeggs(W)" ||
		string(itemname) == "PowerHeroArmor(W)" ||
		string(itemname) == "PowerHeroRobe(W)" ||
		string(itemname) == "PowerHeroHelm(W)" ||
		string(itemname) == "PowerHeroCap(W)" ||

		string(itemname) == "PowerHeroHauberk(M)" ||
		string(itemname) == "PowerHeroLeggs(M)" ||
		string(itemname) == "PowerHeroArmor(M)" ||
		string(itemname) == "PowerHeroRobe(M)" ||
		string(itemname) == "PowerHeroHelm(M)" ||
		string(itemname) == "PowerHeroCap(M)" ||
		string(itemname) == "aPowerHeroCape" ||
		string(itemname) == "aPowerHeroCape+1" ||
		string(itemname) == "ePowerHeroCape" ||
		string(itemname) == "ePowerHeroCape+1";
}



bool CGame::IsHeroItem(char * itemname)
{
	return string(itemname) == "eHeroRobe(W)" ||
		string(itemname) == "eHeroHelm(W)" ||
		string(itemname) == "eHeroCap(W)" ||
		string(itemname) == "eHeroHauberk(W)" ||
		string(itemname) == "eHeroLeggings(W)" ||
		string(itemname) == "eHeroArmor(W)" ||

		string(itemname) == "aHeroRobe(W)" ||
		string(itemname) == "aHeroHelm(W)" ||
		string(itemname) == "aHeroCap(W)" ||
		string(itemname) == "aHeroHauberk(W)" ||
		string(itemname) == "aHeroLeggings(W)" ||
		string(itemname) == "aHeroArmor(W)" ||

		string(itemname) == "eHeroRobe(M)" ||
		string(itemname) == "eHeroHelm(M)" ||
		string(itemname) == "eHeroCap(M)" ||
		string(itemname) == "eHeroHauberk(M)" ||
		string(itemname) == "eHeroLeggings(M)" ||
		string(itemname) == "eHeroArmor(M)" ||

		string(itemname) == "aHeroRobe(M)" ||
		string(itemname) == "aHeroHelm(M)" ||
		string(itemname) == "aHeroCap(M)" ||
		string(itemname) == "aHeroHauberk(M)" ||
		string(itemname) == "aHeroLeggings(M)" ||
		string(itemname) == "aHeroArmor(M)";
}


void CGame::HandleItemDropForUpgradePowerSet(short mouseX, short mouseY)
{
	int dialogNumber = 87;
	char itemID;
	itemID = (char)m_stMCursor.sSelectedObjectID;

	auto item = m_pItemList[itemID];
	if (!item) return;

	if (m_bIsItemDisabled[itemID] == TRUE || m_cCommand < 0 || item->m_cEquipPos == DEF_EQUIPPOS_NONE || !IsHeroItem(item->m_cName)) return;
	if (m_stDialogBoxInfo[dialogNumber].cMode != 0) return;

	int iValue = item->m_sItemSpecEffectValue3;

	if (iValue >= 5) 
	{
		AddEventList("Haz alcanzado el nivel maximo de este item.");  
		return;
	}

	m_bIsItemDisabled[m_stDialogBoxInfo[dialogNumber].sV1] = FALSE;
	m_stDialogBoxInfo[dialogNumber].sV1 = itemID;
	m_bIsItemDisabled[itemID] = TRUE;
	PlaySound('E', 29, 0);
	
}
void CGame::RenderPowerHeroDialog(short mouseX, short mouseY, short mouseZ, char leftButton)
{
	short dialogX, dialogY, dialogSizeX, dialogSizeY;
	int dialogNumber = 87;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;
	auto &mode = m_stDialogBoxInfo[dialogNumber].cMode;

	DrawShadowDialog2(dialogX, dialogY, dialogNumber);

	// --- Título ---
	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 3,
		m_sLang == LANG_ESP ? "Upgrade Hero Set" : "Upgrade Hero Set", 0, 0, 0);
	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 4,
		m_sLang == LANG_ESP ? "Upgrade Hero Set" : "Upgrade Hero Set", 250, 250, 250);

	// --- Mostrar ítem ---
	if (m_stDialogBoxInfo[dialogNumber].sV1 != -1)
	{
		auto item = m_pItemList[m_stDialogBoxInfo[dialogNumber].sV1];
		if (item)
		{
			short y = dialogY + 30;
			int iValue = item->m_sItemSpecEffectValue3;

			// --- Nombre del ítem ---
			char itemname[256];
			if (iValue > 0)
				wsprintf(itemname, "%s +%d", item->m_cName, iValue);
			else
				wsprintf(itemname, "%s", item->m_cName);
			PutAlignedString2(dialogX, dialogX + dialogSizeX, y, itemname, 228, 161, 6);

			// --- Cálculo de costos ---
			int nextLevel = iValue + 1;
			int contribCost = nextLevel * 100000;
			int eksCost = 1000;

			// --- Mostrar costos ---
			char costContrib[128], costEks[128];
			wsprintf(costContrib, "Cost: %d Contribución", contribCost);
			wsprintf(costEks, "Cost: %d EKS", eksCost);

			PutAlignedString2(dialogX, dialogX + dialogSizeX, y + 15, costContrib, 200, 230, 255);
			PutAlignedString2(dialogX, dialogX + dialogSizeX, y + 30, costEks, 255, 220, 120);

			// --- Sprite del ítem ---
			short spriteX = dialogX + 10 + 115;
			short spriteY = y + 85;

			if (item->m_cItemColor != 0)
			{
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteRGB(
					spriteX, spriteY, item->m_sSpriteFrame,
					m_wWR[item->m_cItemColor] - m_wR[0],
					m_wWG[item->m_cItemColor] - m_wG[0],
					m_wWB[item->m_cItemColor] - m_wB[0], 0);
			}
			else
			{
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteFast(
					spriteX, spriteY, item->m_sSpriteFrame, 0);
			}
		}
	}
	else
	{
		PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 50,
			m_sLang == LANG_ESP ? "Por favor, agrega un ítem para continuar." :
			"Please add one item to continue.", 255, 255, 255);
	}

	// --- Botones ---
	switch (mode)
	{
	case 0: // Normal
		if (m_stDialogBoxInfo[dialogNumber].sV1 != -1)
		{
			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 10, dialogSizeY - 40,
				m_sLang == LANG_ESP ? "Upgrade" : "Upgrade");
			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 175, dialogSizeY - 40,
				m_sLang == LANG_ESP ? "Cancel" : "Cancel");
		}
		else
		{
			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 175, dialogSizeY - 40,
				m_sLang == LANG_ESP ? "Close" : "Close");
		}
		break;

	case 1: // Confirmación
		PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 220,
			m_sLang == LANG_ESP ?
			"¿Seguro que deseas mejorar este ítem?" :
			"Are you sure you want to upgrade this item?", 255, 255, 255);

		DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 10, dialogSizeY - 40,
			m_sLang == LANG_ESP ? "Yes" : "Yes");
		DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 175, dialogSizeY - 40,
			m_sLang == LANG_ESP ? "No" : "No");
		break;

	default:
		break;
	}
}

void CGame::HandlePowerHeroClick(short mouseX, short mouseY)
{
	short dialogX, dialogY, dialogSizeX, dialogSizeY;
	int dialogNumber = 87;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;
	auto &mode = m_stDialogBoxInfo[dialogNumber].cMode;

	switch (mode)
	{
	case 0: 
		if ((mouseX >= dialogX + 10) && (mouseX <= dialogX + 100) && (mouseY >= dialogY + dialogSizeY - 40) && (mouseY <= dialogY + dialogSizeY - 20)) {
			if (m_stDialogBoxInfo[dialogNumber].sV1 != -1) {
				mode = 1;
				PlaySound('E', 14, 5);
			}
		}

		if ((mouseX >= dialogX + 175) && (mouseX <= dialogX + 175 + 90) && (mouseY >= dialogY + dialogSizeY - 40) && (mouseY <= dialogY + dialogSizeY - 20)) {
			if (m_stDialogBoxInfo[dialogNumber].sV1 != -1) {
				m_bIsItemDisabled[m_stDialogBoxInfo[dialogNumber].sV1] = FALSE;
				m_stDialogBoxInfo[dialogNumber].sV1 = -1;
				PlaySound('E', 14, 5);
			}
			else {

				PlaySound('E', 14, 5);
				DisableDialogBox(87);
			}
		}
		break;

	case 1:
		if ((mouseX >= dialogX + 10) && (mouseX <= dialogX + 100) && (mouseY >= dialogY + dialogSizeY - 40) && (mouseY <= dialogY + dialogSizeY - 20)) {
			char data[512];
			auto *cp = data;
			Push(cp, (u32)UPGRADE_SETHERO);
			Push(cp, (u16)0);
			Push(cp, m_stDialogBoxInfo[dialogNumber].sV1);
			m_pGSock->iSendMsg(data, cp - data);

			mode = 0;
			PlaySound('E', 14, 5);
		}

		if ((mouseX >= dialogX + 175) && (mouseX <= dialogX + 175 + 90) && (mouseY >= dialogY + dialogSizeY - 40) && (mouseY <= dialogY + dialogSizeY - 20)) {
			mode = 0;
			PlaySound('E', 14, 5);
		}
		break;

	default:
		break;
	}
}

void CGame::RequestFishing(short msX, short msY)
{
	char data[512];
	auto *cp = data;
	Push(cp, (u32)REQUEST_FISHING);
	Push(cp, (u16)0);
	Push(cp, msX);
	Push(cp, msY);
	m_pGSock->iSendMsg(data, cp - data);
}


void CGame::CancelFishing()
{
	char data[512];
	auto *cp = data;
	Push(cp, (u32)REQUEST_CANCELFISHING);
	Push(cp, (u16)0);
	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::SendWriteStatus(short status)
{
	char data[512];
	auto *cp = data;
	Push(cp, (u32)NOTIFY_WRITESTATUS);
	Push(cp, (u16)0);
	Push(cp, status);
	m_pGSock->iSendMsg(data, cp - data);
}

int CGame::CalculateUpgradeCost(int currentLevel, int statIndex) {
	// Costo = costo_base * nivel_objetivo
	// Ejemplo: pasar de 0->1: costo_base * 1
	//           pasar de 1->2: costo_base * 2
	const StatConfig& config = statConfigs[statIndex];
	return config.tokenCost * (currentLevel + 1);
}

int CGame::CalculateCoinCost(int currentLevel, int statIndex) {
	// Misma fórmula pero para coins
	const StatConfig& config = statConfigs[statIndex];
	return config.coinCost * (currentLevel + 1);
}

void CGame::DrawStatButtonBox(int x, int y, int width, int height, int type) {
	// Usamos DrawDialogShadowBox con un tipo que le dé color sólido o sombreado
	m_DDraw.DrawDialogShadowBoxOrange(x, y, x + width, y + height);
}

void CGame::RenderStatsUpgradeDialog(short mouseX, short mouseY, short mouseZ, char leftButton) {
	short dialogX = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].sX;
	short dialogY = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].sY;
	short dialogSizeX = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].sSizeX;
	short dialogSizeY = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].sSizeY;
	auto& mode = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].cMode;

	DrawShadowDialog2(dialogX, dialogY, STATS_DIALOG_NUMBER);

	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 3,
		m_sLang == LANG_ESP ? "Mejorar Estadisticas" : "Upgrade Stats", 0, 0, 0);
	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 4,
		m_sLang == LANG_ESP ? "Mejorar Estadisticas" : "Upgrade Stats", 250, 250, 250);

	char tokensText[256];
	wsprintf(tokensText, m_sLang == LANG_ESP ? "Tokens disponibles: %d" : "Available Tokens: %d", m_iTokens);
	PutAlignedString(dialogX, dialogX + dialogSizeX, dialogY + 30, tokensText, 200, 200, 200);

	char coinsText[256];
	wsprintf(coinsText, m_sLang == LANG_ESP ? "Coins disponibles: %d" : "Available Coins: %d", m_iCoins);
	PutAlignedString(dialogX, dialogX + dialogSizeX, dialogY + 45, coinsText, 200, 200, 200);

	const char* statNames[STAT_COUNT][2] = {
		{ "Absorcion Fisica", "Physical Absorption" },
		{ "Absorcion Magica", "Magic Absorption" },
		{ "Resistencia Magica", "Magic Resist" },
		{ "Ratio de Defensa", "Defense Ratio" },
		{ "Daño Fisico", "Physical Damage" },
		{ "Hit Fisico", "Physical Hiting" },
		{ "Daño Magico", "Magic Damage" },
		{ "Hit Magico", "Magic Hiting" },
		{ "Ahorro de Mana", "ManaSave" }
	};

	switch (mode) {
	case STATS_MODE_SELECT: {
		int yPos = dialogY + 80;
		int totalTokenCost = 0;
		int totalCoinCost = 0;

		for (int i = 0; i < STAT_COUNT; i++) {
			int currentLevel = GetCurrentStatLevel(i);
			int maxForStat = statConfigs[i].maxLevel;

			// Calcular costos para los incrementos temporales
			for (int j = 0; j < m_iStatTemp[i]; j++) {
				totalTokenCost += CalculateUpgradeCost(currentLevel + j, i);
				totalCoinCost += CalculateCoinCost(currentLevel + j, i);
			}

			char statText[256];
			wsprintf(statText, "%s: %d/%d -> %d (+%d)",
				m_sLang == LANG_ESP ? statNames[i][0] : statNames[i][1],
				currentLevel, maxForStat,
				currentLevel + m_iStatTemp[i],
				m_iStatTemp[i]);

			// Determinar el color del texto
			int textColor;
			if (currentLevel + m_iStatTemp[i] >= maxForStat) {
				// Rojo si alcanzó el máximo
				textColor = RGB(255, 100, 100);
			}
			else if (m_iStatTemp[i] > 0) {
				// Amarillo si ha sido modificado
				textColor = RGB(255, 255, 40);
			}
			else {
				// Blanco si no ha sido modificado
				textColor = RGB(255, 255, 255);
			}

			PutString2(dialogX + 20, yPos, statText, GetRValue(textColor),
				GetGValue(textColor), GetBValue(textColor));

			DrawStatButtonBox(dialogX + 220 + 75, yPos, 10, 10, 2); // Botón +
			PutString2(dialogX + 222 + 1 + 75, yPos - 3, "+", 0, 0, 0);

			DrawStatButtonBox(dialogX + 235 + 75, yPos, 10, 10, 2); // Botón -
			PutString2(dialogX + 237 + 1 + 75, yPos - 3, "-", 0, 0, 0);

			yPos += 22;
		}

		char totalText1[128];
		wsprintf(totalText1, m_sLang == LANG_ESP ?
			"Costo en coins: %d" :
			"Total coin cost: %d",
			totalCoinCost);

		PutString2(dialogX + 20, yPos + 25 - 15, totalText1, 255, 255, 40);

		char totalText2[128];
		wsprintf(totalText2, m_sLang == LANG_ESP ?
			"Costo en tokens: %d" :
			"Total token cost: %d",
			totalTokenCost);
		PutString2(dialogX + 20, yPos + 25 + 15 - 15, totalText2, 255, 255, 40);

		DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, DEF_RBTNPOSX + 115, DEF_BTNPOSY, "Confirm");
		break;
	}

	case STATS_MODE_CONFIRM: {
		// Texto principal de confirmación
		char confirmText[256];
		wsprintf(confirmText, m_sLang == LANG_ESP ?
			"¿Aplicar mejoras por %d tokens y %d coins?" :
			"Apply upgrades for %d tokens and %d coins?",
			CalculateTotalStatUpgradeTokenCost(),
			CalculateTotalStatUpgradeCoinCost());

		PutAlignedString(dialogX, dialogX + dialogSizeX, dialogY + 80, confirmText, 228, 0, 0);

		// Mostrar detalles de cada stat que se va a mejorar
		int yPos = dialogY + 110;
		bool anyUpgrade = false;

		for (int i = 0; i < STAT_COUNT; i++) {
			if (m_iStatTemp[i] > 0) {
				anyUpgrade = true;
				int currentLevel = GetCurrentStatLevel(i);

				// Calcular costos individuales
				int statTokenCost = 0;
				int statCoinCost = 0;
				for (int j = 0; j < m_iStatTemp[i]; j++) {
					statTokenCost += CalculateUpgradeCost(currentLevel + j, i);
					statCoinCost += CalculateCoinCost(currentLevel + j, i);
				}

				char statDetail[256];
				wsprintf(statDetail, "%s: %d -> %d (+%d) - %d tokens, %d coins",
					m_sLang == LANG_ESP ? statNames[i][0] : statNames[i][1],
					currentLevel,
					currentLevel + m_iStatTemp[i],
					m_iStatTemp[i],
					statTokenCost,
					statCoinCost);

				PutAlignedString(dialogX, dialogX + dialogSizeX, yPos, statDetail, 0, 255, 0);
				yPos += 15;
			}
		}

		// Si no hay mejoras (caso inusual pero posible)
		if (!anyUpgrade) {
			PutString2(dialogX + 40, yPos,
				m_sLang == LANG_ESP ? "No hay mejoras seleccionadas" : "No upgrades selected",
				255, 100, 100);
			yPos += 15;
		}

		// Botones - posición fija en la parte inferior del diálogo
		DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, DEF_LBTNPOSX, DEF_BTNPOSY, "Confirm");
		DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, DEF_RBTNPOSX + 100, DEF_BTNPOSY, "Cancel");
		break;
	}
	}
}

void CGame::HandleStatsUpgradeClick(short mouseX, short mouseY) {
	short dialogX = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].sX;
	short dialogY = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].sY;
	short dialogSizeX = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].sSizeX;
	short dialogSizeY = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].sSizeY;
	auto& mode = m_stDialogBoxInfo[STATS_DIALOG_NUMBER].cMode;

	switch (mode) {
	case STATS_MODE_SELECT: {
		int yPos = dialogY + 80;

		for (int i = 0; i < STAT_COUNT; i++) {
			int currentLevel = GetCurrentStatLevel(i);
			int maxForStat = statConfigs[i].maxLevel;

			// + botón
			if (mouseX >= dialogX + 220 + 75 && mouseX <= dialogX + 230 + 75 &&
				mouseY >= yPos && mouseY <= yPos + 10) {
				if (m_iStatTemp[i] + currentLevel < maxForStat) {
					m_iStatTemp[i]++;
					PlaySound('E', 14, 5);
				}
				else {
					PlaySound('E', 14, 0); // Sonido de error
				}
			}

			// - botón
			if (mouseX >= dialogX + 235 + 75 && mouseX <= dialogX + 245 + 75 &&
				mouseY >= yPos && mouseY <= yPos + 10) {
				if (m_iStatTemp[i] > 0) {
					m_iStatTemp[i]--;
					PlaySound('E', 14, 5);
				}
			}

			yPos += 22;
		}

		// Verificar click en botón Confirmar
		int confirmButtonX = dialogX + (dialogSizeX / 2 - 40);
		int confirmButtonY = dialogY + dialogSizeY - 30;

		if ((mouseX >= dialogX + DEF_RBTNPOSX + 115) && (mouseX <= dialogX + DEF_RBTNPOSX + DEF_BTNSZX + 115) && (mouseY > dialogY + DEF_BTNPOSY) && (mouseY < dialogY + DEF_BTNPOSY + DEF_BTNSZY))
		/*if ((mouseX >= confirmButtonX) && (mouseX <= confirmButtonX + 80) &&
			(mouseY >= confirmButtonY) && (mouseY <= confirmButtonY + 21))*/ {

			int totalTokenCost = CalculateTotalStatUpgradeTokenCost();
			int totalCoinCost = CalculateTotalStatUpgradeCoinCost();

			if (m_iTokens >= totalTokenCost &&
				m_iCoins >= totalCoinCost &&
				totalTokenCost > 0) {
				mode = STATS_MODE_CONFIRM;
				PlaySound('E', 14, 5);
			}
			else {
				if (totalTokenCost == 0) {
					AddEventList(m_sLang == LANG_ESP ?
						"No hay mejoras seleccionadas." : "No upgrades selected.");
				}
				else if (m_iTokens < totalTokenCost) {
					AddEventList(m_sLang == LANG_ESP ?
						"No tienes suficientes tokens." : "Not enough tokens.");
				}
				else if (m_iCoins < totalCoinCost) {
					AddEventList(m_sLang == LANG_ESP ?
						"No tienes suficientes coins." : "Not enough coins.");
				}
				PlaySound('E', 14, 0); // Sonido de error
			}
		}
		break;
	}

	case STATS_MODE_CONFIRM: {
		// Posición fija de los botones (igual que en Render)
		int buttonY = dialogY + dialogSizeY - 30;
		int buttonWidth = 75;
		int buttonHeight = 21;
		int buttonSpacing = 25;

		// Coordenadas del botón Confirmar
		int confirmX = dialogX + (dialogSizeX / 2 - buttonWidth - buttonSpacing / 2);
		int confirmX2 = confirmX + buttonWidth;
		int confirmY = buttonY;
		int confirmY2 = buttonY + buttonHeight;

		// Coordenadas del botón Cancelar
		int cancelX = dialogX + (dialogSizeX / 2 + buttonSpacing / 2);
		int cancelX2 = cancelX + buttonWidth;
		int cancelY = buttonY;
		int cancelY2 = buttonY + buttonHeight;

		if ((mouseX >= dialogX + DEF_LBTNPOSX) && (mouseX <= dialogX + DEF_LBTNPOSX + DEF_BTNSZX) && (mouseY > dialogY + DEF_BTNPOSY) && (mouseY < dialogY + DEF_BTNPOSY + DEF_BTNSZY))
		// Verificar clicks
		/*if (mouseX >= confirmX && mouseX <= confirmX2 &&
			mouseY >= confirmY && mouseY <= confirmY2) */{

			int totalTokenCost = CalculateTotalStatUpgradeTokenCost();
			int totalCoinCost = CalculateTotalStatUpgradeCoinCost();

			if (m_iTokens >= totalTokenCost && m_iCoins >= totalCoinCost) {
				m_iTokens -= totalTokenCost;
				m_iCoins -= totalCoinCost;

				for (int i = 0; i < STAT_COUNT; i++) {
					switch (i) {
					case STAT_PA: m_iAddedPA += m_iStatTemp[i]; break;
					case STAT_MA: m_iAddedMA += m_iStatTemp[i]; break;
					case STAT_MR: m_iAddedMR += m_iStatTemp[i]; break;
					case STAT_DR: m_iAddedDR += m_iStatTemp[i]; break;
					case STAT_PHYS_DMG: m_iAddedPhysDmg += m_iStatTemp[i]; break;
					case STAT_PHYS_HIT: m_iAddedPhysHit += m_iStatTemp[i]; break;
					case STAT_MAG_DMG: m_iAddedMagDmg += m_iStatTemp[i]; break;
					case STAT_MAG_HIT: m_iAddedMagHit += m_iStatTemp[i]; break;
					case STAT_MS: m_iAddedMS += m_iStatTemp[i]; break;
					}
				}
				SendBulkStatUpgradeToServer();
				memset(m_iStatTemp, 0, sizeof(m_iStatTemp));

				/*AddEventList(m_sLang == LANG_ESP ?
					"¡Estadísticas mejoradas!" : "Stats upgraded!");*/
				PlaySound('E', 14, 5);
			}
			else {
				/*AddEventList(m_sLang == LANG_ESP ?
					"Error al aplicar mejoras." : "Upgrade error.");*/
				PlaySound('E', 14, 0);
			}
			mode = STATS_MODE_SELECT;
		}
		else if ((mouseX >= dialogX + DEF_RBTNPOSX + 100) && (mouseX <= dialogX + DEF_RBTNPOSX + DEF_BTNSZX + 100) && (mouseY > dialogY + DEF_BTNPOSY) && (mouseY < dialogY + DEF_BTNPOSY + DEF_BTNSZY))/*if (mouseX >= cancelX && mouseX <= cancelX2 &&
			mouseY >= cancelY && mouseY <= cancelY2)*/ {
			ReqStats();
			bSendCommand(CLIENT_REQUEST_FULLDATACHAR, NULL, NULL, NULL, NULL, NULL, NULL);
			memset(m_iStatTemp, 0, sizeof(m_iStatTemp));
			mode = STATS_MODE_SELECT;
			PlaySound('E', 14, 5);
		}
		break;
	}
	}
}
int CGame::GetCurrentStatLevel(int stat) {
	switch (stat) {
	case STAT_PA: return m_iAddedPA;
	case STAT_MA: return m_iAddedMA;
	case STAT_MR: return m_iAddedMR;
	case STAT_DR: return m_iAddedDR;
	case STAT_PHYS_DMG: return m_iAddedPhysDmg;
	case STAT_PHYS_HIT: return m_iAddedPhysHit;
	case STAT_MAG_DMG: return m_iAddedMagDmg;
	case STAT_MAG_HIT: return m_iAddedMagHit;
	case STAT_MS: return m_iAddedMS;
	default: return 0;
	}
}

int CGame::CalculateTotalStatUpgradeTokenCost() {
	int total = 0;
	for (int i = 0; i < STAT_COUNT; i++) {
		int base = GetCurrentStatLevel(i);
		for (int j = 0; j < m_iStatTemp[i]; j++) {
			total += CalculateUpgradeCost(base + j, i);
		}
	}
	return total;
}

int CGame::CalculateTotalStatUpgradeCoinCost() {
	int total = 0;
	for (int i = 0; i < STAT_COUNT; i++) {
		int base = GetCurrentStatLevel(i);
		for (int j = 0; j < m_iStatTemp[i]; j++) {
			total += CalculateCoinCost(base + j, i);
		}
	}
	return total;
}

void CGame::SendBulkStatUpgradeToServer() {
	char data[512];
	auto* cp = data;

	Push(cp, (u32)STAT_UPGRADE_COMMAND);
	Push(cp, (u16)1); // 1 = modo masivo

	for (int i = 0; i < STAT_COUNT; i++) {
		Push(cp, (char)m_iStatTemp[i]);
	}

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::ReqStats()
{
	char data[10];
	auto* cp = data;

	Push(cp, (u32)REQ_STATS);
	Push(cp, (u16)1); // 1 = modo masivo

	m_pGSock->iSendMsg(data, cp - data);
}/*

void CGame::RenderVoiceClientDialog(short mouseX, short mouseY, short mouseZ, char leftButton) {
	short x = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sX;
	short y = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sY;
	short w = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sSizeX;

	DrawShadowDialog2(x, y, VOICE_DIALOG_NUMBER);

	PutCenterString_SpriteFont(x, x + w, y + 3, m_sLang == LANG_ESP ? "Chat de Voz" : "Voice Chat", 0, 0, 0);
	PutCenterString_SpriteFont(x, x + w, y + 4, m_sLang == LANG_ESP ? "Chat de Voz" : "Voice Chat", 250, 250, 250);

	PutString2(x + 20, y + 40, m_sLang == LANG_ESP ? "Nombre de sala:" : "Room name:", 255, 255, 255);
	PutString2(x + 20, y + 70, m_sLang == LANG_ESP ? "Contraseña:" : "Password:", 255, 255, 255);

	const int roomBoxX = x + 140, roomBoxY = y + 40;
	const int passBoxX = x + 140, passBoxY = y + 70;

	// Cuadro de texto: Nombre de sala
	DrawShadowMenu(roomBoxX, roomBoxY, roomBoxX + 150, roomBoxY + 18);
	DrawShadowMenu_DialogBordersNoTitle(roomBoxX, roomBoxY, 150, 18);
	PutString2(roomBoxX + 2, roomBoxY + 2, m_cVoiceRoom, 255, 255, 255);

	// Cuadro de texto: Contraseña
	DrawShadowMenu(passBoxX, passBoxY, passBoxX + 150, passBoxY + 18);
	DrawShadowMenu_DialogBordersNoTitle(passBoxX, passBoxY, 150, 18);
	PutString2(passBoxX + 2, passBoxY + 2, m_cVoicePassword, 255, 255, 255);

	DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 110, "Unirse");

	if (m_bVoiceConnected) {
		PutString2(x + 20, y + 140, m_sLang == LANG_ESP ? "Estado: Conectado" : "Status: Connected", 0, 255, 0);
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 110, m_sLang == LANG_ESP ? "Usuarios" : "Users");
	}
	else {
		if (m_bVoiceTriedConnection)
			PutString2(x + 20, y + 140, m_cVoiceStatus, 255, 100, 100);
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 110, m_sLang == LANG_ESP ? "Ver salas" : "Show rooms");
	}
}

void CGame::HandleVoiceClientClick(short mouseX, short mouseY) {
	short x = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sX;
	short y = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sY;

	const int roomBoxX = x + 140, roomBoxY = y + 40;
	const int passBoxX = x + 140, passBoxY = y + 70;

	// Click en input "Nombre de sala"
	if ((mouseX >= roomBoxX) && (mouseX <= roomBoxX + 150) &&
		(mouseY >= roomBoxY) && (mouseY <= roomBoxY + 18)) {
		m_bInputVoiceRoom = true;
		m_bInputVoicePassword = false;
		StartInputString(roomBoxX + 2, roomBoxY + 2, 31, m_cVoiceRoom, FALSE);
		return;
	}

	// Click en input "Contraseña"
	if ((mouseX >= passBoxX) && (mouseX <= passBoxX + 150) &&
		(mouseY >= passBoxY) && (mouseY <= passBoxY + 18)) {
		m_bInputVoicePassword = true;
		m_bInputVoiceRoom = false;
		StartInputString(passBoxX + 2, passBoxY + 2, 31, m_cVoicePassword, FALSE);
		return;
	}

	// Click en botón "Unirse"
	if ((mouseX >= x + DEF_RBTNPOSX) && (mouseX <= x + DEF_RBTNPOSX + DEF_BTNSZX) &&
		(mouseY >= y + 110) && (mouseY <= y + 110 + DEF_BTNSZY)) {

		EndInputString(); // Cierra input activo
		m_bInputVoiceRoom = false;
		m_bInputVoicePassword = false;

		m_bVoiceTriedConnection = true;
		strcpy(m_cVoiceStatus, m_sLang == LANG_ESP ? "Conectando..." : "Connecting...");

		if (InitVoiceClient(m_cPlayerName, "144.202.42.248", 12345, m_cVoiceRoom, m_cVoicePassword)) {
			m_bVoiceConnected = true;
			strcpy(m_cVoiceStatus, m_sLang == LANG_ESP ? "Conectado exitosamente." : "Connected successfully.");
			PlaySound('E', 14, 5);
		}
		else {
			m_bVoiceConnected = false;
			strcpy(m_cVoiceStatus, m_sLang == LANG_ESP ? "Fallo al conectar." : "Failed to connect.");
			PlaySound('E', 14, 0);
		}
		return;
	}

	// Click en botón izquierdo (ver salas / usuarios)
	if ((mouseX >= x + DEF_LBTNPOSX) && (mouseX <= x + DEF_LBTNPOSX + DEF_BTNSZX) &&
		(mouseY >= y + 110) && (mouseY <= y + 110 + DEF_BTNSZY)) {

		if (m_bVoiceConnected) {
			RequestConnectedUsers("144.202.42.248", 12345, m_cVoiceRoom);
		}
		else {
			ListAvailableRooms("144.202.42.248", 12345);  // No conectado => mostrar salas
		}
	}
}


void CGame::ListAvailableRooms(const char* server_ip, int port) {
	SOCKET tcp = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp == INVALID_SOCKET) return;

	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server_ip);

	if (connect(tcp, (sockaddr*)&addr, sizeof(addr)) < 0) {
		AddEventList("No se pudo conectar al servidor de voz.");
		closesocket(tcp);
		return;
	}

	const char* cmd = "LIST_ROOMS\n";
	send(tcp, cmd, strlen(cmd), 0);

	char buffer[2048] = {};
	int len = recv(tcp, buffer, sizeof(buffer) - 1, 0);
	if (len > 0) {
		buffer[len] = '\0';
		AddEventList("Salas disponibles:");
		char* token = strtok(buffer, "\n");
		while (token != nullptr) {
			AddEventList(token);
			token = strtok(nullptr, "\n");
		}
	}
	else {
		AddEventList("No se recibieron salas.");
	}

	closesocket(tcp);
}

void CGame::RequestConnectedUsers(const char* server_ip, int port, const char* room) {
	SOCKET tcp = socket(AF_INET, SOCK_STREAM, 0);
	if (tcp == INVALID_SOCKET) return;

	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server_ip);

	if (connect(tcp, (sockaddr*)&addr, sizeof(addr)) < 0) {
		AddEventList("No se pudo conectar para listar usuarios.");
		closesocket(tcp);
		return;
	}

	std::string cmd = "LIST_USERS," + std::string(room) + "\n";
	send(tcp, cmd.c_str(), cmd.length(), 0);

	char buffer[2048] = {};
	int len = recv(tcp, buffer, sizeof(buffer) - 1, 0);
	if (len > 0) {
		buffer[len] = '\0';
		AddEventList("Usuarios conectados:");
		char* token = strtok(buffer, "\n");
		while (token != nullptr) {
			AddEventList(token);
			token = strtok(nullptr, "\n");
		}
	}
	else {
		AddEventList("No se recibieron usuarios.");
	}

	closesocket(tcp);
}
*/

// Variables globales temporales
/*

void CGame::RenderVoiceClientDialog(short mouseX, short mouseY, short mouseZ, char leftButton) {
	short x = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sX;
	short y = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sY;
	short w = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sSizeX;
	auto& mode = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].cMode;
	auto& scroll = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sView;

	DrawShadowDialog2(x, y, VOICE_DIALOG_NUMBER);

	PutCenterString_SpriteFont(x, x + w, y + 3, m_sLang == LANG_ESP ? "Chat de Voz" : "Voice Chat", 0, 0, 0);
	PutCenterString_SpriteFont(x, x + w, y + 4, m_sLang == LANG_ESP ? "Chat de Voz" : "Voice Chat", 250, 250, 250);

	const int visibleLines = 10;

	if (mouseZ != 0 && iGetTopDialogBoxIndex() == VOICE_DIALOG_NUMBER) {
		int totalLines = (mode == 0 ? g_voiceRoomList.size() : g_voiceUserList.size());
		if (totalLines > visibleLines) {
			scroll -= mouseZ / 120;
			if (scroll < 0) scroll = 0;
			if (scroll > (int)totalLines - visibleLines)
				scroll = (int)totalLines - visibleLines;
			m_DInput.m_sZ = 0;
		}
	}

	switch (mode) {
	case 0: {
		PutString2(x + 20, y + 30, m_sLang == LANG_ESP ? "Salas disponibles:" : "Available rooms:", 255, 255, 255);
		int end = (scroll + visibleLines < (int)g_voiceRoomList.size()) ? scroll + visibleLines : (int)g_voiceRoomList.size();
		for (int i = scroll; i < end; ++i) {
			short lineY = y + 50 + (i - scroll) * 15;
			bool hovered = (mouseY >= lineY && mouseY <= lineY + 15 && mouseX >= x + 20 && mouseX <= x + 250);
			int r = hovered ? 255 : 255;
			int g = hovered ? 255 : 255;
			int b = hovered ? 0 : 255;
			PutString2(x + 20, lineY, (char*)g_voiceRoomList[i].c_str(), r, g, b);
		}
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 280, m_sLang == LANG_ESP ? "Actualizar" : "Refresh");
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 280, m_sLang == LANG_ESP ? "Crear sala" : "Create room");
		break;
	}
	case 1:
		PutString2(x + 20, y + 40, m_sLang == LANG_ESP ? "Contraseña de sala:" : "Room password:", 255, 255, 255);
		DrawShadowMenu(x + 150, y + 40, x + 230, y + 58);
		DrawShadowMenu_DialogBordersNoTitle(x + 150, y + 40, 80, 18);
		PutString2(x + 152, y + 42, m_cVoicePassword, 255, 255, 255);
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 90, m_sLang == LANG_ESP ? "Conectar" : "Connect");
		break;
	case 3: {
		PutString2(x + 20, y + 30, m_sLang == LANG_ESP ? "Usuarios en sala:" : "Users in room:", 0, 255, 0);
		int end = (scroll + visibleLines < (int)g_voiceUserList.size()) ? scroll + visibleLines : (int)g_voiceUserList.size();
		for (int i = scroll; i < end; ++i) {
			short lineY = y + 50 + (i - scroll) * 15;
			bool hovered = (mouseY >= lineY && mouseY <= lineY + 15 && mouseX >= x + 20 && mouseX <= x + 250);
			int r = hovered ? 255 : 200;
			int g = hovered ? 255 : 255;
			int b = hovered ? 255 : 255;
			PutString2(x + 20, lineY, (char*)g_voiceUserList[i].c_str(), r, g, b);
		}
		short muteX = x + 180;
		short muteY = y + 30;
		bool hoverMute = (mouseX >= muteX && mouseX <= muteX + 100 && mouseY >= muteY && mouseY <= muteY + 15);
		const char* muteText = m_bVoiceMicMuted ? (m_sLang == LANG_ESP ? "Desmutear mic" : "Unmute mic") : (m_sLang == LANG_ESP ? "Mutear mic" : "Mute mic");
		int r = 255, g = 255, b = 255;
		if (hoverMute) { r = 255; g = 255; b = 0; }
		else if (m_bVoiceMicMuted) { r = 255; g = 50; b = 50; }
		else { r = 50; g = 255; b = 50; }
		PutString2(muteX, muteY, (char*)muteText, r, g, b);
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 280, m_sLang == LANG_ESP ? "Actualizar" : "Refresh");
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 280, m_sLang == LANG_ESP ? "Desconectar" : "Disconnect");
		break;
	}
	case 4:
		PutString2(x + 20, y + 40, m_sLang == LANG_ESP ? "Nombre de sala:" : "Room name:", 255, 255, 255);
		DrawShadowMenu(x + 150, y + 40, x + 230, y + 58);
		DrawShadowMenu_DialogBordersNoTitle(x + 150, y + 40, 80, 18);
		PutString2(x + 152, y + 42, m_cVoiceRoom, 255, 255, 255);

		PutString2(x + 20, y + 70, m_sLang == LANG_ESP ? "Contraseña:" : "Password:", 255, 255, 255);
		DrawShadowMenu(x + 150, y + 70, x + 230, y + 88);
		DrawShadowMenu_DialogBordersNoTitle(x + 150, y + 70, 80, 18);
		PutString2(x + 152, y + 72, m_cVoicePassword, 255, 255, 255);

		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 110, m_sLang == LANG_ESP ? "Crear" : "Create");
		break;
	}
}
void CGame::HandleVoiceClientClick(short mouseX, short mouseY) {
	short x = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sX;
	short y = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sY;
	auto& mode = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].cMode;
	auto& scroll = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sView;

	if (mode == 0) {
		int end = (scroll + 10 < (int)g_voiceRoomList.size()) ? scroll + 10 : (int)g_voiceRoomList.size();
		for (int i = scroll; i < end; ++i) {
			short itemY = y + 50 + (i - scroll) * 15;
			if (mouseY >= itemY && mouseY <= itemY + 15 && mouseX >= x + 20 && mouseX <= x + 250) {
				strcpy(m_cVoiceRoom, g_voiceRoomList[i].c_str());
				mode = 1;
				PlaySound('E', 14, 5);
				return;
			}
		}
		if (mouseY >= y + 280 && mouseY <= y + 300) {
			if (mouseX >= x + DEF_LBTNPOSX && mouseX <= x + DEF_LBTNPOSX + DEF_BTNSZX) {
				ListAvailableRooms("144.202.42.248", 12345);
				PlaySound('E', 14, 5);
				return;
			}
			if (mouseX >= x + DEF_RBTNPOSX && mouseX <= x + DEF_RBTNPOSX + DEF_BTNSZX) {
				ZeroMemory(m_cVoiceRoom, sizeof(m_cVoiceRoom));
				ZeroMemory(m_cVoicePassword, sizeof(m_cVoicePassword));
				mode = 4;
				PlaySound('E', 14, 5);
				return;
			}
		}
	}
	else if (mode == 1) {
		if (mouseY >= y + 20 && mouseY <= y + 58 && mouseX >= x + 150 && mouseX <= x + 230) {
			StartInputString(x + 152, y + 42, 12, m_cVoicePassword, FALSE);
			return;
		}
		if (mouseY >= y + 90 && mouseY <= y + 90 + DEF_BTNSZY &&
			mouseX >= x + DEF_RBTNPOSX && mouseX <= x + DEF_RBTNPOSX + DEF_BTNSZX) {
			

			if (InitVoiceClient(m_cPlayerName, "144.202.42.248", 12345, m_cVoiceRoom, m_cVoicePassword)) {
				m_bVoiceConnected = true;
				mode = 3;
				RequestConnectedUsers("144.202.42.248", 12345, m_cVoiceRoom);
				PlaySound('E', 14, 5);
			}
			else {
				AddEventList("No se pudo conectar a la sala.");
				PlaySound('E', 14, 0);
			}
			return;
		}
	}
	else if (mode == 3) {
		short muteX = x + 180;
		short muteY = y + 30;
		if (mouseX >= muteX && mouseX <= muteX + 100 && mouseY >= muteY && mouseY <= muteY + 15) {
			m_bVoiceMicMuted = !m_bVoiceMicMuted;
			PlaySound('E', 14, 5);
			return;
		}
		if (mouseY >= y + 280 && mouseY <= y + 300) {
			if (mouseX >= x + DEF_LBTNPOSX && mouseX <= x + DEF_LBTNPOSX + DEF_BTNSZX) {
				RequestConnectedUsers("144.202.42.248", 12345, m_cVoiceRoom);
				PlaySound('E', 14, 5);
				return;
			}
			if (mouseX >= x + DEF_RBTNPOSX && mouseX <= x + DEF_RBTNPOSX + DEF_BTNSZX) {
				ShutdownVoiceClient();
				ZeroMemory(m_cVoiceRoom, sizeof(m_cVoiceRoom));
				ZeroMemory(m_cVoicePassword, sizeof(m_cVoicePassword));
				ZeroMemory(m_cVoiceStatus, sizeof(m_cVoiceStatus));
				m_bVoiceConnected = false;
				m_bVoiceTriedConnection = false;
				m_bInputVoiceRoom = false;
				m_bInputVoicePassword = false;
				scroll = 0;
				mode = 0;
				PlaySound('E', 14, 5);
				return;
			}
		}
	}
	else if (mode == 4) {
		if (mouseY >= y + 40 && mouseY <= y + 58 && mouseX >= x + 150 && mouseX <= x + 230) {
			StartInputString(x + 152, y + 42, 12, m_cVoiceRoom, FALSE);
			return;
		}
		if (mouseY >= y + 70 && mouseY <= y + 88 && mouseX >= x + 150 && mouseX <= x + 230) {
			StartInputString(x + 152, y + 72, 12, m_cVoicePassword, FALSE);
			return;
		}
		if (mouseY >= y + 110 && mouseY <= y + 110 + DEF_BTNSZY &&
			mouseX >= x + DEF_RBTNPOSX && mouseX <= x + DEF_RBTNPOSX + DEF_BTNSZX) {
			if (strlen(m_cVoiceRoom) == 0) {
				AddEventList("El nombre no puede estar vacío.");
				PlaySound('E', 14, 0);
				return;
			}
			if (InitVoiceClient(m_cPlayerName, "144.202.42.248", 12345, m_cVoiceRoom, m_cVoicePassword)) {
				m_bVoiceConnected = true;
				mode = 3;
				RequestConnectedUsers("144.202.42.248", 12345, m_cVoiceRoom);
				PlaySound('E', 14, 5);
			}
			else {
				AddEventList("No se pudo crear/conectar a la sala.");
				PlaySound('E', 14, 0);
			}
			return;
		}
	}
}*/

void CGame::sendVoiceCommand(const std::string& command, int value) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;

	SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udpSocket == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5000);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	std::string message = command + " " + std::to_string(value) + "\n";
	sendto(udpSocket, message.c_str(), static_cast<int>(message.size()), 0,
		(sockaddr*)&serverAddr, sizeof(serverAddr));

	closesocket(udpSocket);
	WSACleanup();
}

void CGame::sendMuteMicCommand() {
	sendVoiceCommand("MUTE_MIC", m_bVoiceMicMuted ? 1 : 0);
}

void CGame::sendMuteSoundCommand() {
	sendVoiceCommand("MUTE_SOUND", m_bVoiceSoundMuted ? 1 : 0);
}

/*
std::thread tr1([this]() {
	//std::this_thread::sleep_for(std::chrono::milliseconds(180));
	connectToVoiceServer();
});
tr1.detach();
*/

std::vector<std::string> g_voiceUserList;
std::vector<std::string> m_voiceChannels;

void CGame::RenderVoiceClientDialog(short mouseX, short mouseY, short mouseZ, char leftButton) {
	short x = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sX;
	short y = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sY;
	short w = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sSizeX;
	auto& mode = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].cMode;
	auto& scroll = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sView;

	DrawShadowDialog2(x, y, VOICE_DIALOG_NUMBER);

	PutCenterString_SpriteFont(x, x + w, y + 3, m_sLang == LANG_ESP ? "Chat de Voz" : "Voice Chat", 0, 0, 0);
	PutCenterString_SpriteFont(x, x + w, y + 4, m_sLang == LANG_ESP ? "Chat de Voz" : "Voice Chat", 250, 250, 250);

	const int visibleLines = 10;

	if (mouseZ != 0 && iGetTopDialogBoxIndex() == VOICE_DIALOG_NUMBER) {
		int totalLines = (mode == 0 ? m_voiceChannels.size() : g_voiceUserList.size());
		if (totalLines > visibleLines) {
			scroll -= mouseZ / 120;
			if (scroll < 0) scroll = 0;
			if (scroll > (int)totalLines - visibleLines)
				scroll = (int)totalLines - visibleLines;
			m_DInput.m_sZ = 0;
		}
	}

	switch (mode) {
	case 0: {
		if (m_bVoiceConnected) {
			short muteX = x + 180;
			short muteY = y + 30;
			const char* muteText = m_bVoiceMicMuted ? "Unmute mic" : "Mute mic";
			int r = m_bVoiceMicMuted ? 255 : 50;
			int g = m_bVoiceMicMuted ? 50 : 255;
			int b = 50;
			PutString2(muteX, muteY, (char*)muteText, r, g, b);

			short muteSoundX = x + 180;
			short muteSoundY = y + 50;
			const char* muteSoundText = m_bVoiceSoundMuted ? "Unmute sound" : "Mute sound";
			PutString2(muteSoundX, muteSoundY, (char*)muteSoundText, m_bVoiceSoundMuted ? 255 : 50, m_bVoiceSoundMuted ? 50 : 255, 50);

			PutString2(x + 20, y + 30, m_sLang == LANG_ESP ? "Otras salas disponibles:" : "Other available rooms:", 255, 255, 255);
			int end = (scroll + visibleLines < (int)m_voiceChannels.size()) ? scroll + visibleLines : (int)m_voiceChannels.size();
			for (int i = scroll; i < end; ++i) {
				short lineY = y + 50 + (i - scroll) * 15;
				bool hovered = (mouseY >= lineY && mouseY <= lineY + 15 && mouseX >= x + 20 && mouseX <= x + 150);
				int r = hovered ? 255 : 255;
				int g = hovered ? 255 : 255;
				int b = hovered ? 0 : 255;
				PutString2(x + 20, lineY, (char*)m_voiceChannels[i].c_str(), r, g, b);
			}

			PutAlignedString2(x, x + w, y + 235, "Estas en la sala Publica", 255, 255, 255);
		}
		else {
			if (!m_bVoiceConnecting) {
				/*PutString2(x + 20, y + 40, m_sLang == LANG_ESP ? "Nombre de usuario:" : "Username:", 200, 200, 200);
				DrawShadowMenu(x + 150, y + 40, x + 230, y + 58);
				DrawShadowMenu_DialogBordersNoTitle(x + 150, y + 40, 80, 18);
				PutString2(x + 152, y + 42, m_cVoiceUsername, 255, 255, 255);*/

				PutAlignedString2(x + 35, x + w - 35, y + 60,
					"En algunos casos en Windows 11, el cliente puede minimizarse al conectarse al servidor de voz, si no se vuelve a maximizar solo debes hacerlo desde el menu de inicio.",
					255, 230, 130);
			}
		}
		if (!m_bVoiceConnecting) {
			const char* status = m_bVoiceConnected ?
				(m_sLang == LANG_ESP ? "Conectado al servidor de voz" : "Connected to voice server") :
				(m_sLang == LANG_ESP ? "Desconectado del servidor de voz" : "Disconnected from voice server");

			int r = m_bVoiceConnected ? 0 : 255;
			int g = m_bVoiceConnected ? 255 : 0;
			int b = 0;
			PutAlignedString2(x, x + w, y + 250, (char*)status, r, g, b);
		}

		if (m_bVoiceConnecting) {
			PutAlignedString2(x + 20, x + w - 20, y + 85, "Intentando conectar al servidor... por favor espere.", 0, 255, 0);
			PutAlignedString2(x + 20, x + w - 20, y + 140, "Este proceso puede tomar hasta 10 segundos, ante cualquier problema avisa a un administrador.", 255, 255, 0);
		}
		if (!m_bVoiceConnecting) {
			const char* leftLabel = m_bVoiceConnected ?
				(m_sLang == LANG_ESP ? "Actualizar" : "Refresh") :
				(m_sLang == LANG_ESP ? "Conectar" : "Connect");
			DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 280, (char*)leftLabel);

			if (m_bVoiceConnected) {
				DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 280,
					m_sLang == LANG_ESP ? "Desconectar" : "Disconnect");
			}
		}
		break;
	}



	case 1: {
		char buffer[128];
		sprintf(buffer, m_sLang == LANG_ESP ? "Vas a ingresar a la sala %s" : "You are about to join room %s", m_cVoiceRoom);
		PutAlignedString2(x + 30, x + w - 30, y + 180, buffer, 255, 255, 0);

		if (string(m_cVoiceRoom) != "Cursed Radio" && string(m_cVoiceRoom) != "Elvine" && string(m_cVoiceRoom) != "Aresden")
		{
			PutString2(x + 20, y + 40, m_sLang == LANG_ESP ? "Contraseña de sala:" : "Room password:", 255, 255, 255);
			DrawShadowMenu(x + 150, y + 40, x + 230, y + 58);
			DrawShadowMenu_DialogBordersNoTitle(x + 150, y + 40, 80, 18);
			PutString2(x + 152, y + 42, m_cVoicePassword, 255, 255, 255);
		}
		

		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 90, m_sLang == LANG_ESP ? "Conectar" : "Connect");
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 90, m_sLang == LANG_ESP ? "Atrás" : "Back");
		break;
	}
	case 3: {
		char texting[100];
		int totalusers = g_voiceUserList.size();
		wsprintf(texting, "Usuarios en sala (%d)", totalusers);
		PutString2(x + 20, y + 30, texting, 0, 255, 0);

		int end = (scroll + visibleLines < (int)g_voiceUserList.size()) ? scroll + visibleLines : (int)g_voiceUserList.size();
		for (int i = scroll; i < end; ++i) {
			short lineY = y + 50 + (i - scroll) * 15;
			bool hovered = (mouseY >= lineY && mouseY <= lineY + 15 && mouseX >= x + 20 && mouseX <= x + 250);
			int r = hovered ? 255 : 200;
			int g = hovered ? 255 : 255;
			int b = hovered ? 255 : 255;
			PutString2(x + 20, lineY, (char*)g_voiceUserList[i].c_str(), r, g, b);
		}

		short muteX = x + 180;
		short muteY = y + 30;
		const char* muteText = m_bVoiceMicMuted ? "Unmute mic" : "Mute mic";
		int r = m_bVoiceMicMuted ? 255 : 50;
		int g = m_bVoiceMicMuted ? 50 : 255;
		int b = 50;
		PutString2(muteX, muteY, (char*)muteText, r, g, b);

		short muteSoundX = x + 180;
		short muteSoundY = y + 50;
		const char* muteSoundText = m_bVoiceSoundMuted ? "Unmute sound" : "Mute sound";
		PutString2(muteSoundX, muteSoundY, (char*)muteSoundText, m_bVoiceSoundMuted ? 255 : 50, m_bVoiceSoundMuted ? 50 : 255, 50);

		string roomtext = "Estas en la sala ";
		roomtext.append(m_cVoiceRoom);
		PutAlignedString2(x, x + w, y + 250, (char*)roomtext.c_str(), 0, 255, 0);

		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 280, m_sLang == LANG_ESP ? "Actualizar" : "Refresh");
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 280, m_sLang == LANG_ESP ? "Desconectar" : "Disconnect");
		break;
	}
	}
}
void CGame::HandleVoiceClientClick(short mouseX, short mouseY) {
	short x = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sX;
	short y = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sY;
	auto& mode = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].cMode;
	auto& scroll = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].sView;

	if (mode == 0) {
		if (m_bVoiceConnected)
		{
			short muteX = x + 180;
			short muteY = y + 30;
			if (mouseX >= muteX && mouseX <= muteX + 100 && mouseY >= muteY && mouseY <= muteY + 15) {
				m_bVoiceMicMuted = !m_bVoiceMicMuted;
				sendMuteMicCommand();
				PlaySound('E', 14, 5);
				return;
			}

			short muteSoundX = x + 180;
			short muteSoundY = y + 50;
			if (mouseX >= muteSoundX && mouseX <= muteSoundX + 100 && mouseY >= muteSoundY && mouseY <= muteSoundY + 15) {
				m_bVoiceSoundMuted = !m_bVoiceSoundMuted;
				sendMuteSoundCommand();
				PlaySound('E', 14, 5);
				return;
			}

			int end = (scroll + 10 < (int)m_voiceChannels.size()) ? scroll + 10 : (int)m_voiceChannels.size();
			for (int i = scroll; i < end; ++i) {
				short itemY = y + 50 + (i - scroll) * 15;
				if (mouseY >= itemY && mouseY <= itemY + 15 && mouseX >= x + 20 && mouseX <= x + 150) {
					const char* selectedRoom = m_voiceChannels[i].c_str();

					if (_stricmp(m_cLocation, "elvine") == 0 && _stricmp(selectedRoom, "Aresden") == 0) {
						AddEventList("No puedes conectarte a esta sala siendo elvine.");
						PlaySound('E', 14, 5);
						return;
					}
					if (_stricmp(m_cLocation, "Aresden") == 0 && _stricmp(selectedRoom, "Elvine") == 0) {
						AddEventList("No puedes conectarte a esta sala siendo aresden.");
						PlaySound('E', 14, 5);
						return;
					}

					strcpy(m_cVoiceRoom, selectedRoom);
					mode = 1;
					PlaySound('E', 14, 5);
					return;
				}
			}
		}
		else {
		/*	if (mouseY >= y + 20 && mouseY <= y + 58 && mouseX >= x + 150 && mouseX <= x + 230) {
				StartInputString(x + 152, y + 42, 12, m_cVoiceUsername, FALSE);
				return;
			}*/
		}

		if (mouseY >= y + 280 && mouseY <= y + 300) {
			if (mouseX >= x + DEF_LBTNPOSX && mouseX <= x + DEF_LBTNPOSX + DEF_BTNSZX) {
				if (!m_bVoiceConnecting) {
					if (!m_bVoiceConnected) {
						/*if (strlen(m_cVoiceUsername) == 0 || !IsValidVoiceUsername(m_cVoiceUsername)) {
							AddEventList("Nombre de usuario inválido.");
							return;
							}*/
						m_bVoiceConnecting = true;
						connectToVoiceServer();
					}
				}
				else {
					std::thread tr([this]() {
						SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
						if (udpSocket == INVALID_SOCKET) return;

						sockaddr_in serverAddr = {};
						serverAddr.sin_family = AF_INET;
						serverAddr.sin_port = htons(5000);
						serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

						m_bVoiceConnected = requestAndStoreChannels(udpSocket, serverAddr);

						closesocket(udpSocket);
					});
					tr.detach();
				}
				PlaySound('E', 14, 5);
				return;
			}

			if (m_bVoiceConnected && mouseX >= x + DEF_RBTNPOSX && mouseX <= x + DEF_RBTNPOSX + DEF_BTNSZX) {
				KillProcessByName("mumble.exe");
				ZeroMemory(m_cVoiceRoom, sizeof(m_cVoiceRoom));
				ZeroMemory(m_cVoicePassword, sizeof(m_cVoicePassword));
				ZeroMemory(m_cVoiceStatus, sizeof(m_cVoiceStatus));
				ZeroMemory(m_cVoiceUsername, sizeof(m_cVoiceUsername));
				m_bVoiceConnected = false;
				m_bVoiceInRoom = false;
				m_bVoiceConnecting = false;
				mode = 0;
				PlaySound('E', 14, 5);
				return;
			}
		}
	}
	else if (mode == 1) {
		if (mouseY >= y + 20 && mouseY <= y + 58 && mouseX >= x + 150 && mouseX <= x + 230) {
			if (string(m_cVoiceRoom) != "Cursed Radio" && string(m_cVoiceRoom) != "Elvine" && string(m_cVoiceRoom) != "Aresden")
			{
				StartInputString(x + 152, y + 42, 12, m_cVoicePassword, FALSE);
			}
			return;
		}

		if (mouseY >= y + 90 && mouseY <= y + 90 + DEF_BTNSZY &&
			mouseX >= x + DEF_RBTNPOSX && mouseX <= x + DEF_RBTNPOSX + DEF_BTNSZX) {

			auto equalsIgnoreCase = [](const char* a, const char* b) -> bool {
				while (*a && *b) {
					if (tolower(*a) != tolower(*b)) return false;
					++a; ++b;
				}
				return *a == *b;
			};

			if (equalsIgnoreCase(m_cVoiceRoom, "- Al Qaeda Team -") && strcmp(m_cVoicePassword, "dkw546") != 0) {
				AddEventList("Contraseña incorrecta para Al Qaeda Team.");
				PlaySound('E', 14, 5);
				return;
			}
			if (equalsIgnoreCase(m_cVoiceRoom, "- The Black List Guild -") && strcmp(m_cVoicePassword, "wye321") != 0) {
				AddEventList("Contraseña incorrecta para The Black List Guild.");
				PlaySound('E', 14, 5);
				return;
			}
			if (equalsIgnoreCase(m_cVoiceRoom, "Corea Channel") && strcmp(m_cVoicePassword, "qwe123") != 0) {
				AddEventList("Contraseña incorrecta para Corea Channel.");
				PlaySound('E', 14, 5);
				return;
			}

			m_bVoiceConnected = true;
			m_bVoiceInRoom = true;
			mode = 3;

			std::thread tr([this]() {
				this->connectToVoiceChannel(m_cVoiceRoom, m_cVoicePassword);
			});
			tr.detach();

			PlaySound('E', 14, 5);
			return;
		}

		if (mouseY >= y + 90 && mouseY <= y + 90 + DEF_BTNSZY &&
			mouseX >= x + DEF_LBTNPOSX && mouseX <= x + DEF_LBTNPOSX + DEF_BTNSZX) {
			mode = 0;
			PlaySound('E', 14, 5);
			return;
		}
	}
	else if (mode == 3) {
		short muteX = x + 180;
		short muteY = y + 30;
		if (mouseX >= muteX && mouseX <= muteX + 100 && mouseY >= muteY && mouseY <= muteY + 15) {
			m_bVoiceMicMuted = !m_bVoiceMicMuted;
			sendMuteMicCommand();
			PlaySound('E', 14, 5);
			return;
		}

		short muteSoundX = x + 180;
		short muteSoundY = y + 50;
		if (mouseX >= muteSoundX && mouseX <= muteSoundX + 100 && mouseY >= muteSoundY && mouseY <= muteSoundY + 15) {
			m_bVoiceSoundMuted = !m_bVoiceSoundMuted;
			sendMuteSoundCommand();
			PlaySound('E', 14, 5);
			return;
		}

		if (mouseY >= y + 280 && mouseY <= y + 300) {
			if (mouseX >= x + DEF_LBTNPOSX && mouseX <= x + DEF_LBTNPOSX + DEF_BTNSZX) {
				refreshVoiceUserList();
				PlaySound('E', 14, 5);
				return;
			}
			if (mouseX >= x + DEF_RBTNPOSX && mouseX <= x + DEF_RBTNPOSX + DEF_BTNSZX) {
				KillProcessByName("mumble.exe");
				ZeroMemory(m_cVoiceRoom, sizeof(m_cVoiceRoom));
				ZeroMemory(m_cVoicePassword, sizeof(m_cVoicePassword));
				ZeroMemory(m_cVoiceStatus, sizeof(m_cVoiceStatus));
				ZeroMemory(m_cVoiceUsername, sizeof(m_cVoiceUsername));

				m_bVoiceConnecting = false;
				m_bVoiceConnected = false;
				m_bVoiceInRoom = false;
				mode = 0;
				PlaySound('E', 14, 5);
				return;
			}
		}
	}
}

class VoiceConnectGuard {
public:
	bool* pFlag;

	VoiceConnectGuard(bool* flag) {
		pFlag = flag;
	}

	~VoiceConnectGuard() {
		if (pFlag) *pFlag = false;
	}
};

std::string CGame::SanitizeVoiceUsername(const char* name) {
	std::string result;
	for (size_t i = 0; name[i]; ++i) {
		char c = name[i];
		if (isalnum(c) || c == '-' || c == '_' || c == '.') {
			result += c;
		}
		else {
			result += '_';
		}
	}
	return result;
}


void CGame::connectToVoiceServer() {
	DWORD dwNow = GetTickCount();
	if (dwNow - g_dwLastVoiceConnectTime < 8000) {
		AddEventList("Debes esperar 8 segundos antes de volver a conectarte.");
		return;
	}
	g_dwLastVoiceConnectTime = dwNow;

	// Asegurarse de que no quede "Cargando..." si no iniciamos el hilo
	m_bVoiceConnecting = true;

	KillProcessByName("mumble.exe");
	Sleep(500);

	STARTUPINFOW si = { sizeof(si) };
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	PROCESS_INFORMATION pi;
	if (!CreateProcessW(L"mumble\\mumble.exe", nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
		MessageBoxA(nullptr, "No se pudo lanzar Mumble.exe", "Error", MB_OK | MB_ICONERROR);
		m_bVoiceConnecting = false;
		return;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	std::thread([this] {
		DWORD startTime = GetTickCount();
		VoiceConnectGuard guard(&m_bVoiceConnecting); // Garantiza poner m_bVoiceConnecting = false al salir

		Sleep(3000); // Esperar a que Mumble arranque

		if (IsWindows11OrGreater()) {
			HWND hShellWindow = GetShellWindow();
			if (hShellWindow && hShellWindow != G_hWnd) {
				SetForegroundWindow(hShellWindow);
				Sleep(1000);
				SetForegroundWindow(G_hWnd);
				SetActiveWindow(G_hWnd);
				SetFocus(G_hWnd);
			}
		}

		Sleep(1000);

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			return; // El guard limpiará m_bVoiceConnecting
		}

		SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (udpSocket == INVALID_SOCKET) {
			MessageBoxA(nullptr, "Error al crear socket UDP", "Error", MB_OK | MB_ICONERROR);
			WSACleanup();
			return;
		}

		sockaddr_in serverAddr = {};
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(5000);
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		// Construir comando JOIN
		std::string username = SanitizeVoiceUsername(m_cPlayerName);
		std::string ip = DEF_SERVER_IP;
		std::string joinserverCommand = "JOINSERVER " + username + " " + ip;

		sendto(udpSocket,
			joinserverCommand.c_str(),
			static_cast<int>(joinserverCommand.size()),
			0,
			(sockaddr*)&serverAddr,
			sizeof(serverAddr));

		Sleep(2000);

		bool success = requestAndStoreChannels(udpSocket, serverAddr);

		closesocket(udpSocket);
		WSACleanup();

		DWORD elapsed = GetTickCount() - startTime;

		if (!success || elapsed > 15000) {
			// Reiniciar todo si tarda > 15s o falla
			KillProcessByName("mumble.exe");
			ZeroMemory(m_cVoiceRoom, sizeof(m_cVoiceRoom));
			ZeroMemory(m_cVoicePassword, sizeof(m_cVoicePassword));
			ZeroMemory(m_cVoiceStatus, sizeof(m_cVoiceStatus));
			ZeroMemory(m_cVoiceUsername, sizeof(m_cVoiceUsername));
			m_bVoiceConnected = false;
			m_bVoiceInRoom = false;

			// Volver a modo 0
			auto& mode = m_stDialogBoxInfo[VOICE_DIALOG_NUMBER].cMode;
			mode = 0;

			AddEventList("No se pudo conectar al servidor de voz. Inténtalo de nuevo.");
		}
		else {
			m_bVoiceConnected = true;
		}
	}).detach();
}

bool CGame::requestAndStoreChannels(SOCKET udpSocket, sockaddr_in serverAddr) {
	m_voiceChannels.clear();

	const char* command = "LIST_CHANNELS";
	sendto(udpSocket, command, static_cast<int>(strlen(command)), 0,
		(sockaddr*)&serverAddr, sizeof(serverAddr));

	char buffer[2048] = {};
	sockaddr_in fromAddr = {};
	int fromLen = sizeof(fromAddr);

	// Esperar respuesta
	int bytes = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0,
		(sockaddr*)&fromAddr, &fromLen);

	if (bytes > 0) {
		buffer[bytes] = '\0';
		std::istringstream iss(buffer);
		std::string line;
		while (std::getline(iss, line)) {
			if (line == "END")
				break;
			m_voiceChannels.push_back(line);
		}
	}

	return !m_voiceChannels.empty(); // ✅ solo true si hay canales
}

void CGame::refreshVoiceUserList() {
	std::thread([this]() {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			return;

		SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (udpSocket == INVALID_SOCKET) {
			WSACleanup();
			return;
		}

		sockaddr_in serverAddr = {};
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(5000);
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		const char* listCmd = "LIST_USERS\n";
		sendto(udpSocket, listCmd, (int)strlen(listCmd), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

		char buffer[2048] = {};
		int addrLen = sizeof(serverAddr);
		int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&serverAddr, &addrLen);

		if (bytesReceived > 0) {
			buffer[bytesReceived] = '\0';
			std::string response(buffer);
			g_voiceUserList.clear();
			std::istringstream ss(response);
			std::string line;
			while (std::getline(ss, line)) {
				if (line.find(" - ") == 0)
					g_voiceUserList.push_back(line.substr(3));
			}
		}

		closesocket(udpSocket);
		WSACleanup();
	}).detach();
}


bool CGame::KillProcessByName(const char* exeName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	BOOL found = FALSE;
	if (Process32First(hSnapshot, &pe)) {
		do {
			if (_stricmp(pe.szExeFile, exeName) == 0) {
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
				if (hProcess) {
					TerminateProcess(hProcess, 0);
					CloseHandle(hProcess);
					found = TRUE;
				}
			}
		} while (Process32Next(hSnapshot, &pe));
	}

	CloseHandle(hSnapshot);
	return found;
}
void CGame::connectToVoiceChannel(std::string chan, std::string pass) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;

	SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udpSocket == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5000);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	std::string username = SanitizeVoiceUsername(m_cPlayerName);


	// ✅ Enviar canal y contraseña opcional con comillas escapadas
	std::string joinCommand = "JOIN \"" + chan + "\" \"" + pass + "\"\n";
	sendto(udpSocket, joinCommand.c_str(), (int)joinCommand.length(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

	Sleep(1000);

	const char* listCmd = "LIST_USERS\n";
	sendto(udpSocket, listCmd, (int)strlen(listCmd), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

	char buffer[2048] = {};
	int addrLen = sizeof(serverAddr);
	int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&serverAddr, &addrLen);

	if (bytesReceived > 0) {
		buffer[bytesReceived] = '\0';
		std::istringstream ss(buffer);
		std::string line;
		g_voiceUserList.clear();
		while (std::getline(ss, line)) {
			if (line.find(" - ") == 0)
				g_voiceUserList.push_back(line.substr(3));
		}
	}

	closesocket(udpSocket);
	WSACleanup();
}
// Declaración global

void CGame::launchProgramVisible(const char* path) {
	// Permitir que el proceso hijo tome el foco (muy importante en Win11)
	AllowSetForegroundWindow(ASFW_ANY);

	SHELLEXECUTEINFO sei = { sizeof(sei) };
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpFile = path;
	sei.nShow = SW_SHOWNORMAL;

	if (!ShellExecuteEx(&sei)) {
		MessageBoxA(NULL, "No se pudo lanzar el programa", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	// Si querés, podés usar sei.hProcess para esperar o verificar el proceso
	CloseHandle(sei.hProcess);
}

bool CGame::IsWindows11OrGreater() {
	typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
	RTL_OSVERSIONINFOW rovi = { 0 };
	rovi.dwOSVersionInfoSize = sizeof(rovi);

	HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
	if (hMod) {
		RtlGetVersionPtr fn = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
		if (fn && fn(&rovi) == 0) {
			return (rovi.dwMajorVersion == 10 && rovi.dwBuildNumber >= 22000);
		}
	}
	return false;
}


bool CGame::IsValidVoiceUsername(const char* name) {
	if (strlen(name) == 0 || strlen(name) > 25) return false;
	for (const char* p = name; *p; ++p) {
		if (!isalnum(*p)) return false;
	}
	return true;
}

void CGame::RenderGuildBankDialog(short mouseX, short mouseY, short mouseZ, char leftButton) {
	short x = m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].sX;
	short y = m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].sY;
	short w = m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].sSizeX;
	auto& mode = m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].cMode;

	DrawShadowDialog2(x, y, GUILDBANK_DIALOG_NUMBER);

	PutCenterString_SpriteFont(x, x + w, y + 3, "Guild Bank", 0, 0, 0);
	PutCenterString_SpriteFont(x, x + w, y + 4, "Guild Bank", 250, 250, 250);

	switch (mode) {
	case 0: {
		char buffer[128] = {};

		sprintf(buffer, "Tokens: %d", m_iGuildBankTokens);
		PutString2(x + 20, y + 50, buffer, 255, 255, 255);
		bool hoverDepositTokens = (mouseX >= x + 160 && mouseX <= x + 220 && mouseY >= y + 50 && mouseY <= y + 65);
		bool hoverWithdrawTokens = (mouseX >= x + 230 && mouseX <= x + 290 && mouseY >= y + 50 && mouseY <= y + 65);
		PutString2(x + 160, y + 50, "Depositar", hoverDepositTokens ? 255 : 180, hoverDepositTokens ? 255 : 180, 0);
		PutString2(x + 230, y + 50, "Extraer", hoverWithdrawTokens ? 255 : 180, hoverWithdrawTokens ? 255 : 180, 0);

		sprintf(buffer, "Contrib: %d", m_iGuildBankContribution);
		PutString2(x + 20, y + 70, buffer, 255, 255, 255);
		bool hoverDepositContrib = (mouseX >= x + 160 && mouseX <= x + 220 && mouseY >= y + 70 && mouseY <= y + 85);
		bool hoverWithdrawContrib = (mouseX >= x + 230 && mouseX <= x + 290 && mouseY >= y + 70 && mouseY <= y + 85);
		PutString2(x + 160, y + 70, "Depositar", hoverDepositContrib ? 255 : 180, hoverDepositContrib ? 255 : 180, 0);
		PutString2(x + 230, y + 70, "Extraer", hoverWithdrawContrib ? 255 : 180, hoverWithdrawContrib ? 255 : 180, 0);

		sprintf(buffer, "Majestics: %d", m_iGuildBankMajestics);
		PutString2(x + 20, y + 90, buffer, 255, 255, 255);
		bool hoverDepositMaj = (mouseX >= x + 160 && mouseX <= x + 220 && mouseY >= y + 90 && mouseY <= y + 105);
		bool hoverWithdrawMaj = (mouseX >= x + 230 && mouseX <= x + 290 && mouseY >= y + 90 && mouseY <= y + 105);
		PutString2(x + 160, y + 90, "Depositar", hoverDepositMaj ? 255 : 180, hoverDepositMaj ? 255 : 180, 0);
		PutString2(x + 230, y + 90, "Extraer", hoverWithdrawMaj ? 255 : 180, hoverWithdrawMaj ? 255 : 180, 0);

		if (m_iGuildRank == 0 && _stricmp(m_cGuildName, "NONE") != 0) {
			DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 280, "Contraseña");
		}

		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX + 30, 280, "Actualizar");
		break;
	}
	case 1:
	{
		PutString2(x + 20, y + 80, "Valor a depositar:", 255, 255, 255);
		DrawShadowMenu(x + 150, y + 80, x + 230, y + 98);
		DrawShadowMenu_DialogBordersNoTitle(x + 150, y + 80, 80, 18);
		PutString2(x + 152, y + 82, m_cGuildBankAmount, 255, 255, 255);
		PutCenterString_SpriteFont(x, x + w, y + 40, "Ingresá el valor a depositar...", 255, 255, 255);


		if (m_cGuildBankAction == 1)
		{
			char buffer[128] = {};
			wsprintf(buffer, "Tokens actuales del jugador:  %d", m_iTokens);
			PutAlignedString2(x, x + w, y + 120, buffer, 255, 255, 0);
		}
		else if (m_cGuildBankAction == 2)
		{
			char buffer[128] = {};
			wsprintf(buffer, "Contribucion actual del jugador: %d", m_iContribution);
			PutAlignedString2(x, x + w, y + 120, buffer, 255, 255, 0);
		}
		else if (m_cGuildBankAction == 3)
		{
			char buffer[128] = {};
			wsprintf(buffer, "Majestics actuales del jugador: %d", m_iGizonItemUpgradeLeft);
			PutAlignedString2(x, x + w, y + 120, buffer, 255, 255, 0);
		}

		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 280, "Depositar");
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 280, "Cancelar");
		break;
	}
	case 2:
		PutString2(x + 20, y + 80, "Valor a extraer:", 255, 255, 255);
		DrawShadowMenu(x + 150, y + 80, x + 230, y + 98);
		DrawShadowMenu_DialogBordersNoTitle(x + 150, y + 80, 80, 18);
		PutString2(x + 152, y + 82, m_cGuildBankAmount, 255, 255, 255);
		PutCenterString_SpriteFont(x, x + w, y + 40, "Ingresá el valor a extraer...", 255, 255, 255);

		PutString2(x + 20, y + 110, "Contraseña:", 255, 255, 255);
		DrawShadowMenu(x + 150, y + 110, x + 230, y + 128);
		DrawShadowMenu_DialogBordersNoTitle(x + 150, y + 110, 80, 18);
		PutString2(x + 152, y + 112, m_cGuildBankPassword, 255, 255, 255);

		if (m_cGuildBankAction == 1)
		{
			char buffer[128] = {};
			wsprintf(buffer, "Tokens actuales del guild:  %d", m_iGuildBankTokens);
			PutAlignedString2(x, x + w, y + 140, buffer, 255, 255, 0);
		}
		else if (m_cGuildBankAction == 2)
		{
			char buffer[128] = {};
			wsprintf(buffer, "Contribucion actual del guild: %d", m_iGuildBankContribution);
			PutAlignedString2(x, x + w, y + 140, buffer, 255, 255, 0);
		}
		else if (m_cGuildBankAction == 3)
		{
			char buffer[128] = {};
			wsprintf(buffer, "Majestics actuales del guild: %d", m_iGuildBankMajestics);
			PutAlignedString2(x, x + w, y + 140, buffer, 255, 255, 0);
		}

		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 280, "Extraer");
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 280, "Cancelar");
		break;
	case 3: {
		PutCenterString_SpriteFont(x, x + w, y + 40, "Ingresá la nueva contraseña para la guild", 255, 255, 255);
		PutString2(x + 20, y + 100, "Nueva contraseña:", 255, 255, 255);
		DrawShadowMenu(x + 150, y + 100, x + 230, y + 118);
		DrawShadowMenu_DialogBordersNoTitle(x + 150, y + 100, 80, 18);
		PutString2(x + 152, y + 102, m_cGuildBankPassword, 255, 255, 255);

		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_LBTNPOSX, 280, "Cambiar");
		DrawMenu_SmallButton(x, y, mouseX, mouseY, DEF_RBTNPOSX, 280, "Cancelar");
		break;
	}
	}

}

void CGame::HandleGuildBankClick(short mouseX, short mouseY) {
	short x = m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].sX;
	short y = m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].sY;
	auto& mode = m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].cMode;

	switch (mode) {
	case 0:
	{
		// Tokens
		if (mouseX >= x + 160 && mouseX <= x + 220 && mouseY >= y + 50 && mouseY <= y + 65) {
			m_cGuildBankAction = 1; // Tokens
			m_cGuildBankMode = 1;   // Depositar
			PlaySound('E', 14, 5);
			ZeroMemory(m_cGuildBankAmount, sizeof(m_cGuildBankAmount));
			StartInputString(x + 152, y + 82, 7, m_cGuildBankAmount, FALSE);
			mode = 1;
		}
		else if (mouseX >= x + 230 && mouseX <= x + 290 && mouseY >= y + 50 && mouseY <= y + 65) {
			m_cGuildBankAction = 1;
			m_cGuildBankMode = 2;
			PlaySound('E', 14, 5);
			ZeroMemory(m_cGuildBankPassword, sizeof(m_cGuildBankPassword));
			ZeroMemory(m_cGuildBankAmount, sizeof(m_cGuildBankAmount));
			StartInputString(x + 152, y + 82, 7, m_cGuildBankAmount, FALSE);
			mode = 2;
		}
		// Contribution
		else if (mouseX >= x + 160 && mouseX <= x + 220 && mouseY >= y + 70 && mouseY <= y + 85) {
			m_cGuildBankAction = 2;
			m_cGuildBankMode = 1;
			PlaySound('E', 14, 5);
			ZeroMemory(m_cGuildBankAmount, sizeof(m_cGuildBankAmount));
			StartInputString(x + 152, y + 82, 7, m_cGuildBankAmount, FALSE);
			mode = 1;
		}
		else if (mouseX >= x + 230 && mouseX <= x + 290 && mouseY >= y + 70 && mouseY <= y + 85) {
			m_cGuildBankAction = 2;
			m_cGuildBankMode = 2;
			PlaySound('E', 14, 5);
			ZeroMemory(m_cGuildBankPassword, sizeof(m_cGuildBankPassword));
			ZeroMemory(m_cGuildBankAmount, sizeof(m_cGuildBankAmount));
			StartInputString(x + 152, y + 82, 7, m_cGuildBankAmount, FALSE);
			mode = 2;
		}
		// Majestics
		else if (mouseX >= x + 160 && mouseX <= x + 220 && mouseY >= y + 90 && mouseY <= y + 105) {
			m_cGuildBankAction = 3;
			m_cGuildBankMode = 1;
			PlaySound('E', 14, 5);
			ZeroMemory(m_cGuildBankAmount, sizeof(m_cGuildBankAmount));
			StartInputString(x + 152, y + 82, 7, m_cGuildBankAmount, FALSE);
			mode = 1;
		}
		else if (mouseX >= x + 230 && mouseX <= x + 290 && mouseY >= y + 90 && mouseY <= y + 105) {
			m_cGuildBankAction = 3;
			m_cGuildBankMode = 2;
			PlaySound('E', 14, 5);
			ZeroMemory(m_cGuildBankPassword, sizeof(m_cGuildBankPassword));
			ZeroMemory(m_cGuildBankAmount, sizeof(m_cGuildBankAmount));
			StartInputString(x + 152, y + 82, 7, m_cGuildBankAmount, FALSE);
			mode = 2;
		}

		if (m_iGuildRank == 0 && _stricmp(m_cGuildName, "NONE") != 0 &&
			mouseX >= x + DEF_LBTNPOSX && mouseX <= x + DEF_LBTNPOSX + 90 &&
			mouseY >= y + 280 && mouseY <= y + 296) {
			PlaySound('E', 14, 5);
			ZeroMemory(m_cGuildBankPassword, sizeof(m_cGuildBankPassword));
			StartInputString(x + 152, y + 102, 11, m_cGuildBankPassword, TRUE);
			mode = 3;
			return;
		}

		// Click en botón Actualizar
		if (mouseX >= x + DEF_RBTNPOSX + 30 && mouseX <= x + DEF_RBTNPOSX + 90 + 30 &&
			mouseY >= y + 280 && mouseY <= y + 296) {
			SendChat("/getguildbank");
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}
	case 1: // Depositar
	case 2: { // Extraer
		const int inputX = x + 150;
		const int inputY = y + 80;
		const int inputW = 80;
		const int inputH = 18;

		const int btnY = y + 280;
		const int btnH = 18;

		// Clic en input de valor
		if (mouseX >= inputX && mouseX <= inputX + inputW &&
			mouseY >= inputY && mouseY <= inputY + inputH) {
			StartInputString(inputX + 2, inputY + 2, 7, m_cGuildBankAmount, FALSE);
			PlaySound('E', 14, 5);
			return;
		}

		// Clic en input de contraseña (solo extracción)
		if (mode == 2 &&
			mouseX >= inputX && mouseX <= inputX + inputW &&
			mouseY >= y + 110 && mouseY <= y + 110 + inputH) {
			StartInputString(inputX + 2, y + 110 + 2, 11, m_cGuildBankPassword, TRUE); // con asteriscos
			PlaySound('E', 14, 5);
			return;
		}

		// Botón izquierdo (Depositar o Extraer)
		if (mouseX >= x + DEF_LBTNPOSX && mouseX <= x + DEF_LBTNPOSX + 90 &&
			mouseY >= btnY && mouseY <= btnY + btnH) {

			// Validar que el valor sea numérico
			bool valid = true;
			for (int i = 0; m_cGuildBankAmount[i] != '\0'; ++i) {
				if (m_cGuildBankAmount[i] < '0' || m_cGuildBankAmount[i] > '9') {
					valid = false;
					break;
				}
			}

			int val = atoi(m_cGuildBankAmount);
			if (!valid || val <= 0 || val > 100000) {
				AddEventList("El valor ingresado no es válido.", 10);
				PlaySound('E', 2, 5);
				return;
			}

			char data[100];
			auto* cp = data;

			if (mode == 1)
				Push(cp, (u32)NOT_GUILDBANKDEPOSIT);
			else
				Push(cp, (u32)NOT_GUILDBANKWITHDRAW);

			Push(cp, (u16)1); // protocolo
			Push(cp, (short)m_cGuildBankAction); // recurso
			Push(cp, val); // valor
			if (mode == 2) Push(cp, m_cGuildBankPassword, 16); // solo si es extracción

			m_pGSock->iSendMsg(data, cp - data);
			m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].cMode = 0;
			PlaySound('E', 14, 5);
			return;
		}

		// Botón derecho (Cancelar)
		if (mouseX >= x + DEF_RBTNPOSX && mouseX <= x + DEF_RBTNPOSX + 90 &&
			mouseY >= btnY && mouseY <= btnY + btnH) {
			m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].cMode = 0;
			PlaySound('E', 14, 5);
			return;
		}

		break;
	}
	case 3:
		// input contraseña
		if (mouseX >= x + 150 && mouseX <= x + 230 &&
			mouseY >= y + 100 && mouseY <= y + 118) {
			StartInputString(x + 152, y + 102, 11, m_cGuildBankPassword, TRUE);
			PlaySound('E', 14, 5);
			return;
		}

		// Botón Cambiar
		if (mouseX >= x + DEF_LBTNPOSX && mouseX <= x + DEF_LBTNPOSX + 90 &&
			mouseY >= y + 280 && mouseY <= y + 296) {
			/*if (strlen(m_cGuildBankPassword) < 3) {*//*AddEventList("La contraseña debe tener al menos 3 caracteres.", 10);
				PlaySound('E', 2, 5);
				return;*/
				
			/*}*/
			char data[100], *cp = data;
			Push(cp, (u32)NOT_GUILDBANKPASSWORD);
			Push(cp, (u16)1);
			Push(cp, m_cGuildBankPassword, 16);
			m_pGSock->iSendMsg(data, cp - data);
			m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].cMode = 0;
			PlaySound('E', 14, 5);
			return;
		}

		// Botón Cancelar
		if (mouseX >= x + DEF_RBTNPOSX && mouseX <= x + DEF_RBTNPOSX + 90 &&
			mouseY >= y + 280 && mouseY <= y + 296) {
			m_stDialogBoxInfo[GUILDBANK_DIALOG_NUMBER].cMode = 0;
			PlaySound('E', 14, 5);
			return;
		}
		break;

	}
}

// En CGame.h (o donde declares la clase)
DWORD m_dwLastPingSentTime = 0;   // Tiempo cuando se envió
DWORD m_dwLastPingDelay = 0;      // Ping en ms (resultado)


void CGame::testingpingserver()
{
	char data[6];
	auto* cp = data;

	Push(cp, (u32)MSG_TESTPING);
	Push(cp, (u16)1);

	// Guardar tiempo actual en ms
	m_dwLastPingSentTime = timeGetTime(); // o GetTickCount()

	m_pGSock->iSendMsg(data, cp - data);
}


void CGame::responseping()
{
	DWORD now = timeGetTime();
	m_dwLastPingDelay = now - m_dwLastPingSentTime;

	char cMsg[64];
	wsprintf(cMsg, "Ping: %u ms", m_dwLastPingDelay);
	AddEventList(cMsg, 10); // o cualquier sistema de mensajes que uses
}

void CGame::DrawPortal(int portalX, int portalY, int relicType)
{
	int tileX = m_sViewPointX / 32;
	int tileY = m_sViewPointY / 32;

	bool inPortal1 = (tileX <= portalX     && portalX <= tileX + 20) &&
		(tileY <= portalY     && portalY <= tileY + 15);
	bool inPortal2 = (tileX <= portalX + 3 && portalX + 3 <= tileX + 20) &&
		(tileY <= portalY + 1 && portalY + 1 <= tileY + 15);

	if (inPortal1 || inPortal2)
	{
		DrawNewDialogBox2(SPRID_DYNPORTAL + portaltile,
			portalX * 32 - m_sViewPointX,
			portalY * 32 - m_sViewPointY,
			0, FALSE, FALSE);

		if (m_sRelicStatus == relicType)
		{
			DrawNewDialogBox(SPRID_RELIC,
				(portalX + 3) * 32 - m_sViewPointX - 10,
				(portalY + 1) * 32 - m_sViewPointY - 50,
				0, FALSE, FALSE);
		}
	}
}

int CGame::CheckMenu_Button(short msX, short msY, short boxX, short boxY, short btnX, short btnY, short btnWidth, short btnHeight)
{
	if (msX >= boxX + btnX && msX <= boxX + btnX + btnWidth &&
		msY >= boxY + btnY && msY <= boxY + btnY + btnHeight)
		return 1; 
	return 0;
}

void CGame::draw_ekquestlist(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 91;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	int tmpindex = -1;

	DrawShadowDialog2(sX, sY, 91);
		
	switch (mode)
	{
		case 0:
		{

			PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Quest List", 0, 0, 0);
			PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Quest List", 250, 250, 250);

			int y1 = sY + 40;
			bool hover1 = (msY >= y1 && msY <= y1 + 15 && msX >= sX + 10 && msX <= sX + sizeX - 10);
			PutAlignedString2(
				sX + 10, sX + sizeX - 10, y1,
				"Mobs quests",
				hover1 ? 255 : 255,
				hover1 ? 165 : 255,
				hover1 ? 0 : 255
				);

			int y2 = sY + 60;
			bool hover2 = (msY >= y2 && msY <= y2 + 15 && msX >= sX + 10 && msX <= sX + sizeX - 10);
			PutAlignedString2(
				sX + 10, sX + sizeX - 10, y2,
				"Enemy kill quests",
				hover2 ? 255 : 255,
				hover2 ? 165 : 255,
				hover2 ? 0 : 255
				);
			break;
		}

		case 1:
		{
			PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Enemy Kill Quests", 0, 0, 0);
			PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Enemy Kill Quests", 250, 250, 250);

			PutString2(sX + 23, sY + 33, "Quest list", 228, 161, 6);
			PutString2(sX + 23, sY + 55, "Reward", 228, 161, 6);
			PutString2(sX + 165, sY + 55, "Enemy Kills", 228, 161, 6);
			PutString2(sX + 240, sY + 55, "Map", 228, 161, 6);

			int iTotalLines = _countof(ek_quests);
			int iPointerLoc = 0;
			double d1, d2, d3;

			// scroll
			if (iTotalLines > maxlines)
				DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);

			if (iGetTopDialogBoxIndex() == dnumber && msZ != 0) {
				m_stDialogBoxInfo[dnumber].sView -= msZ / 60;
				m_DInput.m_sZ = 0;
			}
			if (m_stDialogBoxInfo[dnumber].sView < 0) m_stDialogBoxInfo[dnumber].sView = 0;
			if (iTotalLines > maxlines && m_stDialogBoxInfo[dnumber].sView > iTotalLines - maxlines)
				m_stDialogBoxInfo[dnumber].sView = iTotalLines - maxlines;

			if (iTotalLines > maxlines) {
				d1 = (double)m_stDialogBoxInfo[dnumber].sView;
				d2 = (double)(iTotalLines - maxlines);
				d3 = (274.0f * d1) / d2;
				iPointerLoc = (int)(d3 + 0.5);
				DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 342, sY + 35 + iPointerLoc, 7);
			}

			for (int i = 0; i < maxlines; i++) {
				int realIndex = i + m_stDialogBoxInfo[dnumber].sView;
				if (realIndex >= iTotalLines) break;

				auto& quest = ek_quests[realIndex];
				int y = sY + 75 + (15 * i);

				bool hover = (msX > sX + 20 && msX < sX + 327 && msY > y && msY < y + 15);

				int r = hover ? 228 : 255;
				int g = hover ? 161 : 255;
				int b = hover ? 6 : 255;

				PutString2(sX + 23, y, quest.m_cRewardName, r, g, b);
				PutString2(sX + 165, y, (char*)std::to_string(quest.m_iMaxKills).c_str(), r, g, b);
				PutString2(sX + 240, y, quest.m_cShowMapName, r, g, b);

				if (hover) tmpindex = realIndex;
			}

			// Scroll con clic
			if (cLB != 0 && iTotalLines > maxlines) {
				if (iGetTopDialogBoxIndex() == dnumber) {
					if ((msX >= sX + 334) && (msX <= sX + 346) && (msY >= sY + 40) && (msY <= sY + 320)) {
						d1 = (double)(msY - (sY + 35));
						d2 = (double)(iTotalLines - maxlines);
						d3 = (d1 * d2) / 274.0f;
						iPointerLoc = (int)d3;
						if (iPointerLoc > iTotalLines - maxlines) iPointerLoc = iTotalLines - maxlines;
						m_stDialogBoxInfo[dnumber].sView = iPointerLoc;
					}
				}
			}
			else {
				m_stDialogBoxInfo[dnumber].bIsScrollSelected = FALSE;
			}

			break;
		}

		case 2:
		{
			PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Start Quest?", 0, 0, 0);
			PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Start Quest?", 255, 255, 255);

			if (m_iSelectedEkQuestIndex >= 0 && m_iSelectedEkQuestIndex < _countof(ek_quests))
			{
				auto& quest = ek_quests[m_iSelectedEkQuestIndex];

				int y = sY + 50;
				PutString2(sX + 30, y, "Reward:", 255, 255, 255);
				PutString2(sX + 120, y, quest.m_cRewardName, 228, 228, 0);

				y += 20;
				PutString2(sX + 30, y, "Enemy Kills:", 255, 255, 255);
				PutString2(sX + 120, y, (char*)std::to_string(quest.m_iMaxKills).c_str(), 255, 200, 200);

				y += 20;
				PutString2(sX + 30, y, "Map:", 255, 255, 255);
				PutString2(sX + 120, y, quest.m_cShowMapName, 200, 255, 200);
			}

			// Botones
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Aceptar");
			DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "Cancelar");

			break;
		}

	}
}

void CGame::click_ekquestlist(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 91;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto& mode = m_stDialogBoxInfo[dnumber].cMode;

	switch (mode)
	{
		case 0:
		{
			int y1 = sY + 40;
			int y2 = sY + 60;

			if (msY >= y1 && msY <= y1 + 15 && msX >= sX + 10 && msX <= sX + sizeX - 10)
			{
				mode = 1; 
				PlaySound('E', 14, 5);
				SendChat("/getquests");
				EnableDialogBox(77, NULL, NULL, NULL);
				m_stDialogBoxInfo[77].sX = m_stDialogBoxInfo[dnumber].sX;
				m_stDialogBoxInfo[77].sY = m_stDialogBoxInfo[dnumber].sY;
				DisableDialogBox(dnumber);
			}
	
			else if (msY >= y2 && msY <= y2 + 15 && msX >= sX + 10 && msX <= sX + sizeX - 10)
			{
				mode = 1; 
				PlaySound('E', 14, 5);
			}
			break;
		}
		case 1:
		{
			int iTotalLines = _countof(ek_quests);
			int view = m_stDialogBoxInfo[dnumber].sView;

			for (int i = 0; i < maxlines; ++i)
			{
				int realIndex = i + view;
				if (realIndex >= iTotalLines) break;

				int y = sY + 75 + (15 * i);

				if (msX > sX + 20 && msX < sX + 327 && msY > y && msY < y + 15)
				{
					// Quest clickeada
					auto& quest = ek_quests[realIndex];

					m_iSelectedEkQuestIndex = realIndex;
					mode = 2;
					PlaySound('E', 14, 5);
					break;
				}
			}
			break;
		}
		case 2:
		{
			int btn = CheckMenu_Button(msX, msY, sX, sY, 30, 290, 76, 22); // Accept
			if (btn == 1 && m_iSelectedEkQuestIndex >= 0)
			{
				auto& quest = ek_quests[m_iSelectedEkQuestIndex];

				char data[512];
				auto* cp = data;

				Push(cp, (u32)REQ_STARTEKQUEST);
				Push(cp, (u16)1);
				Push(cp, quest.m_iQuestID);

				m_pGSock->iSendMsg(data, cp - data);
				PlaySound('E', 14, 5);

				m_iSelectedEkQuestIndex = -1;
				mode = 1;
				break;
			}

			btn = CheckMenu_Button(msX, msY, sX, sY, 246, 290, 76, 22); // Cancel
			if (btn == 1)
			{
				PlaySound('E', 14, 5);
				m_iSelectedEkQuestIndex = -1;
				mode = 1;
				break;
			}

			break;
		}

	}
}

void CGame::get_charekquests(char* data)
{
	vec_charekquest.clear();
	short size = 0;

	Pop(data, size);

	for (int i = 0; i < size; i++)
	{
		CharacterEk_Quests u;

		Pop(data, u.m_iParentQuestID);
		Pop(data, u.m_cMapName, 12);
		Pop(data, u.m_cShowMapName, 22);
		Pop(data, u.m_cRewardName, 50);
		Pop(data, u.m_iKills);
		Pop(data, u.m_iMaxKills);
		short sStatus;
		Pop(data, sStatus);
		u.status = sStatus != 0;
		Pop(data, u.m_sCompleted);

		vec_charekquest.push_back(u);
	}
}

void CGame::getPlayerRoleData(char* data)
{
	Pop(data, m_sPlayerRole);
	Pop(data, m_sRaiderLvl); 
	Pop(data, m_sHunterLvl);
}

void CGame::draw_ekcharquestlist(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 92;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	int tmpindex = -1;

	DrawShadowDialog2(sX, sY, 92);

	// Detectar hover sobre "<<"
	bool hoverBack = (msX >= sX + 5 && msX <= sX + 5 + 20 && msY >= sY + 5 && msY <= sY + 5 + 12);

	// Dibujar "<<", cambia color si hover
	PutString2(sX + 5, sY + 5, "<<",
		hoverBack ? 255 : 255,
		hoverBack ? 165 : 255,
		hoverBack ? 0 : 255
		);

	switch (mode)
	{
	case 0:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Quest List", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Quest List", 250, 250, 250);

		int y1 = sY + 40;
		bool hover1 = (msY >= y1 && msY <= y1 + 15 && msX >= sX + 10 && msX <= sX + sizeX - 10);
		PutAlignedString2(
			sX + 10, sX + sizeX - 10, y1,
			"Mobs quests",
			hover1 ? 255 : 255,
			hover1 ? 165 : 255,
			hover1 ? 0 : 255
			);

		int y2 = sY + 60;
		bool hover2 = (msY >= y2 && msY <= y2 + 15 && msX >= sX + 10 && msX <= sX + sizeX - 10);
		PutAlignedString2(
			sX + 10, sX + sizeX - 10, y2,
			"Enemy kill quests",
			hover2 ? 255 : 255,
			hover2 ? 165 : 255,
			hover2 ? 0 : 255
			);
		break;
	}

	case 1:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Misiones activas", 255, 255, 0);

		if (vec_charekquest.empty())
		{
			PutAlignedString(sX, sX + sizeX, sY + 60, "No hay misiones activas.", 255, 255, 255);
			break;
		}

		const int total = vec_charekquest.size();
		short& view = m_stDialogBoxInfo[dnumber].sView;
		int pointerLoc = 0;
		double d1, d2, d3;

		if (iGetTopDialogBoxIndex() == dnumber && msZ != 0) {
			view -= msZ / 60;
			m_DInput.m_sZ = 0;
		}
		if (view < 0) view = 0;
		if (total > maxlines && view > total - maxlines) view = total - maxlines;

		if (total > maxlines)
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 100, sY, 3);

		if (total > maxlines) {
			d1 = static_cast<double>(view);
			d2 = static_cast<double>(total - maxlines);
			d3 = (274.0f * d1) / d2;
			pointerLoc = static_cast<int>(d3 + 0.5);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 342, sY + 35 + pointerLoc, 7);
		}

		for (int i = 0; i < maxlines; ++i)
		{
			int index = i + view;
			if (index >= total) break;

			auto& q = vec_charekquest[index];
			int y = sY + 40 + (i * 15);

			bool hover = (msX >= sX + 10 && msX <= sX + sizeX - 10 && msY >= y && msY <= y + 15);
			int r, g, b;

			if (hover) {
				r = 228; g = 161; b = 6; // Naranja: hover
				tmpindex = index;
			}
			else if (q.status) {
				r = 0; g = 255; b = 0; // Verde: activa
			}
			else if (q.m_sCompleted == 1) {
				r = 255; g = 255; b = 0; // Amarillo: completada
			}
			else {
				r = 255; g = 0; b = 0; // Rojo puro: inactiva
			}

			char label[256];
			sprintf_s(label, "%d enemigos en %s -> %s (%d/%d)",
				q.m_iMaxKills, q.m_cShowMapName, q.m_cRewardName, q.m_iKills, q.m_iMaxKills);

			PutString2(sX + 10, y, label, r, g, b);
		}

		if (cLB != 0 && total > maxlines) {
			if (iGetTopDialogBoxIndex() == dnumber) {
				if ((msX >= sX + 334) && (msX <= sX + 346) && (msY >= sY + 40) && (msY <= sY + 320)) {
					d1 = static_cast<double>(msY - (sY + 35));
					d2 = static_cast<double>(total - maxlines);
					d3 = (d1 * d2) / 274.0f;
					pointerLoc = static_cast<int>(d3);
					if (pointerLoc > total - maxlines) pointerLoc = total - maxlines;
					view = pointerLoc;
				}
			}
		}
		else {
			m_stDialogBoxInfo[dnumber].bIsScrollSelected = FALSE;
		}

		break;
	}


	case 2:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Informacion de la mision", 255, 255, 255);

		if (vec_charekquest.size() == 0) break;
		if (m_iSelectedCharEkQuestIndex >= 0 && m_iSelectedCharEkQuestIndex < vec_charekquest.size())
		{
			auto& q = vec_charekquest[m_iSelectedCharEkQuestIndex];

			int y = sY + 50;
			PutString2(sX + 30, y, "Recompensa:", 255, 255, 255);
			PutString2(sX + 120, y, q.m_cRewardName, 255, 255, 0);

			y += 20;
			PutString2(sX + 30, y, "Progreso:", 255, 255, 255);
			char progress[32]; sprintf_s(progress, "%d / %d", q.m_iKills, q.m_iMaxKills);
			PutString2(sX + 120, y, progress, 255, 220, 220);

			y += 20;
			PutString2(sX + 30, y, "Mapa:", 255, 255, 255);
			PutString2(sX + 120, y, q.m_cShowMapName, 200, 255, 200);

			// Botones
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, q.status ? "Desactivar" : "Activar");

			if (q.m_sCompleted == 1)
				DrawMenu_SmallButton(sX, sY, msX, msY, 138, 290, "Recompensa");
			else
				DrawMenu_SmallButton(sX, sY, msX, msY, 138, 290, "Eliminar");

			DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "Atras");
		}
		break;
	}

	case 3:
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Eliminar mision", 255, 100, 100);

		if (vec_charekquest.size() == 0) break;
		if (m_iSelectedCharEkQuestIndex >= 0 && m_iSelectedCharEkQuestIndex < vec_charekquest.size())
		{
			auto& q = vec_charekquest[m_iSelectedCharEkQuestIndex];

			int y = sY + 50;
			PutString2(sX + 30, y, "¿Estas seguro que quieres eliminar esta mision?", 255, 100, 100);

			y += 20;
			char resumen[256];
			sprintf_s(resumen, "%d enemigos en %s -> %s (%d/%d)", q.m_iMaxKills, q.m_cShowMapName, q.m_cRewardName, q.m_iKills, q.m_iMaxKills);
			PutString2(sX + 30, y, resumen, 255, 200, 200);

			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 290, "Eliminar");
			DrawMenu_SmallButton(sX, sY, msX, msY, 246, 290, "Cancelar");
		}
		break;
	}
	}
}
void CGame::click_ekcharquestlist(short msX, short msY)
{
	short sX, sY, sizeX;
	int dnumber = 92;
	int maxlines = 16;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto& mode = m_stDialogBoxInfo[dnumber].cMode;

	bool hoverBack = (msX >= sX + 5 && msX <= sX + 5 + 20 && msY >= sY + 5 && msY <= sY + 5 + 12);
	if (hoverBack)
	{
		if (mode != 0)
		{
			mode--;
		}		
		PlaySound('E', 14, 5);
		return;
	}

	switch (mode)
	{
	case 0:
	{
		int y1 = sY + 40;
		int y2 = sY + 60;

		if (msY >= y1 && msY <= y1 + 15 && msX >= sX + 10 && msX <= sX + sizeX - 10)
		{
			/*mode = 1;*/
			PlaySound('E', 14, 5);
			SendChat("/getcharquests");
			EnableDialogBox(78, NULL, NULL, NULL);
			m_stDialogBoxInfo[78].sX = m_stDialogBoxInfo[dnumber].sX;
			m_stDialogBoxInfo[78].sY = m_stDialogBoxInfo[dnumber].sY;
			DisableDialogBox(dnumber);
		}

		else if (msY >= y2 && msY <= y2 + 15 && msX >= sX + 10 && msX <= sX + sizeX - 10)
		{
			SendChat("/getcharekquests");			
			mode = 1;
			PlaySound('E', 14, 5);
		}
		break;
	}
	case 1:
	{
		if (vec_charekquest.size() == 0) break;
		for (int i = 0; i < maxlines; ++i)
		{
			int index = i + m_stDialogBoxInfo[dnumber].sView;
			if (index >= vec_charekquest.size()) break;

			int y = sY + 40 + (i * 15);
			if (msX > sX + 20 && msX < sX + sizeX - 20 && msY > y && msY < y + 15)
			{
				m_iSelectedCharEkQuestIndex = index;
				mode = 2;
				PlaySound('E', 14, 5);
				break;
			}
		}
		break;
	}

	case 2:
	{
		if (vec_charekquest.size() == 0) break;

		// Activar/Desactivar
		if (CheckMenu_Button(msX, msY, sX, sY, 30, 290, 76, 22))
		{
			if (m_iSelectedCharEkQuestIndex >= 0 && m_iSelectedCharEkQuestIndex < vec_charekquest.size())
			{
				auto& q = vec_charekquest[m_iSelectedCharEkQuestIndex];
				reqChangeEkQuestStatus(q.status, q.m_iParentQuestID);
				PlaySound('E', 14, 5);
			}
		}

		// Eliminar o Recompensa
		if (CheckMenu_Button(msX, msY, sX, sY, 138, 290, 76, 22)) {
			if (m_iSelectedCharEkQuestIndex >= 0 && m_iSelectedCharEkQuestIndex < vec_charekquest.size()) {
				auto& q = vec_charekquest[m_iSelectedCharEkQuestIndex];

				if (q.m_sCompleted == 1) {
					// Pedir recompensa
					reqGetEkQuestReward(q.m_iParentQuestID);
					mode--;
				}
				else {
					// Confirmar eliminación
					mode = 3;
				}
				PlaySound('E', 14, 5);
			}
		}


		// Atrás
		if (CheckMenu_Button(msX, msY, sX, sY, 246, 290, 76, 22))
		{
			m_iSelectedCharEkQuestIndex = -1;
			mode = 1;
			PlaySound('E', 14, 5);
		}
		break;
	}

	case 3:
	{
		// Confirmar eliminación

		if (CheckMenu_Button(msX, msY, sX, sY, 30, 290, 76, 22)) {
			if (m_iSelectedCharEkQuestIndex >= 0 && m_iSelectedCharEkQuestIndex < vec_charekquest.size()) {
				auto& q = vec_charekquest[m_iSelectedCharEkQuestIndex];
				
				DeleteCharQuest(q.m_iParentQuestID);

				m_iSelectedCharEkQuestIndex = -1;
				mode = 1;
				PlaySound('E', 14, 5);
			}
		}

		// Cancelar
		if (CheckMenu_Button(msX, msY, sX, sY, 246, 290, 76, 22)) {
			mode = 2;
			PlaySound('E', 14, 5);
		}
		break;
	}
	}
}

void CGame::DeleteCharQuest(int quest_id)
{
	char data[10];
	auto* cp = data;

	Push(cp, (u32)REQ_DELETECHAREKQUEST);
	Push(cp, (u16)1);
	Push(cp, quest_id);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::EnableCharQuest(int quest_id)
{
	char data[10];
	auto* cp = data;

	Push(cp, (u32)REQ_ENABLECHAREKQUEST);
	Push(cp, (u16)1);
	Push(cp, quest_id);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::reqGetEkQuestReward(int quest_id)
{
	char data[10];
	auto* cp = data;
	Push(cp, (u32)REQ_GETREWARCHARQUEST);
	Push(cp, (u16)1);
	Push(cp, quest_id);
	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::reqChangeEkQuestStatus(bool status, int quest_id)
{
	char data[10];
	auto* cp = data;

	Push(cp, (u32)(status ? REQ_DISABLECHAREKQUEST : REQ_ENABLECHAREKQUEST));
	Push(cp, (u16)1);
	Push(cp, quest_id);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::draw_characterrole(short msX, short msY, short msZ, char cLB)
{
	short sX = m_stDialogBoxInfo[93].sX;
	short sY = m_stDialogBoxInfo[93].sY;
	short sizeX = m_stDialogBoxInfo[93].sSizeX;
	short sizeY = m_stDialogBoxInfo[93].sSizeY;
	auto mode = m_stDialogBoxInfo[93].cMode;

	DrawShadowDialog2(sX, sY, 93);

	const int btnLeft = sX + 41;
	const int btnRight = sX + 141;
	const int btnCenter = sX + 91;

	switch (mode)
	{
	case 0: // Menú principal con descripción y estado actual
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Player Role Manager", 0, 0, 0);
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Player Role Manager", 250, 250, 250);

		/*	// Descripciones de los roles
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 45, "Raider: +10 PvP damage per level.", 255, 255, 0);
		r = 228; g = 161; b = 6; // Naranja: hover*/

		PutAlignedString(sX + 10, sX + sizeX - 10, sY + 60, "Character roles information", 228, 161, 6);
		PutAlignedString(sX + 10, sX + sizeX - 10, sY + 75, "Raider: +10 PvP damage per level", 255, 255, 255);
		PutAlignedString(sX + 10, sX + sizeX - 10, sY + 90, "Hunter: +10 PvE damage per level", 255, 255, 255);

		// Estado actual del jugador
		const char* role =
			m_sPlayerRole == 1 ? "Raider" :
			m_sPlayerRole == 2 ? "Hunter" : "None";

		char txt[64];
		sprintf_s(txt, "Current Role: %s", role);
		PutAlignedString(sX + 10, sX + sizeX - 10, sY + 140, txt, 228, 161, 6);

		sprintf_s(txt, "Raider Level: %d/2", m_sRaiderLvl);
		PutAlignedString(sX + 10, sX + sizeX - 10, sY + 155, txt, 255, 255, 255);

		sprintf_s(txt, "Hunter Level: %d/3", m_sHunterLvl);
		PutAlignedString(sX + 10, sX + sizeX - 10, sY + 170, txt, 255, 255, 255);

		// Botones
		const int btnLeft = sX + 41;
		const int btnRight = sX + 141;
		DrawMenu_SmallButton(sX, sY, msX, msY, btnLeft - sX, 250, "Upgrade Role");
		DrawMenu_SmallButton(sX, sY, msX, msY, btnRight - sX, 250, "Change Role");
		break;
	}



	case 1: // Mostrar niveles y botones para upgrade
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Upgrade Player Role", 255, 255, 0);

		PutString2(sX + 30, sY + 50, "Raider Level:", 255, 255, 255);
		char r_lvl[16]; sprintf_s(r_lvl, "%d/2", m_sRaiderLvl);
		PutString2(sX + 150, sY + 50, r_lvl, 0, 255, 0);

		PutString2(sX + 30, sY + 70, "Hunter Level:", 255, 255, 255);
		char h_lvl[16]; sprintf_s(h_lvl, "%d/3", m_sHunterLvl);
		PutString2(sX + 150, sY + 70, h_lvl, 0, 255, 0);

		// Botones nuevos para upgrade
		const int btnLeft = sX + 41;
		const int btnRight = sX + 141;
		const int btnCenter = sX + 91;

		DrawMenu_SmallButton(sX, sY, msX, msY, btnLeft - sX, 110, "Up Raider");
		DrawMenu_SmallButton(sX, sY, msX, msY, btnRight - sX, 110, "Up Hunter");

		DrawMenu_SmallButton(sX, sY, msX, msY, btnCenter - sX, 290, "Back");
		break;
	}


	case 2: // Activar/cambiar rol
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Activate Player Role", 255, 255, 255);

		PutString2(sX + 30, sY + 60, "Current Role:", 255, 255, 255);
		const char* role =
			m_sPlayerRole == 1 ? "Raider" :
			m_sPlayerRole == 2 ? "Hunter" : "None";
		PutString2(sX + 150, sY + 60, (char*)role, 200, 255, 200);

		DrawMenu_SmallButton(sX, sY, msX, msY, btnLeft - sX, 100, "Select Raider");
		DrawMenu_SmallButton(sX, sY, msX, msY, btnRight - sX, 100, "Select Hunter");
		DrawMenu_SmallButton(sX, sY, msX, msY, btnCenter - sX, 290, "Back");
		break;
	}

	case 3: // Confirmar cambio a Raider
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Confirm Raider Role", 255, 100, 100);
		PutString2(sX + 30, sY + 60, "¿Activar Raider a cambio de 250 EKs?", 255, 255, 255);
		DrawMenu_SmallButton(sX, sY, msX, msY, btnLeft - sX, 290, "Aceptar");
		DrawMenu_SmallButton(sX, sY, msX, msY, btnRight - sX, 290, "Cancelar");
		break;

	case 4: // Confirmar cambio a Hunter
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Confirm Hunter Role", 255, 100, 100);
		PutString2(sX + 30, sY + 60, "¿Activar Hunter a cambio de 250 EKs?", 255, 255, 255);
		DrawMenu_SmallButton(sX, sY, msX, msY, btnLeft - sX, 290, "Aceptar");
		DrawMenu_SmallButton(sX, sY, msX, msY, btnRight - sX, 290, "Cancelar");
		break;

	case 5: // Confirmar mejora Raider
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Confirm Raider Upgrade", 255, 100, 100);
		PutString2(sX + 30, sY + 60, "¿Subir Raider a cambio de 15,000 EKs?", 255, 255, 255);
		DrawMenu_SmallButton(sX, sY, msX, msY, btnLeft - sX, 290, "Aceptar");
		DrawMenu_SmallButton(sX, sY, msX, msY, btnRight - sX, 290, "Cancelar");
		break;

	case 6: // Confirmar mejora Hunter
	{
		PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Confirm Hunter Upgrade", 255, 100, 100);

		int cost = (m_sHunterLvl + 1) * 5000;
		char text[128];
		sprintf_s(text, "¿Subir Hunter a cambio de %d EKs?", cost);
		PutString2(sX + 30, sY + 60, text, 255, 255, 255);

		const int btnLeft = sX + 41;
		const int btnRight = sX + 141;
		DrawMenu_SmallButton(sX, sY, msX, msY, btnLeft - sX, 290, "Aceptar");
		DrawMenu_SmallButton(sX, sY, msX, msY, btnRight - sX, 290, "Cancelar");
		break;
	}

	}
}

void CGame::click_characterrole(short msX, short msY)
{
	short sX = m_stDialogBoxInfo[93].sX;
	short sY = m_stDialogBoxInfo[93].sY;
	auto& mode = m_stDialogBoxInfo[93].cMode;

	switch (mode)
	{
	case 0:
		if (CheckMenu_Button(msX, msY, sX, sY, 41, 250, 76, 22)) {
			mode = 1;
			PlaySound('E', 14, 5);
		}
		else if (CheckMenu_Button(msX, msY, sX, sY, 141, 250, 76, 22)) {
			mode = 2;
			PlaySound('E', 14, 5);
		}
		break;


	case 1:
		if (CheckMenu_Button(msX, msY, sX, sY, 41, 110, 76, 22)) {
			mode = 5; // Confirm upgrade Raider
			PlaySound('E', 14, 5);
		}
		else if (CheckMenu_Button(msX, msY, sX, sY, 141, 110, 76, 22)) {
			mode = 6; // Confirm upgrade Hunter
			PlaySound('E', 14, 5);
		}
		else if (CheckMenu_Button(msX, msY, sX, sY, 91, 290, 76, 22)) {
			mode = 0;
			PlaySound('E', 14, 5);
		}
		break;


	case 2:
		if (CheckMenu_Button(msX, msY, sX, sY, 41, 100, 76, 22)) {
			mode = 3;
			PlaySound('E', 14, 5);
		}
		else if (CheckMenu_Button(msX, msY, sX, sY, 141, 100, 76, 22)) {
			mode = 4;
			PlaySound('E', 14, 5);
		}
		else if (CheckMenu_Button(msX, msY, sX, sY, 91, 290, 76, 22)) {
			mode = 0;
			PlaySound('E', 14, 5);
		}
		break;

	case 3:
		if (CheckMenu_Button(msX, msY, sX, sY, 41, 290, 76, 22)) {
			reqSelectRole(1);
			mode = 0;
			PlaySound('E', 14, 5);
		}
		else if (CheckMenu_Button(msX, msY, sX, sY, 141, 290, 76, 22)) {
			mode = 2;
			PlaySound('E', 14, 5);
		}
		break;

	case 4:
		if (CheckMenu_Button(msX, msY, sX, sY, 41, 290, 76, 22)) {
			reqSelectRole(2);
			mode = 0;
			PlaySound('E', 14, 5);
		}
		else if (CheckMenu_Button(msX, msY, sX, sY, 141, 290, 76, 22)) {
			mode = 2;
			PlaySound('E', 14, 5);
		}
		break;

	case 5:
		if (CheckMenu_Button(msX, msY, sX, sY, 41, 290, 76, 22)) {
			reqUpgradeRole(1);
			mode = 0;
			PlaySound('E', 14, 5);
		}
		else if (CheckMenu_Button(msX, msY, sX, sY, 141, 290, 76, 22)) {
			mode = 1;
			PlaySound('E', 14, 5);
		}
		break;

	case 6:
		if (CheckMenu_Button(msX, msY, sX, sY, 41, 290, 76, 22)) {
			reqUpgradeRole(2);
			mode = 0;
			PlaySound('E', 14, 5);
		}
		else if (CheckMenu_Button(msX, msY, sX, sY, 141, 290, 76, 22)) {
			mode = 1;
			PlaySound('E', 14, 5);
		}
		break;
	}
}

void CGame::reqUpgradeRole(short role)
{
	char data[10];
	auto* cp = data;

	Push(cp, (u32)REQ_UPGRADEPLAYERROLE);
	Push(cp, (u16)1);
	Push(cp, role);

	m_pGSock->iSendMsg(data, cp - data);
}


void CGame::reqSelectRole(short role)
{
	char data[10];
	auto* cp = data;

	Push(cp, (u32)REQ_SELECTPLAYERROLE);
	Push(cp, (u16)1);
	Push(cp, role);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::Render_AdminPanel(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 94;
	int maxlines = 13;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	int tmpindex = -1;

	DrawShadowDialog2(sX, sY, 94);

	switch (mode)
	{
	case 0:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Panel de administracion", 255, 200, 0);
		//PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Panel de administracion", 0, 0, 0);
		//PutCenterString_SpriteFont(sX, sX + sizeX, sY + 4, "Panel de administracion", 250, 250, 250);

		const int lineHeight = 15;
		const int startY = sY + 40;

		struct LineOption {
			const char* text;
			int offsetY;
		};

		LineOption options[] = {
			{ "Administrar eventos", 0 },
			{ "Gestionar jugadores", 15 }
		};

		for (const auto& opt : options) {
			int y = startY + opt.offsetY;
			bool hovered = (msX >= sX + 10 && msX <= sX + sizeX - 10 && msY >= y && msY < y + lineHeight); // NOTA: "<" en vez de "<="

			int r = hovered ? 255 : 255;
			int g = hovered ? 200 : 255;
			int b = hovered ? 0 : 255;

			PutAlignedString(sX + 10, sX + sizeX - 10, y, (char*)opt.text, r, g, b);
		}
		break;
	}

	case 2:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Administrar eventos", 255, 200, 0);
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		break;
	}

	case 3:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Elegi el jugador a gestionar", 255, 200, 0);

		const int iSearchBoxX = sX + 70;
		const int iSearchBoxY = sY + 28;
		const int iSearchBoxWidth = 150;
		const int iSearchBoxHeight = 18;
		const int maxlines = 11;

		PutString2(sX + 20, sY + 30, "Buscar", 255, 255, 255);
		DrawShadowMenu(iSearchBoxX, iSearchBoxY, iSearchBoxX + iSearchBoxWidth, iSearchBoxY + iSearchBoxHeight);
		DrawShadowMenu_DialogBordersNoTitle(iSearchBoxX, iSearchBoxY, iSearchBoxWidth, iSearchBoxHeight);
		PutString2(iSearchBoxX + 2, iSearchBoxY + 2, searchFilter, 255, 255, 255);

		std::vector<int> filtered_indices;
		if (strlen(searchFilter)) {
			for (int i = 0; i < vPlayers.size(); ++i)
				if (stristr(vPlayers[i].m_cCharName, searchFilter) != nullptr)
					filtered_indices.push_back(i);
		}
		else {
			for (int i = 0; i < vPlayers.size(); ++i)
				filtered_indices.push_back(i);
		}

		int iTotalLines = static_cast<int>(filtered_indices.size());
		int iPointerLoc;
		double d1, d2, d3;

		PutAlignedString(sX + 22, sX + 165, sY + 55, "Jugadores conectados", 0, 255, 0);

		if (iTotalLines > maxlines) {
			d1 = static_cast<double>(m_stDialogBoxInfo[dnumber].sView);
			d2 = static_cast<double>(iTotalLines - maxlines);
			d3 = (275.0f * d1) / d2;
			iPointerLoc = static_cast<int>(d3 + 0.5);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + sizeX - 13, sY + 44 + iPointerLoc, 7);
		}
		else {
			iPointerLoc = 0;
		}

		for (int i = 0; i < maxlines; i++) {
			int viewIndex = i + m_stDialogBoxInfo[dnumber].sView;
			if (viewIndex < iTotalLines) {
				int realIndex = filtered_indices[viewIndex];
				auto& player = vPlayers[realIndex];

				int y = sY + i * 18 + 75;
				bool hovered = (msX >= sX + 20 && msX <= sX + 220 && msY >= y && msY < y + 14); // NOTA: "<" en vez de "<="

				int r = hovered ? 255 : 255;
				int g = hovered ? 255 : 200;
				int b = hovered ? 255 : 0;

				PutAlignedString(sX + 10, sX + 190, y, player.m_cCharName, r, g, b);
				if (hovered) tmpindex = realIndex;
			}
		}

		if (iGetTopDialogBoxIndex() == dnumber && msZ != 0) {
			m_stDialogBoxInfo[dnumber].sView -= msZ / 120;
			if (m_stDialogBoxInfo[dnumber].sView < 0)
				m_stDialogBoxInfo[dnumber].sView = 0;

			int maxScroll = (iTotalLines - maxlines > 0) ? (iTotalLines - maxlines) : 0;
			if (m_stDialogBoxInfo[dnumber].sView > maxScroll)
				m_stDialogBoxInfo[dnumber].sView = maxScroll;
			m_DInput.m_sZ = 0;
		}

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY, "Refresh");
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		break;
	}

	case 4:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Gestionar jugador", 255, 200, 0);

		if (m_iSelectedPlayerIndex != -1) {
			wsprintf(G_cTxt, "%s", vPlayers[m_iSelectedPlayerIndex].m_cCharName);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 25, G_cTxt, 0, 255, 0);
		}

		const int lineHeight = 15;
		const int startY = sY + 40;

		struct LineOption {
			const char* text;
			int offsetY;
		};

		LineOption options[] = {
			{ "Elegir ciudad", 0 },
			{ "Agregar puntos", 15 },
			{ "Banear nombre", 30 },
			{ "Banear IP", 45 },
			{ "Banear MAC", 60 },
			{ "Banear por completo", 75 },
			{ "Desconectar jugador", 90 }
		};

		for (const auto& opt : options) {
			int y = startY + opt.offsetY;
			bool hovered = (msX >= sX + 10 && msX <= sX + sizeX - 10 && msY >= y && msY < y + lineHeight); // NOTA: "<"

			int r = hovered ? 255 : 255;
			int g = hovered ? 200 : 255;
			int b = hovered ? 0 : 255;

			PutAlignedString(sX + 10, sX + sizeX - 10, y, (char*)opt.text, r, g, b);
		}

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		break;
	}
	case 5:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Elegir ciudad de jugador", 255, 200, 0);

		if (m_iSelectedPlayerIndex != -1) {
			wsprintf(G_cTxt, "%s", vPlayers[m_iSelectedPlayerIndex].m_cCharName);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 25, G_cTxt, 0, 255, 0);

			char city[50];
			switch (vPlayers[m_iSelectedPlayerIndex].m_sSide)
			{
			case 0:
				strcpy(city, "Traveler");
				break;
			case 1: 
				strcpy(city, "Aresden");
				break;
			case 2:
				strcpy(city, "Elvine");
				break;
			default:
				break;
			}

			wsprintf(G_cTxt, "Ciudad actual: %s", city);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 40, G_cTxt, 255, 255, 255);


			wsprintf(G_cTxt, "Nueva ciudad para el jugador", city);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 70, G_cTxt, 255, 200, 0);


			DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, 280 - 165, "Aresden");
			DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, 280 - 165, "Elvine");
		}


		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		break;
	}
	case 6:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Agregar puntos al jugador", 255, 200, 0);

		if (m_iSelectedPlayerIndex != -1)
		{
			wsprintf(G_cTxt, "%s", vPlayers[m_iSelectedPlayerIndex].m_cCharName);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 25, G_cTxt, 0, 255, 0);

			const int lineHeight = 15;
			const int startY = sY + 40;

			struct LineOption {
				const char* text;
				int offsetY;
			};

			LineOption options[] = {
				{ "Enemy Kills", 0 },
				{ "Contribucion", 15 },
				{ "Reputacion", 30 },
				{ "Majestics", 45 },
				{ "Tokens", 60 },
				{ "Pa Fragments", 75 },
				{ "Ma Fragments", 90 },
				{ "Mr Fragments", 105 },
				{ "Dr Fragments", 120 },
				{ "HitProb Fragments", 135 }
			};

			for (const auto& opt : options) {
				int y = startY + opt.offsetY;
				bool hovered = (msX >= sX + 10 && msX <= sX + sizeX - 10 && msY >= y && msY < y + lineHeight); // NOTA: "<"

				int r = hovered ? 255 : 255;
				int g = hovered ? 200 : 255;
				int b = hovered ? 0 : 255;

				PutAlignedString(sX + 10, sX + sizeX - 10, y, (char*)opt.text, r, g, b);
			}
		}

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		break;
	}
	case 7:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Confirmar puntos", 255, 200, 0);

		if (m_iSelectedPlayerIndex != -1)
		{
			const char* types[] = {
				"Enemy Kills", "Contribucion", "Reputacion", "Majestics", "Tokens",
				"Pa Fragments", "Ma Fragments", "Mr Fragments", "Dr Fragments", "HitProb Fragments"
			};

			const char* typeName = (m_iSelectedPointType >= 0 && m_iSelectedPointType < 10)
				? types[m_iSelectedPointType]
				: "Desconocido";

			wsprintf(G_cTxt, "Jugador: %s", vPlayers[m_iSelectedPlayerIndex].m_cCharName);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 30, G_cTxt, 0, 255, 0);

			wsprintf(G_cTxt, "Tipo de puntos: %s", typeName);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 50, G_cTxt, 255, 255, 255);

			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 80, "Cantidad a agregar (1 ~ 500000):", 255, 200, 0);

			const int inputX = sX + 50;
			const int inputY = sY + 100;
			const int inputW = 100;
			const int inputH = 18;

			DrawShadowMenu(inputX, inputY, inputX + inputW, inputY + inputH);
			DrawShadowMenu_DialogBordersNoTitle(inputX, inputY, inputW, inputH);
			PutString2(inputX + 2, inputY + 2, m_cPointAmountInput, 255, 255, 255);

			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 130, "Presiona Confirmar para continuar", 255, 255, 0);
		}

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY, "Confirmar");
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		break;
	}

	case 8:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Confirmar baneo", 255, 200, 0);

		if (m_iSelectedPlayerIndex != -1)
		{
			const char* typeName = "Desconocido";

			switch (m_iBanType)
			{
			case 0: typeName = "Nombre"; break;
			case 1: typeName = "IP";     break;
			case 2: typeName = "MAC";    break;
			case 3: typeName = "Nombre, IP y MAC"; break;
			}

			wsprintf(G_cTxt, "Jugador: %s", vPlayers[m_iSelectedPlayerIndex].m_cCharName);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 30, G_cTxt, 0, 255, 0);

			wsprintf(G_cTxt, "Tipo de ban: %s", typeName);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 50, G_cTxt, 255, 255, 255);

			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 80, "¿Estás seguro que deseas continuar?", 255, 255, 0);
		}

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY, "Confirmar");
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Cancelar");
		break;
	}
	case 9:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Desconectar jugador", 255, 200, 0);

		if (m_iSelectedPlayerIndex != -1)
		{
			wsprintf(G_cTxt, "Jugador: %s", vPlayers[m_iSelectedPlayerIndex].m_cCharName);
			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 30, G_cTxt, 0, 255, 0);

			PutAlignedString(sX + 10, sX + sizeX - 10, sY + 60, "¿Deseas desconectarlo del servidor?", 255, 255, 0);
		}

		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY, "Confirmar");
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Cancelar");
		break;
	}
	case 10:
	{
		PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Administrar eventos", 255, 200, 0);

		const int lineHeight = 14;
		const int startY = sY + 30;
		const int maxLines = 18;
		const int totalEvents = (int)EventID::Max;

		int iPointerLoc = 0;

		// Scrollbar si hay más eventos que líneas visibles
		if (totalEvents > maxLines) {
			double d1 = (double)m_stDialogBoxInfo[dnumber].sView;
			double d2 = (double)(totalEvents - maxLines);
			double d3 = (275.0 * d1) / d2;
			iPointerLoc = (int)(d3 + 0.5);
			DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + sizeX - 13, sY + 44 + iPointerLoc, 7);
		}

		// Scroll con la rueda del mouse
		if (iGetTopDialogBoxIndex() == dnumber && msZ != 0) {
			m_stDialogBoxInfo[dnumber].sView -= msZ / 120;
			if (m_stDialogBoxInfo[dnumber].sView < 0)
				m_stDialogBoxInfo[dnumber].sView = 0;

			int maxScroll = totalEvents - maxLines;
			if (m_stDialogBoxInfo[dnumber].sView > maxScroll)
				m_stDialogBoxInfo[dnumber].sView = maxScroll;
			m_DInput.m_sZ = 0;
		}

		// Dibujar cada línea visible
		for (int i = 0; i < maxLines; ++i) {
			int viewIndex = i + m_stDialogBoxInfo[dnumber].sView;
			if (viewIndex >= totalEvents) break;

			EventID id = (EventID)viewIndex;
			int y = startY + i * lineHeight;

			bool hovered = (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
				msY >= y && msY < y + lineHeight);

			bool isActive = g_ev.Is(id);

			int r, g, b;
			if (hovered) {
				r = 255; g = 180; b = 0;
			}
			else if (isActive) {
				r = 0; g = 255; b = 0;
			}
			else {
				r = 255; g = 0; b = 0;
			}

			PutAlignedString(sX + 10, sX + sizeX - 20, y, getEventName(id), r, g, b);
		}

		// Botón Back
		DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY, "Back");
		break;
	}


	}
}

void CGame::HandleClick_AdminPanel(short msX, short msY)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 94;
	const int lineHeight = 15;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto& mode = m_stDialogBoxInfo[dnumber].cMode;


	switch (mode)
	{
	case 0:
	{
		// Click en "Administrar eventos"
		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			msY >= sY + 40 && msY < sY + 40 + 15) {

			PlaySound('E', 14, 5);
			mode = 10;
			return;
		}

		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			msY >= sY + 55 && msY < sY + 55 + lineHeight) { // NOTA: "<"

			SendChat("/reqtotalplayers");
			PlaySound('E', 14, 5);
			ZeroMemory(searchFilter, sizeof(searchFilter));
			mode = 3;
		}
		break;
	}

	case 3:
	{
		const int iSearchBoxX = sX + 70;
		const int iSearchBoxY = sY + 28;
		const int iSearchBoxWidth = 150;
		const int maxlines = 11;

		// Click en cuadro de búsqueda
		if ((msX >= iSearchBoxX) && (msX <= iSearchBoxX + iSearchBoxWidth) &&
			(msY >= iSearchBoxY) && (msY <= iSearchBoxY + 18))
		{
			isSearchActive = true;
			StartInputString(iSearchBoxX + 2, iSearchBoxY + 2, sizeof(searchFilter) - 1, searchFilter, FALSE);
			return;
		}

		std::vector<int> filtered_indices;
		if (strlen(searchFilter)) {
			for (int i = 0; i < vPlayers.size(); ++i)
				if (stristr(vPlayers[i].m_cCharName, searchFilter) != nullptr)
					filtered_indices.push_back(i);
		}
		else {
			for (int i = 0; i < vPlayers.size(); ++i)
				filtered_indices.push_back(i);
		}
		
		int iTotalLines = static_cast<int>(filtered_indices.size());
		int viewOffset = m_stDialogBoxInfo[dnumber].sView;

		for (int i = 0; i < maxlines; i++) {
			int viewIndex = i + viewOffset;
			if (viewIndex >= iTotalLines) break;

			int realIndex = filtered_indices[viewIndex];
			int y = sY + i * 18 + 75;

			if (msX >= sX + 20 && msX <= sX + 220 &&
				msY >= y && msY < y + 14) { // NOTA: "<"

				EndInputString();
				PlaySound('E', 14, 5);
				m_iSelectedPlayerIndex = realIndex;
				mode = 4;
				break;
			}
		}

		auto refresh = (msX >= sX + 25) && (msX <= sX + 120) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (refresh)
		{
			SendChat("/reqtotalplayers");
			PlaySound('E', 14, 5);
			return;
		}

		auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back)
		{
			EndInputString();
			ZeroMemory(searchFilter, sizeof(searchFilter));
			mode = 0;
			PlaySound('E', 14, 5);
			return;
		}
		break;
	}

	case 4: 
	{
		// Opción: Elegir ciudad
		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			msY >= sY + 40 && msY < sY + 40 + lineHeight) {

			PlaySound('E', 14, 5);
			mode = 5;
			return;
		}		

		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			msY >= sY + 55 && msY < sY + 55 + lineHeight) {

			m_iSelectedPointType = -1;
			PlaySound('E', 14, 5);
			mode = 6;
			return;
		}

		// Banear nombre
		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			msY >= sY + 70 && msY < sY + 70 + lineHeight) {
			m_iBanType = 0;
			PlaySound('E', 14, 5);
			mode = 8;
			return;
		}

		// Banear IP
		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			msY >= sY + 85 && msY < sY + 85 + lineHeight) {
			m_iBanType = 1;
			PlaySound('E', 14, 5);
			mode = 8;
			return;
		}

		// Banear MAC
		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			msY >= sY + 100 && msY < sY + 100 + lineHeight) {
			m_iBanType = 2;
			PlaySound('E', 14, 5);
			mode = 8;
			return;
		}

		// Banear todo
		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			msY >= sY + 115 && msY < sY + 115 + lineHeight) {
			m_iBanType = 3;
			PlaySound('E', 14, 5);
			mode = 8;
			return;
		}

		// Desconectar jugador
		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			msY >= sY + 130 && msY < sY + 130 + lineHeight) {
			PlaySound('E', 14, 5);
			mode = 9;
			return;
		}

		auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back)
		{
			SendChat("/reqtotalplayers");
			ZeroMemory(searchFilter, sizeof(searchFilter));
			mode = 3;
			PlaySound('E', 14, 5);
			return;
		}

		break;
	}

	case 5:
	{
		// Botón "Back"
		auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back)
		{
			mode = 4;
			PlaySound('E', 14, 5);
			return;
		}

		if (m_iSelectedPlayerIndex != -1)
		{
			// Botón "Aresden"
			short btnAresX = sX + DEF_LBTNPOSX;
			short btnY = sY + 280 - 165;
			if (msX >= btnAresX && msX <= btnAresX + 77 &&
				msY >= btnY && msY <= btnY + 20)
			{
				if (vPlayers[m_iSelectedPlayerIndex].m_sSide == 1)
				{
					AddEventList("Error. Este jugador ya es Aresden");
					return;
				}
				AdminReqChangeCity(m_iSelectedPlayerIndex, 1);
				//vPlayers[m_iSelectedPlayerIndex].m_sSide = 1; // Aresden
				PlaySound('E', 14, 5);
				mode = 4;
				return;
			}

			// Botón "Elvine"
			short btnElvineX = sX + DEF_RBTNPOSX;
			if (msX >= btnElvineX && msX <= btnElvineX + 77 &&
				msY >= btnY && msY <= btnY + 20)
			{
				if (vPlayers[m_iSelectedPlayerIndex].m_sSide == 2)
				{
					AddEventList("Error. Este jugador ya es Elvine");
					return;
				}
				AdminReqChangeCity(m_iSelectedPlayerIndex, 2);
				PlaySound('E', 14, 5);
				mode = 4;
				return;
			}
		}
		break;
	}

	case 6:
	{
		// Botón Back
		auto back = (msX >= sX + 153) && (msX <= sX + 230) &&
			(msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back)
		{
			mode = 4;
			PlaySound('E', 14, 5);
			return;
		}

		// Opción seleccionada
		const int lineHeight = 15;
		const int startY = sY + 40;

		for (int i = 0; i < 10; ++i)
		{
			int y = startY + i * lineHeight;
			if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
				msY >= y && msY < y + lineHeight)
			{
				m_iSelectedPointType = i; // Guardar tipo de punto
				ZeroMemory(m_cPointAmountInput, sizeof(m_cPointAmountInput));
				mode = 7;
				PlaySound('E', 14, 5);
				return;
			}
		}
		break;
	}
	case 7:
	{
		// Botón "Back"
		auto back = (msX >= sX + 153) && (msX <= sX + 230) &&
			(msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back)
		{
			m_iSelectedPointType = -1;
			m_bPointAmountInputActive = false;
			ZeroMemory(m_cPointAmountInput, sizeof(m_cPointAmountInput));
			mode = 6;
			PlaySound('E', 14, 5);
			EndInputString();
			return;
		}

		// Botón "Confirmar"
		auto confirm = (msX >= sX + 43) && (msX <= sX + 120) &&
			(msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (confirm)
		{
			int value = atoi(m_cPointAmountInput);
			if (value <= 0 || value > 500000)
			{
				AddEventList("Error: la cantidad debe estar entre 1 y 500000.");
				EndInputString();
				return;
			}

			if (m_iSelectedPlayerIndex == -1 || m_iSelectedPointType < 0 || m_iSelectedPointType >= 10)
			{
				AddEventList("Error: selección inválida.");
				EndInputString();
				return;
			}

			const char* type = nullptr;
			const char* typeName = nullptr;
			switch (m_iSelectedPointType)
			{
			case 0: type = "ek";      typeName = "Enemy Kills"; break;
			case 1: type = "contrib"; typeName = "Contribución"; break;
			case 2: type = "rep";     typeName = "Reputación"; break;
			case 3: type = "maj";     typeName = "Majestics"; break;
			case 4: type = "tok";     typeName = "Tokens"; break;
			case 5: type = "fragpa";  typeName = "Pa Fragments"; break;
			case 6: type = "fragma";  typeName = "Ma Fragments"; break;
			case 7: type = "fragmr";  typeName = "Mr Fragments"; break;
			case 8: type = "fragdr";  typeName = "Dr Fragments"; break;
			case 9: type = "fraghp";  typeName = "HitProb Fragments"; break;
			default: type = "unk";    typeName = "Desconocido"; break;
			}

			char command[64];
			sprintf(command, "/givepoints %s %s %d", vPlayers[m_iSelectedPlayerIndex].m_cCharName, type, value);
			SendChat(command);

			char logMsg[128];
			sprintf(logMsg, "Se enviaron %d puntos de %s a %s.", value, typeName, vPlayers[m_iSelectedPlayerIndex].m_cCharName);
			AddEventList(logMsg);

			// Limpiar estado
			m_bPointAmountInputActive = false;
			ZeroMemory(m_cPointAmountInput, sizeof(m_cPointAmountInput));
			mode = 4;
			PlaySound('E', 14, 5);
			EndInputString();
			return;
		}

		// Activar input al hacer clic en el cuadro de texto
		const int inputX = sX + 50;
		const int inputY = sY + 100;
		const int inputW = 100;
		const int inputH = 18;

		if (msX >= inputX && msX <= inputX + inputW &&
			msY >= inputY && msY <= inputY + inputH)
		{
			m_bPointAmountInputActive = true;
			StartInputString(inputX + 2, inputY + 2, 8, m_cPointAmountInput, TRUE); // solo números
			return;
		}
		break;
	}
	case 8:
	{
		// Confirmar
		if (msX >= sX + DEF_LBTNPOSX && msX <= sX + DEF_LBTNPOSX + 80 &&
			msY >= sY + DEF_BTNPOSY && msY <= sY + DEF_BTNPOSY + 15)
		{
			if (m_iSelectedPlayerIndex == -1)
			{
				AddEventList("Error: no se seleccionó ningún jugador.");
				PlaySound('E', 14, 5);
				return;
			}

			switch (m_iBanType)
			{
			case 0:
				AdminReqBanUserName(m_iSelectedPlayerIndex);
				break;
			case 1:
				AdminReqBanIP(m_iSelectedPlayerIndex);
				break;
			case 2:
				AdminReqBanMAC(m_iSelectedPlayerIndex);
				break;
			case 3:
				AdminReqBanAll(m_iSelectedPlayerIndex);
				break;
			default:
				AddEventList("Error: tipo de ban desconocido.");
				PlaySound('E', 14, 5);
				return;
			}

			char msg[128];
			sprintf(msg, "Se envió solicitud de ban (%d) a %s.", m_iBanType, vPlayers[m_iSelectedPlayerIndex].m_cCharName);
			AddEventList(msg);

			PlaySound('E', 14, 5);
			mode = 4;
			return;
		}

		// Cancelar
		if (msX >= sX + DEF_RBTNPOSX && msX <= sX + DEF_RBTNPOSX + 80 &&
			msY >= sY + DEF_BTNPOSY && msY <= sY + DEF_BTNPOSY + 15)
		{
			m_iBanType = -1;
			PlaySound('E', 14, 5);
			mode = 4;
			return;
		}
		break;
	}

	case 9:
	{
		// Confirmar
		if (msX >= sX + DEF_LBTNPOSX && msX <= sX + DEF_LBTNPOSX + 80 &&
			msY >= sY + DEF_BTNPOSY && msY <= sY + DEF_BTNPOSY + 15)
		{
			if (m_iSelectedPlayerIndex == -1)
			{
				AddEventList("Error: no se seleccionó ningún jugador.");
				PlaySound('E', 14, 5);
				return;
			}

			AdminReqDisconnectPlayer(m_iSelectedPlayerIndex);

			char msg[128];
			sprintf(msg, "Solicitud de desconexión enviada a %s.", vPlayers[m_iSelectedPlayerIndex].m_cCharName);
			AddEventList(msg);

			PlaySound('E', 14, 5);
			mode = 4;
			return;
		}

		// Cancelar
		if (msX >= sX + DEF_RBTNPOSX && msX <= sX + DEF_RBTNPOSX + 80 &&
			msY >= sY + DEF_BTNPOSY && msY <= sY + DEF_BTNPOSY + 15)
		{
			PlaySound('E', 14, 5);
			mode = 4;
			return;
		}
		break;
	}
	case 10:
	{
		const int lineHeight = 14;
		const int maxLines = 18;
		const int startY = sY + 30;

		int totalEvents = (int)EventID::Max;

		int mouseLine = (msY - startY) / lineHeight;
		int viewLine = mouseLine + m_stDialogBoxInfo[dnumber].sView;

		if (msX >= sX + 10 && msX <= sX + sizeX - 10 &&
			mouseLine >= 0 && mouseLine < maxLines &&
			viewLine < totalEvents)
		{
			EventID selectedEvent = (EventID)viewLine;
			AdminReqChangeEventStatus(getEventNameShort(selectedEvent));
			PlaySound('E', 14, 5);
			return;
		}

		

		// Botón Back
		auto back = (msX >= sX + 153) && (msX <= sX + 230) && (msY >= sY + 293) && (msY <= sY + 145 + 313);
		if (back) {
			mode = 0;
			PlaySound('E', 14, 5);
			return;
		}

		break;
	}

	case 2:
	default:
		break;
	}
}

void CGame::AdminReqChangeCity(int ObjectIndex, short city)
{
	char data[20];
	auto* cp = data;

	Push(cp, (u32)ADMINREQ_CHANGECITY);
	Push(cp, (u16)1);
	Push(cp, vPlayers[ObjectIndex].m_cCharName, 12);
	Push(cp, city);

	m_pGSock->iSendMsg(data, cp - data);
}


bool CGame::GetMacAddress(char* outMacStr)
{
	if (!outMacStr) return false;

	ULONG bufferSize = 15000;
	IP_ADAPTER_ADDRESSES* pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(bufferSize);

	if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST |
		GAA_FLAG_SKIP_DNS_SERVER, NULL, pAddresses, &bufferSize) != ERROR_SUCCESS) {
		free(pAddresses);
		return false;
	}

	IP_ADAPTER_ADDRESSES* pCurr = pAddresses;
	while (pCurr) {
		if (pCurr->PhysicalAddressLength == 6) {
			sprintf(outMacStr, "%02X:%02X:%02X:%02X:%02X:%02X",
				pCurr->PhysicalAddress[0],
				pCurr->PhysicalAddress[1],
				pCurr->PhysicalAddress[2],
				pCurr->PhysicalAddress[3],
				pCurr->PhysicalAddress[4],
				pCurr->PhysicalAddress[5]);

			free(pAddresses);
			return true;
		}
		pCurr = pCurr->Next;
	}

	free(pAddresses);
	return false;
}
void CGame::SendMacToServer(char* mac)
{
	char data[128]; // ampliamos por seguridad
	auto* cp = data;

	char mac_address[18] = { 0 };
	strcpy(mac_address, mac);

	Push(cp, (u32)SEND_MAC_TOSERVER); 
	Push(cp, (u16)1);          // versión o cantidad de campos (antes era 1)

	// MAC (18 bytes)
	Push(cp, mac_address, 18);

	// IP pública (string de longitud variable, terminada en 0)
	Push(cp, ip_address, 32);

	m_pGSock->iSendMsg(data, cp - data);

#ifdef _DEBUG
	printf("[SEND] MAC+IP → MAC:%s | IP:%s\n", mac_address, ip_address);
#endif
}


void CGame::AdminReqBanUserName(int ObjectIndex)
{
	char data[20];
	auto* cp = data;

	Push(cp, (u32)ADMINREQ_BANUSERNAME);
	Push(cp, (u16)1);
	Push(cp, vPlayers[ObjectIndex].m_cCharName, 12);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::AdminReqBanMAC(int ObjectIndex)
{
	char data[20];
	auto* cp = data;

	Push(cp, (u32)ADMINREQ_BANMAC);
	Push(cp, (u16)1);
	Push(cp, vPlayers[ObjectIndex].m_cCharName, 12);

	m_pGSock->iSendMsg(data, cp - data);
}



void CGame::AdminReqBanIP(int ObjectIndex)
{
	char data[20];
	auto* cp = data;

	Push(cp, (u32)ADMINREQ_BANIP);
	Push(cp, (u16)1);
	Push(cp, vPlayers[ObjectIndex].m_cCharName, 12);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::AdminReqBanAll(int ObjectIndex)
{
	char data[20];
	auto* cp = data;

	Push(cp, (u32)ADMINREQ_BANALL);
	Push(cp, (u16)1);
	Push(cp, vPlayers[ObjectIndex].m_cCharName, 12);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::AdminReqDisconnectPlayer(int ObjectIndex)
{
	char data[20];
	auto* cp = data;

	Push(cp, (u32)ADMINREQ_DCPLAYER);
	Push(cp, (u16)1);
	Push(cp, vPlayers[ObjectIndex].m_cCharName, 12);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::AdminReqChangeEventStatus(char * eventname)
{
	char data[100];
	auto* cp = data;

	char m_cEventName[50];
	strcpy(m_cEventName, eventname);

	Push(cp, (u32)ADMINREQ_CHANGEEVENTSTATUS);
	Push(cp, (u16)1);
	Push(cp, m_cEventName, 50);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::click_advance_smenu(short msX, short msY)
{
	short sX, sY, sizeX;
	sX = m_stDialogBoxInfo[95].sX;
	sY = m_stDialogBoxInfo[95].sY;
	sizeX = m_stDialogBoxInfo[95].sSizeX;

	HandleTabsClick(sX, sY, msX, msY);

	auto mode = m_stDialogBoxInfo[95].cMode;
	switch (mode)
	{
	case 0: // Main tab
	{
		
		// Click en <<
		if ((msX >= sX + 130 - 30) && (msX <= sX + 150 - 30) && (msY >= sY + 195) && (msY <= sY + 210))
		{
			client_lang = (client_lang == 0) ? 1 : 0;

			UpdateSettings();
			PlaySound('E', 14, 5);
			return; // Evitar procesar otros clics si ya cambió idioma
		}

		// Click en >>
		if ((msX >= sX + 215 - 30) && (msX <= sX + 235 - 30) && (msY >= sY + 195) && (msY <= sY + 210))
		{
			client_lang = (client_lang == 0) ? 1 : 0;

			UpdateSettings();
			PlaySound('E', 14, 5);
			return; // Evitar procesar otros clics si ya cambió idioma
		}



		if ((msX >= sX + 30) && (msX <= sX + 30 + 76) && (msY >= sY + 95) && (msY <= sY + 95 + 22))
		{
			if (m_bIsDialogEnabled[9]) DisableDialogBox(9);
			else EnableDialogBox(9, 0, 0, 0, NULL);

			UpdateSettings();
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 30) && (msX <= sX + 30 + 76) && (msY >= sY + 95 + 25) && (msY <= sY + 95 + 25 + 22))
		{
			m_bZoomMap = !m_bZoomMap;
			UpdateSettings();
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 30) && (msX <= sX + 30 + 76) && (msY >= sY + 95 + 25 + 25) && (msY <= sY + 95 + 25 + 25 + 22))
		{
			m_bShowTiming = !m_bShowTiming;
			UpdateSettings();
			PlaySound('E', 14, 5);
		}

		if ((msX >= sX + 30) && (msX <= sX + 30 + 76) && (msY >= sY + 95 + 25 + 25 + 25) && (msY <= sY + 95 + 25 + 25 + 25 + 22))
		{
			m_bShout = !m_bShout;
			UpdateSettings();
			PlaySound('E', 14, 5);
		}

		auto logout = (msX >= sX + 30) && (msX <= sX + 30 + 76) && (msY >= sY + 220) && (msY <= sY + 220 + 22);
		auto restart = (msX >= sX + 153) && (msX <= sX + 153 + 76) && (msY >= sY + 220) && (msY <= sY + 220 + 22);

		if (logout)
		{
			if (m_bForceDisconn) return;
			if (m_cLogOutCount == -1)
			{
				m_cLogOutCount = 6;
			}
			else
			{
				m_cLogOutCount = -1;
				AddEventList(DLGBOX_CLICK_SYSMENU2, 10);
				DisableDialogBox(95);
			}
			PlaySound('E', 14, 5);
		}

		if (restart)
		{
			if ((m_iHP <= 0) && (m_cRestartCount == -1))
			{
				m_cRestartCount = 5;
				m_dwRestartCountTime = timeGetTime();
				DisableDialogBox(95);
				wsprintf(G_cTxt, DLGBOX_CLICK_SYSMENU1, m_cRestartCount);
				AddEventList(G_cTxt, 10);
				PlaySound('E', 14, 5);
			}
		}
		break;
	}

	case 1: // Game tab
	{
		// Definición de áreas de click - Columna izquierda
		auto whisper = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70) && (msY <= sY + 70 + 15);
		auto showgrid = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20) && (msY <= sY + 70 + 20 + 15);
		auto gridtrans = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 15);
		auto dialogtrans = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 15);
		auto windowskey = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 15);
		auto showglares = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 15);
		auto showstars = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 15);
		auto showshadows = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20 + 15);

		// Definición de áreas de click - Columna derecha
		auto showcolors = (msX >= sX + 142) && (msX <= sX + 142 + 100) && (msY >= sY + 70) && (msY <= sY + 70 + 15);
		auto showtrees = (msX >= sX + 142) && (msX <= sX + 142 + 100) && (msY >= sY + 70 + 20) && (msY <= sY + 70 + 20 + 15);
		auto showsteeps = (msX >= sX + 142) && (msX <= sX + 142 + 100) && (msY >= sY + 70 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 15);
		auto itemsbox = (msX >= sX + 142) && (msX <= sX + 142 + 100) && (msY >= sY + 70 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 15);
		auto dialogtrans2 = (msX >= sX + 142) && (msX <= sX + 142 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 15);
		auto questhelper = (msX >= sX + 142) && (msX <= sX + 142 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 15);
		auto redsteeps = (msX >= sX + 142) && (msX <= sX + 142 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 15);
		auto candysize = (msX >= sX + 142) && (msX <= sX + 142 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20 + 15);

		bool settingsChanged = false;

		// Procesar clicks - Columna izquierda
		if (whisper) { m_bWhisper = !m_bWhisper; settingsChanged = true; }
		if (showgrid) { m_bGrid = !m_bGrid; settingsChanged = true; }
		if (gridtrans) { m_bGridTrans = !m_bGridTrans; settingsChanged = true; }
		if (dialogtrans) {
			CandySize = !CandySize;
			settingsChanged = true;
		}
		if (windowskey) { m_windowsKey = !m_windowsKey; settingsChanged = true; }
		if (showglares) { Glares = !Glares; settingsChanged = true; }
		if (showstars) { Stars = !Stars; settingsChanged = true; }
		if (showshadows) { Shadows = !Shadows; settingsChanged = true; }

		// Procesar clicks - Columna derecha
		if (showcolors) { Colors = !Colors; settingsChanged = true; }
		if (showtrees) { 
		if (Trees) Trees = FALSE;
		else Trees = TRUE;
		m_cDetailLevel = 3;
		settingsChanged = true; 
		}
		if (showsteeps) { Steeps = !Steeps; settingsChanged = true; }
		if (itemsbox) { ShadowsBox = !ShadowsBox; settingsChanged = true; }
		if (dialogtrans2) { m_bDialogTrans = !m_bDialogTrans; settingsChanged = true; }
		if (questhelper) { m_bQuestHelper = !m_bQuestHelper; settingsChanged = true; }
		if (redsteeps) {
			if (RedSteeps)
			{
				RedSteeps = false;
				MakeEffectSpr("effect2", 10, 3, FALSE);
			}
			else
			{
				RedSteeps = true;
				MakeEffectSpr("effect2b", 10, 3, FALSE);
			}
			settingsChanged = true;
		}
		if (candysize) { 
			if (DEF_SPRID_ITEMGROUND_PIVOTPOINT == ItemGround) ItemGround = DEF_SPRID_ITEMPACK_PIVOTPOINT;
			else ItemGround = DEF_SPRID_ITEMGROUND_PIVOTPOINT;

			settingsChanged = true; }

		if (settingsChanged) {
			UpdateSettings();
			PlaySound('E', 14, 5);
		}
		break;
	}

	case 2: // Audio tab (sin cambios)
	{
		auto showalldmg = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70) && (msY <= sY + 70 + 15);
		auto grid = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20) && (msY <= sY + 70 + 20 + 15);
		auto floorit = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 15);
		auto sprlow = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 15);
		auto autoekss = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 15);
		auto listdamage = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 15);
		auto autopickgold = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 15);
		auto blockexp = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20 + 15);
		auto fastdrop = (msX >= sX + 30) && (msX <= sX + 30 + 100) && (msY >= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20) && (msY <= sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20 + 20 + 15);
	
		bool settingsChanged = false;

		if (showalldmg) { 
		
		if (LowSprites)
		{
			LowSprites = false;
			MakeEffectSpr("effect7", 45, 12, FALSE);
			MakeEffectSpr("effect11", 89, 14, FALSE);
			MakeEffectSpr("Magic-Effect", 168, 5, FALSE, FALSE);
		}
		else
		{
			LowSprites = true;
			MakeEffectSpr("effect7b", 45, 12, FALSE);
			MakeEffectSpr("effect11b", 89, 14, FALSE);
			MakeEffectSpr("Magic-Effectb", 168, 5, FALSE, FALSE);
		}

		settingsChanged = true;
		}
		if (grid) { StaggerDmg = !StaggerDmg; settingsChanged = true; }
		if (floorit) {
			if (Roofs)
			{
				Roofs = false;
				MakeTileSpr("Tile422-429", 422, 8, TRUE);
				MakeTileSpr("Tile382-387", 382, 6, TRUE);
				MakeTileSpr("Tile370-381", 370, 12, TRUE);
				MakeTileSpr("objects7", 242, 7, TRUE);
			}
			else
			{
				Roofs = true;
				MakeTileSpr("Tile422-429b", 422, 8, TRUE);
				MakeTileSpr("Tile382-387b", 382, 6, TRUE);
				MakeTileSpr("Tile370-381b", 370, 12, TRUE);
				MakeTileSpr("objects7b", 242, 7, TRUE);
			}

			settingsChanged = true;
		}

		

		if (sprlow) { bPlayerShines = !bPlayerShines; settingsChanged = true; }
		if (autoekss) { BarraNpc = !BarraNpc; settingsChanged = true; }
		if (listdamage) { ShowShin = !ShowShin; settingsChanged = true; }
		if (autopickgold) { m_DInput.m_bModernMouse = !m_DInput.m_bModernMouse; settingsChanged = true; }
		if (fastdrop) { m_bOldCamera = !m_bOldCamera; settingsChanged = true; }

		if (settingsChanged) {
			UpdateSettings();
			PlaySound('E', 14, 5);
		}
		break;
	}

	case 3: // Mouse tab (sin cambios)
	{
		const int offsetX = -10;
		const int offsetY = -40;

		// Sound toggle (ON/OFF)
		if ((msX >= sX + 221 + offsetX) && (msX <= sX + 255 + offsetX) &&
			(msY >= sY + 122 + offsetY) && (msY <= sY + 137 + offsetY))
		{
			if (m_bSoundFlag)
			{
				if (m_bSoundStat == TRUE)
				{
					m_pESound[38]->bStop();
					m_bSoundStat = FALSE;
					AddEventList(NOTIFY_MSG_SOUND_OFF, 10);
				}
				else
				{
					m_bSoundStat = TRUE;
					AddEventList(NOTIFY_MSG_SOUND_ON, 10);
				}
			}
		}

		// Music toggle (ON/OFF)
		if ((msX >= sX + 221 + offsetX) && (msX <= sX + 255 + offsetX) &&
			(msY >= sY + 142 + offsetY) && (msY <= sY + 157 + offsetY))
		{
			if (m_bSoundFlag)
			{
				if (m_bMusicStat == TRUE) 	// Music Off
				{
					m_bMusicStat = FALSE;
					AddEventList(NOTIFY_MSG_MUSIC_OFF, 10);

					if (m_pBGM != NULL)
					{
						m_pBGM->bStop();
						delete m_pBGM;
						m_pBGM = NULL;
					}
				}
				else // Music On
				{
					m_bMusicStat = TRUE;
					AddEventList(NOTIFY_MSG_MUSIC_ON, 10);
					StartBGM(false);
				}
			}
		}
		break;
	}

	}

	const char* controlText = "Control panel menu (Ctrl + P)";

	// Definir el área de detección del texto
	SIZE size;
	HDC hDC = m_DDraw.m_hDC;
	HFONT hOldFont = (HFONT)SelectObject(hDC, m_DDraw.m_hFontInUse4);
	GetTextExtentPoint32(hDC, controlText, strlen(controlText), &size);
	SelectObject(hDC, hOldFont);

	// Coordenadas del texto
	int textX = sX + (sizeX / 2) - (size.cx / 2); // Centrado horizontal
	int textY = sY + 248;
	int textW = size.cx;
	int textH = size.cy;

	// Mouseover check
	bool controlPanel = (msX >= textX) && (msX <= textX + textW) &&
		(msY >= textY) && (msY <= textY + textH);

	// Color dinámico
	if (controlPanel)
	{
		if (m_bIsDialogEnabled[96] == FALSE)
		{
			EnableDialogBox(96, NULL, NULL, NULL);
		}
		else DisableDialogBox(96);


		PlaySound('E', 14, 5);

		return;
	}
}

void CGame::draw_advance_smenu(short msX, short msY, char cLB)
{
	short sX, sY, sizeX;
	int dnumber = 95;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	auto mode = m_stDialogBoxInfo[95].cMode;

	// Text definitions for both languages
	struct LanguageTexts {
		const char* mainTitle;
		const char* guideMap;
		const char* mapZoom;
		const char* showFrames;
		const char* shout;
		const char* language;
		const char* logOut;
		const char* cancel;
		const char* restart;
		const char* whisper;
		const char* showGrid;
		const char* gridTrans;
		const char* candyBig;
		const char* windowsKey;
		const char* showGlares;
		const char* showStars;
		const char* showShadows;
		const char* showColors;
		const char* hideTrees;
		const char* showSteeps;
		const char* itemsBoxs;
		const char* dialogTrans;
		const char* questHelper;
		const char* redSteps;
		const char* bigItems;
		const char* spritesLowFps;
		const char* threeDamage;
		const char* roofs;
		const char* shines;
		const char* npcHealthBar;
		const char* showShinning;
		const char* modernMouse;
		const char* newCamera;
		const char* controlPanel;
		const char* sound;
		const char* music;
		const char* on;
		const char* off;
		const char* disabled;
	};

	static const LanguageTexts english = {
		"Helbreath Cursed",
		"Guide Map",
		"Map zoom",
		"Show Frames",
		"Shout",
		"Language:",
		"Log out",
		"Cancel",
		"Restart",
		"Whisper",
		"Show grid",
		"Grid trans",
		"Candy big",
		"Windows Key",
		"Show Glares",
		"Show Stars",
		"Show shadows",
		"Show colors",
		"Hide Trees",
		"Show Steeps",
		"Items Boxs",
		"Dialog Trans",
		"Quest Helper",
		"Red steps",
		"Big Items",
		"Sprites Low Fps",
		"Three Damage",
		"Roofs off",
		"Shines",
		"Npc Healt Bar",
		"Show shinning",
		"Modern Mouse",
		"New Camera",
		"Control panel menu (Ctrl + P)",
		"Sound",
		"Music",
		"ON",
		"OFF",
		"DISABLED"
	};

	static const LanguageTexts spanish = {
		"Helbreath Cursed",
		"Guia mapa",
		"Zoom mapa",
		"Marcos",
		"Gritar",
		"Idioma:",
		"Salir",
		"Cancelar",
		"Reiniciar",
		"Susurro",
		"Grid",
		"Grid transp.",
		"Candys XL",
		"Tecla Win",
		"Destellos",
		"Estrellas",
		"Sombras",
		"Colores",
		"Ocultar arboles",
		"Mostrar bordes",
		"Cajas item",
		"Dialogo transp.",
		"Ayuda quest",
		"Pasos rojos",
		"Items grandes",
		"Sprites bajos Fps",
		"Triple dano",
		"Techos Off",
		"Brillos",
		"Vida NPC",
		"Brillos ON",
		"Raton nuevo",
		"Camara nueva",
		"Panel control (Ctrl + P)",
		"Soni.",
		"Music.",
		"ON",
		"OFF",
		"DESACT."
	};


	const LanguageTexts* texts = (client_lang == 0) ? &english : &spanish;

	switch (mode)
	{
	case 0: // Main tab
	{
		DrawMenu_Tab(sX, sY, true, false, false, false);
		DrawMenu_Tabs_AutoSpacing(sX, sY, msX, msY, 0);

		if (strcmp(m_cWorldServerName, NAME_WORLDNAME1) == 0) {
			PutCambriaString(sX + 30, sY + 70, (char*)texts->mainTitle, 255, 140, 0);
		}

		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 95, (char*)texts->guideMap, m_bIsDialogEnabled[9]);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 95 + 25, (char*)texts->mapZoom, m_bZoomMap);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 95 + 25 + 25, (char*)texts->showFrames, m_bShowTiming);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 95 + 25 + 25 + 25, (char*)texts->shout, m_bShout);


		const char* langStr = (client_lang == 0) ? "English" : "Español";


		PutCambriaString(sX + 30, sY + 195, (char*)texts->language, 255, 255, 255);

		// Hover para <<
		int colorL_R = 255, colorL_G = 255, colorL_B = 255;
		if ((msX >= sX + 130 - 30) && (msX <= sX + 150 - 30) && (msY >= sY + 195) && (msY <= sY + 195 + 15))
		{
			colorL_G = 140; // Hover color
			colorL_B = 0;
		}
		PutCambriaString(sX + 130 - 30, sY + 195, "<<", colorL_R, colorL_G, colorL_B);

		// Idioma actual
		PutCambriaString(sX + 160 - 30, sY + 195, (char*)langStr, 255, 255, 255);

		// Hover para >>
		int colorR_R = 255, colorR_G = 255, colorR_B = 255;
		if ((msX >= sX + 215 - 30) && (msX <= sX + 235 - 30) && (msY >= sY + 195) && (msY <= sY + 195 + 15))
		{
			colorR_G = 140; // Hover color
			colorR_B = 0;
		}
		PutCambriaString(sX + 215 - 30, sY + 195, ">>", colorR_R, colorR_G, colorR_B);

		if (m_cLogOutCount == -1)
		{
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 220, (char*)texts->logOut);
		}
		else
		{
			DrawMenu_SmallButton(sX, sY, msX, msY, 30, 220, (char*)texts->cancel);
		}

		if (m_iHP < 1)
		{
			DrawMenu_SmallButton(sX, sY, msX, msY, 153, 220, (char*)texts->restart);
		}

		DrawDialogCord(sX, sY, msX, msY);
		break;
	}

	case 1: // Game tab
	{
		DrawMenu_Tab(sX, sY, false, true, false, false);
		DrawMenu_Tabs_AutoSpacing(sX, sY, msX, msY, 1);

		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70, (char*)texts->whisper, m_bWhisper);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20, (char*)texts->showGrid, m_bGrid);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20, (char*)texts->gridTrans, m_bGridTrans);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20, (char*)texts->candyBig, CandySize);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20 + 20, (char*)texts->windowsKey, m_windowsKey);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20 + 20 + 20, (char*)texts->showGlares, Glares);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20 + 20 + 20 + 20, (char*)texts->showStars, Stars);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20, (char*)texts->showShadows, Shadows);

		DrawMenu_CheckBoxSettings(msX, msY, sX + 142, sY + 70, (char*)texts->showColors, Colors);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 142, sY + 70 + 20, (char*)texts->hideTrees, Trees);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 142, sY + 70 + 20 + 20, (char*)texts->showSteeps, Steeps);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 142, sY + 70 + 20 + 20 + 20, (char*)texts->itemsBoxs, ShadowsBox);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 142, sY + 70 + 20 + 20 + 20 + 20, (char*)texts->dialogTrans, m_bDialogTrans);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 142, sY + 70 + 20 + 20 + 20 + 20 + 20, (char*)texts->questHelper, m_bQuestHelper);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 142, sY + 70 + 20 + 20 + 20 + 20 + 20 + 20, (char*)texts->redSteps, RedSteeps);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 142, sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20, (char*)texts->bigItems, DEF_SPRID_ITEMGROUND_PIVOTPOINT != ItemGround);

		DrawDialogCord(sX, sY, msX, msY);
		break;
	}

	case 2: // Audio tab
	{
		DrawMenu_Tab(sX, sY, false, false, true, false);
		DrawMenu_Tabs_AutoSpacing(sX, sY, msX, msY, 2);

		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70, (char*)texts->spritesLowFps, LowSprites);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20, (char*)texts->threeDamage, StaggerDmg);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20, (char*)texts->roofs, Roofs);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20, (char*)texts->shines, bPlayerShines);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20 + 20, (char*)texts->npcHealthBar, BarraNpc);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20 + 20 + 20, (char*)texts->showShinning, ShowShin);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20 + 20 + 20 + 20, (char*)texts->modernMouse, m_DInput.m_bModernMouse);
		DrawMenu_CheckBoxSettings(msX, msY, sX + 30, sY + 70 + 20 + 20 + 20 + 20 + 20 + 20 + 20, (char*)texts->newCamera, !m_bOldCamera);

		DrawDialogCord(sX, sY, msX, msY);
		break;
	}
	case 3: // Mouse tab
	{
		DrawMenu_Tab(sX, sY, false, false, false, true);
		DrawMenu_Tabs_AutoSpacing(sX, sY, msX, msY, 3);

		const int offsetX = -10; // desplazamiento hacia la izquierda
		const int offsetY = -40; // desplazamiento hacia arriba

		// Sound
		PutString(sX + 34 + offsetX - 1, sY + 120 + offsetY, (char*)texts->sound, RGB(255, 140, 0));
		PutString(sX + 34 + offsetX, sY + 120 + offsetY, (char*)texts->sound, RGB(255, 140, 0));
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GRID, sX + 184 + offsetX, sY + 122 + offsetY, 12, TRUE, true);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GRID, sX + 71 + offsetX, sY + 127 + offsetY, 11, TRUE, true);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 71 + m_cSoundVolume + offsetX, sY + 127 + offsetY, 8);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GRID, sX + 221 + offsetX, sY + 122 + offsetY, 12, TRUE, true);
		wsprintf(G_cTxt, "%d", m_cSoundVolume);
		PutAlignedString(sX + 184 + offsetX, sX + 218 + offsetX, sY + 122 + offsetY, G_cTxt, 255, 255, 255);

		if (m_bSoundFlag) {
			if (m_bSoundStat) PutAlignedString(sX + 221 + offsetX, sX + 255 + offsetX, sY + 122 + offsetY, (char*)texts->on, 255, 255, 255);
			else PutAlignedString(sX + 221 + offsetX, sX + 255 + offsetX, sY + 122 + offsetY, (char*)texts->off, 200, 200, 200);
		}
		else PutString(sX + 221 + offsetX, sY + 122 + offsetY, (char*)texts->disabled, RGB(100, 100, 100));

		// Music
		PutString(sX + 35 + offsetX - 1, sY + 140 + offsetY, (char*)texts->music, RGB(255, 140, 0));
		PutString(sX + 35 + offsetX, sY + 140 + offsetY, (char*)texts->music, RGB(255, 140, 0));
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GRID, sX + 184 + offsetX, sY + 142 + offsetY, 12, TRUE, true);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GRID, sX + 71 + offsetX, sY + 147 + offsetY, 11, TRUE, true);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 71 + m_cMusicVolume + offsetX, sY + 147 + offsetY, 8);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GRID, sX + 221 + offsetX, sY + 142 + offsetY, 12, TRUE, true);
		wsprintf(G_cTxt, "%d", m_cMusicVolume);
		PutAlignedString(sX + 184 + offsetX, sX + 218 + offsetX, sY + 142 + offsetY, G_cTxt, 255, 255, 255);

		if (m_bSoundFlag) {
			if (m_bMusicStat) PutAlignedString(sX + 221 + offsetX, sX + 255 + offsetX, sY + 142 + offsetY, (char*)texts->on, 255, 255, 255);
			else PutAlignedString(sX + 221 + offsetX, sX + 255 + offsetX, sY + 142 + offsetY, (char*)texts->off, 200, 200, 200);
		}
		else PutString(sX + 221 + offsetX, sY + 142 + offsetY, (char*)texts->disabled, RGB(100, 100, 100));

		if ((cLB != 0) && (iGetTopDialogBoxIndex() == 95))
		{
			// Control de volumen de sonido
			if ((msX >= sX + 66 + offsetX) && (msX <= sX + 178 + offsetX) && (msY >= sY + 122 + offsetY) && (msY <= sY + 136 + offsetY))
			{
				m_cSoundVolume = msX - (sX + 77 + offsetX);
				if (m_cSoundVolume > 100) m_cSoundVolume = 100;
				if (m_cSoundVolume < 0) m_cSoundVolume = 0;
			}

			// Control de volumen de música
			if ((msX >= sX + 66 + offsetX) && (msX <= sX + 178 + offsetX) && (msY >= sY + 142 + offsetY) && (msY <= sY + 156 + offsetY))
			{
				m_cMusicVolume = msX - (sX + 77 + offsetX);
				if (m_cMusicVolume > 100) m_cMusicVolume = 100;
				if (m_cMusicVolume < 0) m_cMusicVolume = 0;

				if (m_bSoundFlag)
				{
					int iVol = (m_cMusicVolume - 100) * 20;
					if (iVol > 0) iVol = 0;
					if (iVol < -10000) iVol = -10000;

					if (m_pBGM != NULL)
					{
						m_pBGM->bStop(TRUE);
						m_pBGM->Play(FALSE, 0, iVol);
					}
				}
			}
		}
		else m_stDialogBoxInfo[95].bIsScrollSelected = FALSE;

		break;
	}
	}

	// Control panel text
	const char* controlText = texts->controlPanel;

	// Definir el área de detección del texto
	SIZE size;
	HDC hDC = m_DDraw.m_hDC;
	HFONT hOldFont = (HFONT)SelectObject(hDC, m_DDraw.m_hFontInUse4);
	GetTextExtentPoint32(hDC, controlText, strlen(controlText), &size);
	SelectObject(hDC, hOldFont);

	// Coordenadas del texto
	int textX = sX + (sizeX / 2) - (size.cx / 2); // Centrado horizontal
	int textY = sY + 248;
	int textW = size.cx;
	int textH = size.cy;

	// Mouseover check
	bool isHover = (msX >= textX) && (msX <= textX + textW) &&
		(msY >= textY) && (msY <= textY + textH);

	// Color dinámico
	if (isHover)
		PutAlignedCambriaString(sX, sX + sizeX, textY, (char*)controlText, 255, 140, 0); // Amarillo
	else
		PutAlignedCambriaString(sX, sX + sizeX, textY, (char*)controlText, 255, 255, 255); // Blanco
}
void CGame::DrawMenu_Tab(short sX, short sY, bool tab1, bool tab2, bool tab3, bool tab4)
{	
	DrawShadowDialog(sX, sY, 95);

	PutCenterStringTitle(sX, sX + m_stDialogBoxInfo[95].sSizeX, sY + 5, "System Menu", 255, 255, 255);

	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	ZeroMemory(G_cTxt, sizeof(G_cTxt));
	wsprintf(G_cTxt, "%02d:%02d:%02d", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	PutCambriaString(sX + 195, sY + 9, G_cTxt, 255, 255, 255);

	//PutString_SprFont2(sX + 80, sY + 5, "", 255, 255, 255);	
}


void CGame::DrawMenuTab_Text(short msX, short msY, short sX, short sY, char * text, bool active)
{
	if (active) { PutCambriaTabString(sX, sY, text, 255, 140, 0); return; }
	else if ((msX >= sX - 6) && (msX <= sX + 53 - 6) && (msY >= sY) && (msY <= sY + 24)) PutCambriaTabString(sX, sY, text, 255, 140, 0); // tab text
	else PutCambriaTabString(sX, sY, text, 255, 255, 255); // tab text		 //blue 0, 120, 255 yellow 255, 247, 0
}

const char* TabsLang[2][4] = {
	{ "System", "Options", "Extras", "Audio" },    // Inglés (client_lang = 0)
	{ "Sistema", "Opciones", "Extras", "Audio" }   // Español (client_lang = 1)
	// Si querés agregar coreano: { "시스템", "옵션", "기타", "오디오" }
};


void CGame::HandleTabsClick(short sX, short sY, short msX, short msY)
{
	const int tabCount = 4;
	const char** tabs = TabsLang[client_lang];

	HDC hDC = m_DDraw.m_hDC;
	HFONT hOldFont = (HFONT)SelectObject(hDC, m_DDraw.m_hFontInUse4);

	int totalWidth = 0;
	for (int i = 0; i < tabCount; ++i)
	{
		SIZE size;
		GetTextExtentPoint32(hDC, tabs[i], strlen(tabs[i]), &size);
		totalWidth += size.cx;
		if (i < tabCount - 1) totalWidth += 20;
	}

	int startX = sX + (m_stDialogBoxInfo[95].sSizeX / 2) - (totalWidth / 2);
	int currentX = startX;

	for (int i = 0; i < tabCount; ++i)
	{
		SIZE size;
		GetTextExtentPoint32(hDC, tabs[i], strlen(tabs[i]), &size);

		if ((msX >= currentX) && (msX <= currentX + size.cx) &&
			(msY >= sY + 31) && (msY <= sY + 52))
		{
			m_stDialogBoxInfo[95].cMode = i;
			PlaySound('E', 14, 5);
			SelectObject(hDC, hOldFont);
			return;
		}

		currentX += size.cx + 20;
	}

	SelectObject(hDC, hOldFont);
}
void CGame::DrawMenu_Tabs_AutoSpacing(short sX, short sY, short msX, short msY, int activeTab)
{
	const int tabCount = 4;
	const char** tabs = TabsLang[client_lang];

	HDC hDC = m_DDraw.m_hDC;
	HFONT hOldFont = (HFONT)SelectObject(hDC, m_DDraw.m_hFontInUse4);

	int totalWidth = 0;
	for (int i = 0; i < tabCount; ++i)
	{
		SIZE size;
		GetTextExtentPoint32(hDC, tabs[i], strlen(tabs[i]), &size);
		totalWidth += size.cx;
		if (i < tabCount - 1) totalWidth += 20;
	}

	int startX = sX + (m_stDialogBoxInfo[95].sSizeX / 2) - (totalWidth / 2);
	int currentX = startX;

	for (int i = 0; i < tabCount; ++i)
	{
		SIZE size;
		GetTextExtentPoint32(hDC, tabs[i], strlen(tabs[i]), &size);

		bool isActive = (i == activeTab);
		DrawMenuTab_Text(msX, msY, currentX, sY + 36, (char*)tabs[i], isActive);

		currentX += size.cx + 20;
	}

	SelectObject(hDC, hOldFont);
}


void CGame::DrawMenu_CheckBoxSettings(short msX, short msY, short sX, short sY, char * text, bool active)
{
	//spr
	m_DDraw.DrawShadowBox(sX, sY, sX + 15, sY + 15);
	m_DDraw.DrawShadowBox(sX, sY, sX + 15, sY + 15);
	DrawMenu_TextBox(sX, sY, 15, 15);

	if (active) { DrawNewDialogBox2(SPRID_SETTINGS_CHECKED, sX, sY + 2, 0, FALSE, FALSE); }

	//text
	if ((msX >= sX) && (msX <= sX + 100) && (msY >= sY) && (msY <= sY + 13))
	{
		PutCambriaString(sX + 20, sY + 1, text, 255, 140, 0);
	}
	else
	{
		PutCambriaString(sX + 20, sY + 1, text, 255, 255, 255);
	}
}


void CGame::DrawMenu_CheckBoxSettingsAudio(short msX, short msY, short sX, short sY, char * text, bool active)
{
	//spr
	m_DDraw.DrawShadowBox(sX, sY, sX + 15, sY + 15);
	m_DDraw.DrawShadowBox(sX, sY, sX + 15, sY + 15);
	DrawMenu_TextBox(sX, sY, 15, 15);

	if (active) { DrawNewDialogBox2(SPRID_SETTINGS_CHECKED, sX, sY + 2, 0, FALSE, FALSE); }

	//text
	if ((msX >= sX) && (msX <= sX + 55) && (msY >= sY) && (msY <= sY + 13))
	{
		PutString2(sX + 20, sY + 1, text, 255, 140, 0);
	}
	else
	{
		PutString2(sX + 20, sY + 1, text, 255, 255, 255);
	}
}

void CGame::DrawDialogLine(short sX, short sY, short posX, short posY, int ilength)
{
	for (int i = 0; i < ilength; i++)
	{
		auto add = posX + i;
		m_DDraw.PutPixel(sX + add, sY + posY, 255, 255, 255);
	}
}

void CGame::DrawPanelButton(int cType, int sX, int sY, int iFrame)
{
	DWORD dwTime = G_dwGlobalTime;

	if (m_pSprite[cType] == NULL) return;	
	m_pSprite[cType]->PutSpriteFastMiniScaled(sX, sY, iFrame, dwTime);
}

void CGame::Render_ControlPanel(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 96;
	int maxlines = 13;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;
	int tmpindex = -1;

	DrawNewDialogBox2(SPRID_NEWHEATPANEL, sX, sY, 0, FALSE, TRUE);
	DrawNewDialogBox2(SPRID_NEWHEATPANEL, sX, sY, 0, FALSE, TRUE);

	//m_pSprite[SPRID_FIREEFFECT]->PutTransSprite70(sX + 115, sY + 120, sFireFrame, 0);
/*
	DrawNewDialogBox2(SPRID_NEWHEATPANEL, sX, sY, 0, FALSE, TRUE);/ *
	DrawNewDialogBox2(SPRID_NEWHEATPANEL, sX, sY, 0, FALSE, TRUE);* /*/
	
	const int buttonStartX = sX + 10;

	const int buttonPosY[] = {
		sY + 27,
		sY + 27 + 36,
		sY + 27 + 36 + 38,
		sY + 27 + 36 + 38 + 38,
		sY + 27 + 36 + 38 + 38 + 37
	};

	const int iconFrames[] = { 0, 2, 1, 3, 4 };

	const int textPosX[] = {
		sX + 33,
		sX + 40,
		sX + 37,
		sX + 37,
		sX + 33
	};

	const int textPosY[] = {
		sY + 28,
		sY + 28 + 38,
		sY + 28 + 38 + 36,
		sY + 28 + 38 + 36 + 38,
		sY + 28 + 38 + 36 + 38 + 37
	};

	const char* labels[] = { "Noticias", "Info", "Drops", "Cmds", "Ranking" };

	for (int i = 0; i < 5; ++i)
	{
		DrawPanelButton(SPRID_NEWHEATPANELBUTTONS, buttonStartX, buttonPosY[i], iconFrames[i]);

		bool isHover = (msX >= textPosX[i]) && (msX <= textPosX[i] + 90) &&
			(msY >= textPosY[i]) && (msY <= textPosY[i] + 15);

		bool isActive = (m_stDialogBoxInfo[96].cMode == i);

		int r = (isHover || isActive) ? 255 : 255;
		int g = (isHover || isActive) ? 140 : 255;
		int b = (isHover || isActive) ? 0 : 255;

		PutCambriaTabString(textPosX[i], textPosY[i], (char*)labels[i], r, g, b);
	}

	switch (mode)
	{
	case 0:
	{
		PutCenterStringTitle(sX + 100, sX + 330, sY + 30, "Noticias", 255, 140, 0);
		_LoadTextDlgContents(1000);
		LoadTextControlPanel(sX + 100, sY + 50, msX, msY, msZ, cLB);
		break;
	}
	case 1:
	{
		PutCenterStringTitle(sX + 100, sX + 330, sY + 30, "Informacion", 255, 140, 0);
		_LoadTextDlgContents(2000);
		LoadTextControlPanel(sX + 100, sY + 50, msX, msY, msZ, cLB);
		break;
	}

	case 2:
	{
		PutCenterStringTitle(sX + 100, sX + 330, sY + 30, "Drop list", 255, 140, 0);
		_LoadTextDlgContents(4000);
		LoadTextControlPanel(sX + 100, sY + 50, msX, msY, msZ, cLB);
		break;
	}

	case 3: // Panel de comandos
	{
		PutCenterStringTitle(sX + 100, sX + 330, sY + 30, "Comandos disponibles", 255, 140, 0);

		// Barra separadora y títulos
		PutAlignedTahomaString(sX + 100, sX + 330, sY + 60, "------------------------------------------------", 255, 255, 255);
		PutAlignedTahomaString(sX + 100, sX + 330, sY + 75, "Comando                                      Costo", 255, 140, 0);
		PutAlignedTahomaString(sX + 100, sX + 330, sY + 85, "------------------------------------------------", 255, 255, 255);

		//-------------------------------
		// Command Maname
		bool hoverMana = (msX >= sX + 100) && (msX <= sX + 330) &&
			(msY >= sY + 100) && (msY <= sY + 115);

		int rMana = 255, gMana = 255, bMana = 255;

		if (DecriptInt(m_iGizonItemUpgradeLeft) < 300) { rMana = 255; gMana = 140; bMana = 0; }
		if (hoverMana) { rMana = 255; gMana = 140; bMana = 0; }

		PutAlignedTahomaString(sX + 100, sX + 330, sY + 100, "Maname                           50 Majestics", rMana, gMana, bMana);

		if (hoverMana)
		{
			PutAlignedTahomaString(sX + 100, sX + 330, sY + 300, "Maname: Mana ilimitado 30 min (50 Majestics)", 255, 255, 255);
		}

		//-------------------------------
		// Command Zerkme
		bool hoverZerk = (msX >= sX + 100) && (msX <= sX + 330) &&
			(msY >= sY + 120) && (msY <= sY + 135);

		int rZerk = 255, gZerk = 255, bZerk = 255;

		if (DecriptInt(m_iGizonItemUpgradeLeft) < 300) { rZerk = 255; gZerk = 140; bZerk = 0; }

		if (hoverZerk) { rZerk = 255; gZerk = 140; bZerk = 0; }

		PutAlignedTahomaString(sX + 100, sX + 330, sY + 120, "Zerkme                            300 Magestics", rZerk, gZerk, bZerk);

		if (hoverZerk)
		{
			PutAlignedTahomaString(sX + 100, sX + 330, sY + 300, "Zerkme: Berserk 10 min (300 Magestics)", 255, 255, 255);
		}

		//-------------------------------
		// Command Changecity
		bool hoverCity = (msX >= sX + 100) && (msX <= sX + 330) &&
			(msY >= sY + 140) && (msY <= sY + 155);

		int rCity = 255, gCity = 255, bCity = 255;

		if (DecriptInt(m_iEnemyKillCount) < 50) { rCity = 255; gCity = 140; bCity = 0; }
		if (hoverCity) { rCity = 255; gCity = 140; bCity = 0; }

		PutAlignedTahomaString(sX + 100, sX + 330, sY + 140, "Changecity                                  50 EKs", rCity, gCity, bCity);

		if (hoverCity)
		{
			PutAlignedTahomaString(sX + 100, sX + 330, sY + 300, "Changecity: Cambiar ciudad (50 EKs)", 255, 255, 255);
		}

		//-------------------------------
		// Command Expx3
		bool hoverExp = (msX >= sX + 100) && (msX <= sX + 330) &&
			(msY >= sY + 160) && (msY <= sY + 175);

		int rExp = 255, gExp = 255, bExp = 255;

		if (DecriptInt(m_iContribution) < 25000) { rExp = 255; gExp = 140; bExp = 0; }
		if (hoverExp) { rExp = 255; gExp = 140; bExp = 0; }

		PutAlignedTahomaString(sX + 100, sX + 330, sY + 160, "Expx3                                  25k Contrib", rExp, gExp, bExp);

		if (hoverExp)
		{
			PutAlignedTahomaString(sX + 100, sX + 330, sY + 300, "Expx3: EXP x3 15 min (25k Contrib)", 255, 255, 255);
		}

		break;
	}


	case 4: // Ranking opciones
	{
		PutCenterStringTitle(sX + 100, sX + 330, sY + 30, "Ranking", 255, 140, 0);

		const char* rankOptions[] = {
			"Players Killed Top 15",
			"Enemy Killed Top 15",
			"Enemy Kill Top 15",
			"City Friend Killed Top 15"/*,
			"Assassin Top 15"*/
		};

		for (int i = 0; i < 4; ++i)
		{
			int textX1 = sX + 100;
			int textX2 = sX + 330;
			int textY = sY + 70 + i * 30;

			bool isHover = (msX >= textX1) && (msX <= textX2) &&
				(msY >= textY) && (msY <= textY + 20);

			int r = isHover ? 255 : 255;
			int g = isHover ? 140 : 255;
			int b = isHover ? 0 : 255;

			PutAlignedTahomaString(textX1, textX2, textY, (char*)rankOptions[i], r, g, b);
		}


		// Botón Back con hover
		int backX1 = sX + 100;
		int backX2 = sX + 330;
		int backY = sY + 290;

		PutAlignedTahomaString(backX1, backX2, backY, "Los ranking se actualizan cada 30 minutos.", 255, 255, 255);

		break;
	}

	case 5: // Mostrar ranking recibido
	{
	/*	PutCenterStringTitle(sX + 100, sX + 330, sY + 30, "Primeros 50 jugadores", 255, 140, 0);*/

		// Mostrar el nombre del ranking seleccionado
		PutAlignedTahomaString(sX + 100, sX + 330, sY + 25, g_cLastRankName, 255, 140, 0);

		ShowRanking(96, sX + 75, sY - 70, msX, msY, msZ);

		// Botón Back con hover
		int backX1 = sX + 100;
		int backX2 = sX + 330;
		int backY = sY + 310;

		bool isHover = (msX >= backX1) && (msX <= backX2) &&
			(msY >= backY) && (msY <= backY + 20);

		int r = isHover ? 255 : 255;
		int g = isHover ? 140 : 255;
		int b = isHover ? 0 : 255;

		/*PutAlignedTahomaString(backX1, backX2, backY, "Atras", r, g, b);*/

		break;
	}
	case 6: // Confirmación de comando
	{
		PutCenterStringTitle(sX + 100, sX + 330, sY + 30, "Confirmar comando", 255, 140, 0);

		const char* commandTexts[] = {
			"Maname - 300 Magestics",
			"Zerkme - 300 Magestics",
			"Changecity - 50 EKs",
			"Expx3 - 25k Contribution"
		};

		if (g_sLastCommandSelected >= 0 && g_sLastCommandSelected < 4)
		{
			PutAlignedTahomaString(sX + 100, sX + 330, sY + 80, (char*)commandTexts[g_sLastCommandSelected], 255, 255, 255);
			PutAlignedTahomaString(sX + 100, sX + 330, sY + 120, "Deseas confirmar esta operacion?", 255, 255, 255);
		}

		// Botón Confirmar
		bool hoverConfirm = (msX >= sX + 100) && (msX <= sX + 330) &&
			(msY >= sY + 160) && (msY <= sY + 175);

		int rConfirm = hoverConfirm ? 255 : 255;
		int gConfirm = hoverConfirm ? 140 : 255;
		int bConfirm = hoverConfirm ? 0 : 255;

		PutAlignedTahomaString(sX + 100, sX + 330, sY + 160, "Confirmar", rConfirm, gConfirm, bConfirm);

		// Botón Cancelar
		bool hoverCancel = (msX >= sX + 100) && (msX <= sX + 330) &&
			(msY >= sY + 190) && (msY <= sY + 205);

		int rCancel = hoverCancel ? 255 : 255;
		int gCancel = hoverCancel ? 140 : 255;
		int bCancel = hoverCancel ? 0 : 255;

		PutAlignedTahomaString(sX + 100, sX + 330, sY + 190, "Cancelar", rCancel, gCancel, bCancel);

		break;
	}

	}

	DrawDialogCord(sX, sY, msX, msY);
}

void CGame::SendRequestRankToServer(short rankmode)
{
	switch (rankmode)
	{
	case 0: SendChat("/reqtopek"); break;
	case 1: SendChat("/reqtoptotalek"); break;
	case 2: SendChat("/reqtoprep"); break;
	case 3: SendChat("/reqtopcontrib"); break;
	/*case 4: SendChat("/reqtoppk"); break;*/
	default: break;
	}
}


void CGame::HandleClick_ControlPanel(short msX, short msY)
{
	short sX, sY, sizeX, sizeY;
	int dnumber = 96;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto& mode = m_stDialogBoxInfo[dnumber].cMode;

	// Áreas de los botones (igual que en Render_ControlPanel)
	const int textPosX[] = { sX + 33, sX + 40, sX + 37, sX + 37, sX + 33 };
	const int textPosY[] = {
		sY + 28,
		sY + 28 + 38,
		sY + 28 + 38 + 36,
		sY + 28 + 38 + 36 + 38,
		sY + 28 + 38 + 36 + 38 + 37
	};

	for (int i = 0; i < 5; ++i)
	{
		bool isClick = (msX >= textPosX[i]) && (msX <= textPosX[i] + 90) &&
			(msY >= textPosY[i]) && (msY <= textPosY[i] + 15);

		if (isClick)
		{
			mode = i; // Cambiar cMode al botón clickeado
			PlaySound('E', 14, 5);
			break;
		}
	}

	switch (mode)
	{

	case 3: // Handle click en comandos
	{
		const int optionCount = 4; // Cantidad de comandos
		for (int i = 0; i < optionCount; ++i)
		{
			int textX1 = sX + 100;
			int textX2 = sX + 330;
			int textY = sY + 100 + (i * 20); // Mismo spacing que en render

			bool isClick = (msX >= textX1) && (msX <= textX2) &&
				(msY >= textY) && (msY <= textY + 15);

			if (isClick)
			{
				g_sLastCommandSelected = i; // Guarda el comando seleccionado
				m_stDialogBoxInfo[96].cMode = 6; // Va al modo confirmación
				PlaySound('E', 14, 5);
				break;
			}
		}
		break;
	}


	case 4:
	{
		const char* rankOptions[] = {
			"Players Killed Top 15",
			"Enemy Killed Top 15",
			"Enemy Kill Top 15",
			"City Friend Killed Top 15"/*,
			"Assassin Top 15"*/
		};	

		const short sV4Mapping[] = { 5, 1, 0, 2/*, 3*/ };

		for (int i = 0; i < 4; ++i)
		{
			int textX1 = sX + 100;
			int textX2 = sX + 330;
			int textY = sY + 70 + i * 30;

			bool isClick = (msX >= textX1) && (msX <= textX2) &&
				(msY >= textY) && (msY <= textY + 20);

			if (isClick)
			{
				// Guardar nombre del rank seleccionado
				ZeroMemory(g_cLastRankName, sizeof(g_cLastRankName));
				strcpy(g_cLastRankName, rankOptions[i]);

				/*// Enviar comando al servidor
				SendRequestRankToServer(i);*/
				bSendCommand(CLIENT_REQUEST_TOPMEMORY, NULL, NULL, NULL, NULL, NULL, NULL);

				// Cambiar modo de diálogo y setear sV4 como antes
				m_stDialogBoxInfo[96].cMode = 5;
				m_stDialogBoxInfo[96].sV4 = sV4Mapping[i];

				PlaySound('E', 14, 5);
				break;
			}
		}

		break;
	}

	case 5: // Mostrar resultados del ranking (modo 5)
	{
		int backX1 = sX + 100;
		int backX2 = sX + 330;
		int backY = sY + 310; // Ajustable

		bool isClick = (msX >= backX1) && (msX <= backX2) &&
			(msY >= backY) && (msY <= backY + 20);

		/*if (isClick)
		{
			mode = 4; // Volver al menú de opciones
			PlaySound('E', 14, 5);
		}*/
		break;
	}

	case 6: // Click en confirmación de comando
	{
		// Confirmar
		bool isConfirm = (msX >= sX + 100) && (msX <= sX + 330) &&
			(msY >= sY + 160) && (msY <= sY + 175);

		if (isConfirm)
		{
			switch (g_sLastCommandSelected)
			{
			case 0: SendChat("/maname"); break;
			case 1: SendChat("/zerkme"); break;
			case 2:
				bSendCommand(CLIENT_NOTIFY_CHANGECITY, NULL, NULL, NULL, NULL, NULL, NULL);
				DisableDialogBox(96);
				break;
			case 3: SendChat("/expx3"); break;
			}
			m_stDialogBoxInfo[96].cMode = 3; // Volver al menú principal
			PlaySound('E', 14, 5);
			return;
		}

		// Cancelar
		bool isCancel = (msX >= sX + 100) && (msX <= sX + 330) &&
			(msY >= sY + 190) && (msY <= sY + 205);

		if (isCancel)
		{
			m_stDialogBoxInfo[96].cMode = 3; // Volver al menú de comandos
			PlaySound('E', 14, 5);
			return;
		}

		break;
	}

	}

}


void CGame::LoadTextControlPanel(short sX, short sY, short msX, short msY, short msZ, char cLB)
{
	int i, iTotalLines, iPointerLoc, iMaxLines, FinScroll;
	double d1, d2, d3;

	iMaxLines = 20;
	
	//sX = m_stDialogBoxInfo[96].sX;
	//sY = m_stDialogBoxInfo[96].sY;

	iTotalLines = 0;
	for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
		if (m_pMsgTextList[i] != NULL) iTotalLines++;
	//if( iTotalLines > iMaxLines ) DrawNewDialogBox(DEF_SPRID_INTERFACE_NEWPANEL,sX + 237, sY + 93, 8);

	if (iGetTopDialogBoxIndex() == 96 && msZ != 0)
	{
		m_stDialogBoxInfo[96].sView = m_stDialogBoxInfo[96].sView - msZ / 60;
		m_DInput.m_sZ = 0;
	}
	if (m_stDialogBoxInfo[96].sView < 0) m_stDialogBoxInfo[96].sView = 0;
	if (iTotalLines > iMaxLines && m_stDialogBoxInfo[96].sView > iTotalLines - iMaxLines) m_stDialogBoxInfo[96].sView = iTotalLines - iMaxLines;
	if (iTotalLines > iMaxLines) {
		d1 = (double)m_stDialogBoxInfo[96].sView;
		d2 = (double)(iTotalLines - iMaxLines);
		d3 = (285.0f * d1) / d2;
		iPointerLoc = (int)(d3 + 0.5);
		//if( iTotalLines > iMaxLines ) DrawNewDialogBox(DEF_SPRID_INTERFACE_NEWPANEL,sX + 237, sY + 93, 8);
		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 232, sY - 20 + iPointerLoc, 7);
	}
	else iPointerLoc = 0;
	for (i = 0; i < iMaxLines; i++)
		if (m_pMsgTextList[i + m_stDialogBoxInfo[96].sView] != NULL) {
			//	if (m_bDialogTrans == FALSE) { // v2.173
			switch (m_pMsgTextList[i + m_stDialogBoxInfo[96].sView]->m_pMsg[0]) {
				//Blanco
			case '-': PutAlignedTahomaString(sX, sX + 235, sY + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[96].sView]->m_pMsg + 1), 255, 255, 255); break;
				//Amarillo
			case ';': PutAlignedTahomaString(sX, sX + 235, sY + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[96].sView]->m_pMsg + 1), 244, 173, 1); break;
				//Rojo
			case '@': PutAlignedTahomaString(sX, sX + 235, sY + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[96].sView]->m_pMsg + 1), 218, 43, 43); break;
				//Azul
			case '&': PutAlignedTahomaString(sX, sX + 235, sY + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[96].sView]->m_pMsg + 1), 23, 132, 163); break;
				//Verde
			case '$': PutAlignedTahomaString(sX, sX + 235, sY + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[96].sView]->m_pMsg + 1), 92, 163, 23); break;
				//Marron
			case '<': PutAlignedTahomaString(sX, sX + 235, sY + i * 13, (m_pMsgTextList[i + m_stDialogBoxInfo[96].sView]->m_pMsg + 1), 161, 120, 66); break;

			default: PutAlignedTahomaString(sX, sX + 235, sY + i * 13, m_pMsgTextList[i + m_stDialogBoxInfo[96].sView]->m_pMsg, 203, 197, 197); break;
			}
			//	}
			//	else PutAlignedString(sX, sX +235, sY +i*13, m_pMsgTextList[i + m_stDialogBoxInfo[96].sView]->m_pMsg, 203,197,197);
		}
	if (cLB != 0 && iTotalLines > iMaxLines) {
		if ((iGetTopDialogBoxIndex() == 96)) {
			FinScroll = 304;
			if ((msX >= sX + 225) && (msX <= sX + 239) && (msY >= sY) && (msY <= sY + FinScroll)) {
				d1 = (double)(msY - (sY + 98));
				d2 = (double)(iTotalLines - iMaxLines);
				d3 = (d1 * d2) / 285.0f;
				iPointerLoc = (int)d3;
				if (iPointerLoc > iTotalLines - iMaxLines) iPointerLoc = iTotalLines - iMaxLines;
				m_stDialogBoxInfo[96].sView = iPointerLoc;
			}
		}
	}
	else m_stDialogBoxInfo[96].bIsScrollSelected = FALSE;
}

void CGame::DrawRankingPanel(short sX, short sY, short msX, short msY, short msZ, char cLB)
{
	int i, iTotalLines, iPointerLoc, iMaxLines, FinScroll;
	double d1, d2, d3;

	iMaxLines = 18;

	iTotalLines = g_rankList.size();

	// Scroll con rueda de mouse
	if (iGetTopDialogBoxIndex() == 96 && msZ != 0)
	{
		m_stDialogBoxInfo[96].sView -= msZ / 60;
		m_DInput.m_sZ = 0;
	}

	if (m_stDialogBoxInfo[96].sView < 0) m_stDialogBoxInfo[96].sView = 0;
	if (iTotalLines > iMaxLines && m_stDialogBoxInfo[96].sView > iTotalLines - iMaxLines)
		m_stDialogBoxInfo[96].sView = iTotalLines - iMaxLines;

	// Dibujar scrollbar si es necesario
	if (iTotalLines > iMaxLines)
	{
		d1 = (double)m_stDialogBoxInfo[96].sView;
		d2 = (double)(iTotalLines - iMaxLines);
		d3 = (285.0f * d1) / d2;
		iPointerLoc = (int)(d3 + 0.5);

		DrawNewDialogBox(DEF_SPRID_INTERFACE_ND_GAME2, sX + 232, sY - 20 + iPointerLoc, 7);
	}
	else iPointerLoc = 0;

	// Mostrar ranking con mismo formato
	for (i = 0; i < iMaxLines; i++)
	{
		int idx = i + m_stDialogBoxInfo[96].sView;
		if (idx >= iTotalLines) break;

		const auto& entry = g_rankList[idx];

		char text[64];
		sprintf(text, "%d. %s - %d", idx + 1, entry.name, entry.points);

		PutAlignedTahomaString(sX, sX + 235, sY + i * 13, text, 255, 255, 255);
	}

	// Scroll con clic sostenido en barra
	if (cLB != 0 && iTotalLines > iMaxLines)
	{
		if (iGetTopDialogBoxIndex() == 96)
		{
			FinScroll = 304;
			if ((msX >= sX + 225) && (msX <= sX + 239) &&
				(msY >= sY) && (msY <= sY + FinScroll))
			{
				d1 = (double)(msY - (sY + 98));
				d2 = (double)(iTotalLines - iMaxLines);
				d3 = (d1 * d2) / 285.0f;
				iPointerLoc = (int)d3;
				if (iPointerLoc > iTotalLines - iMaxLines)
					iPointerLoc = iTotalLines - iMaxLines;
				m_stDialogBoxInfo[96].sView = iPointerLoc;
			}
		}
	}
	else
	{
		m_stDialogBoxInfo[96].bIsScrollSelected = FALSE;
	}
}/*
void CGame::Render_StoneAssemblySystem(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	const int dnumber = 97;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto mode = m_stDialogBoxInfo[dnumber].cMode;

	// Fondo y título
	DrawShadowDialog2(sX, sY, 97);
	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Stone Assembly", 240, 240, 240);

	// Caja donde irán los ítems
	DrawMenu_EnhBox(sX + 89, sY + 130, 90, 114);

	// Posiciones base para dibujar los 5 slots (en cruz)
	const int slotOffsets[5][2] = {
		{ 0, 0 },       // Centro
		{ -40, 0 },     // Izquierda
		{ 40, 0 },      // Derecha
		{ 0, -40 },     // Arriba
		{ 0, 40 }       // Abajo
	};

	for (int i = 0; i < 5; i++)
	{
		int itemIndex = m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i];
		if (itemIndex == -1) continue; // Slot vacío, no dibujar

		CItem *item = m_pItemList[itemIndex];
		if (!item) continue;

		// Calcular posición del sprite
		int spriteX = sX + 134 + slotOffsets[i][0];
		int spriteY = sY + 176 + slotOffsets[i][1];

		if (item->m_cItemColor != 0)
		{
			m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteRGB(
				spriteX, spriteY, item->m_sSpriteFrame,
				m_wWR[item->m_cItemColor] - m_wR[0],
				m_wWG[item->m_cItemColor] - m_wG[0],
				m_wWB[item->m_cItemColor] - m_wB[0], 0);
		}
		else
		{
			m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteFast(
				spriteX, spriteY, item->m_sSpriteFrame, 0);
		}
	}
	DrawDialogCord(sX, sY, msX, msY);
}
*/

void CGame::Render_StoneAssemblySystem(short msX, short msY, short msZ, char cLB)
{
	short sX, sY, sizeX, sizeY;
	const int dnumber = 97;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;

	// Fondo y título
	DrawShadowDialog2(sX, sY, 97);
	PutCenterString_SpriteFont(sX, sX + sizeX, sY + 3, "Stone Assembly", 240, 240, 240);

	PutAlignedTahomaString(sX, sX + sizeX, sY + 70, "Please enter the 5 CursedStones", 255, 255, 255);

	// Caja gris donde van las piezas
	const int boxX = sX + 89;
	const int boxY = sY + 130;
	DrawMenu_EnhBox(boxX, boxY, 90, 114);

	// Coordenadas relativas dentro de la caja, según orden de pieza
	const int pieceOffsets[5][2] = {
		{ 29, 9 }, // CursedStone(1)
		{ 54, 9 }, // CursedStone(2)
		{ 28, 29 }, // CursedStone(3)
		{ 42, 35 }, // CursedStone(4)
		{ 56, 29 }  // CursedStone(5)
	};

	// Dibujar cada pieza
	for (int i = 0; i < 5; i++)
	{
		int idx = m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i];
		if (idx == -1) continue;

		CItem* item = m_pItemList[idx];
		if (!item) continue;

		// Determinar número de pieza
		int pieceNum = 0;
		if (sscanf(item->m_cName, "CursedStone(%d)", &pieceNum) != 1) continue;
		if (pieceNum < 1 || pieceNum > 5) continue;

		// Calcular posición final dentro de la caja
		int spriteX = boxX + pieceOffsets[pieceNum - 1][0];
		int spriteY = boxY + pieceOffsets[pieceNum - 1][1] + 40;

		// Dibujar con o sin color
		if (item->m_cItemColor != 0)
		{
			m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteRGB(
				spriteX, spriteY, item->m_sSpriteFrame,
				m_wWR[item->m_cItemColor] - m_wR[0],
				m_wWG[item->m_cItemColor] - m_wG[0],
				m_wWB[item->m_cItemColor] - m_wB[0], 0);
		}
		else
		{
			m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteFast(
				spriteX, spriteY, item->m_sSpriteFrame, 0);
		}
	}


	DrawMenu_SmallButton(sX, sY, msX, msY, DEF_RBTNPOSX, DEF_BTNPOSY + 30, "Assemble");
	DrawMenu_SmallButton(sX, sY, msX, msY, DEF_LBTNPOSX, DEF_BTNPOSY + 30, "Cancel");

	DrawDialogCord(sX, sY, msX, msY);
}

void CGame::HandleClick_StoneAssemblySystem(short msX, short msY)
{
	short sX, sY, sizeX, sizeY;
	const int dnumber = 97;
	sX = m_stDialogBoxInfo[dnumber].sX;
	sY = m_stDialogBoxInfo[dnumber].sY;
	sizeX = m_stDialogBoxInfo[dnumber].sSizeX;
	sizeY = m_stDialogBoxInfo[dnumber].sSizeY;
	auto& mode = m_stDialogBoxInfo[dnumber].cMode;

	// Coordenadas de los botones (ajustar según sea necesario)
	const int assembleBtnX = sX + DEF_RBTNPOSX;
	const int assembleBtnY = sY + DEF_BTNPOSY + 30;
	const int cancelBtnX = sX + DEF_LBTNPOSX;
	const int cancelBtnY = sY + DEF_BTNPOSY + 30;

	// Tamaño de los botones (ajustar según el tamaño real de tus botones)
	const int btnWidth = 60;
	const int btnHeight = 20;

	// Verificar clic en el botón "Assemble"
	if (msX >= assembleBtnX && msX <= assembleBtnX + btnWidth &&
		msY >= assembleBtnY && msY <= assembleBtnY + btnHeight)
	{
		// Verificar si todas las piedras están colocadas
		bool allStonesPlaced = true;
		for (int i = 0; i < 5; i++)
		{
			if (m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i] == -1)
			{
				allStonesPlaced = false;
				break;
			}
		}

		if (allStonesPlaced)
		{
			// Todas las piedras están colocadas, proceder con el ensamblaje
			AddEventList("Ensamblando CursedStones");

			// Aquí iría la lógica para crear el ítem resultante del ensamblaje
			//Enviar mensaje
			char data[512];
			auto * cp = data;

			Push(cp, (u32)REQ_STONE_ASSAMBLE);
			Push(cp, (u16)0);

			Push(cp, 5);

			for (int b = 0; b < 5; b++)
			{
				if (m_stDialogBoxInfo[dnumber].m_iAssemblyStone[b] != -1 && m_pItemList[m_stDialogBoxInfo[dnumber].m_iAssemblyStone[b]])
				{
					Push(cp, m_stDialogBoxInfo[dnumber].m_iAssemblyStone[b]);
				}
			}

			m_pGSock->iSendMsg(data, cp - data);

			PlaySound('E', 14, 5);


			// Limpiar las piedras después del ensamblaje
			for (int i = 0; i < 5; i++)
			{
				int itemID = m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i];
				if (itemID != -1)
				{
					m_bIsItemDisabled[itemID] = FALSE;
					m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i] = -1;
				}
			}

			// Cerrar el diálogo
			DisableDialogBox(97);
		}
		else
		{
			AddEventList("Necesitas colocar las 5 piedras malditas para ensamblar.");
		}
	}
	// Verificar clic en el botón "Cancel"
	else if (msX >= cancelBtnX && msX <= cancelBtnX + btnWidth &&
		msY >= cancelBtnY && msY <= cancelBtnY + btnHeight)
	{
		// Devolver todas las piedras al inventario
		for (int i = 0; i < 5; i++)
		{
			int itemID = m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i];
			if (itemID != -1)
			{
				m_bIsItemDisabled[itemID] = FALSE;
				m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i] = -1;
			}
		}

		// Cerrar el diálogo
		DisableDialogBox(97);
		AddEventList("Ensamblaje cancelado. Las piedras han sido devueltas a tu inventario.");
	}
}

bool CGame::IsAssemblyStone(char * itemname)
{
	return string(itemname) == "CursedStone(1)" ||
		string(itemname) == "CursedStone(2)" ||
		string(itemname) == "CursedStone(3)" ||
		string(itemname) == "CursedStone(4)" ||
		string(itemname) == "CursedStone(5)";
}

void CGame::bItemDrop_AssemblyStoneSystem()
{
	int dnumber = 97;
	char cItemID;
	cItemID = (char)m_stMCursor.sSelectedObjectID;

	if (m_bIsItemDisabled[cItemID] == TRUE) return;
	if (m_cCommand < 0) return;

	if (!IsAssemblyStone(m_pItemList[cItemID]->m_cName)) return;

	// Primero verificar si ya existe una piedra con el mismo nombre
	char* currentItemName = m_pItemList[cItemID]->m_cName;
	for (int i = 0; i < 5; i++)
	{
		if (m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i] != -1)
		{
			char* existingItemName = m_pItemList[m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i]]->m_cName;
			if (string(existingItemName) == string(currentItemName))
			{
				AddEventList("Ya has agregado esta piedra antes.");
				return;
			}
		}
	}

	int itemCount = 0;
	for (int i = 0; i < 5; i++)
	{
		if (m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i] != -1)
		{
			itemCount++;
		}
	}

	if (itemCount >= 5)
	{
		AddEventList("No hay espacio para más items.");
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		if (m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i] == -1)
		{
			m_stDialogBoxInfo[dnumber].m_iAssemblyStone[i] = cItemID;
			m_bIsItemDisabled[cItemID] = TRUE;
			PlaySound('E', 29, 0);
			return;
		}
	}
}

// Dibujar borde rectangular del menú
void CGame::DrawMenuBorder(int left, int top, int right, int bottom, BYTE r, BYTE g, BYTE b)
{
	// Línea superior
	for (int x = left; x <= right; x++)
		m_DDraw.PutPixel(x, top, r, g, b);

	// Línea inferior
	for (int x = left; x <= right; x++)
		m_DDraw.PutPixel(x, bottom, r, g, b);

	// Línea izquierda
	for (int y = top; y <= bottom; y++)
		m_DDraw.PutPixel(left, y, r, g, b);

	// Línea derecha
	for (int y = top; y <= bottom; y++)
		m_DDraw.PutPixel(right, y, r, g, b);
}

// Variables miembro de la clase
double bgPosX = 0.0;  // Usamos double para mayor precisión
double bgPosY = 0.0;
double bgSpeedX = 100.0;  // Velocidad en píxeles/segundo
double bgSpeedY = 70.0;   // Velocidad en píxeles/segundo
const int bgWidth = 4000;
const int bgHeight = 4000;
int screenWidth = 800;
int screenHeight = 600;
DWORD lastUpdateTime = timeGetTime();

void CGame::setResolution()
{
	if (c_reso->IsResolution() == C640x480)
	{
		screenWidth = 640;
		screenHeight = 480;
	}
	else if (c_reso->IsResolution() == C800x600)
	{
		screenWidth = 800;
		screenHeight = 600;
	}
	else if (c_reso->IsResolution() == C1024x768)
	{
		screenWidth = 1024;
		screenHeight = 768;
	}
}
// Eliminamos la interpolación para evitar vibraciones
void CGame::RandomizeDirection()
{
	// Mantener la velocidad actual pero cambiar dirección
	double currentSpeed = sqrt(bgSpeedX*bgSpeedX + bgSpeedY*bgSpeedY);
	double angle = atan2(bgSpeedY, bgSpeedX);

	// Variación aleatoria suave (±30 grados máximo)
	angle += (rand() % 174 - 87) / 100.0;

	bgSpeedX = cos(angle) * currentSpeed;
	bgSpeedY = sin(angle) * currentSpeed;

	// Asegurar velocidad mínima
	if (fabs(bgSpeedX) < 20.0) bgSpeedX = bgSpeedX < 0 ? -20.0 : 20.0;
	if (fabs(bgSpeedY) < 20.0) bgSpeedY = bgSpeedY < 0 ? -20.0 : 20.0;
}

void CGame::UpdateBackgroundPosition(DWORD dwCurrentTime)
{
	// Calcular delta time con alta precisión
	double deltaTime = (dwCurrentTime - lastUpdateTime) / 1000.0;
	lastUpdateTime = dwCurrentTime;

	// Calcular nueva posición
	double newX = bgPosX + bgSpeedX * deltaTime;
	double newY = bgPosY + bgSpeedY * deltaTime;

	// Detección de bordes con márgenes para evitar vibración
	const double margin = 0.5; // Medio píxel de margen

	bool hitBorder = false;

	if (newX <= margin || newX >= bgWidth - screenWidth - margin)
	{
		bgSpeedX = -bgSpeedX;
		newX = (newX <= margin) ? margin : bgWidth - screenWidth - margin;
		hitBorder = true;
	}

	if (newY <= margin || newY >= bgHeight - screenHeight - margin)
	{
		bgSpeedY = -bgSpeedY;
		newY = (newY <= margin) ? margin : bgHeight - screenHeight - margin;
		hitBorder = true;
	}

	// Cambio de dirección aleatorio solo al llegar a bordes
	if (hitBorder && (rand() % 100) < 40) // 40% de probabilidad
	{
		RandomizeDirection();
	}

	// Actualizar posición
	bgPosX = newX;
	bgPosY = newY;
}

void CGame::DrawingMovingBackground()
{
	setResolution();
	DWORD dwCurrentTime = timeGetTime();
	UpdateBackgroundPosition(dwCurrentTime);

	// Calcular posición de renderizado con redondeo preciso
	int renderX = static_cast<int>(-bgPosX + 0.5); // Redondeo correcto
	int renderY = static_cast<int>(-bgPosY + 0.5);

	// Dibujar el sprite principal
	m_pSprite[SPRID_DRAWINGBACKGROUND]->PutSpriteFast(renderX, renderY, 0, dwCurrentTime);

	// Dibujar partes adyacentes solo cuando sea necesario
	if (renderX < 0)
		m_pSprite[SPRID_DRAWINGBACKGROUND]->PutSpriteFast(renderX + bgWidth, renderY, 0, dwCurrentTime);

	if (renderY < 0)
		m_pSprite[SPRID_DRAWINGBACKGROUND]->PutSpriteFast(renderX, renderY + bgHeight, 0, dwCurrentTime);

	if (renderX < 0 && renderY < 0)
		m_pSprite[SPRID_DRAWINGBACKGROUND]->PutSpriteFast(renderX + bgWidth, renderY + bgHeight, 0, dwCurrentTime);
}

void CGame::HandleItemDropForUpgradeMagicalForging(short mouseX, short mouseY)
{
	int dialogNumber = 98;
	char itemID;
	itemID = (char)m_stMCursor.sSelectedObjectID;

	auto item = m_pItemList[itemID];
	if (!item) return;

	if (m_bIsItemDisabled[itemID] == TRUE || m_cCommand < 0 | !IsSpecialWeapon(item->m_cName)) return;
	if (m_stDialogBoxInfo[dialogNumber].cMode != 0) return;

	DWORD iValue = item->m_sItemSpecEffectValue3;

	if (iValue >= 30)
	{
		AddEventList("Haz alcanzado el nivel maximo de este item.");
		return;
	}

	m_bIsItemDisabled[m_stDialogBoxInfo[dialogNumber].sV1] = FALSE;
	m_stDialogBoxInfo[dialogNumber].sV1 = itemID;
	m_bIsItemDisabled[itemID] = TRUE;
	PlaySound('E', 29, 0);
}

void CGame::RenderMagicalForgingDialog(short mouseX, short mouseY, short mouseZ, char leftButton)
{
	short dialogX, dialogY, dialogSizeX, dialogSizeY;
	int dialogNumber = 98;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;
	auto &mode = m_stDialogBoxInfo[dialogNumber].cMode;

	DrawShadowDialog2(dialogX, dialogY, dialogNumber);

	// Título del diálogo
	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 3, "Magical Forging", 0, 0, 0);
	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 4, "Magical Forging", 250, 250, 250);

	PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + dialogSizeY - 120, "Current Segments", 255, 255, 255);

	wsprintf(G_cTxt, "Fire Segments: %d, Lightning Segments: %d, Ice Segments: %d", m_iFireSegments, m_iLightingSegments, m_iIceSegments);
	PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + dialogSizeY - 100, G_cTxt, 228, 161, 6);

	// Mostrar el ítem arrastrado
	if (m_stDialogBoxInfo[dialogNumber].sV1 != -1) {
		auto item = m_pItemList[m_stDialogBoxInfo[dialogNumber].sV1];
		if (item) {
			// Definir la posición Y para el texto y el sprite
			short y = dialogY + 30; // Ajusta esta posición según sea necesario

			// Mostrar el nombre del ítem

			DWORD iValue = item->m_sItemSpecEffectValue3;						
			char itemname[256];
			char price[256];

			int cost = iValue * 300;
			if (cost == 0) cost = 300;

			if (IsIceWeapon(item->m_cName))
			{
				wsprintf(itemname, "%s Ice Dmg +%d", item->m_cName, iValue);
				wsprintf(price, "Cost: %d Ice Segments", cost);
			}
			else if (IsFireWeapon(item->m_cName))
			{
				wsprintf(itemname, "%s Fire Dmg +%d", item->m_cName, iValue);
				wsprintf(price, "Cost: %d Fire Segments", cost);
			}
			else if (IsLightningWeapon(item->m_cName))
			{
				wsprintf(itemname, "%s Lightning Dmg +%d", item->m_cName, iValue);
				wsprintf(price, "Cost: %d Lightning Segments", cost);
			}

			PutAlignedString2(dialogX, dialogX + dialogSizeX, y, itemname, 228, 161, 6);
			PutAlignedString2(dialogX, dialogX + dialogSizeX, y + 15, price, 228, 161, 6);

			// Mostrar el sprite del ítem
			short spriteX = dialogX + 10 + 115; // Ajusta la posición X del sprite
			short spriteY = y + 85; // Ajusta la posición Y del sprite

			if (item->m_cItemColor != 0) {
				// Dibujar el sprite del ítem con color
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteRGB(
					spriteX, spriteY, item->m_sSpriteFrame,
					m_wWR[item->m_cItemColor] - m_wR[0],
					m_wWG[item->m_cItemColor] - m_wG[0],
					m_wWB[item->m_cItemColor] - m_wB[0], 0);
			}
			else {
				// Dibujar el sprite del ítem sin color
				m_pSprite[DEF_SPRID_ITEMPACK_PIVOTPOINT + item->m_sSprite]->PutSpriteFast(
					spriteX, spriteY, item->m_sSpriteFrame, 0);
			}
		}
	}
	else {
		// Mostrar mensaje si no hay ítem en el diálogo
		PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 50, m_sLang == LANG_ESP ? "Please add one item to continue." : "Please add one item to continue.", 255, 255, 255);
	}

	// Dibujar botones según el modo
	switch (mode)
	{
	case 0: // Modo normal
		if (m_stDialogBoxInfo[dialogNumber].sV1 != -1)
		{
			// Dibujar botones "Upgrade" y "Cancel"
			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 10, dialogSizeY - 40, m_sLang == LANG_ESP ? "Upgrade" : "Upgrade");
			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 175, dialogSizeY - 40, m_sLang == LANG_ESP ? "Cancel" : "Cancel");
		}
		else
		{
			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 175, dialogSizeY - 40, m_sLang == LANG_ESP ? "Close" : "Close");
		}
		break;

	case 1: // Modo de confirmación
	{
		// Mostrar mensaje de confirmación
		PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 190, m_sLang == LANG_ESP ? "Are you sure you want to upgrade this item?" : "Are you sure you want to upgrade this item?", 255, 255, 255);

		// Dibujar botones "Yes" y "No"
		DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 10, dialogSizeY - 40, m_sLang == LANG_ESP ? "Yes" : "Yes");
		DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 175, dialogSizeY - 40, m_sLang == LANG_ESP ? "No" : "No");
	}
	break;

	default:
		break;
	}
}

void CGame::HandleMagicalForgingClick(short mouseX, short mouseY)
{
	short dialogX, dialogY, dialogSizeX, dialogSizeY;
	int dialogNumber = 98;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;
	auto &mode = m_stDialogBoxInfo[dialogNumber].cMode;

	switch (mode)
	{
	case 0: // Modo normal
		// Manejar clic en el botón "Upgrade"
		if ((mouseX >= dialogX + 10) && (mouseX <= dialogX + 100) && (mouseY >= dialogY + dialogSizeY - 40) && (mouseY <= dialogY + dialogSizeY - 20)) {
			if (m_stDialogBoxInfo[dialogNumber].sV1 != -1) {
				// Cambiar al modo de confirmación
				mode = 1;
				PlaySound('E', 14, 5);
			}
		}

		// Manejar clic en el botón "Cancel"
		if ((mouseX >= dialogX + 175) && (mouseX <= dialogX + 175 + 90) && (mouseY >= dialogY + dialogSizeY - 40) && (mouseY <= dialogY + dialogSizeY - 20)) {
			if (m_stDialogBoxInfo[dialogNumber].sV1 != -1) {
				// Liberar el ítem y resetear el diálogo
				m_bIsItemDisabled[m_stDialogBoxInfo[dialogNumber].sV1] = FALSE;
				m_stDialogBoxInfo[dialogNumber].sV1 = -1;
				PlaySound('E', 14, 5);
			}
			else {
				// Cerrar el diálogo
				PlaySound('E', 14, 5);
				DisableDialogBox(98);
			}
		}
		break;

	case 1: // Modo de confirmación
		// Manejar clic en el botón "Yes"
		if ((mouseX >= dialogX + 10) && (mouseX <= dialogX + 100) && (mouseY >= dialogY + dialogSizeY - 40) && (mouseY <= dialogY + dialogSizeY - 20)) {
			// Enviar mensaje para mejorar el ítem
			char data[512];
			auto *cp = data;
			Push(cp, (u32)UPGRADE_SPECIALITEMS);// UPGRADE_POWER_HERO
			Push(cp, (u16)0);
			Push(cp, m_stDialogBoxInfo[dialogNumber].sV1);
			m_pGSock->iSendMsg(data, cp - data);

			// Resetear el diálogo
			mode = 0;
			/*m_stDialogBoxInfo[dialogNumber].sV1 = -1;*/
			PlaySound('E', 14, 5);
		}

		// Manejar clic en el botón "No"
		if ((mouseX >= dialogX + 175) && (mouseX <= dialogX + 175 + 90) && (mouseY >= dialogY + dialogSizeY - 40) && (mouseY <= dialogY + dialogSizeY - 20)) {
			// Volver al modo normal
			mode = 0;
			PlaySound('E', 14, 5);
		}
		break;

	default:
		break;
	}
}

void CGame::RenderTalents(short mouseX, short mouseY, short mouseZ, char leftButton)
{
	short dialogX, dialogY, dialogSizeX, dialogSizeY;
	int dialogNumber = 99;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;
	auto &mode = m_stDialogBoxInfo[dialogNumber].cMode;

	DrawShadowDialog2(dialogX, dialogY, dialogNumber);

	// Título del diálogo
	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 3, "Talents", 0, 0, 0);
	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 4, "Talents", 250, 250, 0);

	switch (mode)
	{
		case 0: // Modo normal: lista de talentos
		{
			wsprintf(G_cTxt, "Choose your Talent");
			PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 35, G_cTxt, 255, 255, 0);

			bool bFireTalent = m_sTalent == TALENT_FIRE;
			bool bLightningTalent = m_sTalent == TALENT_LIGHTNING;
			bool bIceTalent = m_sTalent == TALENT_ICE;

			wsprintf(G_cTxt, "Fire Talent (Lvl %d)", m_sFireTalentLvl);
			DrawMenu_CheckBoxSettings(mouseX, mouseY, dialogX + 30, dialogY + 70, G_cTxt, bFireTalent);

			wsprintf(G_cTxt, "Lightning Talent (Lvl %d)", m_sLightningTalentLvl);
			DrawMenu_CheckBoxSettings(mouseX, mouseY, dialogX + 30, dialogY + 95, G_cTxt, bLightningTalent);

			wsprintf(G_cTxt, "Ice Talent (Lvl %d)", m_sIceTalentLvl);
			DrawMenu_CheckBoxSettings(mouseX, mouseY, dialogX + 30, dialogY + 120, G_cTxt, bIceTalent);

			int textX = dialogX + 30;
			int textY = dialogY + 150;

			// un “hitbox” aproximado para el texto
			int hitWidth = 120;  // ancho estimado del texto en píxeles
			int hitHeight = 20;   // alto de la fuente en píxeles

			bool mouseOver =
				(mouseX >= textX && mouseX <= textX + hitWidth &&
				mouseY >= textY && mouseY <= textY + hitHeight);

			int r = 255;
			int g = mouseOver ? 140 : 255;
			int b = mouseOver ? 0 : 255;

			PutCambriaString(textX, textY, "Upgrade Talents", r, g, b);

			PutCambriaString(dialogX + 130, dialogY + 150, "(New)", 255, 255, 0);

			PutAlignedString2(dialogX + 15, dialogX + dialogSizeX - 15, dialogY + 200, "Talents allow your character to specialize in Fire, Lightning or Ice. Changing a talent consumes 30k segments of the chosen element. Only can have one active talent at a time.", 200, 200, 200);
		}
		break;

		case 1: // Modo confirmación
		{
			// El talento a cambiar está guardado en sV1 temporalmente
			short talent = m_stDialogBoxInfo[dialogNumber].sV1;

			int currentSegments = 0;
			const int cost = 30000;
			const char* talentName = "";

			switch (talent)
			{
			case TALENT_FIRE:      currentSegments = m_iFireSegments;      talentName = "Fire Talent";      break;
			case TALENT_LIGHTNING: currentSegments = m_iLightingSegments;  talentName = "Lightning Talent"; break;
			case TALENT_ICE:       currentSegments = m_iIceSegments;       talentName = "Ice Talent";       break;
			}

			// Texto de confirmación
			wsprintf(G_cTxt, "Select %s", talentName);
			PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 60, G_cTxt, 255, 255, 0);

			wsprintf(G_cTxt, "Current Segments: %d", currentSegments);
			PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 90, G_cTxt, 228, 161, 6);

			wsprintf(G_cTxt, "Required Segments: %d", cost);
			PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 110, G_cTxt, 228, 161, 6);

			// Botones
			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 10, dialogSizeY - 40, "Accept");
			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 175, dialogSizeY - 40, "Cancel");
	
		break;
		}

		case 2: // modo upgrade options
		{
			wsprintf(G_cTxt, "Upgrade a Talent");
			PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 35, G_cTxt, 255, 255, 0);

			const int baseX = dialogX + 30;
			int y = dialogY + 70;
			const int hitW = 160;
			const int hitH = 20;

			struct TalentInfo {
				short id; const char* name; int lvl;
			} talents[] = {
				{ TALENT_FIRE, "Fire Talent", m_sFireTalentLvl },
				{ TALENT_LIGHTNING, "Lightning Talent", m_sLightningTalentLvl },
				{ TALENT_ICE, "Ice Talent", m_sIceTalentLvl },
			};

			for (auto &t : talents)
			{
				bool hover = (mouseX >= baseX && mouseX <= baseX + hitW &&
					mouseY >= y && mouseY <= y + hitH);

				int r = 255;
				int g = hover ? 140 : 255;
				int b = hover ? 0 : 255;

				wsprintf(G_cTxt, "%s (Lvl %d)", t.name, t.lvl);
				PutCambriaString(baseX, y, G_cTxt, r, g, b); // <<< hover color

				y += 25; // salto a la próxima opción
			}

			PutAlignedString2(dialogX + 15, dialogX + dialogSizeX - 15, dialogY + 200,
				"Select which talent you want to upgrade. Upgrading costs 750 points of the selected element.",
				200, 200, 200);


			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 175, dialogSizeY - 40, "Cancel");
			break;
		}

		case 3: // confirm upgrade
		{
			short talent = m_stDialogBoxInfo[dialogNumber].sV1;
			const int cost = 750;
			int currentSegments = 0;
			const char* talentName = "";

			switch (talent)
			{
			case TALENT_FIRE:      currentSegments = m_iFirePoints;      talentName = "Fire Talent";      break;
			case TALENT_LIGHTNING: currentSegments = m_iLightningPoints;  talentName = "Lightning Talent"; break;
			case TALENT_ICE:       currentSegments = m_iIcePoints;       talentName = "Ice Talent";       break;
			}

			wsprintf(G_cTxt, "Upgrade %s", talentName);
			PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 60, G_cTxt, 255, 255, 0);

			wsprintf(G_cTxt, "Current Points: %d", currentSegments);
			PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 90, G_cTxt, 228, 161, 6);

			wsprintf(G_cTxt, "Required Points: %d", cost);
			PutAlignedString2(dialogX, dialogX + dialogSizeX, dialogY + 110, G_cTxt, 228, 161, 6);

			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 10, dialogSizeY - 40, "Upgrade");
			DrawMenu_SmallButton(dialogX, dialogY, mouseX, mouseY, 175, dialogSizeY - 40, "Cancel");
	
		break;
		}
	}
}

void CGame::HandleTalentsClick(short mouseX, short mouseY)
{
	short dialogX, dialogY, dialogSizeX, dialogSizeY;
	int dialogNumber = 99;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;
	auto &mode = m_stDialogBoxInfo[dialogNumber].cMode;

	if (mode == 0) // Selección de talentos
	{
		if ((mouseX >= dialogX + 30) && (mouseX <= dialogX + 130))
		{
			if ((mouseY >= dialogY + 70) && (mouseY <= dialogY + 85)) {
				m_stDialogBoxInfo[dialogNumber].sV1 = TALENT_FIRE;
				PlaySound('E', 14, 5);
				mode = 1; return;
			}
			if ((mouseY >= dialogY + 95) && (mouseY <= dialogY + 110)) {
				m_stDialogBoxInfo[dialogNumber].sV1 = TALENT_LIGHTNING;
				PlaySound('E', 14, 5);
				mode = 1; return;
			}
			if ((mouseY >= dialogY + 120) && (mouseY <= dialogY + 135)) {
				m_stDialogBoxInfo[dialogNumber].sV1 = TALENT_ICE;
				PlaySound('E', 14, 5);
				mode = 1; return;
			}
		}

		int textX = dialogX + 30;
		int textY = dialogY + 150;

		// un “hitbox” aproximado para el texto
		int hitWidth = 120;  // ancho estimado del texto en píxeles
		int hitHeight = 20;   // alto de la fuente en píxeles

		bool mouseOver =
			(mouseX >= textX && mouseX <= textX + hitWidth &&
			mouseY >= textY && mouseY <= textY + hitHeight);

		if (mouseOver)
		{
			PlaySound('E', 14, 5);
			mode = 2; return;
		}
	}
	else if (mode == 1) // Confirmación
	{
		// Botón "Accept"
		if (mouseX >= dialogX + 10 && mouseX <= dialogX + 100 &&
			mouseY >= dialogY + dialogSizeY - 40 && mouseY <= dialogY + dialogSizeY - 25)
		{
			short talent = m_stDialogBoxInfo[dialogNumber].sV1;
			RequestChangeTalent(talent);
			PlaySound('E', 14, 5);
			mode = 0; return;
		}

		// Botón "Cancel"
		if (mouseX >= dialogX + 175 && mouseX <= dialogX + 265 &&
			mouseY >= dialogY + dialogSizeY - 40 && mouseY <= dialogY + dialogSizeY - 25)
		{
			mode = 0; // volver al menú principal de talentos
			PlaySound('E', 14, 5);
			return;
		}
	}
	else if (mode == 2)
	{
		const int baseX = dialogX + 30;
		int y = dialogY + 70;
		const int hitW = 160;
		const int hitH = 20;

		struct TalentHit {
			short id;
		} hits[] = {
			{ TALENT_FIRE },
			{ TALENT_LIGHTNING },
			{ TALENT_ICE },
		};

		for (auto &h : hits)
		{
			if (mouseX >= baseX && mouseX <= baseX + hitW &&
				mouseY >= y && mouseY <= y + hitH)
			{
				m_stDialogBoxInfo[dialogNumber].sV1 = h.id;
				PlaySound('E', 14, 5);
				mode = 3; // pasa a confirm upgrade
				return;
			}
			y += 25;
		}

		// Botón "Cancel"
		if (mouseX >= dialogX + 175 && mouseX <= dialogX + 265 &&
			mouseY >= dialogY + dialogSizeY - 40 && mouseY <= dialogY + dialogSizeY - 25)
		{
			mode = 0; // volver al menú principal de talentos
			PlaySound('E', 14, 5);
			return;
		}
	}
	else if (mode == 3)
	{
		// Botón "Accept"
		if (mouseX >= dialogX + 10 && mouseX <= dialogX + 100 &&
			mouseY >= dialogY + dialogSizeY - 40 && mouseY <= dialogY + dialogSizeY - 25)
		{
			short talent = m_stDialogBoxInfo[dialogNumber].sV1;
			RequestUpgradeTalent(talent);
			PlaySound('E', 14, 5);
			mode = 2; return;
		}

		// Botón "Cancel"
		if (mouseX >= dialogX + 175 && mouseX <= dialogX + 265 &&
			mouseY >= dialogY + dialogSizeY - 40 && mouseY <= dialogY + dialogSizeY - 25)
		{
			mode = 2;
			PlaySound('E', 14, 5);
			return;
		}
	}

}

void CGame::RequestChangeTalent(short m_sChangeTalent)
{
	if (m_sChangeTalent == m_sTalent) { AddEventList("Error. Ya usas este talento.", 11); return; }

	char data[10];
	auto *cp = data;
	Push(cp, (u32)REQUEST_CHANGETALENT);
	Push(cp, (u16)0);
	Push(cp, m_sChangeTalent);
	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::RequestUpgradeTalent(short m_sUpgradeTalent)
{
	char data[10];
	auto *cp = data;
	Push(cp, (u32)REQUEST_UPGRADETALENT);
	Push(cp, (u16)0);
	Push(cp, m_sUpgradeTalent);
	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::KeepAlive()
{
	if (!bSendKeep) return;
	char data[6];
	auto *cp = data;
	Push(cp, (u32)SEND_KEEPALIVE);
	Push(cp, (u16)0);
	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::RenderEventOfficer(short mouseX, short mouseY, short mouseZ, char leftButton)
{
	short dialogX, dialogY, dialogSizeX, dialogSizeY;
	int dialogNumber = 100;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;
	auto &mode = m_stDialogBoxInfo[dialogNumber].cMode;

	DrawShadowDialog2(dialogX, dialogY, dialogNumber);

	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 3, "Event Officer", 0, 0, 0);
	PutCenterString_SpriteFont(dialogX, dialogX + dialogSizeX, dialogY + 4, "Event Officer", 250, 250, 0);

	const int hoverR = 255, hoverG = 200, hoverB = 0;
	const int normalR = 240, normalG = 240, normalB = 240;

	struct Option
	{
		const char* text;
		int y;
	};

	Option options[] = {
		{ "Get Candys", dialogY + 50 },
		{ "Repair items", dialogY + 70 },
		{ "Exit event", dialogY + 90 }
	};

	for (const auto& opt : options)
	{		
		int textHeight = 18; 
		int marginX = 10;

		int left = dialogX + marginX;
		int right = dialogX + dialogSizeX - marginX;
		int top = opt.y;
		int bottom = opt.y + textHeight;

		bool hover = (mouseX >= left && mouseX <= right && mouseY >= top  && mouseY <= bottom);

		PutAlignedCambriaString(
			dialogX,
			dialogX + dialogSizeX,
			opt.y,
			(char*)opt.text,
			hover ? hoverR : normalR,
			hover ? hoverG : normalG,
			hover ? hoverB : normalB
			);
	}
}

void CGame::HandleEventOfficerClick(short mouseX, short mouseY)
{
	short dialogX, dialogY, dialogSizeX, dialogSizeY;
	int dialogNumber = 100;
	dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;
	auto &mode = m_stDialogBoxInfo[dialogNumber].cMode;

	struct Option
	{
		const char* text;
		int y;
		const char* chatCmd;
	};

	Option options[] = {
		{ "Get Candys", dialogY + 50, "/dkreloadcandys" },
		{ "Repair items", dialogY + 70, "/dkreapiritems" },
		{ "Exit event", dialogY + 90, "/dkexit" }
	};

	int textHeight = 18; 
	int marginX = 10;   

	for (const auto& opt : options)
	{
		int left = dialogX + marginX;
		int right = dialogX + dialogSizeX - marginX;
		int top = opt.y;
		int bottom = opt.y + textHeight;

		if (mouseX >= left && mouseX <= right &&
			mouseY >= top  && mouseY <= bottom)
		{
			SendChat((char*)opt.chatCmd);
			PlaySound('E', 14, 5);
			DisableDialogBox(dialogNumber);
			break; 
		}
	}
}

void CGame::handleClothesGlare(int iItemGlare, int iItemIndex, int sx, int sy, int frame, DWORD dwTime)
{
	switch (iItemGlare) {
	case 0: break;
	case 3: m_pSprite[iItemIndex]->PutSpriteRGB(sx, sy, frame, 0, 0, m_sDrawFlag, dwTime); break; // Blue
	case 4: m_pSprite[iItemIndex]->PutSpriteRGB(sx, sy, frame, m_sDrawFlag, 0, 0, dwTime); break; // Red
	case 5: m_pSprite[iItemIndex]->PutSpriteRGB(sx, sy, frame, 0, m_sDrawFlag, 0, dwTime); break; // Green
	case 6: m_pSprite[iItemIndex]->PutSpriteRGB(sx, sy, frame, m_sDrawFlag, m_sDrawFlag, 0, dwTime); break; //Gold
	case 7:	m_pSprite[iItemIndex]->PutSpriteRGB(sx, sy, frame, m_sDrawFlag, 0, m_sDrawFlag / 2, dwTime); break; //Pink
	case 8: m_pSprite[iItemIndex]->PutSpriteRGB(sx, sy, frame, 0, m_sDrawFlag, m_sDrawFlag, dwTime); break;  // Turquesa
	case 9: m_pSprite[iItemIndex]->PutSpriteRGB(
		sx, sy, frame,
		m_sDrawFlag / 3,    // Rojo: bastante menos
		0,                  // Verde: nada
		m_sDrawFlag,        // Azul: al máximo
		dwTime); break;     // Violet
	}
}

/*

void CGame::handleLeggsGlares(int iPantsGlare, int iPantsIndex, int sx, int sy, int frame, DWORD dwTime)
{
	switch (iPantsGlare) {
	case 0: break;
	case 3: m_pSprite[iPantsIndex]->PutSpriteRGB(sx, sy, frame, 0, 0, m_sDrawFlag, dwTime); break; // Green Glare
	case 4: m_pSprite[iPantsIndex]->PutSpriteRGB(sx, sy, frame, m_sDrawFlag, 0, 0, dwTime); break; // Green Glare
	case 5: m_pSprite[iPantsIndex]->PutSpriteRGB(sx, sy, frame, 0, m_sDrawFlag, 0, dwTime); break; // Green Glare
	case 6: m_pSprite[iPantsIndex]->PutSpriteRGB(sx, sy, frame, m_sDrawFlag, m_sDrawFlag, 0, dwTime); break; // Green Glare
	case 7: m_pSprite[iPantsIndex]->PutSpriteRGB(sx, sy, frame, m_sDrawFlag, 0, m_sDrawFlag / 2, dwTime); break;
	}
}*/



void CGame::RenderRecallDialog(short mouseX, short mouseY, short mouseZ, char leftButton)
{
	const int dialogNumber = 101;

	const int dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	const int dialogY = m_stDialogBoxInfo[dialogNumber].sY;
	const int dialogSizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	const int dialogSizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;

	DrawShadowDialogWithCorners(dialogX, dialogY, dialogNumber);

	// Sprite del mapa dentro del diálogo
	const int spriteX = dialogX + 10;
	const int spriteY = dialogY + 10;
	const int spriteW = 400;
	const int spriteH = 400;

	// Mapa real = 9600 px -> 9600/32 = 300 tiles
	const int MAP_TILES = 300;

	// Factor de escala tiles → sprite
	const float scale = spriteW / (float)MAP_TILES; // 400/300 = 1.333f

	auto TileToSprite = [&](int tileX, int tileY) -> POINT {
		POINT p;
		p.x = spriteX + int(tileX * scale);
		p.y = spriteY + int(tileY * scale);
		return p;
	};

	// === Aresden ===
	if (string(m_cLocation) == "aresden")
	{
		if (isInMap("aresden") || isInMap("gshop_1") || isInMap("wrhus_1")/* || isInMap("2ndmiddle") || isInMap("dungeon")*/)
		{
			DrawNewDialogBox2(SPRID_GUIDEMAP + 7, spriteX, spriteY, 0, TRUE);

			const POINT aresdenTilePts[] = {
				{ 111, 243 },
				{ 137, 203 },
				{ 169, 144 },
				{ 64, 123 },
				{ 137, 47 },
				{ 249, 77 },
				{ 249, 154 },
				{ 251, 235 }
			};

			for (auto &t : aresdenTilePts) {
				POINT q = TileToSprite(t.x, t.y);
				DrawSafeIcon2(q.x + 2, q.y + 2);
			}
		}
	}

	// === Elvine ===
	if (string(m_cLocation) == "elvine")
	{
		if (isInMap("elvine") || isInMap("gshop_2") || isInMap("wrhus_2")/* || isInMap("2ndmiddle") || isInMap("dungeon")*/)
		{
			DrawNewDialogBox2(SPRID_GUIDEMAP + 8, spriteX, spriteY, 0, TRUE);

			const POINT elvineTilePts[] = {
				{ 169, 144 },
				{ 59, 45 },
				{ 79, 197 },
				{ 221, 218 },
				{ 154, 56 },
				{ 156, 247 },
				{ 108, 87 },
				{ 240, 128 },
			};

			for (auto &t : elvineTilePts) {
				POINT q = TileToSprite(t.x, t.y);
				DrawSafeIcon2(q.x + 2, q.y + 2);
			}
		}
	}
}

struct RecallPoint {
	int tileX;
	int tileY;
	short index; // id del recall point
};

void CGame::HandleRecallDialogClick(short mouseX, short mouseY)
{
	const int dialogNumber = 101;
	const int dialogX = m_stDialogBoxInfo[dialogNumber].sX;
	const int dialogY = m_stDialogBoxInfo[dialogNumber].sY;

	const int spriteX = dialogX + 10;
	const int spriteY = dialogY + 10;
	const int spriteW = 400;
	const int spriteH = 400;

	const int MAP_TILES = 300;
	const float scale = spriteW / (float)MAP_TILES;

	auto TileToSprite = [&](int tileX, int tileY) -> POINT {
		POINT p;
		p.x = spriteX + int(tileX * scale);
		p.y = spriteY + int(tileY * scale);
		return p;
	};

	const int RANGE = 30;

	// === Aresden ===
	if (string(m_cLocation) == "aresden")
	{
		if (isInMap("aresden") || isInMap("gshop_1") || isInMap("wrhus_1"))
		{
			static const RecallPoint aresdenPts[] = {
				{ 111, 243, 1 }, { 137, 203, 2 }, { 169, 144, 3 }, { 64, 123, 4 },
				{ 137, 47, 5 }, { 249, 77, 6 }, { 249, 154, 7 }, { 251, 235, 8 }
			};

			for (auto &rp : aresdenPts) {
				POINT q = TileToSprite(rp.tileX, rp.tileY);
				if (mouseX >= q.x - RANGE && mouseX <= q.x + RANGE &&
					mouseY >= q.y - RANGE && mouseY <= q.y + RANGE)
				{
					PlaySound('E', 14, 5);
					SendRecallPoint(rp.index);   // 🔥 enviamos el índice correcto
					m_bRecallCasted = false;
					return;
				}
			}
		}
	}

	// === Elvine ===
	if (string(m_cLocation) == "elvine")
	{
		if (isInMap("elvine") || isInMap("gshop_2") || isInMap("wrhus_2"))
		{
			static const RecallPoint elvinePts[] = {
				{ 169, 144, 1 }, { 59, 45, 2 }, { 79, 197, 3 }, { 221, 218, 4 },
				{ 154, 56, 5 }, { 156, 247, 6 }, { 108, 87, 7 }, { 240, 128, 8 }
			};

			for (auto &rp : elvinePts) {
				POINT q = TileToSprite(rp.tileX, rp.tileY);
				if (mouseX >= q.x - RANGE && mouseX <= q.x + RANGE &&
					mouseY >= q.y - RANGE && mouseY <= q.y + RANGE)
				{
					PlaySound('E', 14, 5);
					SendRecallPoint(rp.index);   // 🔥 enviamos el índice correcto
					m_bRecallCasted = false;
					return;
				}
			}
		}
	}
}


void CGame::DrawSafeIcon2(short centerX, short centerY)
{
	const int RADIUS = 7; // radio (diámetro=15)
	const int R2 = RADIUS * RADIUS;

	for (int y = -RADIUS; y <= RADIUS; y++)
	{
		for (int x = -RADIUS; x <= RADIUS; x++)
		{
			if (x * x + y * y <= R2)
			{
				m_DDraw.PutPixel(centerX + x, centerY + y, 255, 255, 0); // Amarillo
			}
		}
	}
}


bool CGame::IsInSafeZone(short type)
{
	struct SafePoint
	{
		const char* map;
		int x;
		int y;
	};

	if (type == 1)
	{
		if (isInMap("gshop_1") || isInMap("wrhus_1")) return true;
		static const SafePoint safePoints[] =
		{
			// Aresden
			{ "aresden", 111, 243 },
			{ "aresden", 137, 203 },
			{ "aresden", 169, 144 },
			{ "aresden", 64, 123 },
			{ "aresden", 137, 47 },
			{ "aresden", 249, 77 },
			{ "aresden", 249, 154 },
			{ "aresden", 251, 235 }
		};

		const int SAFE_W = 4; // ancho
		const int SAFE_H = 3; // alto

		for (int i = 0; i < sizeof(safePoints) / sizeof(SafePoint); i++)
		{
			const SafePoint& sp = safePoints[i];

			if (isInMap((char*)sp.map))
			{
				if (m_sPlayerX >= sp.x && m_sPlayerX < sp.x + SAFE_W &&
					m_sPlayerY >= sp.y && m_sPlayerY < sp.y + SAFE_H)
				{
					return true;
				}
			}
		}
	}
	else if (type == 2)
	{
		if (isInMap("gshop_2") || isInMap("wrhus_2")) return true;
		static const SafePoint safePoints[] =
		{
			// Elvine
			{ "elvine", 169, 144 },
			{ "elvine", 59, 45 },
			{ "elvine", 79, 197 },
			{ "elvine", 221, 218 },
			{ "elvine", 154, 56 },
			{ "elvine", 156, 247 },
			{ "elvine", 108, 87 },
			{ "elvine", 240, 128 }
		};

		const int SAFE_W = 4; // ancho
		const int SAFE_H = 3; // alto

		for (int i = 0; i < sizeof(safePoints) / sizeof(SafePoint); i++)
		{
			const SafePoint& sp = safePoints[i];

			if (isInMap((char*)sp.map))
			{
				if (m_sPlayerX >= sp.x && m_sPlayerX < sp.x + SAFE_W &&
					m_sPlayerY >= sp.y && m_sPlayerY < sp.y + SAFE_H)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void CGame::SendRecallPoint(short m_sRecallPoint)
{
	char data[8];
	auto *cp = data;
	Push(cp, (u32)SEND_RECALLPOINT);
	Push(cp, (u16)0);
	Push(cp, m_sRecallPoint);
	m_pGSock->iSendMsg(data, cp - data);
}


void CGame::SendSpecialMagic(char * targetname, int magicType, short sX, short sY)
{
	char data[30];
	auto *cp = data;

	char m_cTargetName[12];
	strcpy(m_cTargetName, targetname);

	Push(cp, (u32)CLIENT_SPECIALMAGIC);
	Push(cp, (u16)0);
	Push(cp, m_cTargetName, 12);
	Push(cp, magicType);
	Push(cp, sX);
	Push(cp, sY);

	m_pGSock->iSendMsg(data, cp - data);
}

void CGame::RenderSlotMachine(short mouseX, short mouseY, short mouseZ, char leftButton)
{
	UpdateSlotMachine();

	const int dialogNumber = 102;
	const int sX = m_stDialogBoxInfo[dialogNumber].sX;
	const int sY = m_stDialogBoxInfo[dialogNumber].sY;
	const int sizeX = m_stDialogBoxInfo[dialogNumber].sSizeX;
	const int sizeY = m_stDialogBoxInfo[dialogNumber].sSizeY;

	DrawShadowDialogWithCorners(sX, sY, dialogNumber);
	PutCenterStringTitle(sX, sX + sizeX, sY + 3, "Slot Machine", 255, 215, 0);

	// Posición base de los reels
	const int reelY = sY + 40;
	const int reelX = sX + 20;
	const int reelSpacing = 60;

	for (int i = 0; i < 3; ++i)
	{
		int x = reelX + (i * reelSpacing);
		int y = reelY;
		int symbol = m_SlotMachine.reels[i].currentSymbol;

		int srcW = 32;
		int srcH = 32;
		int srcX = (symbol % 4) * srcW;
		int srcY = (symbol / 4) * srcH;

		m_pSprite[SPRID_SLOTREEL]->PutSpriteFastClip(x, y, srcX, srcY, srcW, srcH, G_dwGlobalTime);
		DrawShadowMenu_DialogBordersNoTitle(x - 2, y - 2, srcW + 4, srcH + 4);
	}

	DrawMenu_SmallButton(sX, sY, mouseX, mouseY, DEF_LBTNPOSX, DEF_BTNPOSY, "Spin");
}

void CGame::HandleSlotMachineClick(short mouseX, short mouseY)
{
	const int dialogNumber = 102;
	const int sX = m_stDialogBoxInfo[dialogNumber].sX;
	const int sY = m_stDialogBoxInfo[dialogNumber].sY;

	// Botón "Spin"
	if (mouseX >= sX + DEF_LBTNPOSX && mouseX <= sX + DEF_LBTNPOSX + 80 &&
		mouseY >= sY + DEF_BTNPOSY && mouseY <= sY + DEF_BTNPOSY + 20)
	{
		if (!m_SlotMachine.spinning)
		{
			m_SlotMachine.spinning = true;
			m_SlotMachine.lastSpinTime = G_dwGlobalTime;
			for (int i = 0; i < 3; ++i)
			{
				m_SlotMachine.reels[i].spinning = true;
				m_SlotMachine.reels[i].speed = 12.0f + i * 2;
			}
			PlaySound('E', 14, 5);
		}
	}
}

void CGame::UpdateSlotMachine()
{
	if (!m_SlotMachine.spinning) return;

	DWORD now = G_dwGlobalTime;
	float delta = (now - m_SlotMachine.lastSpinTime) * 0.05f;
	m_SlotMachine.lastSpinTime = now;

	bool allStopped = true;

	for (int i = 0; i < 3; ++i)
	{
		if (m_SlotMachine.reels[i].spinning)
		{
			m_SlotMachine.reels[i].offset += m_SlotMachine.reels[i].speed * delta;
			if (m_SlotMachine.reels[i].offset > 32.0f)
			{
				m_SlotMachine.reels[i].offset = 0;
				m_SlotMachine.reels[i].currentSymbol = rand() % 8; // total símbolos
			}

			// freno gradual
			if (now - m_SlotMachine.lastSpinTime > 1500 + (i * 600))
			{
				m_SlotMachine.reels[i].speed -= 0.2f;
				if (m_SlotMachine.reels[i].speed <= 0.5f)
				{
					m_SlotMachine.reels[i].spinning = false;
					m_SlotMachine.reels[i].speed = 0;
				}
			}
		}
		if (m_SlotMachine.reels[i].spinning) allStopped = false;
	}

	if (allStopped)
	{
		m_SlotMachine.spinning = false;
		// Determinar resultado
		int s0 = m_SlotMachine.reels[0].currentSymbol;
		int s1 = m_SlotMachine.reels[1].currentSymbol;
		int s2 = m_SlotMachine.reels[2].currentSymbol;

		if (s0 == s1 && s1 == s2)
			AddEventList("Jackpot!");
		else
			AddEventList("No tuviste suerte esta vez.");
	}
}
