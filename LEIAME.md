<div align="center">

<img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cplusplus/cplusplus-original.svg" width="64" alt="C++"/>
<img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/lua/lua-original.svg" width="64" alt="Lua"/>
<img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" width="64" alt="C"/>
<img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/python/python-original.svg" width="64" alt="Python"/>

# TROOPPINEN VALAS

**A linguagem de programação que protege servidores Linux**

<br>

![Versão](https://img.shields.io/badge/vers%C3%A3o-1.7-4FC3F7?style=for-the-badge&labelColor=000000)
![Status](https://img.shields.io/badge/status-est%C3%A1vel-66BB6A?style=for-the-badge&labelColor=000000)
![Licença](https://img.shields.io/badge/licen%C3%A7a-MIT-FFA726?style=for-the-badge&labelColor=000000)
![Plataforma](https://img.shields.io/badge/plataforma-Linux-EF5350?style=for-the-badge&labelColor=000000)

<br>

[**Introdução**](#01--introdução) · [**Instalação**](#06--instalação) · [**Comandos**](#08--comandos) · [**Arquitetura**](#04--arquitetura) · [**Roadmap**](#17--roadmap)

</div>

---

## ◈ Visão Geral

**Trooppinen Valas** *(finlandês: Baleia Tropical)* é uma linguagem de programação interpretada para proteger servidores Linux contra ataques cibernéticos. Combina a simplicidade de scripts em português com o poder de um sistema de defesa automatizado.

> **"Não é apenas uma linguagem. É um escudo vivo que aprende, se esconde e compartilha."**
> — Arthur, criador

---

## ◈ Stack Tecnológica

<div align="center">

| | Tecnologia | Função | Arquivo |
|:---:|:---|:---|:---|
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cplusplus/cplusplus-original.svg" width="32"/> | **C++** | Motor principal | `nucleo.cpp` |
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/lua/lua-original.svg" width="32"/> | **Lua** | Plugins e extensões | `lua_runner.cpp` |
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" width="32"/> | **C** | Watchdog | `watchdog.c` |
| <img src="https://img.shields.io/badge/Forth-8B4513?style=flat-square" width="48"/> | **Forth** | Integridade dos logs | `guarda_logs.fs` |
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/python/python-original.svg" width="32"/> | **Python** | Testes automatizados | `testador_avancado.py` |

</div>

---

## ◈ Índice

| | | |
|:---:|:---:|:---:|
| [**01** Introdução](#01--introdução) | [**02** Filosofia](#02--filosofia) | [**03** História](#03--história) |
| [**04** Arquitetura](#04--arquitetura) | [**05** Linguagens](#05--linguagens) | [**06** Instalação](#06--instalação) |
| [**07** Uso](#07--uso) | [**08** Comandos](#08--comandos) | [**09** Anéis](#09--anéis) |
| [**10** Capacidades](#10--capacidades) | [**11** Lua](#11--lua) | [**12** Dead Hand](#12--dead-hand) |
| [**13** Segurança](#13--segurança) | [**14** Arquivos](#14--arquivos) | [**15** Inspirações](#15--inspirações) |
| [**16** Créditos](#16--créditos) | [**17** Roadmap](#17--roadmap) | [**18** Licença](#18--licença) |

---

## 01 · Introdução

**Trooppinen Valas** é uma linguagem de programação interpretada para **proteger servidores Linux** de pequeno e médio porte contra ataques cibernéticos.

### O que o servidor se torna

| | Recurso | Descrição |
|:---:|:---|:---|
| `▸` | **Alvo Móvel** | Fecha e embaralha portas automaticamente |
| `▸` | **Modo Fantasma** | Esconde processos do sistema |
| `▸` | **Aprendizado** | Cria novas regras sozinho |
| `▸` | **Colmeia P2P** | Compartilha defesas entre servidores |
| `▸` | **Watchdog** | Reinicia o motor automaticamente |

### Exemplo de uso

```bash
proteger porta 22;
ativar fantasma;
aprender ataques cada 30s;
```

---

## 02 · Filosofia

<table align="center">
<tr>
<td align="center" width="33%">

### `◆` Moldável

![argila](https://img.shields.io/badge/como-argila-6b4423?style=flat-square)

Qualquer pessoa pode modificar a linguagem.

**Sem precisar saber programar.**

</td>
<td align="center" width="33%">

### `◆` Segura

![padrão](https://img.shields.io/badge/por-padr%C3%A3o-1a4a2e?style=flat-square)

Comandos perigosos exigem autorização.

**Capacidade + Anel = cofre duplo.**

</td>
<td align="center" width="33%">

### `◆` Modular

![orquestra](https://img.shields.io/badge/como-orquestra-1a2a4a?style=flat-square)

Cada camada tem seu papel.

**Uma sinfonia de linguagens.**

</td>
</tr>
</table>

---

## 03 · História

<details open>
<summary><b>▸ Linha do tempo completa (17 versões)</b></summary>

| Versão | Data | Marco |
|:---:|:---:|:---|
| **v0.1** | Jul 2026 | Primeiro motor em C |
| **v0.2** | Jul 2026 | Listagem de portas |
| **v0.3** | Jul 2026 | Fechamento de portas |
| **v0.4** | Jul 2026 | Embaralhamento (MTD) |
| **v0.5** | Jul 2026 | Modo automático |
| **v0.6** | Jul 2026 | Múltiplas portas + logs |
| **v0.7** | Jul 2026 | Modo fantasma |
| **v0.8** | Jul 2026 | Modo daemon |
| **v0.9** | Jul 2026 | Daemon profissional |
| **v1.0** | Ago 2026 | Reescrita em C++ |
| **v1.1** | Ago 2026 | Suporte bilíngue |
| **v1.2** | Ago 2026 | Lista branca de comandos |
| **v1.3** | Ago 2026 | Anéis do Multics |
| **v1.4** | Ago 2026 | Capacidades do KeyKOS |
| **v1.5** | Ago 2026 | Jaula Reforçada |
| **v1.6** | Ago 2026 | Dead Hand + Manifesto |
| **v1.7** | Set 2026 | Lua nativo, testador, watchdog |

</details>

---

## 04 · Arquitetura

```text
                    TROOPPINEN VALAS
                           │
                    ┌──────▼──────┐
                    │  trooppinen │
                    │  (comando)  │
                    └──────┬──────┘
                           │
                ┌──────────▼──────────┐
                │    MOTOR PRINCIPAL  │
                │     nucleo.cpp      │
                └──┬────────┬────────┬┘
                   │        │        │
           ┌───────▼─┐  ┌───▼───┐  ┌─▼────────┐
           │   LUA   │  │ FORTH │  │ WATCHDOG │
           │ plugins │  │ logs  │  │ reinício │
           └─────────┘  └───────┘  └──────────┘
                            │
                    ┌───────▼────────┐
                    │ TESTADOR       │
                    │ (Python)       │
                    └────────────────┘
```

---

## 05 · Linguagens

<div align="center">

| | Linguagem | Papel |
|:---:|:---:|:---|
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cplusplus/cplusplus-original.svg" width="40"/> | **C++** | Motor principal |
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/lua/lua-original.svg" width="40"/> | **Lua** | Plugins e extensões |
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" width="40"/> | **C** | Watchdog |
| <img src="https://img.shields.io/badge/F-8B4513?style=for-the-badge" width="40"/> | **Forth** | Integridade dos logs |
| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/python/python-original.svg" width="40"/> | **Python** | Testes automatizados |

</div>

> **Nota:** nenhuma é decorativa. Se uma falhar, as outras continuam.

---

## 06 · Instalação

### Pré-requisitos

- Termux instalado (via F-Droid)
- ~500 MB livres
- Conexão com a internet

### Passo a passo

<details open>
<summary><b>▸ Comandos de instalação</b></summary>

**1. Atualize o Termux**

```bash
pkg update && pkg upgrade -y
```

**2. Instale as ferramentas**

```bash
pkg install clang gcc make python gforth lua git -y
```

**3. Entre na pasta do projeto**

```bash
cd /storage/emulated/0/Download/trooppinen
```

**4. Execute o instalador**

```bash
bash install_master.sh
```

**5. Verifique**

```bash
echo 'exibir "ola";' > ~/teste.tv
trooppinen ~/teste.tv
```

</details>

> Se aparecer `ola`, a instalação foi bem-sucedida.

---

## 07 · Uso

### Três passos

**1. Crie um script**

```bash
nano meu_script.tv
```

**2. Escreva os comandos**

```bash
criar chave para porta = "chave_mestra";
proteger porta 22;
ativar fantasma;
metamorfosear a cada 30s;
```

**3. Execute**

```bash
trooppinen meu_script.tv --anel 0
```

### Exemplo completo

```bash
# defesa_web.tv — Script para servidor web

criar chave para porta = "chave_mestra";

# Fecha portas críticas
proteger porta 22;
proteger porta 3306;
proteger porta 5432;

# Ativa ocultação
ativar fantasma;

# Aprende com ataques
aprender ataques cada 30s;

# Metamorfose contínua
metamorfosear a cada 60s;
```

### Localização dos arquivos

| Tipo | Onde fica |
|:---|:---|
| Scripts `.tv` | Qualquer lugar |
| Plugins `.lua` | Diretório atual |
| Logs | Pasta de execução |

---

## 08 · Comandos

### Básicos — Anel 4

| Comando | Descrição |
|:---|:---|
| `exibir "mensagem"` | Mostra uma mensagem |
| `mostrar portas` | Lista portas abertas |
| `ler log` | Exibe o histórico |
| `ajuda` | Lista comandos |
| `versao` | Mostra a versão |

### Defesa — Anel 3

| Comando | Descrição |
|:---|:---|
| `proteger porta <N>` | Fecha a porta |
| `embaralhar porta <N>` | Fecha e sugere nova porta |

### Controle — Anel 2

| Comando | Descrição |
|:---|:---|
| `ativar fantasma` | Oculta o processo |
| `desativar fantasma` | Revela o processo |
| `carregar "arquivo.tv"` | Executa outro script |

### Avançados — Anel 1

| Comando | Descrição |
|:---|:---|
| `metamorfosear a cada <N>s` | Re-embaralha periodicamente |
| `aprender ataques cada <N>s` | Ativa aprendizado |
| `executar script "arq.lua"` | Executa plugin Lua |

### Administrativos — Anel 0

| Comando | Descrição |
|:---|:---|
| `isolar "comando"` | Executa em jaula |
| `conectar a "id"` | Conecta à colmeia |
| `trocar defesas a cada <N>s` | Inicia colmeia P2P |
| `anel <0-4>` | Muda nível de privilégio |
| `criar chave para <X> = "Y"` | Cria capacidade |
| `validar script "arquivo.tv"` | Verifica comandos |
| `modo dead hand a cada <N>s` | Ativa Dead Hand |
| `sinal de vida` | Renova o sinal |

---

## 09 · Anéis

> Inspirado no **Multics** (1965-2000).

| Anel | Nome | Privilégio |
|:---:|:---:|:---|
| **0** | Administrador | Máximo poder |
| **1** | Operador | Comandos avançados |
| **2** | Confiável | Controle de processos |
| **3** | Usuário | Defesa básica |
| **4** | Observador | Apenas leitura (padrão) |

```bash
# Rodando com anel 4, isto é negado:
proteger porta 22;
→ Permissao negada: requer anel 3 ou superior.

# Após elevar para anel 0:
anel 0;
proteger porta 22;
→ Protegendo a porta 22...
```

> **Regra de ouro:** um script nunca pode se auto-promover.

---

## 10 · Capacidades

> Inspirado no **KeyKOS** (1975-1991).

Cada recurso só pode ser usado se uma chave for criada antes.

```bash
# 1. Cria a chave
criar chave para porta = "senha_super_secreta";

# 2. Agora pode usar
proteger porta 22;   ✓ funciona

# 3. Sem a chave, é negado
proteger porta 22;   ✗ acesso negado
```

> **Segurança dupla:** além do anel, você precisa da capacidade.

---

## 11 · Lua

### Exemplo de plugin

```lua
-- auto_defesa.lua
tv.log("Iniciando defesa automatica")
tv.criar_capacidade("porta", "chave_mestra")
tv.proteger(22)
tv.embaralhar(22)
tv.fantasma(true)

if tv.verificar_capacidade("porta") then
    print("Capacidade ativa")
end

print("Anel atual: " .. tv.obter_anel())
```

### Funções disponíveis

| Função | Descrição |
|:---|:---|
| `tv.proteger(porta)` | Protege uma porta |
| `tv.embaralhar(porta)` | Embaralha uma porta |
| `tv.fantasma(bool)` | Ativa/desativa fantasma |
| `tv.criar_capacidade(recurso, chave)` | Cria capacidade |
| `tv.log(mensagem)` | Escreve no log |
| `tv.versao()` | Retorna a versão |
| `tv.ler_log()` | Exibe o log |
| `tv.listar_portas()` | Lista portas |
| `tv.verificar_capacidade(recurso)` | Retorna `true`/`false` |
| `tv.obter_anel()` | Retorna o anel |

---

## 12 · Dead Hand

> Inspirado no sistema soviético de retaliação automática da Guerra Fria.

Quando ativado, o motor vigia um sinal de vida. Se o sinal não chegar, ele executa uma sequência de emergência.

```bash
modo dead hand a cada 60s;
sinal de vida;
```

Se ficar 60 segundos sem sinal:

```text
[DEAD HAND] Sinal de vida ausente! Executando emergencia...
[DEAD HAND] Porta 22 protegida, reembaralhada e fantasma ativado.
```

### Casos de uso

- Administrador desligado inesperadamente
- Servidor isolado em um ataque coordenado
- Processo principal capturado, mas Dead Hand sobrevive

---

## 13 · Segurança

Cinco camadas que se sobrepõem.

| # | Camada | Proteção |
|:---:|:---|:---|
| 1 | Lista branca | Nenhum comando arbitrário |
| 2 | Anéis (Multics) | Níveis de privilégio |
| 3 | Capacidades (KeyKOS) | Chaves por recurso |
| 4 | Jaula (Singularity) | Namespaces isolados |
| 5 | Dead Hand (Guerra Fria) | Sobrevive a quedas |

> Se um atacante passar por uma camada, ainda enfrenta quatro.

---

## 14 · Arquivos

### Essenciais

```text
trooppinen/
├── nucleo.cpp            ← Motor principal
├── lua_runner.cpp        ← Integração Lua
├── lua_runner.h          ← Cabeçalho Lua
├── lua_lib/              ← Biblioteca Lua
├── watchdog.c            ← Watchdog
├── guarda_logs.fs        ← Guardião Forth
├── install_master.sh     ← Instalador
├── idiomas.map           ← Dicionário
├── stdlib.tv             ← Biblioteca padrão
└── testador_avancado.py  ← Testador
```

### Podem ser apagados

```text
teste_*.tv
teste_*.lua
testes_auto/
```

### Gerados automaticamente

```text
defesa.log       ← Histórico (NÃO apagar)
nucleo_backup    ← Cópia de segurança
watchdog.pid     ← PID temporário
nodo_*.dtv       ← Pacotes da colmeia
```

---

## 15 · Inspirações

### As duas maiores influências

**Multics** (1965-2000) — Sistema de anéis de proteção  
**KeyKOS** (1975-1991) — Sistema de capacidades

### Fontes complementares

| Fonte | Ano | Contribuição |
|:---|:---:|:---|
| Lisp | 1958 | Homoiconicidade |
| Forth | 1970 | Simplicidade radical |
| TUNES | 1992-2011 | Metaprogramação |
| Singularity | 2003-2010 | Processos isolados |
| MTD | 2009 | Alvo móvel |
| EROS | 2005 | Design seguro |
| Guerra Fria | 1947-1991 | Dead Hand |
| Bulas de Argila | 5000 a.C. | Integridade |

---

## 16 · Créditos

<table align="center">
<tr>
<td align="center" width="33%">

### Criador

**Arthur**

13 anos · Brasil

*Criou a Trooppinen Valas do zero, em um celular.*

</td>
<td align="center" width="33%">

### Colaborador

**DeepSeek**

Inteligência Artificial

*Pesquisou, escreveu e organizou cada linha.*

</td>
<td align="center" width="33%">

### Empresa

**AL**

Fundada em 2026

*Tornar a segurança acessível a todos.*

</td>
</tr>
</table>

---

## 17 · Roadmap

### Concluído

- [x] Motor C++ estável
- [x] API Lua nativa
- [x] Watchdog inteligente
- [x] Guardião Forth
- [x] Testador Python
- [x] Instalador mestre

### Próximas versões

- [ ] Versão para VPS Linux
- [ ] Interface web
- [ ] Publicação no GitHub
- [ ] Documentação em vídeo
- [ ] Comunidade de usuários
- [ ] App Android nativo
- [ ] Suporte multilíngue

---

## 18 · Licença


# PolyForm Noncommercial License 1.0.0

<https://polyformproject.org/licenses/noncommercial/1.0.0>

## Acceptance

In order to get any license under these terms, you must agree
to them as both strict obligations and conditions to all your
licenses.

## Copyright License

The licensor grants you a copyright license for the software
to do everything you might do with the software that would
otherwise infringe the licensor's copyright in it for any
permitted purpose. However, you may only distribute the
software according to the Distribution License section below,
and make changes or new works based on the software according
to the Changes and New Works License section below.

## Distribution License

The licensor grants you an additional copyright license to
distribute copies of the software. Your license to distribute
covers distributing the software with changes and new works
permitted by the Changes and New Works License section below.

## Notices

You must ensure that anyone who gets a copy of any part of the
software from you also gets a copy of these terms or the URL
for them.

## Changes and New Works License

The licensor grants you an additional copyright license to
make changes and new works based on the software for any
permitted purpose.

## Patent License

The licensor grants you a patent license for the software
that covers patent claims the licensor can license, or becomes
able to license, that you would infringe by using the software.

## Noncommercial Purposes

Any noncommercial purpose is a permitted purpose.

## Personal Uses

Personal use for research, experiment, and testing for the
benefit of public knowledge, personal study, private
entertainment, hobby projects, amateur pursuits, or religious
observance, without any anticipated commercial application,
is use for a permitted purpose.

## Noncommercial Organizations

Use by any charitable organization, educational institution,
public research organization, public safety or health
organization, environmental protection organization, or
government institution is use for a permitted purpose
regardless of the source of funding or obligations resulting
from the funding.

## Fair Use

You may have "fair use" rights for the software under the law.
These terms do not limit them.

## No Other Rights

These terms do not allow you to sublicense or transfer any of
your licenses to anyone else, or prevent the licensor from
granting licenses to anyone else. These terms do not imply any
other licenses.

## Patent Defense

If you make any written claim that the software infringes or
contributes to infringement of any patent, your patent license
for the software granted under these terms ends immediately.
If your company makes such a claim, your patent license ends
immediately for work on behalf of your company.

## Violations

The first time you are notified in writing that you have
violated any of these terms, or done anything with the software
not covered by your licenses, your licenses can nonetheless
continue if you come into full compliance with these terms, and
take practical steps to correct past violations, within 32 days
of receiving notice. Otherwise, all your licenses end
immediately.

## No Liability

***As far as the law allows, the software comes as is, without
any warranty or condition, and the licensor will not be liable
to you for any damages arising out of these terms or the use
or nature of the software, under any kind of legal claim.***

## Definitions

The **licensor** is the individual or entity offering these
terms, and the **software** is the software the licensor makes
available under these terms.

**You** refers to the individual or entity agreeing to these
terms.

**Your company** is any legal entity, sole proprietorship, or
other kind of organization that you work for, plus all its
affiliates.

**Affiliates** means the other organizations that control,
are controlled by, or are under common control with that
organization.

**Control** means ownership of substantially all the assets of
an entity, or the power to direct its management and policies
by vote, contract, or otherwise. Control can be direct or
indirect.

**Your licenses** are all the licenses granted to you for the
software under these terms.

**Use** means anything you do with the software requiring one
of your licenses.

---

Copyright (c) 2026 Arthur / AL

```

TROOPPINEN VALAS

Versão 1.7 · 2026 · Brasil