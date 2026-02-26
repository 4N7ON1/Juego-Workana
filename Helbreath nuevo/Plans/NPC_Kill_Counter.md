# Sistema de Contador de Muertes de NPCs

Este documento detalla la implementación del sistema "Contador de Muertes de NPCs" para Helbreath, incluyendo cambios en el Servidor, Cliente y Protocolo de Comunicación.

## 1. Definiciones de Protocolo (Shared/netmsg.h)

Agregamos los IDs de paquetes necesarios para solicitar y recibir la lista de muertes.

```cpp
// En Shared/netmsg.h

// Request: Cliente pide la lista (payload vacío o con filtro si es necesario)
#define CLIENT_REQUEST_NPC_KILL_DATA    0xAD21 

// Notify: Servidor envía la lista de NPCs y progresos
#define CLIENT_NOTIFY_NPC_KILL_DATA     0xDE73 
```

---

## 2. Implementación en Servidor

### A. Estructura de Datos del Jugador (Client.h)

Modificar la clase `CClient` para guardar el progreso de cada NPC. Si `MAX_NPC_TYPE` no está definido, usaremos un valor seguro (ej: 500).

```cpp
// En [SRC]MapServer/Header/Client.h dentro de class CClient

public:
    // ... otras variables ...

    // Estructura para contadores
    // Asumiendo que hay un máximo de tipos de NPC. 
    // Si los NpcType IDs son altos (ej: 5000), usar std::map es mejor.
    std::map<int, int> m_mapNpcKillCount;   // Key: NpcType, Value: Kills actuales
    std::map<int, int> m_mapNpcHuntLevel;   // Key: NpcType, Value: Nivel de caza

    // Constante para el máximo de muertes por nivel
    const int MAX_KILLS_PER_LEVEL = 2000;

    void AddNpcKill(int npcType);
```

### B. Lógica de Muerte y Nivel (Game.cpp / Client.cpp)

Implementar la función `AddNpcKill` en `Client.cpp` que se llamará cuando un NPC muera.

```cpp
// En [SRC]MapServer/Source/Client.cpp

void CClient::AddNpcKill(int npcType)
{
    // Incrementar contador
    m_mapNpcKillCount[npcType]++;

    int currentKills = m_mapNpcKillCount[npcType];
    
    // Verificar Subida de Nivel
    if (currentKills >= MAX_KILLS_PER_LEVEL)
    {
        // Resetear contador y subir nivel
        m_mapNpcKillCount[npcType] = 0;
        m_mapNpcHuntLevel[npcType]++;
        
        // Opcional: Notificar al usuario "¡Has subido de nivel de caza de Slime!"
        wsprintf(G_cTxt, "¡Has alcanzado el nivel %d de caza para este NPC!", m_mapNpcHuntLevel[npcType]);
        ShowClientMsg(this->m_iClientID, G_cTxt); // Asumiendo función auxiliar existente
    }
}
```

En `Game.cpp` (o donde se maneje `NpcKilledHandler`), llamar a esta función:

```cpp
// Al morir un NPC, identificar al Killer (pAttacker)
if (pAttacker && pAttacker->IsPlayer()) // Pseudo-código
{
    pAttacker->AddNpcKill(pDeadNpc->m_sOriginalType); // Usar el tipo de NPC
}
```

### C. Guardado de Datos (Save)

Agregar la lógica para guardar/cargar estos datos en la base de datos o archivo plano.

**Guardar (Save):**
```cpp
// Pseudo-código para guardar en archivo o DB
// Formato sugerido: "NPCKILL [Type] [Kills] [Level]"
for (auto const& [type, kills] : m_mapNpcKillCount)
{
    int level = m_mapNpcHuntLevel[type];
    if (level > 0 || kills > 0) {
        // Guardar línea: NPCKILL npcType kills level
    }
}
```

### D. Enviar Datos/Paquete (Game.cpp / Client.cpp)

Responder al paquete `CLIENT_REQUEST_NPC_KILL_DATA`.

