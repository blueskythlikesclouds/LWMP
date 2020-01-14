#pragma once

#include <cmath>

#include "Types.h"

inline Vector3 getTranslation(Matrix34& m)
{
	return { m.data[3][0], m.data[3][1], m.data[3][2] };
}

inline Quaternion getQuaternion(Matrix34& m)
{
	Quaternion q;
	
	float fTrace = m.data[0][0] + m.data[1][1] + m.data[2][2];
	float fRoot;

	if (fTrace > 0.0) 
	{
		fRoot = sqrt(fTrace + 1.0f);
		q.w = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		q.x = (m.data[1][2] - m.data[2][1]) * fRoot;
		q.y = (m.data[2][0] - m.data[0][2]) * fRoot;
		q.z = (m.data[0][1] - m.data[1][0]) * fRoot;
	}
	else 
	{
		static size_t s_iNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if (m.data[1][1] > m.data[0][0])
			i = 1;
		if (m.data[2][2] > m.data[i][i])
			i = 2;
		size_t j = s_iNext[i];
		size_t k = s_iNext[j];

		fRoot = sqrt(m.data[i][i] - m.data[j][j] - m.data[k][k] + 1.0f);
		float* apkQuat[3] = { &q.x, &q.y, &q.z };
		*apkQuat[i] = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		q.w = (m.data[j][k] - m.data[k][j]) * fRoot;
		*apkQuat[j] = (m.data[i][j] + m.data[j][i]) * fRoot;
		*apkQuat[k] = (m.data[i][k] + m.data[k][i]) * fRoot;
	}

	return q;
}