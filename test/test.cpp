/*
 * uriparser - RFC 3986 URI parsing library
 *
 * Copyright (C) 2007, Weijia Song <songweijia@gmail.com>
 * Copyright (C) 2007, Sebastian Pipping <webmaster@hartwork.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <uriparser/Uri.h>
#include "CppTest/cpptest.h"
#include "FourSuite.h"
#include <memory>
#include <stdio.h>
#include <wchar.h>
using namespace Test;
using namespace std;



extern "C" {
UriBool uri_TESTING_ONLY_ParseIpSixA(const char * text);
UriBool uri_TESTING_ONLY_ParseIpFourA(const char * text);
}



#define URI_TEST_IP_FOUR_FAIL(x) TEST_ASSERT(URI_FALSE == uri_TESTING_ONLY_ParseIpFourA(x))
#define URI_TEST_IP_FOUR_PASS(x) TEST_ASSERT(URI_TRUE == uri_TESTING_ONLY_ParseIpFourA(x))

// Note the closing brackets! TODO
#define URI_TEST_IP_SIX_FAIL(x) TEST_ASSERT(URI_FALSE == uri_TESTING_ONLY_ParseIpSixA(x "]"))
#define URI_TEST_IP_SIX_PASS(x) TEST_ASSERT(URI_TRUE == uri_TESTING_ONLY_ParseIpSixA(x "]"))



class UriSuite : public Suite {

public:
	UriSuite() {
		TEST_ADD(UriSuite::testIpFour)
		TEST_ADD(UriSuite::testIpSixPass)
		TEST_ADD(UriSuite::testIpSixFail)
		TEST_ADD(UriSuite::testUri)
		TEST_ADD(UriSuite::testUriUserInfoHostPort1)
		TEST_ADD(UriSuite::testUriUserInfoHostPort2)
		TEST_ADD(UriSuite::testUriUserInfoHostPort3)
		TEST_ADD(UriSuite::testUriUserInfoHostPort4)
		TEST_ADD(UriSuite::testUriUserInfoHostPort5)
		TEST_ADD(UriSuite::testUriUserInfoHostPort6)
		TEST_ADD(UriSuite::testUriHostRegname)
		TEST_ADD(UriSuite::testUriHostIpFour1)
		TEST_ADD(UriSuite::testUriHostIpFour2)
		TEST_ADD(UriSuite::testUriHostIpSix1)
		TEST_ADD(UriSuite::testUriHostIpSix2)
		TEST_ADD(UriSuite::testUriHostIpFuture)
		TEST_ADD(UriSuite::testUriComponents)
		TEST_ADD(UriSuite::testUriComponents_Bug20070701)
		TEST_ADD(UriSuite::testEscaping)
		TEST_ADD(UriSuite::testUnescaping)
		TEST_ADD(UriSuite::testTrailingSlash)
		TEST_ADD(UriSuite::testAddBase)
		TEST_ADD(UriSuite::testToString)
		TEST_ADD(UriSuite::testToStringCharsRequired)
		TEST_ADD(UriSuite::testToStringCharsRequired)
		TEST_ADD(UriSuite::testNormalizeSyntaxMaskRequired)
		TEST_ADD(UriSuite::testNormalizeSyntax)
		TEST_ADD(UriSuite::testNormalizeSyntaxComponents)
		TEST_ADD(UriSuite::testNormalizeCrash_Bug20080224)
		TEST_ADD(UriSuite::testFilenameUriConversion)
		TEST_ADD(UriSuite::testCrash_FreeUriMembers_Bug20080116)
		TEST_ADD(UriSuite::testCrash_MakeOwner_Bug20080207)
		TEST_ADD(UriSuite::testQueryList)
		TEST_ADD(UriSuite::testQueryListPair)
	}

private:
	void testIpFour() {
		URI_TEST_IP_FOUR_FAIL("01.0.0.0");
		URI_TEST_IP_FOUR_FAIL("001.0.0.0");
		URI_TEST_IP_FOUR_FAIL("00.0.0.0");
		URI_TEST_IP_FOUR_FAIL("000.0.0.0");
		URI_TEST_IP_FOUR_FAIL("256.0.0.0");
		URI_TEST_IP_FOUR_FAIL("300.0.0.0");
		URI_TEST_IP_FOUR_FAIL("1111.0.0.0");
		URI_TEST_IP_FOUR_FAIL("-1.0.0.0");
		URI_TEST_IP_FOUR_FAIL("0.0.0");
		URI_TEST_IP_FOUR_FAIL("0.0.0.");
		URI_TEST_IP_FOUR_FAIL("0.0.0.0.");
		URI_TEST_IP_FOUR_FAIL("0.0.0.0.0");
		URI_TEST_IP_FOUR_FAIL("0.0..0");
		URI_TEST_IP_FOUR_FAIL(".0.0.0");

		URI_TEST_IP_FOUR_PASS("255.0.0.0");
		URI_TEST_IP_FOUR_PASS("0.0.0.0");
		URI_TEST_IP_FOUR_PASS("1.0.0.0");
		URI_TEST_IP_FOUR_PASS("2.0.0.0");
		URI_TEST_IP_FOUR_PASS("3.0.0.0");
		URI_TEST_IP_FOUR_PASS("30.0.0.0");
	}

	void testIpSixPass() {
		// Quad length
		URI_TEST_IP_SIX_PASS("abcd::");

		URI_TEST_IP_SIX_PASS("abcd::1");
		URI_TEST_IP_SIX_PASS("abcd::12");
		URI_TEST_IP_SIX_PASS("abcd::123");
		URI_TEST_IP_SIX_PASS("abcd::1234");

		// Full length
		URI_TEST_IP_SIX_PASS("2001:0db8:0100:f101:0210:a4ff:fee3:9566"); // lower hex
		URI_TEST_IP_SIX_PASS("2001:0DB8:0100:F101:0210:A4FF:FEE3:9566"); // Upper hex
		URI_TEST_IP_SIX_PASS("2001:db8:100:f101:210:a4ff:fee3:9566");
		URI_TEST_IP_SIX_PASS("2001:0db8:100:f101:0:0:0:1");
		URI_TEST_IP_SIX_PASS("1:2:3:4:5:6:255.255.255.255");

		// Legal IPv4
		URI_TEST_IP_SIX_PASS("::1.2.3.4");
		URI_TEST_IP_SIX_PASS("3:4::5:1.2.3.4");
		URI_TEST_IP_SIX_PASS("::ffff:1.2.3.4");
		URI_TEST_IP_SIX_PASS("::0.0.0.0"); // Min IPv4
		URI_TEST_IP_SIX_PASS("::255.255.255.255"); // Max IPv4

		// Zipper position
		URI_TEST_IP_SIX_PASS("::1:2:3:4:5:6:7");
		URI_TEST_IP_SIX_PASS("1::1:2:3:4:5:6");
		URI_TEST_IP_SIX_PASS("1:2::1:2:3:4:5");
		URI_TEST_IP_SIX_PASS("1:2:3::1:2:3:4");
		URI_TEST_IP_SIX_PASS("1:2:3:4::1:2:3");
		URI_TEST_IP_SIX_PASS("1:2:3:4:5::1:2");
		URI_TEST_IP_SIX_PASS("1:2:3:4:5:6::1");
		URI_TEST_IP_SIX_PASS("1:2:3:4:5:6:7::");

		// Zipper length
		URI_TEST_IP_SIX_PASS("1:1:1::1:1:1:1");
		URI_TEST_IP_SIX_PASS("1:1:1::1:1:1");
		URI_TEST_IP_SIX_PASS("1:1:1::1:1");
		URI_TEST_IP_SIX_PASS("1:1::1:1");
		URI_TEST_IP_SIX_PASS("1:1::1");
		URI_TEST_IP_SIX_PASS("1::1");
		URI_TEST_IP_SIX_PASS("::1"); // == localhost
		URI_TEST_IP_SIX_PASS("::"); // == all addresses

		// A few more variations
		URI_TEST_IP_SIX_PASS("21ff:abcd::1");
		URI_TEST_IP_SIX_PASS("2001:db8:100:f101::1");
		URI_TEST_IP_SIX_PASS("a:b:c::12:1");
		URI_TEST_IP_SIX_PASS("a:b::0:1:2:3");
	}

	void testIpSixFail() {
		// 5 char quad
		URI_TEST_IP_SIX_FAIL("::12345");

		// Two zippers
		URI_TEST_IP_SIX_FAIL("abcd::abcd::abcd");

		// Triple-colon zipper
		URI_TEST_IP_SIX_FAIL(":::1234");
		URI_TEST_IP_SIX_FAIL("1234:::1234:1234");
		URI_TEST_IP_SIX_FAIL("1234:1234:::1234");
		URI_TEST_IP_SIX_FAIL("1234:::");

		// No quads, just IPv4
		URI_TEST_IP_SIX_FAIL("1.2.3.4");
		URI_TEST_IP_SIX_FAIL("0001.0002.0003.0004");

		// Five quads
		URI_TEST_IP_SIX_FAIL("0000:0000:0000:0000:0000:1.2.3.4");

		// Seven quads
		URI_TEST_IP_SIX_FAIL("0:0:0:0:0:0:0");
		URI_TEST_IP_SIX_FAIL("0:0:0:0:0:0:0:");
		URI_TEST_IP_SIX_FAIL("0:0:0:0:0:0:0:1.2.3.4");

		// Nine quads
		URI_TEST_IP_SIX_FAIL("0:0:0:0:0:0:0:0:0");

		// Invalid IPv4 part
		URI_TEST_IP_SIX_FAIL("::ffff:001.02.03.004"); // Leading zeros
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3.1111"); // Four char octet
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3.256"); // > 255
		URI_TEST_IP_SIX_FAIL("::ffff:311.2.3.4"); // > 155
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3:4"); // Not a dot
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3"); // Missing octet
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3."); // Missing octet
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3a.4"); // Hex in octet
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3.4:123"); // Crap input

		// Nonhex
		URI_TEST_IP_SIX_FAIL("g:0:0:0:0:0:0");
	}

	void testUri() {
		UriParserStateA stateA;
		UriParserStateW stateW;
		UriUriA uriA;
		UriUriW uriW;

		stateA.uri = &uriA;
		stateW.uri = &uriW;

		// On/off for each
		TEST_ASSERT(0 == uriParseUriA(&stateA, "//user:pass@[::1]:80/segment/index.html?query#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://[::1]:80/segment/index.html?query#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://user:pass@[::1]/segment/index.html?query#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://user:pass@[::1]:80?query#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://user:pass@[::1]:80/segment/index.html#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://user:pass@[::1]:80/segment/index.html?query"));
		uriFreeUriMembersA(&uriA);

		// Schema, port, one segment
		TEST_ASSERT(0 == uriParseUriA(&stateA, "ftp://host:21/gnu/"));
		uriFreeUriMembersA(&uriA);

		// Relative
		TEST_ASSERT(0 == uriParseUriA(&stateA, "one/two/three"));
		TEST_ASSERT(!uriA.absolutePath);
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "/one/two/three"));
		TEST_ASSERT(uriA.absolutePath);
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "//user:pass@localhost/one/two/three"));
		uriFreeUriMembersA(&uriA);

		// ANSI and Unicode
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://www.example.com/"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriW(&stateW, L"http://www.example.com/"));
		uriFreeUriMembersW(&uriW);

		// Real life examples
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://sourceforge.net/projects/uriparser/"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://sourceforge.net/project/platformdownload.php?group_id=182840"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "mailto:test@example.com"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "../../"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "/"));
		TEST_ASSERT(uriA.absolutePath)
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, ""));
		TEST_ASSERT(!uriA.absolutePath)
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "file:///bin/bash"));
		uriFreeUriMembersA(&uriA);

		// Percent encoding
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://www.example.com/name%20with%20spaces/"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 != uriParseUriA(&stateA, "http://www.example.com/name with spaces/"));
		uriFreeUriMembersA(&uriA);
	}

	void testUriComponents() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0              15 01  0      7  01
		const char * const input = "http" "://" "sourceforge.net" "/" "project" "/"
		//		 0                   20 01  0              15
				"platformdownload.php" "?" "group_id=182840";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.scheme.first == input);
		TEST_ASSERT(uriA.scheme.afterLast == input + 4);
		TEST_ASSERT(uriA.userInfo.first == NULL);
		TEST_ASSERT(uriA.userInfo.afterLast == NULL);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 15);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);

		TEST_ASSERT(uriA.pathHead->text.first == input + 4 + 3 + 15 + 1);
		TEST_ASSERT(uriA.pathHead->text.afterLast == input + 4 + 3 + 15 + 1 + 7);
		TEST_ASSERT(uriA.pathHead->next->text.first == input + 4 + 3 + 15 + 1 + 7 + 1);
		TEST_ASSERT(uriA.pathHead->next->text.afterLast == input + 4 + 3 + 15 + 1 + 7 + 1 + 20);
		TEST_ASSERT(uriA.pathHead->next->next == NULL);
		TEST_ASSERT(uriA.pathTail == uriA.pathHead->next);

		TEST_ASSERT(uriA.query.first == input + 4 + 3 + 15 + 1 + 7 + 1 + 20 + 1);
		TEST_ASSERT(uriA.query.afterLast == input + 4 + 3 + 15 + 1 + 7 + 1 + 20 + 1 + 15);
		TEST_ASSERT(uriA.fragment.first == NULL);
		TEST_ASSERT(uriA.fragment.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}

	void testUriComponents_Bug20070701() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          01  01  01
		const char * const input = "a" ":" "b";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.scheme.first == input);
		TEST_ASSERT(uriA.scheme.afterLast == input + 1);
		TEST_ASSERT(uriA.userInfo.first == NULL);
		TEST_ASSERT(uriA.userInfo.afterLast == NULL);
		TEST_ASSERT(uriA.hostText.first == NULL);
		TEST_ASSERT(uriA.hostText.afterLast == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);

		TEST_ASSERT(uriA.pathHead->text.first == input + 1 + 1);
		TEST_ASSERT(uriA.pathHead->text.afterLast == input + 1 + 1 + 1);
		TEST_ASSERT(uriA.pathHead->next == NULL);
		TEST_ASSERT(uriA.pathTail == uriA.pathHead);

		TEST_ASSERT(uriA.query.first == NULL);
		TEST_ASSERT(uriA.query.afterLast == NULL);
		TEST_ASSERT(uriA.fragment.first == NULL);
		TEST_ASSERT(uriA.fragment.afterLast == NULL);

		TEST_ASSERT(!uriA.absolutePath);
		uriFreeUriMembersA(&uriA);
	}

	void testUriUserInfoHostPort1() {
		// User info with ":", no port
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0      7  01  0        9
		const char * const input = "http" "://" "abc:def" "@" "localhost";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.userInfo.first == input + 4 + 3);
		TEST_ASSERT(uriA.userInfo.afterLast == input + 4 + 3 + 7);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3 + 7 + 1);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 7 + 1 + 9);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}

	void testUriUserInfoHostPort2() {
		// User info with ":", with port
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0      7  01  0        9
		const char * const input = "http" "://" "abc:def" "@" "localhost"
		//		01   0  3
				":" "123";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.userInfo.first == input + 4 + 3);
		TEST_ASSERT(uriA.userInfo.afterLast == input + 4 + 3 + 7);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3 + 7 + 1);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 7 + 1 + 9);
		TEST_ASSERT(uriA.portText.first == input + 4 + 3 + 7 + 1 + 9 + 1);
		TEST_ASSERT(uriA.portText.afterLast == input + 4 + 3 + 7 + 1 + 9 + 1 + 3);
		uriFreeUriMembersA(&uriA);
	}

	void testUriUserInfoHostPort3() {
		// User info without ":", no port
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0      7  01  0        9
		const char * const input = "http" "://" "abcdefg" "@" "localhost";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.userInfo.first == input + 4 + 3);
		TEST_ASSERT(uriA.userInfo.afterLast == input + 4 + 3 + 7);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3 + 7 + 1);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 7 + 1 + 9);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}

	void testUriUserInfoHostPort4() {
		// User info without ":", with port
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0      7  01  0        9
		const char * const input = "http" "://" "abcdefg" "@" "localhost"
		//		01   0  3
				":" "123";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.userInfo.first == input + 4 + 3);
		TEST_ASSERT(uriA.userInfo.afterLast == input + 4 + 3 + 7);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3 + 7 + 1);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 7 + 1 + 9);
		TEST_ASSERT(uriA.portText.first == input + 4 + 3 + 7 + 1 + 9 + 1);
		TEST_ASSERT(uriA.portText.afterLast == input + 4 + 3 + 7 + 1 + 9 + 1 + 3);
		uriFreeUriMembersA(&uriA);
	}

	void testUriUserInfoHostPort5() {
		// No user info, no port
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0        9
		const char * const input = "http" "://" "localhost";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.userInfo.first == NULL);
		TEST_ASSERT(uriA.userInfo.afterLast == NULL);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 9);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}

	void testUriUserInfoHostPort6() {
		// No user info, with port
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0        9  01  0  3
		const char * const input = "http" "://" "localhost" ":" "123";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.userInfo.first == NULL);
		TEST_ASSERT(uriA.userInfo.afterLast == NULL);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 9);
		TEST_ASSERT(uriA.portText.first == input + 4 + 3 + 9 + 1);
		TEST_ASSERT(uriA.portText.afterLast == input + 4 + 3 + 9 + 1 + 3);
		uriFreeUriMembersA(&uriA);
	}

	void testUriHostRegname() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0          11
		const char * const input = "http" "://" "example.com";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.hostText.first == input + 4 + 3);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 11);
		TEST_ASSERT(uriA.hostData.ip4 == NULL);
		TEST_ASSERT(uriA.hostData.ip6 == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}

	void testUriHostIpFour1() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0      7  01  0 2
		const char * const input = "http" "://" "1.2.3.4" ":" "80";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.hostText.first == input + 4 + 3);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 7);
		TEST_ASSERT(uriA.hostData.ip4 != NULL);
		TEST_ASSERT(uriA.hostData.ip6 == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}

	void testUriHostIpFour2() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  0      7
		const char * const input = "http" "://" "1.2.3.4";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.hostText.first == input + 4 + 3);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 7);
		TEST_ASSERT(uriA.hostData.ip4 != NULL);
		TEST_ASSERT(uriA.hostData.ip6 == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}

	void testUriHostIpSix1() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  01  45  01  0 2
		const char * const input = "http" "://" "[::1]" ":" "80";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.hostText.first == input + 4 + 3 + 1);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 4);
		TEST_ASSERT(uriA.hostData.ip4 == NULL);
		TEST_ASSERT(uriA.hostData.ip6 != NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}

	void testUriHostIpSix2() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0   4  0  3  01  45
		const char * const input = "http" "://" "[::1]";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.hostText.first == input + 4 + 3 + 1);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 4);
		TEST_ASSERT(uriA.hostData.ip4 == NULL);
		TEST_ASSERT(uriA.hostData.ip6 != NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}

	void testUriHostIpFuture() {
		// TODO
	}

	bool testEscapingHelper(const wchar_t * in, const wchar_t * expectedOut,
			bool spaceToPlus = false, bool normalizeBreaks = false) {
		wchar_t * const buffer = new wchar_t[(normalizeBreaks ? 6 : 3)
				* wcslen(in) + 1];
		if (uriEscapeW(in, buffer, spaceToPlus, normalizeBreaks)
			!= buffer + wcslen(expectedOut)) {
			delete [] buffer;
			return false;
		}

		const bool equal = !wcscmp(buffer, expectedOut);
		delete [] buffer;
		return equal;
	}

	void testEscaping() {
		const bool SPACE_TO_PLUS = true;
		const bool SPACE_TO_PERCENT = false;
		const bool KEEP_UNMODIFIED = false;
		const bool NORMALIZE = true;

		// '+' to ' '
		TEST_ASSERT(testEscapingHelper(L"abc def", L"abc+def", SPACE_TO_PLUS));
		TEST_ASSERT(testEscapingHelper(L"abc def", L"abc%20def", SPACE_TO_PERCENT));

		// Percent encoding
		TEST_ASSERT(testEscapingHelper(L"\x00", L"\0"));
		TEST_ASSERT(testEscapingHelper(L"\x01", L"%01"));
		TEST_ASSERT(testEscapingHelper(L"\xff", L"%FF"));

		// Linebreak normalization
		TEST_ASSERT(testEscapingHelper(L"\x0d", L"%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"g\x0d", L"g%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0dg", L"%0D%0Ag", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0d", L"%0D", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"g\x0d", L"g%0D", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"\x0dg", L"%0Dg", SPACE_TO_PLUS, KEEP_UNMODIFIED));

		TEST_ASSERT(testEscapingHelper(L"\x0a", L"%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"g\x0a", L"g%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0ag", L"%0D%0Ag", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0a", L"%0A", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"g\x0a", L"g%0A", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"\x0ag", L"%0Ag", SPACE_TO_PLUS, KEEP_UNMODIFIED));

		TEST_ASSERT(testEscapingHelper(L"\x0d\x0a", L"%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"g\x0d\x0a", L"g%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0d\x0ag", L"%0D%0Ag", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0d\x0a", L"%0D%0A", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"g\x0d\x0a", L"g%0D%0A", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"\x0d\x0ag", L"%0D%0Ag", SPACE_TO_PLUS, KEEP_UNMODIFIED));

		TEST_ASSERT(testEscapingHelper(L"\x0a\x0d", L"%0D%0A%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"g\x0a\x0d", L"g%0D%0A%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0a\x0dg", L"%0D%0A%0D%0Ag", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0a\x0d", L"%0A%0D", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"g\x0a\x0d", L"g%0A%0D", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"\x0a\x0dg", L"%0A%0Dg", SPACE_TO_PLUS, KEEP_UNMODIFIED));
	}

	bool testUnescapingHelper(const wchar_t * input, const wchar_t * output,
			bool plusToSpace = false, UriBreakConversion breakConversion = URI_BR_DONT_TOUCH) {
		wchar_t * working = static_cast<URI_CHAR *>(malloc((URI_STRLEN(input) + 1) * sizeof(URI_CHAR)));
		wcscpy(working, input);
		const wchar_t * newTermZero = uriUnescapeInPlaceExW(working,
				plusToSpace ? URI_TRUE : URI_FALSE, breakConversion);
		return ((newTermZero == working + wcslen(output))
				&& !wcscmp(working, output));
	}

	void testUnescaping() {
		const bool PLUS_TO_SPACE = true;
		const bool PLUS_DONT_TOUCH = false;


		// Proper
		TEST_ASSERT(testUnescapingHelper(L"abc%20%41BC", L"abc ABC"));
		TEST_ASSERT(testUnescapingHelper(L"%20", L" "));

		// Incomplete
		TEST_ASSERT(testUnescapingHelper(L"%0", L"%0"));

		// Nonhex
		TEST_ASSERT(testUnescapingHelper(L"%0g", L"%0g"));
		TEST_ASSERT(testUnescapingHelper(L"%G0", L"%G0"));

		// No double decoding
		TEST_ASSERT(testUnescapingHelper(L"%2520", L"%20"));

		// Decoding of '+'
		TEST_ASSERT(testUnescapingHelper(L"abc+def", L"abc+def", PLUS_DONT_TOUCH));
		TEST_ASSERT(testUnescapingHelper(L"abc+def", L"abc def", PLUS_TO_SPACE));

		// Line break conversion
		TEST_ASSERT(testUnescapingHelper(L"%0d", L"\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0d", L"\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0d", L"\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0d", L"\x0d", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));

		TEST_ASSERT(testUnescapingHelper(L"%0d%0d", L"\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0d", L"\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0d", L"\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0d", L"\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));


		TEST_ASSERT(testUnescapingHelper(L"%0a", L"\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0a", L"\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0a", L"\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0a", L"\x0a", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));

		TEST_ASSERT(testUnescapingHelper(L"%0a%0a", L"\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0a", L"\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0a", L"\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0a", L"\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));


		TEST_ASSERT(testUnescapingHelper(L"%0d%0a", L"\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a", L"\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a", L"\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a", L"\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));

		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0a", L"\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0a", L"\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0a", L"\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0a", L"\x0d\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));

		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0d", L"\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0d", L"\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0d", L"\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0d", L"\x0d\x0a\x0d", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));

		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0d%0a", L"\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0d%0a", L"\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0d%0a", L"\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0d%0a%0d%0a", L"\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));


		TEST_ASSERT(testUnescapingHelper(L"%0a%0d", L"\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d", L"\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d", L"\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d", L"\x0a\x0d", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));

		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0a", L"\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0a", L"\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0a", L"\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0a", L"\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));

		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0d", L"\x0a\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0d", L"\x0d\x0a\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0d", L"\x0d\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0d", L"\x0a\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));

		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0a%0d", L"\x0a\x0a\x0a", PLUS_DONT_TOUCH, URI_BR_TO_UNIX));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0a%0d", L"\x0d\x0a\x0d\x0a\x0d\x0a", PLUS_DONT_TOUCH, URI_BR_TO_WINDOWS));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0a%0d", L"\x0d\x0d\x0d", PLUS_DONT_TOUCH, URI_BR_TO_MAC));
		TEST_ASSERT(testUnescapingHelper(L"%0a%0d%0a%0d", L"\x0a\x0d\x0a\x0d", PLUS_DONT_TOUCH, URI_BR_DONT_TOUCH));
	}

	bool testAddBaseHelper(const wchar_t * base, const wchar_t * rel, const wchar_t * expectedResult) {
		UriParserStateW stateW;

		// Base
		UriUriW baseUri;
		stateW.uri = &baseUri;
		int res = uriParseUriW(&stateW, base);
		if (res != 0) {
			return false;
		}

		// Rel
		UriUriW relUri;
		stateW.uri = &relUri;
		res = uriParseUriW(&stateW, rel);
		if (res != 0) {
			uriFreeUriMembersW(&baseUri);
			return false;
		}

		// Expected result
		UriUriW expectedUri;
		stateW.uri = &expectedUri;
		res = uriParseUriW(&stateW, expectedResult);
		if (res != 0) {
			uriFreeUriMembersW(&baseUri);
			uriFreeUriMembersW(&relUri);
			return false;
		}

		// Transform
		UriUriW transformedUri;
		res = uriAddBaseUriW(&transformedUri, &relUri, &baseUri);
		if (res != 0) {
			uriFreeUriMembersW(&baseUri);
			uriFreeUriMembersW(&relUri);
			return false;
		}

		const bool equal = (URI_TRUE == uriEqualsUriW(&transformedUri, &expectedUri));
		if (!equal) {
			wchar_t transformedUriText[1024 * 8];
			wchar_t expectedUriText[1024 * 8];
			uriToStringW(transformedUriText, &transformedUri, 1024 * 8, NULL);
			uriToStringW(expectedUriText, &expectedUri, 1024 * 8, NULL);
			wprintf(L"\n\n\nExpected: \"%s\"\nReceived: \"%s\"\n\n\n", expectedUriText, transformedUriText);
		}

		uriFreeUriMembersW(&baseUri);
		uriFreeUriMembersW(&relUri);
		uriFreeUriMembersW(&expectedUri);
		return equal;
	}

	void testTrailingSlash() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		//                          0  3  01
		const char * const input = "abc" "/";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.pathHead->text.first == input);
		TEST_ASSERT(uriA.pathHead->text.afterLast == input + 3);
		TEST_ASSERT(uriA.pathHead->next->text.first == uriA.pathHead->next->text.afterLast);
		TEST_ASSERT(uriA.pathHead->next->next == NULL);
		TEST_ASSERT(uriA.pathTail == uriA.pathHead->next);
		uriFreeUriMembersA(&uriA);
	}

	void testAddBase() {
		// 5.4.1. Normal Examples
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g:h", L"g:h"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g", L"http://a/b/c/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"./g", L"http://a/b/c/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g/", L"http://a/b/c/g/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"/g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"//g", L"http://g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"?y", L"http://a/b/c/d;p?y"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g?y", L"http://a/b/c/g?y"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"#s", L"http://a/b/c/d;p?q#s"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g#s", L"http://a/b/c/g#s"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g?y#s", L"http://a/b/c/g?y#s"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L";x", L"http://a/b/c/;x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g;x", L"http://a/b/c/g;x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g;x?y#s", L"http://a/b/c/g;x?y#s"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"", L"http://a/b/c/d;p?q"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L".", L"http://a/b/c/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"./", L"http://a/b/c/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"..", L"http://a/b/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../", L"http://a/b/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../g", L"http://a/b/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../..", L"http://a/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../../", L"http://a/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../../g", L"http://a/g"));

		// 5.4.2. Abnormal Examples
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../../../g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../../../../g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"/./g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"/../g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g.", L"http://a/b/c/g."));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L".g", L"http://a/b/c/.g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g..", L"http://a/b/c/g.."));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"..g", L"http://a/b/c/..g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"./../g", L"http://a/b/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"./g/.", L"http://a/b/c/g/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g/./h", L"http://a/b/c/g/h"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g/../h", L"http://a/b/c/h"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g;x=1/./y", L"http://a/b/c/g;x=1/y"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g;x=1/../y", L"http://a/b/c/y"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g?y/./x", L"http://a/b/c/g?y/./x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g?y/../x", L"http://a/b/c/g?y/../x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g#s/./x", L"http://a/b/c/g#s/./x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g#s/../x", L"http://a/b/c/g#s/../x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"http:g", L"http:g"));
	}

	bool testToStringHelper(const wchar_t * text) {
		// Parse
		UriParserStateW state;
		UriUriW uri;
		state.uri = &uri;
		int res = uriParseUriW(&state, text);
		if (res != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		// Back to string, _huge_ limit
		wchar_t shouldbeTheSame[1024 * 8];
		res = uriToStringW(shouldbeTheSame, &uri, 1024 * 8, NULL);
		if (res != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		// Compare
		bool equals = (0 == wcscmp(shouldbeTheSame, text));
		if (!equals) {
			wprintf(L"\n\n\nExpected: \"%s\"\nReceived: \"%s\"\n\n\n", text, shouldbeTheSame);
		}

		// Back to string, _exact_ limit
		const int len = static_cast<int>(wcslen(text));
		int charsWritten;
		res = uriToStringW(shouldbeTheSame, &uri, len + 1, &charsWritten);
		if ((res != 0) || (charsWritten != len + 1)) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		// Back to string, _too small_ limit
		res = uriToStringW(shouldbeTheSame, &uri, len, &charsWritten);
		if ((res == 0) || (charsWritten >= len + 1)) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		uriFreeUriMembersW(&uri);
		return equals;
	}

	void testToString() {
		// Scheme
		TEST_ASSERT(testToStringHelper(L"ftp://localhost/"));
		// UserInfo
		TEST_ASSERT(testToStringHelper(L"http://user:pass@localhost/"));
		// IPv4
		TEST_ASSERT(testToStringHelper(L"http://123.0.1.255/"));
		// IPv6
		TEST_ASSERT(testToStringHelper(L"http://[abcd:abcd:abcd:abcd:abcd:abcd:abcd:abcd]/"));
		// IPvFuture
		TEST_ASSERT(testToStringHelper(L"http://[vA.123456]/"));
		// Port
		TEST_ASSERT(testToStringHelper(L"http://example.com:123/"));
		// Path
		TEST_ASSERT(testToStringHelper(L"http://example.com/"));
		TEST_ASSERT(testToStringHelper(L"http://example.com/abc/"));
		TEST_ASSERT(testToStringHelper(L"http://example.com/abc/def"));
		TEST_ASSERT(testToStringHelper(L"http://example.com/abc/def"));
		TEST_ASSERT(testToStringHelper(L"http://example.com//"));
		TEST_ASSERT(testToStringHelper(L"http://example.com/./.."));
		// Query
		TEST_ASSERT(testToStringHelper(L"http://example.com/?abc"));
		// Fragment
		TEST_ASSERT(testToStringHelper(L"http://example.com/#abc"));
		TEST_ASSERT(testToStringHelper(L"http://example.com/?def#abc"));

		// Relative
		TEST_ASSERT(testToStringHelper(L"a"));
		TEST_ASSERT(testToStringHelper(L"a/"));
		TEST_ASSERT(testToStringHelper(L"/a"));
		TEST_ASSERT(testToStringHelper(L"/a/"));
		TEST_ASSERT(testToStringHelper(L"abc"));
		TEST_ASSERT(testToStringHelper(L"abc/"));
		TEST_ASSERT(testToStringHelper(L"/abc"));
		TEST_ASSERT(testToStringHelper(L"/abc/"));
		TEST_ASSERT(testToStringHelper(L"a/def"));
		TEST_ASSERT(testToStringHelper(L"a/def/"));
		TEST_ASSERT(testToStringHelper(L"/a/def"));
		TEST_ASSERT(testToStringHelper(L"/a/def/"));
		TEST_ASSERT(testToStringHelper(L"abc/def"));
		TEST_ASSERT(testToStringHelper(L"abc/def/"));
		TEST_ASSERT(testToStringHelper(L"/abc/def"));
		TEST_ASSERT(testToStringHelper(L"/abc/def/"));
		TEST_ASSERT(testToStringHelper(L"/"));
		TEST_ASSERT(testToStringHelper(L"//a/"));
		TEST_ASSERT(testToStringHelper(L"."));
		TEST_ASSERT(testToStringHelper(L"./"));
		TEST_ASSERT(testToStringHelper(L"/."));
		TEST_ASSERT(testToStringHelper(L"/./"));
		TEST_ASSERT(testToStringHelper(L""));
		TEST_ASSERT(testToStringHelper(L"./abc/def"));
		TEST_ASSERT(testToStringHelper(L"?query"));
		TEST_ASSERT(testToStringHelper(L"#fragment"));
		TEST_ASSERT(testToStringHelper(L"?query#fragment"));

		// Tests for bugs from the past
		TEST_ASSERT(testToStringHelper(L"f:/.//g"));
	}

	bool testToStringCharsRequiredHelper(wchar_t * text) {
		// Parse
		UriParserStateW state;
		UriUriW uri;
		state.uri = &uri;
		int res = uriParseUriW(&state, text);
		if (res != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		// Required space?
		int charsRequired;
		if (uriToStringCharsRequiredW(&uri, &charsRequired) != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		// Minimum
		wchar_t * buffer = new wchar_t[charsRequired + 1];
		if (uriToStringW(buffer, &uri, charsRequired + 1, NULL) != 0) {
			uriFreeUriMembersW(&uri);
			delete [] buffer;
			return false;
		}

		// One less than minimum
		if (uriToStringW(buffer, &uri, charsRequired, NULL) == 0) {
			uriFreeUriMembersW(&uri);
			delete [] buffer;
			return false;
		}

		uriFreeUriMembersW(&uri);
		delete [] buffer;
		return true;
	}

	void testToStringCharsRequired() {
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com:80/"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://user:pass@www.example.com/"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/index.html"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/?abc"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/#def"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/?abc#def"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"/test"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"test"));
	}

	bool testNormalizeMaskHelper(wchar_t * uriText, unsigned int expectedMask) {
		UriParserStateW state;
		UriUriW uri;
		state.uri = &uri;
		int res = uriParseUriW(&state, uriText);
		if (res != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		const unsigned int maskBefore = uriNormalizeSyntaxMaskRequiredW(&uri);
		if (maskBefore != expectedMask) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		res = uriNormalizeSyntaxW(&uri);
		if (res != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		const unsigned int maskAfter = uriNormalizeSyntaxMaskRequiredW(&uri);
		uriFreeUriMembersW(&uri);

		// Second call should be no problem
		uriFreeUriMembersW(&uri);

		return (maskAfter == URI_NORMALIZED);
	}

	void testNormalizeSyntaxMaskRequired() {
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhost/", URI_NORMALIZED));
		TEST_ASSERT(testNormalizeMaskHelper(L"httP://localhost/", URI_NORMALIZE_SCHEME));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://%0d@localhost/", URI_NORMALIZE_USER_INFO));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhosT/", URI_NORMALIZE_HOST));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhost/./abc", URI_NORMALIZE_PATH));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhost/?AB%43", URI_NORMALIZE_QUERY));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhost/#AB%43", URI_NORMALIZE_FRAGMENT));
	}

	bool testNormalizeSyntaxHelper(wchar_t * uriText, wchar_t * expectedNormalized,
			unsigned int mask = static_cast<unsigned int>(-1)) {
		UriParserStateW stateW;
		int res;

		UriUriW testUri;
		stateW.uri = &testUri;
		res = uriParseUriW(&stateW, uriText);
		if (res != 0) {
			uriFreeUriMembersW(&testUri);
			return false;
		}

		// Expected result
		UriUriW expectedUri;
		stateW.uri = &expectedUri;
		res = uriParseUriW(&stateW, expectedNormalized);
		if (res != 0) {
			uriFreeUriMembersW(&testUri);
			uriFreeUriMembersW(&expectedUri);
			return false;
		}

		// First run
		res = uriNormalizeSyntaxExW(&testUri, mask);
		if (res != 0) {
			uriFreeUriMembersW(&testUri);
			uriFreeUriMembersW(&expectedUri);
			return false;
		}

		bool equalAfter = (URI_TRUE == uriEqualsUriW(&testUri, &expectedUri));

		// Second run
		res = uriNormalizeSyntaxExW(&testUri, mask);
		if (res != 0) {
			uriFreeUriMembersW(&testUri);
			uriFreeUriMembersW(&expectedUri);
			return false;
		}

		equalAfter = equalAfter
				&& (URI_TRUE == uriEqualsUriW(&testUri, &expectedUri));

		uriFreeUriMembersW(&testUri);
		uriFreeUriMembersW(&expectedUri);
		return equalAfter;
	}

	void testNormalizeSyntax() {
		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"eXAMPLE://a/./b/../b/%63/%7bfoo%7d",
				L"example://a/b/c/%7Bfoo%7D"));

		// Testcase by Adrian Manrique
		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"http://examp%4Ce.com/",
				L"http://example.com/"));

		// Testcase by Adrian Manrique
		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"http://example.com/a/b/%2E%2E/",
				L"http://example.com/a/"));

		// Reported by Adrian Manrique
		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"http://user:pass@SOMEHOST.COM:123",
				L"http://user:pass@somehost.com:123"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://a:b@HOST:123/./1/2/../%41?abc#def",
				L"http://a:b@host:123/1/A?abc#def"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"../../abc",
				L"../../abc"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"../../abc/..",
				L"../../"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"../../abc/../def",
				L"../../def"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"abc/..",
				L""));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"abc/../",
				L""));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"../../abc/./def",
				L"../../abc/def"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"./def",
				L"def"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"def/.",
				L"def/"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"./abc:def",
				L"./abc:def"));
	}

	void testNormalizeSyntaxComponents() {
		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"http://%41@EXAMPLE.ORG/../a?%41#%41",
				URI_NORMALIZE_SCHEME));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://A@EXAMPLE.ORG/../a?%41#%41",
				URI_NORMALIZE_USER_INFO));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://%41@example.org/../a?%41#%41",
				URI_NORMALIZE_HOST));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://%41@EXAMPLE.ORG/a?%41#%41",
				URI_NORMALIZE_PATH));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://%41@EXAMPLE.ORG/../a?A#%41",
				URI_NORMALIZE_QUERY));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#A",
				URI_NORMALIZE_FRAGMENT));
	}

	void testNormalizeCrash_Bug20080224() {
		UriParserStateW stateW;
		int res;
		UriUriW testUri;
		stateW.uri = &testUri;

		res = uriParseUriW(&stateW, L"http://example.org/abc//../def");
		TEST_ASSERT(res == 0);

		// First call will make us owner of copied memory
		res = uriNormalizeSyntaxExW(&testUri, URI_NORMALIZE_SCHEME);
		TEST_ASSERT(res == 0);
		res = uriNormalizeSyntaxExW(&testUri, URI_NORMALIZE_HOST);
		TEST_ASSERT(res == 0);

		// Frees empty path segment -> crash
		res = uriNormalizeSyntaxW(&testUri);
		TEST_ASSERT(res == 0);

		uriFreeUriMembersW(&testUri);
	}

	void testFilenameUriConversionHelper(const wchar_t * filename,
			const wchar_t * uriString, bool forUnix) {
		const int prefixLen = forUnix ? 7 : 8;

		// Filename to URI string
		const size_t uriBufferLen = prefixLen + 3 * wcslen(filename) + 1;
		wchar_t * uriBuffer = new wchar_t[uriBufferLen];
		if (forUnix) {
			uriUnixFilenameToUriStringW(filename, uriBuffer);
		} else {
			uriWindowsFilenameToUriStringW(filename, uriBuffer);
		}
		// wprintf(L"1 [%s][%s]\n", uriBuffer, uriString);
		TEST_ASSERT(!wcscmp(uriBuffer, uriString));
		delete [] uriBuffer;

		// URI string to filename
		const size_t filenameBufferLen = wcslen(uriString) + 1;
		wchar_t * filenameBuffer = new wchar_t[filenameBufferLen];
		if (forUnix) {
			uriUriStringToUnixFilenameW(uriString, filenameBuffer);
		} else {
			uriUriStringToWindowsFilenameW(uriString, filenameBuffer);
		}
		// wprintf(L"2 [%s][%s]\n", filenameBuffer, filename);
		TEST_ASSERT(!wcscmp(filenameBuffer, filename));
		delete [] filenameBuffer;
	}

	void testFilenameUriConversion() {
		const bool FOR_UNIX = true;
		const bool FOR_WINDOWS = false;
		testFilenameUriConversionHelper(L"/bin/bash", L"file:///bin/bash", FOR_UNIX);
		testFilenameUriConversionHelper(L"./configure", L"./configure", FOR_UNIX);

		testFilenameUriConversionHelper(L"E:\\Documents and Settings", L"file:///E:/Documents%20and%20Settings", FOR_WINDOWS);
		testFilenameUriConversionHelper(L".\\Readme.txt", L"./Readme.txt", FOR_WINDOWS);

		testFilenameUriConversionHelper(L"index.htm", L"index.htm", FOR_WINDOWS);
		testFilenameUriConversionHelper(L"index.htm", L"index.htm", FOR_UNIX);

		testFilenameUriConversionHelper(L"abc def", L"abc%20def", FOR_WINDOWS);
		testFilenameUriConversionHelper(L"abc def", L"abc%20def", FOR_UNIX);
	}

	void testCrash_FreeUriMembers_Bug20080116() {
		// Testcase by Adrian Manrique
		UriParserStateA state;
		UriUriA uri;
		state.uri = &uri;
		uriParseUriA(&state, "http://test/?");
		uriNormalizeSyntaxA(&uri);
		uriFreeUriMembersA(&uri);

		TEST_ASSERT(true);
	}

	void testCrash_MakeOwner_Bug20080207() {
		// Testcase by Adrian Manrique
		UriParserStateA state;
		UriUriA sourceUri;
		state.uri = &sourceUri;
		const char * const sourceUriString = "http://user:pass@somehost.com:80/";
		if (uriParseUriA(&state, sourceUriString) != 0) {
			TEST_ASSERT(false);
		}
		if (uriNormalizeSyntaxA(&sourceUri) != 0) {
			TEST_ASSERT(false);
		}
		uriFreeUriMembersA(&sourceUri);
		TEST_ASSERT(true);
	}

	void testQueryListHelper(const wchar_t * input, int expectedItemCount) {
		int res;
		
		UriBool spacePlusConversion = URI_TRUE;
		UriBool normalizeBreaks = URI_FALSE;
		UriBreakConversion breakConversion = URI_BR_DONT_TOUCH;

		int itemCount;
		UriQueryListW * queryList;
		res = uriDissectQueryMallocExW(&queryList, &itemCount,
				input, input + wcslen(input), spacePlusConversion, breakConversion);
		TEST_ASSERT(res == URI_SUCCESS);
		TEST_ASSERT(itemCount == expectedItemCount);
		TEST_ASSERT((queryList == NULL) == (expectedItemCount == 0));

		if (expectedItemCount != 0) {
			// First 
			int charsRequired;
			res = uriComposeQueryCharsRequiredExW(queryList, &charsRequired, spacePlusConversion, 
					normalizeBreaks);
			TEST_ASSERT(res == URI_SUCCESS);
			TEST_ASSERT(charsRequired >= wcslen(input));

			wchar_t * recomposed = new wchar_t[charsRequired + 1];
			int charsWritten;
			res = uriComposeQueryExW(recomposed, queryList, charsRequired + 1,
					&charsWritten, spacePlusConversion, normalizeBreaks);
			TEST_ASSERT(res == URI_SUCCESS);
			TEST_ASSERT(charsWritten <= charsRequired);
			TEST_ASSERT(charsWritten = wcslen(input) + 1);
			TEST_ASSERT(!wcscmp(input, recomposed));
			delete [] recomposed;

			recomposed = NULL;
			res = uriComposeQueryMallocW(&recomposed, queryList);
			TEST_ASSERT(res == URI_SUCCESS);
			TEST_ASSERT(recomposed != NULL);
			TEST_ASSERT(charsWritten = wcslen(input) + 1);
			TEST_ASSERT(!wcscmp(input, recomposed));
			free(recomposed);
		}

		uriFreeQueryListW(queryList);
	}

	void testQueryList() {
		testQueryListHelper(L"one=ONE&two=TWO", 2);
		testQueryListHelper(L"one=ONE&two=&three=THREE", 3);
		testQueryListHelper(L"one=ONE&two&three=THREE", 3);
		testQueryListHelper(L"one=ONE", 1);
		testQueryListHelper(L"one", 1);
		testQueryListHelper(L"", 0);
	}

	void testQueryListPairHelper(const char * pair, const char * unescapedKey,
			const char * unescapedValue, const char * fixed = NULL) {
		int res;
		UriQueryListA * queryList;
		int itemCount;
		
		res = uriDissectQueryMallocA(&queryList, &itemCount, pair, pair + strlen(pair));
		TEST_ASSERT(res == URI_SUCCESS);
		TEST_ASSERT(queryList != NULL);
		TEST_ASSERT(itemCount = 1);
		TEST_ASSERT(!strcmp(queryList->key, unescapedKey));
		TEST_ASSERT(!strcmp(queryList->value, unescapedValue));

		char * recomposed;
		res = uriComposeQueryMallocA(&recomposed, queryList);
		TEST_ASSERT(res == URI_SUCCESS);
		TEST_ASSERT(recomposed != NULL);
		TEST_ASSERT(!strcmp(recomposed, (fixed != NULL) ? fixed : pair));
		free(recomposed);
	}

	void testQueryListPair() {
		testQueryListPairHelper("one+two+%26+three=%2B", "one two & three", "+");
		testQueryListPairHelper("one=two=three", "one", "two=three", "one=two%3Dthree");
		testQueryListPairHelper("one=two=three=four", "one", "two=three=four", "one=two%3Dthree%3Dfour");
	}

};



int main() {
	Suite suite;
	suite.add(auto_ptr<Suite>(new UriSuite()));
	suite.add(auto_ptr<Suite>(new FourSuite()));
	TextOutput output(TextOutput::Verbose);
	return suite.run(output, false) ? 0 : 1;
}
