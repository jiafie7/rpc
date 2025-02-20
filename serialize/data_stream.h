#pragma once

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "serialize/serializable.h"

namespace melon
{
  namespace serialize
  {
    class DataStream
    {
      public:
        enum DataType
        {
          bool_type = 1,
          char_type,
          int32_type,
          int64_type,
          float_type,
          double_type,
          string_type,
          vector_type,
          list_type,
          map_type,
          set_type,
          custom_type
        };

        enum ByteOrder
        {
          big_endian,
          little_endian
        };

        
        DataStream();
        ~DataStream() = default;

        void write(const char* data, int len);
        void write(bool value);
        void write(char value);
        void write(int32_t value);
        void write(int64_t value);
        void write(float value);
        void write(double value);
        void write(const char* value);
        void write(const std::string& value); 
        template <typename T>
        void write(const std::vector<T>& value);

        template <typename T>
        void write(const std::list<T>& value);
        
        template <typename K, typename V>
        void write(const std::map<K, V>& value);
        
        template <typename T>
        void write(const std::set<T>& value);

        void write(const Serializable& value);
        
        template <typename T, typename ...Args>
        void write_args(const T& head, const Args&... args);
        
        void write_args() {}
       
        DataStream& operator<<(bool value);
        DataStream& operator<<(char value);
        DataStream& operator<<(int32_t value);
        DataStream& operator<<(int64_t value);
        DataStream& operator<<(float value);
        DataStream& operator<<(double value);
        DataStream& operator<<(const char* value);
        DataStream& operator<<(const std::string& value); 
        template <typename T>
        DataStream& operator<<(const std::vector<T>& value);
        
        template <typename T>
        DataStream& operator<<(const std::list<T>& value);
        
        template <typename K, typename V>
        DataStream& operator<<(const std::map<K, V>& value);
        
        template <typename T>
        DataStream& operator<<(const std::set<T>& value);
        
        DataStream& operator<<(const Serializable& value); 
        

        void show() const;


        bool read(char* data, int len);
        bool read(bool& value);
        bool read(char& value);
        bool read(int32_t& value);
        bool read(int64_t& value);
        bool read(float& value);
        bool read(double& value);
        bool read(std::string& value);
        template <typename T>
        bool read(std::vector<T>& value);
        
        template <typename T>
        bool read(std::list<T>& value);

        template <typename K, typename V>
        bool read(std::map<K, V>& value);

        template <typename T>
        bool read(std::set<T>& value);

        bool read(Serializable& value);

        template <typename T, typename ...Args>
        bool read_args(T& head, Args& ...args);

        bool read_args() { return true; }

        DataStream& operator>>(bool& value);
        DataStream& operator>>(char& value);
        DataStream& operator>>(int32_t& value);
        DataStream& operator>>(int64_t& value);
        DataStream& operator>>(float& value);
        DataStream& operator>>(double& value);
        DataStream& operator>>(std::string& value);
        template <typename T>
        DataStream& operator>>(std::vector<T>& value);
        
        template <typename T>
        DataStream& operator>>(std::list<T>& value);

        template <typename K, typename V>
        DataStream& operator>>(std::map<K, V>& value);
        
        template <typename T>
        DataStream& operator>>(std::set<T>& value);

        DataStream& operator>>(Serializable& value);

        const char* data() const;
        int size() const;
        void clear();
        void reset();
        void save(const std::string& filename);
        void load(const std::string& filename);

      private:
        void reserve(int len);
        ByteOrder set_byte_order();

      private:
        std::vector<char> m_buffer;
        int m_index;
        ByteOrder m_byte_order;
    };

    template <typename T>
    void DataStream::write(const std::vector<T>& value)
    {
      char type = DataType::vector_type;
      write((char*)(&type), sizeof(char));

      int32_t len = value.size();
      write(len);

      for (int i = 0; i < len; ++ i)
        write(value[i]);
    }

