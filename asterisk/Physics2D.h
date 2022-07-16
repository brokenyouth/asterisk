#pragma once

namespace Physics2D
{
	struct Box
	{
		int x;
		int y;
		float size;


		Box(int _x, int _y, int _size) :
			x(_x), y(_y), size(_size)
		{
			
		}
	};

	static bool BoxCollide(Box& b1, Box& b2)
	{
		bool CollisionX = (b1.x + b1.size >= b2.x) && (b2.x + b2.size >= b1.x);
		bool CollisionY = (b1.y + b1.size >= b2.y) && (b2.y + b2.size >= b1.y);

		return CollisionX && CollisionY;
	}

	static bool BoxCollide2(Box& b1, Box& b2)
	{
		return (std::abs(b1.x - b2.x) * 2 < (b1.size + b2.size)) && (std::abs(b1.y - b2.y) * 2 < (b1.size + b2.size));
	}
}