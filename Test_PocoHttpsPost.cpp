// Test_PocoHttpsPost.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "dep/Poco/URIStreamOpener.h"
#include "dep/Poco/StreamCopier.h"
#include "dep/Poco/URI.h"
#include "dep/Poco/SharedPtr.h"
#include "dep/Poco/Exception.h"

#include "dep/Poco/Net/HTTPSClientSession.h"
#include "dep/Poco/Net/HTTPClientSession.h"
#include "dep/Poco/Net/HTTPRequest.h"
#include "dep/Poco/Net/HTTPResponse.h"
#include "dep/Poco/Net/SSLException.h"
#include "dep/Poco/Net/HTTPBasicCredentials.h"
#include "dep/Poco/Net/HTMLForm.h"
#pragma comment (lib, "crypt32")
int main()
{
    Poco::Net::initializeSSL();

    try
    {
        const Poco::URI uri("https://httpbin.org");
        const Poco::Net::Context::Ptr context(
            new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "",
                Poco::Net::Context::VERIFY_NONE));
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
        Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, "/post");

        Poco::Net::HTMLForm form;
        form.set("username", "ligang");
        form.set("fingerprint", "abcd");
        form.prepareSubmit(req);

        std::ostream &ostr = session.sendRequest(req) /*<< reqBody*/;
        form.write(ostr);

        Poco::Net::HTTPResponse res;
        std::istream& rs = session.receiveResponse(res);
        std::string resp;

        std::string content;
        Poco::StreamCopier::copyToString(rs, content);
        std::cout << content << std::endl;
    }
    catch (const Poco::Net::SSLException& e)
    {
        std::cerr << e.what() << ": " << e.message() << std::endl;
    }
    catch (const std::exception&)
    {

    }

    Poco::Net::uninitializeSSL();
    return 0;
}