#include "tcp_connection.h"
#include "../event/event_loop.h"

void on_close_handle(uv_handle_t* handle) {
	delete handle;
}

EQ::Net::TCPConnection::TCPConnection(uv_tcp_t *socket)
{
	m_socket = socket;
	m_socket->data = this;
}

EQ::Net::TCPConnection::~TCPConnection() {
	Disconnect();
}

void EQ::Net::TCPConnection::Connect(const std::string &addr, int port, bool ipv6, std::function<void(std::shared_ptr<TCPConnection>)> cb)
{
	struct EQTCPConnectBaton
	{
		uv_tcp_t *socket;
		std::function<void(std::shared_ptr<EQ::Net::TCPConnection>)> cb;
	};

	auto loop = EQ::EventLoop::Get().Handle();
	uv_tcp_t *socket = new uv_tcp_t;
	memset(socket, 0, sizeof(uv_tcp_t));
	uv_tcp_init(loop, socket);

	if (ipv6) {
		sockaddr_in6 iaddr;
		uv_ip6_addr(addr.c_str(), port, &iaddr);

		uv_connect_t *connect = new uv_connect_t;
		memset(connect, 0, sizeof(uv_connect_t));

		EQTCPConnectBaton *baton = new EQTCPConnectBaton;
		baton->cb = cb;
		baton->socket = socket;
		connect->data = baton;
		uv_tcp_connect(connect, socket, (sockaddr*)&iaddr,
			[](uv_connect_t* req, int status) {
			EQTCPConnectBaton *baton = (EQTCPConnectBaton*)req->data;
			auto socket = baton->socket;
			auto cb = baton->cb;

			delete baton;

			if (status < 0) {
				uv_close((uv_handle_t*)socket, on_close_handle);
				delete req;
				cb(nullptr);
			}
			else {
				delete req;
				std::shared_ptr<EQ::Net::TCPConnection> connection(new EQ::Net::TCPConnection(socket));
				cb(connection);
			}
		});
	}
	else {
		sockaddr_in iaddr;
		uv_ip4_addr(addr.c_str(), port, &iaddr);

		uv_connect_t *connect = new uv_connect_t;
		memset(connect, 0, sizeof(uv_connect_t));

		EQTCPConnectBaton *baton = new EQTCPConnectBaton;
		baton->cb = cb;
		baton->socket = socket;
		connect->data = baton;
		uv_tcp_connect(connect, socket, (sockaddr*)&iaddr,
			[](uv_connect_t* req, int status) {
			EQTCPConnectBaton *baton = (EQTCPConnectBaton*)req->data;
			auto socket = baton->socket;
			auto cb = baton->cb;

			delete baton;

			if (status < 0) {
				uv_close((uv_handle_t*)socket, on_close_handle);
				delete req;
				cb(nullptr);
			}
			else {
				delete req;
				std::shared_ptr<EQ::Net::TCPConnection> connection(new EQ::Net::TCPConnection(socket));
				cb(connection);
			}
		});
	}
}

void EQ::Net::TCPConnection::Start() {
	uv_read_start((uv_stream_t*)m_socket, [](uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
		buf->base = new char[suggested_size];
		buf->len = suggested_size;
	}, [](uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {

		TCPConnection *connection = (TCPConnection*)stream->data;

		if (nread > 0) {
			connection->Read(buf->base, nread);

			if (buf->base) {
				delete[] buf->base;
			}
		}
		else if (nread == UV_EOF) {
			if (buf->base) {
				delete[] buf->base;
			}
		}
		else if (nread < 0) {
			connection->Disconnect();

			if (buf->base) {
				delete[] buf->base;
			}
		}
	});
}

void EQ::Net::TCPConnection::OnRead(std::function<void(TCPConnection*, const unsigned char*, size_t)> cb)
{
	m_on_read_cb = cb;
}

void EQ::Net::TCPConnection::OnDisconnect(std::function<void(TCPConnection*)> cb)
{
	m_on_disconnect_cb = cb;
}

void EQ::Net::TCPConnection::Disconnect()
{
	if (m_socket) {
		uv_close((uv_handle_t*)m_socket, on_close_handle);
		m_socket = nullptr;
	
		if (m_on_disconnect_cb) {
			m_on_disconnect_cb(this);
		}
	}
}

void EQ::Net::TCPConnection::Read(const char *data, size_t count)
{
	if (m_on_read_cb) {
		m_on_read_cb(this, (unsigned char*)data, count);
	}
}

void EQ::Net::TCPConnection::Write(const char *data, size_t count)
{
	if (!m_socket) {
		return;
	}

	uv_write_t *write_req = new uv_write_t;
	memset(write_req, 0, sizeof(uv_write_t));
	write_req->data = this;
	uv_buf_t send_buffers[1];
	send_buffers[0].base = (char*)data;
	send_buffers[0].len = count;

	uv_write(write_req, (uv_stream_t*)m_socket, send_buffers, 1, [](uv_write_t* req, int status) {
		EQ::Net::TCPConnection *connection = (EQ::Net::TCPConnection*)req->data;
		delete req;

		if (status < 0) {
			connection->Disconnect();
		}
	});
}