
///////////////////////////////////////////////////////////////////////////////
//
// This source file is part of the MDXUI source distribution
// Copyright (C) 2017-2018 by Mengjin (sh06155@hotmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __MAPI_DEBUG__
#define DXUI_API 
#else
#if defined(DIRECTUI_EXPORTS)
#define DXUI_API __declspec(dllexport)
#else
#define DXUI_API __declspec(dllimport)
#endif
#endif 

//+----------------------------------------+
//
// DX ��Ⱦ�������һЩͷ�ļ�
//
//+----------------------------------------+
#include <D3D11.h>
#include <D3DX11.h>
#include <DxErr.h>
#include <d3dx11effect.h>
#include <D3Dcompiler.h>
#include <xnamath.h>
#include <functional>
#include <D3D10_1.h>
#include <DXGI.h>
#include <D2D1.h>
#include <DWrite.h>
#include <wincodec.h>
#include <MToolLib>
#include <MBox.h>
#include <HMath.h>
#include <olectl.h>
#include "DxSize.h"
#include <assert.h>
#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#pragma warning(disable:4505)
#pragma warning(disable:4251)
#pragma warning(disable:4189)
#pragma warning(disable:4121)
#pragma warning(disable:4100)
#pragma warning (disable : 4511) // copy operator could not be generated
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
#pragma warning (disable : 4786) // identifier was truncated
#pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WINXP//0x0501
#endif

#ifndef dx_min
#define dx_min(x,y) (((x) <= (y)) ? (x) : (y))
#endif

#ifndef dx_max
#define dx_max(x,y) (((x) >= (y)) ? (x) : (y))
#endif

#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX dx_max
#define MIN dx_min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))

#ifndef DxTRACE
#define DxTRACE DxTrace
#endif


/************************************************************************/
/* �ú�ɶ�DX���ص�HRֵ������֤											*/
/************************************************************************/
#define V_RETURN(hr)  { if( FAILED(hr) ) {  DXTraceW( __FILE__, (DWORD)__LINE__, hr, L#hr, true ); } }
#define dxhr_return(hr) V_RETURN(hr)

/************************************************************************/
/* �򵥵��ض���                                                         */
/************************************************************************/
#define dx_file __FILE__
#define dx_function __FUNCTION__
#define dx_line __LINE__


/************************************************************************/
/* ���ָ�����Ƿ񱻰���                                                 */
/************************************************************************/
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

/************************************************************************/
/* ���弸���¼�
/* DX_BLINKMSG_TIMER ������������˸�Ķ�ʱ
/* DX_KILLBLINK_TIMER �ر���������˸��ʱ��
/* DX_PAINTER_EVENT ǿ�ƽ����ػ�
/* DX_QUEUE_EVENT ��Ϣ�����¼�
/* DX_APPMANAGER_SERIAL_EVENT һ�������¼�
/* DX_RUNNOARGRFUN_EVENT ִ���޲κ����¼�
/* Dx_ASYNC_RUN_EVENT �첽ִ�жԻ����¼�
/* DX_IDLE_EVENT IDLE�¼�
/************************************************************************/
#define DX_BLINKMSG_TIMER  WM_USER + 10000
#define DX_KILLBLINK_TIMER WM_USER + 10001
#define DX_PAINTER_EVENT WM_USER + 10002
#define DX_QUEUE_EVENT WM_USER + 10003
#define DX_APPMANAGER_SERIAL_EVENT WM_USER + 10004
#define DX_RUNNOARGRFUN_EVENT WM_USER + 10005
#define Dx_ASYNC_RUN_EVENT WM_USER + 10006
#define DX_IDLE_EVENT WM_USER + 10007


//+----------------------------
//
// ��ʽʱʹ�õĴ�ӡ����
//
//+----------------------------
template<class T,class...Args>
void  DxTrace(LPCTSTR pstrFormat,const T& value, const Args&...args){
	MString str = MString::Format(pstrFormat,value, args...);
	DxTrace(str);
}


