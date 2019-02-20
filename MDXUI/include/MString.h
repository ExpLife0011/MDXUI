

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
#include "StringImpl.h"
#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include "MStringIter.h"
#include <mutex>
#include <memory>
#include <fstream>

//
// ����MString�����쳣��ȫ�ģ������쳣����std::runtime_error �쳣
// �����ھ���ʹ����ע���쳣�Ĳ���
//

/***********************************************
* �̰߳�ȫ:
*************************************************/
namespace mj{

	class MString;


	///
	/// \brief The MBuffer struct
	/// ����һ����ʶbuffer�Ķ���
	/// �������Ķ��Ƕ���������
	/// �������紫��ʹ��MBuffer��ʹ��MString
	///
	class MSTRING_API MBuffer{
	private:
		char*  mBuffer{ nullptr };
		uint64 mLength{ 0ull }; // ��ǰ��ʹ�õĳ���
		uint64 mMemLength{ 0ull }; // �����ڴ��С
		uint64 mStreamPos{ 0ull };
		bool bIsBigEndian{ true };


	public:
		MBuffer(const char* ch = "");
		MBuffer(const char* ch, uint64 len);
		MBuffer(const MBuffer& other);
		~MBuffer();


		const char* c_str() const;
		char* c_str();

		operator char*();
		operator const char*() const;


		operator bool();
		operator bool() const;

		uint64 size();
		uint64 size() const;
		uint64 length() const;

		bool empty();
		bool empty() const;

		void clear();


		MBuffer& operator=(const MBuffer& other);
		MBuffer& operator=(const char* buffer);

		char& operator[](unsigned index);
		const char& operator[](unsigned index) const;

		char& operator()(unsigned index);
		const char& operator()(unsigned index) const;

		char& at(unsigned index);
		const char& at(unsigned index) const;


		bool initBuffer(uint64 size); // ��ʼ��һ���ڴ�,�Ѵ����ڴ�ֱ������
		void resize(uint64 size);     // ���·����ڴ��С�����С��ԭ���ڴ���ô������ڴ��ʽ����������ڵ�ǰ�ڴ���ô���·����ڴ�
		bool attach(const char* buffer, uint64 size); // ����һ��buffer ����ԭ������
		void append(const char* buffer, uint64 len);  // ׷��һ������
		void append(char* buffer, uint64 len);
		uint64 toFile(const MString& fileName, bool isApp = false);
		void fromFile(const MString& fileName);
		void fromFile(const MString& fileName, uint64 pos, uint64 len);
		void fromMString(const MString& mStr);

		///
		/// ׷��buffer
		///
		template<class T>
		MBuffer& append(T value, bool isBigEndian = true){
			char* ch = nullptr;
			int len = 0;
			if (isBigEndian){
				len = MString::ToBigEndianBuffer(value, ch);
			}
			else{
				len = MString::ToLittleEndianBuffer(value, ch);
			}
			append(ch, len);
			MString::FreeBuffer(ch);
			return *this;
		}

		template<class T, class A, template<class T1, class A1> class C>
		MBuffer& append(const C<T, A>& values, bool isBigEndian = true){
			for (const T& val : values){
				append(val, isBigEndian);
			}
			return *this;
		}

		template<class T, template<class T1> class C>
		MBuffer& append(const C<T>& values, bool isBigEndian = true){
			for (const T& val : values){
				append(val, isBigEndian);
			}
			return *this;
		}

		///
		/// �޸�ĳ��buffer
		///
		template<class T>
		MBuffer& modify(T value, uint64 pos, bool isBigEndian = true){
			if (pos >= mLength){
				append(value, isBigEndian);
				return *this;
			}

			if (pos + sizeof(T) > mLength){
				resize(pos);
				append(value, isBigEndian);
				return *this;
			}

			char* ch = nullptr;
			int len = 0;
			if (isBigEndian){
				len = MString::ToBigEndianBuffer(value, ch);
			}
			else{
				len = MString::ToLittleEndianBuffer(value, ch);
			}

			memcpy(&mBuffer[pos], ch, len);
			MString::FreeBuffer(ch);
			return *this;
		}


		///
		/// ��buffer��ԭ��һ�����ݣ���Ա�׼��
		///
		template<class T>
		void tovalue(T& value, uint64 pos = 0ull, bool isBigEndian = true){
			if (isBigEndian){
				MString::FromBigEndianBuffer(&mBuffer[pos], value);
			}
			else{
				MString::FromLittleEndianBuffer(&mBuffer[pos], value);
			}
		}

		template<class T, class A, template<class T1, class A1> class C>
		void tovalue(C<T, A>& values, uint64 pos = 0ull, bool isBigEndian = true){
			values.clear();
			T val;
			while (pos < mLength){
				if (isBigEndian){
					MString::FromBigEndianBuffer(&mBuffer[pos], val);
				}
				else{
					MString::FromLittleEndianBuffer(&mBuffer[pos], val);
				}
				values.push_back(val);
				pos += sizeof(T);
			}
		}

		///
		/// ��buffer��ԭ��һ�����ݣ����Qt��������������
		///
		template<class T, template<class T1> class C>
		void tovalue(C<T>& values, uint64 pos = 0ull, bool isBigEndian = true){
			values.clear();
			T val;
			while (pos < mLength){
				if (isBigEndian){
					MString::FromBigEndianBuffer(&mBuffer[pos], val);
				}
				else{
					MString::FromLittleEndianBuffer(&mBuffer[pos], val);
				}
				values.push_back(val);
				pos += sizeof(T);
			}
		}

		//
		// ����������
		//
		bool setEndianFormat(bool isBigEndian); // �����ý���Ӱ�������������
		void setStreamPos(uint64 pos);// ��λ��λ��

		//
		// ����
		//
		MBuffer& operator<<(__int8 val);
		MBuffer& operator<<(unsigned __int8 val);

		MBuffer& operator<<(__int16 val);
		MBuffer& operator<<(unsigned __int16 val);

		MBuffer& operator<<(__int32 val);
		MBuffer& operator<<(unsigned __int32 val);

		MBuffer& operator<<(__int64 val);
		MBuffer& operator<<(unsigned __int64 val);

		MBuffer& operator<<(float val);
		MBuffer& operator<<(double val);

		//
		// ����
		//
		MBuffer& operator>>(__int8& val);
		MBuffer& operator>>(unsigned __int8& val);

		MBuffer& operator>>(__int16& val);
		MBuffer& operator>>(unsigned __int16& val);

		MBuffer& operator>>(__int32& val);
		MBuffer& operator>>(unsigned __int32& val);

		MBuffer& operator>>(__int64& val);
		MBuffer& operator>>(unsigned __int64& val);

