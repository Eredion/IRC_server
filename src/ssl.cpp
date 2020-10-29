#include "../inc/ft_irc.hpp"

void Server::ssl_init()
{
	const SSL_METHOD *method;

	SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();
	method = TLS_server_method();
	this->ctx = SSL_CTX_new(method);
	if (!ctx)
		ft_perror("Unable to create SSL context");
	SSL_CTX_set_ecdh_auto(ctx, 1);
	if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
		ft_perror("SSL error");
	}
	if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
		ft_perror("SSL error");
	}
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, listener);
}

void Server::ssl_new_connection()
{
	int					newfd;
	socklen_t			addrlen;
	struct sockaddr_in	remoteaddr;
	SSL *ssl;

	addrlen = sizeof(remoteaddr);
	newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);
	if (newfd == -1)
	{
		std::cerr << ("Error: Accept.") << std::endl;
		return ;
	}
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, newfd);
	FD_SET(newfd, &master);
	if (newfd > fdmax)
		fdmax = newfd;
	if (SSL_accept(ssl) <= 0)
	{
		print("accept error");
        ERR_print_errors_fp(stderr);
		return;
	}
	Client *tmp = new Client(newfd);
	tmp->setSSL(ssl);
	tmp->setIP(inet_ntoa(remoteaddr.sin_addr));
	clients.push_back(tmp);
	sendmsg(tmp->socket, "NOTICE * : PLEASE LOG IN");
	std::cout << "New connection from " << inet_ntoa(remoteaddr.sin_addr)
			<< " on socket " << newfd << std::endl;
}

void Client::setSSL(SSL *ssl){
	this->ssl = ssl;
}

void Server::ssl_receive(int socket)
{
	int rec = 0;
	Client *tmp;
// int SSL_read(SSL *ssl, void *buf, int num);

	tmp = getClient(socket);
	if ((rec = SSL_read(tmp->ssl, &tmp->buff[tmp->count], 512 - tmp->count)) >= 0)
	{
		tmp->count += rec;
	}
	if (rec == -1)
	{
		print(strerror(rec));
		std::cerr << "Error: recv returned -1" << std::endl;
	}
	else if (rec == 0)
	{
		std::cerr << "Error: socket " << socket << " disconnected\n";
		close(socket);
		SSL_shutdown(tmp->ssl);
        SSL_free(tmp->ssl);
		deleteClient(socket);
		FD_CLR(socket, &master);
	}
	if (std::string(tmp->buff).find("\n") != std::string::npos || tmp->count >= 512)
	{
		std::cout << "Received " << tmp->buff << " from " << tmp->socket<< "\n";
		Message msg(tmp->buff, tmp);
		exec(msg);
		tmp->resetBuffer();
	}
}