#ifndef DEDICATED_SERVER_RPC_H
#define DEDICATED_SERVER_RPC_H

#include "dry.h"

#include <godot_cpp/classes/node.hpp>

#ifdef UNIX_ENABLED

class DedicatedServerRPC : public Node {
	GDCLASS(DedicatedServerRPC, Node);

protected:
	static void _bind_methods();

private:
	int _socket = -1;

public:
	DedicatedServerRPC();
	~DedicatedServerRPC();

	void _ready() override;
	void _physics_process(double p_delta) override;
};

class DedicatedServerRPCConnection : public Node {
	GDCLASS(DedicatedServerRPCConnection, Node);

protected:
	static void _bind_methods();

private:
	int _socket = -1;

public:
	explicit DedicatedServerRPCConnection() {} // for doc generator
	DedicatedServerRPCConnection(int socket);
	~DedicatedServerRPCConnection();

	void _physics_process(double p_delta) override;
	PackedByteArray process_command(const PackedByteArray &request);
};

#endif

#endif // DEDICATED_SERVER_RPC_H
