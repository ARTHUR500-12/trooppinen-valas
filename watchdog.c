#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#define MOTOR_PATH   "./trooppinen_nucleo"
#define BACKUP_PATH  "./nucleo_backup"
#define SCRIPT_PATH  "defesa.tv"
#define PIDFILE      "./watchdog.pid"
#define INTERVALO    5
#define MAX_FALHAS   3

int falhas_consecutivas = 0;

// Verifica se o processo do motor existe
int motor_vivo() {
    return system("pgrep -f trooppinen_nucleo > /dev/null 2>&1") == 0;
}

// Verifica se o motor responde (envia sinal 0 – teste de existência)
int motor_responde() {
    FILE *fp = popen("pgrep -f trooppinen_nucleo", "r");
    if (!fp) return 0;
    char buf[32];
    int pid = 0;
    if (fgets(buf, sizeof(buf), fp)) pid = atoi(buf);
    pclose(fp);
    if (pid <= 0) return 0;
    return kill(pid, 0) == 0;
}

// Restaura o executável a partir do backup
void restaurar_backup() {
    struct stat st;
    if (stat(MOTOR_PATH, &st) != 0) {
        printf("[WATCHDOG] Motor sumiu. Restaurando backup...\n");
        if (stat(BACKUP_PATH, &st) == 0) {
            char cmd[256];
            snprintf(cmd, sizeof(cmd), "cp %s %s && chmod +x %s", BACKUP_PATH, MOTOR_PATH, MOTOR_PATH);
            system(cmd);
            printf("[WATCHDOG] Backup restaurado.\n");
        } else {
            printf("[WATCHDOG] ERRO: backup nao encontrado!\n");
        }
    }
}

// Inicia o motor em segundo plano
void iniciar_motor() {
    printf("[WATCHDOG] Iniciando motor...\n");
    pid_t pid = fork();
    if (pid == 0) {
        // Filho executa o motor com script de defesa padrão
        execl(MOTOR_PATH, MOTOR_PATH, SCRIPT_PATH, "--daemon", NULL);
        perror("execl");
        exit(1);
    }
}

// Verificação principal
void verificar_motor() {
    restaurar_backup();

    if (!motor_vivo()) {
        printf("[WATCHDOG] Motor nao esta rodando. Reiniciando...\n");
        iniciar_motor();
        falhas_consecutivas = 0;
        sleep(2);
        return;
    }

    if (!motor_responde()) {
        falhas_consecutivas++;
        printf("[WATCHDOG] Motor nao responde (falha %d/%d).\n", falhas_consecutivas, MAX_FALHAS);
        if (falhas_consecutivas >= MAX_FALHAS) {
            printf("[WATCHDOG] Motor travado. Matando e reiniciando...\n");
            system("pkill -9 -f trooppinen_nucleo");
            sleep(2);
            iniciar_motor();
            falhas_consecutivas = 0;
        }
    } else {
        falhas_consecutivas = 0;
    }
}

// Handler para encerramento limpo
void encerrar(int sig) {
    printf("\n[WATCHDOG] Encerrando vigilancia.\n");
    remove(PIDFILE);
    exit(0);
}

int main() {
    signal(SIGINT, encerrar);
    signal(SIGTERM, encerrar);

    // Salva o PID do watchdog
    FILE *fp = fopen(PIDFILE, "w");
    if (fp) {
        fprintf(fp, "%d", getpid());
        fclose(fp);
    }

    printf("[WATCHDOG] Vigilancia 2.0 iniciada. Monitorando %s a cada %d segundos...\n", MOTOR_PATH, INTERVALO);

    while (1) {
        verificar_motor();
        sleep(INTERVALO);
    }

    return 0;
}