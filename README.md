# Kybesp32 "Efficient Implementation of CRYSTALS-KYBER Key Encapsulation Mechanism on ESP32"
**Authors:** Fabian Segatz & Muhammad Ihsan Al Hafiz

This research project was carried out as part of the course [II2202, Research Methodology and Scientific Writing](https://www.kth.se/student/kurser/kurs/II2202?l=en), at KTH Stockholm to gain knowledge in conducting scientific research in an information and communications technology-related topic. We have decided on the topic ”Efficient Implementation of CRYSTALS-KYBER Key Encapsulation Mechanism on ESP32” as it is relevant but still open-ended enough so that meaningful results can be presented after 8 weeks and 20h work time per person per week. 
In the course of this project, both authors learned a lot about cryptography, the challenges that cryptography faces in the fast-advancing quantum computer technology and the process of implementing and optimizing algorithms on the ESP32 platform using FreeRTOS and the ESP-IDF. When progressing through the project, we were confronted with many open questions that we would like to investigate more in the future. 
Finally, we want to thank  [Assoc. Prof. Masoumeh (Azin) Ebrahimi](https://people.kth.se/~mebr/) for her amazing support, guidance
and tips, but particularly the incredible amount of time she took for the other groups and us.

We want to give credits to Bos et al. for their [official reference implementation](https://github.com/pq-crystals/kyber/blob/master/README.md) of the [Kyber](https://www.pq-crystals.org/kyber/) key encapsulation mechanism, which was the baseline for our implementation effort. All components are basically their code, and we just modified it to fit our purpose of creating an efficient implementation on ESP32.

## Build instructions

We can recommend using the [VSCode plugin for the ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/vscode-setup.html). It allows to easily setup the ESP-IDF framework, which allows to configure the project for any ESP32 developement board.

Alternatively one can follow the [official documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html#get-started-windows-first-steps) for setting up the ESP-IDF.

## Folder contents

Below is short overview of the files in the project folder.

```
├── components
│   ├── indcpa
│   ├── kem
│   ├── ...
├── main
│   ├── CMakeLists.txt
│   └── main.c
├── .gitignore
├── CMakeLists.txt
└── README.md           This is the file you are currently reading
```

The project **kybesp32** contains a source file in C language [main.c](main/main.c). The file is located in folder [main](main). This file contains the program entry point `app_main()`.

The components folder holds every primitive, that is required for the Kyber algorithm. We want to highlight the component `indcpa` which holds all the functionality for the Public Key Encryption (PKE) and the component `kem`, that defines the Key Encapsulation Mechanism (KEM).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

## Test results

We used the ESP-IDF development framework in version 5.0. The compilation is done by the framework’s default compiler, which is GCC in version 8.4.0. We did not activate any compiler optimization. The execution was performed on a [ESP32-S3-DevKitC-1 development board](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/hw-reference/esp32s3/user-guide-devkitc-1.html) with a clock frequency of 160 MHz. The compiler settings were kept default as they were specified when setting up the project with the ESP-IDF.

In the file `CMakeLists.txt` there are a number of preprocessor defines that can be used to activate either the dual-core optimizations or the SHA and ACC accelerators.

After building the firmware and flashing it to the device, the clock tick cylcle count measurments for the Kyber KEM keypair generation, encapsulation and decapsulation get reported via the serial interface of the dev board.

We measured 3 scenarios:

1. Scenario 1: Single-core
```
add_compile_definitions("KYBER_90S")
add_compile_definitions("KYBER_K=2")
add_compile_definitions("SHA_ACC=0")
add_compile_definitions("AES_ACC=0")
add_compile_definitions("INDCPA_KEYPAIR_DUAL=0")
add_compile_definitions("INDCPA_ENC_DUAL=0")
add_compile_definitions("INDCPA_DEC_DUAL=0")
```
2. Scenario 2: Dual-core
```
add_compile_definitions("KYBER_90S")
add_compile_definitions("KYBER_K=2")
add_compile_definitions("SHA_ACC=0")
add_compile_definitions("AES_ACC=0")
add_compile_definitions("INDCPA_KEYPAIR_DUAL=1")
add_compile_definitions("INDCPA_ENC_DUAL=1")
add_compile_definitions("INDCPA_DEC_DUAL=0")
```
3. Scenario 3: Dual-core and accelerator
```
add_compile_definitions("KYBER_90S")
add_compile_definitions("KYBER_K=2")
add_compile_definitions("SHA_ACC=1")
add_compile_definitions("AES_ACC=1")
add_compile_definitions("INDCPA_KEYPAIR_DUAL=1")
add_compile_definitions("INDCPA_ENC_DUAL=1")
add_compile_definitions("INDCPA_DEC_DUAL=0")
```

The following table shows our results for Kyber512 in the 90s variant:

| Implementation| Algorithm         | Cycle count   | Speedup ratio |
| -----------   | -----------       | -----------   | ----------- |
| Scenario 1    | Key Generation    | 2.439.083     | 1x |
|               | Encapsulation     | 2.736.256     | 1x |
|               | Decapsulation     | 2.736.256     | 1x |
| Scenario 2    | Key Generation    | 2.007.689     | 1.21x |
|               | Encapsulation     | 2.243.652     | 1.22x |
|               | Decapsulation     | 2.471.286     | 1.20x |
| Scenario 3    | Key Generation    | 1.414.389     | 1.72x |
|               | Encapsulation     | 1.490.784     | 1.84x |
|               | Decapsulation     | 1.756.638     | 1.69x |

