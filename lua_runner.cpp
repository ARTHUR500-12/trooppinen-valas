extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <iostream>
#include <string>

// =====================================================
// Funções exportadas pelo nucleo.cpp (wrapper em C++)
// =====================================================
extern "C" {
    void tv_proteger(int porta);
    void tv_embaralhar(int porta);
    void tv_fantasma(bool ativar);
    void tv_criar_capacidade(const char* recurso, const char* chave);
    void tv_log(const char* mensagem);
    const char* tv_versao();

    // Novas funções
    void tv_ler_log();
    void tv_listar_portas();
    bool tv_verificar_capacidade(const char* recurso);
    int tv_obter_anel();
}

// =====================================================
// Funções Lua que chamam as funções exportadas
// =====================================================

int lua_tv_proteger(lua_State* L) {
    int porta = (int)luaL_checkinteger(L, 1);
    tv_proteger(porta);
    return 0;
}

int lua_tv_embaralhar(lua_State* L) {
    int porta = (int)luaL_checkinteger(L, 1);
    tv_embaralhar(porta);
    return 0;
}

int lua_tv_fantasma(lua_State* L) {
    bool ativar = lua_toboolean(L, 1);
    tv_fantasma(ativar);
    return 0;
}

int lua_tv_criar_capacidade(lua_State* L) {
    const char* recurso = luaL_checkstring(L, 1);
    const char* chave = luaL_checkstring(L, 2);
    tv_criar_capacidade(recurso, chave);
    return 0;
}

int lua_tv_log(lua_State* L) {
    const char* mensagem = luaL_checkstring(L, 1);
    tv_log(mensagem);
    return 0;
}

int lua_tv_versao(lua_State* L) {
    const char* versao = tv_versao();
    lua_pushstring(L, versao);
    return 1;
}

// ---- Novas funções registradas no Lua ----

int lua_tv_ler_log(lua_State* L) {
    tv_ler_log();
    return 0;
}

int lua_tv_listar_portas(lua_State* L) {
    tv_listar_portas();
    return 0;
}

int lua_tv_verificar_capacidade(lua_State* L) {
    const char* recurso = luaL_checkstring(L, 1);
    bool existe = tv_verificar_capacidade(recurso);
    lua_pushboolean(L, existe);
    return 1;
}

int lua_tv_obter_anel(lua_State* L) {
    int anel = tv_obter_anel();
    lua_pushinteger(L, anel);
    return 1;
}

// =====================================================
// Execução de um script Lua
// =====================================================
int executar_lua(const char* nome_arquivo) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_newtable(L);

    lua_pushcfunction(L, lua_tv_proteger);
    lua_setfield(L, -2, "proteger");

    lua_pushcfunction(L, lua_tv_embaralhar);
    lua_setfield(L, -2, "embaralhar");

    lua_pushcfunction(L, lua_tv_fantasma);
    lua_setfield(L, -2, "fantasma");

    lua_pushcfunction(L, lua_tv_criar_capacidade);
    lua_setfield(L, -2, "criar_capacidade");

    lua_pushcfunction(L, lua_tv_log);
    lua_setfield(L, -2, "log");

    lua_pushcfunction(L, lua_tv_versao);
    lua_setfield(L, -2, "versao");

    // Novas funções
    lua_pushcfunction(L, lua_tv_ler_log);
    lua_setfield(L, -2, "ler_log");

    lua_pushcfunction(L, lua_tv_listar_portas);
    lua_setfield(L, -2, "listar_portas");

    lua_pushcfunction(L, lua_tv_verificar_capacidade);
    lua_setfield(L, -2, "verificar_capacidade");

    lua_pushcfunction(L, lua_tv_obter_anel);
    lua_setfield(L, -2, "obter_anel");

    lua_setglobal(L, "tv");

    if (luaL_dofile(L, nome_arquivo) != LUA_OK) {
        std::cerr << "Erro ao executar o script Lua: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return 0;
    }

    lua_close(L);
    return 1;
}