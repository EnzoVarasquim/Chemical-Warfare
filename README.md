# Chemical Warfare

Jogo 2D educacional desenvolvido em C utilizando a biblioteca Allegro 5.

Projeto desenvolvido em grupo com Visual Studio, Git e GitHub.

## Tecnologias

* C
* Allegro 5
* Visual Studio 2022/2026
* Git
* GitHub

## Estrutura

```text
Chemical-Warfare/
├── assets/
│   ├── fonts/
│   ├── images/
│   └── sounds/
├── include/
│   ├── geral.h
│   ├── inimigos.h
│   ├── player.h
│   └── tiro.h
├── src/
│   ├── geral.c
│   ├── inimigos.c
│   ├── main.c
│   ├── player.c
│   └── tiro.c
├── packages/
├── .gitignore
├── README.md
├── packages.config
├── ConsoleApplication1.vcxproj
└── ConsoleApplication1.vcxproj.filters
```

### `src/`

Código-fonte do jogo.

### `include/`

Headers, estruturas e protótipos das funções.

### `assets/`

Imagens, fontes e sons utilizados pelo jogo.

### `packages/`

Dependências do Allegro utilizadas pelo projeto. A pasta é versionada no repositório para facilitar a configuração em outros computadores.

---

# Configuração

## 1. Instalar o Git

Baixe e instale o Git:

[Git — Site oficial](https://git-scm.com/downloads?utm_source=chatgpt.com)

Verifique a instalação:

```bash
git --version
```

## 2. Configurar o Git

Na primeira utilização do Git:

```bash
git config --global user.name "Seu Nome"
git config --global user.email "seu@email.com"
```

Verifique:

```bash
git config --global --list
```

## 3. Instalar o Visual Studio

Instale o Visual Studio e selecione a carga de trabalho:

```text
Desenvolvimento para Desktop com C++
```

O projeto utiliza as ferramentas de compilação C/C++ do Visual Studio.

O projeto utiliza o toolset `v143`, compatível com o Visual Studio 2022 e versões posteriores que disponibilizem esse toolset.

---

# Clonar o projeto

No Git Bash:

```bash
cd ~/Documents
git clone https://github.com/EnzoVarasquim/Chemical-Warfare.git
cd Chemical-Warfare
```

Verifique os arquivos:

```bash
ls
```

---

# Abrir o projeto

Abra:

```text
ConsoleApplication1.vcxproj
```

pelo Visual Studio.

Não é necessário executar `git init`.

A pasta `.git` já existe no repositório clonado.

---

# Configuração do projeto

Os headers do projeto estão em:

```text
include/
```

No Visual Studio, confira:

```text
Project Properties
→ C/C++
→ General
→ Additional Include Directories
```

Deve existir:

```text
$(ProjectDir)include
```

As dependências do Allegro estão no diretório:

```text
packages/
```

Portanto, não remova essa pasta do projeto.

---

# Compilar e executar

No Visual Studio:

```text
Build → Rebuild Solution
```

Ou:

```text
Ctrl + Shift + B
```

Para executar:

```text
F5
```

ou:

```text
Local Windows Debugger
```

Os arquivos gerados pelo Visual Studio, como `x64/`, `Debug/` e `Release/`, não devem ser enviados para o Git.

---

# Git

## Verificar alterações

```bash
git status
```

Versão resumida:

```bash
git status --short
```

## Atualizar o projeto

Antes de começar a trabalhar:

```bash
git pull
```

## Ver alterações

```bash
git diff
```

## Adicionar alterações

```bash
git add .
```

Ou todos os arquivos:

```bash
git add -A
```

## Criar commit

```bash
git commit -m "Descrição da alteração"
```

Exemplo:

```bash
git commit -m "Adiciona sistema de inimigos"
```

## Enviar alterações

```bash
git push
```

## Ver histórico

```bash
git log --oneline
```

---

# Fluxo de trabalho

Antes de programar:

```bash
git pull
```

Faça as alterações no código.

Depois:

```bash
git status
git add .
git commit -m "Descrição da alteração"
git push
```

Fluxo básico:

```text
git pull
    ↓
Programar
    ↓
git status
    ↓
git add .
    ↓
git commit
    ↓
git push
```

---

# Branches

Para desenvolver uma funcionalidade separadamente:

```bash
git checkout -b nome-da-feature
```

Exemplo:

```bash
git checkout -b sistema-inimigos
```

Ver branches:

```bash
git branch
```

Trocar de branch:

```bash
git checkout main
```

ou:

```bash
git checkout nome-da-feature
```

Enviar uma nova branch:

```bash
git push -u origin nome-da-feature
```

Depois, abra um Pull Request no GitHub para juntar a branch à `main`.

---

# Conflitos

Se o Git informar:

```text
CONFLICT
```

não faça:

```bash
git init
```

e não apague:

```text
.git/
```

Abra os arquivos indicados pelo Git e resolva manualmente as partes conflitantes.

Depois:

```bash
git add .
git commit -m "Resolve conflitos"
```

Se não souber qual alteração deve permanecer, converse com o integrante responsável antes de continuar.

---

# Arquivos ignorados

O `.gitignore` impede o envio de arquivos gerados pelo Visual Studio e pelo compilador.

Entre eles:

```text
.vs/
x64/
x86/
Debug/
Release/
*.obj
*.exe
*.dll
*.lib
*.pdb
*.ilk
*.user
*.suo
```

A pasta `packages/` **não deve ser adicionada ao `.gitignore`**, pois as dependências do Allegro utilizadas pelo projeto estão versionadas no repositório.

---

# Regras para desenvolvimento

* Não executar `git init` dentro do projeto.
* Não apagar a pasta `.git/`.
* Não trabalhar diretamente dentro de `x64/`, `Debug/` ou `Release/`.
* Código `.c` deve ficar em `src/`.
* Headers `.h` devem ficar em `include/`.
* Recursos devem ficar em `assets/`.
* Sempre executar `git pull` antes de começar a trabalhar.
* Faça commits pequenos e com mensagens descritivas.
* Evite trabalhar diretamente na `main` quando estiver desenvolvendo uma funcionalidade nova.

---

# Repositório

[Chemical Warfare — GitHub](https://github.com/EnzoVarasquim/Chemical-Warfare)
