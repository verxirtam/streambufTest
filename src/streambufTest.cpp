/*
 * streambufTest.cpp
 *
 *  Created on: 2015/07/12
 *      Author: daisuke
 */

#include <iostream>

#include <streambuf>
#include <cstring>

using namespace std;

class TestStreambuf: public streambuf
{
public:
	static const int length = 8;
private:
	//読み込み用文字列バッファ
	char read[length];
	//書き込み用文字列バッファ
	char write[length];
public:
	TestStreambuf()
	{
		strcpy(read, "0123456");
		read[7] = '7';
		strcpy(write, "ABCDEFG");
		write[7] = 'H';

		//読み込み対象の文字列を設定する
		setg(read, read, read + length);
		//書き込み対象の文字列を設定する
		setp(write, write + length);
	}
	virtual ~TestStreambuf()
	{
	}
	//読み込み用文字列バッファの内容を返却する
	const char* getRead()
	{
		return read;
	}
	//書き込み用文字列バッファの内容を返却する
	const char* getWrite()
	{
		return write;
	}
	char getReadCurrent(void) const
	{
		return *this->gptr();
	}
	char getReadBegin(void) const
	{
		return *this->eback();
	}
	char getReadEnd(void) const
	{
		return *this->egptr();
	}
	char getWriteCurrent(void) const
	{
		return *this->pptr();
	}
	char getWriteBegin(void) const
	{
		return *this->pbase();
	}
	char getWriteEnd(void) const
	{
		return *this->epptr();
	}

//////////////////////////
protected:
	///////////////////////////////////////////////////////////////
	// [27.5.2.4] virtual functions
	// [27.5.2.4.1] locales
	/**
	 *  @brief  Changes translations.
	 *  @param  __loc  A new locale.
	 *
	 *  Translations done during I/O which depend on the current
	 *  locale are changed by this call.  The standard adds,
	 *  <em>Between invocations of this function a class derived
	 *  from streambuf can safely cache results of calls to locale
	 *  functions and to members of facets so obtained.</em>
	 *
	 *  @note  Base class version does nothing.
	 */
	virtual void imbue(const locale& __loc)
	{
		cout<<"imbue() called."<<endl;
	}

	// [27.5.2.4.2] buffer management and positioning
	/**
	 *  @brief  Manipulates the buffer.
	 *
	 *  Each derived class provides its own appropriate behavior.  See
	 *  the next-to-last paragraph of
	 *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt11ch25s02.html
	 *  for more on this function.
	 *
	 *  @note  Base class version does nothing, returns @c this.
	 */
	virtual streambuf*
	setbuf(char_type*, streamsize)
	{
		cout<<"setbuf() called."<<endl;
		return this;
	}

	/**
	 *  @brief  Alters the stream positions.
	 *
	 *  Each derived class provides its own appropriate behavior.
	 *  @note  Base class version does nothing, returns a @c pos_type
	 *         that represents an invalid stream position.
	 */
	virtual pos_type seekoff(off_type, ios_base::seekdir,
			ios_base::openmode /*__mode*/= ios_base::in | ios_base::out)
	{
		cout<<"seekoff() called."<<endl;
		return pos_type(off_type(-1));
	}

	/**
	 *  @brief  Alters the stream positions.
	 *
	 *  Each derived class provides its own appropriate behavior.
	 *  @note  Base class version does nothing, returns a @c pos_type
	 *         that represents an invalid stream position.
	 */
	virtual pos_type seekpos(pos_type,
			ios_base::openmode /*__mode*/= ios_base::in | ios_base::out)
	{
		cout<<"seekpos() called."<<endl;
		return pos_type(off_type(-1));
	}

	/**
	 *  @brief  Synchronizes the buffer arrays with the controlled sequences.
	 *  @return  -1 on failure.
	 *
	 *  Each derived class provides its own appropriate behavior,
	 *  including the definition of @a failure.
	 *  @note  Base class version does nothing, returns zero.
	 */
	virtual int sync()
	{
		cout<<"sync() called."<<endl;
		return 0;
	}

	// [27.5.2.4.3] get area
	/**
	 *  @brief  Investigating the data available.
	 *  @return  An estimate of the number of characters available in the
	 *           input sequence, or -1.
	 *
	 *  <em>If it returns a positive value, then successive calls to
	 *  @c underflow() will not return @c traits::eof() until at
	 *  least that number of characters have been supplied.  If @c
	 *  showmanyc() returns -1, then calls to @c underflow() or @c
	 *  uflow() will fail.</em> [27.5.2.4.3]/1
	 *
	 *  @note  Base class version does nothing, returns zero.
	 *  @note  The standard adds that <em>the intention is not only that the
	 *         calls [to underflow or uflow] will not return @c eof() but
	 *         that they will return immediately.</em>
	 *  @note  The standard adds that <em>the morphemes of @c showmanyc are
	 *         @b es-how-many-see, not @b show-manic.</em>
	 */
	virtual streamsize showmanyc()
	{
		cout<<"showmanyc() called."<<endl;
		return 0;
	}

