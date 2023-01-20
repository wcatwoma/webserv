#include "CGI.hpp"
#include "./../utils/UtilsPrint.ipp"

CGI::CGI() {
    _env = vector_string();
    _argv = vector_string();
    _url = URL();
    _server = Server();
    _request = Request();
    _expansion = string();
    _url_cgi_file = string();
}

CGI::~CGI() {}

CGI::CGI(CGI const &x) {
    *this = x;
}

CGI     &CGI::operator=(const CGI &x) {
    _env = x._env;
    _argv = x._argv;
    _url = x._url;
    _server = x._server;
    _request = x._request;
    _expansion = x._expansion;
    _url_cgi_file = x._url_cgi_file;
    return (*this);
}

void    CGI::set_url(URL url) {
    _url = url;
}

void    CGI::set_server(const Server &x) {
    _server = x;
}

void    CGI::set_request(const Request &x) {
    _request = x;
}

void    CGI::set_expansion(string expansion) {
    _expansion = expansion;
}

void    CGI::set_filename_script(string filename_script) {
    _filename_script = filename_script;
}

void    CGI::set_url_cgi_file(string cgi_file) {
    _url_cgi_file = cgi_file;
}

void    CGI::set_data_socket_client(sockaddr_t data_socket_client) {
    _data_socket_client = data_socket_client;
}

string  CGI::start() {
    int             status;
    pid_t           pid;
    string          cgi_exec;
    string          cgi_script;
    vector_string   argv;
    char            **argv_c;
    char            **env_c;
    int             fd_write;
    int             fd_file;

    set_env();
    cgi_exec = "/Users/qmarowak/Desktop/WebServ/" + trim_line(_server.get_cgi_pass(), "./");
    cgi_script = "/Users/qmarowak/Desktop/WebServ/cgi_tester";
    argv.push_back(cgi_script);
    _env.push_back("SCRIPT_FILENAME=youpi.bla");
    argv_c = convert_array_string_to_char(argv);
    env_c = convert_array_string_to_char(_env);

    for (int i = 0; env_c[i] != NULL; i++) {
        std::cout << "str " << i << ": ";
        std::cout << env_c[i] << "\n";
    }
    for (int i = 0; argv_c[i] != NULL; i++) {
        std::cout << "str " << i << ": ";
        std::cout << argv_c[i] << "\n";
    }
    fd_write = dup(1);
    int fd = open("./youpi.bla", O_RDONLY);
    fd_file = open("./tmp.txt", O_CREAT | O_WRONLY | O_TRUNC, ~0);
    if (fd_file < 0) {
        std::cout << "fd_file err\n";
        exit(1);
    }
    if ((pid = fork()) < 0) {
        print_error("Error: fork");
    } else if (pid == 0) {

        if (dup2(fd, 0) < 0) {
            print_error("Error: dup2");
        }
        if (dup2(fd_file, 1) < 0) {
            print_error("Error: dup2");
        }
        if (execve("cgi_tester", argv_c, env_c) < 0) {
            print_error("Error: dup2");
        }
    }
    waitpid(pid, &status, 0);
    close(fd_file);
    close(fd);
    free_array_char(argv_c, argv.size());
    free_array_char(env_c, _env.size());
    fd_file = open("./tmp.txt", O_RDONLY);

    string res = File::read_file(fd_file);
    return (res);
}

char **CGI::convert_array_string_to_char(vector_string array_string) {
    char **array_char;

    array_char = (char**)malloc(sizeof(char*) * (array_string.size() + 1));
    for (int i = 0; i < array_string.size(); i++) {
        array_char[i] = strdup(array_string[i].c_str());
    }
    array_char[array_string.size()] = NULL;
    return (array_char);
}

void    CGI::free_array_char(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

void    CGI::set_env() {
    string  value;

    _env.push_back(definition_auth_type());
    _env.push_back(definition_content_length());
    _env.push_back(definition_content_type());
    _env.push_back(definition_path_info());
    _env.push_back(definition_path_translated());
    _env.push_back(definition_request_method());
    _env.push_back(definition_request_uri());
    _env.push_back(definition_script_name());
    _env.push_back(definition_server_protocol());
    
}

string  CGI::definition_auth_type() {
    vector_string auth_basic;
    
    auth_basic = _server.get_auth_basic();
    if (auth_basic.empty()) {
        return ("AUTH_TYPE=");
    }
    return ("AUTH_TYPE=" + auth_basic[0]);
}

string  CGI::definition_content_length() {
    string body;
    string content_length;
 
    if (_request.get_method() != "POST") {
        return ("");
    }
    body = _request.get_body();
    content_length = "CONTENT_LENGHT=100000000";
    return (content_length);

}

string  CGI::definition_content_type() {
    string content_type;

    if (_expansion.empty())
        return ("");
    content_type = "CONTENT_TYPE=text/file";
    return (content_type);
}

string  CGI::definition_gateway_interface() {
    return (string("GATEWAY_INTERFACE=CGI/1.1"));
}

string CGI::definition_path_info() {
    return ("PATH_INFO=/directory/youpi.bla");
}

string CGI::definition_path_translated() {
    return ("PATH_TRANSLATED=youpi.bla");
}

string CGI::definition_query_string() {
    vector_string   keys;
    vector_string   value;
    string          result;

    if (_url.get_values_argv().empty() || _request.get_method() != "GET") {
        return ("QUERY_STRING=");
    }
    keys = _url.get_keys_argv();
    value = _url.get_values_argv();
    result = "QUERY_STRING=" + keys[0] + "=" + value[0];;
    for (int i = 1; i < keys.size(); i++) {
        result = result + "&" + keys[i] + "=" + value[i];
    }
    return (result);
}

string CGI::definition_remote_addr() {
    return ("REMOTE_ADDR=" + string(inet_ntoa(_data_socket_client.sin_addr)));
}

string CGI::definition_remote_ident() {
    return ("REMOTE_IDENT=");
}

string CGI::definition_remote_user() {
    return ("REMOTE_USER=");
}

string CGI::definition_request_method() {
    return ("REQUEST_METHOD=" + _request.get_method());
}

string CGI::definition_request_uri() {
    vector_string   request_uri;
    
    if (request_uri.empty()) {
        return ("REQUEST_URI=/directory/youpi.bla");
    }
    request_uri = _request.get_values_header("Content-Location");
    return ("REQUEST_URI=" + request_uri[0]);
}

string CGI::definition_script_name() {
    if (_url_cgi_file.empty())
        return ("SCRIPT_NAME=youpi.bla");
    return ("SCRIPT_NAME=" + _url_cgi_file);
}

string CGI::definition_server_name() {
    return ("SERVER_NAME=WEBSERV 1.1");
}

string CGI::definition_server_port() {
    return ("SERVER_PORT=" + std::to_string(_server.get_port()));
}

string CGI::definition_server_protocol() {
    return ("SERVER_PROTOCOL=HTTP/1.1");
}

string CGI::definition_server_software() {
    return ("SERVER_SOFTWARE=WebServ/1.1");
}
