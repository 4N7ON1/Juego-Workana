
#include "../Header/Game.h"
#include <map>
#include <unordered_set>
#include <string>
#include <tchar.h>  
#include <stdio.h> 
#include <chrono>
#include <thread>
#include <algorithm>
#include <set>

extern class cCoreServer * c_map;
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

void cCoreServer::HelBotProcess()
{
	if (m_vHBotClients.size() == 0) return;

	for (int i = 0; i < m_vHBotClients.size(); i++)
	{
		int clientHandle = m_vHBotClients[i];
		auto hbot = m_pClientList[clientHandle];

		if (hbot && hbot->m_bHelBot)
		{
			handleHelBots(clientHandle);
		}
	}
}

bool cCoreServer::isPlayerInSafeZone(CClient* player)
{
	if (!player) return false;

	int playerX = player->m_sX;
	int playerY = player->m_sY;

	if (player->IsInMap("aresden"))
	{
		if (IsInSafeZone(player->client, 1))
			return true;
	}

	if (player->IsInMap("elvine"))
	{
		if (IsInSafeZone(player->client, 2))
			return true;
	}

	return false;
}

int cCoreServer::getNearbyPlayerIndex(int iBot)
{
	CClient* hbot = m_pClientList[iBot];
	if (!hbot) return -1;

	int bestD = 1000000;
	int bestIdx = -1;
	int lowestAssignmentCount = INT_MAX;

	short mapSizeX = m_pMapList[hbot->m_cMapIndex]->m_sSizeX;
	short mapSizeY = m_pMapList[hbot->m_cMapIndex]->m_sSizeY;

	DWORD dwNow = timeGetTime();
	for (auto it = m_mTargetAssignments.begin(); it != m_mTargetAssignments.end();) {
		if (dwNow - it->second.lastUpdateTime > 5000 ||
			it->first >= DEF_MAXCLIENTS ||
			!m_pClientList[it->first] ||
			m_pClientList[it->first]->m_bIsKilled) {
			it = m_mTargetAssignments.erase(it);
		}
		else {
			++it;
		}
	}

	for (int i = 1; i < DEF_MAXCLIENTS; i++) {
		CClient* targetPlayer = m_pClientList[i];
		if (!targetPlayer || targetPlayer->m_iHP <= 0 || targetPlayer->m_bIsKilled) continue;
		if (targetPlayer->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) continue;
		if (targetPlayer->m_cMapIndex != hbot->m_cMapIndex) continue;
		if (string(targetPlayer->m_cLocation) == hbot->m_cLocation) continue;
		if (string(targetPlayer->m_cLocation) == "NONE") continue;
		if (targetPlayer->m_iAdminUserLevel != 0) continue;
		if (targetPlayer->m_bHelBot) continue;

		if (isPlayerInSafeZone(targetPlayer)) continue;

		if (targetPlayer->m_sX < 0 || targetPlayer->m_sX >= mapSizeX ||
			targetPlayer->m_sY < 0 || targetPlayer->m_sY >= mapSizeY) continue;

		int d = max(abs(targetPlayer->m_sX - hbot->m_sX), abs(targetPlayer->m_sY - hbot->m_sY));

		int assignmentCount = 0;
		if (m_mTargetAssignments.find(i) != m_mTargetAssignments.end()) {
			assignmentCount = m_mTargetAssignments[i].botCount;
		}

		if (assignmentCount < lowestAssignmentCount ||
			(assignmentCount == lowestAssignmentCount && d < bestD)) {
			lowestAssignmentCount = assignmentCount;
			bestD = d;
			bestIdx = i;
		}
	}

	return bestIdx;
}

bool cCoreServer::handleWhouseMovement(int iBot, CClient* hbot, DWORD dwNow)
{
	if (!hbot) return false;
	/*if (string(hbot->m_cMapName) != "whouse") return false;*/
	if (!hbot->IsShopMap()) return false;

	// Limpiar direcciones bloqueadas antiguas (más de 3 segundos)
	for (auto it = hbot->m_mBlockedDirections.begin(); it != hbot->m_mBlockedDirections.end();) {
		if (dwNow - it->second > 3000) { // 3 segundos
			it = hbot->m_mBlockedDirections.erase(it);
		}
		else {
			++it;
		}
	}

	struct Coord { short x, y; };
	Coord targets[] = { { 34, 87 }, { 34, 88 }, { 34, 89 }, { 35, 89 }, { 36, 89 }, { 37, 89 } };

	int bestD = 1000000;
	Coord bestTarget = targets[0];
	for (auto& t : targets) {
		int d = max(abs(t.x - hbot->m_sX), abs(t.y - hbot->m_sY));
		if (d < bestD) { bestD = d; bestTarget = t; }
	}

	if (dwNow - hbot->m_dwLastMoveTime >= hbot->m_dwMoveDelay)
	{
		bool moved = false;
		char dir = m_Misc.cGetNextMoveDir(hbot->m_sX, hbot->m_sY, bestTarget.x, bestTarget.y);

		// Función para calcular distancia desde una posición hipotética
		auto calculateDistanceFromPosition = [&](short testX, short testY) -> int {
			return max(abs(bestTarget.x - testX), abs(bestTarget.y - testY));
		};

		// Intentar la dirección principal si no está bloqueada
		if (dir >= 1 && dir <= 8) {
			if (hbot->m_mBlockedDirections.find(dir) == hbot->m_mBlockedDirections.end()) {
				moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, dir, TRUE);

				// Si el movimiento falla, bloquear esta dirección por 3 segundos
				if (!moved) {
					hbot->m_mBlockedDirections[dir] = dwNow;
				}
			}
		}

		int tries = 0;
		while (!moved && tries < 8) {
			// BUSCAR LA MEJOR DIRECCIÓN ALTERNATIVA (no aleatoria)
			char bestDir = -1;
			int bestDistance = INT_MAX;

			// Evaluar las 8 direcciones posibles
			for (char testDir = 1; testDir <= 8; testDir++) {
				// Saltar direcciones bloqueadas
				if (hbot->m_mBlockedDirections.find(testDir) != hbot->m_mBlockedDirections.end()) {
					continue;
				}

				// Calcular la nueva posición si nos movemos en esta dirección
				short newX = hbot->m_sX;
				short newY = hbot->m_sY;

				switch (testDir) {
				case 1: newY--; break;    // Arriba
				case 2: newX++; newY--; break; // Derecha-Arriba
				case 3: newX++; break;    // Derecha
				case 4: newX++; newY++; break; // Derecha-Abajo
				case 5: newY++; break;    // Abajo
				case 6: newX--; newY++; break; // Izquierda-Abajo
				case 7: newX--; break;    // Izquierda
				case 8: newX--; newY--; break; // Izquierda-Arriba
				}

				// Calcular distancia desde la nueva posición al objetivo
				int distance = calculateDistanceFromPosition(newX, newY);

				// Si esta dirección nos acerca más al objetivo, es mejor
				if (distance < bestDistance) {
					bestDistance = distance;
					bestDir = testDir;
				}
			}

			// Si encontramos una dirección buena, intentar mover
			if (bestDir != -1) {
				moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, bestDir, TRUE);

				// Si el movimiento falla, bloquear esta dirección
				if (!moved) {
					hbot->m_mBlockedDirections[bestDir] = dwNow;
				}
			}

			tries++;

			// Si después de evaluar todas las direcciones no podemos movernos,
			// limpiar bloqueos y dar una oportunidad más
			if (!moved && tries >= 4) {
				hbot->m_mBlockedDirections.clear();
			}
		}

		if (moved) {
			SendEventToNearClient_TypeA((short)iBot, DEF_OWNERTYPE_PLAYER,
				CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);
		}

		hbot->m_dwLastMoveTime = dwNow;
	}
	return true;
}

