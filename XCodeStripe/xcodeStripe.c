//
//  xcodeStripe.c
//  XCodeStripe
//
//  Created by 李萍萍 on 2021/9/28.
//

#include "xcodeStripe.h"


/**
 取模操作
 */
int getMod(int m, int n) {
    if(m >= 0){
        return m % n;
    }else {
        return (m + n) % n;
    }
    
}

/**
 编码
 */
void encoding(uint8_t **codeArray, int p) {
    int mod__1, mod_1;
    for(int i = 0; i < p; i++) {
        for(int j = 0; j < p - 2; j++) {
            mod_1 = getMod(i+j+2, p);
            codeArray[p-2][i] ^= codeArray[j][mod_1];
            mod__1 = getMod(i-j-2, p);
            codeArray[p-1][i] ^= codeArray[j][mod__1];
        }
    }
}

/**
 整个条带编码
 */
void encodingOnStripe(uint8_t ***codeArray, int stripeSize, int p) {
    for(int i = 0; i < stripeSize; i++) {
        encoding(codeArray[i], p);
    }
}

/**
 将列置0
 某列号为-1时，不置0
 */
void setColError(uint8_t **codeArray, int p, int col1, int col2) {
    if(col1 >= 0) {
        for(int i = 0; i < p; i++) {
            codeArray[i][col1] = 0;
        }
    }
    if(col2 >= 0) {
        for(int i = 0; i < p; i++) {
            codeArray[i][col2] = 0;
        }
    }
}

/**
 根据条带置两列失效
 */
void setColEraseOnStripe(uint8_t ***codeArray, int stripeSize, int p, int *col) {
    for(int i = 0; i < stripeSize; i++) {
        setColError(codeArray[i], p, col[0], col[1]);
    }
}

/**
 两列修复
 从斜率为1，开始解码
 */
void decodingOnSlope_1(uint8_t **codeArray, int p, int col1, int col2,int x, int y, int flagOneCol) {
    
}

/**
 根据一个起始点开始解码
 */
