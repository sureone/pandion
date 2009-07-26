/**
 * This file is part of Pandion.
 *
 * Pandion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pandion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pandion.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Filename:    PdnWnd.h
 * Author(s):   Dries Staelens
 * Copyright:   Copyright (c) 2009 Dries Staelens
 * Description: TODOTODOTODO
 */
#pragma once
#include "External.h"
#include "DispInterfaceImpl.h"
#include "AxHostWnd.h"

class PdnModule;

class CPdnWnd :
	public DispInterfaceImpl<IPdnWnd>,
	public IDocHostUIHandler,
	public IDocHostShowUI,
	public IInternetSecurityManager
//	public IServiceProvider 
{
protected:
	_CrtMemState state;

	/*
	 * The internal COM reference counter, used by the IUnknown implementation.
	 */
	unsigned long			m_COMReferenceCounter;
	/*
	 * This object's TypeInfo, used by the IDispatch implementation.
	 */
//	ITypeInfo*				m_TypeInfo;
	/*
	 *
	 */
	bool            m_COMCannotSelfDelete;

	WNDCLASSEX      m_WindowClass;
	HWND            m_hWnd;
	HWND	        m_hWndFocus;
	HWND	        m_hWndActiveWindow;
	HWND	        m_hWndLastFocusedWindow;
	AxHostWnd       m_ActiveXHost;

	UINT            m_TaskbarRestartMessage;

	BOOL	        m_bPopUnder;

	_bstr_t         m_sMinHandler;
	_bstr_t         m_sCloseHandler;
	_bstr_t         m_sMenuHandler;
	_bstr_t         m_sCmdLineHandler;
	_bstr_t         m_sRestoreHandler;
	_bstr_t         m_sActivationHandler;

	std::wstring    m_Name;
	std::wstring    m_URL;
	_variant_t      m_windowParams;

	POINT		    m_minSize;

	PdnModule* m_Module;
	External        m_External;
	IWebBrowser2*   m_pBrowser;
	IMenuBar*       m_pMenuBar;

	IInternetSecurityManager* m_pSecurityMgr;
public:
	CPdnWnd();
	~CPdnWnd();

	int Create(RECT& rect, std::wstring Name, std::wstring URL,
		_variant_t& windowParams, PdnModule* Module);

	static LRESULT CALLBACK StartWindowProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);
	LRESULT WindowProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

private:
	void ChangeIcon(int icon);
	void PopUnder(BOOL b);
	HRESULT ContainerCreate();

