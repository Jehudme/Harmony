#pragma once
#include <functional>

namespace Harmony::Internal {

    template <typename Tag, typename MemberPtr>
    struct PrivateMemberStealer {
        static MemberPtr stored_ptr;
    };

    template <typename Tag, typename MemberPtr>
    MemberPtr PrivateMemberStealer<Tag, MemberPtr>::stored_ptr = nullptr;

    template <typename Tag, typename MemberPtr, MemberPtr Ptr>
    struct Robber {
        Robber() {
            PrivateMemberStealer<Tag, MemberPtr>::stored_ptr = Ptr;
        }
        static Robber instance;
    };

    template <typename Tag, typename MemberPtr, MemberPtr Ptr>
    Robber<Tag, MemberPtr, Ptr> Robber<Tag, MemberPtr, Ptr>::instance;

    template <typename Tag, typename MemberPtr>
    MemberPtr steal(Tag) {
        return PrivateMemberStealer<Tag, MemberPtr>::stored_ptr;
    }
}

#define PREPARE_STEAL(Tag, ClassType, MemberName, Signature) \
    struct Tag { \
        using __Sig = Signature; \
        using PtrType = __Sig ClassType::*; \
    }; \
    template struct Harmony::Internal::Robber<Tag, Tag::PtrType, &ClassType::MemberName>;

#define CALL_PRIVATE(Obj, Tag, ...) \
    std::invoke(Harmony::Internal::steal<Tag, typename Tag::PtrType>(Tag{}), Obj, __VA_ARGS__)

#define GET_PRIVATE(Obj, Tag) \
    std::invoke(Harmony::Internal::steal<Tag, typename Tag::PtrType>(Tag{}), Obj)