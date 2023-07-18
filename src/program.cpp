#include <iostream>
#include <map>

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include <restfulapi.h>

// Functions for processing the arguments passed to the program as values passed to specific flags
char* getCmdOption(char** begin, char** end, const std::string& option, bool isFlag)
{
	char** itr = std::find(begin, end, option);
	if (isFlag || (itr != end && ++itr != end))
	{
		return *itr;
	}
	return nullptr;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}


int main(int argc, char** argv){

	std::cout << "RestfulApi Test Application" << std::endl;

	plog::init(plog::verbose, "console.log", 10000, 25);
	plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::get()->addAppender(&consoleAppender);

	char* help = getCmdOption(argv, argv + argc, "-h", true);
	char* operation = getCmdOption(argv, argv + argc, "-o", false);
	char* targetApi = getCmdOption(argv, argv + argc, "-u", false);
	char* baseService = getCmdOption(argv, argv + argc, "-s", false);
	char* fileToUpoad = getCmdOption(argv, argv + argc, "-f", false);

	std::shared_ptr<FILE> sourceFileToUpload;
	std::string baseServiceStr = "http://127.0.0.1:8080";
	if (targetApi) { PLOG(plog::info) << "TargetAPI: " + std::string(targetApi); }
	if (baseService) { PLOG(plog::info) << "BaseService: " + std::string(baseService); baseServiceStr = std::string(baseService); }
	if (help) 
	{ 
		std::cout
			<< "usage: restfulapi_test [options] -u <target_api>" << std::endl
			<< "options:" << std::endl
			<< "-o <operation>, required  Pass the operation to perform: [head, get, put]" << std::endl
			<< "-u <api>, required        Pass the api that this program should target" << std::endl
			<< "-f <filename>, optional   Pass the name of the file whos contents should be uploaded if using PUT" << std::endl
			<< "-s <server>, optional     The target server that the api is hosted on" << std::endl
			;
		return 0;
	}

	if (!operation)
	{
		PLOG(plog::fatal) << "no operation entered. operation is required. See (-h) for details.";
		return 1;
	}

	if (!targetApi)
	{ 
		PLOG(plog::fatal) << "no api target entered. api target is required. See (-h) for details."; 
		return 1;
	}

    // Get the type of operation to be executed against the target endpoint
    auto operationType = RestfulAPI::GetOperationEnum(std::string(operation));

	// init the rest client here and process the url request
	PLOG(plog::debug) << "building the rest client";
	RestfulAPI::RestApiClient client = RestfulAPI::RestApiClient(baseServiceStr);
	client.SetOption(RestfulAPI::RESTCLIENTOPT_USER_AGENT, "testRestful/1.0");
    client.SetOption(RestfulAPI::RESTCLIENTOPT_READ_BODY, "true");
	client.SetOption(RestfulAPI::RESTCLIENTOPT_FOLLOW_REDIRECT, "true");
	//client.SetOption(RestfulAPI::RESTCLIENTOPT_SSL_DISABLE_VERIFY_PEER, "true");
	//client.SetOption(RestfulAPI::RESTCLIENTOPT_SSL_DISABLE_VERIFY_HOST, "true");
	RestfulAPI::HttpRequest request = RestfulAPI::HttpRequest("/");
	request.SetHeader(RestfulAPI::HTTPHEADER_ACCEPTS, "*/*");

	RestfulAPI::ApiResponse response;
	switch (operationType)
	{
	case RestfulAPI::HTTPOPERATION_HEAD:
        PLOG(plog::debug) << "sending a HEAD request using client";
		response = client.Head(std::string(targetApi));
		break;
	case RestfulAPI::HTTPOPERATION_GET:
        PLOG(plog::debug) << "sending a GET request using client";
		response = client.Get(std::string(targetApi));
		break;
	case RestfulAPI::HTTPOPERATION_PUT:
        PLOG(plog::debug) << "sending a PUT request using client";
        // This will give a warning of uninitialized, ignore, since it is in a CASE
		struct stat dataInfo;

		stat(fileToUpoad, &dataInfo);
#ifdef _WIN32
        FILE* dataFile;
        fopen_s(&dataFile, fileToUpoad, "rb");
		sourceFileToUpload = std::make_shared<FILE>(dataFile);
#elif linux
        sourceFileToUpload = std::make_shared<FILE>(fopen(fileToUpoad, "rb"));
#endif
		response = client.Put(std::string(targetApi), sourceFileToUpload, dataInfo);
		break;
	default:
		PLOG(plog::fatal) << "no operation is implemented. See (-h) for implemented operation details.";
		return 1;
	}

	if (sourceFileToUpload)
	{
		fclose(sourceFileToUpload.get());
	}

	RestfulAPI::HttpStatus httpStatus = response.GetHttpStatus();
	PLOG(plog::info) << "the http response status is: " + std::to_string(httpStatus.GetHttpStatusCode());
	std::shared_ptr<std::string> responseHeaders = response.GetResponseHeaders();
	if (responseHeaders)
	{
        // Want to print the total bytes, so using sizeof()
		PLOG(plog::info) << "the total response headers size (in bytes) from the rest client is: [" + std::to_string(sizeof(*responseHeaders)) + "]";
		PLOG(plog::info) << "the response headers from the rest client is: [" + *responseHeaders + "]";
	}
	std::shared_ptr<std::string> responseBody = response.GetResponseBody();
	if (responseBody) 
	{
        // Want to print the total bytes, so using sizeof()
		PLOG(plog::info) << "the total response body size (in bytes) from the rest client is: [" + std::to_string(sizeof(*responseBody)) + "]";
		PLOG(plog::info) << "the response body from the rest client is: [" + *responseBody + "]";
	}

	return 0;
}