#include <sys/types.h>
#include <unistd.h>

#include <limits>
#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <map>

#include "../include/egg/variable.hpp"

struct X
{
  bool _v;

  X() : _v(false) {}
 ~X() noexcept {}

  X(bool v) : _v(v) {}
};

template <typename T>
void check();

template <>
void check<void>()
{
  using egg::variable;
  using std::cout;
  using std::endl;
  using std::cerr;

  cout  << "Checking default constructor" << endl
        << "---------------------------------------------------------" << endl;

  variable v;
  cout  << "Default constructor. "
        << "Is empty? "       << (v.is_empty() ? "yes" : "no")
        << ", to_string(): "  << v
        << endl;

  variable vc(v);
  cout  << "Copy constructor. "
        << "Is empty? "       << (vc.is_empty() ? "yes" : "no")
        << ", to_string(): "  << vc
        << endl;

  variable voc = v;
  cout  << "Copy operator. "
        << "Is empty? "       << (voc.is_empty() ? "yes" : "no")
        << ", to_string(): "  << voc
        << endl;

  variable vm(std::move(vc));
  cout  << "Move constructor. "
        << "Is empty? "       << (vm.is_empty() ? "yes" : "no")
        << ", to_string(): "  << vm
        << endl;

  variable vmo = std::move(voc);
  cout  << "Move operator. "
        << "Is empty? "       << (vmo.is_empty() ? "yes" : "no")
        << ", to_string(): "  << vmo
        << endl;

  cout  << "---------------------------------------------------------" << endl
        << "Done." << endl << endl;
}

template <>
void check<bool>()
{
  using egg::variable;
  using std::cout;
  using std::endl;
  using std::cerr;

  cout << "Checking 'bool' constructor" << endl;
  cout << "---------------------------------------------------------" << endl;

  variable v(true);
  cout  << "Bool constructor. "
        << "Type: " << v.type() << ", to_string(): " << v
        << endl;

  variable vc(v);
  cout  << "Copy constructor. "
        << "Type: " << vc.type() << ", to_string(): " << vc
        << endl;

  variable voc = v;
  cout  << "Copy operator. "
        << "Type: " << voc.type() << ", to_string(): " << voc
        << endl;

  variable vm(std::move(vc));
  cout  << "Move constructor. "
        << "Type: " << vm.type() << ", to_string(): "  << vm
        << endl;

  variable vmo = std::move(voc);
  cout  << "Move operator. "
        << "Type: " << vmo.type() << ", to_string(): " << vmo
        << endl;

  variable v1 = false;
  cout  << "Assign to value. "
        << "Type: " << v1.type() << ", to_string(): " << v1
        << endl;

  cout  << "---------------------------------------------------------" << endl
        << "Done." << endl << endl;
}

