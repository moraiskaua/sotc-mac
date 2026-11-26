# Contribuindo

## Branches

| Branch | Para que serve |
| --- | --- |
| `develop` | branch padrão; recebe as mudanças |
| `staging` | o que está sendo testado antes de ir para produção |
| `production` | o que está estável |

Fluxo:

1. Crie a branch a partir de `develop`: `feat/<nome>`, `fix/<nome>` ou
   `docs/<nome>`.
2. Abra o PR para `develop`.
3. Depois de testado, `develop` avança para `staging` e, por fim, para
   `production`.

## Mudanças para o projeto original

Correções e recursos que servem para todo mundo devem ir também para
[HenryKun55/celosia](https://github.com/HenryKun55/celosia). Nesse caso a
branch sai do `main` dele, não do `develop` deste fork, e o PR vai para
`HenryKun55/celosia:main`. Mudanças só deste fork (como o README em português)
ficam aqui.

Para trazer as novidades do projeto original:

```sh
git remote add upstream https://github.com/HenryKun55/celosia.git
git fetch upstream
git switch develop
git merge upstream/main
```

## Padrão de código

Siga o que já existe no projeto:

- Código, comentários, mensagens de commit e documentos técnicos em inglês.
  O que é só deste fork (README, este arquivo, `docs/arquitetura.md`,
  `docs/README.md`) fica em português.
- Toda opção nova tem item no menu (F1) **e** variável `SOTC_*`, e começa
  desligada quando muda o comportamento do jogo.
- Função do jogo reescrita à mão precisa ser comparada com a recompilada no
  mesmo estado (`SOTC_DIFFTEST` ou uma checagem `*_CHECK`), como descrito em
  [docs/decomp.md](docs/decomp.md).
- Estrutura usada por mais de um `.cpp` vai num header. O unity build do Mac
  junta arquivos vizinhos e uma definição duplicada quebra o build
  ([docs/arquitetura.md](docs/arquitetura.md#build)).
- Testes novos entram na suíte existente (`PS2Recomp/ps2xTest`).
- Documento novo: `docs/<assunto>.md`, e uma linha em
  [docs/README.md](docs/README.md).
- Nos textos, sem travessão.

## Antes de abrir o PR

```sh
./celosia build      # compila o runner
./celosia run        # o jogo abre e chega na cena de abertura
```

Se mexeu em `celosia` ou na ordem dos arquivos, rode também
`./celosia clean && ./celosia setup` para conferir o build do zero.

## Licença

Contribuições entram sob a GPL-3.0 ([LICENSE](LICENSE)).
