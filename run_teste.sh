#!/bin/bash
for i in {1..6}; do
    echo "Rodando teste $i..."
    ./carga < ./testes/teste$i.txt > model/model$i.lp
    if [ $? -ne 0 ]; then
        echo "Erro no teste $i: programa falhou."
        continue
    fi
    echo "Modelo gerado: model$i.lp"
    lp_solve model/model$i.lp > resultados/result$i.txt
    if [ $? -ne 0 ]; then
        echo "Erro no teste $i: lp_solve falhou."
    else
        echo "Resultado do lp_solve para teste $i:"
        cat resultados/result$i.txt
    fi
    echo ""
done