		MBuffer& operator>>(float& val);
		MBuffer& operator>>(double& val);

		friend std::ostream& operator<<(std::ostream& os, const MBuffer& buffer){
			os << buffer.mBuffer;
			return os;
		}
	};

	//
	// ���弸����������صĳ���������ʽ
	// REGFORFLOAT ������
	// REGFORIP IP��ַ
	// REFFORTEL �绰����
	// REGFORIDENTITY 18λ���֤
	//
	static const char* REGFORFLOAT = "(-?\\d+\\.?\\d*e?E?[+-]?\\d*)";
	static const char* REGFORIP = "((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)";
	static const char* REFFORTEL = "(0{0,1}(13[4-9]|15[7-9]|15[0-2]|18[7-8])[0-9]{8})";
	static const char* REGFORIDENTITY = "([1-9]\\d{5}[1-9]\\d{3}((0\\d)|(1[0-2]))(([0|1|2]\\d)|3[0-1])\\d{4})";


	/**********************************************
	* ����Ķ�����Ҫ��Ϊ�˴���ģʽ�滻
	* ������滻���ַ������� "*&%*@*" ��ʱ��
	* ��ô���滻֮ǰ���Ƚ� �ַ��� �滻Ϊ������
	* �滻��ɺ��ٽ����ַ����滻����
	***********************************************/
#ifndef __SPLIT__
#undef __SPLIT__
#define __SPLIT__ L"^"
#endif 

	class MSTRING_API MString
	{
	public:
		//
		// ����class
		// ʵ��������дʱ����
		//
		class MSTRING_API MCharProxy{
		public:
			MCharProxy(MString& str, size_t index);
			MCharProxy& operator=(const MCharProxy& r);
			MCharProxy& operator=(wchar_t ch);

			operator wchar_t()const;
			operator wchar_t();

			wchar_t* operator&();
			const std::wstring& operator&() const;

		private:
			MString& mStr;
			size_t mIndex;
		}; //class MCharProxy

		//==============================================
		// �ô������Ϊ��Ա��
		// �Ա��ܹ�����˽������
		//==============================================
		friend class MCharProxy;

		//
		// ��ϵ�����ʹ�õ�typedef
		//
		typedef MString type;
		typedef const MString const_type;
		typedef MIterator<MString> iterator;
		typedef ConstMIterator<MString> const_iterator;
		typedef std::random_access_iterator_tag iterator_category;
		typedef MCharProxy value_type;
		typedef /*char*/std::ptrdiff_t difference_type;
		typedef /*char*/std::ptrdiff_t distance_type;
		typedef MCharProxy* pointer;
		typedef MCharProxy reference; //���ﲢ�����������ã���ֻ�Ǵ�������

		//
		// ������Ч����ֵ
		//
		static uint64 npos;

		//
		// �������ı�����ʽ
		//
		enum FLOATFORMAT{
			FIXED,      // ʹ�ö����ʽ
			SCIENTIFIC  // ʹ�ÿ�ѧ��������ʽ
		};


		//
		// ���ø�������ת����ʽ
		//
		static void setFloatFormat(FLOATFORMAT format);
		static void setFloatPrecision(int precision);
	
		//
		// ���ֽںͶ��ֽڵ�ת��
		//
		static std::string w2c(const std::wstring& wstr);
		static std::wstring c2w(const std::string& str);


		MString(const char* str = "");
		MString(const std::string& str);
		MString(const wchar_t* str);
		MString(const std::wstring& str);
		MString(const wchar_t* buf, uint64 size);
		MString(char value);
		MString& fromFile(const MString& FileName);
		MString& fromFile(const MString &FileName, uint64 pos, uint64 len = 0ull);
		MString& fromUtf8File(const MString& utf8FileName);
		MString(MString&& other);
		MString& operator=(MString&& other);
		MString(const MString& other);
		MString& operator=(const MString& other);

		~MString();


		//
		// ���Ͳ���
		//
		template<class T>
		MString& operator+=(const T& value){
			return private_append(value);
		}

		template<class T>
		MString& push_back(const T& value){
			return private_append(value);
		}

		template<class T>
		MString& append(const T& value){
			return private_append(value);
		}

		template<class T>
		MString& operator<<(const T& value){
			return private_append(value);
		}

		//
		// ��ΪЧ�ʺͳ����Կ��������⼸�����ú��������⴦��
		//
		MString& operator+=(const std::string& value);
		MString& operator+=(const std::wstring& value);
		MString& operator+=(const char* value);
		MString& operator+=(const wchar_t* value);

		MString& push_back(const std::string& value);
		MString& push_back(const std::wstring& value);
		MString& push_back(const char* value);
		MString& push_back(const wchar_t* value);
		MString& push_back(const wchar_t* buf, uint64 size);

		MString& append(const std::string& value);
		MString& append(const std::wstring& value);
		MString& append(const char* value);
		MString& append(const wchar_t* value);
		MString& append(const wchar_t* buf, uint64 size);


		MString& operator<<(const std::string& value);
		MString& operator<<(const char* value);
		MString& operator<<(const std::wstring& value);
		MString& operator<<(const wchar_t* value);


		size_t find(const MString& value);
		size_t find(const MString& value) const;
		size_t find(const MString& value, unsigned offset);
		size_t find(const MString& value, unsigned offset) const;

		bool contains(const MString& value) const;
		bool contains(const MString& value);

		size_t find_first(const MString& value);
		size_t find_first(const MString& value) const;
		size_t find_last(const MString& value);
		size_t find_last(const MString& value) const;

		size_t find_nth(const MString& value, size_t index);
		size_t find_nth(const MString& value, size_t index) const;
		size_t find_count(const MString& value);
		size_t find_count(const MString& value) const;


		//
		// ����
		//
		void   insert(size_t pos, const MString& value);

		//
		// �滻������
		//
		MString& replace_first(const MString& replaced, const MString& value);
		MString& replace_all(const MString& replaced, const MString& value);
		MString& replace_nth(const MString& replaced, const MString& value, size_t index);
		MString& replace_last(const MString& replaced, const MString& value);

		//
		// �滻����
		// ���޸�ԭʼ��ֵ
		//
		MString replace_first_copy(const MString& replaced, const MString& value);
		MString replace_first_copy(const MString& replaced, const MString& value) const;
		MString replace_all_copy(const MString& replaced, const MString& value);
		MString replace_all_copy(const MString& replaced, const MString& value) const;
		MString replace_nth_copy(const MString& replaced, const MString& value, size_t index);
		MString replace_nth_copy(const MString& replaced, const MString& value, size_t index) const;
		MString replace_last_copy(const MString& replaced, const MString& value);
		MString replace_last_copy(const MString& replaced, const MString& value) const;

