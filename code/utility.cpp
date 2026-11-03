// NOTE(mark): must be imported below Client_TicTacToe.cpp


#define ONE 1
#define TEN 10
#define HUNDRED 100
#define THOUSAND 1000
#define TENTHOUSAND 10000
#define HUNDREDTHOUSAND 100000
#define MILLION 1000000
#define TENMILLION 10000000
#define HUNDREDMILLION 100000000
#define BILION 1000000000
#define TENBILION 10000000000
#define HUNDREDBILION 100000000000
#define TRILLION 1000000000000
#define TENTRILLION 10000000000000
#define HUNDREDTRILLION 100000000000000
#define QUADRILLION 1000000000000000
#define TENQUADRILLION 10000000000000000
#define HUNDREDQUADRILLION 100000000000000000
#define QUINTILLION 1000000000000000000



#if SLOW
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif


// 19 long good enough for now

uint64_t StrToInt64(char *data, int size)
{
    // NOTE(mark): this function only goes up to Ten Quintillion - 1
    uint64_t Result = 0;
    switch (size)
    {
        // NOTE(mark): this whole switch is a fallthrough
        case 19:
        {
            Result += QUINTILLION * atoi(&data[18]);
        }
        case 18:
        {
            Result += HUNDREDQUADRILLION * atoi(&data[17]);
        }
        case 17:
        {
            Result += TENQUADRILLION * atoi(&data[16]);
        }
        case 16:
        {
            Result += QUINTILLION * atoi(&data[15]);
        }
        case 15:
        {
            Result += HUNDREDTRILLION * atoi(&data[14]);
        }
        case 14:
        {
            Result += TENTRILLION * atoi(&data[13]);
        }
        case 13:
        {
            Result += TRILLION * atoi(&data[12]);
        }
        case 12:
        {
            Result += HUNDREDBILION * atoi(&data[11]);
        }
        case 11:
        {
            Result += TENBILION * atoi(&data[10]);
        }
        case 10:
        {
            Result += BILION * atoi(&data[9]);
        }
        case 9:
        {
            Result += HUNDREDMILLION * atoi(&data[8]);
        }
        case 8:
        {
            Result += TENMILLION * atoi(&data[7]);
        }
        case 7:
        {
            Result += MILLION * atoi(&data[6]);
        }
        case 6:
        {
            Result += HUNDREDTHOUSAND * atoi(&data[5]);
        }
        case 5:
        {
            Result += TENTHOUSAND * atoi(&data[4]);
        }
        case 4:
        {
            Result += THOUSAND * atoi(&data[3]);
        }
        case 3:
        {
            Result += HUNDRED * atoi(&data[2]);
        }
        case 2:
        {
            Result += TEN * atoi(&data[1]);
        }
        case 1:
        {
            Result += ONE * atoi(&data[0]);
        } break; // STOP The Fallthrough Here
        default:
        {
            Result = 0;
        }
    }
    return Result;
}
