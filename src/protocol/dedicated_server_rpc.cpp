#include "protocol/dedicated_server_rpc.h"

#ifdef UNIX_ENABLED

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define MAX_WAITING_CONNECTIONS 256
#define CRASH_SOCKET(m_cond, m_msg) \
	if (unlikely(m_cond)) { \
		perror(m_msg); \
		CRASH_NOW_MSG("socket encountered fatal error: " #m_cond); \
	}

void DedicatedServerRPC::_bind_methods() {
}

DedicatedServerRPC::DedicatedServerRPC() {
}

DedicatedServerRPC::~DedicatedServerRPC() {
	if (_socket != -1) {
		close(_socket);
		_socket = -1;
	}
}

void DedicatedServerRPC::_ready() {
	_socket = socket(AF_UNIX, SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
	// dedicated server crashes on fatal error.
	CRASH_SOCKET(_socket == -1, "socket");

	const sockaddr_un name = {
		.sun_family = AF_UNIX,
		.sun_path = "/go/src/spy-cards/godot.sock",
	};
	const int bind_ret = bind(_socket, reinterpret_cast<const sockaddr *>(&name), sizeof(name));
	CRASH_SOCKET(bind_ret == -1, "bind");

	const int listen_ret = listen(_socket, MAX_WAITING_CONNECTIONS);
	CRASH_SOCKET(listen_ret == -1, "listen");
}

void DedicatedServerRPC::_physics_process(double p_delta) {
	if (unlikely(_socket == -1)) {
		return;
	}

	for (;;) {
		int conn = accept(_socket, nullptr, nullptr);
		if (likely(conn == -1)) {
			CRASH_SOCKET(errno != EAGAIN && errno != EWOULDBLOCK, "accept");

			return;
		}

		add_child(memnew(DedicatedServerRPCConnection(conn)));
	}
}

void DedicatedServerRPCConnection::_bind_methods() {
}

DedicatedServerRPCConnection::DedicatedServerRPCConnection(int socket) : _socket(socket) {
}

DedicatedServerRPCConnection::~DedicatedServerRPCConnection() {
	if (_socket != -1) {
		close(_socket);
		_socket = -1;
	}
}

void DedicatedServerRPCConnection::_physics_process(double p_delta) {
	if (unlikely(_socket == -1)) {
		queue_free();
		return;
	}

	for (;;) {
		// check if there's a message.
		char dummy[1];
		ssize_t size = recv(_socket, dummy, sizeof(dummy), MSG_DONTWAIT | MSG_PEEK | MSG_TRUNC);

		if (likely(size == -1)) {
			// we hit an error; if it's EAGAIN, that means we don't have a message. otherwise, something broke.
			CRASH_SOCKET(errno != EAGAIN, "recv");

			return;
		} else if (unlikely(size) == 0) {
			// stream was closed from the other end
			shutdown(_socket, SHUT_RDWR);
			close(_socket);
			_socket = -1;
			queue_free();

			return;
		}

		// we have a message! actually allocate the buffer and receive it.
		PackedByteArray message;
		message.resize(size);

		ssize_t n = read(_socket, message.ptrw(), size);
		CRASH_SOCKET(n != size, "read");

		PackedByteArray response = process_command(message);

		// send back the response in two packets to avoid having to allocate a buffer for a packet we don't know the size of in Go.
		uint64_t response_size = response.size();
		n = write(_socket, &response_size, sizeof(response_size));
		CRASH_SOCKET(n != sizeof(response_size), "write (size)");

		if (response_size) {
			// now the actual data. (if we have any.)
			n = write(_socket, response.ptr(), response.size());
			CRASH_SOCKET(n != response.size(), "write");
		}
	}
}

PackedByteArray DedicatedServerRPCConnection::process_command(const PackedByteArray &request) {
	ERR_PRINT(vformat("TODO: do something with buffer (%d bytes)", request.size())); // TODO: oops

	// for now, just write a zero.
	return PackedByteArray{0};
}

#endif
