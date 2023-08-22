#include "replay.h"

namespace Pyewton::Frigg
{
	void Player::SetBodyList(std::vector<Body> list)
	{
		displayBodyList = list;
	}

	void Player::SetHolder(SystemStateHolder* holder)
	{
		this->holder = holder;
	}

	void Player::SetSpeed(float speed)
	{
		this->replaySpeed = speed;
	}

	void Player::ResetReplayPos()
	{
		this->replayPos = 0;
	}

	void Player::StartReplay()
	{
		replayPos = 0;
	}

	void Player::Next()
	{
		if (replayPos < holder->size / holder->systemSize - 1)
		{
			replayPos ++;
			holder->PasteState(&displayBodyList, replayPos);
			for (auto& body : displayBodyList)
			{
				body.orbit.AppendPoint(body.position);
			}
		}
	}

}