void cCoreServer::updateBotTarget(int iBot, CClient* hbot, DWORD dwNow)
{
	if (!hbot) return;

	if (hbot->m_iTargetPlayer != -1 && hbot->m_iTargetPlayer < DEF_MAXCLIENTS &&
		m_mTargetAssignments.find(hbot->m_iTargetPlayer) != m_mTargetAssignments.end()) {

		m_mTargetAssignments[hbot->m_iTargetPlayer].botCount--;
		m_mTargetAssignments[hbot->m_iTargetPlayer].lastUpdateTime = dwNow;

		if (m_mTargetAssignments[hbot->m_iTargetPlayer].botCount <= 0) {
			m_mTargetAssignments.erase(hbot->m_iTargetPlayer);
		}
	}

	bool needNewTarget = false;

	if (hbot->m_iTargetPlayer == -1) {
		needNewTarget = true;
	}
	else if (hbot->m_iTargetPlayer >= DEF_MAXCLIENTS || !m_pClientList[hbot->m_iTargetPlayer]) {
		needNewTarget = true;
	}
	else {
		CClient* currentTarget = m_pClientList[hbot->m_iTargetPlayer];
		if (!currentTarget || currentTarget->m_bIsKilled ||
			currentTarget->m_cMapIndex != hbot->m_cMapIndex ||
			isPlayerInSafeZone(currentTarget) ||
			currentTarget->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0 ||
			dwNow - hbot->m_dwTargetSearchTime >= hbot->m_dwTargetSearchDelay) {
			needNewTarget = true;
		}
	}

	if (needNewTarget) {
		int newTarget = getNearbyPlayerIndex(iBot);

		if (newTarget != -1 && newTarget < DEF_MAXCLIENTS && m_pClientList[newTarget]) {
			if (m_mTargetAssignments.find(newTarget) == m_mTargetAssignments.end()) {
				m_mTargetAssignments[newTarget] = { newTarget, 1, dwNow };
			}
			else {
				m_mTargetAssignments[newTarget].botCount++;
				m_mTargetAssignments[newTarget].lastUpdateTime = dwNow;
			}
		}

		hbot->m_iTargetPlayer = newTarget;
		hbot->m_dwTargetSearchTime = dwNow;
	}
	else {
		if (hbot->m_iTargetPlayer != -1 &&
			m_mTargetAssignments.find(hbot->m_iTargetPlayer) != m_mTargetAssignments.end()) {
			m_mTargetAssignments[hbot->m_iTargetPlayer].lastUpdateTime = dwNow;
		}
	}
}
/*

void cCoreServer::handleIdleMovement(int iBot, CClient* hbot, DWORD dwNow)
{
	if (!hbot) return;

	if (dwNow - hbot->m_dwLastMoveTime >= hbot->m_dwMoveDelay) {
		if (hbot->fakemovedir == -1) hbot->fakemovedir = DropRollDice(1, 8);

		bool moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, (char)hbot->fakemovedir, TRUE);
		if (!moved) hbot->fakemovedir = DropRollDice(1, 8);

		if (moved)
			SendEventToNearClient_TypeA((short)iBot, DEF_OWNERTYPE_PLAYER,
			CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);

		hbot->m_dwLastMoveTime = dwNow;
	}
}
*/

void cCoreServer::handleIdleMovement(int iBot, CClient* hbot, DWORD dwNow)
{
    if (!hbot) return;

    // Limpiar direcciones bloqueadas antiguas (más de 3 segundos)
    for (auto it = hbot->m_mBlockedDirections.begin(); it != hbot->m_mBlockedDirections.end();) {
        if (dwNow - it->second > 3000) { // 3 segundos
            it = hbot->m_mBlockedDirections.erase(it);
        }
        else {
            ++it;
        }
    }

    if (dwNow - hbot->m_dwLastMoveTime >= hbot->m_dwMoveDelay) {
        bool moved = false;
        
        // Si no tiene dirección asignada o la dirección está bloqueada, elegir una nueva
        if (hbot->fakemovedir == -1 || 
            hbot->m_mBlockedDirections.find((char)hbot->fakemovedir) != hbot->m_mBlockedDirections.end()) {
            
            // Buscar una dirección no bloqueada
            int attempts = 0;
            do {
                hbot->fakemovedir = DropRollDice(1, 8);
                attempts++;
                
                // Si después de 8 intentos no encontramos dirección no bloqueada,
                // limpiar el bloqueo y usar cualquier dirección
                if (attempts > 8) {
                    hbot->m_mBlockedDirections.clear();
                    break;
                }
            } while (hbot->m_mBlockedDirections.find((char)hbot->fakemovedir) != hbot->m_mBlockedDirections.end());
        }

        // Intentar mover en la dirección seleccionada
        moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, (char)hbot->fakemovedir, TRUE);
        
        // Si el movimiento falla, bloquear esta dirección y elegir una nueva
        if (!moved) {
            hbot->m_mBlockedDirections[(char)hbot->fakemovedir] = dwNow;
            hbot->fakemovedir = -1; // Forzar nueva selección en el próximo intento
        }

        if (moved) {
            SendEventToNearClient_TypeA((short)iBot, DEF_OWNERTYPE_PLAYER,
                CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);
        }

        hbot->m_dwLastMoveTime = dwNow;
    }
}/*
bool cCoreServer::executeBotMovement(int iBot, CClient* hbot, short targetX, short targetY, DWORD dwNow)
{
	if (!hbot) return false;

	char dir = m_Misc.cGetNextMoveDir(hbot->m_sX, hbot->m_sY, targetX, targetY);
	bool moved = false;

	if (dir >= 1 && dir <= 8)
		moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, dir, TRUE);

	int tries = 0;
	while (!moved && tries < 5) {
		char randomDir = DropRollDice(1, 8);
		moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, randomDir, TRUE);
		tries++;
	}

	if (moved)
		SendEventToNearClient_TypeA((short)iBot, DEF_OWNERTYPE_PLAYER,
		CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);

	hbot->m_dwLastMoveTime = dwNow;
	return moved;
}*/

