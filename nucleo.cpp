#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <signal.h>
#include "lua_runner.h"

#ifndef CLONE_NEWNS
#define CLONE_NEWNS 0x00020000
#endif
#ifndef CLONE_NEWPID
#define CLONE_NEWPID 0x20000000
#endif

extern void executar_linha(const std::string &linha);

using namespace std;

// ---------- Variáveis globais ----------
bool modo_fantasma = false;
bool modo_daemon_ativo = false;
string meu_id = "servidor1";
string versao = "1.7 (2026)";
int anel_atual = 4;
map<string, string> capacidades;

vector<int> portas_protegidas;
set<string> nodos_conectados;

map<string, int> tentativas_por_ip;
map<string, string> regras_aprendidas;

int dead_hand_interval = 0;
time_t dead_hand_ultimo_sinal = 0;

// ---------- Log do sistema ----------
void escrever_log(const string &tipo, const string &detalhes) {
    ofstream log("defesa.log", ios::app);
    if (!log.is_open()) return;
    time_t agora = time(nullptr);
    struct tm *t = localtime(&agora);
    char buf[100];
    strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S]", t);
    log << buf << " [" << tipo << "] " << detalhes << endl;
    log.close();
}

// ---------- Função para remover aspas duplas ----------
string strip_quotes(const string &s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

// ---------- Função de segurança para comandos do sistema ----------
bool executar_comando_seguro(const string &comando, const string &argumentos) {
    static const set<string> comandos_permitidos = {
        "iptables", "nft", "netstat", "ss", "cat"
    };
    if (comandos_permitidos.find(comando) == comandos_permitidos.end()) {
        cout << "Comando bloqueado por seguranca: " << comando << endl;
        escrever_log("SEGURANCA", "Comando bloqueado: " + comando);
        return false;
    }
    string cmd = comando + " " + argumentos;
    return system(cmd.c_str()) == 0;
}

// ---------- Ferramentas de rede ----------
int gerar_porta_aleatoria() {
    return 1024 + rand() % (65535 - 1024 + 1);
}

void fechar_porta(int porta) {
    string args = "-A INPUT -p tcp --dport " + to_string(porta) + " -j DROP 2>/dev/null";
    if (!executar_comando_seguro("iptables", args)) {
        args = "add rule ip filter INPUT tcp dport " + to_string(porta) + " drop 2>/dev/null";
        if (!executar_comando_seguro("nft", args)) {
            cout << "(Simulacao: iptables/nftables ausentes)" << endl;
        }
    }
}

void embaralhar_porta(int porta) {
    fechar_porta(porta);
    int nova = gerar_porta_aleatoria();
    cout << "Embaralhando porta " << porta << " -> nova porta sugerida " << nova << endl;
    escrever_log("EMBARALHAR", "Porta " + to_string(porta) + " -> " + to_string(nova));
}

void mostrar_portas() {
    if (modo_fantasma) cout << "Modo fantasma ativo: ocultando portas reais." << endl;
    cout << "\nPortas abertas no servidor:" << endl;
    if (!executar_comando_seguro("netstat", "-tlnp 2>/dev/null")) {
        executar_comando_seguro("ss", "-tlnp 2>/dev/null");
    }
    cout << endl;
}

// ---------- Motor de tradução ----------
map<string, string> carregar_traducoes() {
    map<string, string> traducoes;
    ifstream arquivo("idiomas.map");
    if (!arquivo.is_open()) return traducoes;
    string linha;
    while (getline(arquivo, linha)) {
        if (linha.empty() || linha[0] == '#') continue;
        size_t pos = linha.find('=');
        if (pos != string::npos) {
            string chave = linha.substr(0, pos);
            string valor = linha.substr(pos + 1);
            traducoes[chave] = valor;
        }
    }
    arquivo.close();
    return traducoes;
}

string traduzir_linha(const string &linha) {
    static map<string, string> traducoes = carregar_traducoes();
    string resultado = linha;
    for (const auto &par : traducoes) {
        size_t pos = resultado.find(par.first);
        if (pos == 0) {
            resultado.replace(0, par.first.length(), par.second);
            break;
        }
    }
    return resultado;
}

// ---------- Funções auxiliares dos comandos ----------
void executar_linha(const string &linha);

bool validar_linha(const string &linha) {
    string linha_traduzida = traduzir_linha(linha);
    if (linha_traduzida.empty() || linha_traduzida[0] == '\n' || linha_traduzida[0] == '/') return true;
    string cmd = linha_traduzida;
    if (cmd.back() == ';') cmd.pop_back();
    istringstream iss(cmd);
    string palavra; vector<string> tokens;
    while (iss >> palavra) tokens.push_back(palavra);
    if (tokens.empty()) return true;

    static const set<string> comandos_validos = {
        "exibir", "mostrar", "proteger", "embaralhar", "ativar", "desativar",
        "ler", "aprender", "metamorfosear", "isolar", "conectar", "trocar",
        "carregar", "executar", "anel", "criar", "ajuda", "versao", "sair",
        "parar", "modo", "sinal", "validar"
    };
    if (comandos_validos.find(tokens[0]) == comandos_validos.end()) {
        cout << "Comando invalido: " << tokens[0] << endl;
        return false;
    }
    return true;
}

void executar_sequencia_emergencia() {
    cout << "\n[DEAD HAND] Sinal de vida ausente! Executando sequencia de emergencia..." << endl;
    escrever_log("DEAD_HAND", "Sequencia de emergencia ativada");
    fechar_porta(22);
    embaralhar_porta(22);
    modo_fantasma = true;
    cout << "[DEAD HAND] Porta 22 protegida, reembaralhada e modo fantasma ativado." << endl;
    escrever_log("DEAD_HAND", "Emergencia concluida");
}

void monitorar_log(const string &arquivo, const string &padrao, int limite, const string &reacao) {
    cout << "\n=== MONITORANDO " << arquivo << " ===" << endl;
    cout << "Procurando: \"" << padrao << "\" (limite: " << limite << " tentativas)" << endl;
    cout << "Reacao: " << reacao << endl;
    cout << "(Pressione Ctrl+C para interromper)" << endl << endl;
    escrever_log("MONITORAR", "Iniciado");

    ifstream f(arquivo);
    if (!f.is_open()) {
        cout << "Erro ao abrir o arquivo '" << arquivo << "'." << endl;
        return;
    }
    f.seekg(0, ios::end);
    streampos posicao_atual = f.tellg();
    f.close();

    int contador = 0;
    while (true) {
        f.open(arquivo);
        if (f.is_open()) {
            f.seekg(posicao_atual);
            string linha_log;
            while (getline(f, linha_log)) {
                if (linha_log.find(padrao) != string::npos) {
                    contador++;
                    cout << "[TENTATIVA " << contador << "/" << limite << "] Detectado: " << linha_log << endl;
                    escrever_log("TENTATIVA", "Tentativa " + to_string(contador) + "/" + to_string(limite) + ": " + linha_log);
                    if (contador >= limite) {
                        cout << "[ALERTA] Limite de " << limite << " tentativas atingido! Reagindo..." << endl;
                        escrever_log("ALERTA", "Limite atingido");
                        executar_linha(reacao);
                        contador = 0;
                    }
                }
            }
            posicao_atual = f.tellg();
            f.close();
        }
        sleep(2);
    }
}

void metamorfosear() {
    if (portas_protegidas.empty()) {
        cout << "[METAMORFOSE] Nenhuma porta protegida para embaralhar." << endl;
        return;
    }
    cout << "\n--- Metamorfose: embaralhando todas as portas protegidas ---" << endl;
    for (int porta : portas_protegidas) {
        embaralhar_porta(porta);
    }
    escrever_log("METAMORFOSE", "Ciclo concluído");
}

void iniciar_metamorfose(int intervalo) {
    cout << "\n=== METAMORFOSE ATIVADA ===" << endl;
    cout << "Embaralhando todas as portas protegidas a cada " << intervalo << " segundos." << endl;
    cout << "(Pressione Ctrl+C para interromper)" << endl << endl;
    escrever_log("METAMORFOSE", "Iniciada (intervalo " + to_string(intervalo) + "s)");
    while (true) {
        metamorfosear();
        cout << "[METAMORFOSE] Aguardando " << intervalo << " segundos...\n" << endl;
        sleep(intervalo);
    }
}

void isolar_programa(const string &comando, int timeout = 0) {
    cout << "[ISOLAR] Executando comando em jaula: " << comando << endl;
    escrever_log("ISOLAR", comando);

    pid_t pid = fork();
    if (pid == -1) {
        cout << "Erro ao criar processo isolado." << endl;
        return;
    }

    if (pid == 0) {
        if (unshare(CLONE_NEWNS | CLONE_NEWPID) == 0) {
            mount("none", "/proc", "proc", 0, NULL);
            cout << "[JAULA] Isolamento de namespaces ativado." << endl;
        } else {
            cout << "[JAULA] Namespaces nao suportados. Isolamento limitado ao fork()." << endl;
        }
        execl("/bin/sh", "sh", "-c", comando.c_str(), (char*)nullptr);
        _exit(1);
    } else {
        if (timeout > 0) {
            int status;
            pid_t result;
            time_t inicio = time(nullptr);
            while (true) {
                result = waitpid(pid, &status, WNOHANG);
                if (result == pid) {
                    if (WIFEXITED(status))
                        cout << "[ISOLAR] Comando concluido com codigo " << WEXITSTATUS(status) << "." << endl;
                    else
                        cout << "[ISOLAR] Comando finalizado de forma anormal." << endl;
                    escrever_log("ISOLAR", "Concluido");
                    return;
                }
                if (time(nullptr) - inicio >= timeout) {
                    kill(pid, SIGKILL);
                    waitpid(pid, &status, 0);
                    cout << "[ISOLAR] Tempo limite (" << timeout << "s) atingido. Comando encerrado." << endl;
                    escrever_log("ISOLAR", "Timeout (" + to_string(timeout) + "s)");
                    return;
                }
                sleep(1);
            }
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                cout << "[ISOLAR] Comando concluido com codigo " << WEXITSTATUS(status) << "." << endl;
            else
                cout << "[ISOLAR] Comando finalizado de forma anormal." << endl;
            escrever_log("ISOLAR", "Concluido");
        }
    }
}

void iniciar_colmeia(int intervalo) {
    cout << "\n=== COLMEIA P2P ATIVADA ===" << endl;
    cout << "Nodos conectados: ";
    for (const string &n : nodos_conectados) cout << n << " ";
    cout << endl;
    cout << "Intervalo de troca: " << intervalo << " segundos." << endl;
    cout << "(Pressione Ctrl+C para interromper)" << endl << endl;
    escrever_log("COLMEIA", "Iniciada (intervalo " + to_string(intervalo) + "s)");
    while (true) {
        string nome_arquivo = "nodo_" + meu_id + "_defesas.dtv";
        ofstream pack(nome_arquivo);
        if (pack.is_open()) {
            pack << "# Defesas do nodo " << meu_id << endl;
            if (modo_fantasma) pack << "ativar fantasma;" << endl;
            for (int p : portas_protegidas) pack << "proteger porta " << p << ";" << endl;
            for (const auto &regra : regras_aprendidas) pack << regra.second << ";" << endl;
            pack.close();
        }
        for (const string &nodo : nodos_conectados) {
            string arquivo_remoto = "nodo_" + nodo + "_defesas.dtv";
            ifstream pacote(arquivo_remoto);
            if (pacote.is_open()) {
                string linha;
                while (getline(pacote, linha)) {
                    if (!linha.empty() && linha[0] != '#') executar_linha(linha);
                }
                pacote.close();
                cout << "[COLMEIA] Defesas importadas de " << nodo << endl;
                escrever_log("COLMEIA", "Importadas defesas de " + nodo);
            }
        }
        cout << "[COLMEIA] Aguardando " << intervalo << " segundos...\n" << endl;
        sleep(intervalo);
    }
}

void analisar_e_aprender() {
    ifstream log_sistema("system.log");
    if (!log_sistema.is_open()) return;
    string linha;
    while (getline(log_sistema, linha)) {
        size_t pos_fail = linha.find("Failed password");
        if (pos_fail != string::npos) {
            size_t pos_from = linha.find("from ", pos_fail);
            if (pos_from != string::npos) {
                size_t inicio_ip = pos_from + 5;
                size_t fim_ip = linha.find_first_of(" \t\n", inicio_ip);
                string ip = linha.substr(inicio_ip, fim_ip - inicio_ip);
                if (!ip.empty()) {
                    tentativas_por_ip[ip]++;
                    cout << "[APRENDIZADO] IP " << ip << " com " << tentativas_por_ip[ip] << " falhas." << endl;
                    if (tentativas_por_ip[ip] >= 5) {
                        string regra = "proteger porta 22";
                        if (regras_aprendidas.find(ip) == regras_aprendidas.end()) {
                            regras_aprendidas[ip] = regra;
                            cout << "[APRENDIZADO] Nova regra criada: " << regra << endl;
                            escrever_log("APRENDIZADO", "Regra criada para IP " + ip + ": " + regra);
                        }
                        executar_linha(regra + ";");
                        tentativas_por_ip[ip] = 0;
                    }
                }
            }
        }
    }
    log_sistema.close();
}

void iniciar_aprendizado(int intervalo) {
    cout << "\n=== MODO APRENDIZADO ATIVADO ===" << endl;
    cout << "Analisando system.log a cada " << intervalo << " segundos." << endl;
    cout << "(Pressione Ctrl+C para interromper)" << endl << endl;
    escrever_log("APRENDIZADO", "Iniciado (intervalo " + to_string(intervalo) + "s)");
    while (true) {
        analisar_e_aprender();
        cout << "[APRENDIZADO] Aguardando " << intervalo << " segundos...\n" << endl;
        sleep(intervalo);
    }
}

void mostrar_ajuda() {
    cout << "\n=== COMANDOS DISPONIVEIS ===" << endl;
    cout << "exibir \"<mensagem>\"  – mostra uma mensagem na tela" << endl;
    cout << "mostrar portas        – lista as portas abertas" << endl;
    cout << "proteger porta <N>    – fecha a porta (Requer anel 3+ e capacidade 'porta')" << endl;
    cout << "embaralhar porta <N>  – fecha e sugere nova porta secreta (Requer anel 3+ e capacidade 'porta')" << endl;
    cout << "ativar fantasma       – oculta o processo (Requer anel 2+)" << endl;
    cout << "desativar fantasma    – revela o processo (Requer anel 2+)" << endl;
    cout << "ler log               – exibe o historico de defesas" << endl;
    cout << "aprender ataques cada <N>s  – ativa modo de aprendizado automatico (Requer anel 1+)" << endl;
    cout << "metamorfosear a cada <N>s – reembaralha todas as portas periodicamente (Requer anel 1+ e capacidade 'porta')" << endl;
    cout << "isolar \"<comando>\" [limite <N>s] – executa programa em jaula (Requer anel 0)" << endl;
    cout << "conectar a \"<id>\"    – adiciona nodo à colmeia (Requer anel 0)" << endl;
    cout << "trocar defesas a cada <N>s – inicia troca automática com a colmeia (Requer anel 0)" << endl;
    cout << "carregar \"<arquivo.tv>\" – executa outro script .tv (Requer anel 2+)" << endl;
    cout << "executar script \"<script.lua>\" – executa um plugin Lua (Requer anel 1+)" << endl;
    cout << "anel <0-4>            – muda o nivel de privilegio (Requer anel 0)" << endl;
    cout << "criar chave para <recurso> = \"<chave>\" – cria uma capacidade de acesso (Requer anel 0)" << endl;
    cout << "validar script \"<arquivo.tv>\" – verifica se os comandos do script sao validos (Requer anel 0)" << endl;
    cout << "modo dead hand a cada <N>s – ativa o protocolo de emergencia Dead Hand (Requer anel 0)" << endl;
    cout << "sinal de vida         – envia um sinal de vida para o Dead Hand (Requer anel 0)" << endl;
    cout << "ajuda                 – mostra esta lista" << endl;
    cout << "versao                – exibe a versao da linguagem" << endl;
    cout << "sair / parar          – encerra o script" << endl;
    cout << "\nComandos em ingles tambem sao aceitos (ex: protect port 22, show ports, activate ghost...)" << endl;
    cout << endl;
}

// ---------- Processamento de linhas de comando ----------
void executar_linha(const string &linha) {
    string linha_traduzida = traduzir_linha(linha);
    if (linha_traduzida.empty() || linha_traduzida[0] == '\n' || linha_traduzida[0] == '/') return;
    string cmd = linha_traduzida;
    if (cmd.back() == ';') cmd.pop_back();
    istringstream iss(cmd);
    string palavra; vector<string> tokens;
    while (iss >> palavra) tokens.push_back(palavra);
    if (tokens.empty()) return;

    // BÁSICOS (Anel 4)
    if (tokens[0] == "exibir") {
        size_t inicio = linha_traduzida.find('"'), fim = linha_traduzida.rfind('"');
        if (inicio != string::npos && fim != string::npos && inicio != fim)
            cout << linha_traduzida.substr(inicio + 1, fim - inicio - 1) << endl;
        else cout << "Erro: mensagem nao finalizada com aspas." << endl;
    }
    else if (tokens[0] == "mostrar" && tokens.size() >= 2 && tokens[1] == "portas") { mostrar_portas(); }
    else if (tokens[0] == "ler" && tokens.size() >= 2 && tokens[1] == "log") {
        cout << "\n=== HISTORICO DE DEFESA (defesa.log) ===" << endl;
        executar_comando_seguro("cat", "defesa.log 2>/dev/null || echo '(log vazio)'");
        cout << endl;
    }
    else if (tokens[0] == "ajuda") { mostrar_ajuda(); }
    else if (tokens[0] == "versao") {
        cout << "Trooppinen Valas v" + versao << endl;
        cout << "Linguagem interpretada para defesa de servidores Linux." << endl;
        cout << "Criada por Arthur & pela DeepSeek." << endl;
    }
    // DEFESA ESSENCIAL (Anel 3 + Capacidade)
    else if (tokens[0] == "proteger" && tokens.size() >= 3 && tokens[1] == "porta") {
        if (anel_atual > 3) { cout << "Permissao negada: requer anel 3 ou superior." << endl; return; }
        if (capacidades.find("porta") == capacidades.end()) {
            cout << "Acesso negado: nenhuma capacidade 'porta' criada. Use 'criar chave para porta'." << endl;
            return;
        }
        int porta = stoi(tokens[2]);
        if (porta > 0 && porta < 65536) {
            cout << "Protegendo a porta " << porta << "..." << endl;
            fechar_porta(porta);
            escrever_log("PROTEGER", "Porta " + to_string(porta) + " protegida");
            if (find(portas_protegidas.begin(), portas_protegidas.end(), porta) == portas_protegidas.end())
                portas_protegidas.push_back(porta);
        } else cout << "Erro: porta invalida '" << porta << "'." << endl;
    }
    else if (tokens[0] == "embaralhar" && tokens.size() >= 3 && tokens[1] == "porta") {
        if (anel_atual > 3) { cout << "Permissao negada: requer anel 3 ou superior." << endl; return; }
        if (capacidades.find("porta") == capacidades.end()) {
            cout << "Acesso negado: nenhuma capacidade 'porta' criada." << endl;
            return;
        }
        int porta = stoi(tokens[2]);
        if (porta > 0 && porta < 65536) embaralhar_porta(porta);
        else cout << "Erro: porta invalida." << endl;
    }
    // CONTROLE (Anel 2)
    else if (tokens[0] == "ativar" && tokens.size() >= 2 && tokens[1] == "fantasma") {
        if (anel_atual > 2) { cout << "Permissao negada: requer anel 2 ou superior." << endl; return; }
        modo_fantasma = true; cout << "Modo fantasma ATIVADO." << endl; escrever_log("FANTASMA", "Ativado");
    }
    else if (tokens[0] == "desativar" && tokens.size() >= 2 && tokens[1] == "fantasma") {
        if (anel_atual > 2) { cout << "Permissao negada: requer anel 2 ou superior." << endl; return; }
        modo_fantasma = false; cout << "Modo fantasma DESATIVADO." << endl; escrever_log("FANTASMA", "Desativado");
    }
    else if (tokens[0] == "carregar" && tokens.size() >= 2) {
        if (anel_atual > 2) { cout << "Permissao negada: requer anel 2 ou superior." << endl; return; }
        size_t inicio = linha_traduzida.find('"'), fim = linha_traduzida.rfind('"');
        if (inicio != string::npos && fim != string::npos && inicio != fim) {
            string nome_script = linha_traduzida.substr(inicio + 1, fim - inicio - 1);
            cout << "Carregando " << nome_script << "..." << endl;
            ifstream sub(nome_script);
            if (sub.is_open()) {
                string sub_linha;
                while (getline(sub, sub_linha)) {
                    if (!sub_linha.empty() && sub_linha.back() == '\r') sub_linha.pop_back();
                    executar_linha(sub_linha);
                }
                sub.close();
            } else cout << "Erro: arquivo '" << nome_script << "' nao encontrado." << endl;
        }
    }
    // DEFESA AVANÇADA (Anel 1 + Capacidade)
    else if (tokens[0] == "metamorfosear" && tokens.size() >= 4 && tokens[1] == "a" && tokens[2] == "cada") {
        if (anel_atual > 1) { cout << "Permissao negada: requer anel 1 ou superior." << endl; return; }
        if (capacidades.find("porta") == capacidades.end()) {
            cout << "Acesso negado: nenhuma capacidade 'porta' criada." << endl;
            return;
        }
        string s = tokens[3]; if (s.back() == 's') s.pop_back();
        int intervalo = stoi(s);
        if (intervalo > 0) iniciar_metamorfose(intervalo);
        else cout << "Erro: intervalo invalido." << endl;
    }
    else if (tokens[0] == "aprender" && tokens.size() >= 2 && tokens[1] == "ataques") {
        if (anel_atual > 1) { cout << "Permissao negada: requer anel 1 ou superior." << endl; return; }
        int intervalo = 10;
        if (tokens.size() >= 4 && tokens[2] == "cada") {
            string s = tokens[3]; if (s.back() == 's') s.pop_back();
            try { intervalo = stoi(s); } catch (...) {}
        }
        iniciar_aprendizado(intervalo);
    }
    else if (tokens[0] == "executar" && tokens.size() >= 3 && tokens[1] == "script") {
        if (anel_atual > 1) { cout << "Permissao negada: requer anel 1 ou superior." << endl; return; }
        size_t inicio = linha_traduzida.find('"'), fim = linha_traduzida.rfind('"');
        if (inicio != string::npos && fim != string::npos && inicio != fim) {
            string nome_lua = linha_traduzida.substr(inicio + 1, fim - inicio - 1);
            cout << "Executando script Lua: " << nome_lua << "..." << endl;
            escrever_log("LUA", "Executando " + nome_lua);
            if (!executar_lua(nome_lua.c_str())) cout << "Erro ao executar o script Lua." << endl;
        } else cout << "Erro: formato invalido. Use: executar script \"<script.lua>\"" << endl;
    }
    // ADMINISTRAÇÃO (Anel 0)
    else if (tokens[0] == "isolar") {
        if (anel_atual > 0) { cout << "Permissao negada: requer anel 0." << endl; return; }
        size_t inicio_cmd = linha_traduzida.find('"'), fim_cmd = linha_traduzida.rfind('"');
        if (inicio_cmd == string::npos || fim_cmd == string::npos || inicio_cmd == fim_cmd) {
            cout << "Erro: comando deve estar entre aspas." << endl; return;
        }
        string comando = linha_traduzida.substr(inicio_cmd + 1, fim_cmd - inicio_cmd - 1);
        int timeout = 0;
        size_t pos_limite = linha_traduzida.find("limite", fim_cmd);
        if (pos_limite != string::npos) {
            string resto = linha_traduzida.substr(pos_limite + 6);
            try { timeout = stoi(resto); } catch (...) {}
        }
        isolar_programa(comando, timeout);
    }
    else if (tokens[0] == "conectar" && tokens.size() >= 3 && tokens[1] == "a") {
        if (anel_atual > 0) { cout << "Permissao negada: requer anel 0." << endl; return; }
        size_t inicio = linha_traduzida.find('"'), fim = linha_traduzida.rfind('"');
        if (inicio != string::npos && fim != string::npos && inicio != fim) {
            nodos_conectados.insert(linha_traduzida.substr(inicio + 1, fim - inicio - 1));
            cout << "Conectado ao nodo." << endl;
        }
    }
    else if (tokens[0] == "trocar" && tokens.size() >= 5 && tokens[1] == "defesas" && tokens[2] == "a" && tokens[3] == "cada") {
        if (anel_atual > 0) { cout << "Permissao negada: requer anel 0." << endl; return; }
        string s = tokens[4]; if (s.back() == 's') s.pop_back();
        int intervalo = stoi(s);
        if (intervalo > 0) iniciar_colmeia(intervalo);
    }
    else if (tokens[0] == "anel" && tokens.size() >= 2) {
        if (anel_atual > 0) { cout << "Permissao negada: requer anel 0." << endl; return; }
        int novo = stoi(tokens[1]);
        if (novo >= 0 && novo <= 4) { anel_atual = novo; cout << "Anel alterado para " << anel_atual << "." << endl; }
        else cout << "Erro: anel invalido. Use 0-4." << endl;
    }
    // CRIAÇÃO DE CAPACIDADES (Anel 0)
    else if (tokens[0] == "criar" && tokens.size() >= 4 && tokens[1] == "chave" && tokens[2] == "para") {
        if (anel_atual > 0) { cout << "Permissao negada: requer anel 0." << endl; return; }
        string recurso = tokens[3];
        string chave = "chave_padrao";
        if (tokens.size() >= 6 && tokens[4] == "=") {
            chave = strip_quotes(tokens[5]);
        }
        capacidades[recurso] = chave;
        cout << "Capacidade criada para " << recurso << " com chave \"" << chave << "\"." << endl;
        escrever_log("CAPACIDADE", "Criada para " + recurso);
    }
    // VALIDAR SCRIPT (Anel 0)
    else if (tokens[0] == "validar" && tokens.size() >= 3 && tokens[1] == "script") {
        if (anel_atual > 0) { cout << "Permissao negada: requer anel 0." << endl; return; }
        size_t inicio = linha_traduzida.find('"'), fim = linha_traduzida.rfind('"');
        if (inicio != string::npos && fim != string::npos && inicio != fim) {
            string nome_script = linha_traduzida.substr(inicio + 1, fim - inicio - 1);
            cout << "Validando " << nome_script << "..." << endl;
            ifstream sub(nome_script);
            if (!sub.is_open()) { cout << "Erro: arquivo '" << nome_script << "' nao encontrado." << endl; return; }
            string sub_linha;
            bool script_valido = true;
            while (getline(sub, sub_linha)) {
                if (!sub_linha.empty() && sub_linha.back() == '\r') sub_linha.pop_back();
                if (!validar_linha(sub_linha)) script_valido = false;
            }
            sub.close();
            if (script_valido) cout << "Script valido. Nenhum comando desconhecido encontrado." << endl;
            else cout << "Script contem erros. Corrija antes de executar." << endl;
            escrever_log("VALIDACAO", script_valido ? "Script valido" : "Script invalido");
        } else cout << "Erro: formato invalido. Use: validar script \"<arquivo.tv>\"" << endl;
    }
    // MODO DEAD HAND (Anel 0)
    else if (tokens[0] == "modo" && tokens.size() >= 5 && tokens[1] == "dead" && tokens[2] == "hand" && tokens[3] == "a" && tokens[4] == "cada") {
        if (anel_atual > 0) { cout << "Permissao negada: requer anel 0." << endl; return; }
        string s = tokens[5];
        if (s.back() == 's') s.pop_back();
        int intervalo = stoi(s);
        if (intervalo <= 0) { cout << "Erro: intervalo deve ser maior que zero." << endl; return; }
        dead_hand_interval = intervalo;
        dead_hand_ultimo_sinal = time(nullptr);
        cout << "Modo Dead Hand ativado: emergencia se ficar " << intervalo << "s sem sinal de vida." << endl;
        escrever_log("DEAD_HAND", "Ativado com intervalo de " + to_string(intervalo) + "s");
    }
    else if (tokens[0] == "sinal" && tokens.size() >= 2 && tokens[1] == "de" && tokens[2] == "vida") {
        if (anel_atual > 0) { cout << "Permissao negada: requer anel 0." << endl; return; }
        dead_hand_ultimo_sinal = time(nullptr);
        cout << "Sinal de vida recebido. Temporizador resetado." << endl;
        escrever_log("DEAD_HAND", "Sinal de vida recebido");
    }
    else {
        cout << "Comando desconhecido: " << linha_traduzida << endl;
    }
}

// ---------- Modo daemon ----------
void iniciar_daemon() {
    modo_daemon_ativo = true;
    pid_t pid = fork(); if (pid < 0) exit(1); if (pid > 0) exit(0);
    setsid(); pid = fork(); if (pid < 0) exit(1); if (pid > 0) exit(0);
    umask(0); fclose(stdin); fclose(stdout); fclose(stderr);
}

void sinal_vida_handler(int sig) {
    dead_hand_ultimo_sinal = time(nullptr);
}

// ---------- Funções exportadas para o Lua ----------
extern "C" {

void tv_proteger(int porta) {
    if (anel_atual > 3) { cout << "Permissao negada: requer anel 3 ou superior." << endl; return; }
    if (capacidades.find("porta") == capacidades.end()) {
        cout << "Acesso negado: nenhuma capacidade 'porta' criada." << endl; return;
    }
    fechar_porta(porta);
    if (find(portas_protegidas.begin(), portas_protegidas.end(), porta) == portas_protegidas.end())
        portas_protegidas.push_back(porta);
    cout << "Protegendo a porta " << porta << "..." << endl;
    escrever_log("PROTEGER", "Porta " + to_string(porta) + " protegida via Lua");
}

void tv_embaralhar(int porta) {
    if (anel_atual > 3) { cout << "Permissao negada: requer anel 3 ou superior." << endl; return; }
    if (capacidades.find("porta") == capacidades.end()) {
        cout << "Acesso negado: nenhuma capacidade 'porta' criada." << endl; return;
    }
    embaralhar_porta(porta);
}

void tv_fantasma(bool ativar) {
    if (anel_atual > 2) { cout << "Permissao negada: requer anel 2 ou superior." << endl; return; }
    modo_fantasma = ativar;
    cout << "Modo fantasma " << (ativar ? "ATIVADO" : "DESATIVADO") << "." << endl;
    escrever_log("FANTASMA", ativar ? "Ativado via Lua" : "Desativado via Lua");
}

void tv_criar_capacidade(const char* recurso, const char* chave) {
    if (anel_atual > 0) { cout << "Permissao negada: requer anel 0." << endl; return; }
    capacidades[string(recurso)] = string(chave);
    cout << "Capacidade criada para " << recurso << " com chave \"" << chave << "\"." << endl;
    escrever_log("CAPACIDADE", "Criada para " + string(recurso) + " via Lua");
}

void tv_log(const char* mensagem) {
    escrever_log("LUA", mensagem);
    cout << "[LUA] " << mensagem << endl;
}

const char* tv_versao() {
    static string versao_str = versao;
    return versao_str.c_str();
}

void tv_ler_log() {
    cout << "\n=== HISTORICO DE DEFESA (defesa.log) ===" << endl;
    executar_comando_seguro("cat", "defesa.log 2>/dev/null || echo '(log vazio)'");
    cout << endl;
}

void tv_listar_portas() {
    mostrar_portas();
}

bool tv_verificar_capacidade(const char* recurso) {
    return capacidades.find(string(recurso)) != capacidades.end();
}

int tv_obter_anel() {
    return anel_atual;
}

} // fim extern "C"

// ---------- Ponto de entrada ----------
int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--daemon") iniciar_daemon();
        else if (arg == "--id" && i + 1 < argc) meu_id = argv[++i];
        else if (arg == "--anel" && i + 1 < argc) anel_atual = stoi(argv[++i]);
    }
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <arquivo.tv> [--daemon] [--id <nome>] [--anel <0-4>]" << endl;
        return 1;
    }
    srand(time(nullptr));
    escrever_log("SISTEMA", "Defesa iniciada (ID: " + meu_id + ")");
    ifstream script(argv[1]);
    if (!script.is_open()) { cout << "Erro ao abrir o arquivo '" << argv[1] << "'." << endl; return 1; }
    string linha;
    while (getline(script, linha)) {
        if (!linha.empty() && linha.back() == '\r') linha.pop_back();
        stringstream ss(linha);
        string comando;
        while (getline(ss, comando, ';')) {
            size_t inicio = comando.find_first_not_of(" \t");
            size_t fim = comando.find_last_not_of(" \t");
            if (inicio != string::npos && fim != string::npos) {
                executar_linha(comando.substr(inicio, fim - inicio + 1));
            }
        }
    }
    script.close();
    escrever_log("SISTEMA", "Defesa concluida");

    // Se Dead Hand estiver ativo, entra em modo de monitoramento contínuo
    if (dead_hand_interval > 0) {
        cout << "\n=== DEAD HAND EM VIGILIA ===" << endl;
        cout << "Aguardando sinal de vida a cada " << dead_hand_interval << " segundos." << endl;
        signal(SIGUSR1, sinal_vida_handler);
        while (true) {
            if (time(nullptr) - dead_hand_ultimo_sinal > dead_hand_interval) {
                executar_sequencia_emergencia();
                dead_hand_ultimo_sinal = time(nullptr);
            }
            sleep(1);
        }
    }

    // Se for daemon (e sem Dead Hand), fica vivo em espera
    if (modo_daemon_ativo) {
        while (true) {
            sleep(3600);
        }
    }

    return 0;
}