		//
		// ɾ��������
		//
		MString& erase_first(const MString& erase);
		MString& erase_all(const MString& erase);
		MString& erase_nth(const MString& erase, size_t index);
		MString& erase_last(const MString& erase);

		//
		// ֻ�Ա���ִ��ɾ������
		//
		MString erase_first_copy(const MString& erase);
		MString erase_all_copy(const MString& erase);
		MString erase_nth_copy(const MString& erase, size_t index);
		MString erase_last_copy(const MString& erase);
		MString erase_first_copy(const MString& erase) const;
		MString erase_all_copy(const MString& erase) const;
		MString erase_nth_copy(const MString& erase, size_t index) const;
		MString erase_last_copy(const MString& erase) const;

		//
		// �ͱ�׼��string����ʽ����
		//
		operator std::string() const;
		operator std::string();
		operator std::wstring() const;
		operator std::wstring();

		std::string toStdString();
		std::string toStdString() const;

		std::wstring toStdWString();
		std::wstring toStdWString() const;

		//
		// ��ѯ�Ƿ���ȣ�i��ͷ���ǲ����ִ�Сд
		//
		bool iequal(const MString& other);
		bool iequal(const MString& other) const;
		bool equal(const MString& other);
		bool equal(const MString& other) const;

		//
		// ��ѯ�Ƿ��а�����i��ͷ���ǲ����ִ�Сд
		//
		bool icontains(const MString& other) const;

		//
		// ��������ʽ��֧��
		// reg_match Ҫ��ȫƥ��
		// reg_serch ֻҪ�а�������
		//
		bool reg_match(const MString& reg);
		bool reg_serch(const MString& reg);

		bool reg_match(const MString& reg) const;
		bool reg_serch(const MString& reg) const;

		//
		// ʹ��������ʽ�����滻
		//
		MString& reg_replace(const MString& reg, const MString& fomart);

		//
		// ʹ��������ʽ���и����滻
		//
		MString reg_replace_copy(const MString& reg, const MString& fomart);
		MString reg_replace_copy(const MString& reg, const MString& fomart) const;

		//
		// ����ַ���
		//
		void clear();

		//
		// ��ȡ�ַ����ĳ���
		//
		size_t size();
		const size_t size() const;

		size_t length();
		const size_t length() const;

		//
		// ��ѯ�ַ����Ƿ���
		//
		bool isshared();
		bool isshared() const;



		//
		// ��ѯ���ַ����Ƿ�ɹ���
		//
		bool isshareable();
		bool isshareable() const;

		//
		// ���Ϊ������
		//
		void markunshare();
		void markunshare() const;

		//
		// �Ƿ�Ϊ����
		//
		bool isdigit();
		bool isdigit() const;

		//
		// ���ı�д���ļ�
		//
		bool toFile(const MString& fileName);
		bool toFile(const MString& fileName) const;

		//
		// ʹ�ô�����ͼ�ĺô���ȻҲ�����Ĳ���֮��
		//
		const MCharProxy operator[](size_t index) const;
		MCharProxy operator[](size_t index);

		//
		// ת��Ϊc�ַ���
		//
		const wchar_t* c_str() const;
		const wchar_t* c_str();

		//
		// ת��Ϊc���ַ���
		//
		const wchar_t* c_wstr() const;
		const wchar_t* c_wstr();

		//
		// ��ѯ�Ƿ�Ϊ��
		//
		bool empty();
		bool empty() const;

		//
		// ȥ���ַ������˵Ŀհ�
		//
		MString& trim();
		MString& trim_left();
		MString& trim_right();

		//
		// ��ȡ���ݵ�ַ
		//
		const void* GetAddr() const;
		void* GetAddr();

		//
		// ��ȡ���ü���
		//
		size_t reference_count() const;

		//
		// ����������
		//
		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

		//
		// �Ƚϲ���,i��ͷ���ǲ����ִ�Сд
		//
		bool end_with(const MString& str);
		bool iend_with(const MString& str);
		bool start_with(const MString& str);
		bool istart_with(const MString& str);

		bool end_with(const MString& str) const;
		bool iend_with(const MString& str) const;
		bool start_with(const MString& str) const;
		bool istart_with(const MString& str) const;


		//
		// ��Сдת������
		//
		MString& to_upper();
		MString to_upper_copy();
		MString to_upper_copy() const;

		MString& to_lower();
		MString to_lower_copy();
		MString to_lower_copy() const;

		//
		// ���������ַ���
		//
		void swap(MString& other);


		bool operator==(const MString& other);
		bool operator!=(const MString& other);
		bool operator<(const MString& other);
		bool operator>(const MString& other);

		friend bool operator==(const MString& Ldata, const MString& Rdata){
			return const_cast<MString&>(Ldata).operator==(Rdata);
		}

		friend bool operator!=(const MString& Ldata, const MString& Rdata){
			return const_cast<MString&>(Ldata).operator!=(Rdata);
		}

		friend bool operator<(const MString& Ldata, const MString& Rdata){
			return const_cast<MString&>(Ldata).operator<(Rdata);
		}

		friend bool operator>(const MString& Ldata, const MString& Rdata){
			return const_cast<MString&>(Ldata).operator>(Rdata);
		}

		//
		// ��ȡһ���������ø���������
		//
		MString copy();

		//
		// ת��Ϊutf8�ַ���
		//
		MString to_utf8();
		MString to_utf8() const;

		//
		// ת��Ϊchar
		//
		char toChar();
		char toChar() const;

		//
		// ���ٷָ��ַ���
		//
		void split(const MString& isanyof, std::vector<std::wstring>& result, bool isDiscardEmpty = true);



		//
		// ����Ĳ���������ģ����������ʵ��MString�ķ��ͻ�
		//

		//
		// ���͹��캯�����������޷��ж�ԭʼ���ͣ����Բ���������б���ת��
		//
		template<class iter>
		MString(iter beg, iter end){
			int len = std::distance(beg, end);
			if (len == 0){
				Init("", 0);
			}
			else{
				Init(&(*beg), len);
			}
		}

		template<>
		MString(char* beg, char* end)
		{
			std::string str(beg, end);
			Init(str.c_str());
		}

		template<>
		MString(const char* beg,const char* end)
		{
			std::string str(beg, end);
			Init(str.c_str());
		}

		template<>
		MString(std::istreambuf_iterator<char> beg, std::istreambuf_iterator<char> end)
		{
			std::string str(beg, end);
			Init(str.c_str());
		}

