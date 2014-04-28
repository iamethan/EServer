#include <iostream>
#include "uv.h"
using namespace std;

uv_buf_t alloc_buf(uv_handle_t* handle, size_t size)
{
	char buf[256];
	return uv_buf_init(buf, sizeof(buf));
}

void on_disconnected(uv_stream_t* stream)
{
	printf("on discontected\n");

	uv_read_stop(stream);
	uv_close((uv_handle_t*)stream, NULL);
}

void on_write(uv_write_t* req, int status)
{
	delete req;
	printf("after write!<%d> <%d>\n", status);
}

void write(uv_stream_t* stream)
{
	uv_write_t* write = new uv_write_t;
	char ch[32] = "hello world\n";
	uv_buf_t b = uv_buf_init(ch, sizeof(ch));

	int r = uv_write(write, stream, &b, 1, on_write);
	if (r) {
		printf("write error:<%s>\n", uv_err_name(r));
	}
}

void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buff)
{
	printf("on_read: len<%d>, msg<%d, %s>\n", nread, buff.len, buff.base);
	
	if (nread < 0 || !uv_is_readable(stream)) {
		on_disconnected(stream);
		return;
	}

	// send
	write(stream);
}

void on_new_connection(uv_stream_t* server, int status)
{
	if (status == -1) {
		return;
	}

	uv_tcp_t* client = new uv_tcp_t;
	uv_tcp_init(uv_default_loop(), client);

	int r = uv_accept(server, (uv_stream_t*)client);
	if (r) {
		uv_close((uv_handle_t*)client, NULL);
		return;
	}

	uv_read_start((uv_stream_t*)client, alloc_buf, on_read);

	printf("accept a new client...\n");
}

int main()
{
	uv_loop_t* loop = uv_default_loop();

	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	uv_tcp_bind(&server, uv_ip4_addr("0.0.0.0", 3366));

	int r = uv_listen((uv_stream_t*)&server, 128, on_new_connection);
	if (r) {
		fprintf(stderr, "Listen error %s\n", uv_err_name(r));
		return 1;
	}

	printf("server started...\n");

	while (1) {
		uv_run(loop, UV_RUN_NOWAIT);
		Sleep(100);
	}

	return 0;
}