void decodingByOnePoint(uint8_t **codeArray, int p, int col1, int col2, int x, int y, int flagOneCol) {
    int row, col;
    if(x == p-2 && col1 == flagOneCol) {
        col = flagOneCol;
        row = getMod(x - y + flagOneCol, p);
        codeArray[row][col] = codeArray[x][y];
        // vcol 记录n-2或n-1行变化的列值
        int vcol = y,krow;
        while(1) {
            // 斜率1
            for(int i = 0; i < p-2; i++ ) {
                krow = getMod(vcol+i+2, p);
                if(row != i && col != krow)
                codeArray[row][col] ^= codeArray[i][krow];
            }
            if(row == p-2 || row == p-1) {
                break;
            }
            //斜率-1
            vcol = getMod(col+row-(p-2), p);
            row = getMod(row+col-col2, p);
            col = col2;
            if(row == p-2)
                row = p-1;
            codeArray[row][col] = codeArray[p-1][vcol];
            for(int i = 0; i < p-2; i++) {
                krow = getMod(vcol-i-2, p);
                if(row != i && col != krow)
                codeArray[row][col] ^= codeArray[i][krow];
            }
            if(row == p-2 || row == p-1) {
                break;
            }
            //斜率1
            vcol = getMod(p-2-row+col, p);
            row = getMod(row+col1-col, p);
            col = col1;
            codeArray[row][col] = codeArray[p-2][vcol];
        }
    } else if(x == p-2 && col2 == flagOneCol) {
        col = flagOneCol;
        row = getMod(x-y+flagOneCol, p);
        codeArray[row][col] = codeArray[x][y];
        // vrow 记录n-2或n-1行变化的列值
        int vcol = y,krow;
        while(1) {
            //斜率1
            for(int i = 0; i < p-2; i++) {
                krow = getMod(vcol + i + 2, p);
                if(row != i && col != krow)
                codeArray[row][col] ^= codeArray[i][krow];
            }
            if(row == p-2 || row == p-1) {
                break;
            }
            //斜率-1
            vcol = getMod(row + col - (p - 2), p);
            row = getMod(row + col - col1, p);
            if(row == p-2)
                row = p-1;
            col = col1;
            codeArray[row][col] = codeArray[p-1][vcol];
            for(int i = 0; i < p - 2; i++) {
                krow = getMod(vcol - i - 2, p);
                if(row != i && col != krow)
                    codeArray[row][col] ^= codeArray[i][krow];
            }
            if(row == p-2 || row == p-1) {
                break;
            }
            //斜率1
            vcol = getMod(p-2 + col - row, p);
            row = getMod(row + col2 - col, p);
            col = col2;
            codeArray[row][col] = codeArray[p-2][vcol];
            
        }
    }else if(x == p-1 && col1 == flagOneCol){
        col = flagOneCol;
        int vcol = y, krow;
        row = getMod(p-2 + y - flagOneCol, p);
        codeArray[row][col] = codeArray[x][y];
        
        while(1) {
            for(int i = 0; i < p - 2; i++) {
                krow = getMod(vcol - i - 2, p);
                if(row != i && col != krow)
                codeArray[row][col] ^= codeArray[i][krow];
            }
            if(row == p-2 || row == p-1) {
                break;
            }
            
            //斜率1
            vcol = getMod(p-2-row+col, p);
            row = getMod(row+col2-col, p);
            col = col2;
            codeArray[row][col] = codeArray[p-2][vcol];
            for(int i = 0; i < p - 2; i++) {
                krow = getMod(vcol + i + 2, p);
                if(row != i && col != krow)
                    codeArray[row][col] ^= codeArray[i][krow];
            }
            if(row == p-2 || row == p-1) {
                break;
            }
            
            //斜率-1
            vcol = getMod(row + col -(p - 2), p);
            row = getMod(row + col - col1, p);
            if(row == p-2)
                row = p-1;
            col = col1;
            codeArray[row][col] = codeArray[p-1][vcol];
        }
        
    }else if(x == p-1 && col2 == flagOneCol) {
        col = flagOneCol;
        int vcol = y, krow;
        if(y!=col2) {
            row = getMod(p-2 + y -col, p);
        }else {
            row = p -1;
        }
        codeArray[row][col] = codeArray[p-1][vcol];
        
        while(1){
            for(int i = 0; i < p - 2; i++) {
                krow = getMod(vcol - i - 2, p);
                if(row != i && col != krow)
                    codeArray[row][col] ^= codeArray[i][krow];
            }
            if(row == p-2 || row == p-1) {
                break;
            }
            //斜率1
            vcol = getMod(p - 2 + col -row, p);
            row = getMod(row + col1 - col, p);
            col = col1;
            codeArray[row][col] = codeArray[p-2][vcol];
            for(int i = 0; i < p-2; i++) {
                krow = getMod(vcol + i + 2, p);
                if(row != i && col != krow)
                    codeArray[row][col] ^= codeArray[i][krow];
            }
            if(row == p-2 || row == p-1) {
                break;
            }
            
            //斜率-1
            vcol = getMod(row + col - (p - 2), p);
            row = getMod(row + col - col2, p);
            if(row == p-2)
                row = p-1;
            col = col2;
            codeArray[row][col] = codeArray[p-1][vcol];
            
        }
    }
}

/**
 从四个起点开始解码｜｜ 优化
 */
void decodingByFourPoint(uint8_t **codeArray, int p, int col1, int col2) {
    //临时列值
    int tempCol;
    if(col1 < col2) {
        decodingByOnePoint(codeArray, p, col1, col2, p-2, col2-1, col1);
        tempCol = getMod(col1-1, p);
        decodingByOnePoint(codeArray, p, col1, col2, p-2, tempCol, col2);
        tempCol = getMod(col2+1, p);
        decodingByOnePoint(codeArray, p, col1, col2, p-1, tempCol, col1);
        decodingByOnePoint(codeArray, p, col1, col2, p-1, col1 + 1, col2);
    }else {
        exit(1);
    }
       
}


/**
 根据条带解码
 */
void decodingOnStripe(uint8_t ***codeArray, int stripeSize, int p, int *col) {
    for(int i = 0; i < stripeSize; i++) {
        decodingByFourPoint(codeArray[i], p, col[0], col[1]);
    }
}


/**
 两列擦除解码
 */
void decodingTwoErase(uint8_t **codeArray, int p, int col1, int col2) {
    if(col2 - col1 > 1) {
        //起始点
        //斜率为1
        //与col1列相交，不与col2相交(n-2,j-1)
        
        
        
    }
}

/**
 判断是否为素数
 */
int getPrime(int number) {
    int sqrtNumber = (int)sqrt((double)number), i;
    for(i = 2; i <= sqrtNumber; i++) {
        if(number % i == 0)
            break;
    }
    if(i > sqrtNumber) {
        return 1;
    }else{
        return 0;
    }
}

/**
 是否修复成功
 */
