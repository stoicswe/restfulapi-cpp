//#include <plog/Log.h>
#include <curl/curl.h>
#include <sys/stat.h>

#include <restfulapi.h>

using namespace RestfulAPI;

void RestApiClient::Initialize()
{
	//PLOG(plog::debug) << "in initialize";
	// Clear out any old values that dont matter anymore
	m_response_headers = std::make_shared<std::string>("");
	m_response_body = std::make_shared<std::string>("");
	m_header_struct = NULL;
	// Initialize the curl instance
	curl = curl_easy_init();
	// Populate any values/options that should be set for the RestClient
	SetOpts();
	//PLOG(plog::debug) << "exiting initialize";
}

void RestApiClient::Clean()
{
	//PLOG(plog::debug) << "In clean";
	if (curl) {
		curl_easy_cleanup(curl);
	}
	if (m_header_struct) {
		curl_slist_free_all(m_header_struct);
	}
	if (m_response_headers) {
		m_response_headers.reset();
	}
	if (m_response_body) {
		m_response_body.reset();
	}
	//PLOG(plog::debug) << "exiting clean";
}

const ApiResponse RestApiClient::Head(HttpRequest request)
{
	//PLOG(plog::debug) << "in head()";
	//PLOG(plog::debug) << "passed: " + request.GetUrl();
	// Initialize the CURL and other required things
	Initialize();
	// Populate any headers that have been set
	SetHeaders(request);
	// Setup the CURL ops to issue a HEAD request
	//PLOG(plog::debug) << "preparing head request";
	curl_easy_setopt(curl, CURLOPT_URL, std::string(m_apiEndpoint + request.GetUrl()).c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, m_header_struct);
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1l);
	int httpStatusCode;
	CURLcode curlStatusCode;
	// Run the CURL command
	if (curl)
	{
		//PLOG(plog::info) << "making HEAD request against " + std::string(m_apiEndpoint + request.GetUrl());
		curlStatusCode = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatusCode);
	}
	if (curlStatusCode != CURLE_OK)
	{
		//PLOG(plog::error) << "error: " + std::string(curl_easy_strerror(curlStatusCode));
	}
	// Build the response to be returned
	//PLOG(plog::debug) << "retrieving the response";
	HttpStatus httpStatus = HttpStatus(httpStatusCode, std::string(curl_easy_strerror(curlStatusCode)));
	ApiResponse response = ApiResponse(httpStatus, m_response_headers, m_response_body);
	// Clean the CURL instance
	Clean();
	// Conclude
	//PLOG(plog::debug) << "exiting head()";
	return response;
}

const ApiResponse RestApiClient::Get(HttpRequest request)
{
	//PLOG(plog::debug) << "in get()";
	//PLOG(plog::debug) << "passed: " + request.GetUrl();
	// Initialize the CURL and other required things
	Initialize();
	// Populate any headers that have been set
	SetHeaders(request);
	// Setup the CURL ops to issue a HEAD request
	//PLOG(plog::debug) << "preparing get request";
	curl_easy_setopt(curl, CURLOPT_URL, std::string(m_apiEndpoint + request.GetUrl()).c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, m_header_struct);
	int httpStatusCode;
	CURLcode curlStatusCode;
	// Run the CURL command
	if (curl)
	{
		//PLOG(plog::info) << "making GET request against " + std::string(m_apiEndpoint + request.GetUrl());
		curlStatusCode = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatusCode);
	}
	if (curlStatusCode != CURLE_OK)
	{
		//PLOG(plog::error) << "error: " + std::string(curl_easy_strerror(curlStatusCode));
	}
	// Build the response to be returned
	//PLOG(plog::debug) << "retrieving the response";
	HttpStatus httpStatus = HttpStatus(httpStatusCode, std::string(curl_easy_strerror(curlStatusCode)));
	ApiResponse response = ApiResponse(httpStatus, m_response_headers, m_response_body);
	// Clean the CURL instance
	Clean();
	// Conclude
	//PLOG(plog::debug) << "exiting get()";
	return response;
}

const ApiResponse RestApiClient::Put(HttpRequest request, std::shared_ptr<FILE> data, struct stat dataInfo)
{
	//PLOG(plog::debug) << "in head()";
	//PLOG(plog::debug) << "passed: " + request.GetUrl();
	// Initialize the CURL and other required things
	Initialize();
	// Populate any headers that have been set
	SetHeaders(request);
	// Setup the CURL ops to issue a HEAD request
	//PLOG(plog::debug) << "preparing head request";
	curl_easy_setopt(curl, CURLOPT_URL, std::string(m_apiEndpoint + request.GetUrl()).c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, m_header_struct);
	curl_easy_setopt(curl, CURLOPT_READDATA, data.get());
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t) dataInfo.st_size);
	int httpStatusCode;
	CURLcode curlStatusCode;
	// Run the CURL command
	if (curl)
	{
		//PLOG(plog::info) << "making HEAD request against " + std::string(m_apiEndpoint + request.GetUrl());
		curlStatusCode = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatusCode);
	}
	if (curlStatusCode != CURLE_OK)
	{
		//PLOG(plog::error) << "error: " + std::string(curl_easy_strerror(curlStatusCode));
	}
	// Build the response to be returned
	//PLOG(plog::debug) << "retrieving the response";
	HttpStatus httpStatus = HttpStatus(httpStatusCode, std::string(curl_easy_strerror(curlStatusCode)));
	ApiResponse response = ApiResponse(httpStatus, m_response_headers, m_response_body);
	// Clean the CURL instance
	Clean();
	// Conclude
	//PLOG(plog::debug) << "exiting head()";
	return response;
}