		template<>
		MString(std::istream_iterator<char> beg, std::istream_iterator<char> end)
		{
			std::string str(beg, end);
			Init(str.c_str());
		}

		template<>
		MString(wchar_t* beg, wchar_t* end)
		{
			std::wstring str(beg, end);
			Init(str.c_str());
		}

		template<>
		MString(const wchar_t* beg, const wchar_t* end)
		{
			std::wstring str(beg, end);
			Init(str.c_str());
		}

		//
		// ���͹��캯��
		//
		template<class T>
		MString(const T& value) {
			std::wostringstream s;
			s.setf(sFloatFromat, std::ios_base::floatfield);
			s.precision(sFloatPrecision);
			if (!(s << value).fail()){
				Init(s.str().c_str());
			}
			else{
				Init("", 0);
			}
		}


		//
		// ���͵ķ�ֵ��������
		//
		template<class iter>
		MString& assign(iter beg, iter end){
			chechshared();
			*this = "";
			for (; beg != end;){
				*this += *beg++;
			}
			return *this;
		}

		template<>
		MString& assign(char* beg, char* end){
			chechshared();
			clear();
			std::string str(beg, end);
			*this = str;
			return *this;
		}

		template<>
		MString& assign(const char* beg, const char* end){
			chechshared();
			clear();
			std::string str(beg, end);
			*this = str;
			return *this;
		}

		template<>
		MString& assign(std::istreambuf_iterator<char> beg, std::istreambuf_iterator<char> end){
			chechshared();
			std::string str(beg, end);
			clear();
			*this += str;
			return *this;
		}

		template<>
		MString& assign(std::istream_iterator<char> beg, std::istream_iterator<char> end){
			chechshared();
			std::string str(beg, end);
			clear();
			*this += str;
			return *this;
		}


		MString& erase(int beg, int end);

		//
		// ��ǰ���
		//
		template<class T>
		MString& push_front(const T& value){
			chechshared();
			MString __v(value);
			__v += *this;
			*this = __v;
			return *this;
		}



		//
		// ���ͷ�ֵ����
		//
		template<class T>
		MString& operator=(T value){
			chechshared();
			std::wostringstream s;
			s.setf(sFloatFromat, std::ios_base::floatfield);
			s.precision(sFloatPrecision);
			if (!(s << value).fail()){
				clear();
				m_value->append(s.str().c_str());
			}
			return *this;
		}

		//
		// ���const std::string&�Լ�string���ػ�
		//
		template<>
		MString& operator=(const std::string& str){
			chechshared();
			clear();
			m_value->append(str.c_str());
			return *this;
		}

		template<>
		MString& operator=(std::string str){
			chechshared();
			clear();
			m_value->append(str.c_str());
			return *this;
		}

		template<>
		MString& operator=(const char* str){
			chechshared();
			clear();
			m_value->append(str);
			return *this;
		}

		template<>
		MString& operator=(char* str){
			chechshared();
			clear();
			m_value->append(str);
			return *this;
		}

		template<>
		MString& operator=(const std::wstring& str){
			chechshared();
			clear();
			m_value->append(str.c_str());
			return *this;
		}

		template<>
		MString& operator=(std::wstring str){
			chechshared();
			clear();
			m_value->append(str.c_str());
			return *this;
		}

		template<>
		MString& operator=(const wchar_t* str){
			chechshared();
			clear();
			m_value->append(str);
			return *this;
		}

		template<>
		MString& operator=(wchar_t* str){
			chechshared();
			clear();
			m_value->append(str);
			return *this;
		}

		//
		// ת��Ϊ��������
		//
		template<class T>
		T ToOtherType() const{
			T x;
			if (m_value->empty())
				return T();
			std::wistringstream s(m_value->c_str());
			//
			// Ŀǰ�����ֻҪ���ܹ�ת�����־�ִ��ת������,����Ҫȫ��ת��
			// �����Ҫȫ��ת���ɹ�����ɹ��Ļ����Լ���������� && s.get() == std::char_traits<wchar>::eof()
			// ��s>>x ����
			//
			if (s >> x){
				return x;
			}
			else{
				throw std::runtime_error("convent fail .... ");
			}
		}

		//
		// ���string��char*��ת��
		//
		template<>
		char* ToOtherType() const{
			char* buffer = new char[size() + 1];
			memset(buffer, 0, size() + 1);
			if (size() == 0)
				return buffer;
			memcpy(buffer, c_str(), size());
			return buffer;
		}

		

		template<>
		wchar_t* ToOtherType() const{
			wchar_t* buffer = new wchar_t[size() + 1];
			memset(buffer, 0, size() + 1);
			if (size() == 0)
				return buffer;
			memcpy(buffer, c_wstr(), size());
			return buffer;
		}

		template<>
		const wchar_t* ToOtherType() const{
			return c_wstr();
		}

		template<>
		std::string ToOtherType() const{
			return toStdString();
		}

		template<>
		const std::string ToOtherType() const{
			return toStdString();
		}

		template<>
		std::wstring ToOtherType() const{
			return toStdWString();
		}

		template<>
		const std::wstring ToOtherType() const{
			return toStdWString();
		}

		template<>
		MString ToOtherType() const{
			return *this;
		}

		template<>
		const MString ToOtherType() const{
			return *this;
		}


		template<>
		char ToOtherType() const{
			return toChar();
		}

		//
		// ���������ѧ��������ʾ������ת������ʱ�ᱻ����С�������ģ�����ת����double�ٶ���ȡ�ٽ�����
		//
		template<>
		__int16 ToOtherType() const{
			return std::round(ToOtherType<double>());
		}

		template<>
		unsigned __int16 ToOtherType() const{
			return std::round(ToOtherType<double>());
		}

		template<>
		__int32 ToOtherType() const{
			return std::round(ToOtherType<double>());
		}

		template<>
		unsigned __int32 ToOtherType() const{
			return std::round(ToOtherType<double>());
		}

		template<>
		__int64 ToOtherType() const{
			return std::round(ToOtherType<double>());
		}

		template<>
		unsigned __int64 ToOtherType() const{
			return std::round(ToOtherType<double>());
		}

		//
		// �ָ����
		//
		template<typename T, typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<T, A>& result, bool isDiscardEmpty = true){
			result.clear();
			if (isanyof.empty())return;
			std::vector<std::wstring> __result;
			split(isanyof, __result, isDiscardEmpty);
			if (__result.empty())return;
			for (std::wstring& mstr : __result){
				try{
					MString __mstr = mstr;
					T other = __mstr.ToOtherType<T>();
					result.push_back(other);
				}
				catch (...){
					;
				}
			}
		}

