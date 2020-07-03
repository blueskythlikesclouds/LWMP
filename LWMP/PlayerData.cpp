#include "PlayerData.h"

PlayerData::PlayerData(CPlayer* player) : player(player)
{

}

CPlayer* PlayerData::getPlayer() const
{
	return player;
}