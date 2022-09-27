#include <Windows.h>
#include <math.h>
#include "AMCMathServerHeader.h"

/* Declaration of all co-classes */

// CBasicOne
class CBasicOne : public ISum, ISubtract
{
private:
	long m_cRef;
public:
	// Constructor and destructor methods
	CBasicOne(void);
	~CBasicOne(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ISum specific methods
	HRESULT __stdcall SumOfTwoNumbers(float, float, float*);

	// ISubtract specific methods
	HRESULT __stdcall SubtractionOfTwoNumbers(float, float, float*);
};

// CBasicTwo's contained (inner) component
class CBasicTwoInner : public IDivide
{
private:
	long m_cRef;
public:
	// Constructor and destructor methods
	CBasicTwoInner(void);
	~CBasicTwoInner(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IDivide specific methods
	HRESULT __stdcall DivisionOfTwoNumbers(float, float, float*);
};

// CBasicTwo's container (outer) component
class CBasicTwoOuter : public IMultiply, IDivide
{
private:
	long m_cRef;
	IDivide* m_pIDivide;
public:
	// Constructor and destructor methods
	CBasicTwoOuter(void);
	~CBasicTwoOuter(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IMultiply specific methods
	HRESULT __stdcall MultiplicationOfTwoNumbers(float, float, float*);

	// IDivide specific methods
	HRESULT __stdcall DivisionOfTwoNumbers(float, float, float*);

	// CBasicTwoOuter's custom methods
	HRESULT __stdcall InitializeInnerComponent(void);
};

// Internal interface providing unaggregated IUnknown's services
interface INoAggregationIUnknown
{
	virtual HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**) = 0;
	virtual ULONG __stdcall AddRef_NoAggregation(void) = 0;
	virtual ULONG __stdcall Release_NoAggregation(void) = 0;
};

// CPowerOne's inner component in the aggregation
class CPowerOneInner : public INoAggregationIUnknown, ICube
{
private:
	long m_cRef;
	IUnknown* m_pIUnknownOuter;
public:
	// Constructor and destructor methods
	CPowerOneInner(IUnknown*);
	~CPowerOneInner(void);

	// INoAggregationIUnknown specific methods
	HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**);
	ULONG __stdcall AddRef_NoAggregation(void);
	ULONG __stdcall Release_NoAggregation(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ICube specific methods
	HRESULT __stdcall CubeOfNumber(float, float*);
};

// CPowerOne's outer component in the aggregation
class CPowerOneOuter : public ISquare
{
private:
	long m_cRef;
	IUnknown* m_pIUnknownInner;
	ICube* m_pICube;
public:
	// Constructor and destructor methods
	CPowerOneOuter(void);
	~CPowerOneOuter(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ISquare specific methods
	HRESULT __stdcall SquareOfNumber(float, float*);

	// CPowerOne's custom methods
	HRESULT __stdcall InitializeInnerComponent(void);
};

// CPowerTwo
class CPowerTwo : public IPowerTwo
{
private:
	long m_cRef;
	ITypeInfo* m_pITypeInfo;
public:
	// Constructor and destructor methods
	CPowerTwo(void);
	~CPowerTwo(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IDispatch specific methods
	HRESULT __stdcall GetTypeInfoCount(UINT*);
	HRESULT __stdcall GetTypeInfo(UINT, LCID, ITypeInfo**);
	HRESULT __stdcall GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*);
	HRESULT __stdcall Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

	// IPowerTwo specific methods
	HRESULT __stdcall SquareRootOfNumber(float, float*);
	HRESULT __stdcall CubeRootOfNumber(float, float*);

