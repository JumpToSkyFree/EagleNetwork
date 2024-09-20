//
// Created by HAMZA ALAOUY on 28/2/2023.
//

#ifndef EAGLENETWORK_UTILITIES_H
#define EAGLENETWORK_UTILITIES_H

#include <functional>
#include <type_traits>

namespace Eagle::Core::Utilities {
    template <typename T>
    struct TypeWrapper {
        using Type = T;
        using Pointer = T*;
        using ConstType = const T;
        using ConstPointer = const T*;
        using Ref = T&;
        using ConstRef = T&;
    };

    template <typename Sig>
        requires(std::is_function_v<Sig>)
    struct ResourceReleaser
    {
        using ResourceReleaserType = std::function<Sig>;
    };

    template <typename T>
    struct Resource
    {
        using ResourceType = T;
    };

    template <template <typename, typename> typename Container, typename T, typename Releaser>
    struct ResourceContainerized
    {
        using ResourceType = Container<T, Releaser>;
    };
    
    template <typename TDependencies>
    struct ResourceDependenciesContainerType
    {
        using DependenciesContainerType = TDependencies;
    };

    struct IResourceInitializer
    {
    protected:
        virtual ~IResourceInitializer() = default;

    public:
        IResourceInitializer() = default;
        virtual void InitializeResource() = 0;
    };

    struct IResourceReleaser
    {
    protected:
        virtual ~IResourceReleaser() = default;

    public:
        IResourceReleaser() = default;
        virtual void Release() = 0;
    };
}

#endif //EAGLENETWORK_UTILITIES_H