template <typename T>
void check()
{
  using egg::variable;
  using std::cout;
  using std::endl;
  using std::cerr;

  cout << "Checking '" << typeid(T).name() << "' constructor" << endl;
  cout << "---------------------------------------------------------" << endl;

  if (std::is_fundamental<T>::value)
  {
    if (std::is_arithmetic<T>::value)
    {
      if (std::is_floating_point<T>::value)
      {
          T x = std::numeric_limits<T>::min() + 1,
            x1 = std::numeric_limits<T>::max() - 1;

          variable v(x), v1(x1);

          cout  << "Default constructor. "
                << "Type: " << v.type() << ", to_string(): " << v
                << endl
                << "Type: " << v1.type() << ", to_string(): " << v1
                << endl;

          variable vc(v);
          cout  << "Copy constructor. "
                << "Type: " << vc.type() << ", to_string(): " << vc
                << endl;

          variable voc = v;
          cout  << "Copy operator. "
                << "Type: " << voc.type() << ", to_string(): " << voc
                << endl;

          variable vm(std::move(v));
          cout  << "Move constructor. "
                << "Type: " << vm.type() << ", to_string(): " << vm
                << endl;

          variable vmo = std::move(v1);
          cout  << "Move operator. "
                << "Type: " << vmo.type() << ", to_string(): " << vmo
                << endl;
      }
      else
      {
        if (std::is_signed<T>::value)
        {
          T x = std::numeric_limits<T>::min(),
            x1 = std::numeric_limits<T>::max();

          variable v(x), v1(x1);

          cout  << typeid(T).name()  << " constructor. "
                << "Type: " << v.type() << ", to_string(): " << v
                << endl
                << "Type: " << v1.type() << ", to_string(): " << v1
                << endl;

          variable vc(v);
          cout  << "Copy constructor. "
                << "Type: " << vc.type() << ", to_string(): " << vc
                << endl;

          variable voc = v;
          cout  << "Copy operator. "
                << "Type: " << voc.type() << ", to_string(): " << voc
                << endl;

          variable vm(std::move(v));
          cout  << "Move constructor. "
                << "Type: " << vm.type() << ", to_string(): " << vm
                << endl;

          variable vmo = std::move(v1);
          cout  << "Move operator. "
                << "Type: " << vmo.type() << ", to_string(): " << vmo
                << endl;
          }
        else
        {
            T x = 0, x1 = std::numeric_limits<T>::max();

            variable v(x), v1(x1);

            cout  << typeid(T).name() << " constructor. "
                  << "Type: " << v.type() << ", to_string(): " << v
                  << endl
                  << "Type: " << v1.type() << ", to_string(): " << v1
                  << endl;

            variable vc(v);
            cout  << "Copy constructor. "
                  << "Type: " << vc.type() << ", to_string(): " << vc
                  << endl;

            variable voc = v;
            cout  << "Copy operator. "
                  << "Type: " << voc.type() << ", to_string(): " << voc
                  << endl;

            variable vm(std::move(v));
            cout  << "Move constructor. "
                  << "Type: " << vm.type() << ", to_string(): " << vm
                  << endl;

            variable vmo = std::move(v1);
            cout  << "Move operator. "
                  << "Type: " << vmo.type() << ", to_string(): " << vmo
                  << endl;

            variable v2 = x1;
            cout  << "Move operator. "
                  << "Type: " << v2.type() << ", to_string(): " << v2
                  << endl;
        }
      }
    }
    else
    {
      throw std::invalid_argument(typeid(T).name());
    }
  }
  else
  {
    throw std::invalid_argument(typeid(T).name());
  }

  // Done
  cout  << "---------------------------------------------------------" << endl
        << "Done." << endl << endl;
}

template <>
void check<std::string>()
{
  using egg::variable;
  using std::cout;
  using std::endl;
  using std::cerr;

  cout << "Checking 'std::string' constructor" << endl;
  cout << "---------------------------------------------------------" << endl;

  std::string s("This is the simple string!");
  variable v(s);
  cout  << "String constructor. "
        << "Type: " << v.type() << ", to_string(): " << v
        << endl;

  variable vc(v);
  cout  << "Copy constructor. "
        << "Type: " << vc.type() << ", to_string(): " << vc
        << endl;

  variable voc = v;
  cout  << "Copy operator. "
        << "Type: " << voc.type() << ", to_string(): " << voc
        << endl;

  variable vm(std::move(vc));
  cout  << "Move constructor. "
        << "Type: " << vm.type() << ", to_string(): " << vm
        << endl;

  variable vmo = std::move(voc);
  cout  << "Move operator. "
        << "Type: " << vmo.type() << ", to_string(): " << vmo
        << endl;

  cout  << "---------------------------------------------------------" << endl
        << "Done." << endl << endl;
}

template <>
void check<egg::variable::stringlist>()
{
  using egg::variable;
  using std::cout;
  using std::endl;
  using std::cerr;

  cout << "Checking 'value::stringlist' constructor" << endl;
  cout << "---------------------------------------------------------" << endl;

  variable::stringlist s;
  s.push_back("One");
  s.push_back("Two");
  s.push_back("Three");
  s.push_back("Two");
  s.push_back("One");

  variable v(s);
  cout  << "StringList constructor. "
        << "Type: " << v.type() << ", to_string(): " << v
        << endl;

  variable vc(v);
  cout  << "Copy constructor. "
        << "Type: " << vc.type() << ", to_string(): "  << vc
        << endl;

  variable voc = v;
  cout  << "Copy operator. "
        << "Type: " << voc.type() << ", to_string(): " << voc
        << endl;

  variable vm(std::move(vc));
  cout  << "Move constructor. "
        << "Type: " << vm.type() << ", to_string(): " << vm
        << endl;

  variable vmo = std::move(voc);
  cout  << "Move operator. "
        << "Type: " << vmo.type() << ", to_string(): " << vmo
        << endl;

  cout  << "---------------------------------------------------------" << endl
        << "Done." << endl << endl;
}

