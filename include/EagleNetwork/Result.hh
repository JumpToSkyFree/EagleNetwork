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

#ifndef EAGLENETWORK_RESULT_HH
#define EAGLENETWORK_RESULT_HH

#include <exception>
#include <stdexcept>
#include <type_traits>

namespace Eagle::Core::Utilities {
    /**
     * Exception thrown when trying to access uninitialized result value by the method
     * Result::GetResult.
     */
    class BadResultAccess : public std::runtime_error
    {
    public:
        BadResultAccess() : runtime_error("BadResultAccess: error can't access uninitialized result.") {}
    };

    /**
     * Exception thrown when trying to access uninitialized error value by the method
     * Result::GetResult.
     */
    class BadErrorAccess : public std::runtime_error
    {
    public:
        BadErrorAccess() : runtime_error("BadErrorAccess: error can't access uninitialized result.") {}
    };

    template <typename T>
    concept ResultValidErrorType = !(std::is_void_v<T> && std::is_reference_v<T>) && (std::is_default_constructible_v<T>
            || (std::is_integral_v<T> || std::is_floating_point_v<T>));

    /**
     * A type wrapper that handles error values.
     * @tparam T Error type.
     */
    template <ResultValidErrorType T>
    struct Error
    {
        T value;

        Error() = default;
        ~Error() = default;

        template<typename U = T> requires(ResultValidErrorType<U> && std::is_convertible_v<U, T>)
        explicit Error(U&& _value) : value(std::forward<U>(_value)) {}

        Error(const Error&) = delete;
        Error& operator=(const Error&) = delete;
        Error(Error&&) = default;
        Error& operator=(Error&&) noexcept = default;

        template <typename U = T>
            requires(std::is_integral_v<U> || std::is_floating_point_v<U>)
        Error(Error<U>&&) 
        {
        }

        template <typename U = T>
        Error& operator=(Error<U>&&)
        {

        }
    };

    /**
     * This function constructs a new Error structure with U type value.
     * @tparam U The type of the value.
     * @param value The value.
     * @return Returns a new Error structure of type U.
     */
    template <typename U>
    Error<U> MakeError(U&& value) noexcept requires(std::is_nothrow_move_constructible_v<U>)
    {
        return Error<U>(std::forward<U>(value));
    }

    template <typename T>
    concept ResultValidResultType = !(std::is_reference_v<T> && std::is_void_v<T>) && (std::is_default_constructible_v<T>
            || (std::is_integral_v<T> || std::is_floating_point_v<T>));

    template <ResultValidResultType ResultType, ResultValidErrorType ErrorType>
    class Result
    {
        bool errorInitialized{false};
        ResultType result;
        Error<ErrorType> error;

    public:
        Result() = default;
        ~Result() = default;

        Result(const Result<ResultType, ErrorType>&) = delete;
        Result& operator=(const Result<ResultType, ErrorType>&) = delete;
        Result(Result<ResultType, ErrorType>&& other) = default;
        Result& operator=(Result<ResultType, ErrorType>&& other) = default;

        template <typename T>
        Result(Result<T, ErrorType>&& other)
            requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
        {
            if(other.HasResult())
            {
                std::swap(this->result, other.result);
                other.result = 0;
            } else {
                this->error = std::move(other.error);
            }
        }

        template <typename T>
        Result& operator=(Result<T, ErrorType>&& other)
            requires(std::is_integral_v<T> || std::is_floating_point_v<T>)
        {
            if(other.HasResult())
            {
                std::swap(this->result, other.result);
                other.result = 0;
            } else {
                this->error = std::move(other.error);
            }
            return *this;
        }

        template<typename U> requires(ResultValidResultType<U> && std::is_convertible_v<U, ResultType>)
        Result(U&& value) : result(std::forward<U>(value)) {}

        template <typename U> requires(ResultValidErrorType<U> && std::is_convertible_v<U, ErrorType>)
        Result(Error<U>&& value) : errorInitialized{true}, error(std::move(value)) {}
        /**
         * This function returns the state of the value and error if initialized or not.
         * 
         * @return The state of error initialization.
         */
        inline bool HasResult() const {
            return !errorInitialized;
        }

        /**
         * This function returns the result value, in case trying to access uninitialized
         * result value an exception BadResultAccess will thrown.
         * 
         * @return Returns a result value of type const ResultType.
         */
        [[nodiscard]] ResultType GetResult() const
        {
            if(errorInitialized) throw BadResultAccess();
            return this->result;
        }

        /**
         * This function returns the error value, in case trying to access uninitialized
         * error value an exception BadResultAccess will thrown.
         * 
         * @return Returns a result value of type const ErrorType.
         */
        [[nodiscard]] ErrorType GetError() const
        {
            if(!errorInitialized) throw BadResultAccess();
            return this->error.value;
        }
    };
}

#endif // EAGLENETWORK_RESULT_HH
