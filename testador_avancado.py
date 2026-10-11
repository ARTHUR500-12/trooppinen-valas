import subprocess
import time
import os
import html

# Configurações
NUCLEO = "trooppinen_nucleo"
PASTA_PROJETO = "/storage/emulated/0/Download/trooppinen"
PASTA_TEMP = "/storage/emulated/0/Download/trooppinen/testes_auto"
RELATORIO_HTML = "/storage/emulated/0/Download/trooppinen/relatorio_testes.html"

# Cria pasta temporária para scripts de teste
os.makedirs(PASTA_TEMP, exist_ok=True)

# Lista de testes: cada teste é um dicionário com:
# - nome: descrição
# - script_tv: conteúdo do .tv
# - esperado: substring que deve aparecer na saída
# - args: argumentos adicionais (ex: ["--anel", "0"])
testes = [
    {
        "nome": "Exibir mensagem",
        "script_tv": 'exibir "ola mundo";\n',
        "esperado": "ola mundo",
        "args": []
    },
    {
        "nome": "Versao da linguagem",
        "script_tv": "versao;\n",
        "esperado": "Trooppinen Valas v1.7",
        "args": []
    },
    {
        "nome": "Proteger porta com capacidade (anel 0)",
        "script_tv": 'criar chave para porta = "chave_mestra";\nproteger porta 80;\n',
        "esperado": "Protegendo a porta 80",
        "args": ["--anel", "0"]
    },
    {
        "nome": "Embaralhar porta com capacidade",
        "script_tv": 'criar chave para porta = "chave_mestra";\nembaralhar porta 80;\n',
        "esperado": "Embaralhando porta 80",
        "args": ["--anel", "0"]
    },
    {
        "nome": "Ativar fantasma (anel 2)",
        "script_tv": "ativar fantasma;\n",
        "esperado": "Modo fantasma ATIVADO",
        "args": ["--anel", "2"]
    },
    {
        "nome": "Validar script com comando inexistente",
        "script_tv": 'validar script "script_invalido.tv";\n',
        "esperado": "Comando invalido",
        "args": ["--anel", "0"],
        "preparar_arquivo": ("script_invalido.tv", "exibir \"teste\";\ncomando_inexistente;\n")
    },
    {
        "nome": "Ler log",
        "script_tv": "ler log;\n",
        "esperado": "HISTORICO DE DEFESA",
        "args": []
    },
    # Teste do Lua nativo
    {
        "nome": "Lua nativo: proteger porta",
        "script_tv": 'executar script "teste_lua_rapido.lua";\n',
        "esperado": "Protegendo a porta 80",
        "args": ["--anel", "0"],
        "preparar_arquivo": ("teste_lua_rapido.lua", 'tv.log("teste");\ntv.criar_capacidade("porta", "chave_mestra")\ntv.proteger(80)\n')
    }
]

# Função para gerar um script temporário
def criar_script_temp(nome_base, conteudo):
    caminho = os.path.join(PASTA_TEMP, nome_base)
    with open(caminho, "w") as f:
        f.write(conteudo)
    return caminho

# Prepara arquivos adicionais necessários para alguns testes
for t in testes:
    if "preparar_arquivo" in t:
        nome_extra, conteudo_extra = t["preparar_arquivo"]
        # O arquivo extra será criado na pasta do projeto (onde o motor procura)
        caminho_extra = os.path.join(PASTA_PROJETO, nome_extra)
        with open(caminho_extra, "w") as f:
            f.write(conteudo_extra)

# Executa os testes
resultados = []
for t in testes:
    nome = t["nome"]
    script_tv_content = t["script_tv"]
    esperado = t["esperado"]
    args = t["args"]

    # Cria arquivo .tv temporário
    caminho_tv = criar_script_temp(f"teste_{abs(hash(nome))%10000}.tv", script_tv_content)

    # Prepara comando
    comando = [NUCLEO, caminho_tv] + args

    # Marca tempo de início
    inicio = time.time()

    try:
        resultado = subprocess.run(comando, capture_output=True, text=True, timeout=10)
        stdout = resultado.stdout
        stderr = resultado.stderr
        sucesso = esperado in stdout
        tempo = time.time() - inicio
        detalhe = f"stdout: {stdout.strip()[:200]}" + (f"\nstderr: {stderr.strip()[:200]}" if stderr.strip() else "")
    except subprocess.TimeoutExpired:
        sucesso = False
        tempo = time.time() - inicio
        detalhe = "TIMEOUT (mais de 10s)"
    except Exception as e:
        sucesso = False
        tempo = time.time() - inicio
        detalhe = f"ERRO: {str(e)}"

    resultados.append({
        "nome": nome,
        "sucesso": sucesso,
        "tempo": tempo,
        "detalhe": detalhe
    })

    # Remove o .tv temporário
    try:
        os.remove(caminho_tv)
    except:
        pass

# Gera relatório HTML
linhas_html = []
linhas_html.append("<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Relatório de Testes - Trooppinen Valas</title>")
linhas_html.append("<style>body{font-family:sans-serif;background:#111;color:#eee;}table{border-collapse:collapse;width:100%;}th,td{border:1px solid #333;padding:8px;text-align:left;}.pass{color:lime;}.fail{color:red;}</style></head><body>")
linhas_html.append("<h1>Relatório de Testes Automáticos</h1>")
linhas_html.append(f"<p>Gerado em: {time.strftime('%d/%m/%Y %H:%M:%S')}</p>")
linhas_html.append("<table><tr><th>Teste</th><th>Status</th><th>Tempo (s)</th><th>Detalhes</th></tr>")

for r in resultados:
    classe = "pass" if r["sucesso"] else "fail"
    status = "PASSOU" if r["sucesso"] else "FALHOU"
    linhas_html.append(f"<tr><td>{html.escape(r['nome'])}</td><td class='{classe}'>{status}</td><td>{r['tempo']:.3f}</td><td><pre>{html.escape(r['detalhe'])}</pre></td></tr>")

linhas_html.append("</table></body></html>")

with open(RELATORIO_HTML, "w") as f:
    f.write("\n".join(linhas_html))

# Imprime resumo no console
total = len(resultados)
passaram = sum(1 for r in resultados if r["sucesso"])
falharam = total - passaram
print(f"\n=== RELATÓRIO DE TESTES ===")
print(f"Total: {total} | Passaram: {passaram} | Falharam: {falharam}")
for r in resultados:
    status = "✅" if r["sucesso"] else "❌"
    print(f"{status} {r['nome']} ({r['tempo']:.3f}s)")
print(f"\nRelatório HTML gerado em: {RELATORIO_HTML}")