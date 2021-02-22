#include "vector.h"

vector_t VectorScale(const vector_t& _v1, const vector_t& _v2) {
	return vector_t(_v1.x * _v2.x,
		_v1.y * _v2.y,
		_v1.z * _v2.z);
}

float VectorDot(const vector_t& _v, const vector_t& _w) {
	return _v.x * _w.x + _v.y * _w.y + _v.z * _w.z;
}

vector_t VectorCross(const vector_t& _v, const vector_t& _w) {
	return vector_t(_v.y * _w.z - _v.z * _w.y,
		_v.z * _w.x - _v.x * _w.z,
		_v.x * _w.y - _v.y * _w.x);
}

float VectorMagnitude(const vector_t& _v) {
	return std::sqrt(_v.x * _v.x + _v.y * _v.y + _v.z * _v.z);
}

float VectorSqrMagnitude(const vector_t& _v) {
	return _v.x * _v.x + _v.y * _v.y + _v.z * _v.z;
}


vector_t VectorNormalized(const vector_t& _v) {
	auto mag = VectorMagnitude(_v);
	return vector_t(_v.x / mag,
		_v.y / mag,
		_v.z / mag);
}


float VectorDistance(const vector_t& _v, const vector_t& _w) {
	vector_t v = _v;
	vector_t w = _w;
	return VectorMagnitude(v - w);
}