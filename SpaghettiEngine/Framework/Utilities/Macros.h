#pragma once
#ifndef NOMEMORY
// if define no memory then NO!!
#include <memory>
#define CLASS_FORWARD_DECLARATION(ClassName) CLASS_FORWARD_DECLARATION_POINTER_ONLY(ClassName); typedef std::shared_ptr<ClassName> S##ClassName; typedef std::weak_ptr<ClassName> W##ClassName

#endif

#define PROPERTY(t,n)  __declspec( property (put = property__set_##n, get = property__get_##n)) t n; typedef t property__tmp_type_##n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n;typedef t property__tmp_type_##n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n;typedef t property__tmp_type_##n
#define GET(n) property__tmp_type_##n property__get_##n() 
#define SET(n) void property__set_##n(const property__tmp_type_##n& value)

#define STATIC_CONSTRUCTOR(code) struct STATICCONSTRUCTOR {STATICCONSTRUCTOR(){code}}; static inline STATICCONSTRUCTOR _STATICCONSTRUCTOR;

#define CLASS_FORWARD_DECLARATION_POINTER_ONLY(ClassName) typedef class ClassName* P##ClassName; typedef const ClassName* CP##ClassName