	/**
	 *  @brief  Multiple character extraction.
	 *  @param  __s  A buffer area.
	 *  @param  __n  Maximum number of characters to assign.
	 *  @return  The number of characters assigned.
	 *
	 *  Fills @a __s[0] through @a __s[__n-1] with characters from the input
	 *  sequence, as if by @c sbumpc().  Stops when either @a __n characters
	 *  have been copied, or when @c traits::eof() would be copied.
	 *
	 *  It is expected that derived classes provide a more efficient
	 *  implementation by overriding this definition.
	 */
	virtual streamsize xsgetn(char_type* __s, streamsize __n)
	{
		cout<<"xsgetn() called."<<endl;
		streamsize __ret = 0;
		while (__ret < __n)
		{
			const streamsize __buf_len = this->egptr() - this->gptr();
			if (__buf_len)
			{
				const streamsize __remaining = __n - __ret;
				const streamsize __len = std::min(__buf_len, __remaining);
				traits_type::copy(__s, this->gptr(), __len);
				__ret += __len;
				__s += __len;
				this->__safe_gbump(__len);
			}

			if (__ret < __n)
			{
				const int_type __c = this->uflow();
				if (!traits_type::eq_int_type(__c, traits_type::eof()))
				{
					traits_type::assign(*__s++, traits_type::to_char_type(__c));
					++__ret;
				}
				else
					break;
			}
		}
		return __ret;
	}

	/**
	 *  @brief  Fetches more data from the controlled sequence.
	 *  @return  The first character from the <em>pending sequence</em>.
	 *
	 *  Informally, this function is called when the input buffer is
	 *  exhausted (or does not exist, as buffering need not actually be
	 *  done).  If a buffer exists, it is @a refilled.  In either case, the
	 *  next available character is returned, or @c traits::eof() to
	 *  indicate a null pending sequence.
	 *
	 *  For a formal definition of the pending sequence, see a good text
	 *  such as Langer & Kreft, or [27.5.2.4.3]/7-14.
	 *
	 *  A functioning input streambuf can be created by overriding only
	 *  this function (no buffer area will be used).  For an example, see
	 *  http://gcc.gnu.org/onlinedocs/libstdc++/manual/bk01pt11ch25.html
	 *
	 *  @note  Base class version does nothing, returns eof().
	 */
	virtual int_type underflow()
	{
		cout << "underflow() called." << endl;
		//return traits_type::eof();
		_M_in_cur=_M_in_beg;
		return (int)(*_M_in_cur);
	}

	/**
	 *  @brief  Fetches more data from the controlled sequence.
	 *  @return  The first character from the <em>pending sequence</em>.
	 *
	 *  Informally, this function does the same thing as @c underflow(),
	 *  and in fact is required to call that function.  It also returns
	 *  the new character, like @c underflow() does.  However, this
	 *  function also moves the read position forward by one.
	 */
	virtual int_type uflow()
	{
		cout << "uflow() called." << endl;
		int_type __ret = traits_type::eof();
		const bool __testeof = traits_type::eq_int_type(this->underflow(),
				__ret);
		if (!__testeof)
		{
			__ret = traits_type::to_int_type(*this->gptr());
			this->gbump(1);
		}
		return __ret;
	}

	// [27.5.2.4.4] putback
	/**
	 *  @brief  Tries to back up the input sequence.
	 *  @param  __c  The character to be inserted back into the sequence.
	 *  @return  eof() on failure, <em>some other value</em> on success
	 *  @post  The constraints of @c gptr(), @c eback(), and @c pptr()
	 *         are the same as for @c underflow().
	 *
	 *  @note  Base class version does nothing, returns eof().
	 */
	virtual int_type pbackfail(int_type __c = traits_type::eof())
	{
		cout<<"pbackfail() called."<<endl;
		return traits_type::eof();
	}

