#ifndef __MERTTURE_HW3_H
#define __MERTTURE_HW3_H


typedef struct ExprType {
        int ifStr;
        int ifNum;
        int ifFloat;
	int ifGet;
        int LineNo;
        int literal;
        double db;
	int inum;
        char *st;
} ExprType;


typedef struct OprType {
        char *literal;
        char *strResult;
        double dbResult;
        int intResult;
	int ifGet;
	int lineNo;
} OprType;


#endif
