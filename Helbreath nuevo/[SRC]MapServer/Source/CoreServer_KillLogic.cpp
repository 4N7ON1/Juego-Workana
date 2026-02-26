
int cCoreServer::GetRequiredKills(int npcType, int level)
{
	// Default base formula: 100 * (Level + 1)
	int baseKills = 100 * (level + 1);

	// Custom overrides based on NPC Type (or Name if you prefer, but Index is faster if known)
	// You can lookup the name using the npcType index in m_pNpcConfigList if needed.
	
	if (npcType >= 0 && npcType < DEF_MAXNPCTYPES && m_pNpcConfigList[npcType] != NULL)
	{
		char* npcName = m_pNpcConfigList[npcType]->m_cNpcName;

		// --- CUSTOM CONFIGURATION HERE ---
		// Example: Slimes need less kills
		if (strcmp(npcName, "Slime") == 0) return 50 * (level + 1);
		
		// Example: Abaddon needs MORE kills
		if (strcmp(npcName, "Abaddon") == 0) return 500 * (level + 1);

		// Example: Fixed amount regardless of level?
		// if (strcmp(npcName, "Goblin") == 0) return 200;
	}

	return baseKills;
}
