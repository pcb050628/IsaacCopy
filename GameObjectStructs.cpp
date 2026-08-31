#include "GameObjectStructs.h"

EPlayerHeartState operator+(const EPlayerHeartState& from, const EPlayerHeartState& to)
{
	int result = static_cast<int>(from) + static_cast<int>(to);
	return result >= 2 ? EPlayerHeartState::Full : result > 0 ? EPlayerHeartState::Half : EPlayerHeartState::Empty;
}
EPlayerHeartState operator-(const EPlayerHeartState& from, const EPlayerHeartState& to)
{
	int result = static_cast<int>(from) - static_cast<int>(to);
	return result <= 0 ? EPlayerHeartState::Empty : result < 2 ? EPlayerHeartState::Half : EPlayerHeartState::Full;
}
EPlayerHeartState operator-(const EPlayerHeartState& v)
{
	int result = static_cast<int>(v);
	return result <= 0 ? EPlayerHeartState::Full : result < 2 ? EPlayerHeartState::Half : EPlayerHeartState::Empty;
}