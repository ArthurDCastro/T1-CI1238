# Trabalho 1 - Otimização (CI1238)

Este projeto resolve uma **instância de otimização de carregamento de compartimentos** e gera um modelo no formato aceito pelo `lp_solve`.

O objetivo é **maximizar o ganho total** ao distribuir diferentes tipos de carregamento em vários compartimentos de um veículo, respeitando:

- Restrições de peso e volume por compartimento
- Limite de disponibilidade de cada carregamento
- Proporcionalidade da carga entre compartimentos
- Não-negatividade das variáveis de decisão

## 📦 Estrutura do Projeto

- `main.c`: ponto de entrada do programa
- `lib/`: módulos auxiliares
  - `input.[ch]`: leitura da entrada
  - `lpwriter.[ch]`: construção do modelo `.lp`
  - `utils.[ch]`: funções auxiliares
  - `structs.h`: definição das estruturas
- `exemplos/`: arquivos de entrada de teste
- `Makefile`: para compilar o projeto

## ⚙️ Compilação

```bash
make
```

## ▶️ Execução

```bash
./carga < exemplos/ex1.txt
```

## 📄 Saída

A saída é um modelo de programação linear no formato do `lp_solve`, impresso no `stdout`.

Para salvar a saída em arquivo:

```bash
./carga < exemplos/ex1.txt > modelo.lp
```

E rodar no `lp_solve`:

```bash
lp_solve modelo.lp
```
