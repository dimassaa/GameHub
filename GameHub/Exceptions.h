#pragma once
#include <stdexcept>
#include <string>

using namespace std;

class AuthenticationError : public runtime_error 
{
public:
    AuthenticationError(const string& message) : runtime_error(message) {}
};

class ValidationError : public runtime_error 
{
public:
    ValidationError(const string& message) : runtime_error(message) {}
};

class FileIOException : public runtime_error 
{
public:
    FileIOException(const string& message) : runtime_error(message) {}
};

class AuthorizationError : public runtime_error 
{
public:
    AuthorizationError(const string& message) : runtime_error(message) {}
};

class BusinessLogicError : public runtime_error 
{
public:
    BusinessLogicError(const string& message) : runtime_error(message) {}
};
