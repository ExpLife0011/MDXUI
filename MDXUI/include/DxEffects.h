

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
#include "DxConfig.h"

namespace DxUI{

	class CDxWidget;

	//+--------------------------
	//
	// CDxEffects Ч����Ϣ
	//
	//+-------------------------
	class DXUI_API CDxEffects
	{
	public:

		//+-------------------
		//
		// �ı�ѡ����Ϣ
		//
		//+-------------------
		struct TextSelectedInfo{
			int		 mStart{ 0 };
			int		 mLen{ 0 };
			DxColor	 mBackColor;
			DxColor	 mFrontColor;
		};

		//+------------------
		//
		// �ı�����
		//
		//+------------------
		struct TextFontInfo : public DXFontInfo{
			int		 mStart{ 0 };
			int		 mLen{ 0 };
		};


		//+--------------------
		//
		// �ı���ɫ
		//
		//+---------------------
		struct TextColorInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor mColor;
		};


		//+----------------
		//
		// �����С
		//
		//+----------------
		struct TextSizeInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			int		mSize{ 0 };
		};


		//+--------------------------
		//
		// �»���
		//
		//+---------------------------
		struct TextUnderLinerInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor	mColor;
			CDxUnderlineType mType{ Dx_Single };
		};


		//+---------------------
		//
		// �ϻ���
		//
		//+----------------------
		struct TextOverLinerInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor	mColor;
		};

		//+---------------------
		//
		// ����
		//
		//+----------------------
		struct TextHighLightInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor	mColor;
		};

		//+-----------------------
		//
		// �л���
		// 
		//+-----------------------
		struct TextStrikethroughInfo{
			int		mStart{ 0 };
			int		mLen{ 0 };
			DxColor	mColor;
			int		mCounts{ 1 };
		};


		//+--------------
		//
		// ����ɫ����
		// ����
		// ������
		//
		//+--------------
		enum GradientType{
			Dx_Linear = 0,
			Dx_Radia,
			Dx_Double // ��ˮƽ����һ�飬�ٴ�ֱ����һ�飬����
		};


		//+----------------
		//
		// ��Ч����
		// ͼ��
		// ɫ��
		// ˫
		//
		//+----------------
		enum EffectsType{
			Dx_ImageType,
			Dx_ColorType,
			Dx_ImageAndColorType,
		};


		//+----------------
		//
		// ����ͼ��
		//
		//+-----------------
		enum GeometyShape{
			Dx_Line_Shape = 0,    // ����һ���߶�
			Dx_Fill_Shape,    // ���һ������ͼ��
			Dx_Draw_Shape,    // ����һ������ͼ�α߿�
			Dx_Draw_Null
		};

	public:
		CDxEffects();
		~CDxEffects();


		void	BindWidget(CDxWidget* window);
		void	SetCurrentStatus(DXWindowStatus status); // ���õ�ǰ״̬
		void	SetColors(DXWindowStatus status, const std::vector<DxColor>& cols,const std::vector<double>& points);  // ������ɫ
		void	SetColor(DXWindowStatus status, const DxColor& col);
		void	SetBitmaps(DXWindowStatus status, const MString& bitmap,int w = -1,int h = -1); // ����λͼ
		void	SetResource(DXWindowStatus status, const MString& idType, LONG ID, int w = -1, int h = -1);
		void	SetGradientDirect(DXDirection dirct); // ���ý��䷽�򣬽������Խ�����Ч
		void	SetGradientType(GradientType type);  // ���ý������� ���Ի��߷�����
		void	SetDisabelColor(const DxColor& col);
		void	SetGeometryShapeType(GeometyShape type);
		void	SetGeometryShapeData(const TL::Vector<TL::Vector<DxPointD>>& points);
		void	SetGeometryShapeColor(DXWindowStatus status, const DxColor& col);
		void	SetOpacity(double val);


		void	Clear();


		//
		// ������ʾ����
		//
		void	SetShowRegion(const RECT& rc, const SIZE& randius, DXShape shap = Dx_Rectangle);  // ������ʾ���� ����λͼ��Ч
		void	SetShowRegion(const SIZE& randius, DXShape shap = Dx_Rectangle);  // ������ʾ���� ����λͼ��Ч
		void	SetShowRegion(const std::vector<DxPointD>& pts); // ������ʾ����,��λͼ������������Ч

		//
		// �ı�Ч��
		//
		void	SetDefaulFont(const MString& fontName, int fSize, bool isBold = false, bool isItalic = false); // �����ı�Ĭ������


		void	ClearTextSelected();
		void	SetTextSelectedRange(int iStart, int nLen, const DxColor& cBack, const DxColor& cFront); // Set ��������� ������������
		void	AddTextSelectedRange(int iStart, int nLen, const DxColor& cBack, const DxColor& cFront); // Add ׷��������

		void	ClearTextFontInfo();
		void	SetTextFontInfo(int iStart, int nLen, const MString& frontName, int fSize, bool isBold = false, bool isItalic = false); // Set ��������� ������������
		void	AddTextFontInfo(int iStart, int nLen, const MString& frontName, int fSize, bool isBold = false, bool isItalic = false); // Add ׷��������

		void	SetTextFontProperty(int iStart, int nLen, bool isBold, bool isItalic = false);
		void	AddTextFontProperty(int iStart, int nLen, bool isBold, bool isItalic = false);

		void	ClearTextFontSize();
		void	SetTextFontSize(int iStart, int nLen, int fSize); // Set ��������� ������������
		void	AddTextFontSize(int iStart, int nLen, int fSize); // Add ׷��������

		void	ClearTextColor();
		void	SetTextColor(int iStart, int nLen, const DxColor& col); // Set ���������
		void	AddTextColor(int iStart, int nLen, const DxColor& col);// Add ׷��������

		void	ClearUnderLine();
		void	SetUnderLine(int iStart, int nLen, const DxColor& col,CDxUnderlineType type);// Set ��������� ������������
		void	AddUnderLine(int iStart, int nLen, const DxColor& col,CDxUnderlineType type);// Add ׷��������

		void	ClearOverLine();
		void	SetOverLine(int iStart, int nLen, const DxColor& col);// Set ��������� ������������
		void	AddOverLine(int iStart, int nLen, const DxColor& col);// Add ׷��������

		void	ClearStrikeThrough(); 
		void	SetStrikeThrough(int iStart, int nLen, const DxColor& col, int counts = 1);// Set ��������� ������������
		void	AddStrikeThrough(int iStart, int nLen, const DxColor& col, int counts = 1);// Add ׷��������


		void	ClearHighlight();
		void	SetHighlight(int iStart, int nLen, const DxColor& col);// Set ��������� ������������
		void	AddHighlight(int iStart, int nLen, const DxColor& col);// Add ׷��������

		void	SetTextAlignment(DXTextAlignment alignment); // �����ı����뷽ʽ
		void	SetTextDefaultColor(const DxColor& col);     // ����Ĭ���ı���ɫ
		void	SetTextDefaultColor(DXWindowStatus windowstatus, const DxColor& col);
		void	ClearTextDefaultColor();


		CDxWidget*					GetWindow() const;
		DXWindowStatus				GetCurrentStatus() const;
		EffectsType					GetEffectType() const;
		DXDirection					GetDirection() const;
		const std::vector<DxColor>& GetColors() const;
		const std::vector<double>&	GetPoints() const;
		MString						GetBitmap() const;
		std::pair<MString, LONG>    GetResource() const;
		void						QueryImageSize(int& w, int & h) const;
		RECT						GetMaskRect() const;
		SIZE						GetMaskRadius() const;
		DXShape						GetMaskShape() const;
		GradientType				GetGradientType() const;
		GeometyShape				GetGeometryShapeType() const;
		DxColor						GetGeometryShapeColor() const;
		double						GetOpacity() const;
		const TL::Vector<TL::Vector<DxPointD>>& GetGeometryShapeData() const;

		
		const TL::Vector<DxPointD>&	GetGeometryPoints() const;
		const TL::Vector<TextSelectedInfo>& GetTextSelectedInfo() const;
		const TL::Vector<TextFontInfo>& GetTextFontInfo() const;
		const TL::Vector<TextColorInfo>& GetTextColorInfo() const;
		const TL::Vector<TextSizeInfo>&  GetTextSizeInfo() const;
		const TL::Vector<TextUnderLinerInfo>& GetTextUnderLineInfo() const;
		const TL::Vector<TextOverLinerInfo>&  GetOverLineInfo() const;
		const TL::Vector<TextStrikethroughInfo>& GetStrikethroughInfo() const;
		const TL::Vector<TextHighLightInfo>& GetHighLightInfo() const;
		DXTextAlignment				GetTextAlignment() const;
		const DXFontInfo& 			GetDefaultFont() const;
		const DxColor&				GetDefaultColor() const;

	private:
		CDxWidget*											pOwnWindow{ nullptr };
		std::map<DXWindowStatus, std::vector<DxColor>>		mColors;
		std::map<DXWindowStatus, std::vector<double>>		mPoints;
		std::map<DXWindowStatus, MString>					mBitmaps;
		std::map<DXWindowStatus, std::pair<MString,LONG>>	mReSourceMap;
		std::map<DXWindowStatus, DxColor>					mTextColMap;
		DXWindowStatus										mCurrentStatus;
		DXDirection											mDirection;
		GradientType										mGradientType;
		int													mImageWidth;
		int													mImageHeight;
		double												mOpacity{ 1.0 };
		RECT												mClipRect;
		SIZE												mClipRadius;
		DXShape												mClipShape;
		DxColor												mDisableColor;

		//+------------
		//
		// ���
		//
		//+------------
		TL::Vector<DxPointD>							   mGeometyPoints;


		//+------------------
		//
		// ����ͼ��
		//
		//+------------------
		TL::Vector<TL::Vector<DxPointD>>					mGeometryShapes;
		GeometyShape										mGeometryType;
		std::map<DXWindowStatus, DxColor>					mGeometryColors;


		//+------------
		//
		// �ַ�Ч��
		//
		//+------------
		DXFontInfo											mDefaultFont;
		DxColor												mTextDefaultColor;
		TL::Vector<TextSelectedInfo>						mTextSelectedVec;
		TL::Vector<TextFontInfo>							mTextFontVec;
		TL::Vector<TextColorInfo>							mTextColorVec;
		TL::Vector<TextSizeInfo>							mTextSizeVec;
		TL::Vector<TextUnderLinerInfo>						mTextUnderLinerVec;
		TL::Vector<TextOverLinerInfo>						mTextOverLineVec;
		TL::Vector<TextStrikethroughInfo>					mTextStrickeThroughVec;
		TL::Vector<TextHighLightInfo>						mTextHighLightVec;
		DXTextAlignment										mTextAlignment{ Dx_AlignNormal };
	};
}