extern "C" DXUI_API void  DxTrace(const MString& str);


namespace DxUI{
	

	//
	// �����ź�ͼ�����ִ���
	//
	struct DXUI_API CDxSignalMath{

		//
		// fft �任
		//
		static TL::Vector<std::complex<double>>  fft(const TL::Vector<double>& indata, int num);
		static TL::Vector<std::complex<double>>  fft(const TL::Vector<std::complex<double>>& indata, int num);
		static mj::cx_dmat  fft(const mj::cx_dmat& indata, int row, int col,int N = 3);
		static mj::cx_dmat  fft(const mj::dmat& indata, int row, int col, int N = 3);

		//
		// Nά����Ҷ�任
		//
		static TL::Vector<mj::cx_dmat> fft(const TL::Vector<mj::dmat>& indata, int row = -1, int col = -1, int dim = 1);
		static TL::Vector<mj::cx_dmat> fft(const TL::Vector<mj::cx_dmat>& indata, int row = -1, int col = -1, int dim = 1);


		//
		// ifft �任
		//
		static TL::Vector<std::complex<double>>  ifft(const TL::Vector<std::complex<double>>& indata, int num);
		static TL::Vector<std::complex<double>>  ifft(const TL::Vector<double>& indata, int num);
		static mj::cx_dmat  ifft(const mj::cx_dmat& indata, int row, int col, int N = 3);
		static mj::cx_dmat  ifft(const mj::dmat& indata, int row, int col, int N = 3);


		//
		// N ά����Ҷ��任
		//
		static TL::Vector<mj::cx_dmat> ifft(const TL::Vector<mj::dmat>& indata, int row = -1, int col = -1, int dim = 1);
		static TL::Vector<mj::cx_dmat> ifft(const TL::Vector<mj::cx_dmat>& indata, int row = -1, int col = -1, int dim =1);


		//
		// ���
		//
		static TL::Vector<std::complex<double>>  conv(const TL::Vector<std::complex<double>>& left, const TL::Vector<std::complex<double>>& right, bool issame = false);
		static TL::Vector<double>  conv(const TL::Vector<double>& left, const TL::Vector<double>& right, bool issame = false);
		static mj::cx_dmat  conv(const mj::cx_dmat& left, const mj::cx_dmat& right, bool issame = false);
		static mj::dmat  conv(const mj::dmat& left, const mj::dmat& right, bool issame = false);


		//
		// ������
		//
		static mj::cx_dmat  mult(const mj::cx_dmat& left, const mj::cx_dmat& right);
		static mj::dmat  mult(const mj::dmat& left, const mj::dmat& right);

		//
		// ��������
		//
		static mj::cx_dmat  inv(const mj::cx_dmat& m);
		static mj::dmat  inv(const mj::dmat& m);

		//
		// ����������ʽ
		//
		static double det(const mj::dmat& m);
		static std::complex<double> det(const mj::cx_dmat& m);


		//
		// ������ת dim = 0������ת 1 ����ת 3 �ݺ���ת
		//

		static mj::dmat ifftshift(const mj::dmat& m, int dim = 0);
		static mj::cx_dmat ifftshift(const mj::cx_dmat& m, int dim = 0);


		//
		// ������ת dim = 0������ת 1 ����ת
		//
		static mj::dmat shift(const mj::dmat& m, int N,int dim = 0);
		static mj::cx_dmat shift(const mj::cx_dmat& m, int N,int dim = 0);

		static TL::Vector<double> shift(const TL::Vector<double>& m, int N);
		static TL::Vector<std::complex<double>> shift(const TL::Vector<std::complex<double>>& m, int N);


