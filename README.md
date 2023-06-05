# reversi

## Description

* 이 프로젝트는 UI를 위한 `ncurses` 라이브러리와 멀티플레이어 기능을 위한 `sockets`를 사용하여 C에서 구현한 **리버시 게임** 입니다.

## Features

* 텍스트 기반 그래픽 (TUI)으로 게임 보드를 구현했습니다.
* 네트워크를 통해 멀티로 플레이가 가능합니다.
* 표준 8x8 리버시 보드를 지원합니다.
* 보통의 리버시 게임과 규칙이 동일합니다.

## Requirements

이 프로젝트를 실행하기 위해서는 다음이 필요합니다.

* C compiler
* Socket server
* Linux OS

## How to build

1. 이 레포지토리를 `git clone` 합니다.

```
git clone https://github.com/seungjoonH/reversi.git
```

2. clone 된 디렉토리 위치로 이동합니다.
3. 소스코드를 컴파일 합니다. (C compiler required)

```
make
```

4. 실행파일을 실행합니다.

  * `-option` 으로 역할을 설정합니다.
  * server ip 와 port 번호를 입력합니다.
  
  ```
  ./reversi -server 8888            # for server
  ```
  ```
  ./reversi -client 127.0.0.1 8888  # for client
  ```
  
  ## Usage
  
  1. reversi 게임을 설치합니다.
  2. 방향키를 사용하여 게임 보드의 커서를 움직입니다.
  3. 스페이스 바를 눌러 원하는 위치에 말을 놓습니다.
  4. `^C` (Ctrl + C) 를 눌러 종료합니다.
