#!/bin/bash

# =====================================================
#  Instalador Mestre da Trooppinen Valas
#  Compila e instala: motor, Lua, Forth, watchdog e Python
#  Uso: bash install_master.sh
# =====================================================

set -e

PREFIX_BIN="$PREFIX/bin"
PROJ_DIR="/storage/emulated/0/Download/trooppinen"
HOME_DIR="$HOME"

echo "============================================"
echo " INSTALADOR MESTRE DA TROOPPINEN VALAS"
echo "============================================"

# 1. Verificações básicas
echo "[1/8] Verificando arquivos essenciais..."
if [ ! -f "$PROJ_DIR/nucleo.cpp" ] || [ ! -f "$PROJ_DIR/lua_runner.cpp" ]; then
    echo "ERRO: nucleo.cpp ou lua_runner.cpp nao encontrados."
    exit 1
fi
if [ ! -f "$PROJ_DIR/watchdog.c" ]; then
    echo "AVISO: watchdog.c nao encontrado. Pulando watchdog."
fi
if [ ! -f "$PROJ_DIR/lua_lib/liblua.a" ]; then
    echo "ERRO: Biblioteca Lua nao encontrada em lua_lib/."
    exit 1
fi

# 2. Compilar o motor C++
echo "[2/8] Compilando o motor (nucleo.cpp + lua_runner.cpp)..."
cd "$PROJ_DIR"
clang++ nucleo.cpp lua_runner.cpp -o nucleo \
    -I lua_lib/include -L lua_lib -llua -lm -Wall -std=c++17
echo "      Motor compilado com sucesso."

# 3. Compilar o watchdog
if [ -f "$PROJ_DIR/watchdog.c" ]; then
    echo "[3/8] Compilando o watchdog..."
    gcc watchdog.c -o watchdog
    echo "      Watchdog compilado."
else
    echo "[3/8] Watchdog nao encontrado. Pulando."
fi

# 4. Instalar o motor e o comando global
echo "[4/8] Instalando o motor em $PREFIX_BIN..."
install -m 755 nucleo "$PREFIX_BIN/trooppinen_nucleo"

cat > "$PREFIX_BIN/trooppinen" << 'EOF'
#!/bin/bash
trooppinen_nucleo "$@"
EOF
chmod +x "$PREFIX_BIN/trooppinen"
echo "      Comando 'trooppinen' pronto."

# 5. Copiar arquivos para a home (necessários para o watchdog)
echo "[5/8] Copiando arquivos de suporte para a home..."
cp -f nucleo "$HOME_DIR/trooppinen_nucleo"
chmod +x "$HOME_DIR/trooppinen_nucleo"
cp -f nucleo "$PROJ_DIR/nucleo_backup"
cp -f nucleo "$HOME_DIR/nucleo_backup"
if [ -f "$PROJ_DIR/watchdog" ]; then
    cp -f "$PROJ_DIR/watchdog" "$HOME_DIR/watchdog"
    chmod +x "$HOME_DIR/watchdog"
fi
if [ -f "$PROJ_DIR/defesa.tv" ]; then
    cp -f "$PROJ_DIR/defesa.tv" "$HOME_DIR/defesa.tv"
fi
echo "      Arquivos copiados."

# 6. Verificar dependências (Python, gforth)
echo "[6/8] Verificando dependencias opcionais..."
if command -v python >/dev/null 2>&1; then
    echo "      Python: OK"
else
    echo "      AVISO: Python nao encontrado. Instale com 'pkg install python'."
fi
if command -v gforth >/dev/null 2>&1; then
    echo "      Gforth: OK"
else
    echo "      AVISO: Gforth nao encontrado. Instale com 'pkg install gforth'."
fi

# 7. Criar arquivos auxiliares (idiomas.map, defesa.log)
echo "[7/8] Criando arquivos auxiliares..."
if [ ! -f "$PROJ_DIR/idiomas.map" ]; then
    cat > "$PROJ_DIR/idiomas.map" << 'EOF'
protect port=proteger porta 
shuffle port=embaralhar porta 
show ports=mostrar portas
activate ghost=ativar fantasma
deactivate ghost=desativar fantasma
read log=ler log
learn attacks=aprender ataques
metamorphose every=metamorfosear a cada
help=ajuda
version=versao
quit=sair
exit=sair
EOF
fi
if [ ! -f "$PROJ_DIR/defesa.log" ]; then
    touch "$PROJ_DIR/defesa.log"
fi
if [ ! -f "$HOME_DIR/defesa.log" ]; then
    touch "$HOME_DIR/defesa.log"
fi
echo "      Arquivos auxiliares prontos."

# 8. Instalação concluída
echo "[8/8] Finalizando..."
echo "============================================"
echo " INSTALACAO CONCLUIDA COM SUCESSO!"
echo "============================================"
echo " Comandos disponiveis:"
echo "   trooppinen <arquivo.tv> [--anel <0-4>]"
echo "   ./watchdog  (rodar da home)"
echo "   python testador_avancado.py"
echo "   gforth guarda_logs.fs"
echo ""
echo " Para testar rapidamente:"
echo "   echo 'exibir \"ola\";' > ~/teste.tv"
echo "   trooppinen ~/teste.tv"
echo "============================================"