		//
		// fliplr(): generate a copy of matrix X, with the order of the columns reversed 
		// flipud(): generate a copy of matrix X, with the order of the rows reversed 
		//
		static mj::dmat fliplr(const mj::dmat& m);
		static mj::cx_dmat fliplr(const mj::cx_dmat& m);
		static mj::dmat flipud(const mj::dmat& m);
		static mj::cx_dmat flipud(const mj::cx_dmat& m);


		//
		// ����һ�������б�
		//
		static TL::Vector<double> linspace(double start, double end, int N);


		//
		// ���������Ƿ�����Ч����
		//
		static bool has_inf(const mj::dmat& m);
		static bool has_nan(const mj::dmat& m);


		//
		// ��������������� dim = 0 ���� ���´������� 3 �ݺ�����
		//
		static mj::dmat shuffle(const mj::dmat& m, int dim = 0);
		static mj::cx_dmat shuffle(const mj::cx_dmat& m, int dim = 0);

		//
		// ���Բ�ֵ
		//
		static TL::Vector<double> linear_interp(const TL::Vector<double>& xs, const TL::Vector<double>& ys, const TL::Vector<double>& disx);

		//
		// ����������ֵ
		//
		static TL::Vector<double> spline_interp(const TL::Vector<double>& xs, const TL::Vector<double>& ys, const TL::Vector<double>& disx);


		//
		// ��ά��ֵ
		// ���ݾ��������ʹ��
		// ʹ������������ֵ
		//
		static mj::dmat spline_interp2(
			const TL::Vector<double>& xs,
			const TL::Vector<double>& ys,
			const mj::dmat& datas,
			const mj::dmat& disx,
			const TL::Vector<double>& disy);

		static mj::dmat spline_interp2(
			const TL::Vector<double>& xs,
			const mj::dmat& ys,
			const mj::dmat& datas,
			const TL::Vector<double>& disx,
			const TL::Vector<double>& disy);

		static mj::dmat spline_interp2(
			const TL::Vector<double>& xs,
			const TL::Vector<double>& ys,
			const mj::dmat& datas,
			const TL::Vector<double>& disx,
			const TL::Vector<double>& disy);

		static mj::cx_dmat spline_interp2_cx(
			const TL::Vector<double>& xs,
			const TL::Vector<double>& ys,
			const mj::cx_dmat& datas,
			const mj::dmat& disx,
			const TL::Vector<double>& disy);


		//
		// FFT ��ֵ
		// 
		//  fre = ����Ƶ��;
		//  c = 3e8;
		//  lamda = c / fre;
		//  d = lamda * 2 / pi%�벨����Ԫ���
		//	beta = 2 * pi / lamda;
		//  N = 16;
		//  n = (N - 1)*d*beta + 8;
		//  delta_phi = 180 / n;
		//  �������ϲ������� ����ʹ��fft��ֵ�õ���׼�ĸ߾��� ֵ
		//
		static TL::Vector<double> fftinterp(const TL::Vector<double>& xs,int Npts);
		static TL::Vector<std::complex<double>> fftinterp(const TL::Vector<std::complex<double>>& xs, int Npts);
	};
}



//+-----------------------------------------
// 
// �ͷ�com���
//
//+-----------------------------------------
#ifndef safe_release
#define safe_release(x)		 { if(x) { (x)->Release(); (x) = 0;	} }
#endif


//+----------------------------------------
//
// ����Ч
//
//+----------------------------------------
#ifndef null_ptr
#define null_ptr(x) x = nullptr
#endif

//+----------------------------------------
//
// ��ȫɾ��һ��ָ��
//
//+----------------------------------------
#ifndef safe_delete
#define safe_delete(x)		{	if(x){delete (x);(x) = 0;}}
#endif



//+----------------------------------------
//
// �����˸�Ķ�ʱ��
//
//+----------------------------------------
#ifndef CARET_TIMERID
#define CARET_TIMERID 65536
#endif

//+-----------------------------------------------------
//
// ����һЩ����״̬
//
//+------------------------------------------------------
namespace DxUI{