    template <typename T>
    void DataStream::write(const std::list<T>& value)
    {
      char type = DataType::list_type;
      write((char*)(&type), sizeof(char));
      int32_t len = value.size();
      write(len);
      for (auto it = value.begin(); it != value.end(); ++ it)
        write(*it);
    }
        
    template <typename K, typename V>
    void DataStream::write(const std::map<K, V>& value)
    {
      char type = DataType::map_type;
      write((char*)(&type), sizeof(char));
      int32_t len = value.size();
      write(len);
      for (auto it = value.begin(); it != value.end(); ++ it)
      {
        write(it->first);
        write(it->second);
      }
    }
        
    template <typename T>
    void DataStream::write(const std::set<T>& value)
    {
      char type = DataType::set_type;
      write((char*)(&type), sizeof(char));
      int32_t len = value.size();
      write(len);
      for (auto it = value.begin(); it != value.end(); ++ it)
        write(*it);
    }

    template <typename T, typename ...Args>
    void DataStream::write_args(const T& head, const Args&... args)
    {
      write(head);
      write_args(args...);
    }

    template <typename T>
    DataStream& DataStream::operator<<(const std::vector<T>& value)
    {
      write(value);
      return *this;
    }
        
    template <typename T>
    DataStream& DataStream::operator<<(const std::list<T>& value)
    {
      write(value);
      return *this;
    }
        
    template <typename K, typename V>
    DataStream& DataStream::operator<<(const std::map<K, V>& value)
    {
      write(value);
      return *this;
    }
        
    template <typename T>
    DataStream& DataStream::operator<<(const std::set<T>& value)
    {
      write(value);
      return *this;
    }

    template <typename T>
    bool DataStream::read(std::vector<T>& value)
    {
      value.clear();

      if ((DataType)m_buffer[m_index] != DataType::vector_type)
        return false;
      ++ m_index;

      int32_t len = 0;
      read(len);
      
      for (int i = 0; i < len; ++ i)
      {
        T t;
        read(t);
        value.push_back(t);
      }
      return true;
    }
        
    template <typename T>
    bool DataStream::read(std::list<T>& value)
    {
      value.clear();
      if ((DataType)m_buffer[m_index] != DataType::list_type)
        return false;
      ++ m_index;
      int32_t len = 0;
      read(len);
      for (int i = 0; i < len; ++ i)
      {
        T t;
        read(t);
        value.push_back(t);
      }
      return true;
    }

    template <typename K, typename V>
    bool DataStream::read(std::map<K, V>& value)
    {
      value.clear();
      if ((DataType)m_buffer[m_index] != DataType::map_type)
        return false;
      ++ m_index;
      int32_t len = 0;
      read(len);
      for (int i = 0; i < len; ++ i)
      {
        K key;
        read(key);
        V val;
        read(val);
        value[key] = val;
      }
      return true;
    }

    template <typename T>
    bool DataStream::read(std::set<T>& value)
    {
      value.clear();
      if ((DataType)m_buffer[m_index] != DataType::set_type)
        return false;
      ++ m_index;
      int32_t len = 0;
      read(len);
      for (int i = 0; i < len; ++ i)
      {
        T t;
        read(t);
        value.insert(t);
      }
      return true;
    }

    template <typename T, typename ...Args>
    bool DataStream::read_args(T& head, Args& ...args)
    {
      read(head);
      return read_args(args...);
    }

    template <typename T>
    DataStream& DataStream::operator>>(std::vector<T>& value)
    {
      read(value);
      return *this;
    }
        
    template <typename T>
    DataStream& DataStream::operator>>(std::list<T>& value)
    {
      read(value);
      return *this;
    }

    template <typename K, typename V>
    DataStream& DataStream::operator>>(std::map<K, V>& value)
    {
      read(value);
      return *this;
    }
        
    template <typename T>
    DataStream& DataStream::operator>>(std::set<T>& value)
    {
      read(value);
      return *this;
    }
  }
}
