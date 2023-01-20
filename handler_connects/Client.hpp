#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client;

#include <string>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "./../parse_confige/Server.hpp"
#include "./../utils/decode_base64.hpp"
#include "./../utils/File.hpp"
#include "./../create_response/Autoindex.hpp"
#include "./../parse_url/URL.hpp"
#include "./../utils/Structs.hpp"
#include "./../parse_request/Request.hpp"
#include "./../parse_request/ParseRequest.hpp"
#include "./../create_response/Response.hpp"
#include "./../create_response/ErrorPage.hpp"
#include "./../CGI/CGI.hpp"

enum	Status {
	READ,
	WRITE,
	CLOSE,
	NOTWRITEHEADLER,
	NULLPTR
};

class Client {
public:
	typedef std::string string;

	Client();
	~Client();
	Client(int fd);
	Client(Client const &x);
	Client	&operator=(Client const &x);

private:
	enum Status				_status;
	struct sockaddr_in  	_data_socket;
	string					_buffer_request;
	int						_fd;
	Server					_server;
	Response				_response;
	Request					_request;
	URL						_url;

public:
	enum Status			get_status();
	string				get_response();
	Request     		&get_request();
	int					get_fd();
	struct sockaddr_in	get_data_socket();
	string				get_buffer_request();
	void				set_status(enum Status x);
	void				set_request(string headlers, int flag);
	void				set_fd(int const &x);
	void				set_server(Server server);
	void				set_data_socket(struct sockaddr_in data_socket);
	void				close_fd(void);
	void				clear(void);

	bool				is_first;
	int					fd;
	string				method;

private:
    bool        check_data_user(string str_in, string str_find);
	void		shape_the_response(void);
    void		shape_the_response_for_logged_user(vector_string shredded_url, Server &server);
    void        registration_run(string body);
	void        authorization_run();
    bool        authorization_run(std::vector<string>, Server);
    void		index_run(Server &server);
    void		index_run(Server &server, string root, string name_file);
	void		index_run(Server &server, vector_string shredded_url);
	void		cgi_run(Server &server, vector_string shredded_url);
	void		method_delete_run(Server &server, vector_string shredded_url);
	void		method_put_run(Server &server, vector_string shredded_url);
	void		method_post_run(Server &server, vector_string shredded_url);
	void		autoindex_run(Server &server, vector_string shredded_url);
	void		redirect_run(Server &server);
	bool		check_error_max_body(Server &server);
	bool		check_error_allow_methods(vector_string allow_methods);
	void		error_run(Server &server, int code_error);
	Server		&find_location(Server &server, vector_string &shredded_url);
	int			find_count_coincidence(vector_string shredded_url_location,
										vector_string shredded_url);
	string		glue_link(vector_string &shredded_url);

	bool	is_directory(string path);
	string	ft_getcwd(void);

	string response_http(string body);
};

#endif
