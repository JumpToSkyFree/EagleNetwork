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

#ifndef EAGLE_NETWORK_RESOURCE_INITIALIZER_HH
#define EAGLE_NETWORK_RESOURCE_INITIALIZER_HH

#include <EagleNetwork/Utilities.hh>
#include <EagleNetwork/Result.hh>
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace Eagle::Core {
    class ResourceInitializerInvalidResource : std::runtime_error
    {
    public:
        ResourceInitializerInvalidResource() : runtime_error("ResourceInitializer: can't get an invalid resource.") {}
    };

    class ResourceInitializerNoErrorExists : std::runtime_error
    {
    public:
        ResourceInitializerNoErrorExists() : runtime_error("ResourceInitializer: can't get an error while resource is valid.") {}
    };

    /**
     * This class will initialize a resource of type T by a custom initializer
     * defined by the user.
     * 
     * @tparam TResource The resource type.
     */
    template <typename TResource, typename TError>
        requires(std::is_default_constructible_v<TResource>)
    class ResourceInitializerWithoutDependencies
        : public Utilities::IResourceInitializer
    {

    public:
        /**
         * The resource initializer signature.
         */
        using InitializerFuncSig = Utilities::Result<TResource, TError>();

        explicit ResourceInitializerWithoutDependencies(std::function<InitializerFuncSig> initializer)
            : initializer(initializer)
        {}

        ResourceInitializerWithoutDependencies(const ResourceInitializerWithoutDependencies<TResource, TError>&) = delete;
        ResourceInitializerWithoutDependencies& operator=(const ResourceInitializerWithoutDependencies<TResource, TError>&) = delete;
        ResourceInitializerWithoutDependencies(ResourceInitializerWithoutDependencies<TResource, TError>&& other) = default;
        ResourceInitializerWithoutDependencies& operator=(ResourceInitializerWithoutDependencies<TResource, TError>&& other) = default;

        /**
         * This function will call the initializer to perform the initialization
         * of a temporary value and then return to ResourceInitializer class safe.
         */
        void InitializeResource() override
        {
            resourceInitResult = initializer();
        }

        /**
         * This function will check if the initializer successfully initalized
         * the resource.
         * 
         * @return true if the resource is initalized successfully.
         * @return false if the resource is not initialized successfully.
         */
        inline bool IsValidResource()
        {
            return this->resourceInitResult.HasResult();
        }

        /**
         * @brief Get the Actual Resrouce object.
         * 
         * @return TResource The actual resource value.
         */
        [[nodiscard]] TResource GetActualResrouce()
        {
            if(!this->IsValidResource())
            {
                throw ResourceInitializerInvalidResource();
            }

            return resourceInitResult.GetResult(); 
        }

        [[nodiscard]] TError GetActualError()
        {
            if(this->IsValidResource())
            {
                throw ResourceInitializerNoErrorExists();
            }

            return resourceInitResult.GetError();
        }

        /**
         * @brief Get the Unique Resource object.
         * 
         * @return std::unique_ptr<TResource> A unique pointer to the resource value.
         */
        [[nodiscard]] std::unique_ptr<TResource> GetUniqueResource()
            requires(std::is_pointer_v<TResource>)
        {
            if(this->IsValidResource())
            {
                return std::make_unique<TResource>(new TResource(*this->resourceInitResult.GetResult()));
            } else {
                throw ResourceInitializerInvalidResource();
            }
        }

        [[nodiscard]] std::unique_ptr<TResource> GetUniqueResource()
        {
            if(this->IsValidResource())
            {
                return std::make_unique<TResource>(new TResource(*this->resourceInitResult.GetResult()));
            } else {
                throw ResourceInitializerInvalidResource();
            }
        }

        [[nodiscard]] std::unique_ptr<TResource> GetSharedResource()
            requires(std::is_pointer_v<TResource>)
        {
            if(this->IsValidResource())
            {
                return std::make_shared<TResource>(new TResource(*this->resourceInitResult.GetResult()));
            } else {
                throw ResourceInitializerInvalidResource();
            }
        }

        [[nodiscard]] std::unique_ptr<TResource> GetSharedResource()
        {
            if(this->IsValidResource())
            {
                
                return std::make_shared<TResource>(new TResource(this->resourceInitResult.GetResult()));
            } else {
                throw ResourceInitializerInvalidResource();
            }
        }
    private:
        /**
         * The initializer function.
         */
        std::function<InitializerFuncSig> initializer;
        /**
         * The initializer resource result.
         */
        Utilities::Result<TResource, TError> resourceInitResult;
    };

    /**
     * This class will initialize a resource of type T by a custom initializer
     * defined by the user.
     * 
     * @tparam TResource The resource type.
     */
    template <typename TResource, typename TError, typename TDependencies>
        requires(std::is_default_constructible_v<TResource>)
    class ResourceInitializer
        : public Utilities::IResourceInitializer
    {
    public:
        /**
         * The resource initializer signature.
         */
        using InitializerFuncSig = Utilities::Result<TResource, TError>(const TDependencies&);

        explicit ResourceInitializer(std::function<InitializerFuncSig>&& initializer)
            : initializer(initializer)
        {}

        explicit ResourceInitializer(std::function<InitializerFuncSig>&& initializer, TDependencies& dependencies)
            : initializer(initializer), dependencies(dependencies)
        {}

        ResourceInitializer(const ResourceInitializer<TResource, TError, TDependencies>&) = delete;
        ResourceInitializer& operator=(const ResourceInitializer<TResource, TError, TDependencies>&) = delete;
        ResourceInitializer(ResourceInitializer<TResource, TError, TDependencies>&& other) = default;
        ResourceInitializer& operator=(ResourceInitializer<TResource, TError, TDependencies>&& other) = default;

        /**
         * @brief Set the resource dependencies.
         * 
         * @param deps The dependencies.
         */
        void SetResourceDependencies(const TDependencies& deps)
        {
            dependencies = deps;
        }

        /**
         * This function will call the initializer to perform the initialization
         * of a temporary value and then return to ResourceInitializer class safe.
         */
        void InitializeResource() override
        {
            resourceInitResult = initializer(dependencies);
        }

        /**
         * This function will return the resource dependencies.
         */
        inline TDependencies GetResourceDependencies() const
        {
            return dependencies;
        }

        /**
         * This function will check if the initializer successfully initalized
         * the resource.
         * 
         * @return true if the resource is initalized successfully.
         * @return false if the resource is not initialized successfully.
         */
        inline bool IsValidResource()
        {
            return this->resourceInitResult.HasResult();
        }

        [[nodiscard]] TError GetActualError()
        {
        }

        /**
         * @brief Get the Actual Resrouce object.
         * 
         * @return TResource The actual resource value.
         */
        [[nodiscard]] TResource GetActualResrouce()
        {
            if(!this->IsValidResource())
            {
                throw ResourceInitializerInvalidResource();
            }

            return resourceInitResult.GetResult(); 
        }

        /**
         * @brief Get the Unique Resource object.
         * 
         * @return std::unique_ptr<TResource> A unique pointer to the resource value.
         */
        [[nodiscard]] std::unique_ptr<TResource> GetUniqueResource()
            requires(std::is_pointer_v<TResource>)
        {
            if(!this->IsValidResource())
            {
                throw ResourceInitializerInvalidResource();
            }

            return std::make_unique<TResource>(new TResource(*this->resourceInitResult.GetResult()));
        }

        [[nodiscard]] std::unique_ptr<TResource> GetUniqueResource()
        {
            if(!this->IsValidResource())
            {
                throw ResourceInitializerInvalidResource();
            }

            return std::make_unique<TResource>(new TResource(*this->resourceInitResult.GetResult()));
        }

        [[nodiscard]] std::unique_ptr<TResource> GetSharedResource()
            requires(std::is_pointer_v<TResource>)
        {
            if(!this->IsValidResource())
            {
                throw ResourceInitializerInvalidResource();
            }

            return std::make_shared<TResource>(new TResource(*this->resourceInitResult.GetResult()));
        }

        [[nodiscard]] std::unique_ptr<TResource> GetSharedResource()
        {
            if(this->IsValidResource())
            {
                
                throw ResourceInitializerInvalidResource();
            }
            return std::make_shared<TResource>(new TResource(this->resourceInitResult.GetResult()));
        }
    private:
        /**
         * The initializer function.
         */
        std::function<InitializerFuncSig> initializer;
        /**
         * The initializer resource result.
         */
        Utilities::Result<TResource, TError> resourceInitResult;
        /**
         * The resource dependencies.
         */
        TDependencies& dependencies;

    };
}

#endif
