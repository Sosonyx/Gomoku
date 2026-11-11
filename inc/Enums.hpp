#pragma once

#include <cstdint>

enum PlayerType: uint8_t 
{
	AI = 0,
	HUMAN = 1
};

enum class PlayerID: uint8_t 
{
	P1 = 0,
	P2 = 1,
};

enum Stone: uint8_t 
{
	P1 = static_cast<uint8_t>(PlayerID::P1),
	P2 = static_cast<uint8_t>(PlayerID::P2),
	NONE,
};

constexpr	int	toInt(PlayerID id) {
	return (id == PlayerID::P1 ? 0 : 1);
}
