// Defines a templated Singleton class

#pragma once

// You must place this macro inside the class definition of your derived Singleton
#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;

template <class T>
class Singleton
{
private:
    
protected:
    Singleton() {}
    static T* _instance;
public:
    static T* get()
    {
        if (_instance)
        {
            return _instance;
        }
        else
        {
//            _instance = new T();
            return nullptr;
        }
    }
};

template <class T> T* Singleton<T>::_instance = 0;

