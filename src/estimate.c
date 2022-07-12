#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void multiply(double **temp, double **mA, double **mB, int rA, int cA, int rB, int cB)
{
    for (int j = 0; j < rA; j++)
    {
        for (int k = 0; k < cB; k++)
        {
            temp[j][k] = 0;
            for (int l = 0; l < cA; l++)
            {
                temp[j][k] += mA[j][l] * mB[l][k];
            }
        }
    }
}

void inverseALGO(double **inv, double **matrix, int n)
{
    double ratio = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 2 * n; j++)
        {
            inv[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                inv[i][j + n] = 1;
            }
            else
            {
                inv[i][j + n] = 0;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            inv[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                ratio = inv[j][i] / inv[i][i];
                for (int k = 0; k < 2 * n; k++)
                {
                    inv[j][k] = inv[j][k] - ratio * inv[i][k];
                }
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = n; j < 2 * n; j++)
        {
            inv[i][j] = inv[i][j] / inv[i][i];
        }
    }
}

void matrixFreer(double **matrix, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

int main(int argc, char **argv)
{
    FILE *fp = fopen(argv[1], "r");

    char *train = (char *)malloc(6 * sizeof(char));
    fscanf(fp, "%s", train);

    FILE *fpData = fopen(argv[2], "r");

    char *data = (char *)malloc(5 * sizeof(char));
    fscanf(fpData, "%s", data);

    if (train[0] != 't' && data[0] != 'd')
    {
        printf("error\n");
        exit(1);
    }

    int rowsT, columnsX;
    fscanf(fp, "%d", &columnsX);
    fscanf(fp, "%d", &rowsT);
    columnsX += 1;

    //--------------------------------------------------------------------------------making matrix X XT Y
    //alloc mX mXT mY
    double **mX = (double **)malloc(sizeof(double *) * rowsT);
    for (int i = 0; i < rowsT; i++)
    {
        mX[i] = (double *)malloc(sizeof(double) * (columnsX));
    }
    double **mXT = (double **)malloc(sizeof(double *) * columnsX);
    for (int i = 0; i < columnsX; i++)
    {
        mXT[i] = (double *)malloc(sizeof(double) * (rowsT));
    }
    double **mY = (double **)malloc(sizeof(double *) * rowsT);
    for (int i = 0; i < rowsT; i++)
    {
        mY[i] = (double *)malloc(sizeof(double));
    }

    //scanner to get values form train.txt with last col for mY

    for (int i = 0; i < rowsT; i++)
    {
        mX[i][0] = 1;
    }

    for (int i = 0; i < rowsT; i++)
    {
        for (int j = 1; j < columnsX; j++)
        {
            fscanf(fp, "%lf", &mX[i][j]);
        }
        fscanf(fp, "%lf", &mY[i][0]);
        //Y is col size 1 so it only needs the rows input
    }
    for (int i = 0; i < rowsT; i++)
    {
        for (int j = 0; j < columnsX; j++)
        {
            mXT[j][i] = mX[i][j];
        }
    }
    fclose(fp);

    //--------------------------------------------------------------------------------made matrix X XT Y

    double **mXTX = (double **)malloc(sizeof(double *) * columnsX);
    for (int i = 0; i < columnsX; i++)
    {
        mXTX[i] = (double *)malloc(sizeof(double) * (columnsX));
    }
    multiply(mXTX, mXT, mX, columnsX, rowsT, rowsT, columnsX);

    //--------------------------------------------------------------------------------mul X XT worked

    double **mXTY = (double **)malloc(sizeof(double *) * columnsX);
    for (int i = 0; i < columnsX; i++)
    {
        mXTY[i] = (double *)malloc(sizeof(double));
    }
    multiply(mXTY, mXT, mY, columnsX, rowsT, rowsT, 1);

    //--------------------------------------------------------------------------------mul XT Y worked

    double **mXTXinvtemp = (double **)malloc(sizeof(double *) * columnsX);
    for (int i = 0; i < columnsX; i++)
    {
        mXTXinvtemp[i] = (double *)malloc(sizeof(double) * (columnsX * 2));
    }

    inverseALGO(mXTXinvtemp, mXTX, columnsX);

    double **mXTXinvmain = (double **)malloc(sizeof(double *) * columnsX);
    for (int i = 0; i < columnsX; i++)
    {
        mXTXinvmain[i] = (double *)malloc(sizeof(double) * (columnsX));
    }
    for (int i = 0; i < columnsX; i++)
    {
        for (int j = 0; j < columnsX; j++)
        {
            mXTXinvmain[i][j] = mXTXinvtemp[i][j + columnsX];
        }
    }

    //--------------------------------------------------------------------------------inv XTX worked

    double **mW = (double **)malloc(sizeof(double *) * columnsX);
    for (int i = 0; i < columnsX; i++)
    {
        mW[i] = (double *)malloc(sizeof(double) * 1);
    }

    multiply(mW, mXTXinvmain, mXTY, columnsX, columnsX, columnsX, 1);

    //--------------------------------------------------------------------------------W worked assume

    int rowsData, columnsData;
    fscanf(fpData, "%d", &columnsData);
    fscanf(fpData, "%d", &rowsData);
    columnsData += 1;

    double **mData = (double **)malloc(sizeof(double *) * rowsData);
    for (int i = 0; i < rowsData; i++)
    {
        mData[i] = (double *)malloc(sizeof(double) * columnsData);
    }

    for (int i = 0; i < rowsData; i++)
    {
        mData[i][0] = 1;
    }

    for (int i = 0; i < rowsData; i++)
    {
        for (int j = 1; j < columnsData; j++)
        {
            fscanf(fpData, "%lf", &mData[i][j]);
        }
    }

    double **mFinalOut = (double **)malloc(sizeof(double *) * rowsData);
    for (int i = 0; i < rowsData; i++)
    {
        mFinalOut[i] = (double *)malloc(sizeof(double) * (1));
    }

    multiply(mFinalOut, mData, mW, rowsData, columnsData, columnsX, 1);

    for (int i = 0; i < rowsData; i++)
    {

        printf("%.0lf", mFinalOut[i][0]);

        printf("\n");
    }

    fclose(fpData);

    matrixFreer(mX, rowsT);
    matrixFreer(mXT, columnsX);
    matrixFreer(mY, rowsT);
    matrixFreer(mXTX, columnsX);
    matrixFreer(mXTY, columnsX);
    matrixFreer(mXTXinvtemp, columnsX);
    matrixFreer(mXTXinvmain, columnsData);
    matrixFreer(mW, columnsX);
    matrixFreer(mData, rowsData);
    matrixFreer(mFinalOut, rowsData);
    free(data);
    free(train);

    return 0;
}