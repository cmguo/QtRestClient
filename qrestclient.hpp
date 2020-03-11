#ifndef QRESTCLIENT_HPP
#define QRESTCLIENT_HPP

#include "qrestclient.h"
#include "qrestjson.hpp"

template<template<typename> class R, typename T>
QtPromise::QPromise<T> QRestClient::request(QRestRequest & req)
{
    typedef R<T> RT;
    return request(req).then([this](QByteArray data){
        RT rt = json_.fromJson<RT>(data);
        rt.check();
        return rt.data();
    });
}

template<typename T>
QtPromise::QPromise<T> QRestClient::request(QRestRequest & req)
{
    return request(req).then([this](QByteArray data){
        return json_.fromJson<T>(data);
    });
}

#endif // QRESTCLIENT_HPP
