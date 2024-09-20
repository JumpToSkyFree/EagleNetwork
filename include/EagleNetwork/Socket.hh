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

#ifndef EAGLE_NETWORK_SOCKET_HH
#define EAGLE_NETWORK_SOCKET_HH

#include "EagleNetwork/Utilities.hh"
#include <EagleNetwork/Platform/PlatofrmDefs.hh>
#include <EagleNetwork/ResourceInitializer.hh>
#include <memory>
#include <exception>
#include <stdexcept>
#include <sys/socket.h>
#include <type_traits>

namespace Eagle::Core
{
    class BasicSocketInvalidDependencies : std::runtime_error
    {
    public:
        BasicSocketInvalidDependencies()
            : runtime_error("BasicSocket: Invalid dependendencies to initialize socket") {}
    };

    class BasicSocket
    {
    public:
        using ResourceInitializerType = ResourceInitializer<Detail::SocketResourceType::ResourceType,
            Detail::SocketPlatformErrorType::Type,
            Detail::SocketResourceDependencies>;

        explicit BasicSocket(Detail::SocketResourceType resource);
        BasicSocket& operator=(const Detail::SocketResourceType& resource);

        explicit BasicSocket(ResourceInitializerType initializer);
        BasicSocket& operator=(ResourceInitializerType initializer);

        BasicSocket(const BasicSocket&) = delete;
        BasicSocket& operator=(const BasicSocket&) = delete;
        BasicSocket(const BasicSocket&&) = default;
        BasicSocket& operator=(const BasicSocket&&) = default;

        BasicSocket();
        ~BasicSocket();

        bool OpenSocket(Detail::SocketResourceDependencies& dependencies);
        Detail::SocketResourceType::ResourceType GetSocket();
        bool CloseSocket();

        Detail::SocketResourceType::ResourceType GetSocketResource();

    private:
        struct BasicSocketImpl;
        std::unique_ptr<BasicSocketImpl> impl;
    };
}

#endif
