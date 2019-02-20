

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
#include "DxWindow.h"
#include "MCurve.h"
#include "DxListItem.h"
#include "DxColorMap.h"
namespace DxUI{
	class CDxFloatWindow;
	class CDxListWindow;
	class CDxListWindowEx;
	class CDxPushButton;
	class CDxPopWindow;
	class CDxTableWindow;
}

using DxUI::CDxWindow;
using DxUI::MCurve;
using DxUI::MMarker;
using DxUI::MPlotTitle;
using DxUI::DxColor;
using DxUI::DxPointD;
using DxUI::IPainterInterface;
using DxUI::RgbI;
using DxUI::interp;
using DxUI::DXFontInfo;
using DxUI::DXAlignment;
using DxUI::CDxPushButton;
using DxUI::CDxListWindow;
using DxUI::CDxListWindowEx;
using DxUI::CDxFloatWindow;
using DxUI::CDxPopWindow;
using DxUI::CDxTableWindow;

//+---------------------------
//
// ͼ����ͼ��
//
//+-----------------------------
class DXUI_API CLenged : public DxUI::CDxListItem{
	DECLARE_CLASS(CLenged)
public:
	CLenged();
	~CLenged();
	CLenged* CreateThis();
	bool DestroyThis();
	SIZE GetHintSize() const;
	bool OnDrawItem(const RECT& rc, IPainterInterface* painter);
};



