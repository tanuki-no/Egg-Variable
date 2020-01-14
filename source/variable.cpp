/*!
 *	\file		variable.cpp
 *	\brief		Implements variable
 *	\author		Vladislav "Tanuki" Mikhailikov \<vmikhailikov\@gmail.com\>
 *	\copyright	GNU GPL v3
 *	\date		14/01/2020
 *	\version	1.0
 */

#include <limits>
#include <functional>

#include <egg/variable.hpp>


namespace egg
{

static const std::uint32_t _cs_hash = std::hash<void *>()(nullptr);

static const std::string _cs_type_to_string[] =
{
  "empty",

  "bool",

  "int8",  "uint8",
  "int16", "uint16",
  "int32", "uint32",
  "int64", "uint64",

  "float", "double", "long double",

  "string", "string list",

  "unknown"
};

// Union
variable::variant::variant() noexcept
  : _d(0)
{}

variable::variant::~variant() noexcept
{}

// Construct/destruct
variable::variable() noexcept
  : _type(content::is_empty),
    _hash(_cs_hash)
{
}

variable::~variable() noexcept
{
  reset();
}

// Copy
variable::variable(
    const variable& other)
  : _type(other._type),
    _hash(other._hash)
{
  if (_type != content::is_empty)
  {
    switch (_type)
    {
      case content::is_bool:
      case content::is_int8:
      case content::is_uint8:
      case content::is_int16:
      case content::is_uint16:
      case content::is_int32:
      case content::is_uint32:
      case content::is_int64:
      case content::is_uint64:
        _data._d = other._data._d;
        break;

      default:
      {
        if (other._data._pointer != nullptr)
        {
          if (_type == content::is_float)
            _data._pointer = new float(
              *reinterpret_cast<float *>(other._data._pointer));

          else if (_type == content::is_double)
            _data._pointer = new double(
              *reinterpret_cast<double *>(other._data._pointer));

          else if (_type == content::is_long_double)
            _data._pointer = new long double(
              *reinterpret_cast<long double *>(other._data._pointer));

          else if (_type == content::is_string)
            _data._pointer = new std::string(
              *reinterpret_cast<std::string *>(other._data._pointer));

          else if (_type == content::is_string_list)
            _data._pointer = new stringlist(
              *reinterpret_cast<stringlist *>(other._data._pointer));
        }
        else
          throw std::runtime_error(
            "Null pointer value. Copy-constructor failed.");
      }
    }
  }
}

variable&
variable::operator=(
    const variable& other)
{
  if (this != &other)
  {
    reset();

    _type = other._type;
    _hash = other._hash;

    if (_type != content::is_empty)
    {
      switch (_type)
      {
        case content::is_bool:
        case content::is_int8:
        case content::is_uint8:
        case content::is_int16:
        case content::is_uint16:
        case content::is_int32:
        case content::is_uint32:
        case content::is_int64:
        case content::is_uint64:
          _data._uint64 = other._data._uint64;
          break;

        default:
        {
          if (other._data._pointer != nullptr)
          {
            if (_type == content::is_float)
              _data._pointer = new float(
                *reinterpret_cast<float *>(other._data._pointer));

            else if (_type == content::is_double)
              _data._pointer = new double(
                *reinterpret_cast<double *>(other._data._pointer));

            else if (_type == content::is_long_double)
              _data._pointer = new long double(
                *reinterpret_cast<long double *>(other._data._pointer));

            else if (_type == content::is_string)
              _data._pointer = new std::string(
                *reinterpret_cast<std::string *>(other._data._pointer));

            else if (_type == content::is_string_list)
              _data._pointer = new stringlist(
                *reinterpret_cast<stringlist *>(other._data._pointer));
          }
          else
            throw std::runtime_error(
              "Null pointer value. Copy-operator failed.");
        }
      }
    }
  }

  return *this;
}

// Move
variable::variable(
    variable&& other) noexcept
  : _type(other._type),
    _hash(other._hash)
{
  _data._d = other._data._d;

  other._type = content::is_empty;
  other._hash = _cs_hash;
  other._data._d = 0;
}

variable&
variable::operator=(
    variable&& other) noexcept
{
  if (this != &other)
  {
    reset();

    _type = other._type;
    _hash = other._hash;
    _data._d = other._data._d;

    other._type = content::is_empty;
    other._hash = _cs_hash;
    other._data._d = 0;
  }

  return *this;
}

// Create from value
variable::variable(const bool v) noexcept
  : _type(content::is_bool),
    _hash(_cs_hash)
{
  _data._bool = v;
  __rehash();
}

variable::variable(const std::int8_t v) noexcept
  : _type(content::is_int8),
    _hash(_cs_hash)
{
  _data._int8 = v;
  __rehash();
}

variable::variable(const std::uint8_t v) noexcept
  : _type(content::is_uint8),
    _hash(_cs_hash)
{
  _data._uint8 = v;
  __rehash();
}

variable::variable(const std::int16_t v) noexcept
  : _type(content::is_int16),
    _hash(_cs_hash)
{
  _data._int16 = v;
  __rehash();
}

variable::variable(const std::uint16_t v) noexcept
  : _type(content::is_uint16),
    _hash(_cs_hash)
{
  _data._uint16 = v;
  __rehash();
}

variable::variable(const std::int32_t v) noexcept
  : _type(content::is_int32),
    _hash(_cs_hash)
{
  _data._int32 = v;
  __rehash();
}

variable::variable(const std::uint32_t v) noexcept
  : _type(content::is_uint32),
    _hash(_cs_hash)
{
  _data._uint32 = v;
  __rehash();
}

variable::variable(const std::int64_t v) noexcept
  : _type(content::is_int64),
    _hash(_cs_hash)
{
  _data._int64 = v;
  __rehash();
}

variable::variable(const std::uint64_t v) noexcept
  : _type(content::is_uint64),
    _hash(_cs_hash)
{
  _data._uint64 = v;
  __rehash();
}

variable::variable(const float v) noexcept
  : _type(content::is_float),
    _hash(_cs_hash)
{
  _data._pointer = new float(v);
  __rehash();
}

variable::variable(const double v) noexcept
  : _type(content::is_double),
    _hash(_cs_hash)
{
  _data._pointer = new double(v);
  __rehash();
}

variable::variable(const long double v) noexcept
  : _type(content::is_long_double),
    _hash(_cs_hash)
{
  _data._pointer = new long double(v);
  __rehash();
}

variable::variable(const char* v)
  : _type(v == nullptr ? content::is_empty : content::is_string),
    _hash(_cs_hash)
{
  if (v != nullptr)
  {
    _data._pointer = new std::string(v);
    __rehash();
  }
}

variable::variable(const std::string& v)
  : _type(content::is_string),
    _hash(_cs_hash)
{
  _data._pointer = new std::string(v);
  __rehash();
}

variable::variable(
    const variable::stringlist& v)
  : _type(content::is_string_list),
    _hash(_cs_hash)
{
  _data._pointer = new stringlist(v);
  __rehash();
}

// Compare
bool
variable::operator == (
    const variable& other) noexcept
{
  if (_type != other._type)
    return false;

  if (_type == content::is_empty)
    return true;

  else if (_type == content::is_bool)
    return _data._bool == other._data._bool;

  else if (_type == content::is_int8)
    return _data._int8 == other._data._int8;

  else if (_type == content::is_uint8)
    return _data._uint8 == other._data._uint8;

  else if (_type == content::is_int16)
    return _data._int16 == other._data._int16;

  else if (_type == content::is_uint16)
    return _data._uint16 == other._data._uint16;

  else if (_type == content::is_int32)
    return _data._int32 == other._data._int32;

  else if (_type == content::is_uint32)
    return _data._uint32 == other._data._uint32;

  else if (_type == content::is_int64)
    return _data._int64 == other._data._int64;

  else if (_type == content::is_uint64)
    return _data._uint64 == other._data._uint64;

  if (_data._pointer == nullptr &&
      other._data._pointer == nullptr)
    return true;

  if (_data._pointer != nullptr &&
      other._data._pointer != nullptr)
  {
    if (_type == content::is_float)
      return *reinterpret_cast<float *>(_data._pointer) ==
          *reinterpret_cast<float *>(other._data._pointer);

    else if (_type == content::is_double)
      return *reinterpret_cast<double *>(_data._pointer) ==
          *reinterpret_cast<double *>(other._data._pointer);

    else if (_type == content::is_long_double)
      return *reinterpret_cast<long double *>(_data._pointer) ==
          *reinterpret_cast<long double *>(other._data._pointer);

    else if (_type == content::is_string)
      return *reinterpret_cast<std::string *>(_data._pointer) ==
          *reinterpret_cast<std::string *>(other._data._pointer);

    else if (_type == content::is_string_list)
      return *reinterpret_cast<stringlist *>(_data._pointer) ==
          *reinterpret_cast<stringlist *>(other._data._pointer);
  }

  return false;
}

// Stringify
std::string
variable::to_string() const
{
  if (_type == content::is_bool)
    return (_data._bool ? "true" : "false");

  else if (_type == content::is_int8)
    return std::to_string(_data._int8);

  else if (_type == content::is_uint8)
    return std::to_string(_data._uint8);

  else if (_type == content::is_int16)
    return std::to_string(_data._int16);

  else if (_type == content::is_uint16)
    return std::to_string(_data._uint16);

  else if (_type == content::is_int32)
    return std::to_string(_data._int32);

  else if (_type == content::is_uint32)
    return std::to_string(_data._uint32);

  else if (_type == content::is_int64)
    return std::to_string(_data._int64);

  else if (_type == content::is_uint64)
    return std::to_string(_data._uint64);

  else if (_data._pointer != nullptr)
  {
    if (_type == content::is_float)
      return std::to_string(*reinterpret_cast<float *>(_data._pointer));

    else if (_type == content::is_double)
      return std::to_string(*reinterpret_cast<double *>(_data._pointer));

    else if (_type == content::is_long_double)
      return std::to_string(*reinterpret_cast<long double *>(_data._pointer));

    else if (_type == content::is_string)
      return *reinterpret_cast<std::string *>(_data._pointer);

    else if (_type == content::is_string_list)
    {
      stringlist* slp = reinterpret_cast<stringlist *>(_data._pointer);
      std::string result;

      if (slp->size())
      {
        result = *(slp->cbegin());
        for (auto s = ++slp->cbegin(); s != slp->cend(); ++s)
          result += ',' + *s;
      }

      return result;
    }
  }

  return "<empty>";
}

// Value getters
bool
variable::as_bool() const
{
  throw_if_not_type(content::is_bool);
  return _data._bool;
}

std::int8_t
variable::as_int8() const
{
  // Try to convert string to int8
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const long result = std::stol(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    const std::int8_t _min = std::numeric_limits<std::int8_t>::min(),
                      _max = std::numeric_limits<std::int8_t>::max();

    if (result < _min)
      throw std::invalid_argument(*s + " less than permitted minimal value " + std::to_string(_min));

    if (result > _max)
      throw std::invalid_argument(*s + " more than permitted maximum value " + std::to_string(_max));

    return static_cast<std::int8_t>(result);
  }

  throw_if_not_type(content::is_int8);
  return _data._int8;
}

std::uint8_t
variable::as_uint8() const
{
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const unsigned long result = std::stoul(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    const std::uint8_t _max = std::numeric_limits<std::uint8_t>::max();

    if (result > _max)
      throw std::invalid_argument(*s + " more than permitted maximum value " + std::to_string(_max));

    return static_cast<std::uint8_t>(result);
  }

  throw_if_not_type(content::is_uint8);
  return _data._uint8;
}

std::int16_t
variable::as_int16() const
{
  // Try to convert string to int16
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const long result = std::stol(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    const std::int16_t _min = std::numeric_limits<std::int16_t>::min(),
                       _max = std::numeric_limits<std::int16_t>::max();

    if (result < _min)
      throw std::invalid_argument(*s + " less than permitted minimal value " + std::to_string(_min));

    if (result > _max)
      throw std::invalid_argument(*s + " more than permitted maximum value " + std::to_string(_max));

    return static_cast<std::int16_t>(result);
  }

  throw_if_not_type(content::is_int16);
  return _data._int16;
}

std::uint16_t
variable::as_uint16() const
{
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const unsigned long result = std::stoul(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    const std::uint16_t _max = std::numeric_limits<std::uint16_t>::max();

    if (result > _max)
      throw std::invalid_argument(*s + " more than permitted maximum value " + std::to_string(_max));

    return static_cast<std::uint16_t>(result);
  }

  throw_if_not_type(content::is_uint16);
  return _data._uint16;
}

std::int32_t
variable::as_int32() const
{
  // Try to convert string to int32
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const long result = std::stol(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    const std::int32_t _min = std::numeric_limits<std::int32_t>::min(),
                       _max = std::numeric_limits<std::int32_t>::max();

    if (result < _min)
      throw std::invalid_argument(*s + " less than permitted minimal value " + std::to_string(_min));

    if (result > _max)
      throw std::invalid_argument(*s + " more than permitted maximum value " + std::to_string(_max));

    return static_cast<std::int32_t>(result);
  }

  throw_if_not_type(content::is_int32);
  return _data._int32;
}

std::uint32_t
variable::as_uint32() const
{
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const unsigned long result = std::stoul(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    const std::uint32_t _max = std::numeric_limits<std::uint32_t>::max();

    if (result > _max)
      throw std::invalid_argument(*s + " more than permitted maximum value " + std::to_string(_max));

    return static_cast<std::uint32_t>(result);
  }

  throw_if_not_type(content::is_uint32);
  return _data._uint32;
}

std::int64_t
variable::as_int64() const
{
  // Try to convert string to int64
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const long long result = std::stoll(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    const std::int64_t _min = std::numeric_limits<std::int64_t>::min(),
                       _max = std::numeric_limits<std::int64_t>::max();

    if (result < _min)
      throw std::invalid_argument(*s + " less than permitted minimal value " + std::to_string(_min));

    if (result > _max)
      throw std::invalid_argument(*s + " more than permitted maximum value " + std::to_string(_max));

    return static_cast<std::int64_t>(result);
  }

  throw_if_not_type(content::is_int64);
  return _data._int64;
}

std::uint64_t
variable::as_uint64() const
{
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const unsigned long long result = std::stoull(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    const std::uint64_t _max = std::numeric_limits<std::uint64_t>::max();

    if (result > _max)
      throw std::invalid_argument(*s + " more than permitted maximum value " + std::to_string(_max));

    return static_cast<std::uint64_t>(result);
  }

  throw_if_not_type(content::is_uint64);
  return _data._uint64;
}

float
variable::as_float() const
{
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const float result = std::stof(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    return result;
  }

  throw_if_not_type(content::is_float);

  return (_data._pointer == nullptr ? 0.0 : *reinterpret_cast<float *>(_data._pointer));
}

double
variable::as_double() const
{
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const double result = std::stod(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    return result;
  }

  throw_if_not_type(content::is_double);

  return (_data._pointer == nullptr ? 0.0 : *reinterpret_cast<double *>(_data._pointer));
}

long double
variable::as_long_double() const
{
  if (_type == content::is_string)
  {
    const std::string* s = reinterpret_cast<std::string *>(_data._pointer);
    std::string::size_type position = 0;
    const long double result = std::stold(*s, &position); // Throws if it can't convert

    if (position != s->length())
      throw std::invalid_argument(*s + " contains non-numeric characters.");

    return result;
  }

  throw_if_not_type(content::is_long_double);

  return (_data._pointer == nullptr ? 0.0 : *reinterpret_cast<long double *>(_data._pointer));
}

const std::string&
variable::as_string() const
{
  static const std::string _default;

  throw_if_not_type(content::is_string);

  return (_data._pointer == nullptr ? _default : *reinterpret_cast<std::string *>(_data._pointer));
}

const variable::stringlist&
variable::as_string_list() const
{
  static const stringlist _default;

  throw_if_not_type(content::is_string_list);

  return (_data._pointer == nullptr ? _default : *reinterpret_cast<stringlist *>(_data._pointer));
}

std::uint32_t
variable::hash() const noexcept
{
  return _hash;
}

// Internals
const std::string&
variable::type_as_string(
    content t)
{
  if (t < content::last)
    return _cs_type_to_string[static_cast<int>(t)];

  return _cs_type_to_string[static_cast<int>(content::last)];
}

void
variable::throw_if_not_type(
    content expected) const
{
  if (expected == _type)
    return;

  std::string msg = "Illegal cast to ";
  msg += type_as_string(expected);
  msg += ", while the value type is ";
  msg += type_as_string(_type);

  throw std::invalid_argument(std::move(msg));
}

// Rehash
void
variable::__rehash()
{
  switch (_type)
  {
    case content::is_bool:
      _hash = std::hash<bool>()(_data._bool);
      break;
    case content::is_int8:
      _hash = std::hash<std::int8_t>()(_data._int8);
      break;
    case content::is_uint8:
      _hash = std::hash<std::uint8_t>()(_data._uint8);
      break;
    case content::is_int16:
      _hash = std::hash<std::int16_t>()(_data._int16);
      break;
    case content::is_uint16:
      _hash = std::hash<std::uint16_t>()(_data._uint16);
      break;
    case content::is_int32:
      _hash = std::hash<std::int32_t>()(_data._int32);
      break;
    case content::is_uint32:
      _hash = std::hash<std::uint32_t>()(_data._uint32);
      break;
    case content::is_int64:
      _hash = std::hash<std::int64_t>()(_data._int64);
      break;
    case content::is_uint64:
      _hash = std::hash<std::uint64_t>()(_data._uint64);
      break;
    case content::is_float:
      _hash = std::hash<float>()(
            *reinterpret_cast<float *>(_data._pointer));
      break;
    case content::is_double:
      _hash = std::hash<double>()(
            *reinterpret_cast<double *>(_data._pointer));
      break;
    case content::is_long_double:
      _hash = std::hash<long double>()(
            *reinterpret_cast<long double *>(_data._pointer));
      break;
    case content::is_string:
      _hash = std::hash<std::string>()(
            *reinterpret_cast<std::string *>(_data._pointer));
      break;
    case content::is_string_list:
      {
        stringlist* slp = reinterpret_cast<stringlist *>(_data._pointer);
        _hash = std::hash<std::uint32_t>()(slp->size());
        std::hash<std::string> hasher;

        for(auto const s : *slp)
          _hash ^= hasher(s) + 0x9e3779b9 + (_hash << 6) + (_hash >> 2);
      }
      break;
    default:
      _hash = _cs_hash;
  }
}

void
variable::reset() noexcept
{
  if (_type != content::is_empty)
  {
    if (_type == content::is_float)
      delete reinterpret_cast<float *>(_data._pointer);
    else if (_type == content::is_double)
      delete reinterpret_cast<double *>(_data._pointer);
    else if (_type == content::is_long_double)
      delete reinterpret_cast<long double *>(_data._pointer);
    else if (_type == content::is_string)
      delete reinterpret_cast<std::string *>(_data._pointer);
    else if (_type == content::is_string_list)
      delete reinterpret_cast<stringlist *>(_data._pointer);

    _type = content::is_empty;
    _data._pointer = nullptr;
    _hash = _cs_hash;
  }
}

} // End of egg namespace

namespace std
{

std::ostream&
operator<<(
  std::ostream&                       str,
  const egg::variable::content& c)
{
  if (c < egg::variable::content::last)
    str << egg::_cs_type_to_string[static_cast<int>(c)];
  else
    str << egg::_cs_type_to_string[static_cast<int>(
        egg::variable::content::last)];

  return str;
}

}

/* End of file */