	//+---------------------------
	// 
	// ���弸����������
	//
	//+--------------------------
#ifndef uint
#define uint unsigned int
#endif

#ifndef ushort
#define ushort unsigned short
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef ulong
#define ulong unsigned long
#endif

	//+---------------------------
	//
	// DxColor
	//
	//+---------------------------
	union DxColor
	{
		struct RGB{
			unsigned char a;
			unsigned char b;
			unsigned char g;
			unsigned char r;
		} rgb;
		unsigned long lc{ 0xFFFFFFFF };
	};

	inline DxColor RgbI(uchar r, uchar g, uchar b, uchar a = 255){
		DxColor col;
		col.rgb.r = r;
		col.rgb.g = g;
		col.rgb.b = b;
		col.rgb.a = a;
		return col;
	}

	inline DxColor Color(unsigned long lc){
		DxColor col;
		col.lc = lc;
		return col;
	}

	//+---------------------------
	//
	// ת����D2D
	//
	//+----------------------------
	inline D2D1_COLOR_F ToD2DColor(const DxColor& col){
		return D2D1::ColorF(double(int(col.rgb.r)) / 255.0, double(int(col.rgb.g)) / 255.0, double(int(col.rgb.b)) / 255.0, double(int(col.rgb.a)) / 255.0);
	}


	//+----------------------------
	//
	// ���弸������2D��Ⱦ�ļ�����ɫ
	//
	//+-----------------------------
	namespace MDXColor{
		inline static DxColor white(){		return RgbI(255, 255, 255); } //��ɫ
		inline static DxColor black(){		return RgbI(0, 0, 0); }     //��ɫ
		inline static DxColor red() {		return RgbI(255, 0, 0); }    //��ɫ
		inline static DxColor green(){		return RgbI(0, 255, 0); }  //��ɫ
		inline static DxColor blue(){		return RgbI(0, 0, 255); }  //��ɫ
		inline static DxColor yellow(){		return RgbI(255, 255, 0); } //��ɫ
		inline static DxColor cyan(){		return RgbI(0, 255, 255); }
		inline static DxColor magenta(){	return RgbI(255, 0, 255); }
		inline static DxColor silver(){		return RgbI(192, 192, 192); }
		inline static DxColor deepgreen(){	return RgbI(41, 158, 112); }
	}
	


	//+-----------------------------
	//
	// ���ڵı������
	// Dx_ImageBack �����������һ��ͼƬ
	// Dx_SolidColorBack ��ɫ���
	// Dx_LinearGradientBack ���Խ������
	// Dx_RadiaGradientBack ���佥�����
	// Dx_None �����
	// �����ʱ����͸��
	//
	//+-----------------------------
	enum DXBackGroundType{
		Dx_ImageBack = 0,
		Dx_SolidColorBack,
		Dx_LinearGradientBack,
		Dx_RadiaGradientBack,
		Dx_NoneBack
	};



	//+--------------------------
	//
	// ��ǰ����������״̬
	//
	//+--------------------------
	enum DXWindowStatus{
		Dx_Normal = 0,
		Dx_Clicked,
		Dx_Hover,
		Dx_Disable
	};


	//+-------------------------------
	//
	// ��갴��
	//
	//+-------------------------------
	enum DXMouseButton{
		Dx_LeftButton = 0,   //������
		Dx_WheelButton,       //����м�
		Dx_RightButton,       //����Ҽ�
		Dx_NoMouseButton       //û����
	};



	//+-----------------------------------
	//
	// ��궯��
	//
	//------------------------------------
	enum DXMouseAction{
		Dx_ButtonDown = 0,		// ��갴��
		Dx_ButtonMove,			// ����ƶ�
		Dx_ButtonUp,			// ����ƶ�
		Dx_ButtonDoubleClick,	// ���˫��
		Dx_ButtonHover			// �������
	};


