#pragma once

#include <functional>
#include <initializer_list>
#include <map>
#include <string>

#include "serialize/data_stream.h"
using namespace melon::serialize;

namespace melon
{
  namespace rpc
  {
    class Function
    {
    public:
      Function() = default;
      ~Function() = default;

      // bind function name with concrete function
      template <typename F>
      void bind(const std::string& name, F func);
      
      void call(const std::string& name, DataStream& in, DataStream& out);

    private:
      template <typename F>
      void wrap(F func, DataStream& in, DataStream& out);

      template <typename R, typename... Args>
      void wrap_impl(R(*func)(Args...), DataStream& in, DataStream& out);

      template <typename R, typename... Args>
      void wrap_impl(std::function<R(Args...)> func, DataStream& in, DataStream& out);

      template <typename Tuple, std::size_t... I>
      Tuple get_args(DataStream& ds, std::index_sequence<I...>);

      template <typename Tuple, std::size_t Id>
      void get_arg(DataStream& ds, Tuple& t);

      template<typename R, typename F, typename Tuple>
      typename std::enable_if<std::is_same<R, void>::value, int32_t>::type
      call_impl(F func, Tuple args);

      template<typename R, typename F, typename Tuple>
      typename std::enable_if<!std::is_same<R, void>::value, R>::type
      call_impl(F func, Tuple args);

      template<typename R, typename F, typename Tuple>
      auto invoke(F && func, Tuple && t);

      template<typename R, typename F, typename Tuple, std::size_t... Index>
      auto invoke_impl(F && func, Tuple && t, std::index_sequence<Index...>);
      
    private:
      std::map<std::string, std::function<void(DataStream&, DataStream&)>> m_handlers;
    };


    template <typename F>
    void Function::bind(const std::string& name, F func)
    {
      m_handlers[name] = std::bind(&Function::wrap<F>, this, func, std::placeholders::_1, std::placeholders::_2);
    }
    
    template <typename F>
    void Function::wrap(F func, DataStream& in, DataStream& out)
    {
      wrap_impl(func, in, out);
    }

    template <typename R, typename... Args>
    void Function::wrap_impl(R(*func)(Args...), DataStream& in, DataStream& out)
    {
      wrap_impl(std::function<R(Args...)>, in, out);
    }

    template <typename R, typename... Args>
    void wrap_impl(std::function<R(Args...)> func, DataStream& in, DataStream& out)
    {
      // define a alias for a tuple type
      using args_type = std::tuple<typename std::decay<Args>::type...>;
      // get size of args_type type
      constexpr auto size = std::tuple_size<typename std::decay<args_type>::type>::value;
      // extract element from DataStream object in 
      args_type args = get_args<args_type>(in, std::make_index_sequence<size>{});
     
      // call func
      auto ret = call_impl<R>(func, args);
      // output the function return result to DataStream object out
      out << ret;
    }

    template <typename Tuple, std::size_t... I>
    Tuple Function::get_args(DataStream& ds, std::index_sequence<I...>)
    {
      // extract elements from ds
      Tuple t;
      std::initializer_list<int>{((get_arg<Tuple, I>(ds, t)), 0)...};
      return t;
    }

    template <typename Tuple, std::size_t Id>
    void Function::get_arg(DataStream& ds, Tuple& t)
    {
      // get the element of id is Id from t;
      ds >> std::get<Id>(t);
    }

    // If R is void type, then the return type of call_impl is int32_t
    template<typename R, typename F, typename Tuple>
    typename std::enable_if<std::is_same<R, void>::value, int32_t>::type
    Function::call_impl(F func, Tuple args)
    {
      invoke<R>(func, args);
      return 0;
    }

    // If R isn't void type, then the return type of call_impl is R
    template<typename R, typename F, typename Tuple>
    typename std::enable_if<!std::is_same<R, void>::value, R>::type
    Function::call_impl(F func, Tuple args);
    {
      return invoke<R>(func, args);
    }

    template<typename R, typename F, typename Tuple>
    auto Function::invoke(F && func, Tuple && t)
    {
      constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
      return invoke_impl<R>(std::forward<F>(func), std::forward<Tuple>(t), std::make_index_sequence<size>{});
    }

    template<typename R, typename F, typename Tuple, std::size_t... Index>
    auto Function::invoke_impl(F && func, Tuple && t, std::index_sequence<Index...>)
    {
      // Unpack variable-length parameter pack and execute func
      return func(std::get<Index>(std::forward<Tuple>(t))...);
    }



    inline void Function::call(const std::string& name, DataStream& in, DataStream& out)
    {
      auto func = m_handlers[name];
      func(in, out);
    }
  }
}