public:
	/* Handler prototypes:
	 *  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	 *  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	 *  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	 */
	/* Window Messages */

    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnJSInvoke(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNotifyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCopyData(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTaskbarRestart(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/* Webbrowser Events */
	STDMETHOD_(void, WindowClosing)(VARIANT_BOOL IsChildWindow, VARIANT_BOOL* Cancel);
	STDMETHOD_(void, NavigateComplete2)(IDispatch* pDisp, VARIANT* URL);

	/* IDocHostUIHandler */
    STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);
    STDMETHOD(GetHostInfo)(DOCHOSTUIINFO *pInfo);
    STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget, IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc);
	STDMETHOD(HideUI)();
    STDMETHOD(UpdateUI)();
    STDMETHOD(EnableModeless)(BOOL fEnable);
    STDMETHOD(OnDocWindowActivate)(BOOL fActivate);
    STDMETHOD(OnFrameWindowActivate)(BOOL fActivate);
    STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow *pUIWindow, BOOL fRameWindow);
    STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID *pguidCmdGroup, DWORD nCmdID);
    STDMETHOD(GetOptionKeyPath)(LPOLESTR *pchKey, DWORD dw);
	STDMETHOD(GetDropTarget)(IDropTarget *pDropTarget, IDropTarget **ppDropTarget);
    STDMETHOD(GetExternal)(IDispatch **ppDispatch);
    STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut);
    STDMETHOD(FilterDataObject)(IDataObject *pDO, IDataObject **ppDORet);

	/* IDocHostShowUI */
	STDMETHOD(ShowMessage)(HWND hwnd, LPOLESTR lpstrText,
		LPOLESTR lpstrCaption, DWORD dwType, LPOLESTR lpstrHelpFile,
		DWORD dwHelpContext, LRESULT *plResult);
	STDMETHOD(ShowHelp)(HWND hwnd, LPOLESTR pszHelpFile,
		UINT uCommand, DWORD dwData, POINT ptMouse,
		IDispatch *pDispatchObjectHit);

	/* IServiceProvider */
	STDMETHOD(QueryService)(REFGUID guidService, REFIID riid, void **ppv);

	/* IInternetSecurityManager */
	STDMETHOD(SetSecuritySite)(IInternetSecurityMgrSite *pSite);
	STDMETHOD(GetSecuritySite)(IInternetSecurityMgrSite **ppSite);
	STDMETHOD(MapUrlToZone)(LPCWSTR pwszUrl, DWORD* pdwZone, DWORD dwFlags);
	STDMETHOD(GetSecurityId)(LPCWSTR pwszUrl, BYTE* pbSecurityId, DWORD* pcbSecurityId, DWORD_PTR dwReserved);
	STDMETHOD(ProcessUrlAction)(LPCWSTR pwszUrl, DWORD dwAction, BYTE* pPolicy, DWORD cbPolicy, BYTE* pContext, DWORD cbContext, DWORD dwFlags, DWORD dwReserved);
	STDMETHOD(QueryCustomPolicy)(LPCWSTR pwszUrl, REFGUID guidKey, BYTE** ppPolicy, DWORD* pcbPolicy, BYTE* pContext, DWORD cbContext, DWORD dwReserved);
	STDMETHOD(SetZoneMapping)(DWORD dwZone, LPCWSTR lpszPattern, DWORD dwFlags);
	STDMETHOD(GetZoneMappings)(DWORD dwZone, IEnumString  **ppenumString, DWORD dwFlags);

	/* IPdnWnd */
	STDMETHOD(setTitle)(BSTR);
	STDMETHOD(setAOT)(BOOL);
	STDMETHOD(minimize)();
	STDMETHOD(maximize)();
	STDMETHOD(restore)();
	STDMETHOD(close)();
	void OnFinalMessage(HWND);
	STDMETHOD(setSize)(DWORD, DWORD);
	STDMETHOD(setPos)(int, int);
	STDMETHOD(setIcon)(BSTR);
	STDMETHOD(hide)(BOOL);
	STDMETHOD(flash)(DWORD);
	STDMETHOD(resizeable)(BOOL);
	STDMETHOD(showMinBox)(BOOL);
	STDMETHOD(showMaxBox)(BOOL);
	STDMETHOD(showTitlebar)(BOOL);
	STDMETHOD(translucent)(DWORD);
	STDMETHOD(animate)(DWORD, DWORD);
	STDMETHOD(Do)(BSTR, VARIANT[], DWORD = 1);
	STDMETHOD(get_params)(VARIANT*);
	STDMETHOD(focus)();
	STDMETHOD(get_left)(VARIANT*);
	STDMETHOD(get_top)(VARIANT*);
	STDMETHOD(get_width)(VARIANT*);
	STDMETHOD(get_height)(VARIANT*);
	STDMETHOD(put_minWidth)(DWORD);
	STDMETHOD(put_minHeight)(DWORD);
	STDMETHOD(isActive)(BOOL*);
	STDMETHOD(get_isMinimized)(BOOL *);
	STDMETHOD(get_isMaximized)(BOOL *);
	STDMETHOD(get_isHidden)(BOOL *);
	STDMETHOD(pushFocus)();
	STDMETHOD(popFocus)();
	STDMETHOD(setMinHandler)(BSTR);
	STDMETHOD(setCloseHandler)(BSTR);
	STDMETHOD(setMenuHandler)(BSTR);
	STDMETHOD(setCmdLineHandler)(BSTR);
	STDMETHOD(setRestoreHandler)(BSTR);
	STDMETHOD(setActivationHandler)(BSTR);
	STDMETHOD(FireEvent)(BSTR , VARIANT[], DWORD);
	STDMETHOD(get_menuBar)(VARIANT*);
	STDMETHOD(messageBox)(BOOL, BSTR, BSTR, DWORD, DWORD*);
	STDMETHOD(get_Handle)(DWORD *pHandle);

	/* IUnknown */
	STDMETHOD(QueryInterface)(const IID &riid, void **ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	///* IDispatch */
	//STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
	//STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
	//STDMETHOD(GetIDsOfNames)(REFIID riid,
	//	LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
	//STDMETHOD(Invoke)(
	//	DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, 
	//	DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, 
	//	UINT* puArgErr);
};