	//+------------------------------------
	//
	// �ı����뷽ʽ
	//
	//+------------------------------------
	enum DXTextAlignment{
		Dx_AlignNormal = 0,   // �������
		Dx_AlignLeft,
		Dx_AlignTop,
		Dx_AlignRight,
		Dx_AlignBottom,
		Dx_AlignCenter,
		Dx_AlignLeftTop,
		Dx_AlignLeftBottom,
		Dx_AlignLeftCenter,
		Dx_AlignRightCenter,
		Dx_AlignTopCenter,
		Dx_AlignBottomCenter,
		Dx_AlignBottomRight,
	};

	typedef DXTextAlignment DXAlignment;


	//+------------------------------------------
	//
	// �������͵�����
	//
	//+------------------------------------------
	enum DXWindowType{
		Dx_NormalWindow = 0,    // ��ͨ����
		Dx_FloatWindow,			// ��������
		Dx_DialogWindow,		// �Ի��򴰿�
		Dx_DrawnWindow,         // �Ի洰��
		Dx_PopWindow,			// ��������
		Dx_TopWindow,			// ���㴰��
		Dx_StackedWindow,		// ��ջ����
		Dx_PushButton,			// ��ť
		Dx_StatusButton,		// ״̬��ť
		Dx_FlatButton,			// ���ǰ�ť
		Dx_RadioButton,			// ��ѡ��ť
		Dx_CheckBox,			// ��ѡ��ť
		Dx_Label,				// ��ǩ
		Dx_LineEdit,			// ���������
		Dx_EditWindow,			// �ı���
		Dx_ComboBox,			// ������
		Dx_GroupBox,			// ��Ͽ�
		Dx_SpinBox,				// ΢����
		Dx_ListWindow,			// �б�
		Dx_TableWindow,			// ���
		Dx_TableView,			// ����ͼ
		Dx_ListView,			// �б��
		Dx_TreeView,			// ������
		Dx_MultTreeView,        // ���е�����
		Dx_HScrollBar,			// ˮƽ������
		Dx_VScrollBar,			// ��ֱ������
		Dx_HScrollSlider,		// ˮƽ������
		Dx_VScrollSlider,		// ��ֱ������
		Dx_Chunker,				// ����
		Dx_TableItem,			// table ����
		Dx_TableHeader,         // table ��ͷ
		Dx_ListItem,			// �б���
		Dx_TreeItem,			// ���ṹ��
		Dx_Layout,				// ���ֹ�����
		Dx_HProgressBar,
		Dx_VProgressBar,
		Dx_ActionItem,
		Dx_MenuItem,
		Dx_MainWindow,//������,һ��Ӧ�ó���ֻ����һ��
	};


	//+----------------------------------------
	//
	// ����
	//
	//+----------------------------------------
	enum DXDirection{
		Dx_Vertical,
		Dx_Horizontal,
		Dx_Lean
	};

	//+--------------------------
	//
	// ��ά�б�ʶ��
	//
	//+---------------------------
	enum MDx11Axis{
		MDx11XAxis = 0,
		MDx11YAxis,
		MDx11ZAxis
	};


	//+-----------------------------------------
	//
	// ���� UI ���¼�
	//
	//+-----------------------------------------
	enum DXEventType{
		Dx_None_Event = 0,
		Dx_Hover_Event,
		Dx_LBClicked_Event,
		Dx_LBDClicked_Event,
		Dx_RBClicked_Event,
		Dx_ButtonUp_Event,
		Dx_MBDown_Event,
		Dx_MBUp_Event,
		Dx_ContextMenu_Event,
		Dx_WheelUp_Event,
		Dx_WheelDown_Event,
		Dx_Char_Event,
		Dx_KeyDown_Event,
		Dx_KeyUp_Event,
		Dx_SetCursor_Event,
		Dx_SetFocus_Event,
		Dx_MouseEnter_Event,
		Dx_MouseEleve_Event,
		Dx_MouseMove_Event,
		Dx_MouseMoveOutofRange_Event
	};



