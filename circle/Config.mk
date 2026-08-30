# Config.mk for Raspberry Pi Bare Metal Circle OS

# Toolchain
PREFIX = /home/bestenfalls/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-

# Architecture
AARCH = 32

# Raspberry Pi Model
RASPPI = 1

# Additional Definitions
# DEFINE += -DARM_ALLOW_MULTI_CORE  # Uncomment if multi-core support is needed
DEFINE += -DREALTIME            # Uncomment for real-time mode
# DEFINE += -DDEFAULT_KEYMAP=\\\"US\\\"  # Set your default keymap (e.g., US)
# DEFINE += -DNO_SDHOST           # Uncomment if building for QEMU without SD card
# DEFINE += -DNO_PHYSICAL_COUNTER # Uncomment for QEMU with 32-bit architecture

# C++ Standard
# STANDARD = -std=c++17  # Uncomment if compiling with C++17 standard


