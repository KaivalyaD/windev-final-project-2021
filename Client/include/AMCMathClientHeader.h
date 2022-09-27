#pragma once
#include <Windows.h>

/* Declaration of all interfaces */

class ISum : public IUnknown
{
public:
	virtual HRESULT __stdcall SumOfTwoNumbers(float, float, float*) = 0;
};

class ISubtract : public IUnknown
{
public:
	virtual HRESULT __stdcall SubtractionOfTwoNumbers(float, float, float*) = 0;
};

class IMultiply : public IUnknown
{
public:
	virtual HRESULT __stdcall MultiplicationOfTwoNumbers(float, float, float*) = 0;
};

class IDivide : public IUnknown
{
public:
	virtual HRESULT __stdcall DivisionOfTwoNumbers(float, float, float*) = 0;
};

class ISquare : public IUnknown
{
public:
	virtual HRESULT __stdcall SquareOfNumber(float, float*) = 0;
};

class ICube : public IUnknown
{
public:
	virtual HRESULT __stdcall CubeOfNumber(float, float*) = 0;
};

class IPowerTwo : public IDispatch
{
public:
	virtual HRESULT __stdcall SquareRootOfNumber(float, float*) = 0;
	virtual HRESULT __stdcall CubeRootOfNumber(float, float*) = 0;
};

/* Declaration and initialization of CLSIDs and IIDs for all component classes and interfaces */

// CLSID of co-class CBasicOne: {15C22B88-EEE9-41B9-8198-F038030795E8}
const CLSID CLSID_BasicOne = { 0x15c22b88, 0xeee9, 0x41b9, 0x81, 0x98, 0xf0, 0x38, 0x3, 0x7, 0x95, 0xe8 };

// CLSID of co-class component for CBasicTwo: {652C69BA-F0E0-49D6-881B-EBC7A15FE774}
const CLSID CLSID_BasicTwo = { 0x652c69ba, 0xf0e0, 0x49d6, 0x88, 0x1b, 0xeb, 0xc7, 0xa1, 0x5f, 0xe7, 0x74 };

// CLSID of co-class component for CPowerOne: {BFD69753-8141-4001-B065-B1366CE17E03}
const CLSID CLSID_PowerOne = { 0xbfd69753, 0x8141, 0x4001, 0xb0, 0x65, 0xb1, 0x36, 0x6c, 0xe1, 0x7e, 0x3 };

// CLSID of co-class CPowerTwo: {8CD2023C-EA47-4DE2-8EE6-579C1BD892AD}
const CLSID CLSID_PowerTwo = { 0x8cd2023c, 0xea47, 0x4de2, 0x8e, 0xe6, 0x57, 0x9c, 0x1b, 0xd8, 0x92, 0xad };



// IID of interface ISum: {C69DB150-EE63-4B3A-B55F-15E365F0911F}
const IID IID_ISum = { 0xc69db150, 0xee63, 0x4b3a, 0xb5, 0x5f, 0x15, 0xe3, 0x65, 0xf0, 0x91, 0x1f };

// IID of interface ISubtract: {A65B822E-8052-43AE-B12D-95581E0BB288}
const IID IID_ISubtract = { 0xa65b822e, 0x8052, 0x43ae, 0xb1, 0x2d, 0x95, 0x58, 0x1e, 0xb, 0xb2, 0x88 };

// IID of interface IMultiply: {E0554960-6BB5-4E5D-8B45-D081EF3F1649}
const IID IID_IMultiply = { 0xe0554960, 0x6bb5, 0x4e5d, 0x8b, 0x45, 0xd0, 0x81, 0xef, 0x3f, 0x16, 0x49 };

// IID of interface IDivide: {62E03A2C-E1B4-4232-8FE5-E428E035326A}
const IID IID_IDivide = { 0x62e03a2c, 0xe1b4, 0x4232, 0x8f, 0xe5, 0xe4, 0x28, 0xe0, 0x35, 0x32, 0x6a };

// IID of interface ISquare: {F4DCA18C-495B-4C9D-9EFD-088ED09DAF30}
const IID IID_ISquare = { 0xf4dca18c, 0x495b, 0x4c9d, 0x9e, 0xfd, 0x8, 0x8e, 0xd0, 0x9d, 0xaf, 0x30 };

// IID of interface ICube: {F72B1D0D-3717-45B2-BF78-1137E72CF415}
const IID IID_ICube = { 0xf72b1d0d, 0x3717, 0x45b2, 0xbf, 0x78, 0x11, 0x37, 0xe7, 0x2c, 0xf4, 0x15 };

// IID of interface IPowerTwo: {FACE6818-66A2-47A3-96F6-6D270CBAA508}
const IID IID_IPowerTwo = { 0xface6818, 0x66a2, 0x47a3, 0x96, 0xf6, 0x6d, 0x27, 0xc, 0xba, 0xa5, 0x8 };
