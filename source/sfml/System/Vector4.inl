////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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
template <typename T>
inline Vector4<T>::Vector4() :
x(0),
y(0),
z(0),
w(0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>::Vector4(T X, T Y, T Z, T W) :
x(X),
y(Y),
z(Z),
w(W)
{

}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
inline Vector4<T>::Vector4(const Vector4<U>& vector) :
x(static_cast<T>(vector.x)),
y(static_cast<T>(vector.y)),
z(static_cast<T>(vector.z)),
w(static_cast<T>(vector.w))
{
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator -(const Vector4<T>& left)
{
    return Vector4<T>(-left.x, -left.y, -left.z, -left.w);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>& operator +=(Vector4<T>& left, const Vector4<T>& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
	left.w += right.w;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>& operator -=(Vector4<T>& left, const Vector4<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
	left.w -= right.w;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator +(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator -(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator *(const Vector4<T>& left, T right)
{
    return Vector4<T>(left.x * right, left.y * right, left.z * right, left.w * right);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator *(T left, const Vector4<T>& right)
{
    return Vector4<T>(right.x * left, right.y * left, right.z * left, right.w * left);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>& operator *=(Vector4<T>& left, T right)
{
    left.x *= right;
    left.y *= right;
    left.z *= right;
	left.w *= right;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T> operator /(const Vector4<T>& left, T right)
{
    return Vector4<T>(left.x / right, left.y / right, left.z / right, left.w / right);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector4<T>& operator /=(Vector4<T>& left, T right)
{
    left.x /= right;
    left.y /= right;
    left.z /= right;
	left.w /= right;

    return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator ==(const Vector4<T>& left, const Vector4<T>& right)
{
    return (left.x == right.x) && (left.y == right.y) && (left.z == right.z) && (left.w == right.w);
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Vector4<T>& left, const Vector4<T>& right)
{
    return (left.x != right.x) || (left.y != right.y) || (left.z != right.z) || (left.w != right.w);
}