namespace egg
{

struct EGG_PUBLIC value : public variable
{
  typedef std::map<egg::variable, egg::value> repository;

  typedef repository::iterator iterator;
  typedef repository::reverse_iterator reverse_iterator;

  typedef repository::const_iterator const_iterator;
  typedef repository::const_reverse_iterator const_reverse_iterator;

  typedef repository::mapped_type mapped_type;
  typedef repository::key_type key_type;
  typedef repository::difference_type difference_type;
  typedef repository::allocator_type allocator_type;
  typedef repository::size_type size_type;


  /// An empty value
  inline value() noexcept : variable() {}
  ~value() noexcept {}

  // Copy
  inline value(const value& other)
    : variable(other), _d(other._d)
  {}

  inline value& operator=(const value& other)
  {
    variable::operator =(other);
    _d = other._d;

    return *this;
  }

  // Move
  inline value(value&& other) noexcept
    : variable(other),
      _d(std::move(other._d))
  {}

  inline value& operator=(value&& other) noexcept
  {
    variable::operator =(other);
    _d = std::move(other._d);

    return *this;
  }

  // Build from value
  inline value(const bool value) noexcept : variable(value) {}

  inline value(const std::int8_t   value) noexcept : variable(value) {}
  inline value(const std::uint8_t  value) noexcept : variable(value) {}
  inline value(const std::int16_t  value) noexcept : variable(value) {}
  inline value(const std::uint16_t value) noexcept : variable(value) {}
  inline value(const std::int32_t  value) noexcept : variable(value) {}
  inline value(const std::uint32_t value) noexcept : variable(value) {}
  inline value(const std::int64_t  value) noexcept : variable(value) {}
  inline value(const std::uint64_t value) noexcept : variable(value) {}

  inline value(const float       value) noexcept : variable(value) {}
  inline value(const double      value) noexcept : variable(value) {}
  inline value(const long double value) noexcept : variable(value) {}

  inline value(const char*          value) : variable(value) {}
  inline value(const std::string&   value) : variable(value) {}
  inline value(const stringlist&    value) : variable(value) {}

  // Proxy hidden map
        iterator begin() noexcept				{ return _d.begin();	}
  const_iterator begin() const noexcept				{ return _d.begin();	}

        iterator end() noexcept					{ return _d.end();	}
  const_iterator end() const noexcept				{ return _d.end();	}

  reverse_iterator rbegin() noexcept				{ return _d.rbegin();	}
  const_reverse_iterator rbegin() const noexcept		{ return _d.rbegin();	}

  reverse_iterator rend() noexcept				{ return _d.rend();	}
  const_reverse_iterator rend() const noexcept			{ return _d.rend();	}

  const_iterator cbegin() const noexcept			{ return _d.cbegin();	}
  const_iterator cend() const noexcept				{ return _d.cend();	}

  const_reverse_iterator crbegin() const noexcept		{ return _d.crbegin();	}
  const_reverse_iterator crend() const noexcept			{ return _d.crend();	}

  inline bool empty() const noexcept				{ return _d.empty();	}
  inline size_type size() const noexcept			{ return _d.size();	}
  inline size_type max_size() const noexcept			{ return _d.max_size();	}

  inline mapped_type& operator[] (const key_type& k)		{ return _d[k]; }
  inline mapped_type& operator[] (key_type&& k)			{ return _d[k]; }

  inline mapped_type& at (const key_type& k)			{ return _d.at(k); }
  inline const mapped_type& at (const key_type& k) const	{ return _d.at(k); }

private:

  repository  _d;
};

} // End of egg namespace

typedef std::map<egg::variable, egg::variable> variable_map;

typedef std::map<egg::variable, egg::value> repository;

