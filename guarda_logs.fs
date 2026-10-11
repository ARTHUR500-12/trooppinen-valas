\ guarda_logs.fs – Guardião da integridade dos logs com checksum

variable soma-anterior
variable contador-falhas

: calcular-checksum ( -- n )
    s" defesa.log" slurp-file
    over +
    swap
    0 -rot
    do
        i c@ +
    loop
;

: alerta ( -- )
    ." [FORTH-GUARDIAO] ALERTA: defesa.log foi adulterado!" cr
    s" echo 'Alerta de adulteracao do log!' >> emergencia.log" system
;

: verificar ( -- )
    calcular-checksum
    soma-anterior @
    2dup <> if
        alerta
        soma-anterior !
        1 contador-falhas +!
    else
        drop
    then
;

: iniciar-guarda ( -- )
    calcular-checksum soma-anterior !
    0 contador-falhas !
    ." Guardiao Forth com checksum ativado. Monitorando defesa.log..." cr
    begin
        verificar
        60000 ms
    again
;

iniciar-guarda