#include "pch.h"
#include "functions.h"
#include "CppUnitTest.h"
#include <sstream>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OSLABA4Tests
{
    TEST_CLASS(AllTests)
    {
    public:

        // Тесты для validateArguments
        TEST_METHOD(ValidateArguments_CorrectCount_ReturnsTrue)
        {
            int argc = 3;
            bool result = validateArguments(argc);
            Assert::IsTrue(result);
        }

        TEST_METHOD(ValidateArguments_TooFewArgs_ReturnsFalse)
        {
            int argc = 2;
            bool result = validateArguments(argc);
            Assert::IsFalse(result);
        }

        TEST_METHOD(ValidateArguments_TooManyArgs_ReturnsFalse)
        {
            int argc = 4;
            bool result = validateArguments(argc);
            Assert::IsFalse(result);
        }

        TEST_METHOD(ValidateArguments_ZeroArgs_ReturnsFalse)
        {
            int argc = 1;
            bool result = validateArguments(argc);
            Assert::IsFalse(result);
        }

      
        TEST_METHOD(ParseSenderId_ValidNumber)
        {
            const char* idStr = "123";
            int result = parseSenderId(idStr);
            Assert::AreEqual(123, result);
        }

        TEST_METHOD(ParseSenderId_Zero)
        {
            const char* idStr = "0";
            int result = parseSenderId(idStr);
            Assert::AreEqual(0, result);
        }

        TEST_METHOD(ParseSenderId_NegativeNumber)
        {
            const char* idStr = "-456";
            int result = parseSenderId(idStr);
            Assert::AreEqual(-456, result);
        }

        TEST_METHOD(ParseSenderId_InvalidString_ReturnsZero)
        {
            const char* idStr = "abc";
            int result = parseSenderId(idStr);
            Assert::AreEqual(0, result);
        }

        TEST_METHOD(ParseSenderId_MixedString_ReturnsPartial)
        {
            const char* idStr = "123abc";
            int result = parseSenderId(idStr);
            Assert::AreEqual(123, result);
        }

     
        TEST_METHOD(CalculateRecordCount_EmptyFile_Placeholder)
        {
            Assert::IsTrue(true);
        }

        TEST_METHOD(FindAndWriteMessage_FreeSlot_Placeholder)
        {
            Assert::IsTrue(true);
        }

        
        TEST_METHOD(MaxMessageLength_IsTwenty)
        {
            Assert::AreEqual(20, MAX_MSG_LEN);
        }

        TEST_METHOD(MessageStructSize_IsCorrect)
        {
            size_t expected_size = sizeof(int) + (MAX_MSG_LEN + 1) + sizeof(bool);
            size_t actual_size = sizeof(Message);
            Assert::IsTrue(actual_size >= expected_size);
        }
    };
}