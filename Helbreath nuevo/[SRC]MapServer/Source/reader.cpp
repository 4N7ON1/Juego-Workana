#include "../Header/Game.h"
#include <map>
#include <unordered_set>
#include <string>
#include <tchar.h>  
#include <stdio.h> 
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

extern void PutLogList(char * cMsg);
extern void CharacterLogList(char * cMsg);
extern void ChatLogs(char * cMsg);
extern void Borrado();
extern void ConfigList(char * cMsg);
extern void UpdateConfigList(char* cMsg);
extern char G_cTxt[512];
extern char	G_cData50000[50000];
extern void PutLogHacksFileList(char * cStr);
extern void PutLogOnlinesFileList(char * cStr);
extern void PutLogDrops(char * cStr);
extern void PutGMLogData(char * cStr);
extern void PutEkAndPkLogFileList(char * cStr);
extern void PutLogItemsList(char * cStr);
extern void PutLogTradeFileList(char * cStr);
extern void PutLogCoinsFileList(char * cStr);

void cCoreServer::LoadProcessedPlayers()
{
	std::ifstream file("data\\ProcessedPlayers.txt");
	if (!file.is_open())
	{
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string playerName;
		while (std::getline(ss, playerName, ','))
		{
			playerName.erase(0, playerName.find_first_not_of(' '));
			playerName.erase(playerName.find_last_not_of(' ') + 1);

			m_processedPlayer.push_back(playerName);
		}
	}

	file.close();
}

void cCoreServer::SaveProcessedPlayers()
{
	std::ofstream file("data\\ProcessedPlayers.txt");
	if (!file.is_open())
	{
		PutLogList("Error: No se pudo abrir el archivo ProcessedPlayers.txt para escribir.");
		return;
	}

	for (size_t i = 0; i < m_processedPlayer.size(); ++i)
	{
		file << m_processedPlayer[i];
		if (i < m_processedPlayer.size() - 1)
		{
			file << ",";
		}
	}

	file.close();
}
bool cCoreServer::HasProcessedPlayer(const std::string& playerName)
{
	for (const auto& name : m_processedPlayer)
	{
		if (name == playerName)
		{
			return true;
		}
	}
	return false;
}

void cCoreServer::AddPlayerToProcessedPlayer(const std::string& playerName)
{
	// Verificar si el jugador ya está en la lista
	if (HasProcessedPlayer(playerName))
	{
		return;
	}

	m_processedPlayer.push_back(playerName);

	SaveProcessedPlayers();
}

void cCoreServer::LoadProcessedItems()
{
	std::ifstream file("data\\ProcessedItems.txt");
	if (!file.is_open())
	{
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string playerName;
		while (std::getline(ss, playerName, ','))
		{
			playerName.erase(0, playerName.find_first_not_of(' '));
			playerName.erase(playerName.find_last_not_of(' ') + 1);

			m_processedItems.push_back(playerName);
		}
	}

	file.close();
}

void cCoreServer::SaveProcessedItems()
{
	std::ofstream file("data\\ProcessedItems.txt");
	if (!file.is_open())
	{
		PutLogList("Error: No se pudo abrir el archivo ProcessedItems.txt para escribir.");
		return;
	}

	for (size_t i = 0; i < m_processedItems.size(); ++i)
	{
		file << m_processedItems[i];
		if (i < m_processedItems.size() - 1)
		{
			file << ",";
		}
	}

	file.close();
}
bool cCoreServer::HasProcessedItems(const std::string& playerName)
{
	for (const auto& name : m_processedItems)
	{
		if (name == playerName)
		{
			return true;
		}
	}
	return false;
}

void cCoreServer::AddPlayerToProcessedItems(const std::string& playerName)
{
	// Verificar si el jugador ya está en la lista
	if (HasProcessedItems(playerName))
	{
		return;
	}

	m_processedItems.push_back(playerName);

	SaveProcessedItems();
}


void cCoreServer::LoadProcessedBankItems()
{
	std::ifstream file("data\\ProcessedBankItems.txt");
	if (!file.is_open())
	{
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string playerName;
		while (std::getline(ss, playerName, ','))
		{
			playerName.erase(0, playerName.find_first_not_of(' '));
			playerName.erase(playerName.find_last_not_of(' ') + 1);

			m_ProcessedBankItems.push_back(playerName);
		}
	}

	file.close();
}

void cCoreServer::SaveProcessedBankItems()
{
	std::ofstream file("data\\ProcessedBankItems.txt");
	if (!file.is_open())
	{
		PutLogList("Error: No se pudo abrir el archivo ProcessedBankItems.txt para escribir.");
		return;
	}

	for (size_t i = 0; i < m_ProcessedBankItems.size(); ++i)
	{
		file << m_ProcessedBankItems[i];
		if (i < m_ProcessedBankItems.size() - 1)
		{
			file << ",";
		}
	}

	file.close();
}
bool cCoreServer::HasProcessedBankItems(const std::string& playerName)
{
	for (const auto& name : m_ProcessedBankItems)
	{
		if (name == playerName)
		{
			return true;
		}
	}
	return false;
}

