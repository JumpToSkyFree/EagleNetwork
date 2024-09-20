/**
 * Copyright (c) 2023 JumpToSkyFree 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EAGLENETWORK_PLATOFRMDEFS_HH
#define EAGLENETWORK_PLATOFRMDEFS_HH

#include <EagleNetwork/Utilities.hh>
#include <EagleNetwork/Result.hh>
#include <type_traits>
#include <vector>



#if defined (__unix__) || defined (__MACH__)
#include <sys/socket.h>
#elif defined (__WIN32__) || defined (__WIN64__)
#else
#error "Current platform is not supported by EagleNetwork library."
#endif


namespace Eagle::Core {
    namespace Detail {
        struct SocketPlatformError;
        struct SocketType;
        struct SocketResource;
        template <template <class, class> typename Container, typename Releaser>
        struct SocketResourceContainerized;
        struct SocketResourceReleaser;
        struct SocketResourceDependencies;

#if defined (__unix__) || defined (__MACH__)
        struct SocketPlatformErrorType : Utilities::TypeWrapper<int> {};
        struct SocketType : Utilities::TypeWrapper<int> {};
        struct SocketResourceType : Utilities::Resource<typename SocketType::Type> {};
        template <template <class, class> typename Container, typename Releaser>
        struct SocketResourceContainerizedType
            : Utilities::ResourceContainerized<Container, typename SocketResourceType::ResourceType, Releaser>
        {};
        struct SocketResourceReleaserType : Utilities::ResourceReleaser<void(SocketResourceType::ResourceType)> {};
        using SocketInitResult = Utilities::Result<SocketResourceType::ResourceType, SocketPlatformErrorType::Type>;
        struct SocketResourceDependencies
        {
            int domain;
            int type;
            int protocol;
        };
#elif defined (__WIN32__) || defined (__WIN64__)
        // TODO: Window platform definitions needed.
#else
#error "Current platform is not supported by EagleNetwork library."
#endif
        namespace IO
        {
            template <typename OperationSigniture, typename OperationDependencies>
            struct IOSocketOperationSigniture
            {
                using Sig = OperationSigniture;
                using Deps = OperationDependencies;
            };

            template <typename InputBufferType>
                requires(!std::is_pointer_v<InputBufferType>)
            struct InputSocketOperationDep
            {
                std::size_t inputNumberBytes;
                InputBufferType buffer;
            };

            template <typename OutputBufferType>
                requires(!std::is_pointer_v<OutputBufferType>)
            struct OutputSocketOperationDep
            {
                std::size_t outputNumberBytes;
                OutputBufferType buffer;
            };

            template <template <typename> typename IOSocketOperation, typename BufferType>
                requires(!std::is_class_v<BufferType>)
            struct IOSocketOperationDep
            {
                SocketResourceType::ResourceType resource;
                IOSocketOperation<BufferType> operationBuffer;
                std::size_t size;
            };
#if defined (__unix__) || defined (__MACH__)
            struct IOSocketOperationResult : Utilities::TypeWrapper<int> {};
#elif defined (__WIN32__) || defined (__WIN64__)
#else
#error "Current platform is not supported by EagleNetwork library."
#endif
        }
    }
}

#endif //EAGLENETWORK_PLATOFRMDEFS_HH
