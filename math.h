#pragma once

namespace math {
	// pi constants.
	constexpr float pi = 3.1415926535897932384f; // pi
	constexpr float pi_2 = pi * 2.f;               // pi * 2

	__forceinline float Bias(float x, float biasAmt)
	{
		float lastExponent = log(biasAmt) * -1.4427f; // (-1.4427 = 1 / log(0.5))

		return pow(x, lastExponent);
	}

	typedef __declspec(align(16)) union {
		float f[4];
		__m128 v;
	} m128;

	__forceinline __m128 sqrt_ps(const __m128 squared) {
		return _mm_sqrt_ps(squared);
	}

	bool IntersectLineWithBB(vec3_t& vStart, vec3_t& vEndDelta, vec3_t& vMin, vec3_t& vMax);

	float SegmentToSegment(const vec3_t s1, const vec3_t s2, const vec3_t k1, const vec3_t k2);


	__forceinline float fsel(float fComparand, float fValGE, float fLT)
	{
		return fComparand >= 0 ? fValGE : fLT;
	}

	__forceinline double fsel(double fComparand, double fValGE, double fLT)
	{
		return fComparand >= 0 ? fValGE : fLT;
	}


	__forceinline float RemapValClamped(float val, float A, float B, float C, float D)
	{
		if (A == B)
			return fsel(val - B, D, C);

		float cVal = (val - A) / (B - A);
		cVal = std::clamp(cVal, 0.0f, 1.0f);

		return C + (D - C) * cVal;
	}

	// degrees to radians.
	__forceinline constexpr float deg_to_rad(float val) {
		return val * (pi / 180.f);
	}

	// radians to degrees.
	__forceinline constexpr float rad_to_deg(float val) {
		return val * (180.f / pi);
	}

	// angle mod ( shitty normalize ).
	__forceinline float AngleMod(float angle) {
		return (360.f / 65536) * ((int)(angle * (65536.f / 360.f)) & 65535);
	}

	template <class T>
	__forceinline T Lerp(float flPercent, T const& A, T const& B)
	{
		return A + (B - A) * flPercent;
	}

	void AngleMatrix(const ang_t& ang, const vec3_t& pos, matrix3x4_t& out);

	// normalizes an angle.
	void NormalizeAngle(float& angle);

	__forceinline float NormalizedAngle(float angle) {
		NormalizeAngle(angle);
		return angle;
	}

	__forceinline float AngleDiff(float angle, float angle2) {
		return NormalizedAngle(NormalizedAngle(angle) - NormalizedAngle(angle2));
	}

	__forceinline float InterpSpeed(float delta, float speed) {
		return fabs(delta) * fabs(speed);
	}

	__forceinline   float SmoothStepBounds(float edge0, float edge1, float x) {
		float v1 = std::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
		return v1 * v1 * (3 - 2 * v1);
	}

	__forceinline  float ClampCycle(float flCycleIn) {
		flCycleIn -= int(flCycleIn);

		if (flCycleIn < 0) {
			flCycleIn += 1;
		}
		else if (flCycleIn > 1) {
			flCycleIn -= 1;
		}

		return flCycleIn;
	}

	vec3_t Approach(vec3_t target, vec3_t value, float speed);
	float Approach(float target, float value, float speed);
	float ApproachAngle(float target, float value, float speed);
	void  VectorAngles(const vec3_t& forward, ang_t& angles, vec3_t* up = nullptr);
	void  AngleVectors(const ang_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);
	float GetFOV(const ang_t& view_angles, const vec3_t& start, const vec3_t& end);
	void  VectorTransform(const vec3_t& in, const matrix3x4_t& matrix, vec3_t& out);
	void  VectorITransform(const vec3_t& in, const matrix3x4_t& matrix, vec3_t& out);
	void  MatrixAngles(const matrix3x4_t& matrix, ang_t& angles);
	void  MatrixCopy(const matrix3x4_t& in, matrix3x4_t& out);
	void  ConcatTransforms(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out);

	// computes the intersection of a ray with a box ( AABB ).
	bool IntersectRayWithBox(const vec3_t& start, const vec3_t& delta, const vec3_t& mins, const vec3_t& maxs, float tolerance, BoxTraceInfo_t* out_info);
	bool IntersectRayWithBox(const vec3_t& start, const vec3_t& delta, const vec3_t& mins, const vec3_t& maxs, float tolerance, CBaseTrace* out_tr, float* fraction_left_solid = nullptr);

	// computes the intersection of a ray with a oriented box ( OBB ).
	bool IntersectRayWithOBB(const vec3_t& start, const vec3_t& delta, const matrix3x4_t& obb_to_world, const vec3_t& mins, const vec3_t& maxs, float tolerance, CBaseTrace* out_tr);
	bool IntersectRayWithOBB(const vec3_t& start, const vec3_t& delta, const vec3_t& box_origin, const ang_t& box_rotation, const vec3_t& mins, const vec3_t& maxs, float tolerance, CBaseTrace* out_tr);

	// returns whether or not there was an intersection of a sphere against an infinitely extending ray. 
	// returns the two intersection points.
	bool IntersectInfiniteRayWithSphere(const vec3_t& start, const vec3_t& delta, const vec3_t& sphere_center, float radius, float* out_t1, float* out_t2);

	// returns whether or not there was an intersection, also returns the two intersection points ( clamped 0.f to 1.f. ).
	// note: the point of closest approach can be found at the average t value.
	bool IntersectRayWithSphere(const vec3_t& start, const vec3_t& delta, const vec3_t& sphere_center, float radius, float* out_t1, float* out_t2);

	vec3_t Interpolate(const vec3_t from, const vec3_t to, const float percent);

	template < typename t >
	__forceinline void clamp(t& n, const t& lower, const t& upper) {
		n = std::max(lower, std::min(n, upper));
	}
}