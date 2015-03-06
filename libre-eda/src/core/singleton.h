#ifndef SINGLETON_H
#define SINGLETON_H


template <class T>
class Singleton
{
public:
    virtual ~Singleton()
    {
        m_instance = nullptr;
    }

    static T *instance()
    {
        if (m_instance == nullptr)
            m_instance = new T;
        return m_instance;
    }

private:
    Singleton() {}
    static T *m_instance;
};

template <class T> T *Singleton<T>::m_instance = nullptr;

#endif // SINGLETON_H
