###############################################################
#		File: Makefile                                         #
#		ID: PS_lang_Makefile                                   #
#		Author: Alexandru Radovici                             #
#		Date: February 5, 2004                                 #
#		Modified: never                                        #
#		This file is copyrighted to Alexandru Radovici         #
#		and complies to the terms of PSLA v0.1                 #
#                                                             #
#		Comment: This is the makefile for "transalte.cpp" and  #
#               "compile_psl.cpp".                            #
###############################################################

compile_psl:compile_psl.cpp
	g++ $< -lz -o $@

translate:translate.cpp translate.h
	g++ -c translate.cpp -o ../$@.o
