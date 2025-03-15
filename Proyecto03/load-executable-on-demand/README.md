[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/oLDCyGI5)
[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/8uf79deI)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-7f7980b617ed060a017424585567c406b6ee15c891e84e1186181d67ecf80aa0.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=11870987)
# Pintos Programming Exercises

## Introduction

This project aims to provide an organized way to solve every [pintos project](https://web.stanford.edu/class/cs140/projects/pintos/pintos.html#SEC_Contents). This project includes infrastructure to run the code from a docker image and every exercise provide a script to actually execute the tests to be graded in that specific exercise and the previous tests within the same project.

This project only has the code base, the docker image and some branches with the solutions, if you want to see the exercise descriptions you may visit [this repo](https://github.com/alain-chevanier/pintos-exercise-descriptions).

## Software Dependencies

This project assumes the user is using an operating system with the following software:

* A bash console.
* Docker and Docker deamon

To open a terminal running in the docker image use:

For linux/macos:
```bash
./docker-open-terminal
```

For windows using powershell, and running it as an admin:
```powershell
& "./docker-open-terminal.ps1"
```

I none of the above works, you can simple call docker directly 
by calling the following command:
```sh
docker run -it --volume ${pwd}:/app --workdir /app --rm alainchevanier/so-20231-pintos bash
```
__Note__: This command works in bash, zsh, sh and powershell (windows).

To verify the installation, run the following instructions from a terminal inside the docker image (use the previous command):

1. You should build the utils directory (first time only):
```bash
cd /app/src/utils
make
```
2. Once you successfully accessed the console, move to the `threads` directory and compile the code.
```bash
cd /app/src/threads
make
```
3. Finally just run a test to verify that everything is up and running.
```bash
cd /app/src/threads/build
pintos -q run alarm-single
# to check if a specific test pass use a command like this
make build/tests/threads/alarm-single.result
```

## Alternative Documentation

This project takes as base _thierrysans_ variation of pintos and the docker image he originally proposed. You may find all the related information in [this](https://thierrysans.me/CSCC69/projects/WWW/pintos_1.html) website.
