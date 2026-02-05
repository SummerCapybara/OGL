#pragma once
#include <cmath>
#include <iostream>
namespace gmt {

	constexpr bool FloatEqualZero(float a, float epsilon = 1e-6f) noexcept {
		return std::fabs(a) < epsilon;
	} 

	struct Vector2 {

		float x = 0.0f;
		float y = 0.0f;

		template <typename T = float, typename K = float>
		Vector2(T x, K y) {
			static_assert(std::is_arithmetic_v<T> && std::is_arithmetic_v<K>,
						  "Vector2 components must be numeric.");
			this->x = static_cast<float>(x);
			this->y = static_cast<float>(y);
		}

		Vector2() = default;

		constexpr Vector2 operator+(const Vector2& OtherVec) const noexcept {
			return Vector2(x + OtherVec.x, y + OtherVec.y);
		}

		constexpr Vector2 operator-(const Vector2& OtherVec) const noexcept {
			return Vector2(x - OtherVec.x, y - OtherVec.y);
		}

		constexpr Vector2 operator*(const Vector2& OtherVec) const noexcept {
			return Vector2(x * OtherVec.x, y * OtherVec.y);
		}

		constexpr Vector2 operator*(const float scalar) const noexcept {
			return Vector2(x * scalar, y * scalar);
		}

		Vector2 operator/(const Vector2& OtherVec) const {
			float newX = FloatEqualZero(OtherVec.x) ? throw std::runtime_error("Division by zero") : x / OtherVec.x;
			float newY = FloatEqualZero(OtherVec.y) ? throw std::runtime_error("Division by zero") : y / OtherVec.y;
			return Vector2(newX, newY);
		}

		Vector2 operator/(const float scalar) const {
			if (FloatEqualZero(scalar)) {
				throw std::runtime_error("Division by zero");
			}
			return Vector2(x / scalar, y / scalar);
		}

		constexpr bool operator==(const Vector2& OtherVec) const noexcept {
			return FloatEqualZero(x - OtherVec.x) && FloatEqualZero(y - OtherVec.y);
		}

		constexpr bool operator!=(const Vector2& OtherVec) const noexcept  {
			return !(*this == OtherVec);
		}

		constexpr Vector2& operator+=(const Vector2& OtherVec) noexcept {
			x += OtherVec.x;
			y += OtherVec.y;
			return *this;
		}

		constexpr Vector2& operator-=(const Vector2& OtherVec) noexcept {
			x -= OtherVec.x;
			y -= OtherVec.y;
			return *this;
		}

		constexpr Vector2& operator*=(const Vector2& OtherVec) noexcept {
			x *= OtherVec.x;
			y *= OtherVec.y;
			return *this;
		}

		Vector2& operator/=(const Vector2& OtherVec) {
			x = FloatEqualZero(OtherVec.x) ? throw std::runtime_error("Division by zero") : x / OtherVec.x;
			y = FloatEqualZero(OtherVec.y) ? throw std::runtime_error("Division by zero") : y / OtherVec.y;

			return *this;
		}

		constexpr float Magnitude() const noexcept {
			return float(hypot(x, y));
		}

		Vector2 Unit() const {
			float Mag = Magnitude();
			if (FloatEqualZero(Mag)) {
				throw std::runtime_error("Division by zero");
			}
			return Vector2(x / Mag, y / Mag);
		}

		Vector2& Normalize() {
			float Mag = Magnitude();
			if (FloatEqualZero(Mag)) {
				throw std::runtime_error("Division by zero");
			}
			x /= Mag;
			y /= Mag;
			return *this;
		}

    	Vector2 Pow(const Vector2& OtherVec) const {
			return Vector2(pow(x, OtherVec.x), pow(y, OtherVec.y));
        }

		Vector2 Pow(const float Exponent) const {
			return Vector2(pow(x, Exponent), pow(y, Exponent));
        }

		constexpr float Dot(const Vector2& OtherVec) const noexcept {
			return x * OtherVec.x + y * OtherVec.y;
		}
	};

	std::ostream& operator<<(std::ostream& os, const Vector2& Vector) {
		return os << "(" << Vector.x << ", " << Vector.y << ")";
	}
} 