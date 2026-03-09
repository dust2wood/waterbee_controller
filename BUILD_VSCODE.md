# Cursor/VS Code에서 STM32 빌드 및 플래싱 가이드

Keil 없이 Cursor(또는 VS Code)에서 ID_Solution 프로젝트를 빌드하고 ST-LINK V2로 플래싱할 수 있습니다.

## 사전 요구사항

1. **ARM GCC 툴체인**
   - [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm) 설치
   - 설치 후 `arm-none-eabi-gcc`가 PATH에 추가되어야 함

2. **CMake** (3.16 이상)
   - [CMake](https://cmake.org/download/) 설치

3. **Ninja** (빌드 시스템)
   - `ninja`가 PATH에 있어야 함 (CMake와 함께 설치되거나 별도 설치)

4. **stm32f10x.h 헤더**
   - Keil을 사용 중이라면: `C:\Keil\ARM\INC\ST\STM32F10x` 경로가 자동으로 사용됨
   - Keil이 없다면: STM32F10x_StdPeriph_Lib에서 `stm32f10x.h`를 복사하여 `Working/CMSIS/Device/ST/STM32F10x/Include/`에 넣기

5. **디버깅/플래싱**
   - **OpenOCD**: [OpenOCD](https://openocd.org/) 설치 후 PATH 추가
   - **Cortex-Debug** 확장: VS Code/Cursor에서 "Cortex-Debug" 확장 설치
   - (선택) **stlink**: `st-flash` 명령으로 플래싱 시 [stlink](https://github.com/stlink-org/stlink) 설치

## 빌드 방법

### 1. 최초 1회 Configure
```
메뉴: Terminal > Run Task > STM32: Configure
```

### 2. 빌드
```
메뉴: Terminal > Run Task > STM32: Build
또는 단축키: Ctrl+Shift+B
```

### 3. 결과물
- `build/ID_Solution.elf` - 디버깅용
- `build/ID_Solution.hex` - Hex 파일
- `build/ID_Solution.bin` - 바이너리

## 플래싱 (ST-LINK V2)

### 방법 A: Cortex-Debug로 플래시 및 디버깅
1. **Cortex-Debug** 확장 설치
2. F5 또는 Run > Start Debugging
3. "STM32: Debug (ST-LINK V2)" 또는 "STM32: Flash & Run" 선택
4. 빌드 후 자동으로 플래시되고 디버깅 시작

### 방법 B: st-flash로 플래시만
1. stlink 도구 설치
2. `Terminal > Run Task > STM32: Flash (st-flash)` 실행

### 방법 C: OpenOCD 직접 사용
```bash
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg
# 다른 터미널:
arm-none-eabi-gdb build/ID_Solution.elf
(gdb) target extended-remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) monitor reset run
```

## STM32 VS Code Extension

STM32 VS Code Extension에서 프로젝트를 인식하려면:
- `c_cpp_properties.json`의 "STM32" configuration 사용
- `stm32f10x.h` 경로가 올바르게 설정되어 있어야 IntelliSense 작동

## 문제 해결

### "stm32f10x.h: No such file"
- Keil INC 경로 확인: `C:\Keil\ARM\INC\ST\STM32F10x`
- 또는 STM32F10x_StdPeriph_Lib에서 헤더 복사

### OpenOCD "Cannot find stlink.cfg"
- OpenOCD scripts 경로 확인 (OpenOCD 설치 경로/share/openocd/scripts)
- Cortex-Debug 설정에서 `openocdPath` 또는 `searchDir` 설정

### Ninja 없음
- `.vscode/tasks.json`에서 `-G Ninja`를 `-G "Unix Makefiles"`로 변경
- 또는 `-G "MinGW Makefiles"` (MinGW 사용 시)