bool cCoreServer::executeBotMovement(int iBot, CClient* hbot, short targetX, short targetY, DWORD dwNow)
{
    if (!hbot) return false;

    // Limpiar direcciones bloqueadas antiguas (más de 3 segundos)
    for (auto it = hbot->m_mBlockedDirections.begin(); it != hbot->m_mBlockedDirections.end();) {
        if (dwNow - it->second > 3000) { // 3 segundos
            it = hbot->m_mBlockedDirections.erase(it);
        }
        else {
            ++it;
        }
    }

    bool moved = false;
    char dir = m_Misc.cGetNextMoveDir(hbot->m_sX, hbot->m_sY, targetX, targetY);

    // Función para calcular distancia desde una posición hipotética
    auto calculateDistanceFromPosition = [&](short testX, short testY) -> int {
        return max(abs(targetX - testX), abs(targetY - testY));
    };

    // Intentar la dirección principal si no está bloqueada
    if (dir >= 1 && dir <= 8) {
        if (hbot->m_mBlockedDirections.find(dir) == hbot->m_mBlockedDirections.end()) {
            moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, dir, TRUE);

            // Si el movimiento falla, bloquear esta dirección por 3 segundos
            if (!moved) {
                hbot->m_mBlockedDirections[dir] = dwNow;
            }
        }
    }

    int tries = 0;
    while (!moved && tries < 8) {
        // BUSCAR LA MEJOR DIRECCIÓN ALTERNATIVA (no aleatoria)
        char bestDir = -1;
        int bestDistance = INT_MAX;
        
        // Evaluar las 8 direcciones posibles
        for (char testDir = 1; testDir <= 8; testDir++) {
            // Saltar direcciones bloqueadas
            if (hbot->m_mBlockedDirections.find(testDir) != hbot->m_mBlockedDirections.end()) {
                continue;
            }
            
            // Calcular la nueva posición si nos movemos en esta dirección
            short newX = hbot->m_sX;
            short newY = hbot->m_sY;
            
            switch (testDir) {
                case 1: newY--; break;    // Arriba
                case 2: newX++; newY--; break; // Derecha-Arriba
                case 3: newX++; break;    // Derecha
                case 4: newX++; newY++; break; // Derecha-Abajo
                case 5: newY++; break;    // Abajo
                case 6: newX--; newY++; break; // Izquierda-Abajo
                case 7: newX--; break;    // Izquierda
                case 8: newX--; newY--; break; // Izquierda-Arriba
            }
            
            // Calcular distancia desde la nueva posición al objetivo
            int distance = calculateDistanceFromPosition(newX, newY);
            
            // Si esta dirección nos acerca más al objetivo, es mejor
            if (distance < bestDistance) {
                bestDistance = distance;
                bestDir = testDir;
            }
        }
        
        // Si encontramos una dirección buena, intentar mover
        if (bestDir != -1) {
            moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, bestDir, TRUE);
            
            // Si el movimiento falla, bloquear esta dirección
            if (!moved) {
                hbot->m_mBlockedDirections[bestDir] = dwNow;
            }
        }
        
        tries++;
        
        // Si después de evaluar todas las direcciones no podemos movernos,
        // limpiar bloqueos y dar una oportunidad más
        if (!moved && tries >= 4) {
            hbot->m_mBlockedDirections.clear();
        }
    }

    if (moved) {
        SendEventToNearClient_TypeA((short)iBot, DEF_OWNERTYPE_PLAYER,
            CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);
    }

    hbot->m_dwLastMoveTime = dwNow;
    return moved;
}
bool cCoreServer::executeBotAttack(int iBot, CClient* hbot, CClient* targetPlayer, int targetIdx, DWORD dwNow)
{
	if (!hbot || !targetPlayer || targetIdx >= DEF_MAXCLIENTS || !m_pClientList[targetIdx])
		return false;

	if (isPlayerInSafeZone(hbot)) {
		return false;
	}

	if (!targetPlayer || targetPlayer->m_bIsKilled) {
		return false;
	}

	if (targetPlayer->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != 0) {
		return false;
	}

	short wType = 23;
	char dir = m_Misc.cGetNextMoveDir(hbot->m_sX, hbot->m_sY,
		targetPlayer->m_sX, targetPlayer->m_sY);

	int iRet = iClientMotion_Attack_Handler(iBot, hbot->m_sX, hbot->m_sY,
		targetPlayer->m_sX, targetPlayer->m_sY, wType, dir, targetIdx, TRUE, FALSE);

	if (iRet == 1)
		SendEventToNearClient_TypeA((short)iBot, DEF_OWNERTYPE_PLAYER,
		CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTATTACK,
		hbot->m_sX, hbot->m_sY, wType);

	hbot->m_dwLastAttackTime = dwNow;
	return iRet == 1;
}
int cCoreServer::calculateDistance(CClient* source, CClient* target)
{
	int dx = abs(target->m_sX - source->m_sX);
	int dy = abs(target->m_sY - source->m_sY);
	return max(dx, dy);
}

void cCoreServer::handleBotStateIdle(CClient* hbot, CClient* targetPlayer, DWORD dwNow, int distance)
{
	if (!hbot) return;
	const int kMeleeRange = 3;

	if (distance > kMeleeRange) {
		hbot->botState = BOT_MOVING;
		hbot->m_dwLastMoveTime = dwNow;
	}
	else {
		hbot->botState = BOT_ATTACKING;
		hbot->m_dwLastAttackTime = dwNow;
	}
}

void cCoreServer::handleBotStateMoving(int iBot, CClient* hbot, CClient* targetPlayer, DWORD dwNow, int distance)
{
	if (!hbot) return;
	const int kMeleeRange = 3;

	if (distance <= kMeleeRange) {
		hbot->botState = BOT_ATTACKING;
		hbot->m_dwLastAttackTime = dwNow;
	}
	else if (dwNow - hbot->m_dwLastMoveTime >= hbot->m_dwMoveDelay) {
		executeBotMovement(iBot, hbot, targetPlayer->m_sX, targetPlayer->m_sY, dwNow);
	}
}

void cCoreServer::handleBotStateAttacking(int iBot, CClient* hbot, CClient* targetPlayer, int targetIdx, DWORD dwNow, int distance)
{
	if (!hbot) return;
	const int kMeleeRange = 3;

	if (distance <= kMeleeRange &&
		dwNow - hbot->m_dwLastAttackTime >= hbot->m_dwAttackDelay) {

		if (!isPlayerInSafeZone(hbot)) {
			executeBotAttack(iBot, hbot, targetPlayer, targetIdx, dwNow);
		}
		else {
			hbot->botState = BOT_MOVING;
			hbot->m_dwLastMoveTime = dwNow;
		}
		return;
	}

	if (distance > kMeleeRange) {
		hbot->botState = BOT_MOVING;
		hbot->m_dwLastMoveTime = dwNow;
	}
}

void cCoreServer::handleBotPotions(int iBot, DWORD dwNow)
{
	CClient* hbot = m_pClientList[iBot];
	if (!hbot) return;

	switch (m_sBotLevel)
	{
	case 1:
	{
		if (dwNow - hbot->m_dwLastHealTime >= 1500) {
			int maxHP = iGetMaxHP(iBot);
			hbot->m_iMP = hbot->GetMaxMP();
			hbot->m_iSP = hbot->GetMaxSP();
			hbot->m_iHungerStatus = 100;
			if (hbot->m_iHP < maxHP && hbot->m_iPots != 0) {
				int upHp = DropRollDice(100, 150);
				int nextHP = hbot->m_iHP + upHp;
				if (nextHP > maxHP)	nextHP = maxHP;

				hbot->m_iPots--;
				hbot->m_iHP = nextHP;
			}
			hbot->m_dwLastHealTime = dwNow;
		}
		break;
	}
	case 2:
	{
		if (dwNow - hbot->m_dwLastHealTime >= 1250) {
			int maxHP = iGetMaxHP(iBot);
			hbot->m_iMP = hbot->GetMaxMP();
			hbot->m_iSP = hbot->GetMaxSP();
			hbot->m_iHungerStatus = 100;
			if (hbot->m_iHP < maxHP && hbot->m_iPots != 0) {
				int upHp = DropRollDice(150, 200);
				int nextHP = hbot->m_iHP + upHp;
				if (nextHP > maxHP)	nextHP = maxHP;

				hbot->m_iPots--;
				hbot->m_iHP = nextHP;
			}
			hbot->m_dwLastHealTime = dwNow;
		}
		break;
	}
	case 3:
	{
		if (dwNow - hbot->m_dwLastHealTime >= 1000) {
			int maxHP = iGetMaxHP(iBot);
			hbot->m_iMP = hbot->GetMaxMP();
			hbot->m_iSP = hbot->GetMaxSP();
			hbot->m_iHungerStatus = 100;
			if (hbot->m_iHP < maxHP && hbot->m_iPots != 0) {
				int upHp = DropRollDice(200, 250);
				int nextHP = hbot->m_iHP + upHp;
				if (nextHP > maxHP)	nextHP = maxHP;

				hbot->m_iPots--;
				hbot->m_iHP = nextHP;
			}
			hbot->m_dwLastHealTime = dwNow;
		}
		break;
	}

	default:
		break;
	}
}