	// Put area:
	/**
	 *  @brief  Multiple character insertion.
	 *  @param  __s  A buffer area.
	 *  @param  __n  Maximum number of characters to write.
	 *  @return  The number of characters written.
	 *
	 *  Writes @a __s[0] through @a __s[__n-1] to the output sequence, as if
	 *  by @c sputc().  Stops when either @a n characters have been
	 *  copied, or when @c sputc() would return @c traits::eof().
	 *
	 *  It is expected that derived classes provide a more efficient
	 *  implementation by overriding this definition.
	 */
	virtual streamsize xsputn(const char_type* __s, streamsize __n)
	{
		cout<<"xsputn() called."<<endl;
		streamsize __ret = 0;
		while (__ret < __n)
		{
			const streamsize __buf_len = this->epptr() - this->pptr();
			if (__buf_len)
			{
				const streamsize __remaining = __n - __ret;
				const streamsize __len = std::min(__buf_len, __remaining);
				traits_type::copy(this->pptr(), __s, __len);
				__ret += __len;
				__s += __len;
				this->__safe_pbump(__len);
			}

			if (__ret < __n)
			{
				int_type __c = this->overflow(traits_type::to_int_type(*__s));
				if (!traits_type::eq_int_type(__c, traits_type::eof()))
				{
					++__ret;
					++__s;
				}
				else
					break;
			}
		}
		return __ret;
	}

	/**
	 *  @brief  Consumes data from the buffer; writes to the
	 *          controlled sequence.
	 *  @param  __c  An additional character to consume.
	 *  @return  eof() to indicate failure, something else (usually
	 *           @a __c, or not_eof())
	 *
	 *  Informally, this function is called when the output buffer
	 *  is full (or does not exist, as buffering need not actually
	 *  be done).  If a buffer exists, it is @a consumed, with
	 *  <em>some effect</em> on the controlled sequence.
	 *  (Typically, the buffer is written out to the sequence
	 *  verbatim.)  In either case, the character @a c is also
	 *  written out, if @a __c is not @c eof().
	 *
	 *  For a formal definition of this function, see a good text
	 *  such as Langer & Kreft, or [27.5.2.4.5]/3-7.
	 *
	 *  A functioning output streambuf can be created by overriding only
	 *  this function (no buffer area will be used).
	 *
	 *  @note  Base class version does nothing, returns eof().
	 */
	virtual int_type overflow(int_type __c = traits_type::eof())
	{
		cout << "overflow() called." << endl;
		//return traits_type::eof();

		_M_out_cur=_M_out_beg;
		(*_M_out_cur)=__c;
		_M_out_cur++;
		return traits_type::to_int_type(__c);

	}

	///////////////////////////////////////////////////////////////

//////////////////////////
};

void printCharSequence(const char* name, const char* s)
{
	cout << name << "(char): ";
	for (int i = 0; i < TestStreambuf::length; i++)
	{
		char c = s[i];
		cout << c << "\t";
	}
	cout << endl;
	cout << name << "( int): ";
	for (int i = 0; i < TestStreambuf::length; i++)
	{
		char c = s[i];
		cout << (int) ((c)) << "\t";
	}
	cout << endl;
}

void printRead(TestStreambuf& tsb)
{
	printCharSequence("Read", tsb.getRead());
}
void printWrite(TestStreambuf& tsb)
{
	printCharSequence("Write", tsb.getWrite());
}

void printNow(TestStreambuf& tsb)
{
	cout << "Read(Begin, Current, End)   :";
	cout << tsb.getReadBegin()  <<", ";
	cout << tsb.getReadCurrent()<<", ";
	cout << tsb.getReadEnd()    << endl;
	printRead(tsb);
	cout << "NowWrite(Begin, Current, End):";
	cout << tsb.getWriteBegin()  <<", ";
	cout << tsb.getWriteCurrent()<<", ";
	cout << tsb.getWriteEnd()    << endl;
	printWrite(tsb);
}

void writeString(iostream& io,const char* s)
{
	cout << "io << \""<<s<<"\" start." << endl;
	io << s;
	cout << "io << \""<<s<<"\" end." << endl;
}

void readString(iostream& io, char c)
{
	io >> c;
	cout << "ReadResult:" << c << endl;
}

int main(void)
{
	cout << "test." << endl;

	TestStreambuf tsb;
	iostream io(&tsb);
	char c=0;

	printNow(tsb);
	cout << endl;


	writeString(io, "MNOPQR");
	printNow(tsb);
	cout << endl;

	for (int i = 0; i < 2; i++)
	{
		readString(io, c);
	}
	cout << endl;
	printNow(tsb);
	cout << endl;


	writeString(io, "S");
	printNow(tsb);
	cout << endl;
	readString(io, c);
	printNow(tsb);
	cout << endl;

	writeString(io, "T");
	printNow(tsb);
	cout << endl;
	readString(io, c);
	printNow(tsb);
	cout << endl;

	writeString(io, "U");
	printNow(tsb);
	cout << endl;
	readString(io, c);
	printNow(tsb);
	cout << endl;

	writeString(io, "V");
	printNow(tsb);
	cout << endl;
	readString(io, c);
	printNow(tsb);
	cout << endl;

	writeString(io, "WXYZ");
	printNow(tsb);
	cout << endl;
	for (int i = 0; i < 4; i++)
	{
		readString(io, c);
	}
	printNow(tsb);
	cout << endl;


	return 0;
}

