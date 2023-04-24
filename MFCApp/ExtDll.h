#pragma once

typedef void*	(__stdcall* NEW_VIEW)();
typedef void*	(__stdcall* REMOVE_VIEW)(void *pView);


class CExtDll
{
public:
	CExtDll();
	~CExtDll();

	HINSTANCE		m_hDriver;

	NEW_VIEW		m_NewView;
	REMOVE_VIEW		m_RemoveView;

	void LoadDriver();
};