//+-----------------------------------------
//
// ��������
// ����ֻ�ǻ��ƶ��Ѹ���marker����
// ��Ҫǿ��Ĺ�����Ҫ�Լ����໯
// ������ߵĿ�ݼ���������
// Ctrl + ����ѡ�ֲ��Ŵ�
// Ctrl + F1 ����ͼ��
// F1  ��ʾͼ��
// Delete ɾ����ǰѡ�� ���ѡ�е���mark ɾ��mark ���ѡ�е������� ɾ������
// Ctrl + A ��ѡ�е����߸������㣬������ʵ��Բ��
// Ctrl + X ��ѡ�е����߸������㣬������X
// Ctrl + S ��ѡ�е����߲�����������ֻ��Ƕ��㣬������ʵ��Բ��
// Ctrl + Z ���ѡ�����ߵĸ���״̬
// Ctrl + D ��ѡ������ʹ��������
// Ctrl + C �������״̬ Ч���� Ctrl + Z
// Ctrl + F ʹѡ�е�mark�������ͷ�����֮���л� ����mark֮��mark���ɱ��ƶ�
// Shit + ���������� �ڿ��������������marker
//
//+------------------------------------------
class DXUI_API MPlotWindowBase :
	public CDxWindow
{
	DECLARE_CLASS(MPlotWindowBase)
public:
	enum NormalMarkerType{
		Marker_Max = 0,
		Marker_Min,
	};

	enum CurveMarkPointType{
		RoundPoint = 0,
		CrossPoint
	};
public:
	MPlotWindowBase();
	~MPlotWindowBase();

	void		PlotLine(const MString& Name, const TL::Vector<double>& x, const TL::Vector<double>& y, const DxColor& col = RgbI(0, 255, 0), int size = 1);
	void		PlotLine(const MString& Name, const TL::Vector<double>& y, const DxColor& col = RgbI(0, 255, 0), int size = 1);
	void		PlotLine(const MString& Name, const TL::Vector<DxPointD>& y, const DxColor& col = RgbI(0, 255, 0), int size = 1);

	//
	// �������
	//
	void		PlotVerticalLine(const MString& Name, double x, const DxColor& col = RgbI(0, 255, 0), int size = 1);


	//
	// ��ָ�����ߺ���׷������
	//
	void		Append(const MString& Name, const TL::Vector<double>& x, const TL::Vector<double>& y);
	void		Append(const MString& Name, const TL::Vector<double>& y);
	void		Append(const MString& Name, double x, double y);
	void		Append(const MString& Name, double y);


	//
	// ����׷��
	//
	void		FastPlotLine(const MString& Name, const TL::Vector<double>& x, const TL::Vector<double>& y, const DxColor& col = RgbI(0, 255, 0), int size = 1);
	void		FastAppend(const MString& Name, double x, double y);
	void		RePlot();

	void		AddMarker(const MString& Name, double x, double y, const DxColor& col = RgbI(0, 0, 0));
	void		AddMarker(const MString& Name, double x, double y, const MString& bitmap);
	void		AddMarkerForCurve(const MString& curveName, const MString& markername, double x, const DxColor& col = RgbI(0, 0, 0));
	void		AddMarkerForCurve(const MString& curveName, const MString& markername, double x, const MString& bitmap);
	void		AddMaxMinMarker(const MString& curveName, const MString& markername, NormalMarkerType type, const DxColor& col = RgbI(0, 0, 0));
	void		AddMaxMinMarker(const MString& curveName, const MString& markername, NormalMarkerType type, const MString& bitmap);
	void		SetMarkerFixEnabel(const MString& markername, bool isFix);  // ��marker����Ϊ�������߲�����
	void		ClearFixMarkers(const MString& curveName); // ���ָ����������������marker

	void		FixHorizonAxis(bool isFix, double minx, double maxx);
	void		FixVerticalAxis(bool isFix, double miny, double maxy);
	void		AutoScale();

	void		RemoveMarker(const MString& Name);
	void		RemoveCurve(const MString& Name);
	void		RemoveVerticalCurve(const MString& Name);
	MCurve*		GetCurve(const MString& Name);
	MMarker*	GetMarker(const MString& Name);
	double		GetVerticalPos(const MString& Name);

	bool		CurveIsExist(const MString&  Name);

	void		Clear();

	void		SetMajorGridColor(const DxColor& col);
	void		SetMinorGridColor(const DxColor& col);
	void		SetLabelTextColor(const DxColor& col);
	void		SetPlotBackGroundColor(const DxColor& col);
	void		SetLabelTextFont(const DXFontInfo& font);

	void		SetTraceMouse(bool isTrace);
	void		SetSelectedMarkerColor(const DxColor& col);
	void		SetSelectedCurveSize(int size);


	//
	// �ö����������
	//
	void		SetHighLightPointable(bool isMarkPoint); //�����ߵĶ����ǳ���
	void		SetHighLightPointable(const MString& Name, bool isMarkPoint); // ��ָ����ĳ�����߽������ǳ���
	void		SetHighLightPointType(const MString& Name, CurveMarkPointType type); // ����ʵ��Բ������ǲ�
	void		SetHighLightPointColor(const MString& Name, const DxColor& col); // ����ʵ��Բ������ǲ�

	//
	// ��������ʱֻ��Ҫ��ǳ����㼴��
	//
	void		SetNotNeedLine(const MString& Name, bool isNoLine);


	//
	// ����������
	//
	void		SetStripLine(const MString& Name, bool isStrip);


	//
	// ����������ɫ�ʹ�ϸ
	//
	void		SetCurveColor(const MString& Name, const DxColor& col);
	void		SetCurveSize(const MString& Name, int size);


	//
	// ���ø�����ʾ��Ϣ
	//
	void		SetAdditionInformation(const MString& Information);
	void		SetShowAdditionTextAble(bool isEnable);
	void		SetAdditionTextAreaColor(const DxColor& col);


	//
	// ���ñ���
	//
	void		SetTitle(const MPlotTitle& title);
	void		SetHTitle(const MPlotTitle& title);
	void		SetVTitle(const MPlotTitle& title);

	MPlotTitle* GetTitle();
	MPlotTitle* GetHTitle();
	MPlotTitle* GetVTitle();

	const MPlotTitle* GetTitle() const;
	const MPlotTitle* GetHTitle() const;
	const MPlotTitle* GetVTitle() const;

	void		SetPlotAreaSizeBox(const RECT& sizeBox);
	void		SetShowMarkerLineEnabel(bool isEnabel);
	void		SetMarkerLineColor(const DxColor& col);
	void		SetMarkerLineIsCross(bool isCross);


	TL::Map<MString, MCurve> GetAllCurves() const;
	TL::Map<MString, MMarker> GetAllMarkers() const;

	
	void		Save(const MString& FileName);
	void		Load(const MString& FileName);

	void		OnDealSelectedChanged(int curindex, int preindex, CDxWidget* sender);
	void		OnRendWindow(IPainterInterface* painter);

	void		LeftMoveMarker(const MString& Name);
	void		RightMoveMarker(const MString& Name);

	TL::Vector<double> GetValuesFromX(double x);


msignals:
	void		SelectedChanged(MString cur, MString pre, CDxWidget* sender);
	void		MarkerPositionChanged(MString Name, double x, double y, CDxWidget* sender);
	void		VerticalLineMove(MString Name, double x,  CDxWidget* sender);
	TL::MTLVoidEvent<MString, MString, CDxWidget*> Event_SelectedChanged;
	TL::MTLVoidEvent<MString, double, double, CDxWidget*> Event_MarkerPositionChanged; // maker �ƶ�ʱ�����¼�
	TL::MTLVoidEvent<MString, double, CDxWidget*>	  Event_VerticalLineMove;  // �����ƶ�ʱ�����¼�
	TL::MTLVoidEvent<MString, MString, CDxWidget*>    Event_CurverRename;      // �������Ʊ��޸�ʱ�����¼�
	TL::MTLVoidEvent<MString, MString, CDxWidget*>	  Event_MarkerRemane;	   // Maer���Ʊ��޸�ʱ�����¼�  
	TL::MTLVoidEvent<MString, CDxWidget*>			  Event_DeleteCurve;	   // ɾ�����߳����¼�
	TL::MTLVoidEvent<MString, CDxWidget*>			  Event_DeleteMarker;	   // ɾ��Marker�㴥���¼�



protected:
	int			GetColorId();

protected:
	void				UpdateChildWindowPos();
	bool				OnNotify(DxUI::DXNotifyUI* NotifyEvent);
	virtual void		OnDealTranslateAndScale();
	virtual void		OnDealTranslateDequeScale();
	virtual void		OnDealPoint(int x, int y);
	virtual void		OnDealMarker(int x, int y);
	virtual bool		OnDealVerticalLine(int x, int y);


protected:
	std::map<MString, MCurve>	mCurves;
	std::map<MString, MMarker>	mMarkers;
	typedef std::vector<DxPointD> CurveDataType;
	typedef TL::MSNoSqlDB<double, double, double, double> ScaleTranslateType;
	std::map<MString, CurveDataType> mNormalData;// ����������
	std::map<MString, CurveDataType> mBakData;   // ���ݱ���
	std::map<MString, bool>			 mShowMap;   // �Ƿ���ʾ������
	std::map<MString, bool>			 mMarkerShowMap; // �Ƿ���ʾ��marker
	std::map<MString, bool>			 mShowMarkPointCurvMap; // ָ�������Ƿ���Ҫ��Ƕ���
	std::map<MString, CurveMarkPointType> mMarkPointTypeMap; // ������������ͣ�ʵ��Բ�� ���� ��
	std::map<MString, DxColor>			  mMarkPointColorMap;// �����������ɫ
	std::map<MString, bool>				  mNotNeedLineCurvMap; // ����Ҫ����������ֻ��Ҫ���Ƴ����㼴��
	std::map<MString, bool>				  mStripLineMap;  // ���ƴ�ֱ����
	std::map<MString, MCurve>			  mVerticalLinesMap; // ����һЩ������

	MPlotTitle					mTitle;
	MPlotTitle					mHTitle;
	MPlotTitle					mVTitle;
	double						mXScaleValue{ 1.0 };
	double						mYScaleValue{ 1.0 };
	double						mXTranslate{ 0.0 };
	double						mYTranslate{ 0.0 };
	ScaleTranslateType			mScalDeque;
	DxColor						mMajorGridColor;
	DxColor						mMinorGridColor;
	DxColor						mAxisLabelColor;
	DxColor						mPlotGroundColor;
	DXFontInfo					mAxisLabelFont;

	double						mMinX;
	double						mMinY;
	double						mMaxX;
	double						mMaxY;

	double						mBacMinX;
	double						mBacMinY;
	double						mBacMaxX;
	double						mBacMaxY;

	bool						bIsFixX{ false };
	bool						bIsFixY{ false };
	double						mFixMinX{ -1000 };
	double						mFixMaxX{ 1000 };
	double						mFixMinY{ -1000 };
	double						mFixMaxY{ 1000 };

	bool						bIsNeedTraceMouse{ false };
	bool						bIsTranslate{ false };
	bool						bIsClicked{ false };
	bool						bIsMarkPoint{ false };// �������ǳ���
	bool						bIsShowMarkerLine{ false }; // ��ʾ����
	bool						bIsCrossShowMarkerLine{ false };
	POINT						mMousePt;
	POINT						mClickedPt;
	POINT						mPrePt;
	DxUI::DxRectD				mBoundRect;


	int							mSelectedSize{ 3 };
	MString						mSelectedCurve;
	MString						mSelectedMarkter;
	MString						mSelectedVerticalLine;
	DxColor						mSelectedMarkerColor;
	DxColor						mMarkerOldColor;
	CDxFloatWindow*				mLengedWindow;
	CDxListWindow*				mListWindow;
	DxUI::CDxColorMap			mColorMapForCurve;
	TL::Vector<int>				mCurveColorIds;

	bool						bIsShowAdditionText{ true };
	MString						m_AdditionTextInformation;  // ���ӵ�һЩ��ʾ��Ϣ
	DxColor						m_AdditionAreaColor;
	DxColor						m_MarkerLineColor;
	DxUI::CDxEffects			m_AdditionTextEffect;
	DxUI::CDxEffects			m_HelpEffects;
	RECT						m_PlotAreaSizeBox;

	DxUI::CDxPopWindow*			p_HelpPopWindow{ nullptr };
};

