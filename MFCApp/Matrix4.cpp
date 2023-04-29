#include "pch.h"
#include "Matrix4.h"

CMatrix4::CMatrix4()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) 
			m_dMatrix[i][j] = 0.;
}

CMatrix4::CMatrix4(double dMatrix[4][4])
{
	memcpy(m_dMatrix, dMatrix, sizeof(double) * 16);
}

CMatrix4::~CMatrix4()
{
}

CMatrix4 CMatrix4::operator*(CMatrix4& mtx)
{
	CMatrix4 mtxResult;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) 
			for (int k = 0; k < 4; k++)
				mtxResult.m_dMatrix[i][j] += m_dMatrix[i][k] * mtx.m_dMatrix[k][j];
	return mtxResult;
}

CMatrix4& CMatrix4::operator*=(CMatrix4& mtx)
{
	CMatrix4 mtxResult;
	mtxResult = *this * mtx;
	*this = mtxResult;
	return *this;
}

void CMatrix4::GetValues(double dMatrix[4][4])
{
	memcpy(dMatrix, m_dMatrix, sizeof(double) * 16);
}

void CMatrix4::SetValues(double dMatrix[4][4])
{
	memcpy(m_dMatrix, dMatrix, sizeof(double) * 16);
}

void CMatrix4::SetIdentity()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) 
			m_dMatrix[i][j] = (i == j) ? 1. : 0.;
}

void CMatrix4::SetTranslation(double dX, double dY, double dZ)
{
	SetIdentity();
	m_dMatrix[0][3] = dX;
	m_dMatrix[1][3] = dY;
	m_dMatrix[2][3] = dZ;
}

void CMatrix4::SetRotation(double dX, double dY, double dZ, double dAngle, double dCenterX, double dCenterY, double dCenterZ)
{
	double dLength = sqrt(dX * dX + dY * dY + dZ * dZ);
	if (dLength == 0.)
		return;
	dX /= dLength;
	dY /= dLength;
	dZ /= dLength;
	double dCos = cos(dAngle);
	double dSin = sin(dAngle);
	double dCos1 = 1. - dCos;
	double dMatrix[4][4];
	dMatrix[0][0] = dCos + dX * dX * dCos1;
	dMatrix[0][1] = dX * dY * dCos1 - dZ * dSin;
	dMatrix[0][2] = dX * dZ * dCos1 + dY * dSin;
	dMatrix[0][3] = 0.;
	dMatrix[1][0] = dY * dX * dCos1 + dZ * dSin;
	dMatrix[1][1] = dCos + dY * dY * dCos1;
	dMatrix[1][2] = dY * dZ * dCos1 - dX * dSin;
	dMatrix[1][3] = 0.;
	dMatrix[2][0] = dZ * dX * dCos1 - dY * dSin;
	dMatrix[2][1] = dZ * dY * dCos1 + dX * dSin;
	dMatrix[2][2] = dCos + dZ * dZ * dCos1;
	dMatrix[2][3] = 0.;
	dMatrix[3][0] = 0.;
	dMatrix[3][1] = 0.;
	dMatrix[3][2] = 0.;
	dMatrix[3][3] = 1.;
	CMatrix4 mtxRotation(dMatrix);
	CMatrix4 mtxTranslation;
	mtxTranslation.SetTranslation(dCenterX, dCenterY, dCenterZ);
	CMatrix4 mtxTranslationInv;
	mtxTranslationInv.SetTranslation(-dCenterX, -dCenterY, -dCenterZ);
	*this = mtxTranslation * mtxRotation * mtxTranslationInv;
}