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
			// Changed since its required to work
			return ew::Mat4(dj::Translate(position) * dj::RotateY(ew::Radians(rotation.y)) * dj::RotateX(ew::Radians(rotation.x)) * dj::RotateZ(ew::Radians(rotation.z)) * dj::Scale(scale));
		}
	};
	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 worldUp)
	{
		ew::Vec3 forward = eye - target;
		forward = ew::Normalize(forward);

		//use ew::Cross for cross product!
		ew::Vec3 right = ew::Cross(worldUp, forward);
		right = ew::Normalize(right);
		ew::Vec3 up = Cross(forward, right);
		up = Normalize(up);

		ew::Mat4 viewMat = 
		{
			right.x,   right.y,   right.z,   -ew::Dot(right, eye),
			up.x,      up.y,      up.z,      -ew::Dot(up, eye),
			forward.x, forward.y, forward.z, -ew::Dot(forward, eye),
			0.0,       0.0,       0.0,       1.0
		};

		return viewMat;
	};

	//Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far)
	{
		float width = height * aspect;
		float right = width / 2;
		float left = -(right);
		float top = height / 2;
		float bottom = -(top);

		ew::Mat4 orthographicProj = 
		{
		2 / (right - left), 0.0,                0.0,                 -((right + left) / (right - left)),
		0.0,                2 / (top - bottom), 0.0,                 -((top + bottom) / (top - bottom)),
		0.0,                0.0,                -(2 / (far - near)), -((far + near) / (far - near)),
		0.0,                0.0,                0.0,                 1.0 
		};

		return orthographicProj;
	};

	//Perspective projection
	//fov = vertical aspect ratio (radians)
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far)
	{
		// This probably doesn't look great but I tried
		ew::Mat4 perspectiveProj = {
		1 / (tanf(ew::Radians(fov) / 2) * aspect),       0.0,                        0.0,                               0.0,
		0.0,                                 1 / (tanf(ew::Radians(fov) / 2)),       0.0,                               0.0,
		0.0,                                             0.0,           ((near + far) / (near - far)),  (2 * far * near) / (near - far),
		0.0,                                             0.0,                       -1.0,                               0.0 };

		return perspectiveProj;
	};
}
