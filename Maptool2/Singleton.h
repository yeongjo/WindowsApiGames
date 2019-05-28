#pragma once
template<typename T>
class Singleton {
	static T *inst;
public:
	Singleton();
	~Singleton();
	static T &getIns();
};

template<typename T>
Singleton<T>::Singleton() {
}

template<typename T>
Singleton<T>::~Singleton() {
}

template<typename T>
T &Singleton<T>::getIns() {
	if (!inst) inst = new T();
	return *inst;
}

template<typename T>
T *Singleton<T>::inst = NULL;