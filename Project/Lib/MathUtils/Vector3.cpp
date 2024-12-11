#include "Vector3.h"

//============================================================================*/
//	Vector3 classMethods
//============================================================================*/

Vector3 Vector3::operator+(const Vector3& other) const {
	return Vector3(x + other.x, y + other.y, z + other.z);
}
Vector3 Vector3::operator-(const Vector3& other) const {
	return Vector3(x - other.x, y - other.y, z - other.z);
}
Vector3 Vector3::operator*(const Vector3& other) const {
	return Vector3(x * other.x, y * other.y, z * other.z);
}
Vector3 Vector3::operator/(const Vector3& other) const {
	return Vector3(x / other.x, y / other.y, z / other.z);
}

Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::operator*(float scalar) const {
	return Vector3(x * scalar, y * scalar, z * scalar);
}
Vector3 operator*(float scalar, const Vector3& v) {
	return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}
Vector3 Vector3::operator/(float scalar) const {
	return Vector3(x / scalar, y / scalar, z / scalar);
}
Vector3 operator/(float scalar, const Vector3& v) {
	return Vector3(v.x / scalar, v.y / scalar, v.z / scalar);
}

bool Vector3::operator==(const Vector3& other) const {
	return x == other.x && y == other.y && z == other.z;
}
bool Vector3::operator!=(const Vector3& other) const {
	return !(*this == other);
}

void Vector3::Init() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

float Vector3::Length() const {
	return std::sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::Normalize() const {
	float length = this->Length();
	if (length == 0.0f) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}
	return Vector3(x / length, y / length, z / length);
}

Vector3 Vector3::Cross(const Vector3& v0, const Vector3& v1) {
	return {
	  v0.y * v1.z - v0.z * v1.y,
	  v0.z * v1.x - v0.x * v1.z,
	  v0.x * v1.y - v0.y * v1.x };
}

float Vector3::Dot(const Vector3& v0, const Vector3& v1) {
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

Vector3 Vector3::CalculateValue(const std::vector<Keyframe<Vector3>>& keyframes, float time) {

	//// キーがないものは返す値が分からないのでアウト
	//assert(!keyframes.empty());

	//// キーが1つか、時刻がキーフレーム前なら最初の値とする
	//if (keyframes.size() == 1 || time <= keyframes[0].time) {

	//	return keyframes[0].value;
	//}

	//for (size_t index = 0; index < keyframes.size(); index++) {

	//	size_t nextIndex = index + 1;

	//	// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
	//	if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {

	//		// 範囲内を補完する
	//		float t =
	//			(time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);

	//		return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
	//	}
	//}

	//// ここまで来た場合は1番後の時刻よりも後ろなので最後の値を返す
	//return (*keyframes.rbegin()).value;

	keyframes;
	time;

	return Vector3();
}

Vector3 Vector3::Lerp(const Vector3& v0, const Vector3& v1, float t) {
	return Vector3(std::lerp(v0.x, v1.x, t), std::lerp(v0.y, v1.y, t), std::lerp(v0.z, v1.z, t));
}

Vector3 Vector3::Reflect(const Vector3& input, const Vector3& normal) {
	float dotProduct = Dot(input, normal);
	return input - normal * (2.0f * dotProduct);
}
