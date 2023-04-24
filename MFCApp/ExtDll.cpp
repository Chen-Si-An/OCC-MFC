#include "pch.h"
#include "ExtDll.h"

CExtDll::CExtDll()
{
	m_hDriver = NULL;

	m_NewView = NULL;
	m_RemoveView = NULL;
}

CExtDll::~CExtDll()
{
	if (m_hDriver)
		FreeLibrary(m_hDriver);
}

void CExtDll::LoadDriver()
{
	m_hDriver = LoadLibrary(_T("OCCLib.dll"));
	if (!m_hDriver)
	{
		AfxMessageBox(_T("LoadLibrary Failed"));
		return;
	}

	m_NewView = (NEW_VIEW)GetProcAddress(m_hDriver, "NewView");
	if (!m_NewView)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}
	

	m_RemoveView = (REMOVE_VIEW)GetProcAddress(m_hDriver, "RemoveView");
	if (!m_RemoveView)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}
}
