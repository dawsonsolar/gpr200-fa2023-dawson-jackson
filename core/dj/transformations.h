#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"

namespace dj
{
	//Identity matrix
	inline ew::Mat4 Identity()
	{
		return ew::IdentityMatrix();
	};

	//Scale on x,y,z axes
	inline ew::Mat4 Scale(ew::Vec3 s)
	{
		return ew::Mat4
			(s.x, 0.0, 0.0, 0.0,
			0.0, s.y, 0.0, 0.0,
			0.0, 0.0, s.z, 0.0,
			0.0, 0.0, 0.0, 1.0);
	};

	//Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) 
	{
		ew::Mat4 mat = Identity();
		mat[1][1] = cos(rad);
		mat[2][1] = -sin(rad);
		mat[1][2] = sin(rad);
		mat[2][2] = cos(rad);
		return mat;
	};

	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad)
	{
		ew::Mat4 mat = Identity();
		mat[0][0] = cos(rad);
		mat[2][0] = sin(rad);
		mat[0][2] = -sin(rad);
		mat[2][2] = cos(rad);
		return mat;
	};

	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) 
	{
		ew::Mat4 mat = Identity();
		mat[0][0] = cos(rad);
		mat[1][0] = -sin(rad);
		mat[0][1] = sin(rad);
		mat[1][1] = cos(rad);
		return mat;
	};

	//Translate x,y,z 
	inline ew::Mat4 Translate(ew::Vec3 t)
	{
		return ew::Mat4(1.0, 0.0, 0.0, t.x,
			0.0, 1.0, 0.0, t.y,
			0.0, 0.0, 1.0, t.z,
			0.0, 0.0, 0.0, 1.0);
	};

	struct Transform
	{
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(5.0f, 5.0f, 0.0f);  // Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		ew::Mat4 getModelMatrix() const 
		{	
			return Translate(position) * RotateY(ew::Radians(rotation.y)) * RotateX(ew::Radians(rotation.x)) * RotateZ(ew::Radians(rotation.z)) * Scale(scale);
		}
	};
}