bool cCoreServer::bHandleBotWhenDeath(int iBot, CClient* hbot)
{
	if (hbot->m_bIsKilled) {
		if (hbot->m_iTargetPlayer != -1 && hbot->m_iTargetPlayer < DEF_MAXCLIENTS &&
			m_mTargetAssignments.find(hbot->m_iTargetPlayer) != m_mTargetAssignments.end()) {

			m_mTargetAssignments[hbot->m_iTargetPlayer].botCount--;
			m_mTargetAssignments[hbot->m_iTargetPlayer].lastUpdateTime = timeGetTime();

			if (m_mTargetAssignments[hbot->m_iTargetPlayer].botCount <= 0) {
				m_mTargetAssignments.erase(hbot->m_iTargetPlayer);
			}
		}

		if (hbot->IsInMap("dm")) dmrevive(iBot);
		else RequestRestartHandler(iBot);
		hbot->m_bReachedGuardPoint = false;
		hbot->m_iAssignedGuardPoint = -1;
		hbot->botState = BOT_IDLE;
		hbot->m_iTargetPlayer = -1;

		int swordIndex = getSwordIndex(iBot);
		if (swordIndex != -1)
		{
			if (bEquipItemHandler(iBot, swordIndex, FALSE) == FALSE) {
				if (hbot->m_pItemList[swordIndex] != NULL)
					hbot->m_bIsItemEquipped[swordIndex] = FALSE;
			}
			else
			{
				hbot->weapontype = 1;
			}
		}

		return true;
	}

	return false;
}

bool cCoreServer::bHandleBotTarget(int iBot, CClient* hbot, int targetIdx, CClient* targetPlayer, DWORD dwNow)
{
	updateBotTarget(iBot, hbot, dwNow);

	if (targetIdx == -1 || targetIdx >= DEF_MAXCLIENTS) {
		hbot->botState = BOT_IDLE;
		handleIdleMovement(iBot, hbot, dwNow);
		return false;
	}

	if (!targetPlayer || targetPlayer->m_bIsKilled) {
		if (m_mTargetAssignments.find(targetIdx) != m_mTargetAssignments.end()) {
			m_mTargetAssignments[targetIdx].botCount--;
			m_mTargetAssignments[targetIdx].lastUpdateTime = dwNow;

			if (m_mTargetAssignments[targetIdx].botCount <= 0) {
				m_mTargetAssignments.erase(targetIdx);
			}
		}

		hbot->m_iTargetPlayer = -1;
		hbot->botState = BOT_IDLE;
		return false;
	}

	if (isPlayerInSafeZone(targetPlayer)) {
		if (m_mTargetAssignments.find(targetIdx) != m_mTargetAssignments.end()) {
			m_mTargetAssignments[targetIdx].botCount--;
			m_mTargetAssignments[targetIdx].lastUpdateTime = dwNow;

			if (m_mTargetAssignments[targetIdx].botCount <= 0) {
				m_mTargetAssignments.erase(targetIdx);
			}
		}

		hbot->m_iTargetPlayer = -1;
		hbot->botState = BOT_IDLE;
		handleIdleMovement(iBot, hbot, dwNow);
		return false;
	}

	return true;
}


int cCoreServer::getWandIndex(int iBot)
{
	CClient* hbot = m_pClientList[iBot];
	if (!hbot) return -1;

	for (int i = 0; i < DEF_MAXITEMS; i++) {
		auto it = hbot->m_pItemList[i];
		if (!it) continue;

		if (string(it->m_cName) == "MagicWand(MS0)")
		{
			return i;
		}
	}

	return -1;
}

int cCoreServer::getSwordIndex(int iBot)
{
	CClient* hbot = m_pClientList[iBot];
	if (!hbot) return -1;

	for (int i = 0; i < DEF_MAXITEMS; i++) {
		auto it = hbot->m_pItemList[i];
		if (!it) continue;

		if (string(it->m_cName) == "BlazingSword" || string(it->m_cName) == "DarkDevastator")
		{
			return i;
		}
	}

	return -1;
}

void cCoreServer::handleCancelation(int iBot)
{	
	auto hbot = m_pClientList[iBot];
	if (!hbot) return;

	// Removes Invisibility Flag 0x0010	
	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
	SetInvisibilityFlag(iBot, DEF_OWNERTYPE_PLAYER, FALSE);
	SendNotifyMsg(NULL, iBot, CLIENT_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_INVISIBILITY, NULL, NULL, NULL);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = NULL;	

	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_CONFUSE);
	SetIllusionFlag(iBot, DEF_OWNERTYPE_PLAYER, FALSE);
	SendNotifyMsg(NULL, iBot, CLIENT_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE, 3, NULL, NULL);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] = NULL;
	
	// Removes Defense Shield Flag 0x02000000
	// Removes Great Defense Shield Flag 0x02000000	
	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_PROTECT);
	SetDefenseShieldFlag(iBot, DEF_OWNERTYPE_PLAYER, FALSE);
	SendNotifyMsg(NULL, iBot, CLIENT_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_PROTECT, 3, NULL, NULL);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = NULL;
	
	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_ICE);
	SetIceFlag(iBot, DEF_OWNERTYPE_PLAYER, FALSE);
	SendNotifyMsg(NULL, iBot, CLIENT_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_ICE, NULL, NULL, NULL);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] = NULL;
	
	// Removes Absolute Magic Protection Flag 0x04000000	
	// Removes Protection From Magic Flag 0x04000000		
	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_PROTECT);
	SetMagicProtectionFlag(iBot, DEF_OWNERTYPE_PLAYER, FALSE);
	if (hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2)
	SendNotifyMsg(NULL, iBot, CLIENT_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_PROTECT, 2, NULL, NULL);
	else SendNotifyMsg(NULL, iBot, CLIENT_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_PROTECT, 5, NULL, NULL);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = NULL;
	
	// Removes Protection From Arrow Flag 0x08000000
	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_PROTECT);
	SetProtectionFromArrowFlag(iBot, DEF_OWNERTYPE_PLAYER, FALSE);
	SendNotifyMsg(NULL, iBot, CLIENT_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_PROTECT, 1, NULL, NULL);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] = NULL;
	
	// Removes Illusion Movement Flag 0x00200000	
	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_CONFUSE);
	SetIllusionMovementFlag(iBot, DEF_OWNERTYPE_PLAYER, FALSE);
	SendNotifyMsg(NULL, iBot, CLIENT_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_CONFUSE, 4, NULL, NULL);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_CONFUSE] = NULL;
	
	// Removes Berserk Flag 0x0020	
	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_BERSERK);
	SetBerserkFlag(iBot, DEF_OWNERTYPE_PLAYER, FALSE);
	SendNotifyMsg(NULL, iBot, CLIENT_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_BERSERK, NULL, NULL, NULL);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = NULL;
	
	// Removes Fury Of War Flag 0x100000	
	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_BERSERK);
	SetFuryWarFlag(iBot, DEF_OWNERTYPE_PLAYER, FALSE);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = NULL;
	
	//Remove Inhibition	
	bRemoveFromDelayEventList(iBot, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INHIBITION);
	hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_INHIBITION] = NULL;
	hbot->m_bInhibition = FALSE;	

	SendEventToNearClient_TypeA(iBot, DEF_OWNERTYPE_PLAYER, CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
}

