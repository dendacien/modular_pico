#include "lib/player.hpp"

void Player::draw_on_frame(uint8_t frame[])
{
	for (int row = 0; row < 8; ++row)
	{
		frame[row] = 0xFFu;
	}

	for (int i = 0; i < body.size(); i++)
	{
		int x = body[i].x;
		int y = body[i].y;

		if (i == body.size() -1)
		{
			// Always draw the head - full brightness
			frame[y] &= ~(1u << (7u - x));
		}
		else
		{
			// Body to follow PWM brightness
			if (body_pwm_counter < BODY_BRIGHTNESS)
			{
				frame[y] &= ~(1u << (7u - x));
			}
		}
	}
	body_pwm_counter = (body_pwm_counter + 1) % 4;
}

bool Player::intersects_body(int new_x, int new_y)
{
	for (const auto &segment: body)
	{
		if(segment.x == new_x && segment.y == new_y)
		{
			return true;
		}
	}
	return false;
}

bool Player::move(int new_x, int new_y)
{
	if (new_x > max_x || new_x < 0 || new_y > max_y || new_y < 0)
	{
		return false;
	}

	if (intersects_body(new_x, new_y))
	{
		return false;
	}

	head_x = new_x;
	head_y = new_y;
	body.push_back({head_x, head_y});

	const size_t max_body_len = static_cast<size_t>(score) + 1u;
	while (body.size() > max_body_len)
	{
		body.erase(body.begin());
	}
	return true;
}

bool Player::move_by(int dx, int dy)
{
	// Need to invert because of joystick to display
	if (move(head_x - dy, head_y - dx)) 
	{
		return true;
	}
	else
	{
		return false;
	}
}