	class CDxWidget;
	struct DXNotifyUI
	{
		CDxWidget*	pSender;
		DXEventType	mType;
		POINT		ptMouse;
		WPARAM		wParam;
		LPARAM		lParam;
		HWND		hHwnd;
	};


	//+-------------------------
	//
	// ֪ͨ�ӿ�
	//
	//+-------------------------
	class INotifyUI{
	public:
		virtual bool OnNotify(DXNotifyUI* NotifyEvent) = 0;
	};



	//+-----------------------
	//
	// �ı����»���
	//
	//+-----------------------
	enum CDxUnderlineType
	{
		Dx_None = 0,
		Dx_Single = 1, // �����»���
		Dx_Double = 2, // �����»���
		Dx_Triple = 3, // �����»���
		Dx_Squiggly   // ������
	};


	//+-------------------------
	//
	// ѡ���ı�ʱ��ʾѡ�еĸ߶�
	//
	//+-------------------------
	enum  CDxBackgroundMode
	{
		Dx_TextHeight = 0,
		Dx_TextHeightWithLineGap,
		Dx_LineHeight
	};



	//+--------------------------
	//
	// �����ı��Ű�
	//
	//+-------------------------
	struct DXLineTextInfo{
		uint	mStartIndex;
		uint	mLength;
		double	mHeight;
		double  mBaseLine;
	};

	struct DXDrawTextInfo{
		uint	mIndex;
		float	mX;
		float	mY;
		float	mHeight;
		float	mWidth;
		bool	bIsTrailingHit{ false };
	};


	//+-------------------
	//
	// ������Ϣ
	//
	//+-------------------
	struct DXFontInfo{
		MString  mFontName{ "΢���ź�" };
		int		 mSize{ 14 };
		bool	 bIsBold{ false };
		bool	 bIsItalic{ false };
	};