bool cCoreServer::handleMagicStatus(int iBot, DWORD dwNow)
{
	CClient* hbot = m_pClientList[iBot];
	if (!hbot) return true;

	static std::unordered_map<int, int> botCastingSpell;
	static std::unordered_map<int, bool> botSpellFailed; 

	if (hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) {
		if (hbot->weapontype == 1)
		{
			int wandIndex = getWandIndex(iBot);
			if (wandIndex != -1)
			{
				if (bEquipItemHandler(iBot, wandIndex, FALSE) == FALSE) {
					if (hbot->m_pItemList[wandIndex] != NULL)
						hbot->m_bIsItemEquipped[wandIndex] = FALSE;
				}
				else
				{
					hbot->weapontype = 2;
				}
			}
		}

		if (dwNow - hbot->m_dwLastDebuffTime >= 1050) {
			int spellId = 30; 

			if (hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2 ||
				hbot->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5)
			{
				spellId = 76; 
			}

			if (!hbot->m_bAltDebuffStep) {
				ClientMotionHandlerParams(iBot, DEF_OBJECTMAGIC,
					hbot->m_sX, hbot->m_sY, hbot->m_cDir,
					spellId, 0, 0, 0, dwNow);

				botCastingSpell[iBot] = spellId;
				botSpellFailed[iBot] = false; 
				hbot->m_bAltDebuffStep = true;
			}
			else {
				if (botCastingSpell[iBot] != spellId) {
					hbot->m_bAltDebuffStep = false;
					botCastingSpell.erase(iBot);
					botSpellFailed.erase(iBot); 
					return true; 
				}

				if (spellId == 30 && !botSpellFailed[iBot]) {
					int dice = DropRollDice(1, 2);
					if (dice == 1) {
						hbot->m_bAltDebuffStep = false;
						botSpellFailed[iBot] = true;
						return true; 
					}
				}

				PlayerMagicHandler(iBot, hbot->m_sX, hbot->m_sY, spellId, 0, 0);

				if (spellId == 76) {
					handleCancelation(iBot);
				}

				hbot->m_bAltDebuffStep = false;
				botCastingSpell.erase(iBot);
				botSpellFailed.erase(iBot); 
			}

			hbot->m_dwLastDebuffTime = dwNow;
			return true;
		}
	}
	else
	{
		if (hbot->weapontype == 2)
		{
			int swordIndex = getSwordIndex(iBot);
			if (hbot->m_bIsItemEquipped[swordIndex] == FALSE)
			{
				if (swordIndex != -1)
				{
					if (bEquipItemHandler(iBot, swordIndex, FALSE) == FALSE) {
						if (hbot->m_pItemList[swordIndex] != NULL)
							hbot->m_bIsItemEquipped[swordIndex] = FALSE;
					}
					else
					{
						hbot->weapontype = 1;
					}
				}
			}
		}
	}

	return false;
}

/*

char _tmp_cEmptyPosX2[] = { 0, 1, 1, 0, -1, -1, -1, 0, 1, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2 };
char _tmp_cEmptyPosY2[] = { 0, 0, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2 };

bool cCoreServer::bCheckPosition(int iBot, CClient* hbot)
{
	register int i;
	short sX, sY;

	for (i = 0; i < DEF_MAXMAPS; i++)
		if ((m_pMapList[hbot->m_cMapIndex]->bGetMoveable(*pX + _tmp_cEmptyPosX2[i], *pY + _tmp_cEmptyPosY2[i]) == TRUE) &&
			(m_pMapList[hbot->m_cMapIndex]->bGetIsTeleport(*pX + _tmp_cEmptyPosX2[i], *pY + _tmp_cEmptyPosY2[i]) == FALSE)) {
			sX = *pX + _tmp_cEmptyPosX2[i];
			sY = *pY + _tmp_cEmptyPosY2[i];
			*pX = sX;
			*pY = sY;
			return TRUE;
		}
		}*/
bool cCoreServer::handleAbymapMovement(int iBot, CClient* hbot, DWORD dwNow)
{
	if (!hbot) return false;
	if (string(hbot->m_cMapName) != "abymap") return false;
	if (hbot->m_bReachedGuardPoint) return false;

	struct Coord { short x, y; };
	Coord guardPoints[] = {
		{ 84, 82 }, { 85, 81 }, { 86, 80 },
		{ 85, 98 }, { 86, 99 }, { 87, 100 },
		{ 112, 100 }, { 113, 99 }, { 114, 98 },
		{ 111, 82 }, { 112, 83 }, { 113, 84 }
	};

	int numPoints = sizeof(guardPoints) / sizeof(guardPoints[0]);

	if (hbot->m_iAssignedGuardPoint == -1) {
		hbot->m_iAssignedGuardPoint = iBot % numPoints;
	}

	Coord assignedPoint = guardPoints[hbot->m_iAssignedGuardPoint];

	int distanceToPoint = max(abs(assignedPoint.x - hbot->m_sX), abs(assignedPoint.y - hbot->m_sY));
	if (distanceToPoint <= 1) {
		hbot->m_bReachedGuardPoint = true;
		return false;
	}

	// Limpiar direcciones bloqueadas antiguas (más de 3 segundos)
	for (auto it = hbot->m_mBlockedDirections.begin(); it != hbot->m_mBlockedDirections.end();) {
		if (dwNow - it->second > 3000) { // 3 segundos
			it = hbot->m_mBlockedDirections.erase(it);
		}
		else {
			++it;
		}
	}

	if (dwNow - hbot->m_dwLastMoveTime >= hbot->m_dwMoveDelay) {
		bool moved = false;
		char dir = m_Misc.cGetNextMoveDir(hbot->m_sX, hbot->m_sY, assignedPoint.x, assignedPoint.y);

		// Función para calcular distancia desde una posición hipotética
		auto calculateDistanceFromPosition = [&](short testX, short testY) -> int {
			return max(abs(assignedPoint.x - testX), abs(assignedPoint.y - testY));
		};

		// Intentar la dirección principal si no está bloqueada
		if (dir >= 1 && dir <= 8) {
			if (hbot->m_mBlockedDirections.find(dir) == hbot->m_mBlockedDirections.end()) {
				moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, dir, TRUE);

				// Si el movimiento falla, bloquear esta dirección por 3 segundos
				if (!moved) {
					hbot->m_mBlockedDirections[dir] = dwNow;
				}
			}
		}

		int tries = 0;
		while (!moved && tries < 8) {
			// BUSCAR LA MEJOR DIRECCIÓN ALTERNATIVA (no aleatoria)
			char bestDir = -1;
			int bestDistance = INT_MAX;

			// Evaluar las 8 direcciones posibles
			for (char testDir = 1; testDir <= 8; testDir++) {
				// Saltar direcciones bloqueadas
				if (hbot->m_mBlockedDirections.find(testDir) != hbot->m_mBlockedDirections.end()) {
					continue;
				}

				// Calcular la nueva posición si nos movemos en esta dirección
				short newX = hbot->m_sX;
				short newY = hbot->m_sY;

				switch (testDir) {
				case 1: newY--; break;    // Arriba
				case 2: newX++; newY--; break; // Derecha-Arriba
				case 3: newX++; break;    // Derecha
				case 4: newX++; newY++; break; // Derecha-Abajo
				case 5: newY++; break;    // Abajo
				case 6: newX--; newY++; break; // Izquierda-Abajo
				case 7: newX--; break;    // Izquierda
				case 8: newX--; newY--; break; // Izquierda-Arriba
				}

				// Calcular distancia desde la nueva posición al objetivo
				int distance = calculateDistanceFromPosition(newX, newY);

				// Si esta dirección nos acerca más al objetivo, es mejor
				if (distance < bestDistance) {
					bestDistance = distance;
					bestDir = testDir;
				}
			}

			// Si encontramos una dirección buena, intentar mover
			if (bestDir != -1) {
				moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, bestDir, TRUE);

				// Si el movimiento falla, bloquear esta dirección
				if (!moved) {
					hbot->m_mBlockedDirections[bestDir] = dwNow;
				}
			}

			tries++;

			// Si después de evaluar todas las direcciones no podemos movernos,
			// limpiar bloqueos y dar una oportunidad más
			if (!moved && tries >= 4) {
				hbot->m_mBlockedDirections.clear();
			}
		}

		if (moved) {
			SendEventToNearClient_TypeA((short)iBot, DEF_OWNERTYPE_PLAYER,
				CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);
		}

		hbot->m_dwLastMoveTime = dwNow;
	}

	return true;
}

