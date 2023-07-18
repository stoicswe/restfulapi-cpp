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
	char* targetApi = getCmdOption(argv, argv + argc, "-u", false);
	char* baseService = getCmdOption(argv, argv + argc, "-s", false);

	std::string baseServiceStr = "http://127.0.0.1:8080";
	if (targetApi) { PLOG(plog::info) << "TargetAPI: " + std::string(targetApi); }
	if (baseService) { PLOG(plog::info) << "BaseService: " + std::string(baseService); baseServiceStr = std::string(baseService); }
	if (help) 
	{ 
		std::cout 
			<< "usage: restfulapi_test [options] -u <target_api>" << std::endl
			<< "options:" << std::endl
			<< "-u <api>, required      Pass the api that this program should target" << std::endl
			<< "-s <server>, optional   The target server that the api is hosted on" << std::endl
			;
		return 0;
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
	PLOG(plog::debug) << "executing a head request";
	RestfulAPI::HttpRequest request = RestfulAPI::HttpRequest("/");
	request.SetHeader(RestfulAPI::HTTPHEADER_ACCEPTS, "*/*");
	RestfulAPI::ApiResponse response = client.Head(std::string(targetApi));
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