```cpp
// En Packet Handler (MsgProcess)

case CLIENT_REQUEST_NPC_KILL_DATA:
{
    // Preparar el paquete de respuesta
    char pData[2048]; // Buffer grande
    char * cp = pData;
    int iCount = 0;
    
    // Header
    DWORD * dwp = (DWORD *)cp;
    *dwp = CLIENT_NOTIFY_NPC_KILL_DATA;
    cp += 4;
    
    // Dejar espacio para el número de items
    short * spCount = (short *)cp;
    cp += 2;

    // Iterar y llenar datos
    for (auto const& [type, kills] : m_mapNpcKillCount)
    {
        int level = m_mapNpcHuntLevel[type];
        if (level == 0 && kills == 0) continue;

        // Necesitamos el Nombre del NPC basado en el Type
        // Asumimos función GetNpcName(type)
        char npcName[21];
        GetNpcName(type, npcName); 

        // Structure: [Name 21][Cnt 4][Lvl 4]
        memcpy(cp, npcName, 20); cp[20] = 0; cp += 21;
        *(int*)cp = kills; cp += 4;
        *(int*)cp = level; cp += 4;
        
        iCount++;
        
        // Cuidado con buffer overflow
        if (cp - pData > 1800) break; // Límite de seguridad
    }
    
    *spCount = iCount; // Escribir cantidad total
    
    // Enviar
    pClient->SendMsg(pData, cp - pData);
    break;
}
```

---

## 3. Implementación en Cliente

### A. Estructura Visual (Game.h)

```cpp
// En [SRC]Client Version 13/Headers/Game.h class CGame

struct st_npc_kill_data {
    char m_cNpcName[22];
    int m_iKillCount;
    int m_iHuntingLevel;
};

// Variable para almacenar la lista
std::vector<st_npc_kill_data> m_vNpcKillList;

// Flag para mostrar la ventana
bool m_bShowNpcKillWindow = false;

// Métodos
void DrawNpcKillWindow(short msX, short msY, char cLB);
void ReqNpcKillData();
void GetNpcKillData(char * data);
```

### B. Procesar Paquete (Game.cpp)

```cpp
// En CGame::MsgProcess (o donde se manejen notifies)

case CLIENT_NOTIFY_NPC_KILL_DATA:
{
    GetNpcKillData(pData + 4); // +4 saltando el header MsgID
    break;
}

// Implementación de GetNpcKillData
void CGame::GetNpcKillData(char * data)
{
    m_vNpcKillList.clear();
    char * cp = data;
    
    short count = *(short*)cp;
    cp += 2;
    
    for (int i = 0; i < count; i++)
    {
        st_npc_kill_data entry;
        memcpy(entry.m_cNpcName, cp, 21);
        entry.m_cNpcName[21] = 0;
        cp += 21;
        
        entry.m_iKillCount = *(int*)cp; cp += 4;
        entry.m_iHuntingLevel = *(int*)cp; cp += 4;
        
        m_vNpcKillList.push_back(entry);
    }
    
    // Abrir la ventana automáticamente al recibir datos
    m_bShowNpcKillWindow = true; 
}
```

### C. Dibujar Ventana (UI)

```cpp
// En CGame::DrawInterface (o donde se dibujen ventanas/dialogs)

if (m_bShowNpcKillWindow)
{
    DrawNpcKillWindow(msX, msY, cLB);
}

// Implementación de DrawNpcKillWindow
void CGame::DrawNpcKillWindow(short msX, short msY, char cLB)
{
    // Coordenadas base de la ventana
    int sX = 100; 
    int sY = 100;
    
    // Dibujar fondo de ventana (usando assets existentes o caja negra)
    // DrawDialogBox(sX, sY, ...); // Función ficticia de dibujo base
    DrawCenteredBoxWithPixels(sX, sY, sX, sY, 300, 400, 0, 0, 0); // Fondo negro simple por ahora

    // Título
    dsText(sX + 10, sY + 10, "NPC Hunter Rankings", 255, 255, 255);

    // Listado
    int yOffset = 40;
    for (const auto& npc : m_vNpcKillList)
    {
        char buffer[100];
        // Formato: "Slime Level 1 [100 / 2000]"
        wsprintf(buffer, "%s Level %d [%d / 2000]", npc.m_cNpcName, npc.m_iHuntingLevel, npc.m_iKillCount);
        
        dsText(sX + 20, sY + yOffset, buffer, 200, 200, 200); // Texto gris claro
        
        // Barra de progreso visual (opcional)
        // float pct = (float)npc.m_iKillCount / 2000.0f;
        // DrawBar(..., pct);
        
        yOffset += 20;
    }
    
    // Botón Cerrar (X)
    // Lógica de click para cerrar
    if (cLB && MouseInRect(sX + 280, sY, 20, 20, msX, msY))
    {
        m_bShowNpcKillWindow = false;
    }
}
```

### D. Solicitar Datos

Agregar un botón en el menú principal o un comando (ej: `/npckills`) para llamar a `ReqNpcKillData()`.

```cpp
void CGame::ReqNpcKillData()
{
    // Enviar paquete simple sin payload
    char data[6];
    *(DWORD*)data = CLIENT_REQUEST_NPC_KILL_DATA;
    // ... send logic ...
    if (m_pGSock) m_pGSock->iSendMsg(data, 6);
}
```
