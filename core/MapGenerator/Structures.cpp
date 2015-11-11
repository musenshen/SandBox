
#include "Structures.h"
#include "Math/LineEquation.h" 
#include <iostream>

namespace maps {

bool center::IsInsideBoundingBox(int width, int height){
	if(_position.x < 0 || _position.x >= width || _position.y < 0 || _position.y >= height)
		return false;

	return true;
}

bool center::GoesBefore(Vec2 p_a, Vec2 p_b){
	if((p_a - _position).x >= 0 && (p_b - _position).x < 0)
		return true;

	if(p_a.x == 0 && p_b.x == 0)
		return p_a.y < p_b.y;

	Vec2 ca(_position, p_a);
	Vec2 cb(_position, p_b);
	return ca.CrossProduct(cb) > 0;
}

}