template <>
void check<variable_map>()
{
  using egg::variable;
  using std::cout;
  using std::endl;
  using std::cerr;

  cout << "Checking 'variable_map' constructor" << endl;
  cout << "---------------------------------------------------------" << endl;

  variable_map vm;
  const variable::stringlist key1 = {"one", "two", "three" };

  vm["one"] = "two";
  vm[2] = "three";
  vm[3.14] = 87634;
  vm[key1] = false;
  vm[true] = key1;

  for (auto i : vm)
  {
    cout << "vm[" << i.first.to_string() << "] = "
         << i.second.to_string() << endl;
  }

  vm[key1] = vm[true];

  cout << "Assign vm[key1] to vm[true]: "
          "vm[" << key1 << "] = " << vm[key1] << endl;

  cout  << "---------------------------------------------------------" << endl
        << "Done." << endl << endl;
}

void
recursive_dump(
    repository::iterator start,
    repository::iterator end,
    int level)
{
  using std::cout;
  using std::endl;

  for (auto i  = start; i != end; ++i)
  {
    for (auto l = 0; l < level; ++l)
	cout << "  ";

    cout << "d[" << i->first.to_string() << "] = " 
         << i->second.to_string() << endl;

    if (i->second.size())
      recursive_dump(
        (*i).second.begin(),
	(*i).second.end(),
	level + 1);
  }
}

template <>
void check<repository>()
{
  using egg::variable;
  using std::cout;
  using std::endl;
  using std::cerr;

  cout << "Checking 'repository' constructor" << endl;
  cout << "---------------------------------------------------------" << endl;

  repository vm;
  const variable::stringlist key1 = {"one", "two", "three" };

  vm["one"] = "two";
  vm["one"][1] = 0.1;
  vm["one"][0.2] = 2;
  vm["one"][3] = 0.3;
  vm[2] = "three";
  vm[3.14] = 87634;
  vm[key1] = false;
  vm[true] = key1;
  vm["cmd"]["configuration"] = "/etc/phoenix/test.xml";
  vm["cmd"]["log"]["level"] = 9;
  vm["cmd"]["log"]["file"] = "/var/log/test.log";
  
  recursive_dump(vm.begin(), vm.end(), 0);

  vm[key1] = vm[true];

  cout << "Assign vm[key1] to vm[true]: "
          "vm[" << key1 << "] = " << vm[key1] << endl;

  recursive_dump(vm.begin(), vm.end(), 0);

  cout  << "---------------------------------------------------------" << endl
        << "Done." << endl << endl;
}

void
casts()
{
  using egg::variable;
  using std::cout;
  using std::endl;
  using std::cerr;

  cout << "Checking operator=(T, variable)" << endl;
  cout << "---------------------------------------------------------" << endl;
  {
    variable v = getuid();
    uid_t __uid = v.as<uid_t>();
    cout << "UID (" << v.type() << "): " << v << ", casted: " << __uid << endl;
  }
  cout  << "---------------------------------------------------------" << endl
        << "Done." << endl << endl;
}

int
main(
  const int   argc,
  const char* argv[])
{
  using egg::variable;
  using std::cout;
  using std::endl;
  using std::cerr;

  // Default constructor, checker and stringification
  check<void>();

  // Bool constructor, checker and stringification
  check<bool>();

  // Int8 constructor, checker and stringification
  check<int8_t>();

  // UInt8 constructor, checker and stringification
  check<uint8_t>();

  // Int16 constructor, checker and stringification
  check<int16_t>();

  // UInt16 constructor, checker and stringification
  check<uint16_t>();

  // Int32 constructor, checker and stringification
  check<int32_t>();

  // UInt32 constructor, checker and stringification
  check<uint32_t>();

  // Int64 constructor, checker and stringification
  check<int64_t>();

  // UInt64 constructor, checker and stringification
  check<uint64_t>();

  // Float constructor, checker and stringification
  check<float>();

  // Double constructor, checker and stringification
  check<double>();

  // Long double constructor, checker and stringification
  check<long double>();

  // String constructor, checker and stringification
  check<std::string>();

  // String list constructor, checker and stringification
  check<variable::stringlist>();

  // Check variable as key in map
  check<variable_map>();

  // Implement sample repository
  check<repository>();

  // Check operators  and casts
  casts();

  return 0;
}

/* End of file */
