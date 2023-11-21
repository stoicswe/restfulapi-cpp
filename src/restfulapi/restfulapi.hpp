#include <iostream>
#include <memory>
#include <map>

#include <curl/curl.h>

namespace RestfulAPI
{
	const float _VERSION = 0.1;
    const std::string _DEFAULT_TARGET_ENDPOINT = "http://127.0.0.1:8080";

	class HttpStatus
	{
	public:
		HttpStatus()
		{}
		HttpStatus(int statusCode, std::string statusMessage)
			: m_status_code(statusCode), m_status_message(statusMessage)
		{}
		const int GetHttpStatusCode() { return m_status_code; }
		const std::string GetStatusMesssage() { return m_status_message; }
	protected:
		int m_status_code;
		std::string m_status_message;
	};

	class ApiResponse
	{
	public:
		ApiResponse()
		{}
		ApiResponse(HttpStatus status)
			: m_request_status(status)
		{}
		ApiResponse(HttpStatus& status, std::shared_ptr<std::string> header, std::shared_ptr<std::string> body)
			: m_request_status(status), m_header(header), m_body(body)
		{}
		~ApiResponse()
		{}
		const HttpStatus GetHttpStatus() { return m_request_status; }
		const std::shared_ptr<std::string> GetResponseHeaders() { return m_header; }
		const std::shared_ptr<std::string> GetResponseBody() { return m_body; }
	protected:
		HttpStatus m_request_status;
		std::shared_ptr<std::string> m_header;
		std::shared_ptr<std::string> m_body;
	};

	enum HttpOperations {
		HTTPOPERATION_ERROR = 0,
		HTTPOPERATION_HEAD = 1,
		HTTPOPERATION_GET = 2,
		HTTPOPERATION_PUT = 3
	};

	static const std::map<std::string, HttpOperations> _HTTP_OPERATION_LABELS({
		{"head",HTTPOPERATION_HEAD},
		{"get", HTTPOPERATION_GET},
		{"put",HTTPOPERATION_PUT}
		});

	static const HttpOperations GetOperationEnum(std::string httpOperationLabel)
	{
		auto it = _HTTP_OPERATION_LABELS.find(httpOperationLabel);
		if (it != _HTTP_OPERATION_LABELS.end()) {
			return it->second;
		}
		return HTTPOPERATION_ERROR;
	}

	enum HttpHeader
	{
		HTTPHEADER_ACCEPTS = 1
	};

	static const std::map<int, std::string> _HTTP_HEADER_VALUES({ {HTTPHEADER_ACCEPTS, "Accepts:"} });

	static const std::string GetHeaderLabel(HttpHeader headerOption)
	{
		auto it = _HTTP_HEADER_VALUES.find(headerOption);
		if (it != _HTTP_HEADER_VALUES.end()) {
			return it->second;
		}
		return NULL;
	}

	class HttpRequest
	{
	public:
		HttpRequest(const char* url)
			: m_url(url)
		{}
		HttpRequest(std::string url)
			: m_url(url)
		{}
		HttpRequest(const char* url, std::map<std::string, std::string>& headers)
			: m_url(url), m_headers(headers)
		{}
		HttpRequest(std::string& url, std::map<std::string, std::string>& headers)
			: m_url(url), m_headers(headers)
		{}
		~HttpRequest()
		{}
		const std::string GetUrl() { return m_url; }
		const std::map<std::string, std::string> GetHeaders() { return m_headers; }
		void SetHeader(HttpHeader headerOption, std::string value) { m_headers[GetHeaderLabel(headerOption)] = value; }
		void AddHeaders(std::map<std::string, std::string>& headers) { m_headers.insert(headers.begin(), headers.end()); }
	protected:
		std::string m_url;
		std::map<std::string, std::string> m_headers;
	};

	enum RestApiOption
	{
		RESTCLIENTOPT_USER_AGENT = 1,
		RESTCLIENTOPT_READ_HEADERS = 2,
		RESTCLIENTOPT_READ_BODY = 3,
		RESTCLIENTOPT_FOLLOW_REDIRECT = 4,
		RESTCLIENTOPT_SSL_DISABLE_VERIFY_PEER = 5,
		RESTCLIENTOPT_SSL_DISABLE_VERIFY_HOST = 6
	};
	
