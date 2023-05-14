#include <iostream>
#include <map>

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include <curl/curl.h>

#include <restfulapi.h>

// Functions for processing the arguments passed to the program as values passed to specific flags
char* getCmdOption(char** begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}


int main(int argc, char** argv){

	std::cout << "RestfulApi Test" << std::endl;

	plog::init(plog::verbose, "console.log", 10000, 25);
	plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
	plog::get()->addAppender(&consoleAppender);

	char* help = getCmdOption(argv, argv + argc, "-h");
	char* operation = getCmdOption(argv, argv + argc, "-o");
	char* targetApi = getCmdOption(argv, argv + argc, "-u");
	char* baseService = getCmdOption(argv, argv + argc, "-s");
	char* fileToUpoad = getCmdOption(argv, argv + argc, "-f");

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

	// init the rest client here and process the url request
	PLOG(plog::debug) << "building the rest client";
	RestfulAPI::RestApiClient client = RestfulAPI::RestApiClient(baseServiceStr);
	client.SetOption(RestfulAPI::RESTCLIENTOPT_USER_AGENT, "testRestful/1.0");
	client.SetOption(RestfulAPI::RESTCLIENTOPT_READ_BODY, "true");
	client.SetOption(RestfulAPI::RESTCLIENTOPT_FOLLOW_REDIRECT, "true");
	//client.SetOption(RestfulAPI::RESTCLIENTOPT_SSL_DISABLE_VERIFY_PEER, "true");
	//client.SetOption(RestfulAPI::RESTCLIENTOPT_SSL_DISABLE_VERIFY_HOST, "true");
	PLOG(plog::debug) << "executing a head request";
	RestfulAPI::HttpRequest request = RestfulAPI::HttpRequest("/");
	request.SetHeader(RestfulAPI::HTTPHEADER_ACCEPTS, "*/*");

	RestfulAPI::ApiResponse response;
	switch (RestfulAPI::GetOperationEnum(std::string(operation)))
	{
	case RestfulAPI::HTTPOPERATION_HEAD:
		response = client.Head(std::string(targetApi));
		break;
	case RestfulAPI::HTTPOPERATION_GET:
		response = client.Get(std::string(targetApi));
		break;
	case RestfulAPI::HTTPOPERATION_PUT:
		struct stat dataInfo;
		stat(fileToUpoad, &dataInfo);
		sourceFileToUpload = std::make_shared<FILE>(fopen(fileToUpoad, "rb"));
		response = client.Put(std::string(targetApi), sourceFileToUpload, dataInfo);
		break;
	case RestfulAPI::HTTPOPERATION_ERROR:
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
		PLOG(plog::info) << "the total response headers size from the rest client is: [" + std::to_string(sizeof(*responseHeaders)) + "]";
		PLOG(plog::info) << "the response headers from the rest client is: [" + *responseHeaders + "]";
	}
	std::shared_ptr<std::string> responseBody = response.GetResponseBody();
	if (responseBody) 
	{
		PLOG(plog::info) << "the total response body size from the rest client is: [" + std::to_string(sizeof(*responseBody)) + "]";
		PLOG(plog::info) << "the response body from the rest client is: [" + *responseBody + "]";
	}

	return 0;
}