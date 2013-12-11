#ifndef STATS_H
#define STATS_H

#include "Networking/NetString.h"

struct PlayerStats
{
	PlayerStats()
		: totalKills(0), totalDeaths(0), shotsFired(0), shotsHit(0), experiencePoints(0), captures(0), wins(0), losses(0)
	{}

	NetString *Serialize()
	{
		NetString *string = new NetString();

		if (!string->WriteUInt(totalKills) || !string->WriteUInt(totalDeaths) || !string->WriteUInt(shotsFired) || !string->WriteUInt(shotsHit) || !string->WriteUInt(experiencePoints) || !string->WriteUInt(captures) || !string->WriteUInt(wins) || !string->WriteUInt(losses))
		{
			delete string;
			return NULL;
		}

		return string;
	}

	bool Deserialize(NetString *string)
	{
		if (!string->ReadUInt(totalKills) || !string->ReadUInt(totalDeaths) || !string->ReadUInt(shotsFired) || !string->ReadUInt(shotsHit) || !string->ReadUInt(experiencePoints) || !string->ReadUInt(captures) || !string->ReadUInt(wins) || !string->ReadUInt(losses))
			return false;
		return true;
	}

	bool ContainsNew()
	{
		return (totalKills != 0 || totalDeaths != 0 || shotsFired != 0 || shotsHit != 0 || experiencePoints != 0 || captures != 0 || wins != 0 || losses != 0);
	}

	unsigned int totalKills;
	unsigned int totalDeaths;
	unsigned int shotsFired;
	unsigned int shotsHit;
	unsigned int experiencePoints;
	unsigned int captures;
	unsigned int wins;
	unsigned int losses;
};

#endif