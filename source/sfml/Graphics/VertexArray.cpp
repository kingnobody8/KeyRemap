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
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
VertexArray::VertexArray() :
m_vertices     (),
m_primitiveType(Points)
{
}


////////////////////////////////////////////////////////////
VertexArray::VertexArray(PrimitiveType type, std::size_t vertexCount) :
m_vertices     (vertexCount),
m_primitiveType(type)
{
}


////////////////////////////////////////////////////////////
std::size_t VertexArray::getVertexCount() const
{
    return m_vertices.size();
}


////////////////////////////////////////////////////////////
Vertex& VertexArray::operator [](std::size_t index)
{
    return m_vertices[index];
}


////////////////////////////////////////////////////////////
const Vertex& VertexArray::operator [](std::size_t index) const
{
    return m_vertices[index];
}


////////////////////////////////////////////////////////////
void VertexArray::clear()
{
    m_vertices.clear();
}


////////////////////////////////////////////////////////////
void VertexArray::resize(std::size_t vertexCount)
{
    m_vertices.resize(vertexCount);
}


////////////////////////////////////////////////////////////
void VertexArray::append(const Vertex& vertex)
{
    m_vertices.push_back(vertex);
}


////////////////////////////////////////////////////////////
void VertexArray::setPrimitiveType(PrimitiveType type)
{
    m_primitiveType = type;
}


////////////////////////////////////////////////////////////
PrimitiveType VertexArray::getPrimitiveType() const
{
    return m_primitiveType;
}


////////////////////////////////////////////////////////////
FloatRect VertexArray::getBounds() const
{
    if (!m_vertices.empty())
    {
        float left   = m_vertices[0].position.x;
        float top    = m_vertices[0].position.y;
        float right  = m_vertices[0].position.x;
        float bottom = m_vertices[0].position.y;

        for (std::size_t i = 1; i < m_vertices.size(); ++i)
        {
            Vector2f position = m_vertices[i].position;

            // Update left and right
            if (position.x < left)
                left = position.x;
            else if (position.x > right)
                right = position.x;

            // Update top and bottom
            if (position.y < top)
                top = position.y;
            else if (position.y > bottom)
                bottom = position.y;
        }

        return FloatRect(left, top, right - left, bottom - top);
    }
    else
    {
        // Array is empty
        return FloatRect();
    }
}


////////////////////////////////////////////////////////////
void VertexArray::draw(RenderTarget& target, RenderStates states) const
{
    if (!m_vertices.empty())
        target.draw(&m_vertices[0], m_vertices.size(), m_primitiveType, states);
}


/*HABIG EDIT*/
VertexArray GenerateTrianglesStrip(std::vector<Vector2f> points, Color color, float thickness)
{
	VertexArray va;
	int count = points.size();
	va.resize((count + 1) * 2);
	va.setPrimitiveType(PrimitiveType::TrianglesStrip);
	for (int i = 1; i < count + 2; i++)
	{
		Vector2f v0 = points[(i - 1) % count];
		Vector2f v1 = points[i % count];
		Vector2f v2 = points[(i + 1) % count];

		auto tmp = v1 - v0;
		tmp.Normalize();
		Vector2f v01 = tmp;

		tmp = v2 - v1;
		tmp.Normalize();
		Vector2f v12 = tmp;

		Vector2f d = (v01 + v12).GetNormal();
		if (d.x == 0.0f && d.y == 0.0f)
			d = v01.GetNormal();
		float dot = d.Dot(v01.GetNormal());
		if (dot == 0)
			dot = 1;
		d *= thickness / 2.0f / dot;

		va[(i-1)*2 + 0] = Vertex(v1 + d, color);
		va[(i-1)*2 + 1] = Vertex(v1 - d, color);
	}
	return va;
}

} // namespace sf