		template<typename T, typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<T, A>& result, bool isDiscardEmpty = true) const{
			const_cast<MString*>(this)->split(isanyof, result, isDiscardEmpty);
		}

		//
		// ����ַ��������ػ�
		//
		template<typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<std::string, A>& result, bool isDiscardEmpty = true){
			result.clear();
			if (isanyof.empty())return;
			std::vector<std::wstring> __result;
			split(isanyof, __result, isDiscardEmpty);
			result.reserve(__result.size());
			for (auto& s : __result){
				result.push_back(MString(s));
			}
			
		}

		template<typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<std::wstring, A>& result, bool isDiscardEmpty = true){
			result.clear();
			if (isanyof.empty())return;
			std::vector<std::wstring> __result;
			split(isanyof, __result, isDiscardEmpty);
			result.assign(__result.begin(), __result.end());
		}

		template<typename A, template<typename T1, typename A1> class C>
		void split(const MString& isanyof, C<MString, A>& result, bool isDiscardEmpty = true){
			result.clear();
			if (isanyof.empty())return;
			C<std::wstring, A> __result;
			split(isanyof, __result, isDiscardEmpty);
			result.assign(__result.begin(), __result.end());
		}

		//
		// ʹ��������ʽ�ָ����
		//
		template<typename T, typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<T, A>& result){
			result.clear();
			std::vector<MString>  v;
			reg_split(reg, v);
			for (MString& s : v){
				if (s.empty())
					continue;
				try{
					T other = s.ToOtherType<T>();
					result.push_back(other);
				}
				catch (...){
					;
				}
			}
		}


		//
		// ���ַ��������ػ�
		//
		template<typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<std::string, A>& result){
			result.clear();
			if (reg.empty())
				return;

			MString __str = *this;
			__str.chechshared();
			__str.m_value->MarkUnshareable();
			__str.reg_replace(reg, __SPLIT__);
			__str.split(__SPLIT__, result);
		}

		template<typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<std::wstring, A>& result){
			result.clear();
			if (reg.empty())
				return;

			MString __str = *this;
			__str.chechshared();
			__str.m_value->MarkUnshareable();
			__str.reg_replace(reg, __SPLIT__);
			__str.split(__SPLIT__, result);
		}

		template<typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<MString, A>& result){
			result.clear();
			if (reg.empty())
				return;
			MString __str = *this;
			__str.chechshared();
			__str.m_value->MarkUnshareable();
			__str.reg_replace(reg, __SPLIT__);
			__str.split(__SPLIT__, result);
		}

		template<typename T, typename A, template<typename T1, typename A1> class C>
		void reg_split(const MString& reg, C<T, A>& result) const{
			const_cast<MString*>(this)->reg_split(reg, result);
		}

		//
		// all �ж�����Ԫ���Ƿ������������
		//
		template<class iter, class F>
		bool all(iter beg, iter end, F fun){
			while (beg != end){
				if (!fun(*beg))
					return false;
			}
			return true;
		}

		template<class iter, class F>
		bool all(iter beg, iter end, F fun) const{
			return const_cast<MString*>(this)->all(beg, end, fun);
		}

		template<class F>
		bool all(int beg, int end, F fun){
			int len = m_value->m_data.length();
			beg = end > beg ? beg : end;
			end = end > beg ? end : beg;
			if (beg < 0 || beg >= len || end < 0 || end >= len)
				return false;
			for (; beg != end; ++beg){
				if (!fun(m_value->c_str()[i]))
					return false;
			}
			return true;
		}

		template<class F>
		bool all(F fun){
			int len = m_value->m_data.length();
			for (int i = 0; i < len; ++i){
				if (!fun(m_value->c_str()[i]))
					return false;
			}
			return true;
		}

		template<class F>
		bool all(F fun) const{
			return const_cast<MString*>(this)->all(fun);
		}



		//
		// ������ȫƥ��
		//
		template<class T, class A, template<class T1, class A1>class C>
		bool reg_match(const MString& reg, C<T, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wsmatch m;
			std::wstring __str = this->toStdWString();
			bool __bmatch = std::regex_search(__str, m, __reg);
			if (__bmatch){
				for (int i = 0; i < m.size(); ++i){
					MString str(m[i]);
					try{
						T value = str.ToOtherType<T>();
						result.push_back(value);
					}
					catch (...){
						;
					}
				}
			}
			return __bmatch;
		}

		template<class T, class A, template<class T1, class A1>class C>
		bool reg_match(const MString& reg, C<T, A>& result) const{
			return const_cast<MString*>(this)->reg_match(reg, result);
		}

		//
		// ����ƥ��ĵ�һ������
		//
		template<class T>
		bool reg_find(const MString& reg, T& result){
			std::wregex __reg(reg.toStdWString());
			std::wsmatch m;
			std::wstring __str = this->toStdWString();
			bool __bmatch = std::regex_search(__str, m, __reg);
			if (__bmatch){
				for (int i = 0; i < m.size(); ++i){
					__str = m[i];
					MString str(__str);
					if (str.empty())
						continue;
					try{
						result = str.ToOtherType<T>();
						return true;
					}
					catch (std::runtime_error e){
						return false;
					}
				}
				return true;
			}
			return false;
		}

		template<class T>
		bool reg_find(const MString& reg, T& result) const{
			return const_cast<MString*>(this)->reg_find(reg, result);
		}

		template<class T>
		bool reg_find_replaced(const MString& reg, T& result){
			MString str;
			bool is_ok = reg_find(reg, str);
			if (is_ok){
				replace_first(str, L"");
				trim();
				try{
					result = str.ToOtherType<T>();
				}
				catch (...){
					;
				}
			}
			return is_ok;
		}

		template<class T>
		bool reg_find_replaced(const MString& reg, T& result) const{
			return const_cast<MString*>(this)->reg_find_replaced(reg, result);
		}


		//
		// ֻ���ص�һ��ƥ���������Ϣ
		//
		template<class T, class A, template<class T1, class A1> class C>
		bool reg_serch(const MString& reg, C<T, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wsmatch m;
			std::wstring wstr = this->toStdWString();
			bool __bmatch = std::regex_search(wstr, m, __reg);
			if (__bmatch){
				for (int i = 0; i < m.size(); ++i){
					std::wstring __wstr(m[i]);
					MString str(__wstr);
					try{
						T value = str.ToOtherType<T>();
						result.push_back(value);
					}
					catch (...){
						;
					}
				}
			}
			return __bmatch;
		}

		template<class T, class A, template<class T1, class A1> class C>
		bool reg_serch(const MString& reg, C<T, A>& result) const{
			return const_cast<MString*>(this)->reg_serch(reg, result);
		}

		//
		// ������ƥ���ȫ����ȡ����
		//
		template<class T, class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<T, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wstring str = this->toStdWString();
			std::wsregex_iterator pos(str.begin(), str.end(), __reg);
			std::wsregex_iterator end;
			for (; pos != end;){
				std::wstring wstr((*pos)[0]);
				MString mstr(wstr);
				try{
					T value = mstr.ToOtherType<T>();
					result.push_back(value);
				}
				catch (...){
					;
				}
				++pos;
			}
		}

		//
		// ����ַ��������ػ�
		//
		template<class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<std::string, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wstring str = this->toStdWString();
			std::wsregex_iterator pos(str.begin(), str.end(), __reg);
			std::wsregex_iterator end;
			for (; pos != end;){
				std::wstring wstr((*pos)[0]);
				MString mstr(wstr);
				result.push_back(mstr);
				++pos;
			}
		}

		template<class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<std::wstring, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wstring str = this->toStdWString();
			std::wsregex_iterator pos(str.begin(), str.end(), __reg);
			std::wsregex_iterator end;
			for (; pos != end;){
				result.push_back((*pos)[0]);
				++pos;
			}
		}

		template<class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<MString, A>& result){
			result.clear();
			std::wregex __reg(reg.toStdWString());
			std::wstring str = this->toStdWString();
			std::wsregex_iterator pos(str.begin(), str.end(), __reg);
			std::wsregex_iterator end;
			for (; pos != end;){
				std::wstring  __str = (*pos)[0];
				result.push_back(__str);
				++pos;
			}
		}

		template<class T, class A, template<class T1, class A1> class C>
		void reg_serch_all(const MString& reg, C<T, A>& result) const{
			const_cast<MString*>(this)->reg_serch_all(reg, result);
		}

		//
		// ��ʽ������
		// ��ʹ�ø�ʽ������֮ǰ����ʹ��clear�������
		// ��ǰ����
		// ���п��ܻ�ͬʱ�ı�����������
		//
		template<typename T, typename... Args>
		void operator()(const wchar_t* s, const T& value, const Args&... args){
			chechshared();
			while (s && *s) {
				if (*s == L'%') {
					wchar_t ch = *++s;
					if (ch ==L'+'){
						wchar_t __ch = *++s;
						if (__ch > 48 && __ch < 58){
							int num = __ch;
							num -= 48;
							__ch = *++s;
							if (__ch >= 48 && __ch < 58){
								int __num = __ch;
								__num -= 48;
								num *= 10;
								num += __num;
								++s;
							}
							std::wostringstream os;
							os.width(num);
							os.setf(std::ios::right);
							os << value;
							*this += os.str();
							return operator ()(s, args...);
						}
						else{
							MString mstr;
							mstr = value;
							*this += mstr;
							return operator ()(s, args...);
						}
					}
					else if (ch == L'-'){
						wchar_t __ch = *++s;
						if (__ch > 48 && __ch < 58){
							int num = __ch;
							num -= 48;
							__ch = *++s;
							if (__ch >= 48 && __ch < 58){
								int __num = __ch;
								__num -= 48;
								num *= 10;
								num += __num;
								++s;
							}

							std::wostringstream os;
							os.width(num);
							os.setf(std::ios::left);
							os << value;
							*this += os.str();
							return operator ()(s, args...);
						}
						else{
							MString mstr;
							mstr = value;
							*this += mstr;
							return operator ()(s, args...);
						}
					}
					else if ((ch >= 48 && ch < 58) || (ch >= 65 && ch < 91) || (ch >= 97 && ch < 123))
					{
						std::wostringstream os;
						os << value;
						*this += os.str();
						return operator ()(++s, args...);
					}
					else if (ch == '.'){
						ch = *++s;
						std::wostringstream os;
						if (ch >= 48 && ch < 58){
							int num = ch;
							num -= 48;
							os.precision(num);
							wchar_t __ch = *++s;
							if (__ch == L'e' || __ch == L'E'){
								os.setf(std::ios_base::scientific, std::ios_base::floatfield);
							}
							else if (__ch == L'f' || __ch == L'F'){
								os.setf(std::ios_base::fixed, std::ios_base::floatfield);
							}
							else{
								--s;
							}
						}
						else{
							--s;
						}
						os << value;
						*this += os.str();
						return operator ()(++s, args...);
					}
					--s;
				}
				*this += *s++;
			}
		}

		//
		// ����char*
		// �������ʹ��wchar_t �汾
		// ʹ�øð汾���ܻ���һЩû�в��Ե�������
		//
		template<typename T, typename... Args>
		void operator()(const char* s, const T& value, const Args&... args){
			chechshared();
			while (s && *s) {
				if (*s == '%') {
					wchar_t ch = *++s;
					if (ch == '+'){
						wchar_t __ch = *++s;
						if (__ch > 48 && __ch < 58){
							int num = __ch;
							num -= 48;
							__ch = *++s;
							if (__ch >= 48 && __ch < 58){
								int __num = __ch;
								__num -= 48;
								num *= 10;
								num += __num;
								++s;
							}
							std::ostringstream os;
							os.width(num);
							os.setf(std::ios::right);
							os << value;
							*this += os.str();
							return operator ()(s, args...);
						}
						else{
							MString mstr;
							mstr = value;
							*this += mstr;
							return operator ()(s, args...);
						}
					}
					else if (ch == '-'){
						wchar_t __ch = *++s;
						if (__ch > 48 && __ch < 58){
							int num = __ch;
							num -= 48;
							__ch = *++s;
							if (__ch >= 48 && __ch < 58){
								int __num = __ch;
								__num -= 48;
								num *= 10;
								num += __num;
								++s;
							}

							std::ostringstream os;
							os.width(num);
							os.setf(std::ios::left);
							os << value;
							*this += os.str();
							return operator ()(s, args...);
						}
						else{
							MString mstr;
							mstr = value;
							*this += mstr;
							return operator ()(s, args...);
						}
					}
					else if ((ch >= 48 && ch < 58) || (ch >= 65 && ch < 91) || (ch >= 97 && ch < 123))
					{
						std::ostringstream os;
						os << value;
						*this += os.str();
						return operator ()(++s, args...);
					}
					else if (ch == '.'){
						ch = *++s;
						std::ostringstream os;
						if (ch >= 48 && ch < 58){
							int num = ch;
							num -= 48;
							os.precision(num);
							char __ch = *++s;
							if (__ch == L'e' || __ch == L'E'){
								os.setf(std::ios_base::scientific, std::ios_base::floatfield);
							}
							else if (__ch == L'f' || __ch == L'F'){
								os.setf(std::ios_base::fixed, std::ios_base::floatfield);
							}
							else{
								--s;
							}
						}
						else{
							--s;
						}
						os << value;
						*this += os.str();
						return operator ()(++s, args...);
					}
					--s;
				}
				*this += *s++;
			}
		}

		template<class T, class... Args>
		static MString Format(const wchar_t* s, const T& value, const Args&...args){
			MString str;
			str(s, value, args...);
			return str;
		}

		//
		// ����char*
		// 
		template<class T, class... Args>
		static MString Format(const char* s, const T& value, const Args&...args){
			MString str;
			str(s, value, args...);
			return str;
		}


		template<class iter>
		MString copy(iter beg, iter end){
			MString result;
			for (; beg != end;){
				result += *beg++;
			}
			return result;
		}

		template<class iter>
		MString copy(iter beg, iter end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(iterator beg, iterator end){
			MString result;
			result.append(&((*this)[beg - begin()]), end - beg);
			return result;
		}

		template<>
		MString copy(iterator beg, iterator end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(const_iterator beg, const_iterator end){
			const MString __Str(*this);
			MString result(&(__Str.c_wstr()[beg - __Str.begin()]), end - beg);
			return result;
		}

		template<>
		MString copy(const_iterator beg, const_iterator end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(__int64 beg, __int64 end){
			if (beg > end )
				return "";
			if (beg < 0)
				beg = 0;
			if (end > size())
				end = size();

			MString result(private_copy(beg, end));
			return result;
		}

		template<>
		MString copy(__int64 beg, __int64 end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(size_t beg, size_t end){
			return copy<__int64>(beg, end);
		}

		template<>
		MString copy(size_t beg, size_t end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}

		template<>
		MString copy(int beg, int end){
			return copy<__int64>(beg, end);
		}

		template<>
		MString copy(int beg, int end) const{
			return const_cast<MString*>(this)->copy(beg, end);
		}


		MString substr(int beg, int end) const{
			return this->copy(beg, end);
		}

		//
		// ʵ��������
		//
		friend std::ostream& operator<<(std::ostream& os, const MString& str){
			os << str.toStdString();
			return os;
		}

		friend std::istream& operator>>(std::istream& is, MString& str){
			std::string str1;
			std::getline(is, str1, '\n');
			str = str1;
			return is;
		}

		//
		// ��Կ�����֧��
		//
		friend std::wostream& operator<<(std::wostream& os, const MString& str){
			os << str.toStdWString();
			return os;
		}

		friend std::wistream& operator>>(std::wistream& wis, MString& str){
			std::wstring wstr;
			std::getline(wis, wstr);
			str = MString(wstr);
			return wis;
		}

		//
		// ��ʾת�����ݵ�MString
		//
		template<class T>
		static MString number(T value, FLOATFORMAT Format = FIXED, int pricision = 3){
			std::wostringstream s;
			s.setf(Format, std::ios_base::floatfield);
			s.precision(pricision);
			s << value;
			return s.str();
		}

		template<class T>
		static MString number(const T& value, int pricision, const char& format = 'f'){
			std::wostringstream s;
			if (format == 'f' || format == 'F'){
				s.setf(std::ios_base::fixed, std::ios_base::floatfield);
			}
			else{
				s.setf(std::ios_base::scientific, std::ios_base::floatfield);
			}
			s.precision(pricision);
			s << value;
			return s.str();
		}

		template<class T>
		static MString to_utf8(const T& value){
			MString str = value;
			return str.to_utf8();
		}


		//
		// UTF8 ����ת�� GBK
		//
		static MString utf8_2_gbk(const MString& utf8str);


		//
		// ���涼��һЩ��̬����
		//

		//
		// ���ɴ�С�˵�buffer
		//
		static uint64 ToBigEndianBuffer(__int8 value, char*& buffer);
		static uint64 ToBigEndianBuffer(__int16 value, char*& buffer);
		static uint64 ToBigEndianBuffer(__int32 value, char*& buffer);
		static uint64 ToBigEndianBuffer(__int64 value, char*& buffer);
		static uint64 ToBigEndianBuffer(unsigned __int8 value, char*& buffer);
		static uint64 ToBigEndianBuffer(unsigned __int16 value, char*& buffer);
		static uint64 ToBigEndianBuffer(unsigned __int32 value, char*& buffer);
		static uint64 ToBigEndianBuffer(unsigned __int64 value, char*& buffer);
		static uint64 ToBigEndianBuffer(float value, char*& buffer);
		static uint64 ToBigEndianBuffer(double value, char*& buffer);

		static uint64 ToLittleEndianBuffer(__int8 value, char*& buffer);
		static uint64 ToLittleEndianBuffer(__int16 value, char*& buffer);
		static uint64 ToLittleEndianBuffer(__int32 value, char*& buffer);
		static uint64 ToLittleEndianBuffer(__int64 value, char*& buffer);
		static uint64 ToLittleEndianBuffer(unsigned __int8 value, char*& buffer);
		static uint64 ToLittleEndianBuffer(unsigned __int16 value, char*& buffer);
		static uint64 ToLittleEndianBuffer(unsigned __int32 value, char*& buffer);
		static uint64 ToLittleEndianBuffer(unsigned __int64 value, char*& buffer);
		static uint64 ToLittleEndianBuffer(float value, char*& buffer);
		static uint64 ToLittleEndianBuffer(double value, char*& buffer);

		//
		// ������С��buffer
		//
		static void FromBigEndianBuffer(char* buffer, __int8& value);
		static void FromBigEndianBuffer(char* buffer, __int16& value);
		static void FromBigEndianBuffer(char* buffer, __int32& value);
		static void FromBigEndianBuffer(char* buffer, __int64& value);
		static void FromBigEndianBuffer(char* buffer, unsigned __int8& value);
		static void FromBigEndianBuffer(char* buffer, unsigned __int16& value);
		static void FromBigEndianBuffer(char* buffer, unsigned __int32& value);
		static void FromBigEndianBuffer(char* buffer, unsigned __int64& value);
		static void FromBigEndianBuffer(char* buffer, float& value);
		static void FromBigEndianBuffer(char* buffer, double& value);

		static void FromLittleEndianBuffer(char* buffer, __int8& value);
		static void FromLittleEndianBuffer(char* buffer, __int16& value);
		static void FromLittleEndianBuffer(char* buffer, __int32& value);
		static void FromLittleEndianBuffer(char* buffer, __int64& value);
		static void FromLittleEndianBuffer(char* buffer, unsigned __int8& value);
		static void FromLittleEndianBuffer(char* buffer, unsigned __int16& value);
		static void FromLittleEndianBuffer(char* buffer, unsigned __int32& value);
		static void FromLittleEndianBuffer(char* buffer, unsigned __int64& value);
		static void FromLittleEndianBuffer(char* buffer, float& value);
		static void FromLittleEndianBuffer(char* buffer, double& value);


		///
		/// \brief Buffer
		/// \param buffer
		/// \param pos
		/// ��ȡָ����buffer,ʵ�е������������֮����Ҫʹ��FreeBuffer���ڴ�����ͷ�
		///
		void Buffer(MBuffer& buffer, uint64 pos = 0ull, uint64 size = 0ull);

		///
		/// \brief FreeBuffer
		/// \param buffer
		/// ������������ɵ�buffer�ڴ˴��ͷ�
		///
		static void FreeBuffer(char*& buffer);





		///
		/// ��ȡguid�ַ���
		///
		static MString GuidString();

		///
		/// ��ȡ��ǰʱ��
		///
		static MString CurrentTime();

		/// 
		/// ��ȡ������������
		///
		static MString EnvString(const MString& EnvKey);

		///
		/// ���û�������
		///
		static bool   SetEnv(const MString& EnvKey, const MString& EnvValue);

		///
		/// \brief operator ()
		/// \param s
		///
		void operator()(const wchar_t* s);
		void operator()(const char* s);


		//
		// ����ת��ϵ�к���
		//
		int toInt() const;
		int toInt();

		unsigned int toUInt() const;
		unsigned int toUInt();

		short toShort() const;
		short toShort();

		unsigned short toUShort() const;
		unsigned short toUShort();

		long toLong() const;
		long toLong();

		unsigned long toULong() const;
		unsigned long toULong();

		__int64 toLongLong() const;
		__int64 toLongLong();

		unsigned __int64 toULongLong() const;
		unsigned __int64 toULongLong();

		float toFloat() const;
		float toFloat();

		double toDouble() const;
		double toDouble();

		friend MString operator+(const MString& mstr, const MString& ch){
			MString __result(mstr);
			__result << ch;
			return __result;
		}

		///
		/// ��Ա����
		///
	private:

		//
		// ��ѯ��ǰ�Ƿ��й���
		//
		inline void chechshared();
		MString& private_append(const MString& value);
		void Init(const wchar_t *buf, uint64 size);
		void Init(const char *buf, uint64 size);
		void Init(const char* str);
		void Init(const wchar_t* str);
		const wchar_t* private_copy(int beg, int end);



		//
		// ����ָ��
		//
		StringRef<MStringImpl>  m_value;

		//
		// ��Ҫ��Ը�����ת��Ϊ�ַ���ʱ�Ƿ���ÿ�ѧ��������ʹ�ö����ʽ
		//
		static std::ios_base::fmtflags    sFloatFromat;
		static unsigned					  sFloatPrecision;
	};

	template<bool>
	struct MStringHelp{
		template<class T>
		static MString& do_help(MString& m, const T& t){
			MString s = t;
			m.replace_first(s.c_str(), "");
			return m;
		}
	};

	template<>
	struct MStringHelp<true>{
		template<class T>
		static MString& do_help(MString& m, const T& s){
			m.replace_first(s.c_str(), "");
			return m;
		}
	};

	//
	// ʹ�ñ�׼����������ת��ΪMString
	//
	template<class T, class A, template<class T1, class A1> class C>
	inline MString JoinToMString(const C<T, A>& c, const MString& joinstr){
		MString __result;
		for (auto s : c){
			if (!__result.empty()){
				__result << joinstr;
			}
			__result << s;
		}
		return __result;
	}

	//
	// ʹ�÷Ǳ�׼�����������ת��MString
	//
	template<class T, template<class T1> class C>
	inline MString JoinToMString(const C<T>& c, const MString& joinstr){
		MString __result;
		for (auto s : c){
			if (!__result.empty()){
				__result << joinstr;
			}
			__result << s;
		}
		return __result;
	}
}

//===============================================
// ʹ��operator>>�������
// �Կո�Ϊ���
// ������ݺ��ı�ԭʼ����
//===============================================

template<class T>
static inline mj::MString& operator>>(mj::MString& m, T& t){
	try{
		t = m.ToOtherType<T>();
	}
	catch (...){
		return m;
	}

	const bool is_true = (std::is_same<T, mj::MString>::value ||
		std::is_same<T, std::string>::value) ? true : false;
	m.trim();
	return mj::MStringHelp<is_true>::do_help(m, t);
}



//
// �ð�,�����ֿռ�ʹ�õ�ʱ���С��Щ,��Ϊ��Χ̫���ˣ������׺�����������ͻ�ᵼ�¸���Ī����ı������
//
namespace mjplus{
	static inline std::string operator+(const char* ch, const mj::MString& mstr){
		mj::MString __result(ch);
		__result << mstr;
		return __result;
	}

	static inline std::string operator+(const std::string& ch, const mj::MString& mstr){
		mj::MString __result(ch);
		__result << mstr;
		return __result;
	}

	static inline mj::MString operator+(const mj::MString& mstr, const char* ch){
		mj::MString __result(mstr);
		__result << ch;
		return __result;
	}

	static inline mj::MString operator+(const mj::MString& mstr, const std::string& ch){
		mj::MString __result(mstr);
		__result << ch;
		return __result;
	}

	template<class T>
	static inline mj::MString operator+(const mj::MString& mstr, T value){
		mj::MString __result(mstr);
		__result << value;
		return __result;
	}

	template<class T>
	static inline mj::MString operator+(T value, const mj::MString& mstr){
		mj::MString __result;
		__result << value;
		__result << mstr;
		return __result;
	}
}

//
// ʵ�ֺͱ�׼��ļ���
//
namespace std{
	template<>
	inline void swap(mj::MString& first, mj::MString& second){
		first.swap(second);
	}

	template<>
	inline void std::iter_swap<mj::MString::iterator, mj::MString::iterator>(
		mj::MString::iterator iter1, mj::MString::iterator iter2){
		wchar_t c1 = *iter1;
		wchar_t c2 = *iter2;
		size_t index1 = iter1.CurrentIndex();
		size_t index2 = iter2.CurrentIndex();
		mj::MString& __data = iter1.CurrentData();
		__data[index1] = c2;
		__data[index2] = c1;
	}

	template<>
	inline void std::iter_swap<mj::MString::const_iterator, mj::MString::const_iterator>(
		mj::MString::const_iterator iter1, mj::MString::const_iterator iter2)
	{

	}

	template <>
	struct hash<mj::MString>
	{
		std::size_t operator()(const mj::MString& k) const
		{
			return (hash<std::wstring>()(k.toStdWString()));
		}
	};
}