int isCorrectSuccess(uint8_t **codeArray, uint8_t **correctArray, int p) {
    for(int i = 0; i < p; i++) {
        for(int j = 0; j < p; j++) {
            if(codeArray[i][j] != correctArray[i][j]) {
                
                //修复失败
                printf("[i][j] = [%d][%d]\n", i, j);
//                printf("%d\t %d\t\t", codeArray[i][j], correctArray[i][j] );
                return 0;
            }
        }
    }
    //修复成功
    return 1;
}

/**
 根据条带查找是否解码成功
 */
int isCorrectOnStripe(uint8_t ***codeArray, uint8_t ***correctArray, int stripeSize, int p) {
    int isRight;
    for(int i = 0; i < stripeSize; i++) {
        isRight = isCorrectSuccess(codeArray[i], correctArray[i] , p);
        if(isRight == 0) {
            printf("\t%d\n",i);
            return 0;
        }
    }
    return 1;
}

int main() {
    int p = 103, stripeSize = 1000;
    struct timeval encodeStart, encodeEnd, decodeStart, decodeEnd;
    //编码时间 | 解码时间
    long encodeTime, decodeTime;
    //错误列
    int erase[2] = {0, 2};
    //容量
    long long memory = sizeof(uint8_t) * 8 * (p - 2) * p * stripeSize;
    //是否解码成功
    int isSuccess;
    uint8_t ***array = (uint8_t ***) malloc(stripeSize * sizeof(uint8_t**));
    
    for(int i = 0; i < stripeSize; i++) {
        array[i] = (uint8_t **) malloc(p*sizeof(uint8_t *));
        for(int j = 0; j < p; j++) {
            array[i][j] = (uint8_t *) malloc(p * sizeof(uint8_t));
            // orignlArray[i][j] = (uint8_t *) malloc(sizeof(uint8_t));
        }
    }
    
    uint8_t ***orignlArray = (uint8_t ***) malloc(stripeSize * sizeof(uint8_t **));
  
    for(int i = 0; i < stripeSize; i++) {
        orignlArray[i] = (uint8_t **) malloc(p * sizeof(uint8_t *));
        for(int j = 0; j < p; j++) {
            orignlArray[i][j] = (uint8_t *) malloc(p * sizeof(uint8_t));
        }
    }
    
    //随机写入数据
    for(int i = 0; i < stripeSize; i++) {
        for(int j = 0; j < p; j++) {
            for(int k = 0; k < p; k++) {
                if(j == p - 2 || j == p - 1) {
                    array[i][j][k] = 0;
                }else {
                    array[i][j][k] = rand();
                    orignlArray[i][j][k] = array[i][j][k];
                }
            }
        }
    }
    
    //编码
    //编码开始时间
    gettimeofday(&encodeStart, NULL);
    //编码
    encodingOnStripe((uint8_t ***) array, stripeSize, p);
    //编码结束时间
    gettimeofday(&encodeEnd, NULL);
    
    for(int i = 0; i < stripeSize; i++) {
        for(int j = 0; j < p; j++) {
            orignlArray[i][p-2][j] = array[i][p-2][j];
            orignlArray[i][p-1][j] = array[i][p-1][j];
        }
    }
    
    //编码时间
    encodeTime = (encodeEnd.tv_sec - encodeStart.tv_sec) * 1000000 + (encodeEnd.tv_usec - encodeStart.tv_usec);
    
    //置两列失效
    setColEraseOnStripe((uint8_t ***) array, stripeSize, p, erase);
    //解码
    //解码开始时间
    gettimeofday(&decodeStart, NULL);
    //解码
    decodingOnStripe((uint8_t ***)array, stripeSize, p, erase);
    //解码结束时间
    gettimeofday(&decodeEnd, NULL);
    //解码时间
    decodeTime = (decodeEnd.tv_sec - decodeStart.tv_sec)*1000000 + (decodeEnd.tv_usec - decodeStart.tv_usec);
    //是否解码成功
    isSuccess = isCorrectOnStripe((uint8_t ***) array, (uint8_t ***) orignlArray, stripeSize, p);
    printf("p = %d, stripeNum = %d\n", p, stripeSize);
    printf("失效列: %d, %d\n", erase[0], erase[1]);
    printf("编码速度：%.2f Kb/us\n", (double)memory / encodeTime / 1024.0);
    printf("解码速度：%.2f Kb/us\n", (double)memory / decodeTime / 1024.0);
//    printf("存储容量：%lld\n", memory);
    if(isSuccess) {
        printf("解码成功！\n");
    }else {
        printf("解码失败！\n");
    }
    
    return 0;
}