void cCoreServer::AddPlayerToProcessedBankItems(const std::string& playerName)
{
	// Verificar si el jugador ya está en la lista
	if (HasProcessedBankItems(playerName))
	{
		return;
	}

	m_ProcessedBankItems.push_back(playerName);

	SaveProcessedBankItems();
}

void cCoreServer::LoadProcessedMarketItems()
{
	std::ifstream file("data\\ProcessedMarketItems.txt");
	if (!file.is_open())
	{
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string playerName;
		while (std::getline(ss, playerName, ','))
		{
			playerName.erase(0, playerName.find_first_not_of(' '));
			playerName.erase(playerName.find_last_not_of(' ') + 1);

			m_processedMarketItems.push_back(playerName);
		}
	}

	file.close();
}

void cCoreServer::SaveProcessedMarketItems()
{
	std::ofstream file("data\\ProcessedMarketItems.txt");
	if (!file.is_open())
	{
		PutLogList("Error: No se pudo abrir el archivo ProcessedMarketItems.txt para escribir.");
		return;
	}

	for (size_t i = 0; i < m_processedMarketItems.size(); ++i)
	{
		file << m_processedMarketItems[i];
		if (i < m_processedMarketItems.size() - 1)
		{
			file << ",";
		}
	}

	file.close();
}
bool cCoreServer::HasProcessedMarketItems(const std::string& playerName)
{
	for (const auto& name : m_processedMarketItems)
	{
		if (name == playerName)
		{
			return true;
		}
	}
	return false;
}

void cCoreServer::AddPlayerToProcessedMarketItems(const std::string& playerName)
{
	// Verificar si el jugador ya está en la lista
	if (HasProcessedMarketItems(playerName))
	{
		return;
	}

	m_processedMarketItems.push_back(playerName);

	SaveProcessedMarketItems();
}


void cCoreServer::LoadProcessedDropItems()
{
	std::ifstream file("data\\ProcessedDropItems.txt");
	if (!file.is_open())
	{
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string playerName;
		while (std::getline(ss, playerName, ','))
		{
			playerName.erase(0, playerName.find_first_not_of(' '));
			playerName.erase(playerName.find_last_not_of(' ') + 1);

			m_processedDropItems.push_back(playerName);
		}
	}

	file.close();
}

void cCoreServer::SaveProcessedDropItems()
{
	std::ofstream file("data\\ProcessedDropItems.txt");
	if (!file.is_open())
	{
		PutLogList("Error: No se pudo abrir el archivo ProcessedDropItems.txt para escribir.");
		return;
	}

	for (size_t i = 0; i < m_processedDropItems.size(); ++i)
	{
		file << m_processedDropItems[i];
		if (i < m_processedDropItems.size() - 1)
		{
			file << ",";
		}
	}

	file.close();
}
bool cCoreServer::HasProcessedDropItems(const std::string& playerName)
{
	for (const auto& name : m_processedDropItems)
	{
		if (name == playerName)
		{
			return true;
		}
	}
	return false;
}

void cCoreServer::AddPlayerToProcessedDropItems(const std::string& playerName)
{
	// Verificar si el jugador ya está en la lista
	if (HasProcessedDropItems(playerName))
	{
		return;
	}

	m_processedDropItems.push_back(playerName);

	SaveProcessedDropItems();
}


void cCoreServer::LoadPlayersWithStarterItems()
{
	std::ifstream file("data\\ProcessedStarterPack.txt");
	if (!file.is_open())
	{
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string playerName;
		while (std::getline(ss, playerName, ','))
		{
			playerName.erase(0, playerName.find_first_not_of(' '));
			playerName.erase(playerName.find_last_not_of(' ') + 1);

			m_playersWithStarterItems.push_back(playerName);
		}
	}

	file.close();
}

void cCoreServer::SavePlayersWithStarterItems()
{
	std::ofstream file("data\\ProcessedStarterPack.txt");
	if (!file.is_open())
	{
		PutLogList("Error: No se pudo abrir el archivo ProcessedStarterPack.txt para escribir.");
		return;
	}

	for (size_t i = 0; i < m_playersWithStarterItems.size(); ++i)
	{
		file << m_playersWithStarterItems[i];
		if (i < m_playersWithStarterItems.size() - 1)
		{
			file << ",";
		}
	}

	file.close();
}
bool cCoreServer::HasPlayerReceivedStarterItems(const std::string& playerName)
{
	for (const auto& name : m_playersWithStarterItems)
	{
		if (name == playerName)
		{
			return true;
		}
	}
	return false;
}

void cCoreServer::AddPlayerToStarterItemsList(const std::string& playerName)
{
	// Verificar si el jugador ya está en la lista
	if (HasPlayerReceivedStarterItems(playerName))
	{
		return;
	}

	m_playersWithStarterItems.push_back(playerName);

	SavePlayersWithStarterItems();
}