void cCoreServer::handleHelBots(int iBot)
{
	CClient* hbot = m_pClientList[iBot];
	if (!hbot) return;
	if (!m_bBotProces) return;

	if (bHandleBotWhenDeath(iBot, hbot)) return;

	DWORD dwNow = timeGetTime();

	handleBotPotions(iBot, dwNow);

	if (!hbot->IsShopMap()) handleMagicStatus(iBot, dwNow);

	if (string(hbot->m_cMapName) == "abymap" && !hbot->m_bReachedGuardPoint) {
		if (handleAbymapMovement(iBot, hbot, dwNow)) {
			return; 
		}
	}

	if (handleWhouseMovement(iBot, hbot, dwNow)) return;
	if (handleBotCallMovement(iBot, hbot, dwNow)) return;


	int targetIdx = hbot->m_iTargetPlayer;
	CClient* targetPlayer = m_pClientList[targetIdx];
	if (!bHandleBotTarget(iBot, hbot, targetIdx, targetPlayer, dwNow)) return;

	if (!targetPlayer) return;

	int distance = calculateDistance(hbot, targetPlayer);
	const int kMeleeRange = 3;

	switch (hbot->botState) {
	case BOT_IDLE:
		handleBotStateIdle(hbot, targetPlayer, dwNow, distance);
		break;

	case BOT_MOVING:
		handleBotStateMoving(iBot, hbot, targetPlayer, dwNow, distance);
		break;

	case BOT_ATTACKING:
		handleBotStateAttacking(iBot, hbot, targetPlayer, targetIdx, dwNow, distance);
		break;
	}
}

void cCoreServer::eliminarCliente(int iClientH) {
	m_vHBotClients.erase(
		std::remove(m_vHBotClients.begin(), m_vHBotClients.end(), iClientH),
		m_vHBotClients.end()
		);
}

// Mensajes para cuando el bot mata a alguien
std::vector<std::string> botKillMessages = {
	"sorry, estoy programado para matar petes",
	"un cono me daba mas pelea bro",
	"che, me duele el pito.",
	"busca por Ebay un par de manos",
	"disculpen aca es barracks? creo que mate un Dummy",
	"mandale un Saludito a San Pedro Bro",
	"te tepie de un golpe a Shop",
	"Llegaste temprano a la repartija de Chori",
	"Anda a llorarle a lucas ahora, maraca",
	"GM ¿podes subirle el level a estos pj?",
	"Mas facil que ella",
	"che, la proxima me haces chugear 1 candy?",
	"/CLEARPETES",
	"este va a cagar pasto por 3 dias",
	"lucas subile el nivel a estos bots",
	"che esta facil esto",
	"Traeme a Ella de postre Papiloma",
	"X ALA",
	"Sv win",
	"GM, si me pones contra otros bots me divierto mas",
	"y hoy te vas, te vas, te vas, te vas",
	"no se vayan, ya los sigo pijeando",
	"no me digas que ahora se van a quedar en shop",
	"Sigan viendo",
	"Mira al jardinero como corta el pasto con los dientes",
	"se buscan players con manos",
	"que raro que no pidio back el pete",
	"Ni con merien shield te salvabas cono",
	"se te frozearon los dedos no wachin?",
	"Ni donando se salva el petaso JAJA"
};

// Mensajes para cuando matan al bot
std::vector<std::string> botDieMessages = {
	"el diavlo luca, y ete mmg?",
	"solo donando podes matarme",
	"callate fraca, tenes que venir con 4 para matarme",
	"siempre pidiendo backup cagon",
	"te deje matarme porque me das lastima",
	"alla la estan matando",
	"gm balancea esto hermano, por dios!",
	"bueee, despues dice que no tiene nada",
	"a buenoo, tanta suerte va a tener el pete",
	"GM afloja el porno, alto lag chabon",
	"la proxima no vas a tener tanta suerte loro",
	"q pasa gato? te la aguantas PVP?",
	"te espero en medio gil",
	"pidio bu jajajajajaja",
	"Stream off",
	"y si le bajas un poco al speed?",
	"pete buyer, sin ítems no existis",
	"GM, revisale el pj a ese editado",
	"que lag culeado, no se puede jugar",
	"waa culea tantos items vas a comprar?",
	"naah, no se puede jugar, vengo en un rato",
	"3v1?, que bien eh",
	"primera vez que me mata el pete este",
	"sos horrible chabon, tuviste suerte",
	"che que onda con este, esta tuneado?",
	"cuantas casas le compraste a lucas?",
	"asi? veni medio cagon",
	"imposible, ta re editado",
	"solo con ayuda me mata el pete",
	"yo que vos me escondo en shop ahora"
};

std::string cCoreServer::getRandomMessage(const std::vector<std::string>& messages)
{
	int idx = DropRollDice(1, 30) - 1;
	return messages[idx];
}

void cCoreServer::sendBotKillMessages(int iBot)
{
	auto p = m_pClientList[iBot];
	if (!p) return;

	string msg = getRandomMessage(botKillMessages);

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi || !pi->m_bIsInitComplete || pi->m_bLimbo) continue;

		ShowBotMsg(iBot, i, (char*)msg.c_str());
	}
}

void cCoreServer::sendBotDeathMessages(int iBot)
{
	auto p = m_pClientList[iBot];
	if (!p) return;

	string msg = getRandomMessage(botDieMessages);

	for (int i = 1; i < DEF_MAXCLIENTS; i++)
	{
		auto pi = m_pClientList[i];
		if (!pi || !pi->m_bIsInitComplete || pi->m_bLimbo) continue;

		ShowBotMsg(iBot, i, (char*)msg.c_str());
	}
}

void cCoreServer::ShowBotMsg(int iBot, int iClientH, char* pMsg)
{
	auto p = m_pClientList[iBot];
	if (!p) return;

	if (!m_pClientList[iClientH]) return;

#ifdef DEF_DEBUG
	try {
#endif
		char * cp, cTemp[256];
		DWORD * dwp, dwMsgSize;
		WORD * wp;
		short * sp;

		ZeroMemory(cTemp, sizeof(cTemp));

		dwp = (DWORD *)cTemp;
		*dwp = CLIENT_REQUEST_COMMAND_CHATMSG;

		wp = (WORD *)(cTemp + DEF_INDEX2_MSGTYPE);
		*wp = NULL;

		cp = (char *)(cTemp + DEF_INDEX2_MSGTYPE + 2);
		sp = (short *)cp;
		*sp = NULL;
		cp += 2;

		sp = (short *)cp;
		*sp = NULL;
		cp += 2;

		memcpy_secure(cp, p->m_cCharName, 10);
		cp += 10;

		*cp = 2;
		cp++;

		dwMsgSize = strlen(pMsg);
		if (dwMsgSize > 100) dwMsgSize = 100;
		memcpy_secure(cp, pMsg, dwMsgSize);
		cp += dwMsgSize;
		int iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cTemp, dwMsgSize + 22);
		switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			DeleteClient(iClientH, TRUE, TRUE);
			break;
		}
