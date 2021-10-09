#pragma once

#include <string>
#include <streambuf>
#include "LogLines.h"

// redirect the cout output from the stdout to GUI widget
// adapted from editlog_stream.h, Version: 1.0, created 2000-04-02
// Author: Daniel Lohmann (daniel@uni-koblenz.de)
// see http://www.codeproject.com/articles/455/ceditlog-fast-logging-into-an-edit-control-with-co

namespace std {

	template<class E, class T = std::char_traits<E>, int BUF_SIZE = 512>
	class basic_streambuf_gui : public basic_streambuf<E, T> {
	public:
		basic_streambuf_gui(LogDisplay& Log) : std::basic_streambuf<E, T>(), display(Log) {
			psz = new char_type[BUF_SIZE];
			parent::setbuf(psz, BUF_SIZE);
			parent::setp(psz, psz + BUF_SIZE - 2); // leave place for a single char + \0 terminator
			::InitializeCriticalSection(&m_csLock);
		}
		
		virtual ~basic_streambuf_gui() {
			::DeleteCriticalSection(&m_csLock);
			delete psz;
		}

		using int_type = typename T::int_type;
		using char_type = typename T::char_type;
		using parent = basic_streambuf<E, T>;

	protected:
		
		virtual parent::int_type overflow(int_type c = T::eof()) {
			::EnterCriticalSection(&m_csLock);

			char_type* plast = parent::pptr();
			if (c != T::eof()) {
				*plast++ = c; // add c to buffer
			}
			*plast = char_type();
			display.AddItem(parent::pbase());
			parent::setp(parent::pbase(), parent::epptr());

			::LeaveCriticalSection(&m_csLock);
			return c != T::eof() ? T::not_eof(c) : T::eof();
		}

		virtual int sync() {
			overflow();
			return 0;
		}

		virtual std::streamsize xsputn(const char_type* pch, std::streamsize n) {
			std::streamsize nMax, nPut;
			::EnterCriticalSection(&m_csLock);

			for (nPut = 0; n > 0;) {
				if (parent::pptr() != 0 && (nMax = parent::epptr() - parent::pptr()) > 0) {
					if (n < nMax)
						nMax = n;
					parent::traits_type::copy(parent::pptr(), pch, nMax);
					bool bSync = parent::traits_type::find(pch, nMax, parent::traits_type::to_char_type('\n')) != NULL;
					pch += nMax, nPut += nMax, n -= nMax, parent::pbump((int)nMax);
					if (bSync)
						sync();
				}
				else if (parent::traits_type::eq_int_type(parent::traits_type::eof(),
					overflow(parent::traits_type::to_int_type(*pch))))
					break;
				else
					++pch, ++nPut, --n;
			}

			::LeaveCriticalSection(&m_csLock);
			return nPut;
		}

		parent::char_type* psz;
		LogDisplay& display;
		CRITICAL_SECTION m_csLock;
	};

	typedef basic_streambuf_gui<char> streambuf_gui;
	typedef basic_streambuf_gui<wchar_t> wstreambuf_gui;

}