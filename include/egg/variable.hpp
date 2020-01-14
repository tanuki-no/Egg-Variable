/*!
 *	\file		variable.hpp
 *	\brief		Declares variable
 *	\author		Vladislav "Tanuki" Mikhailikov \<vmikhailikov\@gmail.com\>
 *	\copyright	GNU GPL v3
 *	\date		14/01/2020
 *	\version	1.0
 */

#ifndef EGG_VARIABLE
#define EGG_VARIABLE

#include <string>
#include <vector>
#include <ostream>

#include <egg/common.hpp>


namespace egg
{

// Inspired by QVariable (Qt) and DocOpt
struct EGG_PUBLIC variable
{

	enum class content : std::uint8_t
	{
		is_empty	= 0,

		is_bool		= 1,

		is_int8		= 2,
		is_uint8	= 3,

		is_int16	= 4,
		is_uint16	= 5,

		is_int32	= 6,
		is_uint32	= 7,

		is_int64	= 8,
		is_uint64	= 9,

		is_float	= 10,
		is_double	= 11,
		is_long_double	= 12,

		is_string	= 13,
		is_string_list	= 14,

		first		= is_empty,
		last		= is_string_list + 1
	};

	typedef std::vector<std::string> stringlist;

	/// An empty value
	variable() noexcept;
	virtual ~variable() noexcept;

	// Copy
	variable(const variable& /*other*/);
	variable& operator=(const variable& /*other*/);

	// Move
	variable(variable&& /*other*/) noexcept;
	variable& operator=(variable&& /*other*/) noexcept;

	// Build from value
	variable(const bool		/*value*/) noexcept;

	variable(const std::int8_t	/*value*/) noexcept;
	variable(const std::uint8_t	/*value*/) noexcept;
	variable(const std::int16_t	/*value*/) noexcept;
	variable(const std::uint16_t	/*value*/) noexcept;
	variable(const std::int32_t	/*value*/) noexcept;
	variable(const std::uint32_t	/*value*/) noexcept;
	variable(const std::int64_t	/*value*/) noexcept;
	variable(const std::uint64_t	/*value*/) noexcept;

	variable(const float		/*value*/) noexcept;
	variable(const double		/*value*/) noexcept;
	variable(const long double	/*value*/) noexcept;

	variable(const char*		/*value*/);
	variable(const std::string&	/*value*/);
	variable(const stringlist&	/*value*/);

	// Checkers
	bool is_empty() const noexcept;
	explicit operator bool() const;

	content type() const noexcept;

	template <typename T> T as() noexcept;

	// Getters. Throws std::invalid_argument if the type does not match
	bool as_bool() const;

	std::int8_t   as_int8() const;
	std::uint8_t  as_uint8() const;

	std::int16_t  as_int16() const;
	std::uint16_t as_uint16() const;

	std::int32_t  as_int32() const;
	std::uint32_t as_uint32() const;

	std::int64_t  as_int64() const;
	std::uint64_t as_uint64() const;

	float         as_float() const;
	double        as_double() const;
	long double   as_long_double() const;

	const std::string&  as_string() const;
	const stringlist&   as_string_list() const;

	// Hashing
	std::uint32_t hash() const noexcept;

	// equality is based on hash-equality
	bool operator == (const variable&) noexcept;
	bool operator != (const variable&) noexcept;

	// To string
	std::string to_string() const;
	const std::string& to_type_string() const noexcept;

	// Reset
	void reset() noexcept;

private:

	union EGG_PRIVATE variant
	{
		variant() noexcept;
		~variant() noexcept;

		variant(const variant&) = delete;
		variant& operator=(const variant&) = delete;

		variant(variant&&) = delete;
		variant& operator=(variant&&) = delete;

		std::ptrdiff_t	_d;
		void*		_pointer; // Since void* is 64 bit long, put everything else as pointer

		bool            _bool;

		std::int8_t	_int8;
		std::uint8_t	_uint8;

		std::int16_t	_int16;
		std::uint16_t	_uint16;

		std::int32_t	_int32;
		std::uint32_t	_uint32;

		std::int64_t	_int64;
		std::uint64_t	_uint64;
	};

	static const std::string& type_as_string(content);

	EGG_PRIVATE void
	throw_if_not_type(
		content expected) const;

	EGG_PRIVATE void __rehash();

private:

	content		_type;
	variant		_data;
	std::uint32_t	_hash;
};

} // End of egg namespace

namespace std
{

// Cast
template <>
struct hash<egg::variable>
{

  size_t
  operator()(
      const egg::variable& v) const noexcept
  {
    return v.hash();
  }

};

template<>
struct less<egg::variable>
{

  bool operator() (
      const egg::variable& lhs,
      const egg::variable& rhs) const
  {
    return lhs.hash() < rhs.hash();
  }

};

inline std::ostream&
operator<<(
    std::ostream&               str,
    const egg::variable&  v)
{
  str << v.to_string();

  return str;
}

std::ostream&
operator<<(
    std::ostream&               str,
    const egg::variable::content&  c);
}

namespace egg
{

inline variable::operator bool() const
{
  return _type != content::is_empty;
}

inline bool variable::is_empty() const noexcept
{
  return _type == content::is_empty;
}

inline variable::content variable::type() const noexcept
{
  return _type;
}

inline bool
variable::operator != (const variable& other) noexcept
{
  return !(*this == other);
}

inline const std::string&
variable::to_type_string() const noexcept
{
  return type_as_string(_type);
}

// As
template <> inline bool variable::as<bool>() noexcept { return as_bool(); }

template <> inline std::int8_t variable::as<std::int8_t>() noexcept { return as_int8(); }
template <> inline std::uint8_t variable::as<std::uint8_t>() noexcept { return as_uint8(); }
template <> inline std::int16_t variable::as<std::int16_t>() noexcept { return as_int16(); }
template <> inline std::uint16_t variable::as<std::uint16_t>() noexcept { return as_uint16(); }
template <> inline std::int32_t variable::as<std::int32_t>() noexcept { return as_int32(); }
template <> inline std::uint32_t variable::as<std::uint32_t>() noexcept { return as_uint32(); }
template <> inline std::int64_t variable::as<std::int64_t>() noexcept { return as_int64(); }
template <> inline std::uint64_t variable::as<std::uint64_t>() noexcept { return as_uint64(); }

template <> inline float variable::as<float>() noexcept { return as_float(); }
template <> inline double variable::as<double>() noexcept { return as_double(); }
template <> inline long double variable::as<long double>() noexcept { return as_long_double(); }

template <> inline std::string variable::as<std::string>() noexcept { return as_string(); }
template <> inline variable::stringlist variable::as<variable::stringlist>() noexcept { return as_string_list(); }

template <typename T>
inline T
variable::as() noexcept
{
  return (std::is_pod<T>::value ? 0 : T());
}

} // End of egg namespace

#endif  // EGG_VARIABLE

/* End of file */
