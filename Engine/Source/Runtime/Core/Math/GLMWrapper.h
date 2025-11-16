#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Wi::Math
{
	using Vec1 = glm::vec<1, float>;
	using Vec2 = glm::vec<2, float>;
	using Vec3 = glm::vec<3, float>;
	using Vec4 = glm::vec<4, float>;

	using Vec1D = glm::vec<1, double>;
	using Vec2D = glm::vec<2, double>;
	using Vec3D = glm::vec<3, double>;
	using Vec4D = glm::vec<4, double>;

	using Vec1I = glm::vec<1, int>;
	using Vec2I = glm::vec<2, int>;
	using Vec3I = glm::vec<3, int>;
	using Vec4I = glm::vec<4, int>;

	using Mat2 = glm::mat<2, 2, float>;
	using Mat3 = glm::mat<3, 3, float>;
	using Mat4 = glm::mat<4, 4, float>;

	using Mat2D = glm::mat<2, 2, double>;
	using Mat3D = glm::mat<3, 3, double>;
	using Mat4D = glm::mat<4, 4, double>;

	using Quat = glm::qua<float>;
	using QuatD = glm::qua<double>;

	template<typename T>
	[[nodiscard]] inline T Normalize(const T& v)
	{
		return glm::normalize(v);
	}

	template<typename T>
	[[nodiscard]] inline auto Length(const T& v)
	{
		return glm::length(v);
	}

	template<typename T>
	[[nodiscard]] inline auto Dot(const T& a, const T& b)
	{
		return glm::dot(a, b);
	}

	[[nodiscard]] inline Vec3 Cross(const Vec3& a, const Vec3& b)
	{
		return glm::cross(a, b);
	}

	[[nodiscard]] inline Vec3D Cross(const Vec3D& a, const Vec3D& b)
	{
		return glm::cross(a, b);
	}

	template<typename T>
	[[nodiscard]] inline auto Distance(const T& a, const T& b)
	{
		return glm::distance(a, b);
	}

	template<typename T, typename U>
	[[nodiscard]] inline auto Lerp(const T& a, const T& b, U t)
	{
		return glm::mix(a, b);
	}

	[[nodiscard]] inline Mat2 IdentityMat2()
	{
		return Mat2(1.0f);
	}

	[[nodiscard]] inline Mat3 IdentityMat3()
	{
		return Mat3(1.0f);
	}

	[[nodiscard]] inline Mat4 IdentityMat4()
	{
		return Mat4(1.0f);
	}

	[[nodiscard]] inline Mat2D IdentityMat2D()
	{
		return Mat2D(1.0);
	}

	[[nodiscard]] inline Mat3D IdentityMat3D()
	{
		return Mat3D(1.0);
	}

	[[nodiscard]] inline Mat4D IdentityMat4D()
	{
		return Mat4D(1.0);
	}

	[[nodiscard]] inline Mat3 Transpose(const Mat3& m)
	{
		return glm::transpose(m);
	}

	[[nodiscard]] inline Mat4 Transpose(const Mat4& m)
	{
		return glm::transpose(m);
	}

	[[nodiscard]] inline Mat3D Transpose(const Mat3D& m)
	{
		return glm::transpose(m);
	}

	[[nodiscard]] inline Mat4D Transpose(const Mat4D& m)
	{
		return glm::transpose(m);
	}

	[[nodiscard]] inline Mat3 Inverse(const Mat3& m)
	{
		return glm::inverse(m);
	}

	[[nodiscard]] inline Mat4 Inverse(const Mat4& m)
	{
		return glm::inverse(m);
	}

	[[nodiscard]] inline Mat3D Inverse(const Mat3D& m)
	{
		return glm::inverse(m);
	}

	[[nodiscard]] inline Mat4D Inverse(const Mat4D& m)
	{
		return glm::inverse(m);
	}

	[[nodiscard]] inline Mat4 Translate(const Mat4& m, const Vec3& t)
	{
		return glm::translate(m, t);
	}

	[[nodiscard]] inline Mat4 Scale(const Mat4& m, const Vec3& s)
	{
		return glm::scale(m, s);
	}

	[[nodiscard]] inline Mat4 Rotate(const Mat4& m, float angleRadians, const Vec3& axis)
	{
		return glm::rotate(m, angleRadians, axis);
	}

	[[nodiscard]] inline Mat4 Perspective(float fovRadians, float aspect, float zNear, float zFar)
	{
		return glm::perspective(fovRadians, aspect, zNear, zFar);
	}

	[[nodiscard]] inline Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
	{
		return glm::lookAt(eye, center, up);
	}

	[[nodiscard]] inline Quat MakeQuat(float x, float y, float z, float w)
	{
		return Quat(w, x, y, z);
	}

	[[nodiscard]] inline QuatD MakeQuatD(double x, double y, double z, double w)
	{
		return QuatD(w, x, y, z);
	}

	[[nodiscard]] inline Quat AngleAxis(float angleRadians, const Vec3& axis)
	{
		return glm::angleAxis(angleRadians, axis);
	}

	[[nodiscard]] inline QuatD AngleAxis(double angleRadians, const Vec3D& axis)
	{
		return glm::angleAxis(angleRadians, axis);
	}

	[[nodiscard]] inline Quat Slerp(const Quat& a, const Quat& b, float t)
	{
		return glm::slerp(a, b, t);
	}

	[[nodiscard]] inline QuatD Slerp(const QuatD& a, const QuatD& b, double t)
	{
		return glm::slerp(a, b, t);
	}

	[[nodiscard]] inline Mat3 ToMat3(const Quat& q)
	{
		return glm::toMat3(q);
	}

	[[nodiscard]] inline Mat4 ToMat4(const Quat& q)
	{
		return glm::toMat4(q);
	}

	[[nodiscard]] inline Mat3D ToMat3(const QuatD& q)
	{
		return glm::toMat3(q);
	}

	[[nodiscard]] inline Mat4D ToMat4(const QuatD& q)
	{
		return glm::toMat4(q);
	}
}
