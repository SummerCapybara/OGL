#pragma once
#include <cmath>

struct Vector2 {

	float x,y;

	Vector2(float x = 0.0f, float y = 0.0f): x(x), y(y) {};

	Vector2 operator+(const Vector2& OtherVec) const {
		return Vector2(x + OtherVec.x, y + OtherVec.y);
	};

	Vector2 operator+(const float values[2]) const {
		return Vector2(x + values[0], y + values[1]);
	};

	Vector2 operator-(const Vector2& OtherVec) const {
		return Vector2(x - OtherVec.x, y - OtherVec.y);
	};

	Vector2 operator-(const float values[2]) const {
		return Vector2(x - values[0], y - values[1]);
	};

	Vector2 operator*(const Vector2& OtherVec) const {
		return Vector2(x * OtherVec.x, y * OtherVec.y);
	};

	Vector2 operator*(const float values[2]) const {
		return Vector2(x * values[0], y * values[1]);
	};

	Vector2 operator*(const float scalar) const {
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 operator/(const Vector2& OtherVec) const {
		return Vector2(x / OtherVec.x, y / OtherVec.y);
	};

	Vector2 operator/(const float values[2]) const {
		return Vector2(x / values[0], y / values[1]);
	};

	Vector2 operator/(const float scalar) const {
		return Vector2(x / scalar, y / scalar);
	}

	Vector2 operator^(const double exponent) const {
		return Vector2(pow(x, exponent), pow(y, exponent));
	};

	Vector2 operator^(const float values[2]) const {
		return Vector2(pow(x, values[0]), pow(y, values[1]));
	};

	Vector2 operator^(const double values[2]) const {
		return Vector2(pow(x, values[0]), pow(y, values[1]));
	};

	Vector2 operator^(const Vector2& OtherVec) const {
		return Vector2(pow(x, OtherVec.x), pow(y, OtherVec.y));
	};

	bool operator==(const Vector2& OtherVec) const {
		if (x == OtherVec.x && y == OtherVec.y) {
			return true;
		}
		return false;
	};

	Vector2& operator+=(const Vector2& OtherVec) {
		x += OtherVec.x;
		y += OtherVec.y;
		return *this;
	};

	Vector2& operator+=(const float values[2]) {
		x += values[0];
		y += values[1];
		return *this;
	};

	Vector2& operator-=(const Vector2& OtherVec) {
		x -= OtherVec.x;
		y -= OtherVec.y;
		return *this;
	};

	Vector2& operator-=(const float values[2]) {
		x -= values[0];
		y -= values[1];
		return *this;
	};

	Vector2& operator*=(const Vector2& OtherVec) {
		x *= OtherVec.x;
		y *= OtherVec.y;
		return *this;
	};

	Vector2& operator*=(const float values[2]) {
		x *= values[0]; 
		y *= values[1];
		return *this;
	};

	Vector2& operator/=(const Vector2& OtherVec) {
		x /= OtherVec.x;
		y /= OtherVec.y;
		return *this;
	};

	Vector2& operator/=(const float values[2]) {
		x /= values[0];
		y /= values[1];
		return *this;
	};

	Vector2& operator^=(const Vector2& OtherVec) {
		x = pow(x, OtherVec.x);
		y = pow(y, OtherVec.y);
		return *this;
	};

	Vector2& operator^=(const double exponents[2]) {
		x = pow(x, exponents[0]);
		y = pow(y, exponents[1]);
		return *this;
	};

	Vector2& operator^=(const int exponents[2]) {
		x = pow(x, exponents[0]);
		y = pow(y, exponents[1]);
		return *this;
	};

	float Magnitude() const {
		return float(sqrt((x * x) + (y * y)));
	};

	Vector2 Unit() const {
		float Mag = Magnitude();
		float minMagnitude = 1e-8f; // 0.00000001f
		if (Mag < minMagnitude) {
			return Vector2(0.0f, 0.0f);
		};
		return Vector2(x / Mag, y / Mag);
	};

	Vector2& Normalize() {
		float Mag = Magnitude();
		x /= Mag;
		y /= Mag;
		return *this;
	};

	float dot(const Vector2& OtherVec) const {
		return x * OtherVec.x + y * OtherVec.y;
	}
};