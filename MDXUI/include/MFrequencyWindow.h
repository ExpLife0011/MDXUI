

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
#include "DxGroupBox.h"
#include "DxRadioButton.h"
#include "DxStackedWidget.h"

using DxUI::CDxWindow;
using DxUI::CDxRadioButton;
using DxUI::CDxStackedWidget;
using DxUI::CDxGroupBox;


//
// ������������״̬�Ķ���
//

class UiBase;
class DXUI_API MFrequencyWindow :
	public CDxWindow
{

	typedef std::function<double(double, const mj::MString&)> TRANSLATEDATAFUNTYPE;
	typedef std::function<void(const std::vector<mj::MString>&)> IMPORTDATAFUNTYPE;
public:
	MFrequencyWindow(const MString& laggroup = L"Frequency");
	~MFrequencyWindow();
	std::vector<double> GetData();

	//
	// ��������
	//
	void SetData(unsigned type,const std::vector<double>& data,const MString &unit);

	//
	// ���õ�λ�б�
	//
	void SetUnits(const TL::Vector<MString>& units);

	//
	// ���õ�λת������
	//
	void BindTranlatDataFun(TRANSLATEDATAFUNTYPE fun);

	//
	// ���õ�λת������
	//
	void BindUnitTranslateFun(TRANSLATEDATAFUNTYPE fun);

	//
	// ���õ���������Ĭ�������ļ�ʱʹ�õ��ⲿ������
	//
	void BindImportDataFun(IMPORTDATAFUNTYPE fun);

	//
	// ��ȡ����
	//
	unsigned GetType() const;

	//
	// ��ȡ��λ
	//
	MString GetUnit(); // ��ȡ��λ��Ϣ

	
	//
	// ������������ֻ��Ҫһ�� type -- 1 2 3
	//
	void setOnlyType(unsigned type);
private:
	void init_connect();
	void slot_rad(bool is_checked, CDxWidget* sender);
private:
	void init_style();
	double DefualtUnitTranslateFun(double value, const MString& unit);

	CDxGroupBox*      pTypeGroup;       //���Ϳ��
	CDxGroupBox*      pInfoGroup;       //��Ϣ���
	CDxRadioButton*   pSingleRad;   //��Ƶ��
	CDxRadioButton*   pRangeRad;    //��Χ
	CDxRadioButton*   pListRad;     //�б�
	CDxStackedWidget* pMultWidget;  //info

	UiBase*    pSingleigdet;
	UiBase*    pRangeWidget;
	UiBase*    pListWidget;

private:
	unsigned             mType; //1 2 3

	TRANSLATEDATAFUNTYPE mUnitTranslateFun{ nullptr };

};

