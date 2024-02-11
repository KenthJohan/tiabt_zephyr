#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

target_sources(app PRIVATE src/main.c)

#FILE(GLOB app_sources src/*.c)
FILE(GLOB app_sources src/egadc.c src/MCP356X.c src/MCP45HVX1.c src/myuuids.c src/dpot.c src/mysrv.c src/bt.c src/mydefs.c)
target_sources(app PRIVATE ${app_sources})