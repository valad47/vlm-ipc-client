#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "lua.h"
#include "lualib.h"
#include <string.h>
#include <errno.h>

int vlm_connect_socket(lua_State *L) {
    size_t size = 0;
    const char *spath = luaL_checklstring(L, 1, &size);

    struct sockaddr_un path = { AF_UNIX };
    memcpy(path.sun_path, spath, size);

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    connect(sock, (struct sockaddr*)&path, offsetof(struct sockaddr_un, sun_path) + size + 1);

    lua_pushinteger(L, sock);
    return 1;
}

int vlm_recv(lua_State *L) {
    int connection = luaL_checkinteger(L, 1);
    char *data = malloc(1024);
    if(!data) {
        luaL_error(L, "Failed to allocate [%d] bytes of memory for socket [%d]\n",
                   1024, connection);
    }
    char *handle = data;
    ssize_t length = 0;
    ssize_t size = 1024;
    ssize_t received = 0;

    while((received = recv(connection, handle, size-length, 0)) == 1024) {
        length += received;
        if(length == size) {
            data = realloc(data, size + 1024);
            size += 1024;
        }
        handle = data + length;
    }
    length += received;
    if(length == 0 && received < 0)
    luaL_error(L, "Failed to receive data: %s", strerror(errno));
    if(length) lua_pushlstring(L, data, length);
    else lua_pushnil(L);
    free(data);
    return 1;
}


int vlm_send(lua_State *L) {
    size_t size = 0;
    const char *msg = luaL_checklstring(L, 2, &size);
    int sock = luaL_checkinteger(L, 1);
    lua_pushinteger(L, send(sock, msg, size, 0));
    return 1;
}

int vlm_close(lua_State *L) {
    close(luaL_checkinteger(L, 1));
    return 0;
}
