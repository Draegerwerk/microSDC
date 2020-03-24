#pragma once

#include <cassert>
#include <memory>
#include <type_traits>

// isa<x> templates

template <class To, class From>
[[nodiscard]] inline bool isa(const From& val)
{
  if constexpr (std::is_base_of_v<To, From>)
  {
    return true;
  }
  return To::classof(&val);
}

template <class To, class From>
[[nodiscard]] inline bool isa(const From* val)
{
  assert(val != nullptr && "isa<> used on a null pointer");
  return isa<To>(*val);
}

template <class To, class From>
[[nodiscard]] inline bool isa(const std::unique_ptr<From> val)
{
  assert(val != nullptr && "isa<> used on a null pointer");
  return isa<To>(*val);
}

template <class To, class From>
[[nodiscard]] inline bool isa(const std::shared_ptr<From> val)
{
  assert(val != nullptr && "isa<> used on a null pointer");
  return isa<To>(*val);
}


// cast<x> templates

template <class To, class From>
[[nodiscard]] inline auto cast(const From& val)
{
  assert(isa<To>(val) && "cast<To>() argument of incompatible type!");
  return static_cast<const To*>(val);
}

template <class To, class From>
[[nodiscard]] inline auto cast(std::unique_ptr<From> val)
{
  assert(isa<To>(val) && "cast<To>() argument of incompatible type!");
  return std::static_pointer_cast<To>(std::move(val));
}
template <class To, class From>
[[nodiscard]] inline auto cast(std::unique_ptr<From>&& val)
{
  assert(isa<To>(val) && "cast<To>() argument of incompatible type!");
  return std::static_pointer_cast<To>(val);
}

template <class To, class From>
[[nodiscard]] inline auto cast(std::shared_ptr<From> val)
{
  assert(isa<To>(val) && "cast<To>() argument of incompatible type!");
  return std::static_pointer_cast<To>(val);
}

// dyn_cast<X> - Return the argument parameter cast to the specified type.  This
// casting operator returns null if the argument is of the wrong type, so it can
// be used to test for a type as well as cast if successful.  This should be
// used in the context of an if statement like this:
//
//  if (const Instruction *I = dyn_cast<Instruction>(myVal)) { ... }
//

template <class To, class From>
[[nodiscard]] inline auto dyn_cast(const From* val)
{
  return isa<To>(val) ? cast<const To>(val) : nullptr;
}

template <class To, class From>
[[nodiscard]] inline auto dyn_cast(std::shared_ptr<From> val)
{
  return isa<To>(val) ? cast<To>(val) : nullptr;
}

// unique_dyn_cast<X> - Given a unique_ptr<Y>, try to return a unique_ptr<X>,
// taking ownership of the input pointer iff isa<X>(Val) is true.  If the
// cast is successful, From refers to nullptr on exit and the casted value
// is returned.  If the cast is unsuccessful, the function returns nullptr
// and From is unchanged.
template <class To, class From>
[[nodiscard]] inline auto unique_dyn_cast(std::unique_ptr<From> val)
{
  return isa<To>(val) ? cast<To>(std::move(val)) : nullptr;
}

template <class To, class From>
[[nodiscard]] inline auto unique_dyn_cast(std::unique_ptr<From>&& val)
{
  return unique_dyn_cast<To, From>(val);
}