#ifdef DEF_DEBUG
	}
	catch (...) {
		ErrorList("Crash Evitado en: ShowClientMsg");
	}
#endif
}

void cCoreServer::handleHelBotReward(int client, char * botname)
{
	auto p = m_pClientList[client];
	if (!p) return;

	int dado = DropRollDice(1, 9);

	switch (dado)
	{
	case 1:
	{
		int rew = DropRollDice(200, 400);
		p->m_iIceSegments += rew;
		SendCommand(client, "/icesegments", p->m_iIceSegments);
		wsprintf(G_cTxt, "Recibiste %d Ice Segments por matar a %s", rew, botname);
		ShowClientMsg(client, G_cTxt);
		break;
	}
	case 2:
	{
		int rew = DropRollDice(200, 400);
		p->m_iFireSegments += rew;
		SendCommand(client, "/firesegments", p->m_iFireSegments);
		wsprintf(G_cTxt, "Recibiste %d Fire Segments por matar a %s", rew, botname);
		ShowClientMsg(client, G_cTxt);
		break;
	}
	case 3:
	{
		int rew = DropRollDice(200, 400);
		p->m_iLightingSegments += rew;
		SendCommand(client, "/lightningsegments", p->m_iLightingSegments);
		wsprintf(G_cTxt, "Recibiste %d Lightning Segments por matar a %s", rew, botname);
		ShowClientMsg(client, G_cTxt);
		break;
	}
	case 4:
	{
		int rew = DropRollDice(15000, 30000);
		p->m_iContribution += rew;
		SendCommand(client, "/contrib", p->m_iContribution);
		wsprintf(G_cTxt, "Recibiste %d de contribution por matar a %s", rew, botname);
		ShowClientMsg(client, G_cTxt);
		break;
	}
	case 5:
	{
		int rew = DropRollDice(1000, 4000);
		p->fragment_pa += rew;
		SendCommand(client, "/fragment_pa", p->fragment_pa);
		wsprintf(G_cTxt, "Recibiste %d de PA Frags por matar a %s", rew, botname);
		ShowClientMsg(client, G_cTxt);
		break;
	}
	case 6:
	{
		int rew = DropRollDice(1000, 4000);
		p->fragment_ma += rew;
		SendCommand(client, "/fragment_ma", p->fragment_ma);
		wsprintf(G_cTxt, "Recibiste %d de MA Frags por matar a %s", rew, botname);
		ShowClientMsg(client, G_cTxt);
		break;
	}
	case 7:
	{
		int rew = DropRollDice(1000, 4000);
		p->fragment_mr += rew;
		SendCommand(client, "/fragment_mr", p->fragment_mr);
		wsprintf(G_cTxt, "Recibiste %d de MR Frags por matar a %s", rew, botname);
		ShowClientMsg(client, G_cTxt);
		break;
	}
	case 8:
	{
		int rew = DropRollDice(1000, 4000);
		p->fragment_dr += rew;
		SendCommand(client, "/fragment_dr", p->fragment_dr);
		wsprintf(G_cTxt, "Recibiste %d de DR Frags por matar a %s", rew, botname);
		ShowClientMsg(client, G_cTxt);
		break;
	}
	case 9:
	{
		int rew = DropRollDice(1000, 4000);
		p->fragment_hp += rew;
		SendCommand(client, "/fragment_hp", p->fragment_hp);
		wsprintf(G_cTxt, "Recibiste %d de HitProb Frags por matar a %s", rew, botname);
		ShowClientMsg(client, G_cTxt);
		break;
	}
	default:
		break;
	}
}

void cCoreServer::CallBotsForFaction(int iClient)
{
	if (!m_pClientList[iClient]) return;

	CClient* player = m_pClientList[iClient];
	DWORD dwNow = timeGetTime();

	int factionId = 0;
	if (strcmp(player->m_cLocation, "elvine") == 0) {
		factionId = 1;
	}
	else if (strcmp(player->m_cLocation, "aresden") == 0) {
		factionId = 2;
	}
	else {
		ShowClientMsg(iClient, "No perteneces a ninguna ciudad.");
		return;
	}

	if (m_mLastCallTime.find(factionId) != m_mLastCallTime.end() &&
		dwNow - m_mLastCallTime[factionId] < 30000) {
		ShowClientMsg(iClient, "Espera 30 segundos para pedir refuerzos de nuevo.");
		return;
	}

	int botsCalled = 0;

	for (int i = 0; i < m_vHBotClients.size(); i++) {
		int botHandle = m_vHBotClients[i];
		CClient* bot = m_pClientList[botHandle];

		if (bot && bot->m_bHelBot && !bot->m_bIsKilled) {
			if (strcmp(bot->m_cLocation, player->m_cLocation) == 0 &&
				!bot->m_bBeingCalled &&
				bot->m_iTargetPlayer == -1) {

				bot->m_bBeingCalled = true;
				bot->m_sCallX = player->m_sX;
				bot->m_sCallY = player->m_sY;

				botsCalled++;
			}
		}
	}	

	if (botsCalled > 0) {
		wsprintf(G_cTxt, "%d bots se dirigen a tu posicion!", botsCalled);
		ShowClientMsg(iClient, G_cTxt);

		m_mLastCallTime[factionId] = dwNow;
	}
	else {
		ShowClientMsg(iClient, "No hay bots disponibles para refuerzos.");
	}
}

bool cCoreServer::handleBotCallMovement(int iBot, CClient* hbot, DWORD dwNow)
{
	if (!hbot || !hbot->m_bBeingCalled)
		return false;

	if (hbot->m_iTargetPlayer != -1) {
		hbot->m_bBeingCalled = false;
		return false;
	}

	int distance = max(abs(hbot->m_sX - hbot->m_sCallX), abs(hbot->m_sY - hbot->m_sCallY));
	const int kArrivalDistance = 3;

	if (distance <= kArrivalDistance) {
		hbot->m_bBeingCalled = false;		
		return false; 
	}

	if (dwNow - hbot->m_dwLastMoveTime >= hbot->m_dwMoveDelay) {
		char dir = m_Misc.cGetNextMoveDir(hbot->m_sX, hbot->m_sY, hbot->m_sCallX, hbot->m_sCallY);
		bool moved = false;

		if (dir >= 1 && dir <= 8) {
			moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, dir, TRUE);
		}

		if (!moved) {
			char randomDir = DropRollDice(1, 8);
			moved = iClientMotion_Move_Handler(iBot, hbot->m_sX, hbot->m_sY, randomDir, TRUE);
		}

		if (moved) {
			SendEventToNearClient_TypeA((short)iBot, DEF_OWNERTYPE_PLAYER,
				CLIENT_COMMON_EVENT_MOTION, DEF_OBJECTRUN, NULL, NULL, NULL);
		}

		hbot->m_dwLastMoveTime = dwNow;
	}

	return true;
}

struct AutoLoginEntry {
	const char* charName;
	const char* accountName;
	const char* password;
	BOOL observer;
	short resolution;
};

