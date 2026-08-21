# 🧪 Chemical Warfare

> 🎮 Jogo 2D educacional desenvolvido em **C** utilizando a biblioteca **Allegro 5**.

Projeto desenvolvido em grupo utilizando:

- 💻 **C**
- 🎨 **Allegro 5**
- 🛠️ **Visual Studio**
- 🔧 **Git**
- 🌐 **GitHub**

---

## 📋 Sumário

- [🛠️ Tecnologias](#️-tecnologias)
- [📁 Estrutura do projeto](#-estrutura-do-projeto)
- [1️⃣ Instalar o Git](#1️⃣-instalar-o-git)
- [2️⃣ Configurar o Git](#2️⃣-configurar-o-git)
- [3️⃣ Instalar o Visual Studio](#3️⃣-instalar-o-visual-studio)
- [4️⃣ Instalar o Allegro 5](#4️⃣-instalar-o-allegro-5)
- [5️⃣ Clonar o projeto](#5️⃣-clonar-o-projeto)
- [6️⃣ Abrir o projeto](#6️⃣-abrir-o-projeto)
- [8️⃣ Configurar os Headers](#8️⃣-configurar-os-headers)
- [9️⃣ Compilar o projeto](#9️⃣-compilar-o-projeto)
- [🔟 Comandos Git](#-comandos-git)
- [👥 Fluxo de trabalho da equipe](#-fluxo-de-trabalho-da-equipe)
- [🌿 Criar uma nova branch](#-criar-uma-nova-branch)
- [⬆️ Enviar uma branch para o GitHub](#️-enviar-uma-branch-para-o-github)
- [🔄 Atualizar o projeto](#-atualizar-o-projeto)
- [⚠️ Conflitos](#️-conflitos)
- [🚨 Cuidados importantes](#-cuidados-importantes)

---

# 🛠️ Tecnologias

| Tecnologia | Utilização |
|---|---|
| **C** | Linguagem principal |
| **Allegro 5** | Criação do jogo 2D |
| **Visual Studio** | Desenvolvimento e compilação |
| **Git** | Controle de versão |
| **GitHub** | Armazenamento e colaboração |

---

# 📁 Estrutura do projeto

```text
Chemical-Warfare/
│
├── 📂 assets/
│   ├── 📂 fonts/
│   ├── 📂 images/
│   └── 📂 sounds/
│
├── 📂 include/
│   ├── geral.h
│   ├── inimigos.h
│   ├── player.h
│   └── tiro.h
│
├── 📂 src/
│   ├── geral.c
│   ├── inimigos.c
│   ├── main.c
│   ├── player.c
│   └── tiro.c
│
├── 📄 .gitignore
├── 📄 README.md
├── 📄 packages.config
├── 📄 ConsoleApplication1.vcxproj
└── 📄 ConsoleApplication1.vcxproj.filters
```

### 📂 `src/`

Contém os arquivos `.c` responsáveis pela implementação do jogo.

```text
src/
├── geral.c
├── inimigos.c
├── main.c
├── player.c
└── tiro.c
```

### 📂 `include/`

Contém os arquivos `.h`, que possuem estruturas, declarações e protótipos das funções.

```text
include/
├── geral.h
├── inimigos.h
├── player.h
└── tiro.h
```

### 📂 `assets/`

Contém os recursos utilizados pelo jogo.

```text
assets/
├── fonts/
├── images/
└── sounds/
```

---

# 1️⃣ Instalar o Git

## 🪟 Windows

Baixe o Git pelo site oficial:

https://git-scm.com/downloads

Baixe a versão para **Windows**.

Durante a instalação, as opções padrão podem ser mantidas.

Após a instalação, abra o:

> **Git Bash**

### ✅ Verificar a instalação

Execute:

```bash
git --version
```

Se aparecer algo semelhante a:

```text
git version 2.x.x
```

o Git foi instalado corretamente.

---

# 2️⃣ Configurar o Git

Se esta for a primeira vez que você utiliza Git neste computador, configure seu nome:

```bash
git config --global user.name "SEU NOME"
```

### Exemplo

```bash
git config --global user.name "João Silva"
```

Configure também o e-mail utilizado na sua conta do GitHub:

```bash
git config --global user.email "seuemail@email.com"
```

### 🔎 Verificar a configuração

```bash
git config --global --list
```

Você deverá encontrar algo semelhante a:

```text
user.name=João Silva
user.email=seuemail@email.com
```

---

# 3️⃣ Instalar o Visual Studio

Baixe o Visual Studio pelo site oficial:

https://visualstudio.microsoft.com/

Durante a instalação, selecione a carga de trabalho:

> **Desenvolvimento para Desktop com C++**

> 💡 Mesmo que o projeto seja escrito em C, utilizamos as ferramentas de compilação fornecidas pelo Visual Studio para C/C++.

Após a instalação, abra o Visual Studio pelo menos uma vez para confirmar que está funcionando corretamente.

---

# 4️⃣ Instalar o Allegro 5

O projeto utiliza a biblioteca **Allegro 5**.

O Allegro precisa estar instalado e configurado antes de tentar compilar o projeto.

## 4.1 📥 Baixar o Allegro

Guia detalhado para instalar o Allegro 5 direto pelo Visual Studio:

https://youtu.be/mJFYV8Hk6jY?si=MRp6EtQmP5Q6xG4h

---

# 5️⃣ Clonar o projeto

Depois de instalar o Git, abra o **Git Bash**.

Escolha uma pasta onde deseja armazenar o projeto.

Por exemplo:

```bash
cd ~/Documents
```

Agora clone o repositório:

```bash
git clone https://github.com/EnzoVarasquim/Chemical-Warfare.git
```

Entre na pasta:

```bash
cd Chemical-Warfare
```

### 🔎 Conferir os arquivos

```bash
ls
```

Você deverá encontrar:

```text
assets
include
src
README.md
ConsoleApplication1.vcxproj
ConsoleApplication1.vcxproj.filters
packages.config
```

---

# 6️⃣ Abrir o projeto

Dentro da pasta do projeto existe o arquivo:

```text
ConsoleApplication1.vcxproj
```

## Opção 1 — Abrir diretamente

Dê **duplo clique** em:

```text
ConsoleApplication1.vcxproj
```

## Opção 2 — Abrir pelo Visual Studio

1. Abra o **Visual Studio**.
2. Clique em **Open a project or solution**.
3. Entre na pasta `Chemical-Warfare`.
4. Selecione:

```text
ConsoleApplication1.vcxproj
```

5. Clique em **Open**.

---

# 8️⃣ Configurar os Headers

Os headers do projeto estão dentro de:

```text
include/
```

No Visual Studio:

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

Isso permite utilizar:

```c
#include "geral.h"
#include "player.h"
#include "tiro.h"
#include "inimigos.h"
```

em vez de:

```c
#include "include/geral.h"
```

---

# 9️⃣ Compilar o projeto

No Visual Studio:

```text
Build
→ Rebuild Solution
```

Ou utilize:

```text
Ctrl + Shift + B
```

Se não houver erros, execute o jogo utilizando:

```text
F5
```

ou clique em:

> **Local Windows Debugger**

🎉 Se a janela do jogo abrir corretamente, sua configuração está pronta!

---

# 🔟 Comandos Git

## 🔎 Verificar o estado do projeto

```bash
git status
```

---

## ⬇️ Baixar alterações do GitHub

```bash
git pull
```

---

## 📜 Ver histórico de commits

```bash
git log --oneline
```

---

## 🔍 Ver alterações feitas

```bash
git diff
```

---

## ➕ Adicionar alterações

```bash
git add .
```

---

## 💾 Criar um commit

```bash
git commit -m "Descrição da alteração"
```

### Exemplo

```bash
git commit -m "Adiciona movimentação do jogador"
```

---

## ⬆️ Enviar alterações para o GitHub

```bash
git push
```

---

# 👥 Fluxo de trabalho da equipe

Antes de começar a programar:

```bash
git pull
```

Faça suas alterações no Visual Studio.

Depois:

```bash
git status
```

Adicione os arquivos:

```bash
git add .
```

Crie o commit:

```bash
git commit -m "Descrição da alteração"
```

Envie para o GitHub:

```bash
git push
```

### 🔄 Fluxo completo

```text
        git pull
           │
           ▼
    👨‍💻 Programar
           │
           ▼
       git status
           │
           ▼
        git add .
           │
           ▼
git commit -m "Descrição"
           │
           ▼
        git push
```

---

# 🌿 Criar uma nova branch

Para trabalhar em uma funcionalidade sem modificar diretamente a `main`:

```bash
git checkout -b nome-da-feature
```

### Exemplo

```bash
git checkout -b sistema-inimigos
```

Verifique em qual branch você está:

```bash
git branch
```

A branch atual aparecerá com `*`:

```text
* sistema-inimigos
  main
```

---

# ⬆️ Enviar uma branch para o GitHub

Depois de fazer seus commits:

```bash
git push -u origin sistema-inimigos
```

Depois disso, a branch aparecerá no GitHub.

---

# 🔄 Atualizar o projeto

Antes de começar um novo trabalho:

```bash
git checkout main
git pull
```

Se você estiver trabalhando em uma branch:

```bash
git checkout nome-da-feature
```

E continue trabalhando normalmente.

---

# ⚠️ Conflitos

Pode acontecer de duas pessoas alterarem a mesma parte do mesmo arquivo.

Nesse caso, o Git poderá mostrar:

```text
CONFLICT
```

### ❌ NÃO faça:

```bash
git init
```

### ❌ NÃO apague:

```text
.git/
```

### ❌ NÃO force um `push` sem saber o que está acontecendo.

---

## 🔧 Resolver um conflito

Abra o arquivo indicado pelo Git.

Você poderá encontrar algo semelhante a:

```text
<<<<<<< HEAD

código da sua versão

=======

código da outra versão

>>>>>>> outra-branch
```

Decida qual código deve permanecer e remova os marcadores:

```text
<<<<<<<
=======
>>>>>>>
```

Depois:

```bash
git add .
```

E crie o commit:

```bash
git commit -m "Resolve conflito"
```

> 💡 Se você não souber qual versão deve permanecer, converse com o integrante que fez a outra alteração antes de continuar.

---

# 🚨 Cuidados importantes

## ❌ Não executar `git init`

O projeto **já possui um repositório Git**.

Não execute:

```bash
git init
```

dentro da pasta do projeto.

---

## ❌ Não apagar `.git`

A pasta:

```text
.git/
```

contém o histórico e as configurações do Git.

**Não apague essa pasta.**

---

## 🚫 Não enviar arquivos de compilação

O projeto possui um `.gitignore` configurado.

Arquivos como:

```text
x64/
Debug/
Release/
.vs/
*.obj
*.pdb
*.ilk
*.exe
*.user
*.log
```

não devem ser enviados para o GitHub.

---

## 🚫 Não trabalhar dentro de `x64/`

A pasta:

```text
x64/
```

é utilizada pelo Visual Studio para arquivos gerados durante a compilação.

Não coloque código-fonte nela.

O código deve ficar em:

```text
src/
include/
```

---

# 🔄 Fluxo recomendado para a equipe

## 1. 🌅 Ao começar o trabalho

```bash
git checkout main
git pull
```

## 2. 🌿 Criar sua funcionalidade

```bash
git checkout -b minha-feature
```

## 3. 👨‍💻 Trabalhar

Faça as alterações normalmente no Visual Studio.

## 4. 💾 Salvar no Git

```bash
git status
git add .
git commit -m "Descrição da alteração"
```

## 5. ⬆️ Enviar para o GitHub

```bash
git push -u origin minha-feature
```

## 6. 🔀 Pull Request

Depois, abra um **Pull Request** no GitHub para juntar sua branch à `main`.

---

# 🧪 Chemical Warfare

Projeto desenvolvido para fins educacionais.

### 🌐 Repositório

**GitHub:**  
https://github.com/EnzoVarasquim/Chemical-Warfare

---

### 👥 Equipe

Projeto desenvolvido em grupo utilizando:

**C + Allegro 5 + Visual Studio + Git + GitHub**

---

> 💡 **Dica:** Antes de alterar qualquer código, sempre faça `git pull`.  
> Isso reduz bastante a chance de conflitos entre os integrantes.