	//+------------------------------------------------------------------
	//
	// Ԥ����һЩ��ɫ
	// ����3D��Ⱦ
	//
	//+------------------------------------------------------------------
	const static XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };  //��ɫ
	const static XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };  //��ɫ
	const static XMVECTORF32 Red =   { 1.0f, 0.0f, 0.0f, 1.0f };    //��ɫ
	const static XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };  //��ɫ
	const static XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };   //��ɫ
	const static XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f }; //��ɫ
	const static XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	const static XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	const static XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };


	//+-------------------------------------------------------------
	// 
	// Ԥ���弸��С���� �ڱ�Ҫʱ�������
	//
	//+-------------------------------------------------------------
	template<class T>
	inline T dot(T u, T v){
		return ((u).x * (v).x + (u).y * (v).y + (u).z*(v).z);
	}

	template<class T>
	inline T norm2(T v){
		return dot(v, v);
	}

	template<class T>
	inline T norm(T v){
		return std::sqrt(norm2(v));
	}

	template<class T>
	inline T distance(T u, T v){
		return norm(u - v);
	}

	template<class T>
	T interp(T x1, T y1, T x2, T y2, T x3){
		if (std::abs(x1 - x2) < 1e-12){
			return y1;
		}
		if (std::abs(y1 - y2) < 1e-12){
			return y1;
		}
		return (y2 - y1) / (x2 - x1)*(x3 - x2) + y2;
	}



	//+-------------------------------------------
	//
	// һ���������ݽṹ
	// �����㣬���ߣ�������������
	//
	//+--------------------------------------------
	struct BasicPNT
	{
		BasicPNT() : Pos(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), Tex(0.0f, 0.0f) {}
		BasicPNT(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT2& uv)
			: Pos(p), Normal(n), Tex(uv) {}
		BasicPNT(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
			: Pos(px, py, pz), Normal(nx, ny, nz), Tex(u, v) {}
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};



	//+-------------------------------------------
	//
	// һ���������ݽṹ
	// ������
	//
	//+--------------------------------------------
	struct BasicPoint{
		BasicPoint()
		:Pos(0.f, 0.f, 0.f){}
		BasicPoint(const XMFLOAT3& p)
			: Pos(p) {}
		BasicPoint(float px, float py, float pz)
			: Pos(px, py, pz){}
		XMFLOAT3 Pos;
	};

	static D3D11_INPUT_ELEMENT_DESC PNTLayout[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	static D3D11_INPUT_ELEMENT_DESC PointLayout[1] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};



	//+-------------------------------------------
	//
	// һ���������ݽṹ
	// �����㣬��ɫ
	//
	//+--------------------------------------------
	struct BasicPC :public BasicPoint{
		BasicPC()
		: BasicPoint(), Color(1.f, 1.f, 1.f, 1.f) {}
		BasicPC(const XMFLOAT3& p, const XMFLOAT4& color)
			: BasicPoint(p), Color(color) {}
		BasicPC(float px, float py, float pz, float r, float g, float b, float a)
			: BasicPoint(px, py, pz), Color(r, g, b, a) {}
		XMFLOAT4 Color;
	};

	static D3D11_INPUT_ELEMENT_DESC PCLayout[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};



	//+-------------------------------------------
	//
	// һ���������ݽṹ
	// �����㣬��ɫ
	//
	//+--------------------------------------------
	struct BasicPT :public BasicPoint{
		BasicPT()
		: BasicPoint(), Tex(0.f, 0.f) {}
		BasicPT(const XMFLOAT3& p, const XMFLOAT2& tex)
			: BasicPoint(p), Tex(tex) {}
		BasicPT(float px, float py, float pz, float r, float i)
			: BasicPoint(px, py, pz), Tex(r, i) {}
		XMFLOAT2 Tex;
	};

	static D3D11_INPUT_ELEMENT_DESC PTLayout[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};



	//+----------------------------------------------------------
	//
	// ���㣬��ɫ������
	//
	//+----------------------------------------------------------
	struct BasicPCT : public BasicPC{
		BasicPCT()
		: BasicPC(), Tex(0.f, 0.f){}
		BasicPCT(const XMFLOAT3& p, const XMFLOAT4& color, const XMFLOAT2& tex)
			: BasicPC(p, color), Tex(tex) {}
		BasicPCT(float px, float py, float pz, float r, float g, float b, float a, float u, float v)
			: BasicPC(px, py, pz, r, g, b, a), Tex(u, v){}
		XMFLOAT2 Tex;
	};

	static D3D11_INPUT_ELEMENT_DESC PCTLayout[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};




	//+-------------------------------------------
	//
	// һ���������ݽṹ
	// �����㣬���ߣ���ɫ����������
	//
	//+--------------------------------------------
	struct BasicPNC{
		BasicPNC()
		: Pos(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), Color(1.f, 1.f, 1.f, 1.f) {}
		BasicPNC(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT4& color)
			: Pos(p), Normal(n), Color(color) {}
		BasicPNC(float px, float py, float pz, float nx, float ny, float nz, float r, float g, float b, float a)
			: Pos(px, py, pz), Normal(nx, ny, nz), Color(r, g, b, a) {}
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT4 Color;
	};

	static D3D11_INPUT_ELEMENT_DESC PNCLayout[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};




	//+-------------------------------------------
	//
	// һ���������ݽṹ
	// �����㣬���ߣ���ɫ����������
	//
	//+--------------------------------------------
	struct BasicPNTC{
		BasicPNTC()
		: Pos(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), Tex(0.0f, 0.0f), Color(1.f, 1.f, 1.f, 1.f) {}
		BasicPNTC(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT2& tex, const XMFLOAT4& color)
			: Pos(p), Normal(n), Tex(tex), Color(color) {}
		BasicPNTC(float px, float py, float pz, float nx, float ny, float nz, float u, float v, float r, float g, float b, float a)
			: Pos(px, py, pz), Normal(nx, ny, nz), Tex(u, v), Color(r, g, b, a) {}
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT4 Color;
	};

	static D3D11_INPUT_ELEMENT_DESC PNTCLayout[4] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

}// namespace DxUI


