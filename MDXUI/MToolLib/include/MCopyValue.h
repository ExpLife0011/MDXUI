///////////////////////////////////////////////////////////////////////////////
//
// MToolLib ---- Mengjin Tool Library
// Copyright (C) 2014-2018 by Mengjin (sh06155@hotmail.com)
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
#include <type_traits>
#include <sstream>
#include <iostream>

namespace mj{
	template<class T, class U>
	struct IsConvertible :std::is_convertible<T, U>
	{
	};

	template<bool>
	struct MCopyValue{
		template<class T, class U>
		static void apply(T& val1, const U& val2){
			val1 = val2;
		}
	};

	template<>
	struct MCopyValue<false>{
		template<class T, class U>
		static void apply(T& val1, const U& val2){
			std::stringstream ss;
			ss << val2;
			ss >> val1;
		}

		template<class U>
		static void apply(std::string& str, const U& val2){
			std::stringstream ss;
			ss << val2;
			str = ss.str();
		}
	};

	//
	//  ��right��ֵΪleft
	//
	template<class T, class U>
	inline void copyvalue(T& left, const U& right){
		MCopyValue<IsConvertible<U,T>::value>::apply(left, right);
	}
}
