/**
 ******************************************************************************
 * MasterLib : iOS Process Manipulation Toolkit                               *
 * File : singleton.h                                                         *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

/**
 * A singleton class other classes can inherit to provide a GetInstance method
 * @tparam T type of singleton to return
 */
template <class T>
class Singleton<T> {
public:
    static T *GetInstance() {
        return &instance_;
    }

private:
    static T instance_;
};

template <class T>
static T Singleton<T>::instance_;