

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
#include "DxScrollBar.h"

namespace DxUI{
	//+-------------------------
	//
	// class CDxScrollArea
	//
	//+--------------------------
	class DXUI_API CDxScrollArea :
		public CDxWindow
	{
		DECLARE_CLASS(CDxScrollArea)
	public:
		CDxScrollArea();
		~CDxScrollArea();

		void AddChild(CDxWidget* childWindow);
		void RemoveChild(CDxWidget* childWindow);
		void Attach(CDxWidget* window);
		CDxWidget* Dettach();
		void ReceiveEvent(CDxUIEvent* e);
		void UpdateChildWindowPos();
	protected:
		void OnDealItemForDistance(int disx, int disy);
		bool OnNotify(DXNotifyUI* NotifyEvent);
		void OnRendWindow(IPainterInterface* painter);
	private:
		CDxHorizonScrollBar      mHorizonBar;
		CDxVerticalScrollBar	 mVerticalBar;
		CDxWidget*				 p_Window{ nullptr };
		int						 mXCurrentOffset{ 0 };
		int						 mYCurrentOffset{ 0 };
		int					     mXMaxOffset{ 0 };
		int						 mYMaxOffset{ 0 };
	};
}


