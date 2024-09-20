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

#include <string>
#include <EagleNetwork/Result.hh>
#include <gtest/gtest.h>

using namespace Eagle::Core;

namespace Testing
{
    Utilities::Result<std::string, int> DoOperation(std::string data)
    {
        if(data == "valid")
            return std::string("success");

        return Utilities::MakeError(-1);
    }
}


class ResultTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto _validResult = Testing::DoOperation("valid");
        auto _invalidResult = Testing::DoOperation("invalid");

        validResult = std::move(_validResult);
        invalidResult = std::move(_invalidResult);
    }

    Utilities::Result<std::string, int> validResult;
    Utilities::Result<std::string, int> invalidResult;
};


TEST_F(ResultTest, ConstructAndCheckValidResultValues)
{
    EXPECT_TRUE(validResult.HasResult());
    EXPECT_FALSE(invalidResult.HasResult());
    EXPECT_TRUE(invalidResult.HasResult());

    EXPECT_STREQ(validResult.GetResult().c_str(), "success");

    EXPECT_EQ(invalidResult.GetError(), -1);
}
