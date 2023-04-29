#pragma once

class CMatrix4
{
public:
	CMatrix4();
	CMatrix4(double dMatrix[4][4]);
	virtual ~CMatrix4();

	double m_dMatrix[4][4];

	CMatrix4 operator*(CMatrix4& mtx);
	CMatrix4& operator*=(CMatrix4& mtx);
	void GetValues(double dMatrix[4][4]);
	void SetValues(double dMatrix[4][4]);
	void SetIdentity();
	void SetTranslation(double dX, double dY, double dZ);
	void SetRotation(double dX, double dY, double dZ, double dAngle, double dCenterX = 0., double dCenterY = 0., double dCenterZ = 0.);
};

