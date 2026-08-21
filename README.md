# Chemical Warfare

Jogo 2D educacional desenvolvido em **C** utilizando a biblioteca **Allegro 5**.

O projeto está sendo desenvolvido em grupo utilizando **Visual Studio, C, Allegro 5, Git e GitHub**.

---

# 📋 Sumário

- [Tecnologias](#-tecnologias)
- [Estrutura do projeto](#-estrutura-do-projeto)
- [1. Instalar o Git](#1-instalar-o-git)
- [2. Configurar o Git](#2-configurar-o-git)
- [3. Instalar o Visual Studio](#3-instalar-o-visual-studio)
- [4. Instalar o Allegro 5](#4-instalar-o-allegro-5)
- [5. Clonar o projeto](#5-clonar-o-projeto)
- [6. Abrir o projeto](#6-abrir-o-projeto)
- [7. Configurar o Allegro no Visual Studio](#7-configurar-o-allegro-no-visual-studio)
- [8. Configurar os Headers](#8-configurar-os-headers)
- [9. Compilar o projeto](#9-compilar-o-projeto)
- [10. Comandos Git](#10-comandos-git)
- [11. Fluxo de trabalho da equipe](#11-fluxo-de-trabalho-da-equipe)
- [12. Criar uma nova branch](#12-criar-uma-nova-branch)
- [13. Enviar uma branch para o GitHub](#13-enviar-uma-branch-para-o-github)
- [14. Atualizar o projeto](#14-atualizar-o-projeto)
- [15. Conflitos](#15-conflitos)
- [16. Cuidados importantes](#16-cuidados-importantes)

---

# 🛠 Tecnologias

- C
- Visual Studio
- Allegro 5
- Git
- GitHub

---

# 📁 Estrutura do projeto

```text
Chemical-Warfare/
│
├── assets/
│   ├── fonts/
│   ├── images/
│   └── sounds/
│
├── include/
│   ├── geral.h
│   ├── inimigos.h
│   ├── player.h
│   └── tiro.h
│
├── src/
│   ├── geral.c
│   ├── inimigos.c
│   ├── main.c
│   ├── player.c
│   └── tiro.c
│
├── .gitignore
├── README.md
├── packages.config
├── ConsoleApplication1.vcxproj
└── ConsoleApplication1.vcxproj.filters

src/

Contém os arquivos .c do projeto.

Exemplo:

src/player.c
src/tiro.c
src/inimigos.c
include/

Contém os arquivos .h.

Exemplo:

include/player.h
include/tiro.h
assets/

Contém os recursos do jogo:

assets/fonts/
assets/images/
assets/sounds/
1. Instalar o Git
Windows

Baixe o Git pelo site oficial:

https://git-scm.com/downloads

Baixe a versão para Windows.

Durante a instalação, você pode manter as opções padrão.

Depois de instalar, abra o:

Git Bash

Verifique se o Git foi instalado:

git --version

Se aparecer algo semelhante a:

git version 2.x.x

o Git está instalado corretamente.

2. Configurar o Git

Se esta for a primeira vez que você utiliza Git no computador, configure seu nome:

git config --global user.name "SEU NOME"

Exemplo:

git config --global user.name "João Silva"

Configure também o e-mail da sua conta do GitHub:

git config --global user.email "seuemail@email.com"

Para conferir:

git config --global --list

Você deverá encontrar:

user.name=João Silva
user.email=seuemail@email.com
3. Instalar o Visual Studio

Baixe o Visual Studio:

https://visualstudio.microsoft.com/

Durante a instalação, selecione a carga de trabalho:

Desenvolvimento para Desktop com C++

Mesmo que o projeto seja escrito em C, utilizamos as ferramentas de compilação fornecidas pelo Visual Studio para C/C++.

Depois de instalar, abra o Visual Studio uma vez para confirmar que está funcionando.

4. Instalar o Allegro 5

O projeto utiliza a biblioteca Allegro 5.

O Allegro precisa estar instalado e configurado antes de tentar compilar o projeto.

4.1 Baixar o Allegro

Baixe o Allegro 5 pelo site oficial:

https://liballeg.org/

Procure uma versão compatível com:

Windows
64 bits
Visual Studio

Extraia o arquivo baixado em um local permanente.

Por exemplo:

C:\Allegro

Evite colocar a biblioteca dentro da pasta do projeto.

5. Clonar o projeto

Depois de instalar o Git, abra o Git Bash.

Escolha uma pasta para guardar o projeto.

Por exemplo:

cd ~/Documents

Agora clone o repositório:

git clone https://github.com/EnzoVarasquim/Chemical-Warfare.git

Entre na pasta:

cd Chemical-Warfare

Confira os arquivos:

ls

Você deverá encontrar:

assets
include
src
README.md
ConsoleApplication1.vcxproj
ConsoleApplication1.vcxproj.filters
packages.config
6. Abrir o projeto

Dentro da pasta do projeto existe:

ConsoleApplication1.vcxproj

Você pode abrir dando duplo clique nesse arquivo.

Outra opção:

Abra o Visual Studio.
Clique em Open a project or solution.
Entre na pasta Chemical-Warfare.
Selecione:
ConsoleApplication1.vcxproj
Clique em Open.
7. Configurar o Allegro no Visual Studio

⚠️ Esta etapa precisa ser feita na primeira configuração da máquina.

Depois de abrir o projeto:

Botão direito no projeto → Properties

No topo, selecione:

Configuration: All Configurations
Platform: x64
7.1 Include Directories

Entre em:

C/C++
→ General
→ Additional Include Directories

Adicione a pasta include do Allegro.

Exemplo:

C:\Allegro\include

Use o caminho real onde você instalou o Allegro.

7.2 Library Directories

Entre em:

Linker
→ General
→ Additional Library Directories

Adicione a pasta onde estão os arquivos .lib do Allegro.

Exemplo:

C:\Allegro\lib

Use o caminho real da sua instalação.

7.3 Linker → Input

Entre em:

Linker
→ Input
→ Additional Dependencies

Adicione as bibliotecas necessárias do Allegro.

Exemplo:

allegro-5.x.x-monolith-mt.lib

O nome exato depende da versão do Allegro instalada.

7.4 DLLs

Para executar o jogo, as DLLs do Allegro também precisam estar disponíveis.

Uma forma simples é colocar as DLLs necessárias junto do .exe gerado.

Por exemplo:

x64/Debug/

As DLLs não devem ser adicionadas ao Git se forem arquivos gerados/externos. Cada integrante deve configurar o próprio ambiente.

8. Configurar os Headers do projeto

O projeto possui os headers dentro de:

include/

No Visual Studio:

Project Properties
→ C/C++
→ General
→ Additional Include Directories

Deve existir:

$(ProjectDir)include

Isso permite utilizar:

#include "geral.h"
#include "player.h"
#include "tiro.h"
#include "inimigos.h"

em vez de:

#include "include/geral.h"
9. Compilar o projeto

No Visual Studio:

Build
→ Rebuild Solution

Ou:

Ctrl + Shift + B

Se não houver erros, execute:

F5

ou clique em:

Local Windows Debugger
10. Comandos Git
Verificar o estado do projeto
git status
Baixar alterações do GitHub
git pull
Ver histórico de commits
git log --oneline
Ver alterações
git diff
Adicionar alterações
git add .
Criar um commit
git commit -m "Descrição da alteração"

Exemplo:

git commit -m "Adiciona movimentação do jogador"
Enviar alterações para o GitHub
git push
11. Fluxo de trabalho da equipe

Antes de começar a programar:

git pull

Faça suas alterações no Visual Studio.

Depois:

git status

Adicione os arquivos:

git add .

Crie o commit:

git commit -m "Descrição da alteração"

Envie para o GitHub:

git push

Fluxo completo:

git pull
     ↓
Programar
     ↓
git status
     ↓
git add .
     ↓
git commit -m "Descrição"
     ↓
git push
12. Criar uma nova branch

Para trabalhar em uma funcionalidade sem mexer diretamente na main:

git checkout -b nome-da-feature

Exemplo:

git checkout -b sistema-inimigos

Confira em qual branch você está:

git branch

A branch atual aparecerá com:

* sistema-inimigos
13. Enviar uma branch para o GitHub

Depois de fazer seus commits:

git push -u origin sistema-inimigos

Depois disso, a branch aparecerá no GitHub.

14. Atualizar o projeto

Antes de começar um novo trabalho:

git checkout main
git pull

Se você estiver trabalhando em uma branch:

git checkout nome-da-feature

E continue trabalhando.

15. Conflitos

Pode acontecer de duas pessoas alterarem a mesma parte do mesmo arquivo.

O Git pode mostrar:

CONFLICT

Nesse caso:

NÃO faça:

git init

NÃO apague a pasta .git.

NÃO force um push sem saber o que está acontecendo.

Abra o arquivo indicado pelo Git.

Você poderá encontrar algo como:

<<<<<<< HEAD


código da sua versão


=======


código da outra versão


>>>>>>> outra-branch

Decida qual código deve permanecer e remova os marcadores.

Depois:

git add .

e:

git commit -m "Resolve conflito"

Se houver dúvidas, converse com os outros integrantes antes de continuar.

16. Cuidados importantes
Não executar git init

O projeto já possui um repositório Git.

Não execute:

git init

dentro do projeto.

Não apagar .git

A pasta:

.git/

contém o histórico e as configurações do Git.

Não apague essa pasta.

Não enviar arquivos de compilação

O projeto possui um .gitignore.

Arquivos como:

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

não devem ser enviados para o GitHub.

Não trabalhar dentro de x64/

A pasta:

x64/

é utilizada pelo Visual Studio para arquivos gerados durante a compilação.

Não coloque código-fonte nela.

🔄 Fluxo recomendado para a equipe
Ao começar o dia
git checkout main
git pull
Criar sua funcionalidade
git checkout -b minha-feature
Trabalhar

Faça as alterações no Visual Studio.

Salvar no Git
git status
git add .
git commit -m "Descrição da alteração"
git push -u origin minha-feature
Depois

Abra um Pull Request no GitHub para juntar sua branch à main.

👥 Equipe

Projeto desenvolvido em grupo.

Repositório:

https://github.com/EnzoVarasquim/Chemical-Warfare
