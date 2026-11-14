#pragma once

#include <cstdint>

enum PlayerType : uint8_t 
{
	AI = 0,
	HUMAN = 1
};

enum class PlayerID : uint8_t 
{
	P1 = 0,
	P2 = 1,
};

enum class Stone : uint8_t 
{
	P1 = static_cast<uint8_t>(PlayerID::P1),
	P2 = static_cast<uint8_t>(PlayerID::P2),
	NONE,
};

static inline constexpr
PlayerID	getOther(const PlayerID id) {
	return (id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
}

static inline constexpr
int	toInt(PlayerID id) {
	return (id == PlayerID::P1 ? 0 : 1);
}

static inline constexpr
Stone	toStone(PlayerID id) {
	return (id == PlayerID::P1 ? Stone::P1 : Stone::P2);
}

static inline constexpr
Stone	getOther(const Stone stone) {
	return (stone == Stone::P1 ? Stone::P2 : Stone::P1);
}