	// Custom methods
	HRESULT __stdcall InitInstance(void);
};


/* Declaration of all Class Factories */

// Class Factory for CBasicOne
class CBasicOneClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	// Constructor and destructor methods
	CBasicOneClassFactory(void);
	~CBasicOneClassFactory(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// Class Factory for CBasicTwoInner
class CBasicTwoInnerComponentClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	// Constructor and destructor methods
	CBasicTwoInnerComponentClassFactory(void);
	~CBasicTwoInnerComponentClassFactory(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// Class Factory for CBasicTwoOuter
class CBasicTwoOuterComponentClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	// Constructor and destructor methods
	CBasicTwoOuterComponentClassFactory(void);
	~CBasicTwoOuterComponentClassFactory(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// Class Factory for CPowerOneInner
class CPowerOneInnerComponentClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	// Constructor and destructor methods
	CPowerOneInnerComponentClassFactory(void);
	~CPowerOneInnerComponentClassFactory(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// Class Factory for CPowerOneOuter
class CPowerOneOuterComponentClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	// Constructor and destructor methods
	CPowerOneOuterComponentClassFactory(void);
	~CPowerOneOuterComponentClassFactory(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

// Class Factory for CPowerTwo
class CPowerTwoClassFactory : public IClassFactory
{
private:
	long m_cRef;
public:
	// Constructor and destructor methods
	CPowerTwoClassFactory(void);
	~CPowerTwoClassFactory(void);

	// IUnknown specific methods
	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// IClassFactory specific methods
	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};

/* Global Variable Declarations */

long glNumberOfActiveComponents = 0l;
long glNumberOfServerLocks = 0l;
HMODULE ghModule;

// LIBID of the TypeLib exported by CPowerTwo: {42841933-00B4-4FF6-9472-7A5310887CE9}
const GUID LIBID_PowerTwo = { 0x42841933, 0xb4, 0x4ff6, 0x94, 0x72, 0x7a, 0x53, 0x10, 0x88, 0x7c, 0xe9 };

/* DLL Entry-Point */
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		ghModule = hDll;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}

	return(TRUE);
}

/* Implementation of CBasicOne */

CBasicOne::CBasicOne(void)
{
	// Code
	m_cRef = 1l;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CBasicOne::~CBasicOne(void)
{
	// Code
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CBasicOne::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<ISum*>(this);
	else if (riid == IID_ISum)
		*ppv = static_cast<ISum*>(this);
	else if (riid == IID_ISubtract)
		*ppv = static_cast<ISubtract*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CBasicOne::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CBasicOne::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CBasicOne::SumOfTwoNumbers(float fNum1, float fNum2, float* fResult)
{
	// Code
	*fResult = fNum1 + fNum2;
	return(S_OK);
}

HRESULT CBasicOne::SubtractionOfTwoNumbers(float fNum1, float fNum2, float* fResult)
{
	// Code
	*fResult = fNum1 - fNum2;
	return(S_OK);
}

/* Implementation of CBasicOneClassFactory */

CBasicOneClassFactory::CBasicOneClassFactory(void)
{
	// Code
	m_cRef = 1l;
}

CBasicOneClassFactory::~CBasicOneClassFactory(void)
{
	// Code
}

HRESULT CBasicOneClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CBasicOneClassFactory::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CBasicOneClassFactory::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0);
	}
	return(m_cRef);
}

HRESULT CBasicOneClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	// Local variable declarations
	HRESULT hr;
	CBasicOne* pCBasicOne = NULL;

	// Code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	pCBasicOne = new CBasicOne;
	if (pCBasicOne == NULL)
		return(E_OUTOFMEMORY);

	hr = pCBasicOne->QueryInterface(riid, ppv);
	
	pCBasicOne->Release();

	return(hr);
}

HRESULT CBasicOneClassFactory::LockServer(BOOL bLock)
{
	if (bLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);	
	return(S_OK);
}

/* Implementation of CBasicTwoInner */

CBasicTwoInner::CBasicTwoInner(void)
{
	// Code
	m_cRef = 1l;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CBasicTwoInner::~CBasicTwoInner(void)
{
	// Code
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CBasicTwoInner::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IDivide*>(this);
	else if (riid == IID_IDivide)
		*ppv = static_cast<IDivide*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CBasicTwoInner::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CBasicTwoInner::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CBasicTwoInner::DivisionOfTwoNumbers(float fNum1, float fNum2, float* fResult)
{
	// Code
	*fResult = fNum1 / fNum2;
	return(S_OK);
}

/* Implementation of CBasicTwoOuter */

CBasicTwoOuter::CBasicTwoOuter(void)
{
	// Code
	m_cRef = 1l;
	m_pIDivide = NULL;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CBasicTwoOuter::~CBasicTwoOuter(void)
{
	// Code
	if (m_pIDivide)
	{
		m_pIDivide->Release();
		m_pIDivide = NULL;
	}
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CBasicTwoOuter::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IMultiply*>(this);
	else if (riid == IID_IMultiply)
		*ppv = static_cast<IMultiply*>(this);
	else if (riid == IID_IDivide)
		*ppv = static_cast<IDivide*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CBasicTwoOuter::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CBasicTwoOuter::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CBasicTwoOuter::MultiplicationOfTwoNumbers(float fNum1, float fNum2, float* fResult)
{
	// Code
	*fResult = fNum1 * fNum2;
	return(S_OK);
}

HRESULT CBasicTwoOuter::DivisionOfTwoNumbers(float fNum1, float fNum2, float* fResult)
{
	// Code
	if (fNum2 == 0.0f)
	{
		*fResult = 0.0f;
		MessageBox(NULL,
			TEXT("DivisionOfTwoNumbers(): cannot divide by Zero"),
			TEXT("AMCMathServer Runtime Exception"),
			MB_OK | MB_ICONERROR);
		return(E_INVALIDARG);
	}
	
	return(m_pIDivide->DivisionOfTwoNumbers(fNum1, fNum2, fResult));
}

HRESULT CBasicTwoOuter::InitializeInnerComponent(void)
{
	// Local variable declarations
	HRESULT hr;
	
	// Code
	hr = CoCreateInstance(CLSID_BasicTwoInner,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDivide,
		(void**)&m_pIDivide);

	return(hr);
}

/* Implementation of CBasicTwoInnerComponentClassFactory */

CBasicTwoInnerComponentClassFactory::CBasicTwoInnerComponentClassFactory(void)
{
	// Code
	m_cRef = 1l;
}

CBasicTwoInnerComponentClassFactory::~CBasicTwoInnerComponentClassFactory(void)
{
	// Code
}

HRESULT CBasicTwoInnerComponentClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CBasicTwoInnerComponentClassFactory::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CBasicTwoInnerComponentClassFactory::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CBasicTwoInnerComponentClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	// Local variable declarations
	HRESULT hr;
	CBasicTwoInner* pCBasicTwoInner;

	// Code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	pCBasicTwoInner = new CBasicTwoInner;
	if (pCBasicTwoInner == NULL)
		return(E_OUTOFMEMORY);

	hr = pCBasicTwoInner->QueryInterface(riid, ppv);

	pCBasicTwoInner->Release();

	return(hr);
}

HRESULT CBasicTwoInnerComponentClassFactory::LockServer(BOOL bLock)
{
	// Code
	if (bLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

/* Implementation of CBasicTwoOuterComponentClassFactory */

CBasicTwoOuterComponentClassFactory::CBasicTwoOuterComponentClassFactory(void)
{
	// Code
	m_cRef = 1l;
}

CBasicTwoOuterComponentClassFactory::~CBasicTwoOuterComponentClassFactory(void)
{
	// Code
}

HRESULT CBasicTwoOuterComponentClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CBasicTwoOuterComponentClassFactory::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(S_OK);
}

ULONG CBasicTwoOuterComponentClassFactory::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CBasicTwoOuterComponentClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	// Local variable declarations
	HRESULT hr;
	CBasicTwoOuter* pCBasicTwoOuter;
	TCHAR str[255];

	// Code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	pCBasicTwoOuter = new CBasicTwoOuter;
	if (pCBasicTwoOuter == NULL)
		return(E_OUTOFMEMORY);

	hr = pCBasicTwoOuter->InitializeInnerComponent();
	if (FAILED(hr))
	{
		wsprintf(str, "CBasicTwoOuter::InitializeInnerComponent(): failed with error code 0x%x", hr);
		MessageBox(NULL, str, TEXT("COM Error in AMCMath Server"), MB_OK | MB_ICONERROR);

		pCBasicTwoOuter->Release();
		return(hr);
	}

	hr = pCBasicTwoOuter->QueryInterface(riid, ppv);

	pCBasicTwoOuter->Release();

	return(hr);
}

HRESULT CBasicTwoOuterComponentClassFactory::LockServer(BOOL bLock)
{
	// Code
	if (bLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

/* Implementation of CPowerOneInner */

CPowerOneInner::CPowerOneInner(IUnknown* pIUnknownOuter)
{
	// Code
	if (pIUnknownOuter != NULL)
		m_pIUnknownOuter = pIUnknownOuter;
	else
		m_pIUnknownOuter = reinterpret_cast<IUnknown*>(static_cast<INoAggregationIUnknown*>(this));

	m_cRef = 1l;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CPowerOneInner::~CPowerOneInner(void)
{
	// Code
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CPowerOneInner::QueryInterface_NoAggregation(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<INoAggregationIUnknown*>(this);
	else if (riid == IID_ICube)
		*ppv = static_cast<ICube*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CPowerOneInner::AddRef_NoAggregation(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CPowerOneInner::Release_NoAggregation(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CPowerOneInner::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	return(m_pIUnknownOuter->QueryInterface(riid, ppv));
}

ULONG CPowerOneInner::AddRef(void)
{
	// Code
	return(m_pIUnknownOuter->AddRef());
}

ULONG CPowerOneInner::Release(void)
{
	// Code
	return(m_pIUnknownOuter->Release());
}

HRESULT CPowerOneInner::CubeOfNumber(float fNum, float* fResult)
{
	*fResult = fNum * fNum * fNum;
	return(S_OK);
}

/* Implementation of CPowerOneOuter */

CPowerOneOuter::CPowerOneOuter(void)
{
	// Code
	m_pIUnknownInner = NULL;
	m_pICube = NULL;
	m_cRef = 1l;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CPowerOneOuter::~CPowerOneOuter(void)
{
	// Code
	if (m_pICube)
	{
		m_pICube->Release();
		m_pICube = NULL;
	}
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CPowerOneOuter::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<ISquare*>(this);
	else if (riid == IID_ISquare)
		*ppv = static_cast<ISquare*>(this);
	else if (riid == IID_ICube)
		return(m_pIUnknownInner->QueryInterface(riid, ppv));
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CPowerOneOuter::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CPowerOneOuter::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CPowerOneOuter::SquareOfNumber(float fNum, float* fResult)
{
	// Code
	*fResult = fNum * fNum;
	return(S_OK);
}

HRESULT CPowerOneOuter::InitializeInnerComponent(void)
{
	// Local variable declarations
	HRESULT hr;

	// Code
	hr = CoCreateInstance(CLSID_PowerOneInner,
		reinterpret_cast<IUnknown*>(this),
		CLSCTX_INPROC_SERVER,
		IID_IUnknown,
		(void**)&m_pIUnknownInner);
	if (FAILED(hr))
		return(hr);

	hr = m_pIUnknownInner->QueryInterface(IID_ICube, (void**)&m_pICube);
	if (FAILED(hr))
	{
		m_pIUnknownInner->Release();
		m_pIUnknownInner = NULL;
	}

	return(hr);
}

/* Implementation of CPowerOneInnerComponentClassFactory */

CPowerOneInnerComponentClassFactory::CPowerOneInnerComponentClassFactory(void)
{
	// Code
	m_cRef = 1l;
}

CPowerOneInnerComponentClassFactory::~CPowerOneInnerComponentClassFactory(void)
{
	// Code
}

HRESULT CPowerOneInnerComponentClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CPowerOneInnerComponentClassFactory::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CPowerOneInnerComponentClassFactory::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CPowerOneInnerComponentClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	// Local variable declarations
	HRESULT hr;
	CPowerOneInner* pCPowerOneInner = NULL;

	// Code
	if (pUnkOuter != NULL && riid != IID_IUnknown)
		return(CLASS_E_NOAGGREGATION);

	pCPowerOneInner = new CPowerOneInner(pUnkOuter);
	if (pCPowerOneInner == NULL)
		return(E_OUTOFMEMORY);

	hr = pCPowerOneInner->QueryInterface_NoAggregation(riid, ppv);

	pCPowerOneInner->Release_NoAggregation();

	return(hr);
}

HRESULT CPowerOneInnerComponentClassFactory::LockServer(BOOL bLock)
{
	// Code
	if (bLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

/* Implementation of CPowerOneOuterComponentClassFactory */

CPowerOneOuterComponentClassFactory::CPowerOneOuterComponentClassFactory(void)
{
	// Code
	m_cRef = 1l;
}

CPowerOneOuterComponentClassFactory::~CPowerOneOuterComponentClassFactory(void)
{
	// Code
}

HRESULT CPowerOneOuterComponentClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CPowerOneOuterComponentClassFactory::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CPowerOneOuterComponentClassFactory::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CPowerOneOuterComponentClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	// Local variable declarations
	HRESULT hr;
	CPowerOneOuter* pCPowerOneOuter = NULL;
	TCHAR str[255];

	// Code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	pCPowerOneOuter = new CPowerOneOuter;
	if (pCPowerOneOuter == NULL)
		return(E_OUTOFMEMORY);

	hr = pCPowerOneOuter->InitializeInnerComponent();
	if (FAILED(hr))
	{
		wsprintf(str, TEXT("CPowerOneOuter::InitializeInnerComponent(): failed with error code 0x%x"), hr);
		MessageBox(NULL, str, TEXT("AMCMathServer Runtime Exception"), MB_OK | MB_ICONERROR);

		pCPowerOneOuter->Release();
		return(hr);
	}

	hr = pCPowerOneOuter->QueryInterface(riid, ppv);

	pCPowerOneOuter->Release();

	return(hr);
}

HRESULT CPowerOneOuterComponentClassFactory::LockServer(BOOL bLock)
{
	// Code
	if (bLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

/* Implementation of CPowerTwo */

CPowerTwo::CPowerTwo(void)
{
	// Code
	m_cRef = 1l;
	m_pITypeInfo = NULL;
	InterlockedIncrement(&glNumberOfActiveComponents);
}

CPowerTwo::~CPowerTwo(void)
{
	// Code
	if (m_pITypeInfo)
	{
		m_pITypeInfo->Release();
		m_pITypeInfo = NULL;
	}
	InterlockedDecrement(&glNumberOfActiveComponents);
}

HRESULT CPowerTwo::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IPowerTwo*>(this);
	else if (riid == IID_IDispatch)
		*ppv = static_cast<IPowerTwo*>(this);
	else if (riid == IID_IPowerTwo)
		*ppv = static_cast<IPowerTwo*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CPowerTwo::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CPowerTwo::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CPowerTwo::GetTypeInfoCount(UINT* pcTypeInfo)
{
	// Code
	*pcTypeInfo = 1;
	return(S_OK);
}

HRESULT CPowerTwo::GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo** ppITypeInfo)
{
	// Code
	*ppITypeInfo = NULL;
	
	if (iTypeInfo != 0)
		return(DISP_E_BADINDEX);

	m_pITypeInfo->AddRef();
	*ppITypeInfo = m_pITypeInfo;
	return(S_OK);
}

HRESULT CPowerTwo::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
{
	// Code
	return(DispGetIDsOfNames(m_pITypeInfo, rgszNames, cNames, rgDispId));
}

HRESULT CPowerTwo::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgError)
{
	// Local variable declarations
	HRESULT hr;

	// Code
	hr = DispInvoke(this,
		m_pITypeInfo,
		dispIdMember,
		wFlags,
		pDispParams,
		pVarResult,
		pExcepInfo,
		puArgError);
	return(hr);
}

HRESULT CPowerTwo::SquareRootOfNumber(float fNum, float* fResult)
{
	// Code
	if (fNum < 0.0f)
	{
		MessageBox(NULL,
			TEXT("SquareRootOfNumber(): square roots of negative numbers are not supported"),
			TEXT("AMCMathServer Runtime Exception"),
			MB_OK | MB_ICONERROR);
		return(E_INVALIDARG);
	}

	*fResult = sqrtf(fNum);
	return(S_OK);
}

HRESULT CPowerTwo::CubeRootOfNumber(float fNum, float* fResult)
{
	// Code
	*fResult = cbrtf(fNum);
	return(S_OK);
}

HRESULT CPowerTwo::InitInstance(void)
{
	// Local function declarations
	void DescribeDisplayComError(HWND, HRESULT);

	// Local variable declarations
	HRESULT hr;
	ITypeLib* pITypeLib = NULL;

	// Code
	if (m_pITypeInfo == NULL)
	{
		hr = LoadRegTypeLib(LIBID_PowerTwo,
			1, 0,
			0x00,
			&pITypeLib);
		if (FAILED(hr))
		{
			DescribeDisplayComError(NULL, hr);
			return(hr);
		}

		hr = pITypeLib->GetTypeInfoOfGuid(IID_IPowerTwo, &m_pITypeInfo);
		if (FAILED(hr))
		{
			DescribeDisplayComError(NULL, hr);
			pITypeLib->Release();
			pITypeLib = NULL;
			return(hr);
		}

		pITypeLib->Release();
		pITypeLib = NULL;
	}
	return(S_OK);
}

/* Implementation of CPowerTwoClassFactory */

CPowerTwoClassFactory::CPowerTwoClassFactory(void)
{
	// Code
	m_cRef = 1l;
}

CPowerTwoClassFactory::~CPowerTwoClassFactory(void)
{
	// Code
}

HRESULT CPowerTwoClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	// Code
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();

	return(S_OK);
}

ULONG CPowerTwoClassFactory::AddRef(void)
{
	// Code
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CPowerTwoClassFactory::Release(void)
{
	// Code
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0l)
	{
		delete(this);
		return(0l);
	}
	return(m_cRef);
}

HRESULT CPowerTwoClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	// Local variable declarations
	HRESULT hr;
	CPowerTwo* pCPowerTwo;
	TCHAR str[255];

	// Code
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);

	pCPowerTwo = new CPowerTwo;
	if (pCPowerTwo == NULL)
		return(E_OUTOFMEMORY);

	hr = pCPowerTwo->InitInstance();
	if (FAILED(hr))
	{
		wsprintf(str, TEXT("InitInstance(): failed with error code 0x%x"), hr);
		MessageBox(NULL, str, TEXT("AMCMathServer Runtime Error"), MB_OK | MB_ICONERROR);
		pCPowerTwo->Release();
		return(hr);
	}

	hr = pCPowerTwo->QueryInterface(riid, ppv);

	pCPowerTwo->Release();

	return(hr);
}

HRESULT CPowerTwoClassFactory::LockServer(BOOL bLock)
{
	// Code
	if (bLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	return(S_OK);
}

/* Implementation of all Locally Declared Functions */

void DescribeDisplayComError(HWND hwnd, HRESULT hr)
{
	// Local variable declarations
	TCHAR* szErrorMsg = NULL;
	TCHAR str[255];

	// Code
	if (HRESULT_FACILITY(hr) == FACILITY_WINDOWS)
		hr = HRESULT_CODE(hr);

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&szErrorMsg,
		0,
		NULL))
	{
		wsprintf(str, TEXT("0x%x: %s"), hr, szErrorMsg);
		LocalFree(szErrorMsg);
	}
	else
		wsprintf(str, TEXT("Description for error code 0x%x couldn't be found"), hr);

	MessageBox(NULL, str, TEXT("AMCMathServer Runtime Error"), MB_OK | MB_ICONERROR);
}

/* Implementation of Functions Exported Privately by this DLL */

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	// Local variable declarations
	HRESULT hr;
	IClassFactory* pClassFactory = NULL;

	// Code
	if (rclsid == CLSID_BasicOne)
		pClassFactory = new CBasicOneClassFactory;
	else if (rclsid == CLSID_BasicTwoInner)
		pClassFactory = new CBasicTwoInnerComponentClassFactory;
	else if (rclsid == CLSID_BasicTwoOuter)
		pClassFactory = new CBasicTwoOuterComponentClassFactory;
	else if (rclsid == CLSID_PowerOneInner)
		pClassFactory = new CPowerOneInnerComponentClassFactory;
	else if (rclsid == CLSID_PowerOneOuter)
		pClassFactory = new CPowerOneOuterComponentClassFactory;
	else if (rclsid == CLSID_PowerTwo)
		pClassFactory = new CPowerTwoClassFactory;
	else
		return(CLASS_E_CLASSNOTAVAILABLE);

	if (pClassFactory == NULL)
		return(E_OUTOFMEMORY);

	hr = pClassFactory->QueryInterface(riid, ppv);

	pClassFactory->Release();

	return(hr);
}

extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
	// Code
	if (glNumberOfActiveComponents == 0l && glNumberOfServerLocks == 0l)
		return(S_OK);
	else
		return(S_FALSE);
}

extern "C" HRESULT __stdcall DllRegisterServer(void)
{
	// Local variable declarations
	HKEY hClsidKey_BasicOne = NULL, hInProcSvrKey_BasicOne = NULL;
	HKEY hClsidKey_BasicTwoInner = NULL, hInProcSvrKey_BasicTwoInner = NULL;
	HKEY hClsidKey_BasicTwoOuter = NULL, hInProcSvrKey_BasicTwoOuter = NULL;
	HKEY hClsidKey_PowerOneInner = NULL, hInProcSvrKey_PowerOneInner = NULL;
	HKEY hClsidKey_PowerOneOuter = NULL, hInProcSvrKey_PowerOneOuter = NULL;
	
	HKEY hClsidKey_PowerTwo = NULL, hInProcSvrKey_PowerTwo = NULL;
	HKEY hClsidTypeLibKey_PowerTwo = NULL, hTypeLibKey_PowerTwo = NULL, hTypeLibVersionKey_PowerTwo = NULL, hTypeLibVersionIndexKey_PowerTwo = NULL;
	HKEY hInterfaceKey_PowerTwo = NULL, hInterfaceTypeLibKey_PowerTwo = NULL, hInterfacePSOAInterface32Key_PowerTwo = NULL, hInterfacePSOAInterfaceKey_PowerTwo = NULL;
	
	TCHAR szModulePath[MAX_PATH];
	TCHAR szTypeLibPath_PowerTwo[] = TEXT("C:\\Windows\\System32\\AMCMathServerTypeLib.tlb");
	
	TCHAR szCBasicOneDescription[] = TEXT("Component BasicOne: performs summation and subtraction for AMCMathServer");
	TCHAR szCBasicTwoInnerDescription[] = TEXT("");
	TCHAR szCBasicTwoOuterDescription[] = TEXT("Component BasicTwo: performs multiplication and division for AMCMathServer");
	TCHAR szCPowerOneInnerDescription[] = TEXT("");
	TCHAR szCPowerOneOuterDescription[] = TEXT("Component PowerOne: calculates squares and cubes for AMCMathServer");

	TCHAR szCPowerTwoDescription[] = TEXT("Component PowerTwo: calculates square roots and cube roots for AMCMathServer");
	TCHAR szCPowerTwoTypeLibDescription[] = TEXT("Type Library for component PowerTwo of AMCMathServer");
	TCHAR szCPowerTwoTypeLibVersionDescription[] = TEXT("PowerTwo_64bit v1.0");
	TCHAR szLIBID_PowerTwo[] = TEXT("{42841933-00B4-4FF6-9472-7A5310887CE9}");
	TCHAR szIID_PSOAInterface[] = TEXT("{00020424-0000-0000-C000-000000000046}");
	
	TCHAR szThreadingModel[] = TEXT("Apartment");
	LONG lRet;

	// Code
	__try
	{
		// Create non-volatile registry keys under HCR\CLSID for co-classes implemented by this DLL
		//
		// BasicOne
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("CLSID\\{15C22B88-EEE9-41B9-8198-F038030795E8}"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE | KEY_CREATE_SUB_KEY,
			NULL,
			&hClsidKey_BasicOne,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoInner
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("CLSID\\{AF8CEF58-23ED-4651-8600-17FC413D4920}"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE | KEY_CREATE_SUB_KEY,
			NULL,
			&hClsidKey_BasicTwoInner,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoOuter
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("CLSID\\{652C69BA-F0E0-49D6-881B-EBC7A15FE774}"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE | KEY_CREATE_SUB_KEY,
			NULL,
			&hClsidKey_BasicTwoOuter,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneInner
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("CLSID\\{4C860900-6812-4631-85F0-5485449AEDB4}"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE | KEY_CREATE_SUB_KEY,
			NULL,
			&hClsidKey_PowerOneInner,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneOuter
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("CLSID\\{BFD69753-8141-4001-B065-B1366CE17E03}"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE | KEY_CREATE_SUB_KEY,
			NULL,
			&hClsidKey_PowerOneOuter,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerTwo
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("CLSID\\{8CD2023C-EA47-4DE2-8EE6-579C1BD892AD}"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE | KEY_CREATE_SUB_KEY,
			NULL,
			&hClsidKey_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// These keys hold a human-readable form of their respective classes' description
		//
		// BasicOne
		lRet = RegSetValueEx(hClsidKey_BasicOne,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szCBasicOneDescription,
			sizeof(szCBasicOneDescription));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoInner
		lRet = RegSetValueEx(hClsidKey_BasicTwoInner,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szCBasicTwoInnerDescription,
			sizeof(szCBasicTwoInnerDescription));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoOuter
		lRet = RegSetValueEx(hClsidKey_BasicTwoOuter,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szCBasicTwoOuterDescription,
			sizeof(szCBasicTwoOuterDescription));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneInner
		lRet = RegSetValueEx(hClsidKey_PowerOneInner,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szCPowerOneInnerDescription,
			sizeof(szCPowerOneInnerDescription));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneOuter
		lRet = RegSetValueEx(hClsidKey_PowerOneOuter,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szCPowerOneOuterDescription,
			sizeof(szCPowerOneOuterDescription));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerTwo
		lRet = RegSetValueEx(hClsidKey_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szCPowerTwoDescription,
			sizeof(szCPowerTwoDescription));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// Create a non-volatile 'InprocServer32' subkey under each of the above created keys
		//
		// BasicOne
		lRet = RegCreateKeyEx(hClsidKey_BasicOne,
			TEXT("InprocServer32"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hInProcSvrKey_BasicOne,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoInner
		lRet = RegCreateKeyEx(hClsidKey_BasicTwoInner,
			TEXT("InprocServer32"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hInProcSvrKey_BasicTwoInner,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoOuter
		lRet = RegCreateKeyEx(hClsidKey_BasicTwoOuter,
			TEXT("InprocServer32"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hInProcSvrKey_BasicTwoOuter,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneInner
		lRet = RegCreateKeyEx(hClsidKey_PowerOneInner,
			TEXT("InprocServer32"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hInProcSvrKey_PowerOneInner,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneOuter
		lRet = RegCreateKeyEx(hClsidKey_PowerOneOuter,
			TEXT("InprocServer32"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hInProcSvrKey_PowerOneOuter,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerTwo
		lRet = RegCreateKeyEx(hClsidKey_PowerTwo,
			TEXT("InprocServer32"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hInProcSvrKey_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// Get this DLL's absolute path into szModulePath
		GetModuleFileName(ghModule, szModulePath, MAX_PATH);
		
		// 1. The InprocServer32 subkey, by default, holds the complete path to the server DLL; and
		//
		// BasicOne
		lRet = RegSetValueEx(hInProcSvrKey_BasicOne,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szModulePath,
			sizeof(TCHAR) * (lstrlen(szModulePath) + 1));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoInner
		lRet = RegSetValueEx(hInProcSvrKey_BasicTwoInner,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szModulePath,
			sizeof(TCHAR) * (lstrlen(szModulePath) + 1));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoOuter
		lRet = RegSetValueEx(hInProcSvrKey_BasicTwoOuter,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szModulePath,
			sizeof(TCHAR) * (lstrlen(szModulePath) + 1));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneInner
		lRet = RegSetValueEx(hInProcSvrKey_PowerOneInner,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szModulePath,
			sizeof(TCHAR) * (lstrlen(szModulePath) + 1));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneOuter
		lRet = RegSetValueEx(hInProcSvrKey_PowerOneOuter,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szModulePath,
			sizeof(TCHAR) * (lstrlen(szModulePath) + 1));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerTwo
		lRet = RegSetValueEx(hInProcSvrKey_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szModulePath,
			sizeof(TCHAR) * (lstrlen(szModulePath) + 1));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// 2. The threading model (this is explicit, though, and not the keys' default value)
		//
		// BasicOne
		lRet = RegSetValueEx(hInProcSvrKey_BasicOne,
			TEXT("ThreadingModel"),
			0,
			REG_SZ,
			(const BYTE*)szThreadingModel,
			sizeof(szThreadingModel));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoInner
		lRet = RegSetValueEx(hInProcSvrKey_BasicTwoInner,
			TEXT("ThreadingModel"),
			0,
			REG_SZ,
			(const BYTE*)szThreadingModel,
			sizeof(szThreadingModel));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// BasicTwoOuter
		lRet = RegSetValueEx(hInProcSvrKey_BasicTwoOuter,
			TEXT("ThreadingModel"),
			0,
			REG_SZ,
			(const BYTE*)szThreadingModel,
			sizeof(szThreadingModel));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneInner
		lRet = RegSetValueEx(hInProcSvrKey_PowerOneInner,
			TEXT("ThreadingModel"),
			0,
			REG_SZ,
			(const BYTE*)szThreadingModel,
			sizeof(szThreadingModel));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerOneOuter
		lRet = RegSetValueEx(hInProcSvrKey_PowerOneOuter,
			TEXT("ThreadingModel"),
			0,
			REG_SZ,
			(const BYTE*)szThreadingModel,
			sizeof(szThreadingModel));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// PowerTwo
		lRet = RegSetValueEx(hInProcSvrKey_PowerTwo,
			TEXT("ThreadingModel"),
			0,
			REG_SZ,
			(const BYTE*)szThreadingModel,
			sizeof(szThreadingModel));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// Registry entries required for components in this DLL exposing IDispatch
		//
		// PowerTwo
		//
		// [HKEY_CLASSES_ROOT\CLSID\{CLSID}\TypeLib]
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("CLSID\\{8CD2023C-EA47-4DE2-8EE6-579C1BD892AD}\\TypeLib"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hClsidTypeLibKey_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// @ = {LIBID}
		lRet = RegSetValueEx(hClsidTypeLibKey_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szLIBID_PowerTwo,
			sizeof(szLIBID_PowerTwo));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// [HKEY_CLASSES_ROOT\TypeLib\{LIBID}]
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("TypeLib\\{42841933-00B4-4FF6-9472-7A5310887CE9}"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE | KEY_CREATE_SUB_KEY,
			NULL,
			&hTypeLibKey_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// @ = "Type Library for ..."
		lRet = RegSetValueEx(hTypeLibKey_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szCPowerTwoTypeLibDescription,
			sizeof(szCPowerTwoTypeLibDescription));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// [HKEY_CLASSES_ROOT\TypeLib\{LIBID}\1.0]
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("TypeLib\\{42841933-00B4-4FF6-9472-7A5310887CE9}\\1.0"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE | KEY_CREATE_SUB_KEY,
			NULL,
			&hTypeLibVersionKey_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// @ = "Name_64bit 1.0"
		lRet = RegSetValueEx(hTypeLibVersionKey_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szCPowerTwoTypeLibVersionDescription,
			sizeof(szCPowerTwoTypeLibVersionDescription));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// [HKEY_CLASSES_ROOT\TypeLib\{LIBID}\1.0\0\win32]
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("TypeLib\\{42841933-00B4-4FF6-9472-7A5310887CE9}\\1.0\\0\\win32"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hTypeLibVersionIndexKey_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// @ = "Path\To\TypeLib.tlb"
		lRet = RegSetValueEx(hTypeLibVersionIndexKey_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szTypeLibPath_PowerTwo,
			sizeof(szTypeLibPath_PowerTwo));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// [HKEY_CLASSES_ROOT\Interface\{IID}]
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("Interface\\{FACE6818-66A2-47A3-96F6-6D270CBAA508}"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE | KEY_CREATE_SUB_KEY,
			NULL,
			&hInterfaceKey_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// @ = "IName"
		lRet = RegSetValueEx(hInterfaceKey_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)TEXT("IPowerTwo"),
			sizeof(TEXT("IPowerTwo")));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// [HKEY_CLASSES_ROOT\Interface\{IID}\TypeLib]
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("Interface\\{FACE6818-66A2-47A3-96F6-6D270CBAA508}\\TypeLib"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hInterfaceTypeLibKey_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// @ = "{LIBID}"
		lRet = RegSetValueEx(hInterfaceTypeLibKey_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szLIBID_PowerTwo,
			sizeof(szLIBID_PowerTwo));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// [HKEY_CLASSES_ROOT\Interface\{IID}\ProxyStubClsid32]
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("Interface\\{FACE6818-66A2-47A3-96F6-6D270CBAA508}\\ProxyStubClsid32"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hInterfacePSOAInterface32Key_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// @ = "{IID_PSOAInterface}"
		lRet = RegSetValueEx(hInterfacePSOAInterface32Key_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szIID_PSOAInterface,
			sizeof(szIID_PSOAInterface));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// [HKEY_CLASSES_ROOT\Interface\{IID}\ProxyStubClsid]
		lRet = RegCreateKeyEx(HKEY_CLASSES_ROOT,
			TEXT("Interface\\{FACE6818-66A2-47A3-96F6-6D270CBAA508}\\ProxyStubClsid"),
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_SET_VALUE,
			NULL,
			&hInterfacePSOAInterfaceKey_PowerTwo,
			NULL);
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));

		// @ = "{IID_PSOAInterface}"
		lRet = RegSetValueEx(hInterfacePSOAInterfaceKey_PowerTwo,
			NULL,
			0,
			REG_SZ,
			(const BYTE*)szIID_PSOAInterface,
			sizeof(szIID_PSOAInterface));
		if (lRet != ERROR_SUCCESS)
			return(HRESULT_FROM_WIN32(lRet));
	}
	__finally
	{
		if (hClsidKey_BasicOne)
			RegCloseKey(hClsidKey_BasicOne);
		if (hInProcSvrKey_BasicOne)
			RegCloseKey(hInProcSvrKey_BasicOne);
		if (hClsidKey_BasicTwoInner)
			RegCloseKey(hClsidKey_BasicTwoInner);
		if (hInProcSvrKey_BasicTwoInner)
			RegCloseKey(hInProcSvrKey_BasicTwoInner);
		if (hClsidKey_BasicTwoOuter)
			RegCloseKey(hClsidKey_BasicTwoOuter);
		if (hInProcSvrKey_BasicTwoOuter)
			RegCloseKey(hInProcSvrKey_BasicTwoOuter);
		if (hClsidKey_PowerOneInner)
			RegCloseKey(hClsidKey_PowerOneInner);
		if (hInProcSvrKey_PowerOneInner)
			RegCloseKey(hInProcSvrKey_PowerOneInner);
		if (hClsidKey_PowerOneOuter)
			RegCloseKey(hClsidKey_PowerOneOuter);
		if (hInProcSvrKey_PowerOneOuter)
			RegCloseKey(hInProcSvrKey_PowerOneOuter);
		if (hClsidKey_PowerTwo)
			RegCloseKey(hClsidKey_PowerTwo);
		if (hInProcSvrKey_PowerTwo)
			RegCloseKey(hInProcSvrKey_PowerTwo);
		if (hClsidTypeLibKey_PowerTwo)
			RegCloseKey(hClsidTypeLibKey_PowerTwo);
		if (hTypeLibKey_PowerTwo)
			RegCloseKey(hTypeLibKey_PowerTwo);
		if (hTypeLibVersionKey_PowerTwo)
			RegCloseKey(hTypeLibVersionKey_PowerTwo);
		if (hTypeLibVersionIndexKey_PowerTwo)
			RegCloseKey(hTypeLibVersionIndexKey_PowerTwo);
		if (hInterfaceKey_PowerTwo)
			RegCloseKey(hInterfaceKey_PowerTwo);
		if (hInterfaceTypeLibKey_PowerTwo)
			RegCloseKey(hInterfaceTypeLibKey_PowerTwo);
		if (hInterfacePSOAInterface32Key_PowerTwo)
			RegCloseKey(hInterfacePSOAInterface32Key_PowerTwo);
		if (hInterfacePSOAInterfaceKey_PowerTwo)
			RegCloseKey(hInterfacePSOAInterfaceKey_PowerTwo);
	}

	return(S_OK);
}

extern "C" HRESULT __stdcall DllUnregisterServer(void)
{
	// Code
	// Deleting all BasicOne's keys
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{15C22B88-EEE9-41B9-8198-F038030795E8}\\InprocServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{15C22B88-EEE9-41B9-8198-F038030795E8}"));

	// Deleting all BasicTwoOuter's keys
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{652C69BA-F0E0-49D6-881B-EBC7A15FE774}\\InprocServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{652C69BA-F0E0-49D6-881B-EBC7A15FE774}"));

	// Deleting all BasicTwoInner's keys
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{AF8CEF58-23ED-4651-8600-17FC413D4920}\\InprocServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{AF8CEF58-23ED-4651-8600-17FC413D4920}"));

	// Deleting all PowerOneInner's keys
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{4C860900-6812-4631-85F0-5485449AEDB4}\\InprocServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{4C860900-6812-4631-85F0-5485449AEDB4}"));

	// Deleting all PowerOneOuter's keys
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{BFD69753-8141-4001-B065-B1366CE17E03}\\InprocServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{BFD69753-8141-4001-B065-B1366CE17E03}"));

	// Deleting PowerTwo's keys of form HKCR\Interface\{IID}\* (inclusive of {IID})
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Interface\\{FACE6818-66A2-47A3-96F6-6D270CBAA508}\\ProxyStubClsid"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Interface\\{FACE6818-66A2-47A3-96F6-6D270CBAA508}\\ProxyStubClsid32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Interface\\{FACE6818-66A2-47A3-96F6-6D270CBAA508}\\TypeLib"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Interface\\{FACE6818-66A2-47A3-96F6-6D270CBAA508}"));

	// Deleting PowerTwo's keys of form HKCR\CLSID\{CLSID}\* (inclusive of {CLSID})
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{8CD2023C-EA47-4DE2-8EE6-579C1BD892AD}\\TypeLib"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{8CD2023C-EA47-4DE2-8EE6-579C1BD892AD}\\InprocServer32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\{8CD2023C-EA47-4DE2-8EE6-579C1BD892AD}"));

	// Deleting PowerTwo's keys of form HKCR\TypeLib\{LIBID}\* (inclusive of {LIBID})
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\{42841933-00B4-4FF6-9472-7A5310887CE9}\\1.0\\0\\win32"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\{42841933-00B4-4FF6-9472-7A5310887CE9}\\1.0"));
	RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\{42841933-00B4-4FF6-9472-7A5310887CE9}"));

	return(S_OK);
}
