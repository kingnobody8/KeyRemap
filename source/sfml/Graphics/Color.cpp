////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Color.hpp>
#include <cmath> /*HABIG EDIT*/
#include <algorithm>
#include <sstream> /*HABIG EDIT*/


namespace sf
{
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
const Color Color::Black(0, 0, 0);
const Color Color::White(255, 255, 255);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);
const Color Color::Transparent(0, 0, 0, 0);


////////////////////////////////////////////////////////////
Color::Color() :
r(0),
g(0),
b(0),
a(255)
{

}


////////////////////////////////////////////////////////////
Color::Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) :
r(red),
g(green),
b(blue),
a(alpha)
{

}

/*HABIG EDIT*/
Color::Color(std::string hex)
{
	if (hex.empty())
	{
		r = g = b = a = 0;
		return;
	}

	struct
	{
		union
		{
			uint32_t n;
			uint8_t r[4];
		};
	} fin;

	if (hex.compare(0, 1, "#") == 0)
		hex.erase(hex.begin());

	if (hex.length() == 6)
		hex += std::string("FF");

	std::stringstream stream;
	stream << std::hex << hex.c_str();
	stream >> fin.n;

	//swap values because of byte order
	std::swap(fin.r[0], fin.r[3]);
	std::swap(fin.r[1], fin.r[2]);

	r = fin.r[0];
	g = fin.r[1];
	b = fin.r[2];
	a = fin.r[3];
}

////////////////////////////////////////////////////////////
Color::Color(Uint32 color) :
r((color & 0xff000000) >> 24),
g((color & 0x00ff0000) >> 16),
b((color & 0x0000ff00) >> 8 ),
a((color & 0x000000ff) >> 0 )
{

}

/*HABIG EDIT*/
/*static*/ Color Color::Blend(const Color& c1, const Color& c2, float lerp)
{
	// Algorithm explained here:	https://www.youtube.com/watch?v=LKnqECcg6Gw

	// Clamp lerp value
	lerp = std::max(0.0f, std::min(lerp, 1.0f));

	// Get the real values by squaring
	Vector4f vc1 = c1.toVec4();
	vc1.x = std::pow(vc1.x, 2);
	vc1.y = std::pow(vc1.y, 2);
	vc1.z = std::pow(vc1.z, 2);
	vc1.w = std::pow(vc1.w, 2);
	Vector4f vc2 = c2.toVec4();
	vc2.x = std::pow(vc2.x, 2);
	vc2.y = std::pow(vc2.y, 2);
	vc2.z = std::pow(vc2.z, 2);
	vc2.w = std::pow(vc2.w, 2);

	// Perform the blend
	const auto interpolate = [lerp](const float f1, const float f2)
	{
		return f1 + (f2 - f1) * lerp;
	};
	Vector4f res;
	res.x = interpolate(vc1.x, vc2.x);
	res.y = interpolate(vc1.y, vc2.y);
	res.z = interpolate(vc1.z, vc2.z);
	res.w = interpolate(vc1.w, vc2.w);


	// Square root the values to return to the blended color
	Color ret;
	ret.r = std::sqrt(res.x) * 255;
	ret.g = std::sqrt(res.y) * 255;
	ret.b = std::sqrt(res.z) * 255;
	ret.a = std::sqrt(res.w) * 255;

	return ret;
}

////////////////////////////////////////////////////////////
Uint32 Color::toInteger() const
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

/*HABIG EDIT*/
////////////////////////////////////////////////////////////
Vector4f Color::toVec4() const
{
	return Vector4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}


////////////////////////////////////////////////////////////
bool operator ==(const Color& left, const Color& right)
{
    return (left.r == right.r) &&
           (left.g == right.g) &&
           (left.b == right.b) &&
           (left.a == right.a);
}


////////////////////////////////////////////////////////////
bool operator !=(const Color& left, const Color& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
Color operator +(const Color& left, const Color& right)
{
    return Color(Uint8(std::min(int(left.r) + right.r, 255)),
                 Uint8(std::min(int(left.g) + right.g, 255)),
                 Uint8(std::min(int(left.b) + right.b, 255)),
                 Uint8(std::min(int(left.a) + right.a, 255)));
}


////////////////////////////////////////////////////////////
Color operator -(const Color& left, const Color& right)
{
    return Color(Uint8(std::max(int(left.r) - right.r, 0)),
                 Uint8(std::max(int(left.g) - right.g, 0)),
                 Uint8(std::max(int(left.b) - right.b, 0)),
                 Uint8(std::max(int(left.a) - right.a, 0)));
}


////////////////////////////////////////////////////////////
Color operator *(const Color& left, const Color& right)
{
    return Color(Uint8(int(left.r) * right.r / 255),
                 Uint8(int(left.g) * right.g / 255),
                 Uint8(int(left.b) * right.b / 255),
                 Uint8(int(left.a) * right.a / 255));
}


////////////////////////////////////////////////////////////
Color& operator +=(Color& left, const Color& right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
Color& operator -=(Color& left, const Color& right)
{
    return left = left - right;
}


////////////////////////////////////////////////////////////
Color& operator *=(Color& left, const Color& right)
{
    return left = left * right;
}

} // namespace sf
