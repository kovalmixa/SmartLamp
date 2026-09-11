#ifndef SINGLTON_H
#define SINGLTON_H
#pragma once

template <typename T>
class Singleton{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T& getInstance() {
        static T instance; 
        return instance;
    }

protected:
    Singleton() = default;
    virtual ~Singleton() = default; 
};
#endif