// Lista de personajes a loguear automáticamente
std::vector<AutoLoginEntry> autoLoginListAres = {
	{ "Mbappe", "helbot1", "asd", FALSE, C1024x768 }, //listo
	{ "Messi", "helbot2", "asd", FALSE, C1024x768 }, //listo
	{ "Sabatini", "helbot3", "asd", FALSE, C1024x768 }, //listo
	{ "Serena", "helbot4", "asd", FALSE, C1024x768 }, //listo
	{ "Rambo", "helbot5", "asd", FALSE, C1024x768 }, //listo
	{ "Valthor", "helbot6", "asd", FALSE, C1024x768 }, //listo
	{ "nyx3n", "helbot7", "asd", FALSE, C1024x768 }, //listo
	{ "morlok", "helbot8", "asd", FALSE, C1024x768 }, //listo
	{ "Storm", "helbot9", "asd", FALSE, C1024x768 }, //listo
	{ "Draxor", "helbot10", "asd", FALSE, C1024x768 } //listo
};


// Lista de personajes a loguear automáticamente
std::vector<AutoLoginEntry> autoLoginListElv = {
	{ "dexter", "helbot11", "asd", FALSE, C1024x768 }, //listo
	{ "Draven", "helbot12", "asd", FALSE, C1024x768 }, //listo
	{ "kyros", "helbot13", "asd", FALSE, C1024x768 }, //lisot
	{ "chris", "helbot14", "asd", FALSE, C1024x768 }, //listo
	{ "Dave", "helbot15", "asd", FALSE, C1024x768 }, //listo
	{ "BOOOM", "helbot16", "asd", FALSE, C1024x768 }, //listo
	{ "Jhon", "helbot17", "asd", FALSE, C1024x768 }, //listo
	{ "Philips", "helbot18", "asd", FALSE, C1024x768 }, //listo
	{ "ElGoat", "helbot19", "asd", FALSE, C1024x768 }, //listo
	{ "k21", "helbot20", "asd", FALSE, C1024x768 } //listo
};

// Lista de personajes a loguear automáticamente
std::vector<AutoLoginEntry> autoLoginList = {
	{ "Mbappe", "helbot1", "asd", FALSE, C1024x768 }, //listo
	{ "Messi", "helbot2", "asd", FALSE, C1024x768 }, //listo
	{ "Sabatini", "helbot3", "asd", FALSE, C1024x768 }, //listo
	{ "Serena", "helbot4", "asd", FALSE, C1024x768 }, //listo
	{ "Rambo", "helbot5", "asd", FALSE, C1024x768 }, //listo
	{ "Valthor", "helbot6", "asd", FALSE, C1024x768 }, //listo
	{ "nyx3n", "helbot7", "asd", FALSE, C1024x768 }, //listo
	{ "morlok", "helbot8", "asd", FALSE, C1024x768 }, //listo
	{ "Storm", "helbot9", "asd", FALSE, C1024x768 }, //listo
	{ "Draxor", "helbot10", "asd", FALSE, C1024x768 }, //listo
	{ "dexter", "helbot11", "asd", FALSE, C1024x768 }, //listo
	{ "Draven", "helbot12", "asd", FALSE, C1024x768 }, //listo
	{ "kyros", "helbot13", "asd", FALSE, C1024x768 }, //lisot
	{ "chris", "helbot14", "asd", FALSE, C1024x768 }, //listo
	{ "Dave", "helbot15", "asd", FALSE, C1024x768 }, //listo
	{ "BOOOM", "helbot16", "asd", FALSE, C1024x768 }, //listo
	{ "Jhon", "helbot17", "asd", FALSE, C1024x768 }, //listo
	{ "Philips", "helbot18", "asd", FALSE, C1024x768 }, //listo
	{ "ElGoat", "helbot19", "asd", FALSE, C1024x768 }, //listo
	{ "k21", "helbot20", "asd", FALSE, C1024x768 } //listo
};


void cCoreServer::AutoAresLoginAll()
{
	for (const auto& entry : autoLoginListAres) {
		// Buscar un slot vacío
		int freeSlot = -1;
		for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
			if (m_pClientList[i] == NULL) {
				freeSlot = i;
				break;
			}
		}

		if (freeSlot == -1) {
			PutLogList("No hay más slots libres para auto login.");
			break;
		}

		// Crear cliente y loguear
		m_pClientList[freeSlot] = new CClient(m_hWnd, freeSlot);
		bAddClientShortCut(freeSlot);
		m_iTotalClients++;

		RequestInitPlayerAutoLogin(
			freeSlot,
			entry.charName,
			entry.accountName,
			entry.password,
			entry.observer,
			entry.resolution
			);
	}
}

void cCoreServer::AutoLoginAll()
{
	for (const auto& entry : autoLoginList) {
		// Buscar un slot vacío
		int freeSlot = -1;
		for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
			if (m_pClientList[i] == NULL) {
				freeSlot = i;
				break;
			}
		}

		if (freeSlot == -1) {
			PutLogList("No hay más slots libres para auto login.");
			break;
		}

		// Crear cliente y loguear
		m_pClientList[freeSlot] = new CClient(m_hWnd, freeSlot);
		bAddClientShortCut(freeSlot);
		m_iTotalClients++;

		RequestInitPlayerAutoLogin(
			freeSlot,
			entry.charName,
			entry.accountName,
			entry.password,
			entry.observer,
			entry.resolution
			);
	}
}

void cCoreServer::AutoElvLoginAll()
{
	for (const auto& entry : autoLoginListElv) {
		// Buscar un slot vacío
		int freeSlot = -1;
		for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
			if (m_pClientList[i] == NULL) {
				freeSlot = i;
				break;
			}
		}

		if (freeSlot == -1) {
			PutLogList("No hay más slots libres para auto login.");
			break;
		}

		// Crear cliente y loguear
		m_pClientList[freeSlot] = new CClient(m_hWnd, freeSlot);
		bAddClientShortCut(freeSlot);
		m_iTotalClients++;

		RequestInitPlayerAutoLogin(
			freeSlot,
			entry.charName,
			entry.accountName,
			entry.password,
			entry.observer,
			entry.resolution
			);
	}
}


void cCoreServer::handleLoginBots()
{
	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;
		if (!m_pClientList[i]->m_bHelBot) continue;
		m_pClientList[i]->m_bForceDisconnect = true;
		DeleteClient(i, TRUE, TRUE);
	}

	AutoLoginAll();

	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;
		if (!m_pClientList[i]->m_bHelBot) continue;
		if (m_pClientList[i]->IsLocation("aresden")) continue;
		RequestTeleportHandler(i, "2   ", "crazyshop2", 51, 41);
	}

	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;
		if (!m_pClientList[i]->m_bHelBot) continue;
		if (m_pClientList[i]->IsLocation("elvine")) continue;
		RequestTeleportHandler(i, "2   ", "crazyshop1", 51, 41);
	}

	m_bBotProces = false;
}

void cCoreServer::restoreBotToSafe()
{
	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;
		if (!m_pClientList[i]->m_bHelBot) continue;
		if (m_pClientList[i]->IsLocation("aresden")) continue;
		RequestTeleportHandler(i, "2   ", "crazyshop2", 51, 41);
	}

	for (int i = 1; i < DEF_MAXCLIENTS; ++i) {
		if (m_pClientList[i] == NULL) continue;
		if (!m_pClientList[i]->m_bHelBot) continue;
		if (m_pClientList[i]->IsLocation("elvine")) continue;
		RequestTeleportHandler(i, "2   ", "crazyshop1", 51, 41);
	}

	m_bBotProces = false;
}