extern "C" namespace DxUI{

	//+-------------------------------
	//
	// �Ҷ�ֵת��RGB
	//
	//+---------------------------------
	DXUI_API float Hue2RGB(float v1, float v2, float vH);

	//+---------------------------
	//
	// DxColor ת��ΪHSL
	//
	//+----------------------------
	DXUI_API void DxColor2HSL(const DxColor& clr, double& pHue, double& pSat, double& pLue);

	//+-----------------------------
	//
	// HSL ת��Ϊ DxColor
	//
	//+-----------------------------
	DXUI_API DxColor HSL2DxColor(double H, double S, double L);
}



//+------------------------------------------
//
// �򵥸����������ڶ�DX������д�ӡ
//
//+------------------------------------------
static inline std::ostream& operator<<(std::ostream& os, const XMMATRIX& m){
	os << "\n[" << std::endl;
	os << m._11 << "," << m._12 << "," << m._13 << "," << m._14 << std::endl;
	os << m._21 << "," << m._22 << "," << m._23 << "," << m._24 << std::endl;
	os << m._31 << "," << m._32 << "," << m._33 << "," << m._34 << std::endl;
	os << m._41 << "," << m._42 << "," << m._43 << "," << m._44 << std::endl;
	os << "]" << std::endl;
	return os;
}

static inline std::ostream& operator<<(std::ostream& os, const XMFLOAT4X4& m){
	os << "\n[" << std::endl;
	os << m._11 << "," << m._12 << "," << m._13 << "," << m._14 << std::endl;
	os << m._21 << "," << m._22 << "," << m._23 << "," << m._24 << std::endl;
	os << m._31 << "," << m._32 << "," << m._33 << "," << m._34 << std::endl;
	os << m._41 << "," << m._42 << "," << m._43 << "," << m._44 << std::endl;
	os << "]" << std::endl;
	return os;
}

static inline std::wostream& operator<<(std::wostream& os, const XMMATRIX& m){
	os << L"\n[" << std::endl;
	os << m._11 << L"," << m._12 << L"," << m._13 << L"," << m._14 << std::endl;
	os << m._21 << L"," << m._22 << L"," << m._23 << L"," << m._24 << std::endl;
	os << m._31 << L"," << m._32 << L"," << m._33 << L"," << m._34 << std::endl;
	os << m._41 << L"," << m._42 << L"," << m._43 << L"," << m._44 << std::endl;
	os << L"]" << std::endl;
	return os;
}

static inline std::wostream& operator<<(std::wostream& os, const XMFLOAT4X4& m){
	os << L"\n[" << std::endl;
	os << m._11 << L"," << m._12 << L"," << m._13 << L"," << m._14 << std::endl;
	os << m._21 << L"," << m._22 << L"," << m._23 << L"," << m._24 << std::endl;
	os << m._31 << L"," << m._32 << L"," << m._33 << L"," << m._34 << std::endl;
	os << m._41 << L"," << m._42 << L"," << m._43 << L"," << m._44 << std::endl;
	os << L"]" << std::endl;
	return os;
}
//==================================================================================///












//+----------------------------------------------------
//
// �Զ�����lib
//
//+----------------------------------------------------
#ifndef DIRECTUI_EXPORTS
#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"MDXUId.lib")
#else
#pragma comment(lib,"MDXUI.lib")
#endif // _DEBUG

#else 

#ifdef _DEBUG
#pragma comment(lib,"MDXUId_x86.lib")
#else
#pragma comment(lib,"MDXUI_x86.lib")
#endif // _DEBUG 

#endif // _WIN64


#endif // DIRECTUI_EXPORTS