	class RestApiClient
	{
	public:
        static RestApiClient Instance(const std::string& endpoint = _DEFAULT_TARGET_ENDPOINT) {
            static RestApiClient instance = RestApiClient(endpoint);
            return instance;
        }
		~RestApiClient() { Clean(); }
		void SetOption(RestApiOption option, std::string value){ m_client_opts[option] = value; }
		const ApiResponse Head(const HttpRequest request);
		const ApiResponse Get(const HttpRequest request);
		const ApiResponse Put(const HttpRequest request, const std::shared_ptr<FILE> data, struct stat dataInfo);
	protected:
        RestApiClient(const std::string& endpoint)
                : m_apiEndpoint(endpoint)
        {}
        //curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        static RestApiClient* self;
		std::string m_apiEndpoint;
		std::map<int, std::string> m_client_opts;
		struct curl_slist* m_header_struct;
		std::shared_ptr<std::string> m_response_headers;
		std::shared_ptr<std::string> m_response_body;
		CURL* curl;
		void Initialize();
		void Clean();
	private:
		void SetOpts() 
		{
			//PLOG(plog::debug) << "setting client options";
			std::string usr_agent = FindOpt(RESTCLIENTOPT_USER_AGENT);
			if (!usr_agent.empty()) 
			{ 
				//PLOG(plog::debug) << "setting custom useragent";
				curl_slist_append(m_header_struct, std::string("Agent: " + usr_agent).c_str());
			}
			else
			{
				//PLOG(plog::debug) << "setting default user agent";
				curl_slist_append(m_header_struct, std::string("Agent: restfulapi/"+std::to_string(_VERSION)).c_str());
			}

			std::string read_header = FindOpt(RESTCLIENTOPT_READ_HEADERS);
			if (!read_header.empty() && read_header == "true") 
			{
				//PLOG(plog::debug) << "setting header read callback";
				curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, &RestApiClient::HeaderCallBack);
				RestApiClient::WriteData headerData = { m_response_headers, 0 };
				curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headerData);
			}

			std::string read_body = FindOpt(RESTCLIENTOPT_READ_BODY);
			if (!read_body.empty() && read_body == "true")
			{
				//PLOG(plog::debug) << "setting body read callback";
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &RestApiClient::BodyCallBack);
				RestApiClient::WriteData bodyData = { m_response_body, 0 };
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bodyData);
			}

			std::string follow_redirect = FindOpt(RESTCLIENTOPT_FOLLOW_REDIRECT);
			if (!follow_redirect.empty() && follow_redirect == "true")
			{
				//PLOG(plog::debug) << "setting redirect to true";
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1l);
			}

			std::string ssl_disable_verify_peer = FindOpt(RESTCLIENTOPT_SSL_DISABLE_VERIFY_PEER);
			if (!ssl_disable_verify_peer.empty() && ssl_disable_verify_peer == "true")
			{
				//PLOG(plog::debug) << "setting redirect to true";
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0l);
			}

			std::string ssl_disable_verify_host = FindOpt(RESTCLIENTOPT_SSL_DISABLE_VERIFY_HOST);
			if (!ssl_disable_verify_host.empty() && ssl_disable_verify_host == "true")
			{
				//PLOG(plog::debug) << "setting redirect to true";
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0l);
			}
		}
		void SetHeaders(HttpRequest request)
		{
			std::map<std::string, std::string> headers = request.GetHeaders();
			for (auto pair = headers.begin(); pair != headers.end(); ++pair) {
				curl_slist_append(m_header_struct, std::string(pair->first + pair->second).c_str());
			}
		}
		std::string FindOpt(const int opt) {
			std::string optVal = "";
			auto it = m_client_opts.find(opt);
			if (it != m_client_opts.end()) {
				optVal = it->second;
			}
			return optVal;
		}
		struct WriteData 
		{
			std::shared_ptr<std::string> data;
			size_t pos;
		};
		static size_t HeaderCallBack(char* buffer, size_t size, size_t nitems, void* data)
		{
			size_t numBytes = size * nitems;
			auto *writeData = (WriteData*) data;
			writeData->data->append(buffer, numBytes);
			writeData->pos += numBytes;
			return numBytes;
		}
		static size_t BodyCallBack(char* buffer, size_t size, size_t nitems, void* data)
		{
			std::cout << "in body callback" << std::endl;
			size_t numBytes = size * nitems;
			std::cout << "Byte Syze: " << size << std::endl;
			std::cout << "nItems: " << nitems << std::endl;
			std::cout << "Numbytes: " << numBytes << std::endl;
			auto *writeData = (WriteData*) data;
			std::cout << "Writing data";
			writeData->data->append(buffer, numBytes);
			std::cout << "Wrote: " << sizeof writeData->data << std::endl;
			writeData->pos += numBytes;
			std::cout << "pos: " << writeData->pos << std::endl;
			return numBytes;
		}
	};
}
