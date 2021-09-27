/***************************************************
*		 Copyright (c) 2018 MINE 田宇
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of version 2 of the GNU General Public
* License as published by the Free Software Foundation.
*
***************************************************/

#ifndef __STRING_H__

#define __STRING_H__

/*
		From => To memory copy Num bytes
*/

void * memcpy(void *From,void * To,long Num);


/*
		FirstPart = SecondPart		=>	 0
		FirstPart > SecondPart		=>	 1
		FirstPart < SecondPart		=>	-1
*/

int memcmp(void * FirstPart,void * SecondPart,long Count);

/*
		set memory at Address with C ,number is Count
*/

void * memset(void * Address,unsigned char C,long Count);

/*
		string copy
*/

char * strcpy(char * Dest,char * Src);


/*
		string copy number bytes
*/

char * strncpy(char * Dest,char * Src,long Count);


/*
		string cat Dest + Src
*/

char * strcat(char * Dest,char * Src);


/*
		string compare FirstPart and SecondPart
		FirstPart = SecondPart =>  0
		FirstPart > SecondPart =>  1
		FirstPart < SecondPart => -1
*/

int strcmp(char * FirstPart,char * SecondPart);


/*
		string compare FirstPart and SecondPart with Count Bytes
		FirstPart = SecondPart =>  0
		FirstPart > SecondPart =>  1
		FirstPart < SecondPart => -1
*/

int strncmp(char * FirstPart,char * SecondPart,long Count);
/*

*/

int strlen(char * String);


#endif
